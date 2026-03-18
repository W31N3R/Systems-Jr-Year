#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>

// Memory block header structure
typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 - free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    int survivalAmt; // the number of times the block has moved between young heaps
    struct memoryBlockHeader* next; // pointer to the next block
} memoryBlockHeader;

// Allocation strategies
#define FIRST_FIT 0
#define BEST_FIT 1

// Function prototypes for managed versions
void duManagedInitMalloc(size_t size, int strategy);
void** duManagedMalloc(size_t size);
void duManagedFree(void** mptr);
void duMemoryDump();
void minorCollection();
void majorCollection();

// Macros for managed pointers
#define Managed(p) (*p)
#define Managed_t(t) t*

#endif // MEMORY_ALLOCATOR_H
