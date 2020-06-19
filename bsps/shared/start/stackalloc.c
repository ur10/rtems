/**
 * @file
 *
 * @ingroup bsp_stack
 *
 * @brief Stack initialization, allocation and free functions.
 */

/*
 * Copyright (c) 2009-2013 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Obere Lagerstr. 30
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <bsp/stackalloc.h>

#include <rtems.h>
#include <rtems/score/heapimpl.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/stackmanagement.h>

#include <bsp/linker-symbols.h>

static Heap_Control bsp_stack_heap;

void bsp_stack_allocate_init(size_t stack_space_size)
{
  _Heap_Initialize(
    &bsp_stack_heap,
    bsp_section_stack_begin,
    (uintptr_t) bsp_section_stack_size,
    CPU_STACK_ALIGNMENT
  );
}

void *bsp_stack_allocate(size_t size)
{
  void *stack = NULL;
  uint32_t *page_table_base;
  if (bsp_stack_heap.area_begin != 0) {
    stack = _Heap_Allocate(&bsp_stack_heap, size);
  }

  if (stack == NULL) {
    stack = _Workspace_Allocate(size);
  }
  
  page_table_base = 0x1000;   // This is hard-coded page table base address
  prot_stack_allocate(stack, size, page_table_base);  // The current way to allocate protected stack is to assign memory attributes 
                                                      // to the allocated memory and remove memory-entry of every other stack
  
  return stack;
}

void bsp_stack_free(void *stack)
{
  bool ok = _Heap_Free(&bsp_stack_heap, stack);

  if (!ok) {
    _Workspace_Free(stack);
  }
}
