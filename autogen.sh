#!/bin/bash

# Generate configure.
autoreconf -f -v -i -Wall
# Remove stupid files.
rm -f config.h.in~
rm -f stamp-h1
