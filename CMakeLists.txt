cmake_minimum_required(VERSION 3.12)

project(
    hurst
    VERSION      1.0.0
    LANGUAGES    C
    DESCRIPTION  "Utility for Hurst exponent computation"
    HOMEPAGE_URL "https://hub.mos.ru/fominmv2498/hurst"
)

add_executable(
    hurst

    src/hurst/app/all.h
    src/hurst/app/App.h
    src/hurst/app/App.c
    src/hurst/app/AppConfig.h
    src/hurst/app/AppConfig.c
    src/hurst/app/AppConfigParser.h
    src/hurst/app/AppConfigParser.c
    src/hurst/app/Measure.h
    src/hurst/app/Measure.c
    src/hurst/app/MeasureReader.h
    src/hurst/app/MeasureReader.c
    src/hurst/app/MeasureReaderConfig.h
    src/hurst/app/MeasureReaderConfig.c
    src/hurst/app/ParsedAppConfig.h

    src/hurst/util/collection/all.h
    src/hurst/util/collection/array.h
    src/hurst/util/collection/array.c
    src/hurst/util/collection/Vec.h
    src/hurst/util/collection/Vec.c

    src/hurst/util/all.h
    src/hurst/util/macros.h
    src/hurst/util/mem.h
    src/hurst/util/mem.c
    src/hurst/util/path.h
    src/hurst/util/path.c
    src/hurst/util/str.h
    src/hurst/util/str.c

    src/hurst/util/fmt/input/all.h
    src/hurst/util/fmt/input/FloatIFmt.h
    src/hurst/util/fmt/input/FloatIFmt.c
    src/hurst/util/fmt/input/IntIFmt.h
    src/hurst/util/fmt/input/IntIFmt.c
    src/hurst/util/fmt/input/ParsedFloat.h
    src/hurst/util/fmt/input/ParsedFloat.c
    src/hurst/util/fmt/input/ParsedInt.h
    src/hurst/util/fmt/input/ParsedInt.c

    src/hurst/util/fmt/output/all.h
    src/hurst/util/fmt/output/ArrayOFmt.h
    src/hurst/util/fmt/output/ArrayOFmt.c
    src/hurst/util/fmt/output/ArrayOFmt_fwd.h
    src/hurst/util/fmt/output/IntOFmt.h
    src/hurst/util/fmt/output/IntOFmt.c
    src/hurst/util/fmt/output/OCase.h
    src/hurst/util/fmt/output/OCase.c
    src/hurst/util/fmt/output/OFmt.h
    src/hurst/util/fmt/output/OFmt.c
    src/hurst/util/fmt/output/OFmt_fwd.h
    src/hurst/util/fmt/output/OPrecision.h
    src/hurst/util/fmt/output/OPrecision.c
    src/hurst/util/fmt/output/OWidth.h
    src/hurst/util/fmt/output/OWidth.c
    src/hurst/util/fmt/output/ParsedArrayOFmt.h
    src/hurst/util/fmt/output/ParsedArrayOFmt.c
    src/hurst/util/fmt/output/ParsedOFmt.h
    src/hurst/util/fmt/output/ParsedOFmt.c
    src/hurst/util/fmt/output/ParsedOPrecision.h
    src/hurst/util/fmt/output/ParsedOPrecision.c
    src/hurst/util/fmt/output/ParsedOWidth.h
    src/hurst/util/fmt/output/ParsedOWidth.c

    src/hurst/util/fmt/all.h
    src/hurst/util/fmt/FmtType.h
    src/hurst/util/fmt/FmtType.c
    src/hurst/util/fmt/ParsedFmtType.h
    src/hurst/util/fmt/ParsedFmtType.c
    src/hurst/util/fmt/Radix.h
    src/hurst/util/fmt/Radix.c

    src/hurst/util/io/all.h
    src/hurst/util/io/CSVReader.h
    src/hurst/util/io/CSVReader.c
    src/hurst/util/io/print.h
    src/hurst/util/io/print.c

    src/hurst/util/opt/all.h
    src/hurst/util/opt/Opt.h
    src/hurst/util/opt/Opt.c
    src/hurst/util/opt/OptParser.h
    src/hurst/util/opt/OptParser.c
    src/hurst/util/opt/OptParseStatus.h
    src/hurst/util/opt/OptParseStatus.c
    src/hurst/util/opt/ParsedOpt.h
    src/hurst/util/opt/ParsedOpt.c

    src/hurst/util/time/all.h
    src/hurst/util/time/consts.h
    src/hurst/util/time/Month.h
    src/hurst/util/time/Month.c
    src/hurst/util/time/Timestamp.h
    src/hurst/util/time/Timestamp.c
    src/hurst/util/time/Year.h
    src/hurst/util/time/Year.c

    src/hurst/all.h

    src/hurst/main.c
)

set_target_properties(
    hurst PROPERTIES

    C_STANDARD          99
    C_STANDARD_REQUIRED true
    C_EXTENSIONS        no
)

set(OS_ID_LINUX 0)
set(OS_ID_MAC   1)
set(OS_ID_WIN   2)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(OS_ID ${OS_ID_LINUX})
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(OS_ID ${OS_ID_MAC})
elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(OS_ID ${OS_ID_WIN})
else()
    message(FATAL_ERROR "You OS is unsupported")
endif()

configure_file(
    src/hurst/config.h.in
    src/hurst/config.h
)

target_include_directories(
    hurst
    PRIVATE src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)

target_compile_options(
    hurst PRIVATE

    -Wall
    -Wextra
    -Wpedantic
    -Werror
)