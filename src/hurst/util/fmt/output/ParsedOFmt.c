#include "ParsedOFmt.h"

#include "OFmt.h"

bool ParsedOFmt_isValid(const struct ParsedOFmt* parsedFmt) {
    if (!parsedFmt)
        return false;

    if (parsedFmt->valid)
        return parsedFmt->len
            && OFmt_isValid(&parsedFmt->value);

    return !parsedFmt->len;
}
