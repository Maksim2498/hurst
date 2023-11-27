#ifndef HURST_UTIL_OPT_OPTPARSESTATUS_H
#define HURST_UTIL_OPT_OPTPARSESTATUS_H

#include <stdbool.h>

enum OptParseStatus {
    OPT_PARSE_STATUS_OK,
    OPT_PARSE_STATUS_END,
    OPT_PARSE_STATUS_NOT_AN_OPT,
    OPT_PARSE_STATUS_UNKNOWN,
    OPT_PARSE_STATUS_MISSING_ARG,
};

const char* OptParseStatus_getName(enum OptParseStatus status);
bool OptParseStatus_isValid(enum OptParseStatus status);

#endif
