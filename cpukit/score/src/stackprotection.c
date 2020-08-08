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
#include <rtems/score/thread.h>

void _Stackprotection_Context_restore(
Stack_Control *heir_stack
)
{
  void* stack_address;
  size_t stack_size;
  uint32_t memory_flags;
  uint32_t index;

  for(index = 0;index < heir_stack->shared_stacks_count; index++) {
    stack_address = heir_stack->shared_stacks[index]->shared_stack_area;
    stack_size = heir_stack->shared_stacks[index]->shared_stack_size;
    memory_flags = heir_stack->shared_stacks[index]->Base.access_flags;
    _Memory_protection_Set_entries( stack_address, stack_size, memory_flags );
  }
  
}

static bool get_target_thread( Thread_Control *Control, void *arg)
{
  Stack_Shared_attr *shared_stack;
  uint32_t count;
  shared_stack = arg;
  /**
   * Check for the target thread by comparing the stack address. Add the shared stack
   * attribute structure to the array tracking all the shared stacks.
   */
  if( Control->Start.Initial_stack.area == shared_stack->target_stack_area) {
     count =  Control->Start.Initial_stack.shared_stacks_count + 1;
     if(count >= SHARED_STACK_NUMBER) {
       return false;
     }
     Control->Start.Initial_stack.shared_stacks_count = count;
     Control->Start.Initial_stack.shared_stacks[count] = shared_stack;
     shared_stack->stack_shared = true;

     return true;
  }  
}

int _Stackprotection_Share_stack(
  void* target_stack,
  void* sharing_stack,
  size_t size,
  uint32_t memory_flag
)
{
 Thread_Control *target_thread;
 Stack_Shared_attr shared_stack; 
 
 shared_stack.Base.access_flags= memory_flag;
 shared_stack.shared_stack_area = sharing_stack;
 shared_stack.target_stack_area = target_stack;
 shared_stack.shared_stack_size = size;
 
 //_Thread_Iterate( get_target_thread, &shared_stack );
 if( shared_stack.stack_shared == true ) {
   return 0;
 } else {
   return -1;
 }
}