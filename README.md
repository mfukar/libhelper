# libhelper

A C library providing implementations of data structures and some algorithms.

![Build status](https://mfukar.visualstudio.com/_apis/public/build/definitions/1b93f481-6971-48f6-b76d-f326956e6a08/1/badge)

## What is this?

libhelper is a library of various tools which are often reinvented by C programmers,
because they are not part of the standard library. It aims to collect frequently used
constructs, algorithms, and helpers - for some definition of "frequently". :-)

## Building

Currently the only tested target is 64-bit Windows via MSVC.
Planned targets include 64-bit Linux and macOS. No 32-bit support is planned, but I will
happily accept changes supporting those.

### Microsoft Visual Studio

The library is tested with Visual Studio 2015 (incl. Community edition).

1. Open the solution `libhelper.sln`
2. Build the solution (press F7)
3. Optionally run the unit tests (press CTRL+R, then A)


## Using

### License

libhelper is distributed under a 3-clause BSD license.

### Microsoft Visual Studio

It is intended to be used as a static library (.lib), and have minimal dependencies. At
the moment those dependencies include only the C standard library.

Detailed instructions on how to use it in your project will appear here. The impatient
user can refer to the `libhelper-unit-tests` project preferences.

## Contributing

Feel free to contribute any way you want. New code, tests, documentation, comments,
reviews, feature requests, beers, the list goes on..
