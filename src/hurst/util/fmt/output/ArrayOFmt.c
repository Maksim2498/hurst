#include "ArrayOFmt.h"

#include <assert.h>

#include <hurst/util/str.h>

const struct ArrayOFmt ARRAY_OFMT_DEFAULT = {
    .open     = "[",
    .openLen  = 1,

    .sep      = ", ",
    .sepLen   = 2,

    .close    = "]",
    .closeLen = 1,
};

struct ParsedArrayOFmt ArrayOFmt_Parse(const char* str) {
    return ArrayOFmt_ParseN(str, evalStrLen(str));
}

struct ParsedArrayOFmt ArrayOFmt_ParseN(const char* str, size_t n) {
    assert(str);

    struct ArrayOFmt fmt   = ARRAY_OFMT_DEFAULT;
    size_t           len   = 0;
    bool             valid = true;

    #define ERROR() do { len = 0; valid = false; goto quit; } while (false);

    if (n < 2 || '[' != str[0])
        ERROR();

    ++len;

    if (']' == str[1]) {
        ++len;
        goto quit;
    }

    unsigned    fmtPartId    = 0;
    const char* fmtPartBegin = str + len;

    while (len < n) {
        char c    = str[len];
        bool stop = false;

        switch (c) {
            case ']':
                stop = true;

            case ';': {
                size_t fmtPartLen = (str + len++) - fmtPartBegin;

                switch (fmtPartId++) {
                    case 0: // Open
                        fmt.open    = fmtPartBegin;
                        fmt.openLen = fmtPartLen;

                        break;

                    case 1: // Sep
                        fmt.sep    = fmtPartBegin;
                        fmt.sepLen = fmtPartLen;

                        break;

                    case 2: // Close
                        if (!stop)
                            ERROR();

                        fmt.close    = fmtPartBegin;
                        fmt.closeLen = fmtPartLen;

                        break;

                    default:
                        assert(false);
                        ERROR();
                }

                if (stop)
                    goto quit;

                fmtPartBegin = str + len;

                break;
            }

            case '\\':
                ++len;

            default:
                ++len;
        }
    }

    ERROR();

    quit:
    return (struct ParsedArrayOFmt) {
        .len   = len,
        .valid = valid,
        .value = fmt,
    };
}
