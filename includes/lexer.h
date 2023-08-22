#ifndef _LEXER_H_
#define _LEXER_H_

enum type_token_t{
	TOK_INVALID = -1,
	TOK_NULL = 0,
	TOK_SYMBOL,
	TOK_NUMBER,
	TOK_HEX,
	TOK_COMMENT,
};

struct token_t{
	enum type_token_t type;
	char *content; 
	size_t content_len;
	int line;
	int col;
};

struct lexer_state{
	char *content;
	size_t content_len;
	int line;
	int bol;
	int cursor;
};

struct lexer_state* init_lexer(char *, size_t);
void fini_lexer(struct lexer_state*);
void display_token(struct token_t*);
void token_next(struct lexer_state *, struct token_t*);

#endif // _LEXER_H_