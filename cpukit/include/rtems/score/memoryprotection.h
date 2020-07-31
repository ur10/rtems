/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSScoreMemoryprotection
 *
 * @brief This file provodes APIs for high-level memory protection
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

#ifndef _RTEMS_SCORE_MEMORYPROTECTION_H
#define _RTEMS_SCORE_MEMORYPROTECTION_H

#include <rtems/score/basedefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NO_ACCESS 0x00
#define READ_ONLY 0x01
#define READ_WRITE 0x02
#define MEMORY_CACHED 0x04

/**
 * @brief Define the memory access permission for the specified memory region
 * 
 * @param begin_addr Beginning of the memory region
 * @param size Size of the memory region
 * @param flag Memory access flag
 * 
 */
void _Memory_protection_Set_entries(
  uintptr_t begin_addr,
  size_t size,
  uint32_t flag
);

/**
 * @brief Unset the memory access permission for the specified memory region
 * This operation implicitly sets the specified memory region with 'NO_ACCESS'
 * flag.
 * 
 * @param begin_addr Begining of the memory region
 * @param size Size of the memory region
 */
void _Memory_protection_Unset_entries(
  uintptr_t begin_addr,
  size_t size
);

#ifdef __cplusplus
}
#endif

#endif