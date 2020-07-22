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



void _Stackprotection_Thread_initialize(Stackprotection_Stack *thread_stack, uintptr_t stack_address, size_t stack_size)
{

    if( thread_stack != NULL ) {
        thread_stack->Base.access_flags = ( READ_WRITE |  MEMORY_CACHED );
        thread_stack->Base.size = stack_size;
        thread_stack->Base.stack_address = stack_address;
        /*
         * TO-DO - Integrate stack naming here.  
         */
    }
}

void _Stackprotection_Context_switch(Stackprotection_Stack *executing_stack, Stackprotection_Stack *heir_stack)
{
   
    Chain_Node *node;
    Chain_Control *shared_node_control;
    Stackprotection_Shared_stack *shared_stack;

    shared_node_control = &executing_stack->shared_stack_control;

    if( shared_node_control != NULL) {
        node = _Chain_Get_first_unprotected( shared_node_control );
        while( _Chain_Is_tail(shared_node_control, node) == false) {
            shared_stack = RTEMS_CONTAINER_OF( node, Stackprotection_Shared_stack, node);
            _Memory_protection_Unset_entries( shared_stack->Base.stack_address, shared_stack->Base.size );
        }
    }
    
    _Stackprotection_Context_restore( heir_stack );
}

void _Stackprotection_Context_restore(Stackprotection_Stack *heir_stack)
{
    Chain_Node *node;
    uint32_t flags;
    Chain_Control *shared_node_control;
    Stackprotection_Shared_stack *shared_stack;

    shared_node_control = &heir_stack->shared_stack_control;

    if( shared_node_control != NULL) {
        node = _Chain_Get_first_unprotected( shared_node_control );
        while( _Chain_Is_tail(shared_node_control, node) == false) {
            shared_stack = RTEMS_CONTAINER_OF( node, Stackprotection_Shared_stack, node);
            flags = shared_stack->Base.access_flags;
            _Memory_protection_Set_entries( shared_stack->Base.stack_address, shared_stack->Base.size, flags );
        }
    }
}

void _Stackprotection_Share_stack(Stackprotection_Stack *shared_stack, Stackprotection_Stack *target_stack, size_t shared_stack_size, uint32_t flag)
{
    
    Chain_Control *control;
    void* shared_address;

    control = &target_stack->shared_stack_control;
    shared_address = shared_stack->Base.stack_address;

    shared_stack->shared_stacks->Base.access_flags = flag;
    shared_stack->shared_stacks->Base.size = shared_stack_size;
    shared_stack->shared_stacks->Base.stack_address = shared_address;

    if( control != NULL) {
        _Chain_Append_unprotected( control, &shared_stack->shared_stacks->node);
        _Memory_protection_Set_entries(shared_address, shared_stack_size, flag);
    }
}