#include "Measure.h"

#include <assert.h>

#include <hurst/util/io/print.h>
#include <hurst/util/macros.h>

int Measure_cmpByDate(const struct Measure* lhs, const struct Measure* rhs) {
    assert(lhs && rhs);

    Timestamp diff = lhs->date - rhs->date;

    return SIGN(diff);
}

size_t printMeasure(FILE* file, const struct Measure* measure) {
    assert(measure);

    return printFmt(
        file,
        "(%t, %d)",
        measure->date, measure->value
    );
}
