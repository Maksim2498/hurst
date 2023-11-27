#include "IntIFmt.h"

const struct IntIFmt INT_IFMT_DEFAULT = {
    .radix                  = 10,
    .allowSign              = true,
    .allowSpacesBeforeSign  = true,
    .allowSpacesBeforeValue = true,
    .allowSpacesAfterValue  = true,
};

bool IntIFmt_isValid(const struct IntIFmt* fmt) {
    return fmt
        && Radix_isValid(fmt->radix);
}
