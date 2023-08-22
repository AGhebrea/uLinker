
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"
#include "linker.h"
#include "util.h"
#include "macros.h"

int main()
{
	struct parser_t *parser = init_parser("./test/test.object");
	parse(parser);

	return 0;
}