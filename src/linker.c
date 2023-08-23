
#include "linker.h"
#include "macros.h"

struct linker_t *linker;

void init_linker(void)
{
	linker = (struct linker_t*)calloc(1, sizeof(struct linker_t));
	linker->data = (char *)malloc(LINKER_DATA_SIZE * sizeof(char));

	linker->data_capacity = LINKER_DATA_SIZE;
	linker->data_size = 0;

	return;
}

void init_linker_state()
{
	linker->segments = (struct segment_t*)calloc(linker->nr_segs, sizeof(struct segment_t));
	linker->symbols = (struct symbol_t*)calloc(linker->nr_syms, sizeof(struct symbol_t));
	linker->relocations = (struct relocation_t*)calloc(linker->nr_rels, sizeof(struct relocation_t));

	return;
}

void print_meta(void)
{
	printf("\nnr_segs: %d", linker->nr_segs);
	printf("\nnr_syms: %d", linker->nr_syms);
	printf("\nnr_rels: %d", linker->nr_rels);

	return;
}

void print_segments(void)
{
	for(size_t i = 0; i < linker->nr_segs; ++i){
		printf("\n\nname %s", linker->segments[i].name);
		printf("\naddress %d", linker->segments[i].address);
		printf("\noffset %d", linker->segments[i].offset);
		printf("\npermissions %s", &(linker->segments[i].permissions));
	}

	return;
}

void print_symbols(void)
{
	for(size_t i = 0; i < linker->nr_syms; ++i){
		printf("\n\nname %s", linker->symbols[i].name);
		printf("\nvalue %d", linker->symbols[i].value);
		printf("\nsegment %d", linker->symbols[i].segment);
		printf("\ntype %s", &(linker->symbols[i].type));
	}
}

void print_relocations(void)
{
	for(size_t i = 0; i < linker->nr_rels; ++i){
		printf("\n\nlocation %d", linker->relocations[i].location);
		printf("\nsegment %d", linker->relocations[i].segment);
		printf("\nref %d", linker->relocations[i].ref);
		printf("\ntype %s", &(linker->relocations[i].type));
	}
}

void print_data(void)
{
	printf("\n\nDATA:\n[%.*s]", linker->data_size, linker->data);
}