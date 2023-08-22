#!/bin/bash

BUILD_DIR="./build"
BUILD_NAME="$BUILD_DIR/uLinker"
SOURCE_DIR="./src"
FILES="${SOURCE_DIR}/main.c ${SOURCE_DIR}/lexer.c ${SOURCE_DIR}/parser.c ${SOURCE_DIR}/linker.c ${SOURCE_DIR}/util.c"

if [ ! -d $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi

gcc $FILES -o $BUILD_NAME -ggdb3 -I ./includes/
