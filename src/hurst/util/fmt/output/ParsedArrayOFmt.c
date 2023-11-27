#include "ParsedArrayOFmt.h"

bool ParsedArrayOFmt_isValid(const struct ParsedArrayOFmt* arrayFmt) {
    return arrayFmt
        && arrayFmt->valid == (bool) arrayFmt->len;
}
