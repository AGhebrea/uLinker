
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "linker.h"
#include "parser.h"
#include "util.h"
#include "macros.h"

#define VERBOSE 1

int main()
{
	struct parser_t *parser;

	init_linker();

	parser = init_parser("./test/test.object");
	parse(parser);

	if(VERBOSE){
		print_state();
		print_data();
	}
	linker_link();

	return 0;
}