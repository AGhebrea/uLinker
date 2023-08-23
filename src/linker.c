
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

void linker_link(void)
{
	/* there is actually nothing to do for now */

	return;
}

void linker_save(char *filename)
{
	char magic_exec_number[] = "EXEC";
	FILE *fd = fopen(filename, "w+");

	// TODO: calculate header size

	fprintf(fd, "%s\n", magic_exec_number);
	for(size_t i = 0; i < linker->nr_segs; ++i){
		fprintf(fd, 
			"\n%s %d %d %s", 
			linker->segments[i].name,
			linker->segments[i].address,
			linker->segments[i].size,
			linker->segments[i].permissions
		);
	}
	fwrite("\n", sizeof(char), 1, fd);
	for(size_t i = 0; i < linker->nr_syms; ++i){
		fprintf(fd, 
			"\n%s %d %d %s", 
			linker->symbols[i].name,
			linker->symbols[i].value,
			linker->symbols[i].segment,
			linker->symbols[i].type
		);
	}
	for(size_t i = 0; i < linker->nr_segs; ++i){
		char *tmp;
		tmp = &linker->data[linker->segments[i].address];
		fwrite("\n\n", sizeof(char), 2, fd);
		fwrite(tmp, sizeof(char), linker->segments[i].size, fd);
	}

	return;
}

void print_state(void)
{
	printf("\n\n############################################################");
	printf("\n# META");
	printf("\n############################################################");
	printf("\nheader_size: %d", linker->header_size);
	printf("\nnr_segs: %d", linker->nr_segs);
	printf("\nnr_syms: %d", linker->nr_syms);
	printf("\nnr_rels: %d", linker->nr_rels);

	printf("\n############################################################");
	printf("\n# SEGMENTS");
	printf("\n############################################################");
	for(size_t i = 0; i < linker->nr_segs; ++i){
		printf("\nname %s", linker->segments[i].name);
		printf("\naddress %d", linker->segments[i].address);
		printf("\noffset %d", linker->segments[i].size);
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

void print_data(void)
{
	for(size_t i = 0; i < linker->nr_segs; ++i){
		printf("\n\n# SEGMENT: [%s]", linker->segments[i].name);
		printf("\nDATA:\n[%.*s]", 
			linker->segments[i].size, 
			&linker->data[linker->segments[i].address]
		);
	}
}