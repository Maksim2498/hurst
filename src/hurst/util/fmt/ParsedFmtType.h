#ifndef HURST_UTIL_FMT_PARSEDFMTTYPE_H
#define HURST_UTIL_FMT_PARSEDFMTTYPE_H

#include <stdbool.h>

struct ParsedFmtType {
    unsigned char value;
    unsigned char len;
    bool          valid;
};

bool ParsedFmtType_isValid(struct ParsedFmtType parsedType);

#endif
