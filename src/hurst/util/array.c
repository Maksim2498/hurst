#include "array.h"

#include <assert.h>

bool allNotNull(const void* const* array, size_t len) {
    if (!array)
        return false;

    for (size_t i = 0; i < len; ++i)
        if (!array[i])
            return false;

    return true;
}
