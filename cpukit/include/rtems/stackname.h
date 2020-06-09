/**
 * @file
 * 
 * @brief Stack name API
 */

/*
 *  COPYRIGHT (c) 2020 Utkarsh Rai
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

#ifndef _RTEMS_STACKNAME_H
#define _RTEMS_STACKNAME_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup 
 *
 * @ingroup 
 *
 * @brief Stack name API 
 */

/**
 * @brief Provide name to the address of the given stack.
 * 
 * @param stack_address The address of the given stack.
 * 
*/
void rtems_stack_name_set( uintptr_t stack_address );

/**
 * @brief Get the name of the provided stack address.
 * 
 * @param stack_address The address of the given stack.
 * 
 * @retval NULL @a stack_address is not present in the chain. 
 * @retval name @a stack_address name. 
 */
char *rtems_stack_name_get( uintptr_t stack_address );

#ifdef __cplusplus
}
#endif

#endif