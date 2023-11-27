#ifndef HURST_UTIL_FMT_INPUT_PARSEDFLOAT_H
#define HURST_UTIL_FMT_INPUT_PARSEDFLOAT_H

#include <stdbool.h>
#include <stddef.h>

struct ParsedFloat {
    double value;
    size_t len;
    bool   valid;
    bool   fullMatch;
};

bool ParsedFloat_isValid(const struct ParsedFloat* parsedFloat);

#endif
