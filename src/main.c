
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "linker.h"
#include "parser.h"
#include "util.h"
#include "macros.h"

int main()
{
	struct parser_t *parser;
	
	init_linker();

	parser = init_parser("./test/test.object");
	parse(parser);

	print_meta();
	print_segments();

	return 0;
}