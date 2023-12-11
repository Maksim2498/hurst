#ifndef HURST_UTIL_MEM_H
#define HURST_UTIL_MEM_H

#include <stddef.h>

void* forceAllocMem(size_t size);
void forceReallocMem(void** block, size_t newSize);
void forceFreeMem(void* block);

void swapMem(void* lhs, void* rhs, size_t size);

#endif
