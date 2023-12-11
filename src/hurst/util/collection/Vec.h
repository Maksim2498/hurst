#ifndef HURST_UTIL_COLLECTION_VEC_H
#define HURST_UTIL_COLLECTION_VEC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <hurst/util/fmt/output/ArrayOFmt.h>
#include <hurst/util/io/print.h>

#include "array.h"

enum {
    VEC_DEFAULT_INIT_CAP = 8,
};

typedef struct {
    size_t len;
    size_t cap;
    size_t itemSize;
    char*  items;
} Vec;

void Vec_init(Vec* vec, size_t itemSize);
void Vec_initFilled(Vec* vec, size_t itemSize, const void* item, size_t n);
void Vec_initWithLen(Vec* vec, size_t itemSize, size_t len);
void Vec_initWithCap(Vec* vec, size_t itemSize, size_t cap);
void Vec_initWithLenAndCap(Vec* vec, size_t itemSize, size_t len, size_t cap);

void Vec_deinit(Vec* vec);

size_t Vec_getAllocedSize(const Vec* vec);
size_t Vec_getSize(const Vec* vec);
bool Vec_isEmpty(const Vec* vec);
size_t Vec_getLen(const Vec* vec);
void Vec_setLen(Vec* vec, size_t len);
size_t Vec_getCap(const Vec* vec);
size_t Vec_getItemSize(const Vec* vec);
void* Vec_getItems(Vec* vec);
const void* Vec_getCItems(const Vec* vec);

void* Vec_getFirst(Vec* vec);
const void* Vec_getCFirst(const Vec* vec);
void Vec_setFirst(Vec* vec, const void* item);
void* Vec_getLast(Vec* vec);
const void* Vec_getCLast(const Vec* vec);
void Vec_setLast(Vec* vec, const void* item);
void* Vec_getAt(Vec* vec, size_t at);
const void* Vec_getCAt(const Vec* vec, size_t at);
void Vec_setAt(Vec* vec, const void* item, size_t at);

void Vec_fill(Vec* vec, const void* item);
void Vec_fillRange(Vec* vec, const void* item, size_t at, size_t n);
void Vec_append(Vec* vec, const void* item);
void Vec_appendN(Vec* vec, const void* items, size_t n);
void Vec_prepend(Vec* vec, const void* item);
void Vec_prependN(Vec* vec, const void* items, size_t n);
void Vec_insert(Vec* vec, const void* item, size_t at);
void Vec_insertN(Vec* vec, const void* items, size_t at, size_t n);
bool Vec_pop(Vec* vec);
size_t Vec_popN(Vec* vec, size_t n);
bool Vec_shift(Vec* vec);
size_t Vec_shiftN(Vec* vec, size_t n);
bool Vec_remove(Vec* vec, size_t at);
size_t Vec_removeN(Vec* vec, size_t at, size_t n);
void Vec_clean(Vec* vec);
void Vec_shrinkToFit(Vec* vec);

void Vec_sort(Vec* vec, Cmp cmp);

bool Vec_isValid(const Vec* vec);

size_t printVec(FILE* file, const Vec* vec, Print print);
size_t printVecFmt(FILE* file, const Vec* vec, Print print, const struct ArrayOFmt* fmt);

#endif
