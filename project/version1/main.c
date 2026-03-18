#include "memory_allocator.h"
#include <stdio.h>

int main() {
    duInitMalloc(1024, FIRST_FIT); // Initialize with FIRST_FIT strategy
    
    void* a = duMalloc(128); // Allocate 128 bytes
    void* b = duMalloc(32);  // Allocate 32 bytes
    void* c = duMalloc(40);  // Allocate 40 bytes
    void* d = duMalloc(24);  // Allocate 24 bytes
    void* e = duMalloc(120); // Allocate 120 bytes

    duFree(b); // Free the 32-byte block
    duFree(d); // Free the 24-byte block
    duMemoryDump(); // Dump memory layout

    return 0;
}
