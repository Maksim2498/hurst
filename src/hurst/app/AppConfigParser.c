#include "AppConfigParser.h"

#include <assert.h>

#include <hurst/util/collection/array.h>
#include <hurst/util/io/print.h>
#include <hurst/util/opt/all.h>
#include <hurst/util/time/Timestamp.h>
#include <hurst/util/str.h>

#include "AppConfig.h"


struct ParsedSize_ {
    size_t value;
    bool   valid;
};

static size_t AppConfigParser_printUnknownOpt_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
);

static size_t AppConfigParser_printMissingOptArg_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
);

static size_t AppConfigParser_printBadOptArg_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
);

static struct ParsedSize_ AppConfigParser_parseSizeOptArg_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
);

static size_t AppConfigParser_printOptArgOverflow_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
);

static size_t AppConfigParser_printOptArgTooBig_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt,
    intmax_t                max
);

static size_t AppConfigParser_printOptArgTooSmall_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt,
    intmax_t                min
);


enum OptId_ {
    OPT_ID_HELP,
    OPT_ID_HELP_DATE_FMT,
    OPT_ID_VERSION,
    OPT_ID_SHOW_CONFIG,
    OPT_ID_SEP,
    OPT_ID_DATE_FMT,
    OPT_ID_DATE_COL,
    OPT_ID_VAL_COL,
    OPT_ID_FROM,
    OPT_ID_UNTIL,
    OPT_ID_MAX_INC_LEN,
    OPT_ID_NO_DATE,
    OPT_ID_IGNORE_BAD,
    OPT_ID_SORT,
};

const struct Opt APP_CONFIG_PARSER_OPTS[] = {
    [OPT_ID_HELP] = {
        .longName    = "help",
        .shortName   = 'h',
        .description = "Prints help message and quits",
    },

    [OPT_ID_HELP_DATE_FMT] = {
        .longName    = "help-date-fmt",
        .description = "Prints date format help message and quits",
    },

    [OPT_ID_VERSION] = {
        .longName    = "version",
        .shortName   = 'v',
        .description = "Prints version message and quits",
    },

    [OPT_ID_SHOW_CONFIG] = {
        .longName    = "show-config",
        .description = "Prints current program configuration",
    },

    [OPT_ID_SEP] = {
        .longName    = "sep",
        .shortName   = 's',
        .description = "Specifies column separator (\",\" by default)",
        .takesArg    = true,
    },

    [OPT_ID_DATE_FMT] = {
        .longName    = "date-fmt",
        .description = "Specifies date format (\"%y-%M-%d %h:%m:%s\" by default)",
        .takesArg    = true,
    },

    [OPT_ID_DATE_COL] = {
        .longName    = "date-col",
        .description = "Specifies data column offset (0 by default)",
        .takesArg    = true,
    },

    [OPT_ID_VAL_COL] = {
        .longName    = "val-col",
        .description = "Specifies value column offset (1 by default)",
        .takesArg    = true,
    },

    [OPT_ID_FROM] = {
        .longName    = "from",
        .description = "Specifies minimum value date (minimum possible by default)",
        .takesArg    = true,
    },

    [OPT_ID_UNTIL] = {
        .longName    = "until",
        .description = "Specifies maximum value date (maximum possible by default)",
        .takesArg    = true,
    },

    [OPT_ID_MAX_INC_LEN] = {
        .longName    = "max-inc-len",
        .description = "Specifies maximum increment length (10 by default)",
        .takesArg    = true,
    },

    [OPT_ID_NO_DATE] = {
        .longName    = "no-date",
        .description = "Disables value's date processing",
    },

    [OPT_ID_IGNORE_BAD] = {
        .longName    = "ignore-bad",
        .description = "Enables ignoring of bad values and dates",
    },

    [OPT_ID_SORT] = {
        .longName    = "sort",
        .shortName   = 'S',
        .description = "Enables records sorting by date (in ascending order)",
    },
};

const size_t APP_CONFIG_PARSER_OPT_COUNT = sizeof APP_CONFIG_PARSER_OPTS / sizeof APP_CONFIG_PARSER_OPTS[0];


void AppConfigParser_init(
    AppConfigParser*   parser,
    const char* const* args,
    size_t             argCount
) {
    AppConfigParser_initEx(parser, args, argCount, stderr);
}

void AppConfigParser_initEx(
    AppConfigParser*   parser,
    const char* const* args,
    size_t             argCount,
    FILE*              errorFile
) {
    assert(parser && allNotNull((const void* const*) args, argCount));

    parser->args      = args;
    parser->argCount  = argCount;
    parser->errorFile = errorFile;
}

FILE* AppConfigParser_getErrorFile(const AppConfigParser* parser) {
    assert(AppConfigParser_isValid(parser));
    return parser->errorFile;
}

const char* const* AppConfigParser_getArgs(const AppConfigParser* parser) {
    assert(AppConfigParser_isValid(parser));
    return parser->args;
}

size_t AppConfigParser_getArgCount(const AppConfigParser* parser) {
    assert(AppConfigParser_isValid(parser));
    return parser->argCount;
}

struct ParsedAppConfig AppConfigParser_parse(AppConfigParser* parser) {
    assert(AppConfigParser_isValid(parser));

    struct AppConfig config = APP_CONFIG_DEFAULT;

    #define MAKE_OK_RESULT()    (struct ParsedAppConfig) { .value = config, .valid = true  }
    #define MAKE_ERROR_RESULT() (struct ParsedAppConfig) { .value = config, .valid = false }

    OptParser argParser;

    OptParser_init(
        &argParser,
        parser->args,
        parser->argCount,
        APP_CONFIG_PARSER_OPTS,
        APP_CONFIG_PARSER_OPT_COUNT
    );
    
    while (true) {
        struct ParsedOpt parsedOpt = OptParser_parseNext(&argParser);

        switch (parsedOpt.status) {
            default:
                assert(false);
                return MAKE_ERROR_RESULT();

            case OPT_PARSE_STATUS_NOT_AN_OPT:
                config.filenames     = parser->args     + parsedOpt.argIndex;
                config.filenameCount = parser->argCount - parsedOpt.argIndex;
                return MAKE_OK_RESULT();

            case OPT_PARSE_STATUS_END:
                return MAKE_OK_RESULT();

            case OPT_PARSE_STATUS_UNKNOWN:
                AppConfigParser_printUnknownOpt_(parser, &parsedOpt);
                return MAKE_ERROR_RESULT();

            case OPT_PARSE_STATUS_MISSING_ARG:
                AppConfigParser_printMissingOptArg_(parser, &parsedOpt);
                return MAKE_ERROR_RESULT();

            case OPT_PARSE_STATUS_OK: {
                enum OptId_ optId = parsedOpt.opt - APP_CONFIG_PARSER_OPTS;

                switch (optId) {
                    case OPT_ID_HELP:
                        config.showHelp = true;
                        break;

                    case OPT_ID_HELP_DATE_FMT:
                        config.showDateFmtHelp = true;
                        break;

                    case OPT_ID_VERSION:
                        config.showVersion = true;
                        break;

                    case OPT_ID_SHOW_CONFIG:
                        config.showConfig = true;
                        break;

                    case OPT_ID_SEP: {
                        const char* const arg = parsedOpt.optArg;

                        // Length is zero or not one
                        if (!arg[0] || arg[1]) {
                            AppConfigParser_printBadOptArg_(parser, &parsedOpt);
                            return MAKE_ERROR_RESULT();
                        }

                        config.colSep = *arg;

                        break;
                    }

                    case OPT_ID_DATE_FMT:
                        config.dateFmt = parsedOpt.optArg;
                        break;

                    case OPT_ID_DATE_COL: {
                        const struct ParsedSize_ parsedSize = AppConfigParser_parseSizeOptArg_(parser, &parsedOpt);

                        if (!parsedSize.valid)
                            return MAKE_ERROR_RESULT();

                        config.dateCol = parsedSize.value;

                        break;
                    }

                    case OPT_ID_VAL_COL: {
                        const struct ParsedSize_ parsedSize = AppConfigParser_parseSizeOptArg_(parser, &parsedOpt);

                        if (!parsedSize.valid)
                            return MAKE_ERROR_RESULT();

                        config.valueCol = parsedSize.value;

                        break;
                    }

                    case OPT_ID_FROM:
                        config.fromDate = Timestamp_Parse(parsedOpt.optArg, config.dateFmt);

                        if (TIMESTAMP_BAD == config.fromDate) {
                            AppConfigParser_printBadOptArg_(parser, &parsedOpt);
                            return MAKE_ERROR_RESULT();
                        }

                        break;

                    case OPT_ID_UNTIL:
                        config.untilDate = Timestamp_Parse(parsedOpt.optArg, config.dateFmt);

                        if (TIMESTAMP_BAD == config.untilDate) {
                            AppConfigParser_printBadOptArg_(parser, &parsedOpt);
                            return MAKE_ERROR_RESULT();
                        }

                        break;
                        
                    case OPT_ID_MAX_INC_LEN: {
                        const struct ParsedSize_ parsedSize = AppConfigParser_parseSizeOptArg_(parser, &parsedOpt);

                        if (!parsedSize.valid)
                            return MAKE_ERROR_RESULT();

                        config.maxIncLen = parsedSize.value;

                        break;
                    }

                    case OPT_ID_NO_DATE:
                        config.ignoreDate = true;
                        break;

                    case OPT_ID_IGNORE_BAD:
                        config.ignoreBad = true;
                        break;

                    case OPT_ID_SORT:
                        config.sort = true;
                        break;

                    default:
                        assert(false);
                }

                break;
            }
        }
    }

    #undef MAKE_OK_RESULT
    #undef MAKE_ERROR_RESULT
}

bool AppConfigParser_isValid(const AppConfigParser* parser) {
    return parser
        && allNotNull((const void* const*) parser->args, parser->argCount);
}

size_t AppConfigParser_printUnknownOpt_(AppConfigParser* parser, const struct ParsedOpt* parsedOpt) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_UNKNOWN == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Unknown option: %.*c%.**s\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text
            )
        :   0;
}

size_t AppConfigParser_printMissingOptArg_(AppConfigParser* parser, const struct ParsedOpt* parsedOpt) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_MISSING_ARG == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Missing option argument: %.*c%.**s\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text
            )
        :   0;
}

size_t AppConfigParser_printBadOptArg_(AppConfigParser* parser, const struct ParsedOpt* parsedOpt) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_OK == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Bad option argument: %.*c%.**s=%\"s\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text,
                parsedOpt->optArg
            )
        :   0;
}

struct ParsedSize_ AppConfigParser_parseSizeOptArg_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt
) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_OK == parsedOpt->status
    );
    
    struct ParsedInt parsedInt = parseIntStr(parsedOpt->optArg);

    #define MAKE_OK_RESULT()    (struct ParsedSize_) { .value = parsedInt.value, .valid = true }
    #define MAKE_ERROR_RESULT() (struct ParsedSize_) { .value = 0,               .valid = false }

    if (!parsedInt.valid || !parsedInt.fullMatch) {
        AppConfigParser_printBadOptArg_(parser, parsedOpt);
        return MAKE_ERROR_RESULT();
    }

    if (parsedInt.overflow) {
        AppConfigParser_printOptArgOverflow_(parser, parsedOpt);
        return MAKE_ERROR_RESULT();
    }

    if (parsedInt.value < 0) {
        AppConfigParser_printOptArgTooSmall_(parser, parsedOpt, 0);
        return MAKE_ERROR_RESULT();
    }

    if ((uintmax_t) parsedInt.value > SIZE_MAX) {
        AppConfigParser_printOptArgTooBig_(parser, parsedOpt, SIZE_MAX);
        return MAKE_ERROR_RESULT();
    }

    return MAKE_OK_RESULT();

    #undef MAKE_OK_RESULT
    #undef MAKE_ERROR_RESULT
}

size_t AppConfigParser_printOptArgOverflow_(AppConfigParser* parser, const struct ParsedOpt* parsedOpt) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_OK == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Option argument is out of bounds: %.*c%.**s=%\"s\n"
                "It's value must be inside the range [%m, %m]\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text,
                parsedOpt->optArg,
                INTMAX_MIN, INTMAX_MAX
            )
        :   0;
}

size_t AppConfigParser_printOptArgTooBig_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt,
    intmax_t                max
) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_OK == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Option argument is too big: %.*c%.**s=%\"s\n"
                "Maximum allowed value is %m\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text,
                parsedOpt->optArg,
                max
            )
        :   0;
}

size_t AppConfigParser_printOptArgTooSmall_(
    AppConfigParser*        parser,
    const struct ParsedOpt* parsedOpt,
    intmax_t                min
) {
    assert(
        AppConfigParser_isValid(parser) &&
        ParsedOpt_isValid(parsedOpt)    &&
        OPT_PARSE_STATUS_OK == parsedOpt->status
    );

    return  parser->errorFile
        ?   printFmt(
                parser->errorFile,

                "Option argument is too big: %.*c%.**s=%\"s\n"
                "Minimum allowed value is %m\n",

                1 + !parsedOpt->optShort, '-',
                parsedOpt->textLen, parsedOpt->text,
                parsedOpt->optArg,
                min
            )
        :   0;
}
