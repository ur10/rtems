/*This file needs to be moved to score and refactored*/

#include <stdint.h>
#include <stdbool.h>
#include <memory_management.h>
#include <rtems/score/chainimpl.h>
#include <rtems.h>

/*
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
*/

typedef struct stack_attr
{
  Chain_Node    node; 
  void*         stack_address;
  size_t        size;
  uint32_t      page_table_base;
  memory_flags  access_flags;
}stack_attr;

typedef struct stack_attr_shared
{
  stack_attr    Base;
  Chain_Control shared_node_control;
}stack_attr_shared;

typedef struct stack_attr_prot
{
  stack_attr_shared  *shared_stacks;
  stack_attr    Base; 
  bool          current_stack;  
} stack_attr_prot;


void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base);

// uint32_t *prot_stack_base(void);

void prot_stack_share(stack_attr_shared *shared, stack_attr_prot prot_stack);

stack_attr_prot *prot_stack_context_initialize(void);

void prot_stack_context_switch(stack_attr_prot *stack_attr);

void prot_stack_context_restore(stack_attr_prot *stack_attr);


/*             Shared_Stack Shared_Stack
                  |          |    
protstack_base->node->node->node->node  
   |                        |
Shared_stack            Shared_Stack
*/