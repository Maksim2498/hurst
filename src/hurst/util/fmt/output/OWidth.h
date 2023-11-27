#ifndef HURST_UTIL_FMT_OUTPUT_OWIDTH_H
#define HURST_UTIL_FMT_OUTPUT_OWIDTH_H

#include <stdarg.h>
#include <stddef.h>

#include "ParsedOWidth.h"

struct ParsedOWidth OWidth_Parse(const char* str, ...);
struct ParsedOWidth OWidth_ParseN(const char* str, size_t n, ...);
struct ParsedOWidth OWidth_ParseV(const char* str, va_list* args);
struct ParsedOWidth OWidth_ParseNV(const char* str, size_t n, va_list* args);

#endif
