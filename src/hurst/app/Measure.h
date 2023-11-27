#ifndef HURST_APP_MEASURE_H
#define HURST_APP_MEASURE_H

#include <stddef.h>
#include <stdio.h>

#include <hurst/util/time/Timestamp.h>

struct Measure {
    Timestamp date;
    double    value;
};

int Measure_cmpByDate(const struct Measure* lhs, const struct Measure* rhs);

size_t printMeasure(FILE* file, const struct Measure* measure);

#endif
