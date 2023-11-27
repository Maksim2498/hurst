#ifndef HURST_UTIL_FMT_INPUT_INTIFMT_H
#define HURST_UTIL_FMT_INPUT_INTIFMT_H

#include <stdbool.h>

#include <hurst/util/fmt/Radix.h>

struct IntIFmt {
    Radix radix;
    bool  allowSign;
    bool  allowSpacesBeforeSign;
    bool  allowSpacesBeforeValue;
    bool  allowSpacesAfterValue;
};

extern const struct IntIFmt INT_IFMT_DEFAULT;

bool IntIFmt_isValid(const struct IntIFmt* fmt);

#endif
