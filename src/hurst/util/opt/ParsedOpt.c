#include "ParsedOpt.h"

#include <assert.h>

#include <hurst/util/io/print.h>

size_t printParsedOpt(FILE* file, const struct ParsedOpt* parsedOpt) {
    return printParsedOptFmt(file, parsedOpt, &PARSED_OPT_FMT_DEFAULT);
}

const struct ParsedOptFmt PARSED_OPT_FMT_DEFAULT = {
    .startOffset = 0,
    .fieldOffset = 4,
    .endOffset   = 0,
};

size_t printParsedOptFmt(
    FILE*                      file,
    const struct ParsedOpt*    parsedOpt,
    const struct ParsedOptFmt* fmt
) {
    assert(ParsedOpt_isValid(parsedOpt));

    size_t printed = 0;

    printed += printFmt(
        file,

        "%.**c{         \n"
        "%.**coptArg:   %s\n"
        "%.**cargIndex: %z\n"
        "%.**copt:      ",

        fmt->startOffset, ' ',
        fmt->fieldOffset, ' ', parsedOpt->optArg ? parsedOpt->optArg : "-",
        fmt->fieldOffset, ' ', parsedOpt->argIndex,
        fmt->fieldOffset, ' '
    );

    if (parsedOpt->opt) {
        const struct OptFmt optFmt = {
            .startOffset = 0,
            .fieldOffset = 2 * fmt->fieldOffset - fmt->endOffset,
            .endOffset   = fmt->fieldOffset,
        };

        printed += printOptFmt(file, parsedOpt->opt, &optFmt);
    } else
        printed += printChar(file, '-');

    printed += printFmt(
        file,

        "\n"
        "%.**ctext:     %.**s\n"
        "%.**ctextLen:  %z\n"
        "%.**coptShort: %b\n"
        "%.**cstatus:   %s\n"
        "%.**c}",

        fmt->fieldOffset, ' ', parsedOpt->text ? parsedOpt->textLen : 1, parsedOpt->text ? parsedOpt->text : "-",
        fmt->fieldOffset, ' ', parsedOpt->textLen,
        fmt->fieldOffset, ' ', parsedOpt->optShort,
        fmt->fieldOffset, ' ', OptParseStatus_getName(parsedOpt->status),
        fmt->endOffset,   ' '
    );

    return printed;
}

bool ParsedOpt_isValid(const struct ParsedOpt* parsedOpt) {
    if (!parsedOpt || !OptParseStatus_isValid(parsedOpt->status))
        return false;

    switch (parsedOpt->status) {
        case OPT_PARSE_STATUS_OK:
            return parsedOpt->opt
                && parsedOpt->text
                && (!parsedOpt->opt->takesArg || parsedOpt->optArg);

        case OPT_PARSE_STATUS_MISSING_ARG:
            return  parsedOpt->opt
                &&  parsedOpt->text
                &&  parsedOpt->opt->takesArg
                && !parsedOpt->optArg;

        case OPT_PARSE_STATUS_NOT_AN_OPT:
        case OPT_PARSE_STATUS_UNKNOWN:
            return parsedOpt->text;

        case OPT_PARSE_STATUS_END:
            return true;

        default:
            return false;
    }
}
