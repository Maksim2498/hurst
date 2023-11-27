#include "ParsedFloat.h"

bool ParsedFloat_isValid(const struct ParsedFloat* parsedFloat) {
    return parsedFloat
        && parsedFloat->valid == (bool) parsedFloat->len;
}
