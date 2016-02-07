# John's (Mini-)Java Compiler
A (Mini-)Java Compiler implementation.

## Prerequisites
To build jjc, autoconf and automake are required.  These can usually be
installed through your system's package manager.

## Building
There are two parts that must be done in order to build jjc, configuring the
project using autotools, and compiling/installing jjc after this has been done.

### Autotools configuration
The first step is to configure the initial build system using autotools.  To do
this, run:

    $ ./autogen.sh

### Building the source

    $ make

## Installation
This can also be done using autotools/make:

    # make install
