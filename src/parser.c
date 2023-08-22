/*
	Logic for parsing the actual object format.
*/

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "lexer.h"
#include "parser.h"
#include "macros.h"

char *current_filename;

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

	current_filename = filename;

	return parser;
}

void parseMagic(struct lexer_state *lexer)
{
	
}

// see #NOTE(0)
size_t wrap_fread(char *buffer, size_t size, size_t nmemb, FILE *fd)
{
	size_t ret_size;
	size_t iter;
	int seek_debug;

	ret_size = fread(buffer, size, nmemb, fd);
	seek_debug = ftell(fd);

	if(!feof(fd)){
		for(iter = ret_size; iter > 0 && !isspace(buffer[iter]); --iter);

		if(iter == 0){
			seek_debug = ftell(fd) - (size * nmemb);
			printf("\nfatal error.");
			printf("\nSymbols greater than %d bytes are not allowed.", (size * nmemb));
			printf("\nIllegal symbol starts at position %d inside %s", seek_debug, current_filename);
			exit(1);
		}

		buffer[iter] = '\0';
		fseek(fd, iter - ret_size, SEEK_CUR);
		
		ret_size = iter;
	}

	return ret_size;
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
		content_size = wrap_fread(buffer, sizeof(char), page_size, parser->fd);
		lexer = init_lexer(buffer, content_size);
		do{
			token_next(lexer, token);
			display_token(token);
		}while(token->type != TOK_NULL);

		// TODO: something like this
		// parseMagic(lexer);
		// parseMeta(lexer);
		// parseSegments(lexer);
		// parseSymbols(lexer);
		// parseRels(lexer);
		// parseData(lexer);

	}while(!feof(parser->fd));
}

/*
	#NOTE(0)
		the issue is that if the end of the buffer falls inside 
		of a token then that token won't get parsed properly.

		if a token is greater than 4096 bytes then this will
		throw an error.
*/