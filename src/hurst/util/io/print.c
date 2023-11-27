#include "print.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/fmt/output/OCase.h>
#include <hurst/util/fmt/FmtType.h>
#include <hurst/util/macros.h>
#include <hurst/util/str.h>


typedef size_t (*PrintStr_)(FILE* file, const char*);
typedef size_t (*PrintStrN_)(FILE* file, const char*, size_t);

typedef size_t (*PrintChar_)(FILE* file, char);
typedef size_t (*PrintCharN_)(FILE* file, char, size_t);


static size_t printFmtValueArrayV_(FILE* file, const struct OFmt* fmt, va_list* args);
static size_t printFmtValueValueV_(FILE* file, const struct OFmt* fmt, va_list* args);

static size_t printFmtValueStrV_(
    FILE*              file,
    const struct OFmt* fmt,
    va_list*           args,
    PrintStr_          print,
    PrintStrN_         printN
);

static size_t printFmtValueCharV_(
    FILE*              file,
    const struct OFmt* fmt,
    va_list*           args,
    PrintChar_         print,
    PrintCharN_        printN
);

static size_t printFmtValueIntV_(FILE* file, intmax_t i, const struct OFmt* fmt);
static size_t printFmtValueUIntV_(FILE* file, intmax_t i, const struct OFmt* fmt);

static size_t printUIntMaxMagnitude_(FILE* file, uintmax_t i, const struct IntOFmt* fmt);
static unsigned writeUIntMaxMagnitudeToBuffer_(uintmax_t i, char* buffer, const struct IntOFmt* fmt);
static size_t printUIntMaxFiller_(FILE* file, unsigned bufferLen, const struct IntOFmt* fmt);
static size_t printUIntMaxBuffer_(FILE* file, const char* buffer, unsigned bufferLen, const struct IntOFmt* fmt);


size_t printArray(
    FILE*       file,
    const void* items,
    size_t      itemCount,
    size_t      itemSize,
    Print       print
) {
    return printArrayFmt(file, items, itemCount, itemSize, print, &ARRAY_OFMT_DEFAULT);
}

size_t printArrayFmt(
    FILE*                   file,
    const void*             items,
    size_t                  itemCount,
    size_t                  itemSize,
    Print                   print,
    const struct ArrayOFmt* fmt
) {
    assert(itemSize && print && fmt);

    size_t printed = 0;

    printed += printFilteredStrN(file, fmt->open, fmt->openLen, "\\", 1);

    if (items)
        for (size_t i = 0; i < itemCount; ++i) {
            const void* element = (const char*) items + i * itemSize;

            printed += print(file, element);

            if (i + 1 != itemCount)
                printed += printFilteredStrN(file, fmt->sep, fmt->sepLen, "\\", 1);
        }

    printed += printFilteredStrN(file, fmt->close, fmt->closeLen, "\\", 1);

    return printed;
}

size_t printFmt(FILE* file, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);

    size_t printed = printFmtV(file, fmt, &args);

    va_end(args);

    return printed;
}

size_t printFmtN(FILE* file, const char* fmt, size_t n, ...) {
    va_list args;

    va_start(args, n);

    size_t printed = printFmtNV(file, fmt, n, &args);

    va_end(args);

    return printed;
}

size_t printFmtV(FILE* file, const char* fmt, va_list* args) {
    return printFmtNV(file, fmt, evalStrLen(fmt), args);
}

size_t printFmtNV(FILE* file, const char* fmt, size_t n, va_list* args) {
    assert(fmt && args);

    size_t printed = 0;

    for (size_t i = 0; i < n;) {
        const char c = fmt[i++];

        if ('%' != c || i >= n) {
            printed += printChar(file, c);
            continue;
        }

        const struct ParsedOFmt parsedFmt = OFmt_ParseNV(fmt + i, n - i, args);

        if (!parsedFmt.valid)
            continue;

        printed += printFmtValueV(file, &parsedFmt.value, args);
        i       += parsedFmt.len;
    }

    return printed;
}

size_t printFmtValue(FILE* file, const struct OFmt* fmt, ...) {
    va_list args;

    va_start(args, fmt);

    size_t printed = printFmtValueV(file, fmt, &args);

    va_end(args);

    return printed;
}

size_t printFmtValueV(FILE* file, const struct OFmt* fmt, va_list* args) {
    assert(OFmt_isValid(fmt) && args);

    size_t printed = fmt->isArray ? printFmtValueArrayV_(file, fmt, args)
                                  : printFmtValueValueV_(file, fmt, args);

    if (printed < fmt->width) {
        size_t toFill = fmt->width - printed;

        printed += printCharN(file, fmt->filler, toFill);
    }

    return printed;
}

size_t printFmtValueArrayV_(FILE* file, const struct OFmt* fmt, va_list* args) {
    assert(OFmt_isValid(fmt) && fmt->isArray && args);

    Print  print;
    size_t itemSize;

    switch (fmt->type) {
        default:
            assert(0);
            return 0;

        case FMT_TYPE_BOOL:
            print    = (Print) printBoolPtr;
            itemSize = sizeof(bool);
            break;

        case FMT_TYPE_QUOTED_CHAR:
            print    = (Print) printQuotedCharPtr;
            itemSize = sizeof(char);
            break;

        case FMT_TYPE_DQUOTED_CHAR:
            print    = (Print) printDQuotedCharPtr;
            itemSize = sizeof(char);
            break;

        case FMT_TYPE_ESC_CHAR:
            print    = (Print) printEscCharPtr;
            itemSize = sizeof(char);
            break;

        case FMT_TYPE_CHAR:
            print    = (Print) printCharPtr;
            itemSize = sizeof(char);
            break;

        case FMT_TYPE_QUOTED_STR:
            print    = (Print) printQuotedStrPtr;
            itemSize = sizeof(const char*);
            break;

        case FMT_TYPE_DQUOTED_STR:
            print    = (Print) printDQuotedStrPtr;
            itemSize = sizeof(const char*);
            break;

        case FMT_TYPE_ESC_STR:
            print    = (Print) printEscStrPtr;
            itemSize = sizeof(const char*);
            break;

        case FMT_TYPE_STR:
            print    = (Print) printStrPtr;
            itemSize = sizeof(const char*);
            break;

        case FMT_TYPE_SCHAR:
            print    = (Print) printSCharPtr;
            itemSize = sizeof(signed char);
            break;

        case FMT_TYPE_UCHAR:
            print    = (Print) printUCharPtr;
            itemSize = sizeof(unsigned char);
            break;

        case FMT_TYPE_SINT:
            print    = (Print) printSIntPtr;
            itemSize = sizeof(short);
            break;

        case FMT_TYPE_USINT:
            print    = (Print) printUSIntPtr;
            itemSize = sizeof(unsigned short);
            break;
        
        case FMT_TYPE_INT:
            print    = (Print) printIntPtr;
            itemSize = sizeof(int);
            break;

        case FMT_TYPE_UINT:
            print    = (Print) printUIntPtr;
            itemSize = sizeof(unsigned);
            break;

        case FMT_TYPE_LINT:
            print    = (Print) printLIntPtr;
            itemSize = sizeof(long);
            break;

        case FMT_TYPE_ULINT:
            print    = (Print) printULIntPtr;
            itemSize = sizeof(unsigned long);
            break;

        case FMT_TYPE_LLINT:
            print    = (Print) printLLIntPtr;
            itemSize = sizeof(long long);
            break;

        case FMT_TYPE_ULLINT:
            print    = (Print) printULLIntPtr;
            itemSize = sizeof(unsigned long long);
            break;

        case FMT_TYPE_PTRDIFF:
            print    = (Print) printPtrDiffPtr;
            itemSize = sizeof(ptrdiff_t);
            break;

        case FMT_TYPE_SIZE:
            print    = (Print) printSizePtr;
            itemSize = sizeof(size_t);
            break;

        case FMT_TYPE_MAX:
            print    = (Print) printIntMaxPtr;
            itemSize = sizeof(intmax_t);
            break;

        case FMT_TYPE_UMAX:
            print    = (Print) printUIntMaxPtr;
            itemSize = sizeof(uintmax_t);
            break;

        case FMT_TYPE_FLOAT:
            print    = (Print) printFloatPtr;
            itemSize = sizeof(float);
            break;

        case FMT_TYPE_DOUBLE:
            print    = (Print) printDoublePtr;
            itemSize = sizeof(double);
            break;

        case FMT_TYPE_TIMESTAMP:
            print    = (Print) printTimestampPtr;
            itemSize = sizeof(Timestamp);
            break;
    }

    const void* items     = va_arg(*args, const void*);
    size_t      itemCount = fmt->usePrecision ? fmt->precision : 1;

    return printArrayFmt(file, items, itemCount, itemSize, print, &fmt->arrayFmt);
}

size_t printFmtValueValueV_(FILE* file, const struct OFmt* fmt, va_list* args) {
    assert(OFmt_isValid(fmt) && !fmt->isArray && args);

    switch (fmt->type) {
        default:
            assert(false);
            return 0;

        case FMT_TYPE_BOOL: {
            const bool value = va_arg(*args, int);

            return printBool(file, value);
        }

        case FMT_TYPE_QUOTED_CHAR:
            return printFmtValueCharV_(file, fmt, args, printQuotedChar, printQuotedCharN);

        case FMT_TYPE_DQUOTED_CHAR:
            return printFmtValueCharV_(file, fmt, args, printDQuotedChar, printDQuotedCharN);

        case FMT_TYPE_ESC_CHAR:
            return printFmtValueCharV_(file, fmt, args, printEscChar, printEscCharN);

        case FMT_TYPE_CHAR:
            return printFmtValueCharV_(file, fmt, args, printChar, printCharN);

        case FMT_TYPE_QUOTED_STR:
            return printFmtValueStrV_(file, fmt, args, printQuotedStr, printQuotedStrN);

        case FMT_TYPE_DQUOTED_STR:
            return printFmtValueStrV_(file, fmt, args, printDQuotedStr, printDQuotedStrN);

        case FMT_TYPE_ESC_STR:
            return printFmtValueStrV_(file, fmt, args, printEscStr, printEscStrN);

        case FMT_TYPE_STR:
            return printFmtValueStrV_(file, fmt, args, printStr, printStrN);

        case FMT_TYPE_SCHAR:
        case FMT_TYPE_SINT:
        case FMT_TYPE_INT: {
            const int value = va_arg(*args, int);
            
            return printFmtValueIntV_(file, value, fmt);
        }

        case FMT_TYPE_UCHAR:
        case FMT_TYPE_USINT:
        case FMT_TYPE_UINT: {
            const unsigned value = va_arg(*args, unsigned);

            return printFmtValueUIntV_(file, value, fmt);
        }

        case FMT_TYPE_LINT: {
            const long value = va_arg(*args, long);

            return printFmtValueIntV_(file, value, fmt);
        }

        case FMT_TYPE_ULINT: {
            const unsigned long value = va_arg(*args, unsigned long);

            return printFmtValueUIntV_(file, value, fmt);
        }

        case FMT_TYPE_LLINT: {
            const long long value = va_arg(*args, long long);

            return printFmtValueIntV_(file, value, fmt);
        }

        case FMT_TYPE_ULLINT: {
            const unsigned long long value = va_arg(*args, unsigned long long);

            return printFmtValueUIntV_(file, value, fmt);
        }

        case FMT_TYPE_PTRDIFF: {
            const ptrdiff_t value = va_arg(*args, ptrdiff_t);

            return printFmtValueIntV_(file, value, fmt);
        }

        case FMT_TYPE_SIZE: {
            const size_t value = va_arg(*args, size_t);

            return printFmtValueUIntV_(file, value, fmt);
        }

        case FMT_TYPE_MAX: {
            const intmax_t value = va_arg(*args, intmax_t);

            return printFmtValueIntV_(file, value, fmt);
        }

        case FMT_TYPE_UMAX: {
            const uintmax_t value = va_arg(*args, uintmax_t);

            return printFmtValueUIntV_(file, value, fmt);
        }

        case FMT_TYPE_FLOAT:
        case FMT_TYPE_DOUBLE: {
            const double value = va_arg(*args, double);

            return printFloat(file, value);
        }

        case FMT_TYPE_TIMESTAMP: {
            const Timestamp value = va_arg(*args, Timestamp);

            return printTimestamp(file, value);
        }
    }
}

size_t printFmtValueStrV_(
    FILE*              file,
    const struct OFmt* fmt,
    va_list*           args,
    PrintStr_          print,
    PrintStrN_         printN
) {
    assert(OFmt_isValid(fmt) && !fmt->isArray && args && print && printN);

    const char* value = va_arg(*args, const char*);

    if (fmt->usePrecision)
        return printN(file, value, fmt->precision);
    else
        return print(file, value);
}

static size_t printFmtValueCharV_(
    FILE*              file,
    const struct OFmt* fmt,
    va_list*           args,
    PrintChar_         print,
    PrintCharN_        printN
) {
    assert(OFmt_isValid(fmt) && !fmt->isArray && args && print && printN);

    const char value = va_arg(*args, int);

    if (fmt->usePrecision)
        return printN(file, value, fmt->precision);
    else
        return print(file, value);
}

size_t printFmtValueIntV_(FILE* file, intmax_t i, const struct OFmt* fmt) {
    struct IntOFmt intFmt = OFmt_toIntOFmt(fmt);

    return printIntFmt(file, i, &intFmt);
}

size_t printFmtValueUIntV_(FILE* file, intmax_t i, const struct OFmt* fmt) {
    struct IntOFmt intFmt = OFmt_toIntOFmt(fmt);

    return printIntFmt(file, i, &intFmt);
}

size_t printBoolPtr(FILE* file, const bool* b) {
    assert(b);
    return printBool(file, *b);
}

size_t printBool(FILE* file, bool b) {
    return printStr(file, b ? "true" : "false");
}

size_t printSCharPtr(FILE* file, const signed char* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printSIntPtr(FILE* file, const short* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printIntPtr(FILE* file, const int* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printLIntPtr(FILE* file, const long* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printLLIntPtr(FILE* file, const long long* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printPtrDiffPtr(FILE* file, const ptrdiff_t* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printIntMaxPtr(FILE* file, const intmax_t* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printInt(FILE* file, intmax_t i) {
    return printIntFmt(file, i, &INT_OFMT_DEC);
}

size_t printIntFmt(FILE* file, intmax_t i, const struct IntOFmt* fmt) {
    assert(IntOFmt_isValid(fmt));

    size_t printed = 0;

    if (i < 0) {
        if (fmt->minus)
            printed += printChar(file, fmt->minus);

        i = -i;
    } else if (i > 0 && fmt->plus)
        printed += printChar(file, fmt->plus);

    return printed += printUIntMaxMagnitude_(file, i, fmt);
}

size_t printUCharPtr(FILE* file, const unsigned char* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printUSIntPtr(FILE* file, const unsigned short* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printUIntPtr(FILE* file, const unsigned* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printULIntPtr(FILE* file, const unsigned long* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printULLIntPtr(FILE* file, const unsigned long long* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printSizePtr(FILE* file, const size_t* i) {
    assert(i);
    return printInt(file, *i);
}

size_t printUIntMaxPtr(FILE* file, const uintmax_t* i) {
    assert(i);
    return printUInt(file, *i);
}

size_t printUInt(FILE* file, uintmax_t i) {
    return printUIntFmt(file, i, &INT_OFMT_DEC);
}

size_t printUIntFmt(FILE* file, uintmax_t i, const struct IntOFmt* fmt) {
    assert(IntOFmt_isValid(fmt));

    size_t printed = 0;

    if (i > 0 && fmt->plus)
        printed += printChar(file, fmt->plus);

    return printed += printUIntMaxMagnitude_(file, i, fmt);
}

size_t printUIntMaxMagnitude_(FILE* file, uintmax_t i, const struct IntOFmt* fmt) {
    assert(IntOFmt_isValid(fmt));

    enum {
        // Binary form is the longest possible
        // for positional number systems
        BUFFER_SIZE = CHAR_BIT * sizeof(intmax_t),
    };

    char     buffer[BUFFER_SIZE];
    unsigned bufferLen = writeUIntMaxMagnitudeToBuffer_(i, buffer, fmt);

    size_t printed = 0;

    printed += printUIntMaxFiller_(file, bufferLen, fmt);
    printed += printUIntMaxBuffer_(file, buffer, bufferLen, fmt);

    return printed;
}

unsigned writeUIntMaxMagnitudeToBuffer_(uintmax_t i, char* buffer, const struct IntOFmt* fmt) {
    assert(buffer && IntOFmt_isValid(fmt));

    unsigned len = 0;

    char letterDigitOffset = OCASE_UPPER == fmt->digitCase ? 'A' : 'a';

    do {
        unsigned digit = i % fmt->radix;

        char c = digit < 10 ? '0'               + digit
                            : letterDigitOffset + digit - 10;

        buffer[len++] = c;
        
        i /= fmt->radix;
    } while (i);

    for (unsigned i = 0, j = len - 1; i < len / 2; ++i, --j) {
        char tmp = buffer[i];

        buffer[i] = buffer[j];
        buffer[j] = tmp;
    }

    if (fmt->precision)
        for (unsigned i = fmt->precision; i < len; ++i)
            buffer[i] = '0';

    return len;
}

size_t printUIntMaxFiller_(FILE* file, unsigned bufferLen, const struct IntOFmt* fmt) {
    assert(IntOFmt_isValid(fmt));

    if (!fmt->filler)
        return 0;

    size_t len = bufferLen;

    if (fmt->groupSep) {
        const unsigned groupCount = (len - 1) / fmt->groupSize;

        len += groupCount * fmt->groupSepLen;
    }

    const ptrdiff_t toFill = fmt->width - len;

    if (toFill <= 0)
        return 0;

    if (!fmt->groupFiller || !fmt->groupSep)
        return printCharN(file, fmt->filler, toFill);

    size_t printed = 0;

    printed += printChar(file, fmt->filler);

    len += toFill;

    const unsigned incGroupSize = fmt->groupSize + 1;

    for (size_t i = 1, j = len - 1; i < (size_t) toFill; ++i, --j)
        printed += j % incGroupSize ? printChar(file, fmt->filler)
                                    : printStrN(file, fmt->groupSep, fmt->groupSepLen);

    return printed;
}

size_t printUIntMaxBuffer_(FILE* file, const char* buffer, unsigned bufferLen, const struct IntOFmt* fmt) {
    assert(buffer && IntOFmt_isValid(fmt));

    if (!fmt->groupSep)
        return printStrN(file, buffer, bufferLen);

    size_t printed = 0;

    for (unsigned i = 0, j = bufferLen - 1; i < bufferLen; ++i, --j) {
        printed += printChar(file, buffer[i]);

        if (j % fmt->groupSize || !j)
            continue;

        printed += printStrN(file, fmt->groupSep, fmt->groupSepLen);
    }

    return printed;
}

size_t printFloatPtr(FILE* file, const float* f) {
    assert(f);
    return printFloat(file, *f);
}

size_t printDoublePtr(FILE* file, const double* f) {
    assert(f);
    return printFloat(file, *f);
}

size_t printFloat(FILE* file, double f) {
    assert(file);

    int printed = fprintf(file, "%f", f);

    return printed >= 0 ? printed : 0;
}

size_t printFilteredStr(FILE* file, const char* str, const char* ignore) {
    return printFilteredStrN(file, str, evalStrLen(str), ignore, evalStrLen(ignore));
}

size_t printFilteredStrN(
    FILE*       file,
    const char* str,
    size_t      strLen,
    const char* ignore,
    size_t      ignoreLen
) {
    assert(file && ignore);

    size_t printed = 0;

    for (size_t i = 0; i < strLen; ++i) {
        const char c = str[i];

        bool print = true;

        for (size_t j = 0; j < ignoreLen; ++j)
            if (ignore[j] == c) {
                print = false;
                break;
            }
        
        if (print)
            printed += printChar(file, c);
    }

    return printed;
}

size_t printStrPtr(FILE* file, const char** str) {
    assert(str);
    return printStr(file, *str);
}

size_t printStr(FILE* file, const char* str) {
    assert(file);

    size_t printed = 0;

    for (char c; (c = *str); ++str) {
        if (fputc(c, file) == EOF)
            break;

        ++printed;
    }

    return printed;
}

size_t printStrN(FILE* file, const char* str, size_t n) {
    assert(file);

    size_t printed = 0;

    for (size_t i = 0; i < n; ++i) {
        if (fputc(str[i], file) == EOF)
            break;

        ++printed;
    }

    return printed;
}

size_t printCharPtr(FILE* file, const char* c) {
    assert(c);
    return printChar(file, *c);
}

size_t printChar(FILE* file, char c) {
    assert(file);
    return fputc(c, file) == c;
}

size_t printCharN(FILE* file, char c, size_t n) {
    assert(file);

    size_t printed = 0;

    while (n--) {
        if (fputc(c, file) == EOF)
            break;

        ++printed;
    }

    return printed;
}

size_t printEscStrPtr(FILE* file, const char** str) {
    assert(str);
    return printEscStr(file, *str);
}

size_t printEscStr(FILE* file, const char* str) {
    assert(str);

    size_t printed = 0;

    while (*str)
        printed += printEscChar(file, *str++);

    return printed;
}

size_t printEscStrN(FILE* file, const char* str, size_t n) {
    assert(str);

    size_t printed = 0;

    for (size_t i = 0; i < n; ++i)
        printed += printEscChar(file, str[i]);

    return printed;
}

size_t printEscCharPtr(FILE* file, const char* c) {
    assert(c);
    return printEscChar(file, *c);
}

size_t printEscChar(FILE* file, char c) {
    switch (c) {
        case '\a':
            return printStr(file, "\\a");

        case '\b':
            return printStr(file, "\\b");

        case '\f':
            return printStr(file, "\\f");

        case '\n':
            return printStr(file, "\\n");

        case '\r':
            return printStr(file, "\\r");

        case '\t':
            return printStr(file, "\\t");

        case '\v':
            return printStr(file, "\\v");

        case '\\':
            return printStr(file, "\\\\");

        case '\'':
            return printStr(file, "\\'");

        case '"':
            return printStr(file, "\\\"");

        default:
            if (isprint(c))
                return printChar(file, c);
                
            return printStr(file, "\\x") + printUIntFmt(file, c, &INT_OFMT_HEX);
    }
}

size_t printEscCharN(FILE* file, char c, size_t n) {
    size_t printed = 0;

    while (n--)
        printed += printEscChar(file, c);

    return printed;
}

size_t printQuotedStrPtr(FILE* file, const char** str) {
    assert(str);
    return printQuotedStr(file, *str);
}

size_t printQuotedStr(FILE* file, const char* str) {
    return printEnclosedStr(file, str, '\'', '\'');
}

size_t printQuotedStrN(FILE* file, const char* str, size_t n) {
    return printEnclosedStrN(file, str, n, '\'', '\'');
}

size_t printQuotedCharPtr(FILE* file, const char* c) {
    assert(c);
    return printQuotedChar(file, *c);
}

size_t printQuotedChar(FILE* file, char c) {
    return printEnclosedChar(file, c, '\'', '\'');
}

size_t printQuotedCharN(FILE* file, char c, size_t n) {
    return printEnclosedCharN(file, c, n, '\'', '\'');
}

size_t printDQuotedStrPtr(FILE* file, const char** str) {
    assert(str);
    return printDQuotedStr(file, *str);
}

size_t printDQuotedStr(FILE* file, const char* str) {
    return printEnclosedStr(file, str, '"', '"');
}

size_t printDQuotedStrN(FILE* file, const char* str, size_t n) {
    return printEnclosedStrN(file, str, n, '"', '"');
}

size_t printDQuotedCharPtr(FILE* file, const char* c) {
    assert(c);
    return printDQuotedChar(file, *c);
}

size_t printDQuotedChar(FILE* file, char c) {
    return printEnclosedChar(file, c, '"', '"');
}

size_t printDQuotedCharN(FILE* file, char c, size_t n) {
    return printEnclosedCharN(file, c, n, '"', '"');
}

size_t printEnclosedStr(FILE* file, const char* str, char open, char close) {
    return printChar(file, open) + printEscStr(file, str) + printChar(file, close);
}

size_t printEnclosedStrN(FILE* file, const char* str, size_t n, char open, char close) {
    return printChar(file, open) + printEscStrN(file, str, n) + printChar(file, close);
}

size_t printEnclosedChar(FILE* file, char c, char open, char close) {
    return printChar(file, open) + printEscChar(file, c) + printChar(file, close);
}

size_t printEnclosedCharN(FILE* file, char c, size_t n, char open, char close) {
    return printChar(file, open) + printEscCharN(file, c, n) + printChar(file, close);
}

size_t printTimestampPtr(FILE* file, const Timestamp* ts) {
    assert(ts);
    return printTimestamp(file, *ts);
}

size_t printTimestamp(FILE* file, Timestamp ts) {
    static const struct IntOFmt TWO_WIDTH_INT_FMT = {
        .radix  = 10,
        .width  = 2,
        .filler = '0',
    };

    const int64_t  year  = Timestamp_getYear(ts);
    const unsigned month = Timestamp_getMonthOfYear(ts);
    const unsigned day   = Timestamp_getDayOfMonth(ts);
    const unsigned hour  = Timestamp_getHourOfDay(ts);
    const unsigned min   = Timestamp_getMinOfHour(ts);
    const unsigned sec   = Timestamp_getSecOfMin(ts);

    return printInt(file, year)
         + printChar(file, '-')
         + printUIntFmt(file, month, &TWO_WIDTH_INT_FMT)
         + printChar(file, '-')
         + printUIntFmt(file, day, &TWO_WIDTH_INT_FMT)
         + printChar(file, ' ')
         + printUIntFmt(file, hour, &TWO_WIDTH_INT_FMT)
         + printChar(file, ':')
         + printUIntFmt(file, min, &TWO_WIDTH_INT_FMT)
         + printChar(file, ':')
         + printUIntFmt(file, sec, &TWO_WIDTH_INT_FMT);
}
