#include <bsp/arm-cp15-start.h>
#include <rtems/score/memorymanagement.h>
#include <libcpu/arm-cp15.h>
#include <rtems.h>

static uint32_t translate_flags(uint32_t attr_flags)
{
  uint32_t flags;
  uint32_t memory_attribute;

 /*
  * Access permission
  */
  memory_attribute = ( attr_flags & READ_WRITE ) | ( attr_flags & READ_ONLY ) | 
                     ( attr_flags & NO_ACCESS );

  switch (memory_attribute)
  {
    case READ_WRITE: 
     flags = ARMV7_MMU_READ_WRITE;
    break;

    case READ_ONLY:
     flags = ARMV7_MMU_READ_ONLY;
    break;

    case NO_ACCESS:
    default:
     flags = 0;
    break;
  }

 /*
  * Check for memory-cache operation 
  */
  if( attr_flags & MEMORY_CACHED ) {
    flags |= ARM_MMU_SECT_TEX_0 | ARM_MMU_SECT_C | ARM_MMU_SECT_B;
  }

  return flags;
}
    
void _Memory_protection_Set_entries(uintptr_t begin, size_t size, uint32_t flags)
{
   
    uintptr_t end;
    rtems_interrupt_level irq_level;
    uint32_t access_flags;

    end = begin + size;
    access_flags = translate_flags(flags);
    
    /**
     *  The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
    */
    rtems_interrupt_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, access_flags); 
    rtems_interrupt_enable(irq_level);
}

void _Memory_protection_Unset_entries(uintptr_t begin, size_t size)
{
  uint32_t access_flags;
  uintptr_t end;
  rtems_interrupt_level irq_level;

  end = begin + size;
  access_flags = translate_flags(READ_ONLY);

   /**
     *  The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
    */
  rtems_interrupt_disable(irq_level);
  arm_cp15_set_translation_table_entries(begin, end, access_flags); 
  rtems_interrupt_enable(irq_level);
}