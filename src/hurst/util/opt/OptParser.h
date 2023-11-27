#ifndef HURST_UTIL_OPT_OPTPARSER_H
#define HURST_UTIL_OPT_OPTPARSER_H

#include <stdbool.h>
#include <stddef.h>

#include "Opt.h"
#include "ParsedOpt.h"

typedef struct {
    const struct Opt*  opts;
    size_t             optCount;

    const char* const* args;
    size_t             argCount;

    size_t             curArgIndex;
    size_t             curArgPos;
} OptParser;

void OptParser_init(
    OptParser*         parser,
    const char* const* args,
    size_t             argCount,
    const struct Opt*  opts,
    size_t             optCount
);

struct ParsedOpt OptParser_parseNext(OptParser* parser);
void OptParser_reset(OptParser* parser);

const char* const* OptParser_getArgs(const OptParser* parser);
size_t OptParser_getArgCount(const OptParser* parser);

const struct Opt* OptPaser_getOpts(const OptParser* parser);
size_t OptPaser_getOptCount(const OptParser* parser);

bool OptParser_isReachedEnd(const OptParser* parser);

bool OptParser_isValid(const OptParser* parser);

#endif
