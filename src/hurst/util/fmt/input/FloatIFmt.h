#ifndef HURST_UTIL_FMT_INPUT_FLOATIFMT_H
#define HURST_UTIL_FMT_INPUT_FLOATIFMT_H

#include <stdbool.h>

struct FloatIFmt {
    bool allowSign;
    bool allowSpacesBeforeSign;
    bool allowSpacesBeforeInt;
    bool allowSpacesAfterInt;
    bool allowSpacesBeforeFract;
    bool allowSpacesAfterFract;
};

extern const struct FloatIFmt FLOAT_IFMT_DEFAULT;

#endif
