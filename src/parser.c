/*
	Logic for parsing the actual object format.
*/

#include <stdio.h>
#include <errno.h>

#include "lexer.h"
#include "parser.h"
#include "macros.h"

struct parser_t* init_parser(char *filename)
{
	struct parser_t *parser = (struct parser_t*)calloc(1, sizeof(struct parser_t));

	FILE *fd = fopen(filename, "r");
	if(fd == NULL){
		perror(LINE_FILE);
		exit(1);
	}

	parser->filename = filename;
	parser->fd = fd;

	return parser;
}

void parse(struct parser_t *parser)
{
	int page_size = 0x1000;
	int content_size = 0;
	char *buffer = (char *)malloc(page_size * sizeof(char));

	struct lexer_state *lexer;
	struct token_t *token;

	token = (struct token_t*)calloc(1, sizeof(struct token_t));

	do{
		content_size = fread(buffer, sizeof(char), page_size, parser->fd);
		lexer = init_lexer(buffer, content_size);
		do{
			token_next(lexer, token);
			display_token(token);
		}while(token->type != TOK_NULL);
	}while(!feof(parser->fd));
}