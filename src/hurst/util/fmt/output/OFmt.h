#ifndef HURST_UTIL_FMT_OUTPUT_OFMT_H
#define HURST_UTIL_FMT_OUTPUT_OFMT_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include <hurst/util/fmt/FmtType.h>

#include "IntOFmt.h"
#include "OFmt_fwd.h"
#include "ParsedOFmt.h"

struct ParsedOFmt OFmt_Parse(const char* fmt, ...);
struct ParsedOFmt OFmt_ParseN(const char* fmt, size_t n, ...);
struct ParsedOFmt OFmt_ParseV(const char* fmt, va_list* args);
struct ParsedOFmt OFmt_ParseNV(const char* fmt, size_t n, va_list* args);

struct IntOFmt OFmt_toIntOFmt(const struct OFmt* fmt);

bool OFmt_isValid(const struct OFmt* fmt);

#endif
