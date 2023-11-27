#ifndef HURST_UTIL_FMT_OUTPUT_INTOFMT_H
#define HURST_UTIL_FMT_OUTPUT_INTOFMT_H

#include <stdbool.h>
#include <stddef.h>

#include <hurst/util/fmt/Radix.h>

#include "OCase.h"

struct IntOFmt {
    size_t        width;
    const char*   groupSep;    // Is not printed if null
    size_t        groupSepLen;
    unsigned char groupSize;
    unsigned char precision;   // Is ignored if null
    unsigned char radix;
    unsigned char digitCase;
    char          plus;        // Is not printed if null
    char          minus;       // Is not printed if null
    char          filler;      // Is not printed if null
    bool          groupFiller;
};

extern const struct IntOFmt INT_OFMT_BIN;
extern const struct IntOFmt INT_OFMT_OCT;
extern const struct IntOFmt INT_OFMT_DEC;
extern const struct IntOFmt INT_OFMT_HEX;

bool IntOFmt_isValid(const struct IntOFmt* fmt);

#endif
