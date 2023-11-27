#include "mem.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void memAllocError_(void);

void* forceAllocMem(size_t size) {
    void* block = malloc(size);

    if (!block)
        memAllocError_();

    return block;
}

void forceReallocMem(void** block, size_t newSize) {
    assert(block);

    void* newBlock = realloc(*block, newSize);

    if (!newBlock)
        memAllocError_();

    *block = newBlock;
}

void forceFreeMem(void* block) {
    free(block);
}

void memAllocError_(void) {
    fputs("Not enough memory\n", stderr);
    exit(EXIT_FAILURE);
}
