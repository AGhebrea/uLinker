
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "linker.h"
#include "util.h"
#include "macros.h"

int main()
{
	struct token_t *tmp;
	struct token_list_t *token_list;

	// token_list = init_list();

	char test_string1[] = 
		"     1INVALID_TOKEN abc\n"
		"\n"
		"\n12345745678673457347463573465\n\n\n\n" 
		"\n"
		"3 whatever\n\n"
		"  SYMBOL1 SYMB0L2 S1MB0         a 1 2 3 4 \n abce benone"
		"\n"
	;

	init_lexer(test_string1, strlen(test_string1));

	do{
		tmp = token_next();
		// append_token(tmp);
	} while(tmp->type != TOK_NULL);

	fini_lexer();

	// init_parser();
	// init_linker();

	return 0;
}