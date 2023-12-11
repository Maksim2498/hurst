#include "App.h"

#include <assert.h>
#include <math.h>

#include <hurst/util/collection/Vec.h>
#include <hurst/util/io/print.h>
#include <hurst/util/opt/Opt.h>
#include <hurst/util/array.h>
#include <hurst/util/path.h>
#include <hurst/util/str.h>
#include <hurst/config.h>

#include "AppConfigParser.h"
#include "MeasureReader.h"
#include "MeasureReaderConfig.h"


static size_t App_printHelp_(const App* app, const char* appPath);
static size_t App_printUsage_(const App* app, const char* appPath);
static size_t App_printOptsHelp_(const App* app, size_t offset);
static size_t App_printDateFmtHelp_(const App* app);
static size_t App_printVersion_(const App* app);
static size_t App_printConfig_(const App* app);
static size_t App_printMissingFiles_(const App* app);

static void App_printAllFilesHurstExps_(const App* app);
static void App_printFileHurstExp_(const App* app, MeasureReader* measureReader, size_t filenameIndex);
static double App_evalHurstExp_(const App* app, const Vec* measures);


void App_init(App* app) {
    App_initWithFiles(app, stdout, stderr);
}

void App_initWithFiles(App* app, FILE* outFile, FILE* errorFile) {
    assert(app);

    app->config    = APP_CONFIG_DEFAULT;
    app->outFile   = outFile;
    app->errorFile = errorFile;
    app->error     = false;
}

void App_parseArgs(App* app, const char* const* args, size_t argCount) {
    if (App_getError(app))
        return;

    AppConfigParser configParser;

    AppConfigParser_initEx(&configParser, args + 1, argCount - 1, app->errorFile);

    const struct ParsedAppConfig parsedConfig = AppConfigParser_parse(&configParser);

    if (!parsedConfig.valid) {
        app->error = true;
        return;
    }

    app->config = parsedConfig.value;

    if (app->config.showHelp) {
        App_printHelp_(app, *args);
        return;
    }

    if (app->config.showDateFmtHelp) {
        App_printDateFmtHelp_(app);
        return;
    }

    if (app->config.showVersion) {
        App_printVersion_(app);
        return;
    }

    if (app->config.showConfig) {
        App_printConfig_(app);

        if (app->outFile)
            printChar(app->outFile, '\n');
    }

    if (!app->config.filenames || !app->config.filenameCount) {
        App_printMissingFiles_(app);
        app->error = true;
        return;
    }
}

size_t App_printHelp_(const App* app, const char* appPath) {
    size_t printed = 0;

    printed += App_printUsage_(app, appPath);
    printed += printCharN(app->outFile, '\n', 2);
    printed += printStr(app->outFile, "Where options include:\n\n");
    printed += App_printOptsHelp_(app, 4);
    printed += printChar(app->outFile, '\n');

    return printed;
}

size_t App_printUsage_(const App* app, const char* appPath) {
    assert(App_isValid(app) && appPath);

    return app->outFile
         ? printFmt(app->outFile, "Usage: %s [options] <CSV filenames>", getBasename(appPath))
         : 0;
}

size_t App_printOptsHelp_(const App* app, size_t offset) {
    assert(App_isValid(app));

    if (!app->outFile)
        return 0;

    const struct OptsHelpFmt OPTS_HELP_FMT = {
        .descriptionPadding = 1,
        .offset             = offset,
    };

    return printOptsHelpFmt(
        app->outFile,
        APP_CONFIG_PARSER_OPTS,
        APP_CONFIG_PARSER_OPT_COUNT,
        &OPTS_HELP_FMT
    );
}

size_t App_printDateFmtHelp_(const App* app) {
    assert(App_isValid(app));

    if (!app->outFile)
        return 0;

    return printStr(
        app->outFile,

        "Date format syntax: {%<duration specifier>|<exact>|<space>}\n"
        "\n"
        "<exact> character will only match the same character.\n"
        "\n"
        "<space> character will match any number of space characters.\n"
        "\n"
        "<duration specifier> after '%' character will try to parse a\n"
        "duration component and add it to the result. Any unknown\n"
        "specifier will be treated as an exact character.\n"
        "\n"
        "+-----------+------------------+\n"
        "| Duration  | Description      |\n"
        "| Specifier |                  |\n"
        "+-----------+------------------+\n"
        "| d         | Day of year      |\n"
        "| M         | Month of year    |\n"
        "| y         | Year             |\n"
        "| h         | Hour of day      |\n"
        "| m         | Minute of hour   |\n"
        "| s         | Second of minute |\n"
        "+-----------+------------------+\n"
    );
}

size_t App_printVersion_(const App* app) {
    assert(App_isValid(app));

    return app->outFile
         ? printFmt(app->outFile, "%s\n", VERSION)
         : 0;
}

size_t App_printConfig_(const App* app) {
    assert(App_isValid(app));

    return app->outFile
         ? printAppConfig(app->outFile, &app->config)
         : 0;
}

size_t App_printMissingFiles_(const App* app) {
    assert(App_isValid(app));

    return app->errorFile
         ? printStr(app->errorFile, "Missing files to process\n")
         : 0;
}

void App_run(const App* app) {
    if (App_getError(app))
        return;

    App_printAllFilesHurstExps_(app);
}

void App_printAllFilesHurstExps_(const App* app) {
    assert(App_isValid(app));

    if (!app->outFile && !app->errorFile)
        return;

    MeasureReader measureReader;

    struct MeasureReaderConfig measureReaderConfig = MeasureReaderConfig_FromAppConfig(&app->config);

    measureReaderConfig.errorFile = app->errorFile;

    MeasureReader_initWithConfig(&measureReader, &measureReaderConfig);

    for (size_t i = 0; i < app->config.filenameCount; ++i)
        App_printFileHurstExp_(app, &measureReader, i);

    MeasureReader_deinit(&measureReader);
}

void App_printFileHurstExp_(const App* app, MeasureReader* measureReader, size_t filenameIndex) {
    assert(
        App_isValid(app)                     &&
        MeasureReader_isValid(measureReader) &&
        filenameIndex < app->config.filenameCount
    );

    const char* filename = app->config.filenames[filenameIndex];

    MeasureReader_openFile(measureReader, filename);

    if (MeasureReader_getError(measureReader))
        return;

    Vec measures = MeasureReader_readAll(measureReader);

    if (!MeasureReader_getError(measureReader) && app->outFile) {
        if (app->config.sort)
            Vec_sort(&measures, (Cmp) Measure_cmpByDate);

        const double hurstExp = App_evalHurstExp_(app, &measures);

        printFmt(app->outFile, "%d - %s\n", hurstExp, filename);
    }

    Vec_deinit(&measures);
}

double App_evalHurstExp_(const App* app, const Vec* measures) {
    assert(App_isValid(app) && Vec_isValid(measures));

    const size_t measuresLen = Vec_getLen(measures);
    const size_t maxIncLen   = app->config.maxIncLen;
    const size_t incLen      = measuresLen >= maxIncLen
                             ? maxIncLen
                             : measuresLen;
    const size_t dxLen       = measuresLen - incLen;

    Vec dx;
    Vec xi;
    Vec eta;

    const double ZERO = 0;

    Vec_initFilled(&dx,  sizeof(double), &ZERO, dxLen );
    Vec_initFilled(&xi,  sizeof(double), &ZERO, incLen);
    Vec_initFilled(&eta, sizeof(double), &ZERO, incLen);

    for (size_t p = 0; p < incLen; ++p) {
        for (size_t i = 0; i < dxLen; ++i) {
            const struct Measure* measure    = Vec_getCAt(measures, i        );
            const struct Measure* incMeasure = Vec_getCAt(measures, i + p + 1);
            const double          curDx      = incMeasure->value - measure->value;

            Vec_setAt(&dx, &curDx, i);
        }

        const double* dxItems = Vec_getCItems(&dx);
        const double  std     = evalDArrayStd(dxItems, dxLen);
        const double  curEta  = log(std);
        const double  curXi   = log(p + 1);

        Vec_setAt(&eta, &curEta, p);
        Vec_setAt(&xi,  &curXi,  p);
    }

    struct Line line = evalDArrayLinReg(
        Vec_getCItems(&xi ),
        Vec_getCItems(&eta),
        incLen
    );

    return line.slope;
}

const struct AppConfig* App_getCConfig(const App* app) {
    assert(App_isValid(app));
    return &app->config;
}

struct AppConfig* App_getConfig(App* app) {
    assert(App_isValid(app));
    return &app->config;
}

bool App_getError(const App* app) {
    assert(App_isValid(app));
    return app->error;
}

void App_clearError(App* app) {
    assert(App_isValid(app));
    app->error = false;
}

bool App_isValid(const App* app) {
    return app
        && AppConfig_isValid(&app->config);
}
