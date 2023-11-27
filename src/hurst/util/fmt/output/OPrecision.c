#include "OPrecision.h"

#include <assert.h>

#include <hurst/util/str.h>

#include "OWidth.h"

struct ParsedOPrecision OPrecision_Parse(const char* str, ...) {
    va_list args;

    va_start(args, str);

    struct ParsedOPrecision parsed = OPrecision_ParseV(str, &args);

    va_end(args);

    return parsed;
}

struct ParsedOPrecision OPrecision_ParseN(const char* str, size_t n, ...) {
    va_list args;

    va_start(args, n);

    struct ParsedOPrecision parsed = OPrecision_ParseNV(str, n, &args);

    va_end(args);

    return parsed;
}

struct ParsedOPrecision OPrecision_ParseV(const char* str, va_list* args) {
    return OPrecision_ParseNV(str, evalStrLen(str), args);
}

struct ParsedOPrecision OPrecision_ParseNV(const char* str, size_t n, va_list* args) {
    assert(str && args);
    
    size_t value = 0;
    size_t len   = 0;
    bool   valid = false;

    if (n > 2 && '.' == str[0]) {
        const struct ParsedOWidth parsedWidth = OWidth_ParseNV(str + 1, n - 1, args);

        value = parsedWidth.value;
        valid = parsedWidth.valid;
        len   = valid ? parsedWidth.len + 1 : 0;
    }

    return (struct ParsedOPrecision) {
        .value = value,
        .len   = len,
        .valid = valid,
    };
}
