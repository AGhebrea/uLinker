#ifndef _UTIL_H_
#define _UTIL_H_

#include <stddef.h>

#include "lexer.h"

#define LIST_SIZE 0x1000

struct token_list_t{
	size_t size;
	size_t capacity;
	struct token_t **content;
};

void append_token(struct token_list_t *, struct token_t *);
struct token_list_t **init_list(void);

#endif // _UTIL_H_