#include <stack_management.h>
#include <rtems/score/chainimpl.h>

Chain_Control node_control;

 Chain_Control shared_node_control;

static void shared_stack_entry_remove(stack_attr_shared *shared_stack)
{
    Chain_Node    *node;
    Chain_Control *control;

    control = &shared_stack->shared_node_control;

    if(!_Chain_Is_empty(control)) { // Check if there are shared stacks
        node = _Chain_Head(control);

        while (!_Chain_Is_tail(control, node)) {
        memory_entries_unset(shared_stack->Base.stack_address, shared_stack->Base.size);
          node = node->next;
        }
            
    }
}

static void prot_stack_prev_entry_remove(stack_attr_prot *stack_attr)
{
 
 Chain_Node *node;

 if (!_Chain_Is_empty(&node_control) == true) {
     node = _Chain_Head(&node_control);
     
     while(!_Chain_Is_tail(&node_control, node)) {
        if(!stack_attr->current_stack) {
            memory_entries_unset(stack_attr->Base.stack_address, stack_attr->Base.size);
            shared_stack_entry_remove(&stack_attr->shared_stacks);
        }
     }
 }
 
}

static void prot_stack_chain_append (Chain_Control *control, stack_attr_prot *stack_attr)  //maybe we don't need the chain control parameter to be passed 
{
    Chain_Node *node;

    if( _Chain_Is_empty(control) ) {
    _Chain_Initialize_one(control, &stack_attr->Base.node);
    } else {
        node = _Chain_Head(control);
        /*
        This is done to ensure that we mark all the remaining
        entries as not-current so that they can be removed.
        */
        while(!_Chain_Is_tail(control,node)) {    
            stack_attr->current_stack = false;  
            node = node->next;
        }
        _Chain_Append_unprotected(control, &stack_attr->Base.node);
    }
    
}

void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base)
{
    stack_attr_prot *stack_attr;
    
    stack_attr = malloc(sizeof(stack_attr_prot));

    stack_attr->Base.stack_address = stack_address;
    stack_attr->Base.size = size;
    stack_attr->Base.page_table_base = page_table_base;
    stack_attr->Base.access_flags = READ_WRITE_CACHED;
    stack_attr->current_stack = true;

    prot_stack_chain_append(&node_control, stack_attr); // Add the stack attr. at the end of the chain
    prot_stack_prev_entry_remove(stack_attr);           // Remove the previous stack entry

    memory_entries_set(stack_address, size, READ_WRITE);
    
}

/*
What needs to be done - 

1. Stack is allocated, its attribute structure is appended to the list
2. Stack entry of the previously set stack, including the shared stack need to be removed.
3. Stack entry of the current stack need to be set.

*/