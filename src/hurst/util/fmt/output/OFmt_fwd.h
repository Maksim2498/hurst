#ifndef HURST_UTIL_FMT_OUTPUT_OFMT_FWD_H
#define HURST_UTIL_FMT_OUTPUT_OFMT_FWD_H

#include <stdbool.h>
#include <stddef.h>

#include "ArrayOFmt_fwd.h"

struct OFmt {
    struct ArrayOFmt arrayFmt;
    size_t           width;
    size_t           precision;
    unsigned char    type;
    char             filler;
    bool             isArray;
    bool             usePrecision;
};

#endif
