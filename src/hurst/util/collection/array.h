#ifndef HURST_UTIL_COLLECTION_ARRAY_H
#define HURST_UTIL_COLLECTION_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "cb.h"

bool allNotNull(const void* const* array, size_t len);

void applyToDArray(double* array, size_t len, ApplyD apply);

struct Line {
    double slope;
    double offset;
};

struct Line evalDArrayLinReg(const double* x, const double* y, size_t len);

double evalDArraysProdSum(const double* lhs, const double* rhs, size_t len);
double evalDArrayStd(const double* array, size_t len);
double evalDArrayDisp(const double* array, size_t len);
double evalDArrayAvg(const double* array, size_t len);
double evalDArraySum(const double* array, size_t len);

void sortArray(void* array, void* buffer, size_t len, size_t itemSize, Cmp cmp);

#endif
