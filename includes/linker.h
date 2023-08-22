#ifndef _LINKER_H_
#define _LINKER_H_

#include <stddef.h>

struct linker{
	size_t nr_segs;
	size_t nr_syms;
	size_t nr_rels;

	/* size_t is a placeholder for now */
	size_t *segments;
	size_t *symbols;
	size_t *relocations;
	size_t *data;
};

void init_linker(void);

#endif // _LINKER_H_