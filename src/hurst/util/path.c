#include "path.h"

#include <assert.h>

const char* getBasename(const char* path) {
    assert(path);

    const char* basename = path;

    for (char c; (c = *path); ++path)
        if (PATH_SEP == c)
            basename = path + 1;

    return basename;
}
