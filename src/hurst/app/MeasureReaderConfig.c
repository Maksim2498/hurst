#include "MeasureReaderConfig.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/str.h>

#define DATE_FMT_ "%y-%M-%d %h:%m:%s"

const struct MeasureReaderConfig MEASURE_READER_CONFIG_DEFAULT = {
    .filename       = NULL,
    
    .initBufferSize = 1024,

    .dateFmt        = DATE_FMT_,
    .dateFmtLen     = sizeof(DATE_FMT_) - 1,

    .dateCol        = 0,
    .valueCol       = 1,

    .fromDate       = TIMESTAMP_MIN,
    .untilDate      = TIMESTAMP_MAX,

    .errorFile      = NULL,

    .colSep         = ',',

    .ignoreDate     = false,
    .ignoreBad      = false,
};

#undef DATE_FMT_

struct MeasureReaderConfig MeasureReaderConfig_FromAppConfig(const struct AppConfig* appConfig) {
    assert(AppConfig_isValid(appConfig));

    struct MeasureReaderConfig config = MEASURE_READER_CONFIG_DEFAULT;

    config.dateFmt    = config.dateFmt;
    config.dateFmtLen = evalStrLen(config.dateFmt);
    config.dateCol    = config.dateCol;
    config.valueCol   = config.valueCol;
    config.fromDate   = config.fromDate;
    config.untilDate  = config.untilDate;
    config.colSep     = config.colSep;
    config.ignoreDate = config.ignoreDate;
    config.ignoreBad  = config.ignoreBad;

    return config;
}

bool MeasureReaderConfig_isValid(const struct MeasureReaderConfig* config) {
    return config
        && config->initBufferSize
        && config->dateFmt
        && TIMESTAMP_BAD != config->fromDate
        && TIMESTAMP_BAD != config->untilDate
        && isprint(config->colSep);
}
