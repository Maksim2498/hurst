#include "str.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>

#include <hurst/util/macros.h>

size_t evalStrLen(const char* str) {
    assert(str);

    const char* initStr = str;

    while (*str)
        ++str;

    return str - initStr;
}

int cmpStrZZ(const char* lhs, const char* rhs) {
    assert(lhs && rhs);

    while (*lhs && *rhs) {
        const int diff = *lhs++ - *rhs++;

        if (diff)
            return SIGN(diff);
    }

    if (*lhs)
        return 1;

    if (*rhs)
        return -1;

    return 0;
}

int cmpStrZN(const char* lhs, const char* rhs, size_t rhsLen) {
    return -cmpStrNZ(rhs, rhsLen, lhs);
}

int cmpStrNZ(const char* lhs, size_t lhsLen, const char* rhs) {
    assert(lhs && rhs);

    size_t i = 0;

    while (i < lhsLen && *rhs) {
        const int diff = lhs[i++] - *rhs++;

        if (diff)
            return SIGN(diff);
    }

    if (i < lhsLen)
        return 1;

    if (*rhs)
        return -1;

    return 0;
}

int cmpStrNN(const char* lhs, size_t lhsLen, const char* rhs, size_t rhsLen) {
    assert(lhs && rhs);

    size_t i = 0;
    size_t j = 0;

    while (i < lhsLen && j < rhsLen) {
        const int diff = lhs[i++] - rhs[j++];

        if (diff)
            return SIGN(diff);
    }

    if (i < lhsLen)
        return 1;

    if (j < rhsLen)
        return -1;

    return 0;
}

const char* findCharInStr(char c, const char* str) {
    return findCharInStrN(c, str, evalStrLen(str));
}

const char* findCharInStrN(char c, const char* str, size_t n) {
    assert(str);

    for (const char *it = str, *end = it + n; it < end; ++it)
        if (*it == c)
            return it;

    return NULL;
}

struct ParsedInt parseIntStr(const char* str) {
    return parseIntStrFmt(str, &INT_IFMT_DEFAULT);
}

struct ParsedInt parseIntStrN(const char* str, size_t n) {
    return parseIntStrNFmt(str, n, &INT_IFMT_DEFAULT);
}

struct ParsedInt parseIntStrFmt(const char* str, const struct IntIFmt* fmt) {
    return parseIntStrNFmt(str, evalStrLen(str), fmt);
}

struct ParsedInt parseIntStrNFmt(const char* str, size_t n, const struct IntIFmt* fmt) {
    assert(str && IntIFmt_isValid(fmt));

    intmax_t value = 0;

    size_t len = 0;

    #define CONSUME_SPACES for (; len < n && isspace(str[len]); ++len)

    bool negative = false;

    if (fmt->allowSign) {
        if (fmt->allowSpacesBeforeSign)
            CONSUME_SPACES;

        if (len < n)
            switch (str[len]) {
                case '+':
                    ++len;
                    break;

                case '-':
                    negative = true;
                    ++len;
                    break;

                default:
                    break;
            }
    }

    if (fmt->allowSpacesBeforeValue)
        CONSUME_SPACES;

    bool valid    = false;
    bool overflow = false;

    for (; len < n; ++len) {
        const char c = str[len];

        int digit = -1;

        if (isdigit(c))
            digit = c - '0';
        else if (isupper(c))
            digit = c - 'A' + 10;
        else if (islower(c))
            digit = c - 'a' + 10;

        if (digit < 0 || digit >= (int) fmt->radix)
            break;

        intmax_t newValue = value * fmt->radix + digit;

        if (!overflow)
            overflow = newValue < value;

        value = newValue;
        valid = true;
    }

    if (valid && fmt->allowSpacesAfterValue)
        CONSUME_SPACES;

    #undef CONSUME_SPACES

    if (negative)
        value = -value;

    return (struct ParsedInt) {
        .value     = value,
        .len       = len,
        .valid     = valid,
        .fullMatch = len == n,
        .overflow  = overflow,
    };
}

struct ParsedFloat parseFloatStr(const char* str) {
    return parseFloatStrFmt(str, &FLOAT_IFMT_DEFAULT);
}

struct ParsedFloat parseFloatStrN(const char* str, size_t n) {
    return parseFloatStrNFmt(str, n, &FLOAT_IFMT_DEFAULT);
}

struct ParsedFloat parseFloatStrFmt(const char* str, const struct FloatIFmt* fmt) {
    return parseFloatStrNFmt(str, evalStrLen(str), fmt);
}

struct ParsedFloat parseFloatStrNFmt(const char* str, size_t n, const struct FloatIFmt* fmt) {
    // Algorithm used here produces imprecise
    // floating point values but is pretty
    // simple and easy to implement.

    assert(str && fmt);

    intmax_t  intPart   = 0;
    uintmax_t fractPart = 0;
    size_t    len       = 0;
    bool      valid     = false;

    const struct IntIFmt INT_FMT = {
        .radix                  = 10,
        .allowSign              = fmt->allowSign,
        .allowSpacesBeforeSign  = fmt->allowSpacesBeforeSign,
        .allowSpacesBeforeValue = fmt->allowSpacesBeforeInt,
        .allowSpacesAfterValue  = fmt->allowSpacesAfterInt,
    };
    
    const struct ParsedInt parsedIntPart = parseIntStrNFmt(str + len, n - len, &INT_FMT);

    if (!parsedIntPart.valid)
        goto quit;

    intPart  = parsedIntPart.value;
    len     += parsedIntPart.len;
    valid    = true;

    if (parsedIntPart.fullMatch)
        goto quit;

    if (str[len] != '.')
        goto quit;

    ++len;

    const struct IntIFmt FRACT_FMT = {
        .radix                  = 10,
        .allowSign              = false,
        .allowSpacesBeforeValue = fmt->allowSpacesBeforeFract,
        .allowSpacesAfterValue  = fmt->allowSpacesAfterFract,
    };

    const struct ParsedInt parsedFractPart = parseIntStrNFmt(str + len, n - len, &FRACT_FMT);

    if (!parsedFractPart.valid)
        goto quit;

    fractPart  = parsedFractPart.value;
    len       += parsedFractPart.len;

    quit: (void) 0;

    long double value = intPart;

    if (fractPart)
        value += fractPart / powl(10, floorl(1 + log10l(fractPart)));

    return (struct ParsedFloat) {
        .value     = value,
        .valid     = valid,
        .len       = len,
        .fullMatch = len == n,
    };
}
