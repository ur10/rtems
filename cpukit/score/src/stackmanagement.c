#include <rtems/score/stackmanagement.h>
#include <rtems/score/chainimpl.h>
#include <rtems/score/basedefs.h>

Chain_Control prot_node_control = CHAIN_INITIALIZER_EMPTY(prot_node_control);

Chain_Control *shared_node_control;
#if 0
static void shared_stack_entry_remove(stack_attr_shared *shared_stack)
{
    Chain_Node    *node;
    Chain_Control *control;

    control = &shared_stack->shared_node_control;

    if(control != NULL && _Chain_Is_empty( control ) == false ) { 
        node = _Chain_Head(control);

        while ( _Chain_Is_tail(control, node) == false ) {
            shared_stack = (stack_attr_shared*) node;
            memory_entries_unset(shared_stack->Base.stack_address, shared_stack->Base.size);
            node = node->next;
        }
    }
}
#endif
/*
Iterate through the chain and remove the memory entries of all the
'not-current stack'
*/
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
        
        if( stack_attr->current_stack == false ) {
            memory_entries_unset(stack_attr->Base.stack_address, stack_attr->Base.size);
         //   shared_stack_entry_remove(stack_attr->shared_stacks);

        }
        node =  _Chain_Immutable_next( node );
     }

}

/*
Iterate to the end of the chain and mark all the 'currnet' stack as false
Append the current stack attribute to the end of the chain
*/
static void prot_stack_chain_append (Chain_Control *control, stack_attr_prot *stack_append_attr)
{
    Chain_Node *node;
    stack_attr_prot *present_stacks_attr;

    if(_Chain_Is_empty(&prot_node_control) == true ) {

    _Chain_Initialize_one(&prot_node_control, &stack_append_attr->Base.node);
    } else {
        node = _Chain_First(&prot_node_control);

        while(_Chain_Is_tail(&prot_node_control,node) == false) {
            
            present_stacks_attr = RTEMS_CONTAINER_OF(node, stack_attr_prot, Base.node);
            present_stacks_attr->current_stack = false;  
            node = _Chain_Immutable_next( node );
        }
        _Chain_Append_unprotected(&prot_node_control, &stack_append_attr->Base.node);
    }

}

void prot_stack_allocate(uint32_t *stack_address, size_t size, uint32_t *page_table_base)
{
    stack_attr_prot *stack_attr;
    
/*This field will be refactored and score objects will be used for dynamic allocation*/
    stack_attr = malloc(sizeof(stack_attr_prot));

    if(stack_attr != NULL) {
    stack_attr->Base.stack_address = stack_address;
    stack_attr->Base.size = size;
    stack_attr->Base.page_table_base = page_table_base;
    stack_attr->Base.access_flags = READ_WRITE_CACHED;
    stack_attr->current_stack = true;
    }

    /*
    Add the attribute field to the end of the chain, remove the memory entries of
    previously allocated stack and set the memory entry of the currnet stack.
    */
    prot_stack_chain_append(&prot_node_control, stack_attr );
    prot_stack_prev_entry_remove();
    memory_entries_set(stack_address, size, READ_WRITE_CACHED);
    
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
#if 0


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
#endif
}

void prot_stack_context_restore(stack_attr_prot *stack_attr)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Chain_Control *shared_node_control;
    memory_flags flags;

     if(stack_attr->current_stack == true) {
             memory_entries_set(stack_address, size, READ_WRITE_CACHED);
        }
     /*
      Remove the stacks shared with the current stack by iterating the chain
      */
 #if 0
    if(stack_attr != NULL){
       #if 0 
      /*  stack_attr->current_stack = true;
        stack_address = stack_attr->Base.stack_address;
        size = stack_attr->Base.size;
        */
       #endif
        if(stack_attr->current_stack == true) {
             memory_entries_set(stack_address, size, READ_WRITE_CACHED);
        }
        return ;

    }

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
 #endif
}