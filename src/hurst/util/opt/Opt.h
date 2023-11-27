#ifndef HURST_UTIL_OPT_OPT_H
#define HURST_UTIL_OPT_OPT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct Opt {
    const char* longName;
    const char* description;
    char        shortName;
    bool        takesArg;
};

size_t printOpt(FILE* file, const struct Opt* opt);

struct OptFmt {
    size_t startOffset;
    size_t fieldOffset;
    size_t endOffset;
};

extern const struct OptFmt OPT_FMT_DEFAULT;

size_t printOptFmt(
    FILE*                file,
    const struct Opt*    opt,
    const struct OptFmt* fmt
);

size_t printOptsHelp(FILE* file, const struct Opt* opts, size_t optCount);

struct OptsHelpFmt {
    size_t offset;
    size_t descriptionPadding;
};

extern const struct OptsHelpFmt OPTS_HELP_FMT_DEFAULT;

size_t printOptsHelpFmt(
    FILE*                     file,
    const struct Opt*         opts,
    size_t                    optCount,
    const struct OptsHelpFmt* fmt
);

#endif
