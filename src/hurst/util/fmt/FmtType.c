#include "FmtType.h"

#include <assert.h>

struct ParsedFmtType FmtType_Parse(const char* str) {
    size_t strLen = 0;

    while (str[strLen] && strLen < FMT_TYPE_MAX_LEN)
        ++strLen;

    return FmtType_ParseN(str, strLen);
}

struct ParsedFmtType FmtType_ParseN(const char* str, size_t strLen) {
    assert(str);

    unsigned char type;
    unsigned char len   = 0;
    bool          valid = false;

    int state = 0;

    for (size_t i = 0; i < strLen; ++i) {
        char c = str[i];

        switch (state) {
            // parsed: ""
            // type:   <unknown>
            case 0:
                switch (c) {
                    case 'e':
                        state = 9;
                        continue;

                    case 'b':
                        type  = FMT_TYPE_BOOL;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'c':
                        type  = FMT_TYPE_CHAR;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 's':
                        type  = FMT_TYPE_STR;
                        len   = 1;
                        valid = true;
                        state = 8;
                        continue;

                    case 'i':
                        type  = FMT_TYPE_INT;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'u':
                        type  = FMT_TYPE_UINT;
                        len   = 1;
                        valid = true;
                        state = 1;
                        continue;

                    case 'l':
                        type  = FMT_TYPE_LINT;
                        len   = 1;
                        valid = true;
                        state = 4;
                        continue;

                    case 'd':
                        type  = FMT_TYPE_DOUBLE;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'D':
                        type  = FMT_TYPE_PTRDIFF;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'z':
                        type  = FMT_TYPE_SIZE;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'm':
                        type  = FMT_TYPE_MAX;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 'f':
                        type  = FMT_TYPE_FLOAT;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case 't':
                        type  = FMT_TYPE_TIMESTAMP;
                        len   = 1;
                        valid = true;
                        goto quit;

                    case '"':
                        state = 10;
                        continue;

                    case '\'':
                        state = 11;
                        continue;

                    case 'S':
                        type  = FMT_TYPE_SINT;
                        len   = 1;
                        valid = true;
                        state = 6;
                        continue;

                    default:
                        goto quit;
                }

            // parsed: "u"
            // type:   UINT
            case 1:
                switch (c) {
                    case 'i':
                        len = 2;
                        goto quit;

                    case 'c':
                        type = FMT_TYPE_UCHAR;
                        len  = 2;
                        goto quit;

                    case 'l':
                        type  = FMT_TYPE_ULINT;
                        len   = 2;
                        state = 2;
                        continue;

                    case 'S':
                        type  = FMT_TYPE_USINT;
                        len   = 2;
                        state = 7;
                        continue;

                    case 'm':
                        type = FMT_TYPE_UMAX;
                        len  = 2;

                    default:
                        goto quit;
                }

            // parsed: "ul"
            // type:   ULINT
            case 2:
                switch (c) {
                    case 'l':
                        type  = FMT_TYPE_ULLINT;
                        len   = 3;
                        state = 3;
                        continue;

                    case 'i':
                        len = 3;

                    default:
                        goto quit;
                }

            // parsed: "ull"
            // type:   ULLINT
            case 3:
                switch (c) {
                    case 'i':
                        len = 4;

                    default:
                        goto quit;
                }

            // parsed: "l"
            // type:   LINT
            case 4:
                switch (c) {
                    case 'l':
                        type  = FMT_TYPE_LLINT;
                        len   = 2;
                        state = 5;
                        continue;

                    case 'i':
                        len = 2;

                    default:
                        goto quit;
                }

            // parsed: "ll"
            // type:   LLINT
            case 5:
                switch (c) {
                    case 'i':
                        len = 3;

                    default:
                        goto quit;
                }

            // parsed: "S"
            // type:   SINT
            case 6:
                switch (c) {
                    case 'i':
                        len = 2;

                    default:
                        goto quit;
                }
                
            // parsed: "uS"
            // type:   USINT
            case 7:
                switch (c) {
                    case 'i':
                        len = 3;

                    default:
                        goto quit;
                }

            // parsed: "s"
            // type:   STR
            case 8:
                switch (c) {
                    case 'c':
                        type = FMT_TYPE_SCHAR;
                        len  = 2;

                    default:
                        goto quit;
                }

            // parsed: "s"
            // type:   <unknown>
            case 9:
                switch (c) {
                    case 'c':
                        type  = FMT_TYPE_ESC_CHAR;
                        len   = 2;
                        valid = true;
                        goto quit;

                    case 's':
                        type  = FMT_TYPE_ESC_STR;
                        len   = 2;
                        valid = true;

                    default:
                        goto quit;
                }

            // parsed: "\""
            // type:   <unknown>
            case 10:
                switch (c) {
                    case 's':
                        type  = FMT_TYPE_DQUOTED_STR;
                        len   = 2;
                        valid = true;
                        goto quit;

                    case 'c':
                        type  = FMT_TYPE_DQUOTED_CHAR;
                        len   = 2;
                        valid = true;

                    default:
                        goto quit;
                }

            // parsed: "'"
            // type:   <unknown>
            case 11:
                switch (c) {
                    case 's':
                        type  = FMT_TYPE_QUOTED_STR;
                        len   = 2;
                        valid = true;
                        goto quit;

                    case 'c':
                        type  = FMT_TYPE_QUOTED_CHAR;
                        len   = 2;
                        valid = true;

                    default:
                        goto quit;
                }

            default:
                assert(false);
        }
    }

    quit:
    return (struct ParsedFmtType) {
        .value = type,
        .len   = len,
        .valid = valid,
    };
}

const char* FmtType_getName(enum FmtType type) {
    assert(FmtType_isValid(type));

    switch (type) {
        case FMT_TYPE_BOOL:
            return "FMT_TYPE_BOOL";

        case FMT_TYPE_QUOTED_CHAR:
            return "FMT_TYPE_QUOTED_CHAR";

        case FMT_TYPE_DQUOTED_CHAR:
            return "FMT_TYPE_DQUOTED_CHAR";

        case FMT_TYPE_ESC_CHAR:
            return "FMT_TYPE_ESC_CHAR";

        case FMT_TYPE_CHAR:
            return "FMT_TYPE_CHAR";

        case FMT_TYPE_QUOTED_STR:
            return "FMT_TYPE_QUOTED_STR";

        case FMT_TYPE_DQUOTED_STR:
            return "FMT_TYPE_DQUOTED_STR";

        case FMT_TYPE_ESC_STR:
            return "FMT_TYPE_ESC_STR";

        case FMT_TYPE_STR:
            return "FMT_TYPE_STR";

        case FMT_TYPE_SCHAR:
            return "FMT_TYPE_SCHAR";

        case FMT_TYPE_UCHAR:
            return "FMT_TYPE_UCHAR";

        case FMT_TYPE_SINT:
            return "FMT_TYPE_SINT";

        case FMT_TYPE_USINT:
            return "FMT_TYPE_USINT";

        case FMT_TYPE_INT:
            return "FMT_TYPE_INT";

        case FMT_TYPE_UINT:
            return "FMT_TYPE_UINT";

        case FMT_TYPE_LINT:
            return "FMT_TYPE_LINT";

        case FMT_TYPE_ULINT:
            return "FMT_TYPE_ULINT";

        case FMT_TYPE_LLINT:
            return "FMT_TYPE_LLINT";

        case FMT_TYPE_ULLINT:
            return "FMT_TYPE_ULLINT";

        case FMT_TYPE_PTRDIFF:
            return "FMT_TYPE_PTRDIFF";

        case FMT_TYPE_SIZE:
            return "FMT_TYPE_SIZE";

        case FMT_TYPE_MAX:
            return "FMT_TYPE_MAX";

        case FMT_TYPE_UMAX:
            return "FMT_TYPE_UMAX";

        case FMT_TYPE_FLOAT:
            return "FMT_TYPE_FLOAT";

        case FMT_TYPE_DOUBLE:
            return "FMT_TYPE_DOUBLE";

        case FMT_TYPE_TIMESTAMP:
            return "FMT_TYPE_TIMESTAMP";

        default:
            return "<Bad FmtType>";
    }
}

bool FmtType_isValid(enum FmtType type) {
    return FMT_TYPE_BOOL      <= type
        && FMT_TYPE_TIMESTAMP >= type;
}
