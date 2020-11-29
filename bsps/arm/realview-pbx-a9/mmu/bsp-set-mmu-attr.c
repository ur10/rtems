#include <bsp/arm-cp15-start.h>
#include <rtems/score/memoryprotection.h>
#include <rtems/score/threadimpl.h>
#include <libcpu/arm-cp15.h>

bool set_memory_flags(Thread_Control* thread, void* arg)
{
  uintptr_t begin;
  uintptr_t end;
  uint32_t flags;
  rtems_interrupt_level irq_level;
  Thread_Control *executing;

  executing = _Thread_Executing;

  if(thread !=  executing) {
 
    flags = *( uint32_t *)( arg );
    begin = thread->Start.Initial_stack.area;
    end = begin + thread->Start.Initial_stack.size; 

    rtems_interrupt_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, flags);
    rtems_interrupt_enable(irq_level);
  }
  
  return false;
}

/**
 * We also need to have a check for the memory region that is being set/unset.
 * 
*/

static uint32_t translate_flags(uint32_t attr_flags)
{
  uint32_t flags;
  uint32_t memory_attribute;

  /**
   * This needs to be fixed
  */

  switch (attr_flags)
  {
    case RTEMS_READ_WRITE:
     flags = ARMV7_MMU_READ_WRITE;
     break;

    case RTEMS_READ_ONLY:
     flags = ARMV7_MMU_READ_ONLY;
     break;

    case RTEMS_NO_ACCESS:
    default:
     flags = 0;
     break;
  }

 /*
  * Check for memory-cache operation
  */
  if( attr_flags & RTEMS_MEMORY_CACHED ) {
    flags |= ARM_MMU_SECT_TEX_0 | ARM_MMU_SECT_C | ARM_MMU_SECT_B;
  }

  return flags;
}
    
rtems_status_code _Memory_protection_Set_entries(uintptr_t begin, size_t size, uint32_t flags)
{
  uintptr_t end;
  rtems_interrupt_level irq_level;
  uint32_t access_flags;
  
  if(begin != NULL ) {
    end = begin + size;
    access_flags = translate_flags(flags);
    
    /*
     * The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
     */
    rtems_interrupt_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, access_flags);
    rtems_interrupt_enable(irq_level);
  }
}

rtems_status_code _Memory_protection_Unset_entries(uintptr_t begin, size_t size)
{
  uint32_t access_flags;
  uintptr_t end;
  rtems_interrupt_level irq_level;
  
  if( begin != NULL ) {
    end = begin + size;
    access_flags = translate_flags( RTEMS_NO_ACCESS );

    /*
     *  The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
     */
    rtems_interrupt_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, access_flags);
    rtems_interrupt_enable(irq_level);
  }
}

rtems_status_code _Memory_protection_Enable( void )
{
  uint32_t access_flags;

  access_flags = translate_flags(  RTEMS_NO_ACCESS );

  _Thread_Iterate( set_memory_flags, &access_flags );

  return RTEMS_SUCCESSFUL; // check the return values for iterating function and current method.
}

rtems_status_code _Memory_protection_Disable( void )
{
  uint32_t access_flags;

  access_flags = translate_flags(  RTEMS_READ_WRITE );

  _Thread_Iterate( set_memory_flags, &access_flags );

  return RTEMS_SUCCESSFUL;
}