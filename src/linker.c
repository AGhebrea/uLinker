
#include "linker.h"
#include "macros.h"

struct linker_t *linker;

void init_linker(void)
{
	linker = (struct linker_t*)calloc(1, sizeof(struct linker_t));

	return;
}

void init_linker_segments()
{
	linker->segments = (struct segment_t*)calloc(linker->nr_segs, sizeof(struct segment_t));
	
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