#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    READ_WRITE,
    READ_WRITE_CACHED,
    READ_ONLY,
    READ_ONLY_CACHED,
    NO_ACCESS
} memory_flags;

void memory_entries_set(uint32_t *begin_addr, size_t size, memory_flags flags);

void memory_entries_unset(uint32_t *begin_addr, size_t size, memory_flags flags);

//uint32_t *memory_get_ttb(void);  // The current method of isolating task-stacks is to remove the table entries
                                   // for the allocated stack, instead of changing ttb;

uint32_t memory_translate_flags(memory_flags flags);