#ifndef HURST_UTIL_FMT_OUTPUT_PARSEDOPRECISION_H
#define HURST_UTIL_FMT_OUTPUT_PARSEDOPRECISION_H

#include <stdbool.h>
#include <stddef.h>

struct ParsedOPrecision {
    size_t value;
    size_t len;
    bool   valid;
};

bool ParsedOPrecision_isValid(const struct ParsedOPrecision* parsedPrecision);

#endif
