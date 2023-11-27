#ifndef HURST_UTIL_OPT_PARSEDOPT_H
#define HURST_UTIL_OPT_PARSEDOPT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "Opt.h"
#include "OptParseStatus.h"

struct ParsedOpt {
    size_t            argIndex;

    const char*       text;
    size_t            textLen;

    const struct Opt* opt;
    const char*       optArg;
    bool              optShort;

    unsigned char     status;
};

size_t printParsedOpt(FILE* file, const struct ParsedOpt* parsedOpt);

struct ParsedOptFmt {
    size_t startOffset;
    size_t fieldOffset;
    size_t endOffset;
};

extern const struct ParsedOptFmt PARSED_OPT_FMT_DEFAULT;

size_t printParsedOptFmt(
    FILE*                      file,
    const struct ParsedOpt*    parsedOpt,
    const struct ParsedOptFmt* fmt
);

bool ParsedOpt_isValid(const struct ParsedOpt* parsedOpt);

#endif
