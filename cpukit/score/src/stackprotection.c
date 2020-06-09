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
#include <rtems/score/threadimpl.h>

void _Stack_protection_Context_restore(
Stack_Control *heir_stack
)
{
  void* stack_address;
  size_t stack_size;
  uint32_t memory_flags;
  uint32_t index;

  for(index = 0;index < heir_stack->shared_stacks_count; index++) {
    stack_address = heir_stack->shared_stacks[index].shared_stack_area;
    stack_size = heir_stack->shared_stacks[index].shared_stack_size;
    memory_flags = heir_stack->shared_stacks[index].access_flag;
    _Memory_protection_Set_entries( stack_address, stack_size, memory_flags );
  }
  
}

rtems_status_code _Stack_protection_Share_stack(
  Stack_Control *target_stack,
  Stack_Shared_attr sharing_attr
)
{
  rtems_status_code status;
  uint32_t index;

/**
 * Every time we share a stack with the target thread, we increment the count of
 * shared stacks by one. The shared stacks count should not exceed 
 * 'SHARED_STACK_NUMBER' but since we index from zero we check for the equality
 * case too.
 */
  if(target_stack->shared_stacks_count >= SHARED_STACK_NUMBER ){
    return RTEMS_TOO_MANY;
  } else {
    index = target_stack->shared_stacks_count;
    target_stack->shared_stacks[index].access_flag = sharing_attr.access_flag;
    target_stack->shared_stacks[index].shared_stack_area = 
      sharing_attr.shared_stack_area;
    target_stack->shared_stacks[index].shared_stack_size = 
      sharing_attr.shared_stack_size;
    target_stack->shared_stacks_count = index + 1;

    status = _Memory_protection_Set_entries(
      sharing_attr.shared_stack_area,
      sharing_attr.shared_stack_size,
      sharing_attr.access_flag
    );

    if(status == RTEMS_INVALID_ADDRESS) {
      return RTEMS_INVALID_ADDRESS;
    } else if(status == RTEMS_SUCCESSFUL) {
      return RTEMS_SUCCESSFUL;
    }
  }

}