#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>

// Memory block header structure
typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 - free
    int size; // size of the reserved block
    struct memoryBlockHeader* next; // pointer to the next block
} memoryBlockHeader;

// Allocation strategies
#define FIRST_FIT 0
#define BEST_FIT 1

// Function prototypes
void duInitMalloc(size_t size, int strategy);
void* duMalloc(size_t size);
void duFree(void* ptr);
void duMemoryDump();

#endif // MEMORY_ALLOCATOR_H
