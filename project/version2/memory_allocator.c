#include "memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static variables for the allocator
static memoryBlockHeader* freeList;
static memoryBlockHeader* heapStart;
static int allocationStrategy;
static void** managedList;
static int managedListSize;

// Initialize the memory allocator with the managed list
void duManagedInitMalloc(size_t size, int strategy) {
    duInitMalloc(size, strategy);
    managedListSize = size / (8 * sizeof(void*));
    managedList = (void**)malloc(managedListSize * sizeof(void*));
    if (!managedList) {
        fprintf(stderr, "Failed to allocate managed list\n");
        exit(EXIT_FAILURE);
    }
    memset(managedList, 0, managedListSize * sizeof(void*));
}

// Allocate memory using the managed system
void** duManagedMalloc(size_t size) {
    void* ptr = duMalloc(size);
    if (!ptr) return NULL;

    // Find a free slot in the managed list
    int managedIndex = -1;
    for (int i = 0; i < managedListSize; i++) {
        if (managedList[i] == NULL) {
            managedIndex = i;
            managedList[i] = ptr;
            break;
        }
    }
    if (managedIndex == -1) {
        fprintf(stderr, "Managed list is full\n");
        return NULL;
    }

    // Set the managed index in the memory block
    memoryBlockHeader* block = (memoryBlockHeader*)((char*)ptr - sizeof(memoryBlockHeader));
    block->managedIndex = managedIndex;

    return &managedList[managedIndex];
}

// Free allocated memory in the managed system
void duManagedFree(void** mptr) {
    if (!mptr || !*mptr) return;

    // Find the memory block header
    memoryBlockHeader* block = (memoryBlockHeader*)((char*)(*mptr) - sizeof(memoryBlockHeader));

    // Free the memory block
    duFree(*mptr);

    // Null out the managed list entry
    managedList[block->managedIndex] = NULL;
}

// Initialize the memory allocator
void duInitMalloc(size_t size, int strategy) {
    allocationStrategy = strategy;
    heapStart = (memoryBlockHeader*)malloc(size);
    if (!heapStart) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    heapStart->free = 1;
    heapStart->size = size - sizeof(memoryBlockHeader);
    heapStart->managedIndex = -1;
    heapStart->next = NULL;
    freeList = heapStart;
}

// Allocate memory using the specified strategy
void* duMalloc(size_t size) {
    memoryBlockHeader *current, *best;
    current = freeList;
    best = NULL;

    while (current) {
        if (current->free && current->size >= size) {
            if (allocationStrategy == FIRST_FIT) {
                break;
            } else if (allocationStrategy == BEST_FIT) {
                if (!best || current->size < best->size) {
                    best = current;
                }
            }
        }
        current = current->next;
    }

    if (allocationStrategy == BEST_FIT) {
        current = best;
    }

    if (!current || !current->free || current->size < size) {
        return NULL;
    }

    if (current->size > size + sizeof(memoryBlockHeader)) {
        memoryBlockHeader* newBlock = (memoryBlockHeader*)((char*)current + sizeof(memoryBlockHeader) + size);
        newBlock->free = 1;
        newBlock->size = current->size - size - sizeof(memoryBlockHeader);
        newBlock->managedIndex = -1;
        newBlock->next = current->next;

        current->next = newBlock;
        current->size = size;
    }

    current->free = 0;
    return (void*)((char*)current + sizeof(memoryBlockHeader));
}

// Free allocated memory
void duFree(void* ptr) {
    if (!ptr) return;

    memoryBlockHeader* block = (memoryBlockHeader*)((char*)ptr - sizeof(memoryBlockHeader));
    block->free = 1;

    // Merge with next block if it's free
    if (block->next && block->next->free) {
        block->size += sizeof(memoryBlockHeader) + block->next->size;
        block->next = block->next->next;
    }

    // Merge with previous blocks
    memoryBlockHeader* current = heapStart;
    while (current && current->next != block) {
        current = current->next;
    }

    if (current && current->free) {
        current->size += sizeof(memoryBlockHeader) + block->size;
        current->next = block->next;
    }
}

// Print memory blocks and their states
void printMemoryBlocks() {
    memoryBlockHeader* current = heapStart;
    char representation[129] = {0}; // 128 characters + null terminator
    memset(representation, '.', sizeof(representation) - 1);

    printf("Memory Block\n");
    while (current) {
        printf("%s at %p, size %d\n", current->free ? "Free" : "Used", current, current->size);
        int start = ((char*)current - (char*)heapStart) / 8;
        int end = start + current->size / 8;
        char mark = current->free ? 'a' + (current - heapStart) / sizeof(memoryBlockHeader) % 26 : 'A' + (current - heapStart) / sizeof(memoryBlockHeader) % 26;
        for (int i = start; i < end; i++) {
            representation[i] = mark;
        }
        current = current->next;
    }

    printf("%s\n", representation);
}

// Print the free list
void printFreeList() {
    memoryBlockHeader* current = freeList;
    printf("Free List\n");
    while (current) {
        printf("Block at %p, size %d\n", current, current->size);
        current = current->next;
    }
}

// Print the managed list
void printManagedList() {
    printf("Managed List\n");
    for (int i = 0; i < managedListSize; i++) {
        printf("ManagedList[%d] = %p\n", i, managedList[i]);
    }
}

// Dump the memory layout
void duMemoryDump() {
    printMemoryBlocks();
    printFreeList();
    printManagedList();
}
