
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "macros.h"
#include "lexer.h"

#define CURRENT_CHAR(lexer) lexer->content[lexer->cursor]

// struct lexer_state *lexer;

int isend(struct lexer_state * lexer)
{
	if(lexer->cursor >= lexer->content_len) return true;

	/* TODO(
		think if you actually care about this.
		you might if it were not a toy project
	)*/
	if(CURRENT_CHAR(lexer) == EOF || CURRENT_CHAR(lexer) == '\0'){
		if(lexer->cursor < lexer->content_len){
			DEBUG_INFO_FMT_STR(
				(lexer->cursor < lexer->content_len),
				"\n(!!!)Trailing data in buffer detected:\n[%s]",
				&(lexer->content[lexer->cursor + 1])
			)
			exit(1);
		}
		return true;
	}
	return false;
}

char consume_token (struct lexer_state *lexer)
{
	char ch;
	ch = CURRENT_CHAR(lexer);

	if(lexer->cursor >= lexer->content_len){
		return 0;
	}

	if(ch == '\n'){
		lexer->bol = lexer->cursor + 1;
		lexer->line += 1;
	}
	lexer->cursor += 1;
	return ch;
}

void trim_left (struct lexer_state *lexer)
{
	while(isspace(CURRENT_CHAR(lexer))){
		consume_token(lexer);
	}
	return;
}

struct lexer_state* init_lexer (char *content, size_t content_len)
{
	struct lexer_state *lexer;

	lexer = (struct lexer_state*)calloc(1, sizeof(struct lexer_state));

	lexer->content = content;
	lexer->content_len = content_len;
	lexer->cursor = 0;
	lexer->line = 0;
	lexer->bol = 0;
	
	return lexer;
}

void getSymbol(struct lexer_state *lexer, struct token_t *token)
{
	consume_token(lexer);
	token->content_len += 1;
	while(isalnum(CURRENT_CHAR(lexer)) || ispunct(CURRENT_CHAR(lexer)) ){
		consume_token(lexer);
		token->content_len += 1;
	}
	token->type = TOK_SYMBOL;

	return;
}

void getNumber(struct lexer_state *lexer, struct token_t *token)
{
	consume_token(lexer);
	token->content_len += 1;
	if(CURRENT_CHAR(lexer) == 'x' || CURRENT_CHAR(lexer) == 'X'){
		consume_token(lexer);
		token->content_len += 1;
		while(isxdigit(CURRENT_CHAR(lexer))){
			consume_token(lexer);
			token->content_len += 1;
		}
		token->type = TOK_HEX;
	}else{
		while(isdigit(CURRENT_CHAR(lexer))){
			consume_token(lexer);
			token->content_len += 1;
		}
		token->type = TOK_NUMBER;
	}

	return;
}

void token_next (struct lexer_state *lexer, struct token_t *token)
{
	trim_left(lexer);

	token->content = &CURRENT_CHAR(lexer);
	token->line = lexer->line;
	token->col = lexer->cursor - lexer->bol;
	token->content_len = 0;

	if(isend(lexer)){
		token->type = TOK_NULL;
		return;
	}

	if(isalpha(CURRENT_CHAR(lexer)) || CURRENT_CHAR(lexer) == '.'){
		getSymbol(lexer, token);

	}else if(isdigit(CURRENT_CHAR(lexer))){
		getNumber(lexer, token);
	}

	// display_token(token);

	//
	// INVALID
	//
	if(!isspace(CURRENT_CHAR(lexer))){
		// (!!!) see #NOTE(0)
		while(!isspace(CURRENT_CHAR(lexer))){
			consume_token(lexer);
			token->content_len += 1; 
		}
		token->type = TOK_INVALID;
	}

	return;
}

void display_token (struct token_t *token)
{
	printf("\ncontent:[%.*s]", token->content_len, token->content);
	switch(token->type){
	case TOK_INVALID:
		printf("\ntype [%s]", "TOK_INVALID");
		break;
	case TOK_NULL:
		printf("\ntype [%s]", "TOK_NULL");
		break;
	case TOK_SYMBOL:
		printf("\ntype [%s]", "TOK_SYMBOL");
		break;
	case TOK_NUMBER:
		printf("\ntype [%s]", "TOK_NUMBER");
		break;
	case TOK_HEX:
		printf("\ntype [%s]", "TOK_HEX");
		break;
	default:
		UNREACHABLE;
	}
	printf("\nline: %d\ncol: %d", token->line, token->col);
	printf("\n");
}

void fini_lexer (struct lexer_state *lexer)
{
	// (!!!) check children allocated if that is the case.
	// we should be careful if we want to free the 'content' field
	if(lexer != NULL){
		free(lexer);
	}
}

/*

#NOTE(0)
	The correctnes of the detection works
	only if the previous if statements
	exit if they detect that something is
	wrong. It is fine for now but it might 
	become a problem if more types are added.
	IDK just be mindful of that.

*/