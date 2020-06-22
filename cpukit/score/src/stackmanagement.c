#include <rtems/score/stackmanagement.h>
#include <rtems/score/chainimpl.h>
#include <rtems/score/basedefs.h>

Chain_Control prot_node_control = CHAIN_INITIALIZER_EMPTY(prot_node_control);

Chain_Control *shared_node_control;

static void shared_stack_entry_remove(stack_attr_shared *shared_stack)
{
    Chain_Node    *node;
    Chain_Control *control;

    control = &shared_stack->shared_node_control;

    if(control != NULL && _Chain_Is_empty( control ) == false ) { // Check if there are shared stacks
        node = _Chain_Head(control);

        while ( _Chain_Is_tail(control, node) == false ) {
            shared_stack = (stack_attr_shared*) node;
            memory_entries_unset(shared_stack->Base.stack_address, shared_stack->Base.size);
            node = node->next;
        }
    }
}

static void prot_stack_prev_entry_remove(void)
{

 Chain_Node *node;
 stack_attr_prot *stack_attr;

  if( _Chain_Is_empty(&prot_node_control) == true ) {
      _Chain_Initialize_empty(&prot_node_control);
  }
     node = _Chain_First( &prot_node_control );
 
     while(_Chain_Is_tail(&prot_node_control, node) == false) {

        stack_attr = RTEMS_CONTAINER_OF(node,stack_attr_prot, Base.node); 
        
        if( stack_attr->current_stack == false && _Chain_Is_head(&prot_node_control, node) == false ) {
            memory_entries_unset(stack_attr->Base.stack_address, stack_attr->Base.size);
         //   shared_stack_entry_remove(stack_attr->shared_stacks);
            
        }
        node =  _Chain_Immutable_next( node );
     }
   
   return ;
}

static void prot_stack_chain_append (Chain_Control *control, stack_attr_prot *stack_append_attr)  //maybe we don't need the chain control parameter to be passed 
{
    Chain_Node *node;
    stack_attr_prot *present_stacks_attr;

    if(_Chain_Is_empty(&prot_node_control) == true ) {

    _Chain_Initialize_one(&prot_node_control, &stack_append_attr->Base.node);
    } else {
        node = _Chain_First(&prot_node_control);

        /*
        This is done to ensure that we mark all the remaining
        entries as not-current so that they can be removed.
        */
        while(_Chain_Is_tail(&prot_node_control,node) == false) {
            
            present_stacks_attr = RTEMS_CONTAINER_OF(node, stack_attr_prot, Base.node);
            present_stacks_attr->current_stack = false;  
            node = _Chain_Immutable_next( node );
        }
        _Chain_Append_unprotected(&prot_node_control, &stack_append_attr->Base.node);
    }
    return ;
}

void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base)
{
    stack_attr_prot *stack_attr;
    
    // Have a condition for the case when the same stack is allocated twice, do not allocate a new node, will cause memory leaks.

    stack_attr = malloc(sizeof(stack_attr_prot));
    
    if(stack_attr != NULL)    {
    stack_attr->Base.stack_address = stack_address;
    stack_attr->Base.size = size;
    stack_attr->Base.page_table_base = page_table_base;
    stack_attr->Base.access_flags = READ_WRITE_CACHED;
    stack_attr->current_stack = true;
    }
    prot_stack_chain_append(&prot_node_control, stack_attr ); // Add the stack attr. at the end of the chain
    prot_stack_prev_entry_remove();           // Remove the previous stack entry

    memory_entries_set(stack_address, size, READ_WRITE_CACHED);

    return;
    
}

stack_attr_prot *prot_stack_context_initialize(void)
{
    Chain_Node *node;
    stack_attr_prot *stack_attr;

    if(   _Chain_Is_empty(&prot_node_control) == false ) {
        node = _Chain_First( &prot_node_control );

        while( _Chain_Is_tail(&prot_node_control, node ) == false) {
            stack_attr = RTEMS_CONTAINER_OF( node, stack_attr_prot, Base.node);

            if(stack_attr->current_stack == true) {
                return stack_attr;
            } else {
                node = _Chain_Immutable_next( node );
            }
        }
    }

    return stack_attr;
}

void prot_stack_context_switch(stack_attr_prot *stack_attr)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Chain_Control *shared_node_control;

     /*
      Remove the stacks shared with the current stack by iterating the chain
     */
    if( stack_attr != NULL) {

    stack_address = stack_attr->Base.stack_address;
    size = stack_attr->Base.size;

        if(stack_attr->current_stack == true) {
        memory_entries_unset(stack_address, size);
        stack_attr->current_stack = false;
        }

    shared_node_control = &stack_attr->shared_stacks->shared_node_control;
    }
/*

  The shared node control structure will be initialized during stack sharing

    if( shared_node_control != NULL && _Chain_Is_empty( shared_node_control ) == false) {
        node = _Chain_Head(shared_node_control);

        while(!_Chain_Is_tail( shared_node_control, node )) {
            stack_attr->shared_stacks = (stack_attr_shared*) node;

             stack_address = stack_attr->shared_stacks->Base.stack_address;
             size = stack_attr->shared_stacks->Base.size;
             memory_entries_unset( stack_address, size );

             node = node->next;
        }
    }
*/
}

void prot_stack_context_restore(stack_attr_prot *stack_attr)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Chain_Control *shared_node_control;
    memory_flags flags;
     /*
      Remove the stacks shared with the current stack by iterating the chain
     */
    if(stack_attr != NULL){
        
      /*  stack_attr->current_stack = true;
        stack_address = stack_attr->Base.stack_address;
        size = stack_attr->Base.size;
        */
        if(stack_attr->current_stack == true) {
             memory_entries_set(stack_address, size, READ_WRITE_CACHED);
        }
        return ;

  //      shared_node_control = &stack_attr->shared_stacks->shared_node_control;
    }
/*  The shared node control structure will be initialized during stack sharing

    if( shared_node_control !=NULL && _Chain_Is_empty( shared_node_control ) == false ) {
        node = _Chain_Head( shared_node_control );

        while(!_Chain_Is_tail( shared_node_control, node )) {
            stack_attr->shared_stacks = (stack_attr_shared*) node;

             stack_address = stack_attr->shared_stacks->Base.stack_address;
             size = stack_attr->shared_stacks->Base.size;
             flags = stack_attr->shared_stacks->Base.access_flags;
             memory_entries_set( stack_address, size, flags );

             node = node->next;
        }
    }
  // Possible bug
  */  
}

/*
What needs to be done - 

1. Stack is allocated, its attribute structure is appended to the list
2. Stack entry of the previously set stack, including the shared stack need to be removed.
3. Stack entry of the current stack need to be set.

*/