#include "OWidth.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/str.h>

struct ParsedOWidth OWidth_Parse(const char* str, ...) {
    va_list args;

    va_start(args, str);

    struct ParsedOWidth parsed = OWidth_ParseV(str, &args);

    va_end(args);

    return parsed;
}

struct ParsedOWidth OWidth_ParseN(const char* str, size_t n, ...) {
    va_list args;

    va_start(args, n);

    struct ParsedOWidth parsed = OWidth_ParseNV(str, n, &args);

    va_end(args);

    return parsed;
}

struct ParsedOWidth OWidth_ParseV(const char* str, va_list* args) {
    return OWidth_ParseNV(str, evalStrLen(str), args);
}

struct ParsedOWidth OWidth_ParseNV(const char* str, size_t n, va_list* args) {
    assert(str && args);
    
    size_t value = 0;
    size_t len   = 0;
    bool   valid = false;

    if (n)
        switch (str[0]) {
            case '*':
                if (n >= 2 && '*' == str[1]) {
                    value = va_arg(*args, size_t);
                    len   = 2;
                } else {
                    value = va_arg(*args, unsigned);
                    len   = 1;
                }

                valid = true;

                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                value = str[0] - '0';
                len   = 1;
                valid = true;

                for (; len < n; ++len) {
                    char c = str[len];

                    if (!isdigit(c))
                        break;

                    value *= 10;
                    value += c - '0';
                }

                break;
            }
        }

    return (struct ParsedOWidth) {
        .value = value,
        .len   = len,
        .valid = valid,
    };
}
