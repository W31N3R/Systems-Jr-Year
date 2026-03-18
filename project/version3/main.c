#include "memory_allocator.h"
#include <stdio.h>

int main() {
    duManagedInitMalloc(1024, FIRST_FIT); // Initialize with FIRST_FIT strategy
    
    Managed_t(char *) a1 = (Managed_t(char *))duManagedMalloc(128); // Allocate 128 bytes
    Managed_t(char *) a2 = (Managed_t(char *))duManagedMalloc(32);  // Allocate 32 bytes
    Managed_t(char *) a3 = (Managed_t(char *))duManagedMalloc(40);  // Allocate 40 bytes
    Managed_t(char *) a4 = (Managed_t(char *))duManagedMalloc(24);  // Allocate 24 bytes
    Managed_t(char *) a5 = (Managed_t(char *))duManagedMalloc(120); // Allocate 120 bytes

    duManagedFree((void **)a2); // Free the 32-byte block
    duManagedFree((void **)a4); // Free the 24-byte block
    duMemoryDump(); // Dump memory layout

    minorCollection(); // Perform minor garbage collection
    duMemoryDump(); // Dump memory layout after collection

    return 0;
}
