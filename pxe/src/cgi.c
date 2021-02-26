#include "base.h"
#include "cgi.h"
#include "httpd.h"

int verbose_cgi_logs = 0;
/* The + 1 is for the NULL is complete_env */
static char *common_cgi_env[COMMON_CGI_COUNT + 1];

static char *env_gen_extra(const char *key, const char *value, int extra)
{

}

void clear_common_env(void)
{

}

void create_common_env()
{

}

#if 0
int process_cgi_header(request *req)
{

}

int add_cgi_env(request *req, char *key, char *value, int http_prefix)
{

}


int init_cgi(request *req)
{

}
#endif
