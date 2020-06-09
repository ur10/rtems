#include <bsp/arm-cp15-start.h>
#include <rtems/score/memorymanagement.h>
#include <libcpu/arm-cp15.h>
#include <rtems.h>

#ifdef USE_THREAD_STACK_PROTECTION
  #define ARM_MMU_USE_SMALL_PAGES
#endif
    
void _Memorymanagement_Set_entries(uintptr_t begin, size_t size, Memorymanagement_flags flags)
{
   
    uintptr_t end;
    rtems_interrupt_level irq_level;
    uint32_t access_flags;

    end = begin + size;
    access_flags = _Memorymanagement_Translate_flags(flags);
    
    /**
     *  The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
    */
    rtems_interrupt_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, access_flags); 
    rtems_interrupt_enable(irq_level);
}

void _Memorymanagement_Unset_entries(uintptr_t begin, size_t size)
{
  uint32_t access_flags;
  uintptr_t end;
  rtems_interrupt_level irq_level;

  end = begin + size;
  access_flags = _Memorymanagement_Translate_flags(NO_ACCESS);

   /**
     *  The ARM reference manual instructs to disable all the interrupts before
     * setting up page table entries.
    */
  rtems_interrupt_disable(irq_level);
  arm_cp15_set_translation_table_entries(begin, end, access_flags); 
  rtems_interrupt_enable(irq_level);
}


uint32_t Memorymanagement_Translate_flags(Memorymanagement_flags attr_flags)
{
  uint32_t flags;
  switch (attr_flags)
  {
  case READ_WRITE: 
     flags = ARMV7_MMU_READ_WRITE;
  break;

  case READ_WRITE_CACHED:
    flags = ARMV7_MMU_DATA_READ_WRITE_CACHED;
  break;

  case READ_ONLY:
    flags = ARMV7_MMU_READ_ONLY;
  break;

  case READ_ONLY_CACHED:
    flags = ARMV7_MMU_READ_ONLY_CACHED;
  break;

  case NO_ACCESS:
    flags = 0;
  break;
  
  default:
     return 0;
  break;
  }

  return flags;
}