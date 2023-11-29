#ifndef HURST_UTIL_ARRAY_H
#define HURST_UTIL_ARRAY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

bool allNotNull(const void* const* array, size_t len);

double evalStd(const double* array, size_t len);
double evalDisp(const double* array, size_t len);
double evalAvg(const double* array, size_t len);
double evalSum(const double* array, size_t len);

#endif
