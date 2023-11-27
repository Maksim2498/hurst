#ifndef HURST_UTIL_TIME_CONSTS_H
#define HURST_UTIL_TIME_CONSTS_H

enum {
    MIN_SECS  = 60,
    HOUR_SECS = 60 * MIN_SECS,
    DAY_SECS  = 24 * HOUR_SECS,
    WEEK_SECS = 7  * DAY_SECS,
    YEAR_SECS = 31556952, // 365 * DAY_SECS is not precise enough
                          // for accounting of leap years
};

#endif
