#include <stack_management.h>

void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base)
{
    stack_attr_prot stack_attr;

    stack_attr.stack_address = stack_address;  // These attribute will be added to a chain 
    stack_attr.stack_size = size;             // to keep a track of all allocated stacks
    stack_attr.access_flags = READ_WRITE;     // which wil help in swapping and isolation.
    
    memory_entries_set(stack_address, size, READ_WRITE);
}