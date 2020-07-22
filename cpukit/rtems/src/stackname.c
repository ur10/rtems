/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup 
 *
 * @brief RTEMS Stack name
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

#include <string.h>
#include <rtems/score/objectimpl.h>
#include <rtems/score/thread.h>

/*
 * Address of the required stack
 */
void *address;

static bool stack_address_get_visitor(Thread_Control *the_thread, void *arg)
{
    char* name;

    name = arg;

//#if defined (USE_THREAD_STACK_PROTECTION)
    if(name != NULL) {
        if ( strcmp(name, the_thread->the_stack.name) == 0 ) {
             address = the_thread->the_stack.Base.stack_address;  
             return true;
        }
    }
//#endif
}

void *rtems_stack_address_get( char* stack_name ) 
{  
#if defined (USE_THREAD_STACK_PROTECTION)
    rtems_task_iterate( stack_name_set_visitor, stack_address);
#endif
}