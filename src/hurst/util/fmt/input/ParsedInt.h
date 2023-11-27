#ifndef HURST_UTIL_FMT_INPUT_PARSEDINT_H
#define HURST_UTIL_FMT_INPUT_PARSEDINT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct ParsedInt {
    intmax_t value;
    size_t   len;
    bool     valid;
    bool     fullMatch;
    bool     overflow;
};

bool ParsedInt_isValid(const struct ParsedInt* parsedInt);

#endif
