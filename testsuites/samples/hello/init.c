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

#include <tmacros.h>
<<<<<<< HEAD
#include "test_support.h"
#include <pthread.h>
=======
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <rtems/score/memoryprotection.h>
#include <rtems/score/stack.h>
>>>>>>> 989d286862b21386363ab58a8d3aec9c226bf2de

const char rtems_test_name[] = "PSX 16";

/* forward declarations to avoid warnings */
void *POSIX_Init(void *argument);
void *TestThread(void *argument);

int Index;

void *TestThread(
  void *argument
)
{
  int *index = (int *)argument;

  *index = 7; 

  puts( "TestThread exiting" );
  return argument;
}

void *POSIX_Init(void *argument)
{
  int             status;
  pthread_t       id;
  pthread_attr_t  attr;
  void           *join_return;

  TEST_BEGIN();
  
  int fd = shm_open("/taskfs/", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );

  ftruncate( fd, 8);

 // _Memory_protection_Set_entries( addr1, 8192, READ_WRITE );
  //_Memory_protection_Set_entries( addr2, 8192, READ_ONLY );
  //_Memory_protection_Set_entries( addr3, 8192, NO_ACCESS );  
  char *c = addr1;
  c[0]++;
  char *b = addr2;
  char *d = addr3 + 8192;
  printf( "Hello World\n" );
  TEST_END();

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_MAXIMUM_POSIX_THREADS        3

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 10

#define CONFIGURE_MAXIMUM_POSIX_SHMS           2

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_TASK_STACK_ALLOCATOR_INIT  bsp_stack_allocate_init
#define CONFIGURE_TASK_STACK_ALLOCATOR       bsp_stack_allocate
#define CONFIGURE_TASK_STACK_DEALLOCATOR     bsp_stack_free

#include <bsp/stackalloc.h>
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */