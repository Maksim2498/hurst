#ifndef HURST_UTIL_TIME_TIMESTAMP_H
#define HURST_UTIL_TIME_TIMESTAMP_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include "consts.h"
#include "Month.h"
#include "Year.h"

// Number of seconds from the 1st January, 1970
typedef intmax_t Timestamp;

#define TIMESTAMP_BAD INT64_MIN

#define TIMESTAMP_MIN (INT64_MIN + 1)
#define TIMESTAMP_MAX  INT64_MAX

#define TIMESTAMP_YEAR_ORIGIN 1970
#define TIMESTAMP_YEAR_MAX    (TIMESTAMP_YEAR_ORIGIN + INT64_MAX / YEAR_SECS)
#define TIMESTAMP_YEAR_MIN    (TIMESTAMP_YEAR_ORIGIN + INT64_MIN / YEAR_SECS)

/*
    Format syntax: {%<duration specifier>|<exact>|<space>}

    <exact> character will only match the same character.

    <space> (' ') character will match any number of space characters.

    <duration specifier> after '%' character will try to parse a
    duration component and add it to the result. Any unknown specifier
    will be treated as an exact character.

    +-----------+------------------+
    | Duration  | Description      |
    | Specifier |                  |
    +-----------+------------------+
    | d         | Day of year      |
    | M         | Month of year    |
    | y         | Year             |
    | h         | Hour of day      |
    | m         | Minute of hour   |
    | s         | Second of minute |
    +-----------+------------------+
*/

Timestamp Timestamp_Now(void);
Timestamp Timestamp_Parse(const char* str, const char* fmt);
Timestamp Timestamp_ParseN(const char* str, size_t strLen, const char* fmt, size_t fmtLen);

Year Timestamp_getYear(Timestamp ts);
Month Timestamp_getMonthOfYear(Timestamp ts);
unsigned Timestamp_getDayOfYear(Timestamp ts);
unsigned Timestamp_getDayOfMonth(Timestamp ts);
unsigned Timestamp_getHourOfDay(Timestamp ts);
unsigned Timestamp_getMinOfHour(Timestamp ts);
unsigned Timestamp_getSecOfMin(Timestamp ts);

#endif
