#ifndef HURST_UTIL_FMT_OUTPUT_ARRAYOFMT_H
#define HURST_UTIL_FMT_OUTPUT_ARRAYOFMT_H

#include <stddef.h>

#include "ArrayOFmt_fwd.h"
#include "ParsedArrayOFmt.h"

extern const struct ArrayOFmt ARRAY_OFMT_DEFAULT;

struct ParsedArrayOFmt ArrayOFmt_Parse(const char* fmt);
struct ParsedArrayOFmt ArrayOFmt_ParseN(const char* fmt, size_t n);

#endif
