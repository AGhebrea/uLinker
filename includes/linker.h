#ifndef _LINKER_H_
#define _LINKER_H_

#include <stddef.h>

#define MAX_PERMISSIONS 4

struct relocation_t{

};

struct symbol_t{

};

struct segment_t{
	char *name;
	size_t address;
	size_t offset;
	char permissions[MAX_PERMISSIONS];
};

struct linker_t{
	size_t nr_segs;
	size_t nr_syms;
	size_t nr_rels;

	struct segment_t *segments;
	struct symbol_t *symbols;
	struct relocation_t *relocations;
	char *data;
};

void init_linker(void);
void init_linker_segments(void);
void print_meta(void);
void print_segments(void);

#endif // _LINKER_H_