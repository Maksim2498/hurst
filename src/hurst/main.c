#include <stdlib.h>

#include "all.h"

int main(int argc, char** argv) {
    App app;

    App_init(&app);
    App_parseArgs(&app, (const char* const*) argv, argc);
    App_run(&app);

    return App_getError(&app) ? EXIT_FAILURE
                              : EXIT_SUCCESS;
}
