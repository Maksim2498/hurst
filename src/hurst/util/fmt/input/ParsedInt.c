#include "ParsedInt.h"

bool ParsedInt_isValid(const struct ParsedInt* parsedInt) {
    return parsedInt
        && parsedInt->valid == (bool) parsedInt->len;
}
