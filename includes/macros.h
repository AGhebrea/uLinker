#ifndef _MACROS_H_
#define  _MACROS_H_

#include <stdio.h>
#include <stdlib.h>

#define _STR(x) #x
#define STR(x) _STR(x)
#define LINE_FILE "[line " STR(__LINE__) " in "  __FILE__ "]"
#define PRINT_LINE_FILE printf("\n\n[line %d in %s]\n", __LINE__, __FILE__)
#define DEBUG_INFO_FMT_STR(expr, fmt_str, string)				\
	do{									\
		printf("\n[%d:%s]" #expr " = %g", __LINE__, __FILE__, expr);	\
		printf(fmt_str, string);					\
	} while(0);
#define TODO(x)                                                                         \
	do                                                                              \
	{                                                                               \
		printf("\n########################################################");   \
		printf("\nTODO: %s", x);                                                \
		printf("\n########################################################\n"); \
	} while (0);
#define UNIMPLEMENTED(x)				\
	do{						\
		PRINT_LINE_FILE;			\
		printf(x " is not implemented\n\n");	\
		exit(1);				\
	}while(0);
#define UNREACHABLE									\
	do{										\
		printf("\n %d in %s should be unreachable.", __LINE__, __FILE__);	\
		exit(1);								\
	}while(0);

#endif //  _MACROS_H_