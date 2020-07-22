/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <tmacros.h>

const char rtems_test_name[] = "HELLO WORLD";

void* block1;
void* block2;
void *block3;
void *block4;
static rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_print_printer_fprintf_putc(&rtems_test_printer);
  TEST_BEGIN();
  posix_memalign(&block1, 4096, 8192);
  posix_memalign(&block2, 4096, 8192);
    posix_memalign(&block3, 4096, 8192);
      posix_memalign(&block4, 4096, 8192);
  _Memory_protection_Set_entries(block1, 8192, READ_WRITE);
  _Memory_protection_Set_entries(block2, 8192, READ_ONLY );
  _Memory_protection_Set_entries(block3, 8192, READ_WRITE);
  _Memory_protection_Set_entries(block4, 8192, READ_ONLY);  
char *c= block1;
char *b = block3;
c[0]++;
b[0]++;
  printf( "Hello World\n" );
  TEST_END();
  rtems_test_exit( 0 );
}


/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_TASK_STACK_ALLOCATOR_INIT  bsp_stack_allocate_init
#define CONFIGURE_TASK_STACK_ALLOCATOR       bsp_stack_allocate
#define CONFIGURE_TASK_STACK_DEALLOCATOR     bsp_stack_free

#include <bsp/stackalloc.h>
#define CONFIGURE_INIT
#include <rtems/confdefs.h>