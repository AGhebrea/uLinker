
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "macros.h"
#include "lexer.h"

#define CURRENT_CHAR lexer->content[lexer->cursor]

struct lexer_state *lexer;

int isend()
{
	if(lexer->cursor >= lexer->content_len) return true;

	/* TODO(
		think if you actually care about this.
		you might if it were not a toy project
	)*/
	if(CURRENT_CHAR == EOF || CURRENT_CHAR == '\0'){
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

char consume_token ()
{
	char ch;
	ch = CURRENT_CHAR;

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

void trim_left ()
{
	while(isspace(CURRENT_CHAR)){
		consume_token();
	}
	return;
}

void init_lexer (char *content, size_t content_len)
{
	if(lexer == NULL){
		lexer = (struct lexer_state*)calloc(1, sizeof(struct lexer_state));
	}
	lexer->content = content;
	lexer->content_len = content_len;
	lexer->cursor = 0;
	lexer->line = 0;
	lexer->bol = 0;
	return;
}

struct token_t* token_next (void)
{
	struct token_t *token = (struct token_t *)calloc(1, sizeof(struct token_t));

	trim_left();

	token->content = &CURRENT_CHAR;
	token->line = lexer->line;
	token->col = lexer->cursor - lexer->bol;

	if(isend()){
		token->type = TOK_NULL;
		return token;
	}

	if(isalpha(CURRENT_CHAR)){
		//
		// SYMBOL
		//
		consume_token();
		token->content_len += 1;
		while(isalnum(CURRENT_CHAR)){
			consume_token();
			token->content_len += 1;
		}
		token->type = TOK_SYMBOL;
	}else if(isdigit(CURRENT_CHAR)){
		//
		// NUMBER
		//
		consume_token();
		token->content_len += 1;
		while(isdigit(CURRENT_CHAR)){
			consume_token();
			token->content_len += 1;
		}
		token->type = TOK_NUMBER;
	}

	//
	// INVALID
	//
	if(!isspace(CURRENT_CHAR)){
		// (!!!) see #NOTE(0)
		while(!isspace(CURRENT_CHAR)){
			consume_token();
			token->content_len += 1; 
		}
		token->type = TOK_INVALID;
	}

	return token;
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
	default:
		UNREACHABLE;
	}
	printf("\nline: %d\ncol: %d", token->line, token->col);
	printf("\n");
}

void fini_lexer (void)
{
	// (!!!) check children allocated if that is the case.
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