#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "duMalloc.h"

// Define HEAP_SIZE
#define HEAP_SIZE (128 * 8) // or 1kB

// Define memory block header structure
typedef struct memoryBlockHeader {
    int size; // size of the reserved block
    struct memoryBlockHeader* next; // the next block in the integrated free list
} memoryBlockHeader;

// Define global variables
unsigned char heap[HEAP_SIZE];
memoryBlockHeader* freeListHead;

// Initialize the heap and free list
void duInitMalloc() {
    // Initialize all memory in the heap to 0
    memset(heap, 0, HEAP_SIZE);
    
    // Stamp the header into the starting 16 bytes of the heap
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap;
    (*currentBlock).size = HEAP_SIZE - sizeof(memoryBlockHeader);
    (*currentBlock).next = NULL;

    // Set freeListHead to point to this block header
    freeListHead = currentBlock;
}

// Dump the current state of the heap
void duMemoryDump() {
    printf("MEMORY DUMP\n");
    printf("Free List\n");
    
    // Traverse the free list and print out each block
    memoryBlockHeader* current = freeListHead;
    while (current != NULL) {
        printf("Block at %p, size %d\n", (void*)current, (*current).size);
        current = (*current).next;
    }
}

// Allocate memory from the heap
void* duMalloc(int size) {
    // Round up the requested size to the next multiple of 8
    int adjustedSize = (size + 7) & ~7; // mask off the lower 3 bits
    
    // Traverse the free list to find a suitable block
    memoryBlockHeader* current = freeListHead;
    memoryBlockHeader* previous = NULL;
    while (current != NULL) {
        if ((*current).size >= adjustedSize) {
            // Found a block large enough
            // Remove it from the free list
            if (previous != NULL) {
                (*previous).next = (*current).next;
            } else {
                freeListHead = (*current).next;
            }
            
            // Check if we need to split the block
            if ((*current).size > adjustedSize + sizeof(memoryBlockHeader)) {
                // Split the block
                memoryBlockHeader* remainingBlock = (memoryBlockHeader*)((unsigned char*)current + adjustedSize + sizeof(memoryBlockHeader));
                (*remainingBlock).size = (*current).size - adjustedSize - sizeof(memoryBlockHeader);
                (*remainingBlock).next = freeListHead;
                
                // Update the size of the used block
                (*current).size = adjustedSize;
                
                // Update the free list
                freeListHead = remainingBlock;
            }
            
            // Return a pointer to the allocated memory
            return (void*)((unsigned char*)current + sizeof(memoryBlockHeader));
        }
        
        // Move to the next block
        previous = current;
        current = (*current).next;
    }
    
    // No suitable block found, return NULL
    return NULL;
}

// Free memory block
void duFree(void* ptr) {
    // Find the start of the block header
    memoryBlockHeader* blockHeader = (memoryBlockHeader*)((unsigned char*)ptr - sizeof(memoryBlockHeader));
    
    // Insert the block back into the free list in memory order
    memoryBlockHeader* current = freeListHead;
    memoryBlockHeader* previous = NULL;
    while (current != NULL && current < blockHeader) {
        previous = current;
        current = (*current).next;
    }
    
    // Insert the block before current
    (*blockHeader).next = current;
    if (previous != NULL) {
        (*previous).next = blockHeader;
    } else {
        freeListHead = blockHeader;
    }
    
    // Merge adjacent free blocks
    if (previous != NULL && (unsigned char*)previous + (*previous).size + sizeof(memoryBlockHeader) == (unsigned char*)blockHeader) {
        // Merge with previous block
        (*previous).size += (*blockHeader).size + sizeof(memoryBlockHeader);
        (*previous).next = (*blockHeader).next;
        blockHeader = previous;
    }
    
    if (current != NULL && (unsigned char*)blockHeader + (*blockHeader).size + sizeof(memoryBlockHeader) == (unsigned char*)current) {
        // Merge with current block
        (*blockHeader).size += (*current).size + sizeof(memoryBlockHeader);
        (*blockHeader).next = (*current).next;
    }
}
