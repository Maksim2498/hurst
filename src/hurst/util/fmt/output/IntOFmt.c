#include "IntOFmt.h"

const struct IntOFmt INT_OFMT_BIN = {
    .groupSep    = NULL,
    .groupSepLen = 0,
    .groupSize   = 3,
    .precision   = 0,
    .width       = 0,
    .radix       = 2,
    .digitCase   = OCASE_UPPER,
    .plus        = 0,
    .minus       = '-',
    .filler      = '0',
    .groupFiller = true,
};

const struct IntOFmt INT_OFMT_OCT = {
    .groupSep    = NULL,
    .groupSepLen = 0,
    .groupSize   = 3,
    .precision   = 0,
    .width       = 0,
    .radix       = 8,
    .digitCase   = OCASE_UPPER,
    .plus        = 0,
    .minus       = '-',
    .filler      = '0',
    .groupFiller = true,
};

const struct IntOFmt INT_OFMT_DEC   = {
    .groupSep    = NULL,
    .groupSepLen = 0,
    .groupSize   = 3,
    .precision   = 0,
    .width       = 0,
    .radix       = 10,
    .digitCase   = OCASE_UPPER,
    .plus        = 0,
    .minus       = '-',
    .filler      = '0',
    .groupFiller = true,
};

const struct IntOFmt INT_OFMT_HEX   = {
    .groupSep    = NULL,
    .groupSepLen = 0,
    .groupSize   = 3,
    .precision   = 0,
    .width       = 0,
    .radix       = 16,
    .digitCase   = OCASE_UPPER,
    .plus        = 0,
    .minus       = '-',
    .filler      = '0',
    .groupFiller = true,
};

bool IntOFmt_isValid(const struct IntOFmt* fmt) {
    return fmt
        && Radix_isValid(fmt->radix)
        && (fmt->radix <= 10 || OCase_isValid(fmt->digitCase));
}
