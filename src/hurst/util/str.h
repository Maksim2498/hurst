#ifndef HURST_UTIL_STR_H
#define HURST_UTIL_STR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <hurst/util/fmt/input/FloatIFmt.h>
#include <hurst/util/fmt/input/IntIFmt.h>
#include <hurst/util/fmt/input/ParsedFloat.h>
#include <hurst/util/fmt/input/ParsedInt.h>

size_t evalStrLen(const char* str);


int cmpStrZZ(const char* lhs, const char* rhs);
int cmpStrZN(const char* lhs, const char* rhs, size_t rhsLen);
int cmpStrNZ(const char* lhs, size_t lhsLen, const char* rhs);
int cmpStrNN(const char* lhs, size_t lhsLen, const char* rhs, size_t rhsLen);


const char* findCharInStr(char c, const char* str);
const char* findCharInStrN(char c, const char* str, size_t n);


struct ParsedInt parseIntStr(const char* str);
struct ParsedInt parseIntStrN(const char* str, size_t n);

struct ParsedInt parseIntStrFmt(const char* str, const struct IntIFmt* fmt);
struct ParsedInt parseIntStrNFmt(const char* str, size_t n, const struct IntIFmt* fmt);


struct ParsedFloat parseFloatStr(const char* str);
struct ParsedFloat parseFloatStrN(const char* str, size_t n);

struct ParsedFloat parseFloatStrFmt(const char* str, const struct FloatIFmt* fmt);
struct ParsedFloat parseFloatStrNFmt(const char* str, size_t n, const struct FloatIFmt* fmt);

#endif
