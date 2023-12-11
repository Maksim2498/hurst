#include "array.h"

#include <assert.h>
#include <string.h>
#include <math.h>


static char* mergeSortArray_(char* first, char* second, size_t len, size_t itemSize, Cmp cmp);


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

void sortArray(void* array, void* buffer, size_t len, size_t itemSize, Cmp cmp) {
    void* last = mergeSortArray_(array, buffer, len, itemSize, cmp);

    if (last != array)
        memcpy(array, buffer, len * itemSize);
}

char* mergeSortArray_(char* first, char* second, size_t len, size_t itemSize, Cmp cmp) {
    assert(first && second && itemSize && cmp);

    if (len <= 1)
        return first;

    const size_t leftLen   = len / 2;
    const size_t rightLen  = len - leftLen;
    const size_t leftSize  = leftLen  * itemSize;
    const size_t rightSize = rightLen * itemSize;

    char* left     = mergeSortArray_(first,            second,            leftLen,  itemSize, cmp);
    char* right    = mergeSortArray_(first + leftSize, second + leftSize, rightLen, itemSize, cmp);
    char* target   = left == first ? second : first;

    char* leftEnd  = left  + leftSize;
    char* rightEnd = right + rightSize;

    char* leftIt   = left;
    char* rightIt  = right;
    char* targetIt = target;

    #define PUSH_LEFT()                         \
        do {                                    \
            memcpy(targetIt, leftIt, itemSize); \
            leftIt += itemSize;                 \
        } while (0)

    #define PUSH_RIGHT()                         \
        do {                                     \
            memcpy(targetIt, rightIt, itemSize); \
            rightIt += itemSize;                 \
        } while (0)

    for (size_t i = 0; i < len; ++i, targetIt += itemSize) {
        if (leftIt < leftEnd && rightIt < rightEnd) {
            if (cmp(leftIt, rightIt) <= 0)
                PUSH_LEFT();
            else
                PUSH_RIGHT();

            continue;
        }

        if (leftIt < leftEnd) {
            PUSH_LEFT();
            continue;
        }

        PUSH_RIGHT();
    }

    #undef PUSH_LEFT
    #undef PUSH_RIGHT

    return target;
}
