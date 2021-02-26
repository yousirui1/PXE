#include "base.h"
#include "command.h"

#define S0A STMT_NO_ARGS
#define S1A STMT_ONE_ARGS
#define S2A STMT_TWO_ARGS

#if 0
struct command commands[] = {
	{"Port", S1A, c_set_int, &server_port},
};

static void c_set_int(char *v1, char *v2, void *t)
{
	char *endptr;
	int i;
	if(t)
	{
		//i = strol()
		if(*v1 != '\0' && endptr = '\0')
		{
		}
	}
}
#endif
