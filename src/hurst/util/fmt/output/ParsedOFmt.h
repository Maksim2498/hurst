#ifndef HURST_UTIL_FMT_OUTPUT_PARSEDOFMT_H
#define HURST_UTIL_FMT_OUTPUT_PARSEDOFMT_H

#include <stdbool.h>
#include <stddef.h>

#include "OFmt_fwd.h"

struct ParsedOFmt {
    size_t      len;
    struct OFmt value;
    bool        valid;
};

bool ParsedOFmt_isValid(const struct ParsedOFmt* parsedFmt);

#endif
