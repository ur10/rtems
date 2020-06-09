/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreStackprotection
 *
 */

/*
 * Copyright (C) 2020 Utkarsh Rai
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/stackprotection.h>

Chain_Control _Stackprotection_node_control = CHAIN_INITIALIZER_EMPTY(_Stackprotection_node_control);

static void _Stackprotection_Remove_prev_entry(void)
{

 Chain_Node *node;
 Stackprotection_The_stack *prot_stack;

  if( _Chain_Is_empty(&_Stackprotection_node_control) == true ) {
      _Chain_Initialize_empty(&_Stackprotection_node_control);
  }
     node = _Chain_First( &_Stackprotection_node_control );
 
     while(_Chain_Is_tail(&_Stackprotection_node_control, node) == false) {

        prot_stack = RTEMS_CONTAINER_OF(node,Stackprotection_The_stack, Base.node); 
        
        if( prot_stack->current_stack == false ) {
            Memorymanagement_Unset_entries(prot_stack->Base.stack_address, prot_stack->Base.size);
        }
        node =  _Chain_Immutable_next( node );
     }

}

/*
Iterate to the end of the chain and mark all the 'currnet' stack as false
Append the current stack attribute to the end of the chain
*/
static void _Stackprotection_Append_chain (Chain_Control *control, Stackprotection_The_stack *stack_append_attr)
{
    Chain_Node *node;
    Stackprotection_The_stack *present_stacks_attr;

    if(_Chain_Is_empty(&_Stackprotection_node_control) == true ) {

    _Chain_Initialize_one(&_Stackprotection_node_control, &stack_append_attr->Base.node);
    } else {
        _Chain_Append_unprotected(&_Stackprotection_node_control, &stack_append_attr->Base.node);
    }
}

void _Stackprotection_Thread_initialize(Stackprotection_The_stack *thread_stack, uintptr_t stack_address, size_t stack_size)
{

    if( thread_stack != NULL ) {
        thread_stack->Base.access_flags = READ_WRITE_CACHED;
        thread_stack->Base.size = stack_size;
        thread_stack->Base.stack_address = stack_address;

        /** We mark the stack as 'current' and handle the 'actual' state during
         * context restoration.
        */
        thread_stack->current_stack = true;
    }

    if( _Chain_Is_empty( &_Stackprotection_node_control ) ==true )  {
        _Chain_Initialize_one( &_Stackprotection_node_control, &thread_stack->Base.node );
    } else {
        _Chain_Append_unprotected( &_Stackprotection_node_control, &thread_stack->Base.node );
    }
}

void _Stackprotection_Context_switch(Stackprotection_The_stack *executing_stack, Stackprotection_The_stack *heir_stack)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Chain_Control *shared_node_control;
    Stackprotection_Shared_stack *shared_stack;

    shared_node_control = &executing_stack->shared_node_control;

    if( executing_stack != NULL) {

    stack_address = executing_stack->Base.stack_address;
    size = executing_stack->Base.size;

        if(executing_stack->current_stack == true) {
            executing_stack->current_stack = false;
            Memorymanagement_Unset_entries(stack_address, size);
            /**
             * Take care of the shared stacks(if any).
             */
            if( _Chain_Is_empty( shared_node_control ) == false) {
                node = _Chain_First( shared_node_control );
    
                while (_Chain_Is_tail( shared_node_control, node ) == false) {
                    shared_stack = RTEMS_CONTAINER_OF( node, Stackprotection_Shared_stack, Base.node);
                    Memorymanagement_Unset_entries( shared_stack->Base.stack_address, shared_stack->Base.size );
                }
            }
        }
    }
    
    _Stackprotection_Context_restore(heir_stack);
}

void _Stackprotection_Context_restore(Stackprotection_The_stack *heir_stack)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Memorymanagement_flags flags;
    Chain_Control *shared_node_control;
    Stackprotection_The_stack *present_stacks_attr;
    Stackprotection_Shared_stack *shared_stack;

    if(heir_stack != NULL) {
             heir_stack->current_stack = true;
             stack_address = heir_stack->Base.stack_address;
             size = heir_stack->Base.size;
             flags = heir_stack->Base.access_flags;
             Memorymanagement_Set_entries(stack_address, size, flags);
             /**
              * Take care of the shared stacks (if any).
            `*/
            if( _Chain_Is_empty( shared_node_control ) == false) {
                node = _Chain_First( shared_node_control );
    
                while (_Chain_Is_tail( shared_node_control, node ) == false) {
                    shared_stack = RTEMS_CONTAINER_OF( node, Stackprotection_Shared_stack, Base.node);
                    flags = shared_stack->Base.access_flags;
                    stack_address = shared_stack->Base.stack_address;
                    size = shared_stack->Base.size;
                    Memorymanagement_Set_entries( stack_address, size, flags );
                }
            }
    }

      node = _Chain_First(&_Stackprotection_node_control);
    /** Iterate through the chain and unset memory entries of all the
     *  'not-current' thread-stacks 
    */
    while(_Chain_Is_tail(&_Stackprotection_node_control,node) == false) {
            
            present_stacks_attr = RTEMS_CONTAINER_OF(node, Stackprotection_The_stack, Base.node);

            if(present_stacks_attr->current_stack == true && present_stacks_attr != heir_stack) 
            present_stacks_attr->current_stack = false;  
            Memorymanagement_Unset_entries(present_stacks_attr->Base.stack_address, present_stacks_attr->Base.size);
            node = _Chain_Immutable_next( node );   
    }
   
}