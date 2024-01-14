#!/bin/bash


SUBA=test*

BUILD_DIR="./build"
BUILD_NAME="$BUILD_DIR/uLinker"
SOURCE_DIR="./src"
CCARGS="-ggdb3 -I ./includes/"
FILES="
	${SOURCE_DIR}/main.c 
	${SOURCE_DIR}/lexer.c 
	${SOURCE_DIR}/parser.c 
	${SOURCE_DIR}/linker.c 
	${SOURCE_DIR}/util.c
"

if [ ! -d $BUILD_DIR ]
then
	mkdir $BUILD_DIR
fi

gcc ${FILES} -o ${BUILD_NAME} ${CCARGS}
RET_CODE=$?

if [ $# -ge 1 -a "${RET_CODE}" = 0 ];
then
	echo -e "\n# -- Running program -------------------------------------\n"
	${BUILD_NAME}
	echo -e "\n\nProgram finished with exit code $?\n"
fi

exit
