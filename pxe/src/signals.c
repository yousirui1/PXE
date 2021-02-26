#include "base.h"


#ifndef _WIN32

#include <sys/wait.h>
#include <signal.h>

sigjmp_buf env;
static int handle_sigbus;

static void sig_quit_listen(int e)
{
	char s = 'S';
	DEBUG("recv signals stop msg");	
}

/*
 * Name: init_signals
 * Description: Sets up signal handlers for all our friends.
 */
void init_signals(void)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    struct sigaction act;
    act.sa_handler =  sig_quit_listen;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, 0); 
}

void sigsegv(int dummy)
{


}

void sigbus(int dummy)
{

}

void sigterm(int dummy)
{

}

/* lame duck mode */
void sigterm_stage1_run(int server_s)
{


}

/* lame duck mode */
void sigterm_stage2_run()
{



}

void sighup(int dummy)
{

}

void sighup_run(void)
{

}


void sigint(int dummy)
{

}

void sigchld(int  dummy)
{

}

void sigchld_run(void)
{

}

void sigalrm(int dummy)
{

}

void sigalrm_run(void)
{

}

#else
void init_signals(void)
{
}

void sigsegv(int dummy)
{
}

void sigbus(int dummy)
{
}

void sigterm(int dummy)
{
}

/* lame duck mode */
void sigterm_stage1_run(int server_s)
{
}

/* lame duck mode */
void sigterm_stage2_run()
{
}

void sighup(int dummy)
{
}

void sighup_run(void)
{
}

void sigint(int dummy)
{
}

void sigchld(int  dummy)
{
}

void sigchld_run(void)
{
}

void sigalrm(int dummy)
{
}

void sigalrm_run(void)
{
}
#endif
