/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreMemorymanagement
 *
 * @brief This file provodes APIs for high-level memory management
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

#ifndef _RTEMS_SCORE_MEMORYMANAGEMENT_H
#define _RTEMS_SCORE_MEMORYMANAGEMENT_H

#include <rtems/score/basedefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/** This defines the various high-level memory access flags.*/
typedef enum
{
    READ_WRITE,
    READ_WRITE_CACHED,
    READ_ONLY,
    READ_ONLY_CACHED,
    NO_ACCESS
} Memorymanagement_flags;

/**
 * @brief Define the memory access permission for the specified memory region
 * 
 * @param begin_addr Begining of the memory region
 * @param size Size of the memory region
 * @param flag Memory access flag
 * 
 */
void Memorymanagement_Set_entries(uintptr_t begin_addr, size_t size, Memorymanagement_flags flag);

/**
 * @brief Unset the memory access permission for the specified memory region
 * This operation implcitly sets the specified memory region with 'NO_ACCESS'
 * flag.
 * 
 * @param begin_addr Begining of the memory region
 * @param size Size of the memory region
 */
void Memorymanagement_Unset_entries(uintptr_t begin_addr, size_t size);

/**
 * @brief Translate the high-level flags to BSP-specifc MMU flags.
 * 
 * @param attr_flags High-level memory access flags.
 * 
 * @retval flag BSP-specifc MMU flag
 */
uint32_t Memorymanagement_Translate_flags(Memorymanagement_flags attr_flags);

#ifdef __cplusplus
}
#endif

#endif