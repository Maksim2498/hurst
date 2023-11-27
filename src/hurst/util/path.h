#ifndef HURST_UTIL_PATH_H
#define HURST_UTIL_PATH_H

#include <hurst/config.h>

#if OS_ID == OS_ID_WIN
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

const char* getBasename(const char* path);

#endif
