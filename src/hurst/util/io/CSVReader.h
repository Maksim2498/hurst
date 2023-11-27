#ifndef HURST_UTIL_IO_CSVREADER_H
#define HURST_UTIL_IO_CSVREADER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    FILE*  file;
    size_t line;
    size_t col;
    size_t colCount;
    char   sep;
    bool   error;
} CSVReader;

typedef struct {
    fpos_t filePos;
    size_t line;
    size_t col;
} CSVReaderPos;

void CSVReader_init(CSVReader* reader, FILE* file);
void CSVReader_initWithSep(CSVReader* reader, FILE* file, char sep);

void CSVReader_reset(CSVReader* reader);

void CSVReader_closeFile(CSVReader* reader);

FILE* CSVReader_getFile(CSVReader* reader);
const FILE* CSVReader_getCFile(const CSVReader* reader);

char CSVReader_getSep(const CSVReader* reader);

size_t CSVReader_getColCount(const CSVReader* reader);

size_t CSVReader_getCol(const CSVReader* reader);
size_t CSVReader_getLine(const CSVReader* reader);

size_t CSVReader_skipValue(CSVReader* reader);
size_t CSVReader_readValue(CSVReader* reader, char* buffer, size_t bufferSize, size_t* valueLen);

bool CSVReader_getError(const CSVReader* reader);
void CSVReader_clearError(CSVReader* reader);

CSVReaderPos CSVReader_getPos(CSVReader* reader);
void CSVReader_setPos(CSVReader* reader, const CSVReaderPos* pos);

bool CSVReader_isReachedEnd(const CSVReader* reader);

bool CSVReader_isValid(const CSVReader* reader);

#endif
