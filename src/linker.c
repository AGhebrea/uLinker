
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
};

struct linker_t *linker;
int l_page_size;

void init_linker(void)
{
	l_page_size = getpagesize();

	linker = (struct linker_t*)calloc(1, sizeof(struct linker_t));
	linker->data_buffer = (char *)malloc(LINKER_DATA_SIZE * sizeof(char));

	linker->output_sections = (struct output_sections_t *)calloc(1, sizeof(struct output_sections_t));
	linker->output_sections->segments = (struct segment_t *)calloc(OUTPUT_SEGMENTS_N, sizeof(struct segment_t));
	linker->output_sections->segments[0].name = ".text";
	linker->output_sections->segments[1].name = ".data";
	linker->output_sections->segments[2].name = ".bss";

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

void segment_link(void)
{
	struct segment_t *tmp, *current;

	for(size_t i = 0; i < linker->nr_segs; ++i){
		tmp = &(linker->segments[i]);
		tmp->data = &linker->data_buffer[tmp->address];

		if(!strcmp(tmp->name, ".text")){
			current = &(linker->output_sections->segments[0]);
			current->address = 0x1000;
			if(current->size + tmp->size > current->data_cap){
				current->data = realloc(current->data, current->data_cap + l_page_size);
			}
			strncpy(current->data + current->size, tmp->data, tmp->size);
			current->size += tmp->size;
		}else if(!strcmp(tmp->name, ".data")){
			current = &(linker->output_sections->segments[1]);
			current->address = 0x1000;
			if(current->size + tmp->size > current->data_cap){
				current->data = realloc(current->data, current->data_cap + l_page_size);
			}
			strncpy(current->data + current->size, tmp->data, tmp->size);
			current->size += tmp->size;
		}else{
			current = &(linker->output_sections->segments[2]);
			current->address = 0x1000;
			if(current->size + tmp->size > current->data_cap){
				current->data = realloc(current->data, current->data_cap + l_page_size);
			}
			strncpy(current->data + current->size, tmp->data, tmp->size);
			current->size += tmp->size;
		}
	}

	tmp = &(linker->output_sections->segments[0]);
	current = &(linker->output_sections->segments[1]);
	current->address = tmp->address + tmp->size + 0x1000 - (tmp->address + tmp->size) % 0x1000;

	tmp = &(linker->output_sections->segments[1]);
	current = &(linker->output_sections->segments[2]);
	current->address = tmp->address + tmp->size + 4 - (tmp->address + tmp->size) % 4;

	return; 
}

void symbol_link(void)
{
	struct symbol_t *tmp;
	struct segment_t *seg;

	for(size_t i = 0; i < linker->nr_syms; ++i){
		tmp = &(linker->symbols[i]);
		if(strchr(tmp->type, 'U') != NULL){
			// incr output bss
			seg = get_segment_by_name(".bss");
			seg->size += tmp->value;
		}
	}
}

void linker_link(void)
{
	segment_link();
	symbol_link();

	return;
}

void linker_save(char *filename)
{
	char magic_exec_number[] = "EXEC";
	FILE *fd = fopen(filename, "w");

	// TODO: calculate header size

	fprintf(fd, "%s\n", magic_exec_number);
	for(size_t i = 0; i < OUTPUT_SEGMENTS_N; ++i){
		fprintf(fd, 
			"\n%s %#x %#x %s", 
			linker->output_sections->segments[i].name,
			linker->output_sections->segments[i].address,
			linker->output_sections->segments[i].size,
			linker->output_sections->segments[i].permissions
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
	for(size_t i = 0; i < OUTPUT_SEGMENTS_N; ++i){
		if(strcmp(linker->output_sections->segments[i].name, ".bss")){
			fwrite("\n\n", sizeof(char), 2, fd);
			fwrite(linker->output_sections->segments[i].data, sizeof(char), linker->output_sections->segments[i].size, fd);
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