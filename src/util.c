
#include <stddef.h>

#include "util.h"
#include "lexer.h"

// TODO: YOU don't need pointers to pointers for this...
// you just need a pointer and init size * sizeof(struct token_t *)
// the list...

void extend(struct token_list_t *list)
{
	realloc(list->content, () * sizeof(struct token_t **))
}

void append_token(struct token_list_t *list, struct token_t *token)
{
	list->size += 1;
	if(list->size >= list->capacity){
		extend(list);
	}

	list->content[list->size] = token;

	return;
}

struct token_list_t **init_list(void)
{
	struct token_list_t *tmp_list;
	struct token_t **tmp_token;

	tmp_list = (struct token_list_t*)calloc(1, sizeof(struct token_list_t *));
	tmp_token = (struct token_t **)calloc(LIST_SIZE, sizeof(struct token_t *));

	for(size_t i = 0; i < LIST_SIZE; ++i){
		*(tmp_token + i) = (struct token_t *)calloc(1, sizeof(struct token_t));
	}

	tmp_list->content = tmp_list;
	tmp_list->capacity = LIST_SIZE;
	tmp_list->size = 0;

	return tmp_list;
}