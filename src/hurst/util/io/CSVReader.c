#include "CSVReader.h"

#include <assert.h>
#include <ctype.h>

void CSVReader_init(CSVReader* reader, FILE* file) {
    CSVReader_initWithSep(reader, file, ',');
}

void CSVReader_initWithSep(CSVReader* reader, FILE* file, char sep) {
    assert(reader && file && isprint(sep));

    reader->file     = file;
    reader->line     = 0;
    reader->col      = 0;
    reader->colCount = 0;
    reader->sep      = sep;
    reader->error    = false;

    rewind(file);

    while (true) {
        const int c = fgetc(file);

        switch (c) {
            case EOF:
                reader->error = ferror(file);
                return;

            case '\n':
                return;

            case '\r': {
                const int nc = fgetc(file);

                if ('\n' == nc)
                    return;

                ungetc(nc, file);
            }

            default:
                if (!reader->colCount)
                    reader->colCount = 1;

                if (c == sep)
                    ++reader->colCount;
        }
    }
}

void CSVReader_reset(CSVReader* reader) {
    if (CSVReader_getError(reader))
        return;

    rewind(reader->file);

    reader->line = 0;
    reader->col  = 0;

    while (true)
        switch (fgetc(reader->file)) {
            case EOF:
                reader->error = ferror(reader->file);
                return;

            case '\n':
                return;
        }
}

void CSVReader_closeFile(CSVReader* reader) {
    assert(CSVReader_isValid(reader));

    fclose(reader->file);

    reader->file = NULL;
}

FILE* CSVReader_getFile(CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->file;
}

const FILE* CSVReader_getCFile(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->file;
}

char CSVReader_getSep(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->sep;
}

size_t CSVReader_getColCount(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->colCount;
}

size_t CSVReader_getCol(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->col;
}

size_t CSVReader_getLine(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->line;
}

size_t CSVReader_skipValue(CSVReader* reader) {
    return CSVReader_readValue(reader, NULL, 0, NULL);
}

size_t CSVReader_readValue(CSVReader* reader, char* buffer, size_t bufferSize, size_t* valueLen) {
    assert(CSVReader_isValid(reader));

    size_t len  = 0;
    size_t read = 0;

    if (!reader->error && !feof(reader->file))
        while (true) {
            const int c = fgetc(reader->file);

            if (EOF == c) {
                reader->error = ferror(reader->file);

                if (!reader->error && reader->col)
                    goto newline;

                break;
            }

            if ('\r' == c) {
                const int nc = fgetc(reader->file);

                if ('\n' == nc)
                    goto newline;

                ungetc(nc, reader->file);

                goto nonspecial;
            }

            if ('\n' == c) {
                newline:
                if (reader->col + 1 != reader->colCount)
                    reader->error = true;

                reader->col = 0;
                ++reader->line;

                break;
            }

            if (reader->sep == c) {
                ++reader->col;

                if (reader->col >= reader->colCount)
                    reader->error = true;

                break;
            }

            nonspecial:
            ++len;

            if (!buffer || read >= bufferSize)
                continue;

            buffer[read++] = c;
        }

    if (valueLen)
        *valueLen = len;

    return buffer ? read : len;
}

bool CSVReader_getError(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return reader->error;
}

void CSVReader_clearError(CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    reader->error = false;
}

CSVReaderPos CSVReader_getPos(CSVReader* reader) {
    assert(CSVReader_isValid(reader));

    CSVReaderPos pos = {
        .col  = reader->col,
        .line = reader->line,
    };

    if (fgetpos(reader->file, &pos.filePos))
        reader->error = true;

    return pos;
}

void CSVReader_setPos(CSVReader* reader, const CSVReaderPos* pos) {
    assert(pos);

    if (CSVReader_getError(reader))
        return;

    if (fsetpos(reader->file, &pos->filePos)) {
        reader->error = true;
        return;
    }
    
    reader->col  = pos->col;
    reader->line = pos->line;
}

bool CSVReader_isReachedEnd(const CSVReader* reader) {
    assert(CSVReader_isValid(reader));
    return feof(reader->file);
}

bool CSVReader_isValid(const CSVReader* reader) {
    return reader
        && reader->file
        && isprint(reader->sep);
}
