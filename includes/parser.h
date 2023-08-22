/*
	Logic for parsing the actual object format.
*/
#ifndef _PARSER_H_
#define _PARSER_H_

#define MAGIC_NUMBER "LINK"

enum PARSE_STAGES{
	ERROR = -1,
	MAGIC = 0,
	META,
	SEGMENTS,
	SYMBOLS,
	RELS,
	DATA,
	END
};

struct parser_t{
	char *filename;
	FILE *fd;
};

struct parser_t* init_parser(char *);
void parse(struct parser_t *);

#endif // _PARSER_H_