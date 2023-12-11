#include "Vec.h"

#include <assert.h>
#include <string.h>

#include <hurst/util/mem.h>

void Vec_grow_(Vec* vec);

void Vec_init(Vec* vec, size_t itemSize) {
    Vec_initWithCap(vec, itemSize, VEC_DEFAULT_INIT_CAP);
}

void Vec_initWithCap(Vec* vec, size_t itemSize, size_t cap) {
    assert(vec);

    vec->len      = 0;
    vec->cap      = cap;
    vec->itemSize = itemSize;
    vec->items    = forceAllocMem(cap * itemSize);
}

void Vec_deinit(Vec* vec) {
    assert(Vec_isValid(vec));

    forceFreeMem(vec->items);

    vec->len   = 0;
    vec->cap   = 0;
    vec->items = NULL;
}

size_t Vec_getAllocedSize(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->itemSize * vec->cap;
}

size_t Vec_getSize(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->itemSize * vec->len;
}

bool Vec_isEmpty(const Vec* vec) {
    assert(Vec_isValid(vec));
    return !vec->len;
}

size_t Vec_getLen(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->len;
}

size_t Vec_getCap(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->cap;
}

size_t Vec_getItemSize(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->itemSize;
}

void* Vec_getItems(Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->items;
}

const void* Vec_getCItems(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->items;
}

void* Vec_getFirst(Vec* vec) {
    return (void*) Vec_getCFirst(vec);
}

const void* Vec_getCFirst(const Vec* vec) {
    assert(Vec_isValid(vec));
    return vec->len ? vec->items : NULL;
}

void Vec_setFirst(Vec* vec, const void* item) {
    assert(!Vec_isEmpty(vec) && item);
    memcpy(vec->items, item, vec->itemSize);
}

void* Vec_getLast(Vec* vec) {
    return (void*) Vec_getCLast(vec);
}

const void* Vec_getCLast(const Vec* vec) {
    assert(Vec_isValid(vec));

    return vec->len ? vec->items + (vec->len - 1) * vec->itemSize
                    : NULL;
}

void Vec_setLast(Vec* vec, const void* item) {
    assert(!Vec_isEmpty(vec) && item);

    memcpy(
        vec->items + (vec->len - 1) * vec->itemSize,
        item,
        vec->itemSize
    );
}

void* Vec_getAt(Vec* vec, size_t at) {
    return (void*) Vec_getCAt(vec, at);
}

const void* Vec_getCAt(const Vec* vec, size_t at) {
    assert(Vec_isValid(vec) && at <= vec->len);
    return vec->items + at * vec->itemSize;
}

void Vec_setAt(Vec* vec, const void* item, size_t at) {
    assert(at < Vec_getLen(vec) && item);

    memcpy(
        vec->items + at * vec->itemSize,
        item,
        vec->itemSize
    );
}

void Vec_append(Vec* vec, const void* item) {
    Vec_appendN(vec, item, 1);
}

void Vec_appendN(Vec* vec, const void* items, size_t n) {
    assert(Vec_isValid(vec) && items);

    size_t newLen = vec->len + n;

    while (newLen > vec->cap)
        Vec_grow_(vec);

    memcpy(
        vec->items + vec->len * vec->itemSize,
        items,
        n * vec->itemSize
    );

    vec->len = newLen;
}

void Vec_prepend(Vec* vec, const void* item) {
    Vec_appendN(vec, item, 1);
}

void Vec_prependN(Vec* vec, const void* items, size_t n) {
    assert(Vec_isValid(vec) && items);

    size_t newLen = vec->len + n;

    while (newLen > vec->cap)
        Vec_grow_(vec);

    memmove(
        vec->items + n * vec->itemSize,
        vec->items,
        vec->len * vec->itemSize
    );

    memcpy(
        vec->items,
        items,
        n * vec->itemSize
    );

    vec->len = newLen;
}

void Vec_insert(Vec* vec, const void* item, size_t at) {
    Vec_insertN(vec, item, at, 1);
}

void Vec_insertN(Vec* vec, const void* items, size_t at, size_t n) {
    assert(Vec_isValid(vec) && items && at < vec->len);

    size_t newLen = vec->len + n;

    while (newLen > vec->cap)
        Vec_grow_(vec);

    memmove(
        vec->items + (at + n) * vec->itemSize,
        vec->items +  at      * vec->itemSize,
        n * vec->itemSize
    );

    memcpy(
        vec->items + at * vec->itemSize,
        items,
        n * vec->itemSize
    );

    vec->len = newLen;
}

bool Vec_pop(Vec* vec) {
    return Vec_popN(vec, 1);
}

size_t Vec_popN(Vec* vec, size_t n) {
    assert(Vec_isValid(vec));

    if (n > vec->len)
        n = vec->len;

    vec->len -= n;

    return n;
}

bool Vec_shift(Vec* vec) {
    return Vec_shiftN(vec, 1);
}

size_t Vec_shiftN(Vec* vec, size_t n) {
    assert(Vec_isValid(vec));

    if (n > vec->len)
        n = vec->len;

    memmove(
        vec->items,
        vec->items + n * vec->itemSize,
        (vec->len - n) * vec->itemSize
    );

    return n;
}

bool Vec_remove(Vec* vec, size_t at) {
    return Vec_removeN(vec, at, 1);
}

size_t Vec_removeN(Vec* vec, size_t at, size_t n) {
    assert(Vec_isValid(vec));

    if (at >= vec->len)
        return 0;

    const size_t maxN = vec->len - at;

    if (n > maxN)
        n = maxN;

    memmove(
        vec->items +  at      * vec->itemSize,
        vec->items + (at + n) * vec->itemSize,
        (vec->len - at - n) * vec->itemSize
    );

    return n;
}

void Vec_clean(Vec* vec) {
    assert(Vec_isValid(vec));
    vec->len = 0;
}

void Vec_shrinkToFit(Vec* vec) {
    assert(Vec_isValid(vec));

    if (vec->cap == vec->len)
        return;

    forceReallocMem((void**) &vec->items, vec->len * vec->itemSize);

    vec->cap = vec->len;
}

void Vec_sort(Vec* vec, Cmp cmp) {
    assert(Vec_isValid(vec) && cmp);
    // TODO
}

void Vec_grow_(Vec* vec) {
    assert(Vec_isValid(vec));

    vec->cap *= 2;

    forceReallocMem((void**) &vec->items, vec->cap * vec->itemSize);
}

bool Vec_isValid(const Vec* vec) {
    return vec
        && vec->items
        && vec->len <= vec->cap;
}

size_t printVec(FILE* file, const Vec* vec, Print print) {
    return printVecFmt(file, vec, print, &ARRAY_OFMT_DEFAULT);
}

size_t printVecFmt(FILE* file, const Vec* vec, Print print, const struct ArrayOFmt* fmt) {
    return printArrayFmt(
        file,
        Vec_getCItems(vec),
        Vec_getLen(vec),
        Vec_getItemSize(vec),
        print,
        fmt
    );
}
