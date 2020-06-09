#include<bsp/arm-cp15-start.h>

uint32_t arm_cp15_set_translation_table_entries(
  const void *begin,
  const void *end,
  uint32_t section_flags
);