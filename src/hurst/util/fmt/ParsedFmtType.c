#include "ParsedFmtType.h"

#include "FmtType.h"

bool ParsedFmtType_isValid(struct ParsedFmtType parsedType) {
    return parsedType.valid

         ?  parsedType.len                     &&
            parsedType.len <= FMT_TYPE_MAX_LEN &&
            FmtType_isValid(parsedType.valid)

         :  !parsedType.len;
}
