#ifndef HURST_APP_PARSEDAPPCONFIG_H
#define HURST_APP_PARSEDAPPCONFIG_H

#include <stdbool.h>

#include "AppConfig.h"

struct ParsedAppConfig {
    struct AppConfig value;
    bool             valid;
};

#endif
