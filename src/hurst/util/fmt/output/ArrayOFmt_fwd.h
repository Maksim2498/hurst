#ifndef HURST_UTIL_FMT_OUTPUT_ARRAYOFMT_FWD_H
#define HURST_UTIL_FMT_OUTPUT_ARRAYOFMT_FWD_H

#include <stdbool.h>
#include <stddef.h>

struct ArrayOFmt {
    const char* open;
    size_t      openLen;

    const char* sep;
    size_t      sepLen;

    const char* close;
    size_t      closeLen;
};

#endif
