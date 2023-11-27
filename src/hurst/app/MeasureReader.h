#ifndef HURST_APP_MEASUREREADER_H
#define HURST_APP_MEASUREREADER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <hurst/util/collection/Vec.h>
#include <hurst/util/io/CSVReader.h>
#include <hurst/util/time/Timestamp.h>

#include "Measure.h"
#include "MeasureReaderConfig.h"

typedef struct {
    CSVReader   csvReader;
    const char* filename;

    char*       buffer;
    size_t      bufferSize;

    const char* dateFmt;
    size_t      dateFmtLen;

    size_t      dateCol;
    size_t      valueCol;

    Timestamp   fromDate;
    Timestamp   untilDate;

    FILE*       errorFile;

    char        colSep;

    bool        ignoreDate;
    bool        ignoreBad;

    bool        error;
} MeasureReader;

void MeasureReader_init(MeasureReader* reader);

void MeasureReader_initWithConfig(MeasureReader* reader, const struct MeasureReaderConfig* config);

void MeasureReader_deinit(MeasureReader* reader);

const char* MeasureReader_getFilename(const MeasureReader* reader);
void MeasureReader_openFile(MeasureReader* reader, const char* filename);
void MeasureReader_closeFile(MeasureReader* reader);
bool MeasureReader_isFileOpen(const MeasureReader* reader);

const char* MeasureReader_getDateFmt(const MeasureReader* reader);
void MeasureReader_setDateFmt(MeasureReader* reader, const char* dateFmt);

size_t MeasureReader_getDateFmtLen(const MeasureReader* reader);
void MeasureReader_setDateFmtLen(MeasureReader* reader, size_t dateFmtLen);

size_t MeasureReader_getDateCol(const MeasureReader* reader);
void MeasureReader_setDateCol(MeasureReader* reader, size_t dateCol);

char MeasureReader_getColSep(const MeasureReader* reader);
void MeasureReader_setColSep(MeasureReader* reader, char colSep);

size_t MeasureReader_getValueCol(const MeasureReader* reader);
void MeasureReader_setValueCol(MeasureReader* reader, size_t valueCol);

Timestamp MeasureReader_getFromDate(const MeasureReader* reader);
void MeasureReader_setFromDate(MeasureReader* reader, Timestamp fromDate);

Timestamp MeasureReader_getUntilDate(const MeasureReader* reader);
void MeasureReader_setUntilDate(MeasureReader* reader, Timestamp untilDate);

FILE* MeasureReader_getErrorFile(const MeasureReader* reader);
void MeasureReader_setErrorFile(MeasureReader* reader, FILE* errorFile);

bool MeasureReader_getIgnoreDate(const MeasureReader* reader);
void MeasureReader_setIgnoreDate(MeasureReader* reader, bool ignoreDate);

bool MeasureReader_getIgnoreBad(const MeasureReader* reader);
void MeasureReader_setIgnoreBad(MeasureReader* reader, bool ignoreBad);

bool MeasureReader_getError(const MeasureReader* reader);
void MeasureReader_clearError(MeasureReader* reader);

bool MeasureReader_willIgnoreCol(const MeasureReader* reader, size_t col);

Vec MeasureReader_readAll(MeasureReader* reader);
struct Measure MeasureReader_read(MeasureReader* reader);
bool MeasureReader_canRead(const MeasureReader* reader);

bool MeasureReader_isValid(const MeasureReader* reader);

#endif
