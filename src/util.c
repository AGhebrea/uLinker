
#include <stddef.h>
#include <stdlib.h>

#include "util.h"
#include "lexer.h"

void extend(struct token_list_t *list)
{
	list->content = realloc(list->content, (LIST_SIZE + list->capacity) * sizeof(struct token_t *));
}

void append_token(struct token_list_t *list, struct token_t *token)
{
	list->content[list->size] = token;

	list->size += 1;
	if(list->size > list->capacity){
		extend(list);
	}

	return;
}

struct token_list_t *init_list(void)
{
	struct token_list_t *tmp_list;
	struct token_t **tmp_token;

	tmp_list = (struct token_list_t*)calloc(1, sizeof(struct token_list_t *));
	tmp_token = (struct token_t **)calloc(LIST_SIZE, sizeof(struct token_t *));

	tmp_list->content = tmp_token;
	tmp_list->capacity = LIST_SIZE;
	tmp_list->size = 0;

	return tmp_list;
}