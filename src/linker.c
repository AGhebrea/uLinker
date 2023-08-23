
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

void print_state(void)
{
	printf("\n\n############################################################");
	printf("\n# META");
	printf("\n############################################################");
	printf("\nnr_segs: %d", linker->nr_segs);
	printf("\nnr_syms: %d", linker->nr_syms);
	printf("\nnr_rels: %d", linker->nr_rels);

	printf("\n############################################################");
	printf("\n# SEGMENTS");
	printf("\n############################################################");
	for(size_t i = 0; i < linker->nr_segs; ++i){
		printf("\nname %s", linker->segments[i].name);
		printf("\naddress %d", linker->segments[i].address);
		printf("\noffset %d", linker->segments[i].offset);
		printf("\npermissions %s", &(linker->segments[i].permissions));
	}

	printf("\n\n############################################################");
	printf("\n# SYMBOLS");
	printf("\n############################################################");
	for(size_t i = 0; i < linker->nr_syms; ++i){
		printf("\nname %s", linker->symbols[i].name);
		printf("\nvalue %d", linker->symbols[i].value);
		printf("\nsegment %d", linker->symbols[i].segment);
		printf("\ntype %s", &(linker->symbols[i].type));
	}

	printf("\n\n############################################################");
	printf("\n# RELOCATIONS");
	printf("\n############################################################");
	for(size_t i = 0; i < linker->nr_rels; ++i){
		printf("\nlocation %d", linker->relocations[i].location);
		printf("\nsegment %d", linker->relocations[i].segment);
		printf("\nref %d", linker->relocations[i].ref);
		printf("\ntype %s", &(linker->relocations[i].type));
	}

	printf("\n\n############################################################");
	printf("\n# DATA");
	printf("\n############################################################");
	printf("\nDATA:\n[%.*s]", linker->data_size, linker->data);

	return;
}