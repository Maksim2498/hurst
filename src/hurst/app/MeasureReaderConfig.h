#ifndef HURST_APP_MEASUREREADERCONFIG_H
#define HURST_APP_MEASUREREADERCONFIG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <hurst/util/time/Timestamp.h>

#include "App.h"

struct MeasureReaderConfig {
    const char* filename;

    size_t      initBufferSize;

    const char* dateFmt;
    size_t      dateFmtLen;

    size_t      dateCol;
    size_t      valueCol;

    Timestamp   fromDate;
    Timestamp   untilDate;

    FILE*       errorFile;

    char        colSep;

    bool        ignoreDate;
    bool        ignoreBad;
};

extern const struct MeasureReaderConfig MEASURE_READER_CONFIG_DEFAULT;

struct MeasureReaderConfig MeasureReaderConfig_FromAppConfig(const struct AppConfig* appConfig);

bool MeasureReaderConfig_isValid(const struct MeasureReaderConfig* config);

#endif
