#ifndef HURST_UTIL_IO_PRINT_H
#define HURST_UTIL_IO_PRINT_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <hurst/util/fmt/output/ArrayOFmt.h>
#include <hurst/util/fmt/output/OFmt.h>
#include <hurst/util/fmt/output/IntOFmt.h>
#include <hurst/util/time/Timestamp.h>

typedef size_t (*Print)(FILE*, const void*);

size_t printArray(
    FILE*       file,
    const void* items,
    size_t      elementCount,
    size_t      itemSize,
    Print       print
);

size_t printArrayFmt(
    FILE*                   file,
    const void*             items,
    size_t                  elementCount,
    size_t                  itemSize,
    Print                   print,
    const struct ArrayOFmt* fmt
);

/*
    Format syntax: '%'[<precision>]<conversion specifier>[<array format>]

    Precision only affects chars, strings, integers, and arrays.
    When applied to a char it sets number of
    char's repetitions. When applied to a string
    it sets string length. When applied to an array
    it specifies it's length. When applied to an
    integer it specifies number of digits printed
    as is, the rest will be printed as zero.

    When precision is specified as "*"
    it will be fetched from an argument list
    before format argument as an unsigned
    int. When precision is specified as
    "**" it will also be fetched from an
    argument list before format argument
    but, this time, as size_t.

    Every unrecognized conversion sprecifier is
    printed as is. For example, format string
    "Hello, %world!" will be printed as "Hello, world!".
    Note that even if a conversion specifier is
    unrecognized and precision is specified as
    "*" or "**" precision argument will be fetched
    from the stack.

    Array format, if specified, prescripts to
    treat corresponding argument as an array.
    Additionaly it's possible to specify array begin,
    element separator, and end. By default they are
    set to "[", ", ", and "]" respecively.

    Array format has the following syntax: '['<begin>';'<sep>';'<end>']'

    Every element inside braces can be omitted.

    +------------+--------------------+
    | Conversion | Argument           |
    | Specifier  | Type               |
    +------------+--------------------+
    | b          | bool               |
    | c          | char               |
    | s          | string             |
    | i          | int                |
    | ui?        | unsigned           |
    | li?        | long               |
    | uli?       | unsinged long      |
    | lli?       | long long          |
    | ulli?      | unsinged long long |
    | d          | ptrdiff_t          |
    | z          | size_t             |
    | m          | intmax_t           |
    | um         | uintmax_t          |
    +------------+--------------------+
*/

size_t printFmt(FILE* file, const char* fmt, ...);
size_t printFmtN(FILE* file, const char* fmt, size_t n, ...);
size_t printFmtV(FILE* file, const char* fmt, va_list* args);
size_t printFmtNV(FILE* file, const char* fmt, size_t n, va_list* args);

size_t printFmtValue(FILE* file, const struct OFmt* fmt, ...);
size_t printFmtValueV(FILE* file, const struct OFmt* fmt, va_list* args);


size_t printBoolPtr(FILE* file, const bool* b);
size_t printBool(FILE* file, bool b);


size_t printSCharPtr(FILE* file, const signed char* i);
size_t printSIntPtr(FILE* file, const short* i);
size_t printIntPtr(FILE* file, const int* i);
size_t printLIntPtr(FILE* file, const long* i);
size_t printLLIntPtr(FILE* file, const long long* i);
size_t printPtrDiffPtr(FILE* file, const ptrdiff_t* i);
size_t printIntMaxPtr(FILE* file, const intmax_t* i);
size_t printInt(FILE* file, intmax_t i);
size_t printIntFmt(FILE* file, intmax_t i, const struct IntOFmt* fmt);

size_t printUCharPtr(FILE* file, const unsigned char* i);
size_t printUSIntPtr(FILE* file, const unsigned short* i);
size_t printUIntPtr(FILE* file, const unsigned* i);
size_t printULIntPtr(FILE* file, const unsigned long* i);
size_t printULLIntPtr(FILE* file, const unsigned long long* i);
size_t printSizePtr(FILE* file, const size_t* i);
size_t printUIntMaxPtr(FILE* file, const uintmax_t* i);
size_t printUInt(FILE* file, uintmax_t i);
size_t printUIntFmt(FILE* file, uintmax_t i, const struct IntOFmt* fmt);


size_t printFloatPtr(FILE* file, const float* f);
size_t printDoublePtr(FILE* file, const double* f);
size_t printFloat(FILE* file, double f);


size_t printFilteredStr(FILE* file, const char* str, const char* ignore);
size_t printFilteredStrN(
    FILE*       file,
    const char* str,
    size_t      strLen,
    const char* ignore,
    size_t      ignoreLen
);


size_t printStrPtr(FILE* file, const char** str);
size_t printStr(FILE* file, const char* str);
size_t printStrN(FILE* file, const char* str, size_t n);

size_t printCharPtr(FILE* file, const char* c);
size_t printChar(FILE* file, char c);
size_t printCharN(FILE* file, char c, size_t n);


size_t printEscStrPtr(FILE* file, const char** str);
size_t printEscStr(FILE* file, const char* str);
size_t printEscStrN(FILE* file, const char* str, size_t n);

size_t printEscCharPtr(FILE* file, const char* c);
size_t printEscChar(FILE* file, char c);
size_t printEscCharN(FILE* file, char c, size_t n);


size_t printQuotedStrPtr(FILE* file, const char** str);
size_t printQuotedStr(FILE* file, const char* str);
size_t printQuotedStrN(FILE* file, const char* str, size_t n);

size_t printQuotedCharPtr(FILE* file, const char* c);
size_t printQuotedChar(FILE* file, char c);
size_t printQuotedCharN(FILE* file, char c, size_t n);


size_t printDQuotedStrPtr(FILE* file, const char** str);
size_t printDQuotedStr(FILE* file, const char* str);
size_t printDQuotedStrN(FILE* file, const char* str, size_t n);

size_t printDQuotedCharPtr(FILE* file, const char* c);
size_t printDQuotedChar(FILE* file, char c);
size_t printDQuotedCharN(FILE* file, char c, size_t n);


size_t printEnclosedStr(FILE* file, const char* str, char open, char close);
size_t printEnclosedStrN(FILE* file, const char* str, size_t n, char open, char close);

size_t printEnclosedChar(FILE* file, char c, char open, char close);
size_t printEnclosedCharN(FILE* file, char c, size_t n, char open, char close);


size_t printTimestampPtr(FILE* file, const Timestamp* ts);
size_t printTimestamp(FILE* file, Timestamp ts);

#endif
