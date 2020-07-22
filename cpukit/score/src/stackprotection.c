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

Chain_Control stack_name_control = CHAIN_INITIALIZER_EMPTY(stack_name_control);

#define STACK_ADDRESS_NAME(stack_address)   "/taskfs/"#stack_address

void _Stackprotection_Thread_initialize(Stackprotection_Stack *thread_stack, uintptr_t stack_address, size_t stack_size)
{

    if( thread_stack != NULL ) {
        thread_stack->Base.access_flags = READ_WRITE_CACHED;
        thread_stack->Base.size = stack_size;
        thread_stack->Base.stack_address = stack_address;
    }
}

void _Stackprotection_Context_switch(Stackprotection_Stack *executing_stack, Stackprotection_Stack *heir_stack)
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
    
       _Memory_protection_Unset_entries(stack_address, size);
    }
    
    _Stackprotection_Context_restore(heir_stack);
}

void _Stackprotection_Context_restore(Stackprotection_Stack *heir_stack)
{
    void *stack_address;
    size_t  size;
    Chain_Node *node;
    Memorymanagement_flags flags;
    Chain_Control *shared_node_control;
    Stackprotection_Shared_stack *shared_stack;

    if(heir_stack != NULL) {
        stack_address = heir_stack->Base.stack_address;
        size = heir_stack->Base.size;
        flags = heir_stack->Base.access_flags;
        _Memory_protection_Set_entries(stack_address, size, flags);
    }
}

void _Stackprotection_Set_address_to_name(uintptr_t stack_address, Stackprotection_Stack_name *name_block)
{
    if( stack_address != NULL ) {
        name_block->name = STACK_ADDRESS_NAME(stack_address);
        name_block->stack_address = stack_address;
    }
}

char *_Stackprotection_Get_address_to_name(uintptr_t stack_address, Stackprotection_Stack_name *name_block)
{
    
    if(stack_address != NULL) {
        if(name_block->stack_address == stack_address) {
            return name_block->name;
        }
    }
}