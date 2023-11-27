#include "Timestamp.h"

#include <assert.h>
#include <ctype.h>
#include <time.h>

#include <hurst/util/str.h>

#include "consts.h"

Timestamp Timestamp_Now(void) {
    return time(NULL);
}

Timestamp Timestamp_Parse(const char* str, const char* fmt) {
    return Timestamp_ParseN(str, evalStrLen(str), fmt, evalStrLen(fmt));
}

Timestamp Timestamp_ParseN(const char* str, size_t strLen, const char* fmt, size_t fmtLen) {
    assert(str && fmt);

    intmax_t year  = TIMESTAMP_YEAR_ORIGIN;
    intmax_t month = 1;
    intmax_t day   = 1;
    intmax_t hour  = 0;
    intmax_t min   = 0;
    intmax_t sec   = 0;

    size_t   strI  = 0;

    for (size_t fmtI = 0; fmtI < fmtLen;) {
        if (strI >= strLen)
            return TIMESTAMP_BAD;

        char fmtC = fmt[fmtI++];

        switch (fmtC) {
            default:
                exact:
                if (str[strI++] != fmtC)
                    return TIMESTAMP_BAD;

                continue;

            case ' ':
                if (!isspace(str[strI++]))
                    return TIMESTAMP_BAD;

                while (strI < strLen && isspace(str[strI]))
                    ++strI;

                continue;

            case '%': {
                if (fmtI >= fmtLen)
                    goto exact;
                
                intmax_t* part;
                intmax_t  partMin;
                intmax_t  partMax;

                switch (fmtC = fmt[fmtI++]) {
                    default:
                        goto exact;

                    case 'y':
                        part    = &year;
                        partMin = TIMESTAMP_YEAR_MIN;
                        partMax = TIMESTAMP_YEAR_MAX;
                        break;

                    case 'M':
                        part    = &month;
                        partMin = 1;
                        partMax = 12;
                        break;

                    case 'd':
                        part    = &day;
                        partMin = 1;
                        partMax = 31;
                        break;

                    case 'h':
                        part    = &hour;
                        partMin = 0;
                        partMax = 24;
                        break;

                    case 'm':
                        part    = &min;
                        partMin = 0;
                        partMax = 60;
                        break;
                    
                    case 's':
                        part    = &sec;
                        partMin = 0;
                        partMax = 60;
                        break;
                }
                
                static const struct IntIFmt INT_FMT = { .radix = 10 };

                const struct ParsedInt parsedInt = parseIntStrNFmt(str + strI, strLen - strI, &INT_FMT);

                if (!parsedInt.valid)
                    return TIMESTAMP_BAD;

                if (parsedInt.value < partMin || parsedInt.value > partMax)
                    return TIMESTAMP_BAD;

                *part = parsedInt.value;

                strI += parsedInt.len;
            }
        }
    }

    if (strI < strLen)
        return TIMESTAMP_BAD;

    const bool isYearLeap = Year_isLeap(year);

    if (day > Month_getDayCount(month, isYearLeap))
        return TIMESTAMP_BAD;

    Timestamp result = 0;

    result += (year - TIMESTAMP_YEAR_ORIGIN) * YEAR_SECS;

    for (unsigned i = 1; i < month; ++i)
        result += DAY_SECS * Month_getDayCount(i, isYearLeap);

    result += (day - 1) * DAY_SECS;
    result +=  hour     * HOUR_SECS;
    result +=  min      * MIN_SECS;
    result +=  sec;
    
    return result;
}

Year Timestamp_getYear(Timestamp ts) {
    int year = TIMESTAMP_YEAR_ORIGIN + ts / YEAR_SECS;

    if (ts < 0)
        --year;

    return year;
}

Month Timestamp_getMonthOfYear(Timestamp ts) {
    const Year year       = Timestamp_getYear(ts);
    const bool isYearLeap = Year_isLeap(year);
    
    unsigned day = Timestamp_getDayOfYear(ts);

    for (Month i = 1; i <= 12; ++i) {
        const unsigned dayCount = Month_getDayCount(i, isYearLeap);

        if (dayCount >= day)
            return i;

        day -= dayCount;
    }

    return 1;
}

unsigned Timestamp_getDayOfYear(Timestamp ts) {
    return (YEAR_SECS + ts) % YEAR_SECS / DAY_SECS + 1;
}

unsigned Timestamp_getDayOfMonth(Timestamp ts) {
    const Year year       = Timestamp_getYear(ts);
    const bool isYearLeap = Year_isLeap(year);

    unsigned day = Timestamp_getDayOfYear(ts);

    for (Month i = 1; i <= 12; ++i) {
        const unsigned dayCount = Month_getDayCount(i, isYearLeap);

        if (dayCount >= day)
            return day;

        day -= dayCount;
    }

    return 1;
}

unsigned Timestamp_getHourOfDay(Timestamp ts) {
    return (DAY_SECS + ts % YEAR_SECS) % DAY_SECS / HOUR_SECS;
}

unsigned Timestamp_getMinOfHour(Timestamp ts) {
    return (HOUR_SECS + ts % YEAR_SECS) % HOUR_SECS / MIN_SECS;
}

unsigned Timestamp_getSecOfMin(Timestamp ts) {
    return (MIN_SECS + ts % YEAR_SECS) % MIN_SECS;
}
