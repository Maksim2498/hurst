#ifndef HURST_APP_APP_H
#define HURST_APP_APP_H

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#include "AppConfig.h"

typedef struct {
    struct AppConfig config;
    FILE*            outFile;
    FILE*            errorFile;
    bool             error;
} App;

void App_init(App* app);
void App_initWithFiles(App* app, FILE* outFile, FILE* errorFile);

void App_parseArgs(App* app, const char* const* args, size_t argCount);
void App_run(const App* app);

const struct AppConfig* App_getCConfig(const App* app);
struct AppConfig* App_getConfig(App* app);

bool App_getError(const App* app);
void App_clearError(App* app);

bool App_isValid(const App* app);

#endif
