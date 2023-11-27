#ifndef HURST_APP_APPCONFIGPARSER_H
#define HURST_APP_APPCONFIGPARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <hurst/util/opt/Opt.h>

#include "ParsedAppConfig.h"

typedef struct {
    FILE*              errorFile;
    const char* const* args;
    size_t             argCount;
} AppConfigParser;

extern const struct Opt APP_CONFIG_PARSER_OPTS[];
extern const size_t     APP_CONFIG_PARSER_OPT_COUNT;

void AppConfigParser_init(
    AppConfigParser*   parser,
    const char* const* args,
    size_t             argCount
);

void AppConfigParser_initEx(
    AppConfigParser*   parser,
    const char* const* args,
    size_t             argCount,
    FILE*              errorFile
);

FILE* AppConfigParser_getErrorFile(const AppConfigParser* parser);
const char* const* AppConfigParser_getArgs(const AppConfigParser* parser);
size_t AppConfigParser_getArgCount(const AppConfigParser* parser);

struct ParsedAppConfig AppConfigParser_parse(AppConfigParser* parser);

bool AppConfigParser_isValid(const AppConfigParser* parser);

#endif
