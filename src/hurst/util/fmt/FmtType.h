#ifndef HURST_UTIL_FMT_FMTTYPE_H
#define HURST_UTIL_FMT_FMTTYPE_H

#include <stdbool.h>
#include <stddef.h>

#include "ParsedFmtType.h"

enum {
    FMT_TYPE_MAX_LEN = 4,
};

enum FmtType {
    FMT_TYPE_BOOL,         // b

    FMT_TYPE_QUOTED_CHAR,  // 'c
    FMT_TYPE_DQUOTED_CHAR, // "c
    FMT_TYPE_ESC_CHAR,     // ec
    FMT_TYPE_CHAR,         // c

    FMT_TYPE_QUOTED_STR,   // 's
    FMT_TYPE_DQUOTED_STR,  // "s
    FMT_TYPE_ESC_STR,      // es
    FMT_TYPE_STR,          // s

    FMT_TYPE_SCHAR,        // sc
    FMT_TYPE_UCHAR,        // uc
    FMT_TYPE_SINT,         // Si?
    FMT_TYPE_USINT,        // uSi?
    FMT_TYPE_INT,          // i
    FMT_TYPE_UINT,         // ui?
    FMT_TYPE_LINT,         // li?
    FMT_TYPE_ULINT,        // uli?
    FMT_TYPE_LLINT,        // lli?
    FMT_TYPE_ULLINT,       // ulli?
    FMT_TYPE_PTRDIFF,      // D
    FMT_TYPE_SIZE,         // z
    FMT_TYPE_MAX,          // m
    FMT_TYPE_UMAX,         // um

    FMT_TYPE_FLOAT,        // f
    FMT_TYPE_DOUBLE,       // d

    FMT_TYPE_TIMESTAMP,    // t
};

struct ParsedFmtType FmtType_Parse(const char* str);
struct ParsedFmtType FmtType_ParseN(const char* str, size_t strLen);

const char* FmtType_getName(enum FmtType type);
bool FmtType_isValid(enum FmtType type);

#endif
