#include "Opt.h"

#include <assert.h>

#include <hurst/util/io/print.h>
#include <hurst/util/str.h>


static const char LONG_OPT_ARG_DISPLAY_[]  = "=<arg>";
static const char SHORT_OPT_ARG_DISPLAY_[] = "<arg>";
static const char OPT_NAMES_SEP_[]         = ", ";

static size_t evalOptsHelpFirstColumnMaxLen_(const struct Opt* opts, size_t optCount);
static size_t printOptsHelpWithOffsetFirstColumnAndWidth_(
    FILE*             file,
    const struct Opt* opts,
    size_t            optCount,
    size_t            offset,
    size_t            width
);


size_t printOpt(FILE* file, const struct Opt* opt) {
    return printOptFmt(file, opt, &OPT_FMT_DEFAULT);
}

const struct OptFmt OPT_FMT_DEFAULT = {
    .startOffset = 0,
    .fieldOffset = 4,
    .endOffset   = 0,
};

size_t printOptFmt(
    FILE*                file,
    const struct Opt*    opt,
    const struct OptFmt* fmt
) {
    assert(opt);

    return printFmt(
        file,

        "%.**c{\n"
        "%.**clongName:    %s\n"
        "%.**cdescription: %s\n"
        "%.**cshortName:   %c\n"
        "%.**ctakesArg:    %b\n"
        "%.**c}",

        fmt->startOffset, ' ',
        fmt->fieldOffset, ' ', opt->longName    ? opt->longName    : "-",
        fmt->fieldOffset, ' ', opt->description ? opt->description : "-",
        fmt->fieldOffset, ' ', opt->shortName   ? opt->shortName   : '-',
        fmt->fieldOffset, ' ', opt->takesArg,
        fmt->endOffset,   ' '
    );
}

size_t printOptsHelp(FILE* file, const struct Opt* opts, size_t optCount) {
    return printOptsHelpFmt(file, opts, optCount, 0);
}

const struct OptsHelpFmt OPTS_HELP_FMT_DEFAULT = {
    .descriptionPadding = 1,
    .offset             = 0,
};

size_t printOptsHelpFmt(
    FILE*                     file,
    const struct Opt*         opts,
    size_t                    optCount,
    const struct OptsHelpFmt* fmt
) {
    assert(fmt);

    const size_t maxFirstColumnLen = evalOptsHelpFirstColumnMaxLen_(opts, optCount);
    const size_t firstColumnWidth  = fmt->descriptionPadding + maxFirstColumnLen;

    return printOptsHelpWithOffsetFirstColumnAndWidth_(file, opts, optCount, fmt->offset, firstColumnWidth);
}

size_t evalOptsHelpFirstColumnMaxLen_(const struct Opt* opts, size_t optCount) {
    assert(opts);

    size_t maxFirstColumnLen = 0;

    for (size_t i = 0; i < optCount; ++i) {
        const struct Opt* opt = opts + i;

        size_t firstColumnLen = 0;

        if (opt->shortName) {
            firstColumnLen += 2; // '-' <short name>

            if (opt->takesArg)
                firstColumnLen += sizeof SHORT_OPT_ARG_DISPLAY_ - 1;
        }

        if (opt->longName) {
            firstColumnLen += 2 + evalStrLen(opt->longName); // "--" <long name>

            if (opt->takesArg)
                firstColumnLen += sizeof LONG_OPT_ARG_DISPLAY_ - 1;
        }

        if (opt->shortName && opt->longName)
            firstColumnLen += sizeof OPT_NAMES_SEP_ - 1;

        if (firstColumnLen > maxFirstColumnLen)
            maxFirstColumnLen = firstColumnLen;
    }

    return maxFirstColumnLen;
}

size_t printOptsHelpWithOffsetFirstColumnAndWidth_(
    FILE*             file,
    const struct Opt* opts,
    size_t            optCount,
    size_t            offset,
    size_t            width
) {
    assert(opts);

    size_t printed = 0;

    for (size_t i = 0; i < optCount; ++i) {
        const struct Opt* opt = opts + i;

        printed += printCharN(file, ' ', offset);

        size_t printedFirstColumn = 0;

        if (opt->longName) {
            printedFirstColumn += printFmt(file, "--%s", opt->longName);

            if (opt->takesArg)
                printedFirstColumn += printStr(file, LONG_OPT_ARG_DISPLAY_);
        }

        if (opt->longName && opt->shortName)
            printedFirstColumn += printStr(file, OPT_NAMES_SEP_);

        if (opt->shortName) {
            printedFirstColumn += printFmt(file, "-%c", opt->shortName);

            if (opt->takesArg)
                printedFirstColumn += printStr(file, SHORT_OPT_ARG_DISPLAY_);
        }

        printed += printedFirstColumn;

        if (opt->description) {
            if (width > printedFirstColumn)
                printed += printCharN(file, ' ', width - printedFirstColumn);

            printed += printStr(file, opt->description) ;
        }

        if (i + 1 != optCount)
            printed += printChar(file, '\n');
    }

    return printed;
}
