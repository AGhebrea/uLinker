
#include <string.h>
#include <unistd.h>

#include "linker.h"
#include "macros.h"

#define TEXT_START_ADDRESS 0x1000
#define OUTPUT_SEGMENTS_N 3

char *output_segment_names[OUTPUT_SEGMENTS_N] = {
	".text",
	".data",
	".bss"
}

struct linker_t *linker;
int l_page_size;

void init_linker(void)
{
	l_page_size = getpagesize();

	linker = (struct linker_t*)calloc(1, sizeof(struct linker_t));
	linker->data_buffer = (char *)malloc(LINKER_DATA_SIZE * sizeof(char));

	linker->output_sections = (struct output_sections_t *)calloc(1, sizeof(struct output_sections_t));
	linker->output_sections->segments = (struct segment_t *)malloc(OUTPUT_SEGMENTS_N * sizeof(struct segment_t));
	
	linker->output_sections->sym_size = 32;
	linker->output_sections->symbol_table = (struct symbol_t *)malloc(linker->output_sections->sym_size * sizeof(struct symbol_t));

	linker->output_sections->rel_size = 32;
	linker->output_sections->relocations = (struct relocation_t *)malloc(linker->output_sections->rel_size * sizeof(struct relocation_t));

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

// another solution, maybe a better one would be to have some sort of
// init fun_ptr inside the segment_t definition and a special_flag which
// signals that the segment is treated in a special way.
// todo: see if that is a sane implemenatation later
void init_segments()
{
	struct segment_t *tmp;

	for(size_t i = 0; i < linker->nr_segs; ++i){
		tmp = &(linker->segments[i]);
		tmp->data = &linker->data_buffer[tmp->address];
	}
}

struct segment_t* get_segment_by_name(char *name)
{
	struct segment_t *tmp;

	for(size_t i = 0; i < linker->nr_segs; ++i){
		tmp = &(linker->segments[i]);
		if(!strcmp(name, tmp->name)){
			return tmp;
		}
	}

	return NULL;
}

// TODO: do another DS that keeps track of segment allocations
// since most stuff will be easier if we knew where the segs are
void linker_link(void)
{
	struct segment_t *segment, *tmp_segment;
	int tmp;

	init_segments();

	/* -- SEGMENT ALLOCATIONS  -------------------- */
	/* .text */
	segment = get_segment_by_name(".text");
	segment->address = TEXT_START_ADDRESS;
	strncpy(segment->permissions, "RP", 3);

	/* .data */
	segment = get_segment_by_name(".data");
	// todo: do a bitwise op for this
	tmp = TEXT_START_ADDRESS + l_page_size;
	segment->address = tmp - (tmp % l_page_size);

	/* .bss */
	tmp_segment = get_segment_by_name(".bss");
	// same here
	tmp = segment->address + segment->size;
	tmp_segment->address = tmp + 4 - (tmp % 4);

	return;
}

void linker_save(char *filename)
{
	char magic_exec_number[] = "EXEC";
	FILE *fd = fopen(filename, "w");

	// TODO: calculate header size

	fprintf(fd, "%s\n", magic_exec_number);
	for(size_t i = 0; i < linker->nr_segs; ++i){
		fprintf(fd, 
			"\n%s %#x %#x %s", 
			linker->segments[i].name,
			linker->segments[i].address,
			linker->segments[i].size,
			linker->segments[i].permissions
		);
	}
	fwrite("\n", sizeof(char), 1, fd);
	for(size_t i = 0; i < linker->nr_syms; ++i){
		fprintf(fd,
			"\n%s %#x %#x %s", 
			linker->symbols[i].name,
			linker->symbols[i].value,
			linker->symbols[i].segment,
			linker->symbols[i].type
		);
	}
	for(size_t i = 0; i < linker->nr_segs; ++i){
		if(strcmp(linker->segments[i].name, ".bss")){
			fwrite("\n\n", sizeof(char), 2, fd);
			fwrite(linker->segments[i].data, sizeof(char), linker->segments[i].size, fd);
		}
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
		printf("\nsize %d", linker->segments[i].size);
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
	printf("\nDATA:\n[%.*s]", linker->data_size, linker->data_buffer);

	return;
}

// todo : remove
// void print_segment_data(void)
// {
// 	for(size_t i = 0; i < linker->nr_segs; ++i){
// 		printf("\n\n# SEGMENT: [%s]", linker->segments[i].name);
// 		printf("\nDATA:\n[%.*s]", 
// 			linker->segments[i].size, 
// 			linker->segments[i].data
// 		);
// 	}
// }