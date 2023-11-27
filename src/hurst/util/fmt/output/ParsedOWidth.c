#include "ParsedOWidth.h"

bool ParsedOWidth_isValid(const struct ParsedOWidth* parsedWidth) {
    if (!parsedWidth)
        return false;

    if (parsedWidth->valid)
        return parsedWidth->len;

    return !parsedWidth->len
        && !parsedWidth->value;
}
