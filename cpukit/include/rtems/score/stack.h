/**
 * @file
 *
 * @ingroup RTEMSScoreStack
 *
 * @brief Information About the Thread Stack Handler
 *
 * This include file contains all information about the thread
 * Stack Handler.  This Handler provides mechanisms which can be used to
 * initialize and utilize stacks.
 */

/*
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_STACK_H
#define _RTEMS_SCORE_STACK_H

#include <rtems/score/basedefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSScoreStack Stack Handler
 *
 * @ingroup RTEMSScore
 *
 * @brief Stack Handler
 *
 * This handler encapsulates functionality which is used in the management
 * of thread stacks.
 *
 * @{
 */

#define USE_THREAD_STACK_PROTECTION
/**
 *  The following constant defines the minimum stack size which every
 *  thread must exceed.
 */
#define STACK_MINIMUM_SIZE  CPU_STACK_MINIMUM_SIZE

/**
 * The number of stacks that can be shared with a thread.
 */ 
#define SHARED_STACK_NUMBER 8

#if defined ( USE_THREAD_STACK_PROTECTION )
/**
 * The following defines the attributes of a protected stack
 */
typedef struct
{
  /** The pointer to the page table base */
  uintptr_t      page_table_base;
  /**Memory flag for the alllocated/shared stack */
  uint32_t  access_flags;
} Stack_Protection_attr;


/**
 * The following defines the control block  of a shared stack. Each stack can have
 * different sharing attributes
 */
typedef struct
{
  /** This is the attribute of a shared stack*/
  Stack_Protection_attr    Base;
} Stack_Shared_attr;
#endif

/**
 *  The following defines the control block used to manage each stack.
 */
typedef struct {
  /** This is the stack size. */
  size_t      size;
  /** This is the low memory address of stack. */
  void       *area;
#if defined (USE_THREAD_STACK_PROTECTION)
   /** The attribute of a protected stack */
  Stack_Protection_attr    Base;
  /** The pointer to the attributes of a stack shared with the stack 
   * in question
   */
  Stack_Shared_attr  stack_sharing_attr;

  Stack_Shared_attr shared_stacks[SHARED_STACK_NUMBER];
#endif
}   Stack_Control;

/**
 * @brief The stack allocator initialization handler.
 *
 * @param stack_space_size The size of the stack space in bytes.
 */
typedef void ( *Stack_Allocator_initialize )( size_t stack_space_size );

/**
 * @brief Stack allocator allocate handler.
 *
 * @param stack_size The size of the stack area to allocate in bytes.
 *
 * @retval NULL Not enough memory.
 * @retval other Pointer to begin of stack area.
 */
typedef void *( *Stack_Allocator_allocate )( size_t stack_size );

/**
 * @brief Stack allocator free handler.
 *
 * @param] addr A pointer to previously allocated stack area or NULL.
 */
typedef void ( *Stack_Allocator_free )( void *addr );

/**
 * @brief The minimum stack size.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern uint32_t rtems_minimum_stack_size;

/**
 * @brief The configured stack space size.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern const uintptr_t _Stack_Space_size;

/**
 * @brief Indicates if the stack allocator avoids the workspace.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern const bool _Stack_Allocator_avoids_workspace;

/**
 * @brief The stack allocator initialization handler.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern const Stack_Allocator_initialize _Stack_Allocator_initialize;

/**
 * @brief The stack allocator allocate handler.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern const Stack_Allocator_allocate _Stack_Allocator_allocate;

/**
 * @brief The stack allocator free handler.
 *
 * Application provided via <rtems/confdefs.h>.
 */
extern const Stack_Allocator_free _Stack_Allocator_free;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
