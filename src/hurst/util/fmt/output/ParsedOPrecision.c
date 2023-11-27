#include "ParsedOPrecision.h"

bool ParsedOPrecision_isValid(const struct ParsedOPrecision* parsedPrecision) {
    if (!parsedPrecision)
        return false;

    if (parsedPrecision->valid)
        return parsedPrecision->len > 2;

    return !parsedPrecision->len
        && !parsedPrecision->value;
}
