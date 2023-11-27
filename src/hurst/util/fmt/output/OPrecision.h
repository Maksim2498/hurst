#ifndef HURST_UTIL_FMT_OUTPUT_OPRECISION_H
#define HURST_UTIL_FMT_OUTPUT_OPRECISION_H

#include <stdarg.h>
#include <stddef.h>

#include "ParsedOPrecision.h"

struct ParsedOPrecision OPrecision_Parse(const char* str, ...);
struct ParsedOPrecision OPrecision_ParseN(const char* str, size_t n, ...);
struct ParsedOPrecision OPrecision_ParseV(const char* str, va_list* args);
struct ParsedOPrecision OPrecision_ParseNV(const char* str, size_t n, va_list* args);

#endif
