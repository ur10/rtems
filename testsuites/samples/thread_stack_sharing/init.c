#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <tmacros.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <rtems/score/memoryprotection.h>

const char rtems_test_name[] = " THREAD STACK SHARING ";

void* Test_routine( void* arg )
{
    
}

void *POSIX_Init( void *argument ) 
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
  int fd;
  char name[4] = "0x01";
  char thread_name[13] = "/taskfs/0x01";

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

 /*
  * We set the memory attributes of the stack from the application.
  */
  _Memory_protection_Set_entries( stack_addr1, stack_size1, RTEMS_READ_ONLY | RTEMS_MEMORY_CACHED );

    pthread_create( &id2, &attr2, Test_routine, NULL );
  _Memory_protection_Set_entries( stack_addr2, stack_size2, RTEMS_READ_ONLY | RTEMS_MEMORY_CACHED );

  /*
   * Add leading "/taskfs/" to denote thread-stack name.
   */ 
  strlcat( thread_name, name, 4);

  /* 
  * Set the name of the thread object same as that of the shared memory object name
  */
  rtems_object_set_name( id1, name);

  /*
   * Create a shared memory object of the  stack we want to share with
   * appropraite permissions. We share the stack with read and write permission
   */
  fd = shm_open( thread_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
  
  /*
   * Truncate the size of the file to the size of the stack.
   */
  ftruncate( fd, stack_size1 );
  
  /*
   * For sharing the stack we specify the address of the
   * thread-stack we want to share with, the size of the shared stack,
   * protection and access flags, file descriptor of the shared memory objcet
   */
  addr = mmap( stack_addr2, stack_size1, PROT_READ | PROT_WRITE, O_RDWR, fd, 0 );
  rtems_test_assert( addr != NULL );

  pthread_join( id1, NULL );
  /*
   * Write to the stack address of thread1 after it has been switched out.
   */ 
  memset( stack_addr1, 0, stack_size1 );

  pthread_join( id2, NULL );
   /*
   * Write to the stack address of thread2 after it has been switched out.
   */  
  memset( stack_addr2, 0, stack_size2 );


  TEST_END();
  rtems_test_exit( 0 );
}

/* configuration information */

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_MAXIMUM_POSIX_THREADS        4

#define CONFIGURE_MAXIMUM_POSIX_SHMS           2

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 10

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_TASK_STACK_ALLOCATOR_INIT  bsp_stack_allocate_init
#define CONFIGURE_TASK_STACK_ALLOCATOR       bsp_stack_allocate
#define CONFIGURE_TASK_STACK_DEALLOCATOR     bsp_stack_free

#include <bsp/stackalloc.h>
#define CONFIGURE_INIT
#include <rtems/confdefs.h>