#ifndef _UTIL_H_
#define _UTIL_H_

#include <stddef.h>

#include "lexer.h"

struct list_t{
	size_t size;
	size_t capacity;
	size_t memb_size;
	void *data;
	// void (*append)(struct list_t *, void *);
};

struct list_t *init_list(size_t);
void list_t_append(struct list_t *, void *);

#endif // _UTIL_H_