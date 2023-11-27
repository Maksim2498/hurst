#ifndef HURST_UTIL_FMT_OUTPUT_PARSEDARRAYOFMT_H
#define HURST_UTIL_FMT_OUTPUT_PARSEDARRAYOFMT_H

#include <stdbool.h>
#include <stddef.h>

#include "ArrayOFmt_fwd.h"

struct ParsedArrayOFmt {
    size_t           len;
    struct ArrayOFmt value;
    bool             valid;
};

bool ParsedArrayOFmt_isValid(const struct ParsedArrayOFmt* arrayFmt);

#endif
