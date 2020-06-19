#include <bsp/arm-cp15-start.h>
#include <bsp/arm-cp15-set-ttb-entries.h>
#include <rtems/score/memorymanagement.h>
#include <libcpu/arm-cp15.h>
#include <rtems.h>

#ifdef THREAD_STACK_PROTECTION
  #define ARM_MMU_USE_SMALL_PAGES
#endif
    
void memory_entries_set(uint32_t *begin, size_t size, memory_flags flags)
{
   
    uint32_t *end;
    rtems_interrupt_level irq_level;
    uint32_t access_flags;

    end = begin + size;
    access_flags = memory_translate_flags(flags);
    
    rtems_interrupt_local_disable(irq_level);
    arm_cp15_set_translation_table_entries(begin, end, access_flags); 
    rtems_interrupt_local_enable(irq_level);
}

void memory_entries_unset(uint32_t *begin, size_t size)
{
  uint32_t access_flags;
  uint32_t *end;
  rtems_interrupt_level irq_level;

  end = begin + size;
  access_flags = memory_translate_flags(NO_ACCESS);

  rtems_interrupt_local_disable(irq_level);
  arm_cp15_set_translation_table_entries(begin, end, access_flags); 
  rtems_interrupt_local_enable(irq_level);
}


uint32_t memory_translate_flags(memory_flags attr_flags)
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
    break;
  }
}