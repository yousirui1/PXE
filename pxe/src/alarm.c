#include "base.h"

static int die_if_timeout = 0;
static unsigned int alarm_timeout = 0;

void alarm_handler(int sig)
{
	if(die_if_timeout)
		DEBUG("alarm handler time out");
}

void set_alarm(void)
{
	if(alarm_timeout)
	{
		alarm(alarm_timeout);
		die_if_timeout = 0;	
	}
}

