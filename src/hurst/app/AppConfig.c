#include "AppConfig.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/io/print.h>
#include <hurst/util/array.h>
#include <hurst/config.h>

const struct AppConfig APP_CONFIG_DEFAULT = {
    .filenames       = NULL,
    .filenameCount   = 0,
    .dateFmt         = "%y-%M-%d %h:%m:%s",
    .fromDate        = TIMESTAMP_MIN,
    .untilDate       = TIMESTAMP_MAX,
    .maxIncLen       = 10,
    .dateCol         = 0,
    .valueCol        = 1,
    .colSep          = ',',
    .ignoreDate      = false,
    .showHelp        = false,
    .showDateFmtHelp = false,
    .showVersion     = false,
    .showConfig      = false,
    .sort            = false,
};

size_t printAppConfig(FILE* file, const struct AppConfig* config) {
    assert(AppConfig_isValid(config));

    return printFmt(
        file,

        "Files:                    %.**\"s[]\n"
        "Date Format:              %\"s\n"
        "From Date:                %t\n"
        "Until Date:               %t\n"
        "Maximum Increment Length: %z\n"
        "Date Column:              %z\n"
        "Value Column:             %z\n"
        "Column Separator:         %'c\n"
        "Ignore Date:              %b\n"
        "Ignore Bad:               %b\n"
        "Show Help:                %b\n"
        "Show Date Format Help:    %b\n"
        "Show Version:             %b\n"
        "Show Config:              %b\n"
        "Sort:                     %b\n",

        config->filenameCount, config->filenames,
        config->dateFmt,
        config->fromDate,
        config->untilDate,
        config->maxIncLen,
        config->dateCol,
        config->valueCol,
        config->colSep,
        config->ignoreDate,
        config->ignoreBad,
        config->showHelp,
        config->showDateFmtHelp,
        config->showVersion,
        config->showConfig,
        config->sort
    );
}

bool AppConfig_isValid(const struct AppConfig* config) {
    return config
        && (!config->filenames || allNotNull((const void* const*) config->filenames, config->filenameCount))
        && config->dateFmt
        && TIMESTAMP_BAD != config->fromDate
        && TIMESTAMP_BAD != config->untilDate
        && config->fromDate <= config->untilDate
        && isprint(config->colSep);
}
