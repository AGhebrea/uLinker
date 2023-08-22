
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"
#include "linker.h"
#include "util.h"
#include "macros.h"

void PoC_lexer(void);

int main()
{
	struct parser_t *parser = init_parser("./test/test.object");
	parse(parser);

	// PoC_lexer();

	return 0;
}

void PoC_lexer(void)
{
	struct token_t *tmp;
	struct token_list_t *token_list;

	token_list = init_list();

	char test_string1[] = 
		"     1INVALID_TOKEN abc\n"
		"\n"
		"\n12345745678673457347463573465\n\n\n\n" 
		"\n"
		"3 whatever\n\n"
		"  SYMBOL1 SYMB0L2 S1MB0         a 1 2 3 4 \n abce benone"
		"\n"
	;

	struct lexer_state *lexer = init_lexer(test_string1, strlen(test_string1));

	/*
		a big chunk of memory should be allocated all
		at once but this is sufficient for now
	*/
	do{
		tmp = (struct token_t *)calloc(1, sizeof(struct token_t));
		token_next(lexer, tmp);
		append_token(token_list, tmp);
	} while(tmp->type != TOK_NULL);

	for(size_t i = 0; i < token_list->size; ++i){
		display_token(token_list->content[i]);
	}

	fini_lexer(lexer);
}