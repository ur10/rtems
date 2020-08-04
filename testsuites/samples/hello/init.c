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
#include <rtems/score/memoryprotection.h>

const char rtems_test_name[] = "HELLO WORLD";

void* addr1;
void* addr2;
void* addr3;

static rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_print_printer_fprintf_putc(&rtems_test_printer);
  TEST_BEGIN();
  posix_memalign( &addr1, 4096, 8192);
  posix_memalign( &addr2, 4096, 8192);
  posix_memalign( &addr3, 4096, 8192); 
 // _Memory_protection_Set_entries( addr1, 8192, READ_WRITE );
  //_Memory_protection_Set_entries( addr2, 8192, READ_ONLY );
  //_Memory_protection_Set_entries( addr3, 8192, NO_ACCESS );  
  char *c = addr1;
  c[0]++;
  char *b = addr2;
  char *d = addr3 + 8192;
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

#define CONFIGURE_INIT
#include <rtems/confdefs.h>