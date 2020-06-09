/*
 * Copyright (c) 2016 Gedare Bloom.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

/*
 * From http://pubs.opengroup.org/onlinepubs/9699919799/functions/shm_open.html
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CONFIGURE_INIT

#include "pritime.h"

#include <sys/mman.h>
#include <rtems/score/memoryprotection.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <tmacros.h>
#include <pthread.h>


const char rtems_test_name[] = "PSX SHM01";

#define MAX_LEN 10000
struct region {        /* Defines "structure" of shared memory */
  int len;
  char buf[MAX_LEN];
};

void* Test_routine( void* arg ) {

}

void *POSIX_Init(
  void *argument
)
{
    void *stack_addr1;
  void *stack_addr2;
  void* addr;
  size_t stack_size1;
  size_t stack_size2;
  pthread_t id1;
  pthread_t id2;
  pthread_attr_t attr1;  
  pthread_attr_t attr2;
  struct region *p;
  int fd;
  int err;
  char name[13] = "/taskfs/0x01";
  char *thread_name = "0x01";

  TEST_BEGIN();

 /*
  *  We set the stack size as 8Kb.
  */
  stack_size1 = 8192;
  stack_size2 = 8192;

  /*
   * We allocate page-aligned memory of the stack  from the application.
   */
  posix_memalign(&stack_addr1, sysconf( _SC_PAGESIZE ), stack_size1 );
  posix_memalign(&stack_addr2, sysconf( _SC_PAGESIZE ), stack_size2 );

  pthread_attr_init( &attr1 );  
  pthread_attr_init( &attr2 );
 
 /*
  * We set the stack size and address of the thread from the application itself
  */
  pthread_attr_setstack( &attr1, stack_addr1, stack_size1 );
  pthread_attr_setstack( &attr2, stack_addr2, stack_size2 );

  pthread_create( &id1, &attr1, Test_routine, NULL );
  pthread_create( &id2, &attr2, Test_routine, NULL );
 /*
  * We set the memory attributes of the stack from the application.
  */
  _Memory_protection_Set_entries( stack_addr1, stack_size1, RTEMS_READ_ONLY | RTEMS_MEMORY_CACHED );
  _Memory_protection_Set_entries( stack_addr2, stack_size2, RTEMS_READ_ONLY | RTEMS_MEMORY_CACHED );

  strlcat(name, thread_name, 4);

  rtems_object_set_name( id2, thread_name );

  puts( "Init: shm_open" );
  fd = shm_open( name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
  if ( fd == -1 ) {
    printf ( "Error: %s\n", strerror(errno) );
    rtems_test_assert( fd != -1 );
  }

  puts( "Init: ftruncate" );
  err = ftruncate( fd, stack_size2 );
  if ( err == -1 ) {
    printf ( "Error: %s\n", strerror(errno) );
    rtems_test_assert( err != -1 );
  }

  puts( "Init: mmap" );
  p = mmap(
    NULL,
    sizeof( struct region ),
    PROT_READ | PROT_WRITE, MAP_SHARED,
    fd,
    0
  );

  if ( p != MAP_FAILED ) {
    puts( "Init: write to mapped region" );
    p->len = MAX_LEN;

    puts( "Init: munmap" );
    err = munmap( p, sizeof( struct region ) );
    if ( err == -1 ) {
      printf ( "Error: %s\n", strerror(errno) );
      rtems_test_assert( err != -1 );
    }
  } else {
    puts( "Init: FIXME: mmap() not supported" );
  }

  puts( "Init: close" );
  err = close( fd );
  if ( err == -1 ) {
    printf ( "Error: %s\n", strerror(errno) );
    rtems_test_assert( err != -1 );
  }

  TEST_END();

  rtems_test_exit(0);
  return 0;
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_MAXIMUM_POSIX_THREADS     3
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 10
#define CONFIGURE_MAXIMUM_POSIX_SHMS 1
#define CONFIGURE_MEMORY_OVERHEAD 10

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_TASK_STACK_ALLOCATOR_INIT  bsp_stack_allocate_init
#define CONFIGURE_TASK_STACK_ALLOCATOR       bsp_stack_allocate
#define CONFIGURE_TASK_STACK_DEALLOCATOR     bsp_stack_free

#include <bsp/stackalloc.h>
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
