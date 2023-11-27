#include "OFmt.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/fmt/ParsedFmtType.h>
#include <hurst/util/str.h>

#include "ArrayOFmt.h"
#include "OPrecision.h"
#include "OWidth.h"
#include "ParsedArrayOFmt.h"
#include "ParsedOPrecision.h"
#include "ParsedOWidth.h"

struct ParsedOFmt OFmt_Parse(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);

    struct ParsedOFmt parsed = OFmt_ParseV(fmt, &args);

    va_end(args);

    return parsed;
}

struct ParsedOFmt OFmt_ParseN(const char* fmt, size_t n, ...) {
    va_list args;

    va_start(args, n);

    struct ParsedOFmt parsed = OFmt_ParseNV(fmt, n, &args);

    va_end(args);

    return parsed;
}

struct ParsedOFmt OFmt_ParseV(const char* fmt, va_list* args) {
    return OFmt_ParseNV(fmt, evalStrLen(fmt), args);
}

struct ParsedOFmt OFmt_ParseNV(const char* fmt, size_t n, va_list* args) {
    assert(fmt && args);

    size_t len = 0;

    const struct ParsedOWidth parsedWidth = OWidth_ParseNV(fmt + len, n - len, args);

    len += parsedWidth.len;

    const struct ParsedOPrecision parsedPrecision = OPrecision_ParseNV(fmt + len, n - len, args);

    len += parsedPrecision.len;

    struct ParsedFmtType parsedType = FmtType_ParseN(fmt + len, n - len);

    len += parsedType.len;

    struct ParsedArrayOFmt parsedArrayFmt = { .valid = false };
    
    if (parsedType.valid) {
        parsedArrayFmt = ArrayOFmt_ParseN(fmt + len, n - len);

        len += parsedArrayFmt.len;
    }

    return (struct ParsedOFmt) {
        .len   = len,
        .valid = parsedType.valid,
        .value = (struct OFmt) {
            .arrayFmt     = parsedArrayFmt.value,
            .isArray      = parsedArrayFmt.valid,

            .precision    = parsedPrecision.value,
            .usePrecision = parsedPrecision.valid,

            .type         = parsedType.value,

            .width        = parsedWidth.value,
            .filler       = ' ',
        },
    };
}

struct IntOFmt OFmt_toIntOFmt(const struct OFmt* fmt) {
    assert(OFmt_isValid(fmt));
    
    struct IntOFmt intFmt = INT_OFMT_DEC;

    if (fmt->usePrecision)
        intFmt.precision = fmt->precision;

    return intFmt;
}

bool OFmt_isValid(const struct OFmt* fmt) {
    return fmt
        && FmtType_isValid(fmt->type);
}
