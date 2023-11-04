
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "linker.h"
#include "parser.h"
#include "util.h"
#include "macros.h"

#define VERBOSE 1

struct test_t{
	int a;
	long b;
	long long c;
	char d;
	char *e;
};

void *create_mock(int seed)
{
	struct test_t *ret;
	ret = (struct test_t *)malloc(sizeof(struct test_t));
	ret->a = seed;
	ret->b = seed * 100;
	ret->c = seed * 1000;
	ret->d = (seed + 37) % 89;
	ret->e = calloc(4, sizeof(char));
	for(int i = 0; i < 3; ++i){
		*(ret->e + i) = ret->d;
	}
	return (void *)ret;
}

void print_test(struct test_t *data)
{
	printf("\n\n%x", data);
	printf("\n%d", data->a);
	printf("\n%ld", data->b);
	printf("\n%lld", data->c);
	printf("\n%c", data->d);
	printf("\n%s", data->e);
}

void test_list()
{
	#define DBG(expr) printf("\n" #expr " = %x", expr);

	struct list_t *list;
	void *test;

	list = init_list(sizeof(struct test_t *));

	for(size_t i = 0; i < 0x8; ++i){
		test = create_mock(i + 1);
		print_test(test);
		DBG(test);
		list_t_append(list, test); 
	}

	for(size_t i = 0; i < 0x8; ++i){
		// printf("\n%x\n", *(int *)(list->data + (i * list->memb_size)));
		print_test(list->data + (i * list->memb_size)); // todo: FIX
	}

	return;

	#undef DBG
}

int main()
{
	// struct list_t *list = init_list(sizeof(int *));
	test_list();

	return 1;

	char outfile[] = "./test/outfile.object";
	struct parser_t *parser;

	init_linker();

	parser = init_parser("./test/test.object");
	parse(parser);

	if(VERBOSE){
		print_state();
	}
	linker_link();
	linker_save(outfile);

	return 0;
}