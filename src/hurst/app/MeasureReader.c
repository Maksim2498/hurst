#include "MeasureReader.h"

#include <assert.h>
#include <ctype.h>

#include <hurst/util/io/print.h>
#include <hurst/util/time/Timestamp.h>
#include <hurst/util/macros.h>
#include <hurst/util/mem.h>
#include <hurst/util/str.h>


static void MeasureReader_freeBuffer_(MeasureReader* reader);
static void MeasureReader_raiseBufferTo_(MeasureReader* reader, size_t to);
static size_t MeasureReader_printInvalidNumberOfColumns_(const MeasureReader* reader, size_t line);
static size_t MeasureReader_printBadDate_(const MeasureReader* reader, size_t valueSize, size_t line, size_t col);
static size_t MeasureReader_printBadValue_(const MeasureReader* reader, size_t valueSize, size_t line, size_t col);


void MeasureReader_init(MeasureReader* reader) {
    struct MeasureReaderConfig config = MEASURE_READER_CONFIG_DEFAULT;

    config.errorFile = stderr;

    MeasureReader_initWithConfig(reader, &config);
}

void MeasureReader_initWithConfig(MeasureReader* reader, const struct MeasureReaderConfig* config) {
    assert(reader && MeasureReaderConfig_isValid(config));

    reader->filename   = NULL;

    reader->buffer     = forceAllocMem(config->initBufferSize);
    reader->bufferSize = config->initBufferSize;

    reader->dateFmt    = config->dateFmt;
    reader->dateFmtLen = config->dateFmtLen;

    reader->dateCol    = config->dateCol;
    reader->valueCol   = config->valueCol;

    reader->fromDate   = config->fromDate;
    reader->untilDate  = config->untilDate;

    reader->errorFile  = config->errorFile;

    reader->colSep     = config->colSep;

    reader->ignoreDate = config->ignoreDate;
    reader->ignoreBad  = config->ignoreBad;

    reader->error      = false;

    if (config->filename)
        MeasureReader_openFile(reader, config->filename);
}

void MeasureReader_deinit(MeasureReader* reader) {
    MeasureReader_closeFile(reader);
    MeasureReader_freeBuffer_(reader);
}

void MeasureReader_freeBuffer_(MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));

    forceFreeMem(reader->buffer);

    reader->buffer     = NULL;
    reader->bufferSize = 0;
}

const char* MeasureReader_getFilename(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->filename;
}

void MeasureReader_openFile(MeasureReader* reader, const char* filename) {
    MeasureReader_closeFile(reader);

    FILE* file = fopen(filename, "r");

    if (!file) {
        if (reader->errorFile)
            printFmt(reader->errorFile, "Failed to open a file %s\n", filename);

        reader->error = true;

        return;
    }

    CSVReader csvReader;

    CSVReader_initWithSep(&csvReader, file, reader->colSep);

    if (CSVReader_getError(&csvReader)) {
        if (reader->errorFile)
            printFmt(reader->errorFile, "%s is a malformed CSV file\n", filename);

        CSVReader_closeFile(&csvReader);

        reader->error = true;

        return;
    }

    const size_t colCount         = CSVReader_getColCount(&csvReader);
    const size_t requiredColCount = 1 + MAX(reader->dateCol, reader->valueCol);

    if (colCount < requiredColCount) {
        if (reader->errorFile)
            printFmt(
                reader->errorFile,
                
                "%s hasn't enough columns\n"
                "It has %z columns but a minimum of %z columns is required\n",
                
                filename,
                colCount, requiredColCount
            );

        CSVReader_closeFile(&csvReader);

        reader->error = true;

        return;
    }

    reader->csvReader = csvReader;
    reader->filename  = filename;
}

void MeasureReader_closeFile(MeasureReader* reader) {
    if (!MeasureReader_isFileOpen(reader))
        return;

    fclose(CSVReader_getFile(&reader->csvReader));

    reader->filename = NULL;
    reader->error    = false;
}

bool MeasureReader_isFileOpen(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->filename;
}

const char* MeasureReader_getDateFmt(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->dateFmt;
}

void MeasureReader_setDateFmt(MeasureReader* reader, const char* dateFmt) {
    assert(MeasureReader_isValid(reader) && dateFmt);
    reader->dateFmt = dateFmt;
}

size_t MeasureReader_getDateFmtLen(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->dateFmtLen;
}

void MeasureReader_setDateFmtLen(MeasureReader* reader, size_t dateFmtLen) {
    assert(MeasureReader_isValid(reader));
    reader->dateFmtLen = dateFmtLen;
}

size_t MeasureReader_getDateCol(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->dateCol;
}

void MeasureReader_setDateCol(MeasureReader* reader, size_t dateCol) {
    assert(MeasureReader_isValid(reader));
    reader->dateCol = dateCol;
}

char MeasureReader_getColSep(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->colSep;
}

void MeasureReader_setColSep(MeasureReader* reader, char colSep) {
    assert(MeasureReader_isValid(reader) && isprint(colSep));
    reader->colSep = colSep;
}

size_t MeasureReader_getValueCol(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->valueCol;
}

void MeasureReader_setValueCol(MeasureReader* reader, size_t valueCol) {
    assert(MeasureReader_isValid(reader));
    reader->valueCol = valueCol;
}

Timestamp MeasureReader_getFromDate(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->fromDate;
}

void MeasureReader_setFromDate(MeasureReader* reader, Timestamp fromDate) {
    assert(MeasureReader_isValid(reader) && TIMESTAMP_BAD != fromDate);
    reader->fromDate = fromDate;
}

Timestamp MeasureReader_getUntilDate(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->untilDate;
}

void MeasureReader_setUntilDate(MeasureReader* reader, Timestamp untilDate) {
    assert(MeasureReader_isValid(reader) && TIMESTAMP_BAD != untilDate);
    reader->untilDate = untilDate;
}

FILE* MeasureReader_getErrorFile(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->errorFile;
}

void MeasureReader_setErrorFile(MeasureReader* reader, FILE* errorFile) {
    assert(MeasureReader_isValid(reader));
    reader->errorFile = errorFile;
}

bool MeasureReader_getIgnoreDate(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->ignoreDate;
}

void MeasureReader_setIgnoreDate(MeasureReader* reader, bool ignoreDate) {
    assert(MeasureReader_isValid(reader));
    reader->ignoreDate = ignoreDate;
}

bool MeasureReader_getIgnoreBad(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return reader->ignoreBad;
}

void MeasureReader_setIgnoreBad(MeasureReader* reader, bool ignoreBad) {
    assert(MeasureReader_isValid(reader));
    reader->ignoreBad = ignoreBad;
}

bool MeasureReader_getError(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));

    return reader->error
        || CSVReader_getError(&reader->csvReader);
}

void MeasureReader_clearError(MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));

    reader->error = false;

    CSVReader_clearError(&reader->csvReader);
}

bool MeasureReader_willIgnoreCol(const MeasureReader* reader, size_t col) {
    assert(MeasureReader_isValid(reader));

    return (col != reader->dateCol || reader->ignoreDate)
        &&  col != reader->valueCol;
}

bool MeasureReader_isReachedEnd(const MeasureReader* reader) {
    assert(MeasureReader_isValid(reader));
    return CSVReader_isReachedEnd(&reader->csvReader);
}

Vec MeasureReader_readAll(MeasureReader* reader) {
    Vec measures;

    Vec_init(&measures, sizeof(struct Measure));

    while (true) {
        const struct Measure measure = MeasureReader_read(reader);
        
        if (!MeasureReader_canRead(reader))
            break;

        if (reader->fromDate <= measure.date && measure.date <= reader->untilDate)
            Vec_append(&measures, &measure);
    }

    return measures;
}

struct Measure MeasureReader_read(MeasureReader* reader) {
    struct Measure measure = { 0, 0 };

    if (!MeasureReader_canRead(reader))
        return measure;

    const size_t line = CSVReader_getLine(&reader->csvReader);

    do {
        const size_t col = CSVReader_getCol(&reader->csvReader);

        if (MeasureReader_willIgnoreCol(reader, col)) {
            CSVReader_skipValue(&reader->csvReader);

            if (CSVReader_getError(&reader->csvReader)) {
                MeasureReader_printInvalidNumberOfColumns_(reader, line);
                break;
            }

            continue;
        }

        const CSVReaderPos pos = CSVReader_getPos(&reader->csvReader);

        size_t valueSize = 0;
        size_t read      = CSVReader_readValue(
            &reader->csvReader,
            reader->buffer,
            reader->bufferSize,
            &valueSize
        );

        if (CSVReader_getError(&reader->csvReader)) {
            MeasureReader_printInvalidNumberOfColumns_(reader, line);
            break;
        }

        if (CSVReader_isReachedEnd(&reader->csvReader))
            break;

        if (valueSize > read) {
            CSVReader_setPos(&reader->csvReader, &pos);

            MeasureReader_raiseBufferTo_(reader, valueSize);

            read = CSVReader_readValue(&reader->csvReader, reader->buffer, reader->bufferSize, &valueSize);

            if (CSVReader_getError(&reader->csvReader)) {
                MeasureReader_printInvalidNumberOfColumns_(reader, line);
                break;
            }

            assert(valueSize == read);
        }

        if (col == reader->dateCol) {
            const Timestamp date = Timestamp_ParseN(
                reader->buffer,
                valueSize,
                reader->dateFmt,
                reader->dateFmtLen
            );

            if (TIMESTAMP_BAD != date) {
                measure.date = date;
                continue;
            }

            if (reader->ignoreBad)
                continue;

            if (reader->errorFile)
                MeasureReader_printBadDate_(reader, valueSize, line, col);

            reader->error = true;

            continue;
        }

        if (col == reader->valueCol) {
            const struct ParsedFloat parsedValue = parseFloatStrN(reader->buffer, valueSize);

            if (parsedValue.valid) {
                measure.value = parsedValue.value;
                continue;
            }

            if (reader->errorFile)
                MeasureReader_printBadValue_(reader, valueSize, line, col);

            reader->error = true;

            continue;
        }

        assert(false);
    } while (line == CSVReader_getLine(&reader->csvReader));

    return measure;
}

size_t MeasureReader_printInvalidNumberOfColumns_(const MeasureReader* reader, size_t line) {
    assert(MeasureReader_isValid(reader));

    return  reader->errorFile
         ?  printFmt(
                reader->errorFile,
                "Invalid number of columns at %s:%z\n",
                reader->filename, line + 1
            )
         :  0;
}

size_t MeasureReader_printBadDate_(const MeasureReader* reader, size_t valueSize, size_t line, size_t col) {
    assert(MeasureReader_isValid(reader));

    return  reader->errorFile
         ?  printFmt(
                reader->errorFile,

                "Date at %s:%z:%z is malformed: %**\"s\n",

                reader->filename, line + 1, col,
                valueSize, reader->buffer
            )
        :   0;
}

size_t MeasureReader_printBadValue_(const MeasureReader* reader, size_t valueSize, size_t line, size_t col) {
    assert(MeasureReader_isValid(reader));

    return  reader->errorFile
         ?  printFmt(
                reader->errorFile,

                "Value at %s:%z:%z is malformed: %**\"s\n",

                reader->filename, line + 1, col,
                valueSize, reader->buffer
            )
         :  0;
}

void MeasureReader_raiseBufferTo_(MeasureReader* reader, size_t to) {
    assert(MeasureReader_isValid(reader));

    while (reader->bufferSize < to)
        reader->bufferSize *= 2;

    forceReallocMem((void**) &reader->buffer, reader->bufferSize);
}

bool MeasureReader_canRead(const MeasureReader* reader) {
    return !MeasureReader_getError(reader)
        &&  MeasureReader_isFileOpen(reader)
        && !MeasureReader_isReachedEnd(reader);
}

bool MeasureReader_isValid(const MeasureReader* reader) {
    return reader
        && (!reader->filename || CSVReader_isValid(&reader->csvReader))
        && reader->buffer
        && reader->bufferSize
        && reader->dateFmt
        && TIMESTAMP_BAD != reader->fromDate
        && TIMESTAMP_BAD != reader->untilDate
        && isprint(reader->colSep);
}
