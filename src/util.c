
// #include <stddef.h>
// #include <stdlib.h>

#include <string.h>

#include "util.h"
#include "macros.h"

#define LIST_SIZE 0x100


/*
*	Generic list append
*/
void list_t_append(struct list_t *li, void *data)
{

	if(li->size >= li->capacity){
		li->capacity += LIST_SIZE;
		li->data = (void*)realloc(li->data, li->memb_size * (li->capacity));
	}
	memcpy(li->data + (li->size * li->memb_size), &data, li->memb_size);

	DBG(&data);

	li->size += 1;

	return;
}

struct list_t *init_list(size_t memb_size)
{
	struct list_t *ret;
	ret = (struct list_t *)calloc(1, sizeof(struct list_t));
	ret->data = (void *)malloc(LIST_SIZE * memb_size);
	ret->capacity = LIST_SIZE;
	ret->memb_size = memb_size;

	return ret;
}