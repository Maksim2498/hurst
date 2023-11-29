#include "array.h"

#include <assert.h>
#include <math.h>

bool allNotNull(const void* const* array, size_t len) {
    if (!array)
        return false;

    for (size_t i = 0; i < len; ++i)
        if (!array[i])
            return false;

    return true;
}

double evalStd(const double* array, size_t len) {
    const double avg = evalAvg(array, len);

    double sum = 0;

    for (size_t i = 0; i < len; ++i) {
        const double diff = array[i] - avg;

        sum += diff * diff;
    }

    return sqrt(sum / len);
}

double evalAvg(const double* array, size_t len) {
    return evalSum(array, len) / len;
}

double evalSum(const double* array, size_t len) {
    assert(array);

    if (!len)
        return 0;

    double sum = array[0];

    for (size_t i = 1; i < len; ++i)
        sum += array[i];

    return sum;
}
