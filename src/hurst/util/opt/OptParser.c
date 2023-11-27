#include "OptParser.h"

#include <assert.h>
#include <string.h>

#include <hurst/util/array.h>
#include <hurst/util/str.h>

static bool OptParser_isParsingShort_(const OptParser* parser);
static unsigned OptParser_readDashCount_(OptParser* parser);
static struct ParsedOpt OptParser_parseEnd_(OptParser* parser);
static struct ParsedOpt OptParser_parseNextPos_(OptParser* parser);
static struct ParsedOpt OptParser_parseNextShort_(OptParser* parser);
static struct ParsedOpt OptParser_parseNextLong_(OptParser* parser);


void OptParser_init(
    OptParser*         parser,
    const char* const* args,
    size_t             argCount,
    const struct Opt*  opts,
    size_t             optCount
) {
    assert(parser && allNotNull((const void* const*) args, argCount) && opts);

    parser->args        = args;
    parser->argCount    = argCount;

    parser->opts        = opts;
    parser->optCount    = optCount;

    parser->curArgIndex = 0;
    parser->curArgPos   = 0;
}

struct ParsedOpt OptParser_parseNext(OptParser* parser) {
    // Parsers is validated here
    if (OptParser_isReachedEnd(parser))
        return OptParser_parseEnd_(parser);
    
    if (OptParser_isParsingShort_(parser))
        return OptParser_parseNextShort_(parser);

    switch (OptParser_readDashCount_(parser)) {
        default:
            assert(false);

        case 0:
            return OptParser_parseNextPos_(parser);

        case 1:
            return OptParser_parseNextShort_(parser);

        case 2:
            return OptParser_parseNextLong_(parser);
    }
}

bool OptParser_isParsingShort_(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->curArgPos;
}

unsigned OptParser_readDashCount_(OptParser* parser) {
    unsigned dashCount = 0;

    const char* const arg = parser->args[parser->curArgIndex];

    while ('-' == arg[parser->curArgPos] && dashCount < 2) {
        ++parser->curArgPos;
        ++dashCount;
    }

    return dashCount;
}

struct ParsedOpt OptParser_parseEnd_(OptParser* parser) {
    assert(OptParser_isReachedEnd(parser));

    return (struct ParsedOpt) {
        .status   = OPT_PARSE_STATUS_END,
        .argIndex = parser->curArgIndex,
    };
}

struct ParsedOpt OptParser_parseNextPos_(OptParser* parser) {
    assert(!OptParser_isReachedEnd(parser));

    const char* const arg = parser->args[parser->curArgIndex] + parser->curArgPos;

    return (struct ParsedOpt) {
        .status   = OPT_PARSE_STATUS_NOT_AN_OPT,
        .argIndex = parser->curArgIndex++,
        .text     = arg,
        .textLen  = evalStrLen(arg),
    };
}

struct ParsedOpt OptParser_parseNextShort_(OptParser* parser) {
    assert(!OptParser_isReachedEnd(parser));

    const size_t      argIndex  = parser->curArgIndex;
    const char* const arg       = parser->args[argIndex];
    const char* const optPtr    = arg + parser->curArgPos++;
    const char        opt       = *optPtr;
    const char*       optArg    = NULL;
    unsigned char     status    = OPT_PARSE_STATUS_UNKNOWN;
    const struct Opt* optDesc   = NULL;

    if (!arg[parser->curArgPos]) {
        ++parser->curArgIndex;
        parser->curArgPos = 0;
    }

    for (size_t i = 0; i < parser->optCount; ++i) {
        const struct Opt* curOptDesc = parser->opts + i;

        if (curOptDesc->shortName == opt) {
            optDesc = curOptDesc;
            break;
        }
    }

    if (!optDesc)
        goto quit;

    status = OPT_PARSE_STATUS_OK;

    if (optDesc->takesArg) {
        if (OptParser_isReachedEnd(parser)) {
            status = OPT_PARSE_STATUS_MISSING_ARG;
            goto quit;
        }

        optArg = parser->args[parser->curArgIndex++] + parser->curArgPos;

        parser->curArgPos = 0;
    }

    quit:
    return (struct ParsedOpt) {
        .status   = status,

        .argIndex = argIndex,
        
        .text     = optPtr,
        .textLen  = 1,

        .opt      = optDesc,
        .optArg   = optArg,
        .optShort = true,
    };
}

struct ParsedOpt OptParser_parseNextLong_(OptParser* parser) {
    assert(!OptParser_isReachedEnd(parser));

    size_t            argIndex  = parser->curArgIndex++;
    const char* const arg       = parser->args[argIndex];
    const char* const opt       = arg + parser->curArgPos;
    const char* const optArgSep = strchr(opt, '=');
    const char*       optArg    = optArgSep ? optArgSep + 1      : NULL;
    size_t            optLen    = optArgSep ? optArgSep - opt    : evalStrLen(opt);
    const struct Opt* optDesc   = NULL;
    unsigned char     status    = OPT_PARSE_STATUS_UNKNOWN;

    parser->curArgPos = 0;

    for (size_t i = 0; i < parser->optCount; ++i) {
        const struct Opt* const curOptDesc = parser->opts + i;

        if (!cmpStrZN(curOptDesc->longName, opt, optLen)) {
            optDesc = curOptDesc;
            break;
        }
    }

    if (!optDesc)
        goto quit;

    status = OPT_PARSE_STATUS_OK;

    if (optDesc->takesArg && !optArg) {
        if (OptParser_isReachedEnd(parser)) {
            status = OPT_PARSE_STATUS_MISSING_ARG;
            goto quit;
        }

        optArg = parser->args[parser->curArgIndex++];
    }

    quit:
    return (struct ParsedOpt) {
        .status   = status,

        .argIndex = argIndex,
        
        .text     = opt,
        .textLen  = optLen,

        .opt      = optDesc,
        .optArg   = optArg,
    };
}

void OptParser_reset(OptParser* parser) {
    assert(OptParser_isValid(parser));

    parser->curArgIndex = 0;
    parser->curArgPos   = 0;
}

const char* const* OptParser_getArgs(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->args;
}

size_t OptParser_getArgCount(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->argCount;
}

const struct Opt* OptPaser_getOpts(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->opts;
}

size_t OptPaser_getOptCount(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->optCount;
}

bool OptParser_isReachedEnd(const OptParser* parser) {
    assert(OptParser_isValid(parser));
    return parser->curArgIndex >= parser->argCount;
}

bool OptParser_isValid(const OptParser* parser) {
    return parser
        && allNotNull((const void* const*) parser->args, parser->argCount)
        && parser->opts
        && parser->curArgIndex <= parser->argCount;
}
