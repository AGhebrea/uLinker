#!/bin/bash

BUILD_DIR="./build"
BUILD_NAME="$BUILD_DIR/uLinker"
FILES="./src/main.c ./src/lexer.c ./src/parser.c ./src/linker.c"

if [ ! -d $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi

gcc $FILES -o $BUILD_NAME -ggdb3 -I ./includes/
