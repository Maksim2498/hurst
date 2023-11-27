#include "OCase.h"

#include <assert.h>

const char* OCase_getName(enum OCase c) {
    assert(OCase_isValid(c));

    switch (c) {
        case OCASE_UPPER:
            return "OCASE_UPPER";

        case OCASE_LOWER:
            return "OCASE_LOWER";

        default:
            assert(false);
            return "<Bad OCase>";
    }
}

bool OCase_isValid(enum OCase c) {
    return OCASE_LOWER == c
        || OCASE_UPPER == c;
}
