#!/bin/bash

find . -name "test_*.mj" | while read test; do echo "Now running $test..." && ../bin/jjc $test && echo ""; done
