#ifndef HURST_APP_APPCONFIG_H
#define HURST_APP_APPCONFIG_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include <hurst/util/time/Timestamp.h>

struct AppConfig {
    const char* const* filenames;
    size_t             filenameCount;
    const char*        dateFmt;
    Timestamp          fromDate;
    Timestamp          untilDate;
    size_t             maxIncLen;
    size_t             dateCol;
    size_t             valueCol;
    char               colSep;
    bool               ignoreDate;
    bool               ignoreBad;
    bool               showHelp;
    bool               showDateFmtHelp;
    bool               showVersion;
    bool               showConfig;
    bool               sort;
};

extern const struct AppConfig APP_CONFIG_DEFAULT;

size_t printAppConfig(FILE* file, const struct AppConfig* config);

bool AppConfig_isValid(const struct AppConfig* config);

#endif
