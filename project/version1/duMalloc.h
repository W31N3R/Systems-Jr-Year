#ifndef DUMALLOC_H
#define DUMALLOC_H

typedef struct memoryBlockHeader {
    int size;
    struct memoryBlockHeader* next;
} memoryBlockHeader;

void duInitMalloc();
void duMemoryDump();
void* duMalloc(int bytes);
void duFree(void* ptr);

#endif
