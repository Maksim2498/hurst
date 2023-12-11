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

void applyToDArray(double* array, size_t len, ApplyD apply) {
    assert(array && apply);

    for (double *it = array, *end = array + len; it < end; ++it)
        *it = apply(*it);
}

struct Line evalDArrayLinReg(const double* x, const double* y, size_t len) {
    const double xSum      = evalDArraySum(x, len);
    const double ySum      = evalDArraySum(y, len);
    const double xyProdSum = evalDArraysProdSum(x, y, len);
    const double xProdSum  = evalDArraysProdSum(x, x, len);

    const double slope     = (len * xyProdSum - xSum * ySum)
                             /
                             (len * xProdSum - xSum * xSum);

    const double offset    = (ySum - slope * xSum) / len;

    return (struct Line) {
        .offset = offset,
        .slope  = slope,
    };
}

double evalDArraysProdSum(const double* lhs, const double* rhs, size_t len) {
    assert(lhs && rhs);

    double sum = 0;

    for (size_t i = 0; i < len; ++i)
        sum += lhs[i] * rhs[i];

    return sum;
}

double evalDArrayStd(const double* array, size_t len) {
    return sqrt(evalDArrayDisp(array, len));
}

double evalDArrayDisp(const double* array, size_t len) {
    const double avg = evalDArrayAvg(array, len);

    double sum = 0;

    for (size_t i = 0; i < len; ++i) {
        const double diff = array[i] - avg;

        sum += diff * diff;
    }

    return sum / len;
}

double evalDArrayAvg(const double* array, size_t len) {
    return evalDArraySum(array, len) / len;
}

double evalDArraySum(const double* array, size_t len) {
    assert(array);

    if (!len)
        return 0;

    double sum = array[0];

    for (size_t i = 1; i < len; ++i)
        sum += array[i];

    return sum;
}

void sortArray(const void* array, size_t len, Cmp cmp) {
    assert(array && cmp);

    // TODO
}
