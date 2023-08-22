#ifndef _LEXER_H_
#define _LEXER_H_

enum type_token_t{
	TOK_INVALID = -1,
	TOK_NULL = 0,
	TOK_SYMBOL,
	TOK_NUMBER,

};

struct token_t{
	enum type_token_t type;
	char *content; 
	size_t content_len;
	int line;
	int col;
	char *filename;
};

struct lexer_state{
	char *content;
	size_t content_len;
	int line;
	int bol;
	int cursor;
};

void init_lexer(char *, size_t);
void fini_lexer(void);
void display_token(struct token_t*);
struct token_t* token_next(void);

#endif // _LEXER_H_