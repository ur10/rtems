/**
 * @file
 *
 * @ingroup RTEMSScoreStackprotection
 *
 * @brief Stackprotection API
 *
 * This include file provides the API for the management of protected thread-
 * stacks
 */

/*
 *  COPYRIGHT (c) 2020 Utkarsh Rai.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
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
 * 
 */

#ifndef _RTEMS_SCORE_STACKPROTECTION_H
#define _RTEMS_SCORE_STACKPROTECTION_H

#include <rtems/score/stack.h>
#include <rtems/score/memoryprotection.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Share the stack address of a given thread with the target thread.
 * 
 * @param target_stack Stack control structure the target thread
 * @param sharing_address Stack address of the sharing thread
 * @param sharing_stack_size Size of the sharing stack
 * @param sharing_flags Memory access permission of the shared stack
 */

rtems_status_code _Stack_protection_Share_stack(
Stack_Control *target_stack,
Stack_Shared_attr sharing_attr
);

/**
 * @brief Swap the restored shared stacks  in the page table during context
 * restoration
 * 
 * We set the entries of the restored stack and mark all the remainig stacks as
 * 'NO-ACCESS'.
 * 
 * @param heir_stack Control block of the restored stack
 */ 
void _Stack_protection_Context_restore(
  Stack_Control *heir_stack
);

#ifdef __cplusplus
}
#endif

#endif