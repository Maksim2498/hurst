#ifndef HURST_UTIL_PREPROC_H
#define HURST_UTIL_PREPROC_H

#define STRINGIFY(token) #token
#define STRINGIFY_MACRO(macro) STRINGIFY(macro)

#define SIGN(value) ((value) < 0 ? -1 : (value) > 0 ? 1 : 0)

#define MAX(lhs, rhs) ((lhs) >= (rhs) ? (lhs) : (rhs))
#define MIN(lhs, rhs) ((lhs) <= (rhs) ? (lhs) : (rhs))

#define ABS(value) ((value) >= 0 ? (value) : -(value))

#endif
