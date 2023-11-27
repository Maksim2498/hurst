#ifndef HURST_UTIL_FMT_OUTPUT_OCASE_H
#define HURST_UTIL_FMT_OUTPUT_OCASE_H

#include <stdbool.h>

enum OCase {
    OCASE_UPPER,
    OCASE_LOWER,
};

const char* OCase_getName(enum OCase c);

bool OCase_isValid(enum OCase c);

#endif
