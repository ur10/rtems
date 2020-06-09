#include <stdint.h>
#include <memory_management.h>
typedef struct stack_attr_shared
{
  uint32_t  *stack_address;
  size_t     stack_size;
  uint32_t  *page_table_base;
  memory_flags   access_flags;
} stack_attr_shared;  

typedef struct stack_attr_prot
{
  uint32_t   *stack_address;
  size_t      stack_size;
  uint32_t   *page_table_base;
  memory_flags    access_flags;
  stack_attr_shared *shared_stack;    
} stack_attr_prot;  // These structs will be useful for context-switching and stack sharing

void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base);

// uint32_t *prot_stack_base(void);

void prot_stack_share(stack_attr_shared *shared, stack_attr_prot prot_stack);