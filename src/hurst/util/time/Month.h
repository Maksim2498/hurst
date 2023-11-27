#ifndef HURST_UTIL_TIME_MONTH_H
#define HURST_UTIL_TIME_MONTH_H

#include <stdbool.h>
#include <stdint.h>

typedef unsigned Month;

/*
    Converts intmax_t range to month range [1;12]

     .
     .
     .
    -13 November
    -12 December
    -11 January
    -10 February
    -9  March
    -8  April
    -7  May
    -6  June
    -5  July
    -4  August
    -3  September
    -2  October
    -1  November
     0  December
     1  January
     2  February
     3  March
     4  April
     5  May
     6  June
     7  July
     8  August
     9  September
     10 October
     11 November
     12 December
     13 January
     .
     .
     .
*/

Month Month_Norm(intmax_t month);

unsigned Month_getDayCount(Month month, bool isLeapYear);
const char* Month_getName(Month month);

bool Month_isValid(Month month);

#endif
