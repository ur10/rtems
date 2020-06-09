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

#if defined ( ASM )
#include <rtems/asm.h>
#else
  #include <rtems/score/basedefs.h>
  #include <rtems/score/memorymanagement.h>
  #include <rtems/score/chainimpl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined ( ASM )

/**
 * The following defines the attributes of a protected stack
 */
typedef struct
{
  /** This is the node to the chain for tracking each allocated/shared stack */
  Chain_Node    node; 
  /** This is the stack address */
  uintptr_t        stack_address;
  /** This is the stack size */
  size_t        size;
  /** This is the pointer to the page table base */
  uintptr_t      page_table_base;
  /**Memory flag for the alllocated/shared stack */
  Memorymanagement_flags  access_flags;
} Stackprotection_Attr;

/**
 * The following defines the control block  of a shared stack
 */
typedef struct 
{
  /** This is the attribute of a shared stack*/
  Stackprotection_Attr    Base;
} Stackprotection_Shared_stack;

/**
 * The following defines the control block of an allocated stack 
 */
typedef struct
{ 
  /** This is the attribute of an allocated stack*/
  Stackprotection_Attr    Base;
  /** This is the pointer to the attributes of a stack shared with the stack 
   * in question
   */
  Stackprotection_Shared_stack  *shared_stacks; 
  /** 
  * The chain control for tracking the shared stacks with the thread-stack in 
  * question.
  */
  Chain_Control shared_node_control;
  /**This marks if the stack in question belongs to an executing thread*/
  bool          current_stack;  
} Stackprotection_The_stack;

/**
 * @brief Allocate the attributes of the stack in question.
 *
 * @param freechain The stack address.
 * @param size Size of the stack.
 * @param page_table_base Pointer to the start of a page table
 */
void _Stackprotection_Allocate_attr(uintptr_t stack_address, size_t size, uintptr_t page_table_base);

/**
 * @brief Share a stack with another stack.
 * 
 * @param shared_stack The stack to be shared
 * @param target_stack The stack with which to share
 */
void _Stackprotection_Share_stack(Stackprotection_The_stack *shared_stack, Stackprotection_The_stack* target_stack);

/**
 * @brief Initialize the context of a thread with the control block of a 
 * protected stack
 * 
 * @retval the_stack The protected stack
 */ 
Stackprotection_The_stack *_Stackprotection_Context_initialize(void);

/**
 * @brief Swap out the executing protected stack from the page table during 
 * context switch
 * 
 * The current method of switching the protected stack is to mark the switched
 * out stack as 'NO ACCESS'
 * 
 * @param excuting_stack Control block of the executing stack
 */
void _Stackprotection_Context_switch(Stackprotection_The_stack *executing_stack, Stackprotection_The_stack *heir_stack);

/**
 * @brief Swap the restored protected stack  in the page table during context
 * restoration
 * 
 * We set the entries of the restored stack and mark all the remainig stacks as
 * 'NO-ACCESS'.
 * 
 * @param Control block of the restored stack
 */ 
void _Stackprotection_Context_restore(Stackprotection_The_stack *restored_stack);

#endif /* !defined ( ASM ) */

#ifdef __cplusplus
}
#endif

#endif
