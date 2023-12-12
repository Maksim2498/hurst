# Hurst

## Table of Contents

- [Table of Contents](#table-of-contents);
- [About](#about);
- [Building](#building);
- [Running](#running);
- [Options](#options).

## About

This is a small CLI program used for evaluation of the Hurst's exponent
of the time series from a CSV file.

## Building

For building you will nead the following:

- C99 compiler;
- [CMake](https://cmake.org/).

If you already have both, to build you need firstly got to the project's root
folder (the one in which this README file is located) and secondly execute the
following commands in your terminal:

```bash
cmake -S . -B target
cmake --build target
```

If no errors where emitted, you should have a build program at `./target/hurst`.

## Running

The program has the following CLI interface:

```plaintext
hurst [options] <CSV filenames>
```

To evaluate the Hurst's exponent you need a CSV file containing a time series of
measurements of some value with time stamp or without it.

If you have a CSV file of the following format:

| Date              | Value |
|-------------------|-------|
| 1970-1-1 00:00:00 | 24.98 |
| 1970-1-2 00:00:00 | 13.37 |

then you need no options to pass to the program and you can simply run the following way:

```bash
hurst path/to/csv/file.csv
```

If your time series CSV file has some other format see [Options](#options) section
for more information on program's behaviour customization.

## Options

| Option                   | Description                                                |
|--------------------------|------------------------------------------------------------|
| `--help`, `-h`           | Prints help message and quits                              |
| `--help-date-fmt`        | Prints date format help message and quits                  |
| `--version`, `-v`        | Prints version message and quits                           |
| `--show-config`          | Prints current program configuration                       |
| `--sep=<arg>`, `-s<arg>` | Specifies column separator ("," by default)                |
| `--date-fmt=<arg>`       | Specifies date format ("%y-%M-%d %h:%m:%s" by default)     |
| `--date-col=<arg>`       | Specifies data column offset (0 by default)                |
| `--val-col=<arg>`        | Specifies value column offset (1 by default)               |
| `--from=<arg>`           | Specifies minimum value date (minimum possible by default) |
| `--until=<arg>`          | Specifies maximum value date (maximum possible by default) |
| `--max-inc-len=<arg>`    | Specifies maximum increment length (10 by default)         |
| `--no-date`              | Disables value's date processing                           |
| `--ignore-bad`           | Enables ignoring of bad values and dates                   |
| `--sort`, `-S`           | Enables records sorting by date (in ascending order)       |
