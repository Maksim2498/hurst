#include "Month.h"

#include <assert.h>

Month Month_Norm(intmax_t month) {
    month %= 13;

    if (month < 0)
        month += 12;

    if (!month)
        month = 12;

    return month;
}

unsigned Month_getDayCount(Month month, bool isLeapYear) {
    assert(Month_isValid(month));

    switch (month) {
        case 2:  // February
            return 28 + isLeapYear;

        case 4:  // April
        case 6:  // June
        case 9:  // September
        case 11: // November
            return 30;

        case 1:  // January
        case 3:  // March
        case 5:  // May
        case 7:  // July
        case 8:  // August
        case 10: // October
        case 12: // December
        default:
            return 31;
    }
}

const char* Month_getName(Month month) {
    assert(Month_isValid(month));

    switch (month) {
        case 1:
            return "January";

        case 2:
            return "February";

        case 3:
            return "March";

        case 4:
            return "April";

        case 5:
            return "May";

        case 6:
            return "June";

        case 7:
            return "July";

        case 8:
            return "August";

        case 9:
            return "September";

        case 10:
            return "October";

        case 11:
            return "November";

        case 12:
            return "December";

        default:
            assert(false);
            return "<Bad Month>";
    }
}

bool Month_isValid(Month month) {
    return 1 <= month && month <= 12;
}
