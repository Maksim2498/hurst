#ifndef HURST_UTIL_FMT_OUTPUT_PARSEDOWIDTH_H
#define HURST_UTIL_FMT_OUTPUT_PARSEDOWIDTH_H

#include <stdbool.h>
#include <stddef.h>

struct ParsedOWidth {
    size_t value;
    size_t len;
    bool   valid;
};

bool ParsedOWidth_isValid(const struct ParsedOWidth* parsedWidth);

#endif
