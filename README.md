# John's (Mini-)Java Compiler
A (Mini-)Java Compiler implementation.

## Prerequisites
To build `jjc`, autoconf and automake are required.  These can usually be
installed through your system's package manager.

`flex` is also required, as it is used to implement `jjc`'s lexer.

## Building
There are two parts that must be done in order to build `jjc`, configuring the
project using autotools, and compiling/installing `jjc` after this has been
done.

### Autotools configuration
The first step is to configure the initial build system using autotools.  To do
this, run:

    $ ./autogen.sh

### Building the source

    $ make

## Installation
This can also be done using autotools/make:

    # make install

## Testing
The above steps build the executable `jjc`.  To test it, various Mini-Java
sources have been provided in the `test` directory, which `jjc` can compile.

### Using jjc
The basic usage of `jjc` is to supply a single Mini-Java source file to it.
For more information on the usage and infocation of `jjc`, see either the
man-page under `man/man`, or run

    $ jjc --help
