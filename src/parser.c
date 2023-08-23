/*
	Logic for parsing the actual object format.
*/

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "linker.h"
#include "parser.h"
#include "macros.h"

char *current_filename;

extern struct linker_t *linker;

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

void raiseError(char *s)
{
	printf("\n%s", s);
	exit(1);
}

long long int tokenToLongLongInt(struct token_t *token)
{
	if(token->type == TOK_HEX){
		return strtol(token->content, NULL, 16);
	}else{
		return strtol(token->content, NULL, 10);
	}
}

void parseMagic(struct lexer_state *lexer)
{
	struct token_t token;
	token_next(lexer, &token, RETURN_COMMENT_FALSE);

	if(strncmp(token.content, MAGIC_NUMBER , token.content_len)){
		raiseError("\nNo magic number found at start of file");
	}

	return;
}

void parseMeta(struct lexer_state *lexer)
{
	struct token_t token;

	getNumber(lexer, &token);
	linker->nr_segs = atoi(token.content);

	getNumber(lexer, &token);
	linker->nr_syms = atoi(token.content);

	getNumber(lexer, &token);
	linker->nr_rels = atoi(token.content);

	return;
}

void parseSegments(struct lexer_state *lexer)
{
	struct token_t token;

	for(size_t i = 0; i < linker->nr_segs; ++i){
		getSymbol(lexer, &token);
		linker->segments[i].name = (char *)malloc(token.content_len * sizeof(char));
		strncpy(linker->segments[i].name, token.content, token.content_len);

		getNumber(lexer, &token);
		linker->segments[i].address = tokenToLongLongInt(&token);

		getNumber(lexer, &token);
		linker->segments[i].offset = tokenToLongLongInt(&token);

		getSymbol(lexer, &token);
		strncpy(linker->segments[i].permissions, token.content, token.content_len);
		
		// TODO: see #NOTE(1)
		// discardLine(lexer);
	}

	return;
}

void parseSymbols(struct lexer_state *lexer)
{
	struct token_t token;

	for(size_t i = 0; i < linker->nr_syms; ++i){
		getSymbol(lexer, &token);
		linker->symbols[i].name = (char *)malloc(token.content_len * sizeof(char));
		strncpy(linker->symbols[i].name, token.content, token.content_len);

		getNumber(lexer, &token);
		linker->symbols[i].value = tokenToLongLongInt(&token);

		getNumber(lexer, &token);
		linker->symbols[i].segment = tokenToLongLongInt(&token);

		getSymbol(lexer, &token);
		strncpy(linker->symbols[i].type, token.content, token.content_len);
		
		// TODO: see #NOTE(1)
		// discardLine(lexer);
	}
}

void parseRels(struct lexer_state *lexer)
{
	struct token_t token;

	for(size_t i = 0; i < linker->nr_rels; ++i){
		getNumber(lexer, &token);
		linker->relocations[i].location = tokenToLongLongInt(&token);

		getNumber(lexer, &token);
		linker->relocations[i].segment = tokenToLongLongInt(&token);

		getNumber(lexer, &token);
		linker->relocations[i].ref = tokenToLongLongInt(&token);

		getSymbol(lexer, &token);
		strncpy(linker->relocations[i].type, token.content, token.content_len);
		
		// TODO: see #NOTE(1)
		// discardLine(lexer);
	}
}

void parseData(struct lexer_state *lexer)
{
	struct token_t token;

	do{
		token_next(lexer, &token, RETURN_COMMENT_FALSE);
		
		if(token.content_len + linker->data_size > linker->data_capacity){
			linker->data = (char *)realloc(linker->data, (linker->data_size + LINKER_DATA_SIZE) * sizeof(char));
		}
		strncpy(linker->data + linker->data_size, token.content, token.content_len);
		linker->data_size += token.content_len;
	}while(token.type != TOK_NULL);
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

/*
	TODO: add error handling.
*/
int tryParse(struct lexer_state *lexer, int stage)
{
	switch(stage){
	case MAGIC:
		parseMagic(lexer);
		break;
	
	case META:
		parseMeta(lexer);
		init_linker_state();
		break;
	
	case SEGMENTS:
		parseSegments(lexer);
		break;
	
	case SYMBOLS:
		parseSymbols(lexer);
		break;
	
	case RELS:
		parseRels(lexer);
		break;
	
	case DATA:
		parseData(lexer);
		break;
	
	case END:
		/* cleanup i guess */
		break;
	
	case ERROR:
		UNREACHABLE;
		break;
	}

	stage += 1;

	return stage;
}

void parse(struct parser_t *parser)
{
	int page_size = 0x1000;
	int stage = 0;
	int content_size = 0;
	char *buffer = (char *)malloc(page_size * sizeof(char));

	struct lexer_state *lexer;
	struct token_t *token;

	token = (struct token_t*)calloc(1, sizeof(struct token_t));

	do{
		content_size = wrap_fread(buffer, sizeof(char), page_size, parser->fd);
		lexer = init_lexer(buffer, content_size);
		
		do{
			stage = tryParse(lexer, stage);
		}while(stage != END);

	}while(!feof(parser->fd));
}

/*
	#NOTE(0)
		the issue is that if the end of the buffer falls inside 
		of a token then that token won't get parsed properly.

		if a token is greater than 4096 bytes then this will
		throw an error.
	
	#NOTE(1)
		bug. it's not from the discardLine function but
		if the line actually ends after the last consumed character then we basically
		delete a line
*/