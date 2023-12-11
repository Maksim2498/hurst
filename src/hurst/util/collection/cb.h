#ifndef HURST_UTIL_COLLECTION_CB_H
#define HURST_UTIL_COLLECTION_CB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Apply

typedef bool               (*ApplyB   )(bool);

typedef char               (*ApplyC   )(char              );

typedef signed char        (*ApplySC  )(signed char       );
typedef unsigned char      (*ApplyUC  )(unsigned char     );
typedef short              (*ApplySI  )(short             );
typedef unsigned short     (*ApplyUSI )(unsigned short    );
typedef int                (*ApplyI   )(int               );
typedef unsigned           (*ApplyUI  )(unsigned          );
typedef long               (*ApplyLI  )(long              );
typedef unsigned long      (*ApplyULI )(unsigned long     );
typedef long long          (*ApplyLLI )(long long         );
typedef unsigned long long (*ApplyULLI)(unsigned long long);

typedef float              (*ApplyF   )(float             );
typedef double             (*ApplyD   )(double            );
typedef long double        (*ApplyLD  )(long double       );

typedef size_t             (*ApplyZ   )(size_t            );
typedef ptrdiff_t          (*ApplyP   )(ptrdiff_t         );

typedef intmax_t           (*ApplyM   )(intmax_t          );
typedef uintmax_t          (*ApplyUM  )(uintmax_t         );

// Cmp

typedef int (*Cmp)(const void*, const void*);

#endif
