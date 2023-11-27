#include "Radix.h"

bool Radix_isValid(Radix radix) {
    return 2 <= radix && radix <= 36;
}
