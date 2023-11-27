#include "OptParseStatus.h"

#include <assert.h>

const char* OptParseStatus_getName(enum OptParseStatus status) {
    assert(OptParseStatus_isValid(status));

    switch (status) {
        case OPT_PARSE_STATUS_OK:
            return "OPT_PARSE_STATUS_OK";

        case OPT_PARSE_STATUS_END:
            return "OPT_PARSE_STATUS_END";

        case OPT_PARSE_STATUS_NOT_AN_OPT:
            return "OPT_PARSE_STATUS_NOT_AN_OPT";

        case OPT_PARSE_STATUS_UNKNOWN:
            return "OPT_PARSE_STATUS_UNKNOWN";

        case OPT_PARSE_STATUS_MISSING_ARG:
            return "OPT_PARSE_STATUS_MISSING_ARG";

        default:
            assert(false);
            return "<Bad OptParseStatus>";
    }
}

bool OptParseStatus_isValid(enum OptParseStatus status) {
    return OPT_PARSE_STATUS_OK          <= status
        && OPT_PARSE_STATUS_MISSING_ARG >= status;
}
