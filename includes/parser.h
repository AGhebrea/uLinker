/*
	Logic for parsing the actual object format.
*/
#ifndef _PARSER_H_
#define _PARSER_H_

#include "linker.h"

struct parser_t{
	char *filename;
	FILE *fd;
};

struct parser_t* init_parser(char *);
void parse(struct parser_t *);

#endif // _PARSER_H_