#include "memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static variables for the allocator
static memoryBlockHeader* heaps[2];
static memoryBlockHeader* freeLists[2];
static int currentHeap;
static void** managedList;
static int managedListSize;
static int allocationStrategy;  // Properly declare allocationStrategy

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

// Initialize the memory allocator
void duInitMalloc(size_t size, int strategy) {
    allocationStrategy = strategy;
    for (int i = 0; i < 2; i++) {
        heaps[i] = (memoryBlockHeader*)malloc(size);
        if (!heaps[i]) {
            fprintf(stderr, "Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }

        heaps[i]->free = 1;
        heaps[i]->size = size - sizeof(memoryBlockHeader);
        heaps[i]->managedIndex = -1;
        heaps[i]->survivalAmt = 0;
        heaps[i]->next = NULL;

        freeLists[i] = heaps[i];
    }
    currentHeap = 0;
}

// Allocate memory using the managed system
void** duManagedMalloc(size_t size) {
    void* ptr = duMalloc(size, currentHeap);
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

// Allocate memory using the specified strategy and heap
void* duMalloc(size_t size, int heapIndex) {
    memoryBlockHeader *current, *best;
    current = freeLists[heapIndex];
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
        newBlock->survivalAmt = 0;
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

    int heapIndex = -1;
    for (int i = 0; i < 2; i++) {
        if ((char*)ptr >= (char*)heaps[i] && (char*)ptr < (char*)heaps[i] + heaps[i]->size + sizeof(memoryBlockHeader)) {
            heapIndex = i;
            break;
        }
    }
    if (heapIndex == -1) {
        fprintf(stderr, "Pointer not in any heap\n");
        return;
    }

    // Merge with next block if it's free
    if (block->next && block->next->free) {
        block->size += sizeof(memoryBlockHeader) + block->next->size;
        block->next = block->next->next;
    }

    // Merge with previous blocks
    memoryBlockHeader* current = heaps[heapIndex];
    while (current && current->next != block) {
        current = current->next;
    }

    if (current && current->free) {
        current->size += sizeof(memoryBlockHeader) + block->size;
        current->next = block->next;
    }
}

// Print memory blocks and their states
void printMemoryBlocks(int heapIndex) {
    memoryBlockHeader* current = heaps[heapIndex];
    char representation[129] = {0}; // 128 characters + null terminator
    memset(representation, '.', sizeof(representation) - 1);

    printf("Memory Block\n");
    while (current) {
        printf("%s at %p, size %d, surv: %d\n", current->free ? "Free" : "Used", current, current->size, current->survivalAmt);
        int start = ((char*)current - (char*)heaps[heapIndex]) / 8;
        int end = start + current->size / 8;
        char mark = current->free ? 'a' + (current - heaps[heapIndex]) / sizeof(memoryBlockHeader) % 26 : 'A' + (current - heaps[heapIndex]) / sizeof(memoryBlockHeader) % 26;
        for (int i = start; i < end; i++) {
            representation[i] = mark;
        }
        current = current->next;
    }

    printf("%s\n", representation);
}

// Print the free list
void printFreeList(int heapIndex) {
    memoryBlockHeader* current = freeLists[heapIndex];
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
    printf("Current heap (0/1 young): %d\n", currentHeap);
    printf("Young Heap (only the current one)\n");
    printMemoryBlocks(currentHeap);
    printFreeList(currentHeap);
    printManagedList();
}

// Perform a minor garbage collection
void minorCollection() {
    int oldHeap = currentHeap;
    int newHeap = 1 - currentHeap;
    char* newHeapPtr = (char*)heaps[newHeap] + sizeof(memoryBlockHeader);

    // Iterate over the managed list and move live objects
    for (int i = 0; i < managedListSize; i++) {
        if (managedList[i] != NULL) {
            memoryBlockHeader* oldBlock = (memoryBlockHeader*)((char*)managedList[i] - sizeof(memoryBlockHeader));
            oldBlock->survivalAmt++;

            memoryBlockHeader* newBlock = (memoryBlockHeader*)newHeapPtr;
            memcpy(newBlock, oldBlock, sizeof(memoryBlockHeader) + oldBlock->size);
            managedList[i] = (void*)((char*)newBlock + sizeof(memoryBlockHeader));
            newHeapPtr += sizeof(memoryBlockHeader) + newBlock->size;
        }
    }

    // Create a free block at the end of the new heap
    memoryBlockHeader* freeBlock = (memoryBlockHeader*)newHeapPtr;
    freeBlock->free = 1;
    freeBlock->size = heaps[newHeap]->size - (newHeapPtr - (char*)heaps[newHeap]) - sizeof(memoryBlockHeader);
    freeBlock->managedIndex = -1;
    freeBlock->survivalAmt = 0;
    freeBlock->next = NULL;

    heaps[newHeap]->next = freeBlock;
    freeLists[newHeap] = freeBlock;
    currentHeap = newHeap;
}
