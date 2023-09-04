#ifndef _LINKER_H_
#define _LINKER_H_

#include <stddef.h>

#define LINKER_DATA_SIZE 0x1000

#define MAX_PERMISSIONS_LEN 4
#define MAX_SYMBOL_TYPES_LEN 2
#define MAX_RELOCATION_TYPES_LEN 3

struct relocation_t{
	size_t location;
	size_t segment;
	size_t ref;
	char type[MAX_RELOCATION_TYPES_LEN];
};

struct symbol_t{
	char *name;
	long long int value;
	size_t segment;
	char type[MAX_SYMBOL_TYPES_LEN];
};

struct segment_t{
	char *name;
	size_t address;
	size_t size;
	char permissions[MAX_PERMISSIONS_LEN];
	char *data;
};

struct output_sections_t{
	struct segment_t *segments;
	struct symbol_t *symbol_table;
	struct relocation_t *relocations;

	size_t sym_size;
	size_t rel_size;
};

struct linker_t{
	size_t header_size;

	size_t nr_segs;
	size_t nr_syms;
	size_t nr_rels;

	struct segment_t *segments;
	struct symbol_t *symbols;
	struct relocation_t *relocations;
	char *data_buffer;

	struct output_sections_t *output_sections;

	size_t data_size;
	size_t data_capacity;
};

void init_linker(void);
void init_linker_state(void);
void linker_link(void);
void linker_save(char *);
void print_state(void);
// void print_segment_data(void);

#endif // _LINKER_H_