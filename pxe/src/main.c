#include "base.h"
#include "socket.h"
#include "dhcpd.h"
#include "client.h"
#include "ui.h"

time_t current_time;
const char program_name[] = "PXE";

static pthread_t pthread_httpd, pthread_tftpd, pthread_dhcpd, pthread_ui;

void *thread_httpd(void *param);
void *thread_tftpd(void *param);
void *thread_dhcpd(void *param);
void *thread_ui(void *param);

void do_exit()
{
	void *tret = NULL;

	pthread_join(pthread_httpd, &tret);
	DEBUG("thread httpd exit ret %d", *(int *)tret);

	pthread_join(pthread_tftpd, &tret);
	DEBUG("thread tftpd exit ret %d", *(int *)tret);

	pthread_join(pthread_dhcpd, &tret);
	DEBUG("thread dhcpd exit ret %d", *(int *)tret);

	pthread_join(pthread_ui, &tret);
	DEBUG("thread ui exit ret %d", *(int *)tret);
}

/* argv: ip, port, channel, window_flag, type, server_flag 
 * window_flag : 1 screen 0 window
 * server_flag : 1 server 0 client
 */
void parse_options(int argc, char *argv[])
{
	int c, ret;
	while((c = getopt(argc, argv, "c:r:d")) != -1)
	{
		switch(c)
		{
			case 'c':
			case 'r':
			case 'd':
			default:
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret;
	init_logs();
	ret = load_wsa();
	if(ret != SUCCESS)
	{
		DEBUG("load wsa error");
		return ret;
	}

	init_configs();
	init_signals();
	init_pipe();

	parse_options(argc, argv);

	init_clients();

	ret = pthread_create(&pthread_httpd, NULL, thread_httpd, NULL);
	if(SUCCESS != ret)
    {
        DEBUG("thread create httpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }

	ret = pthread_create(&pthread_tftpd, NULL, thread_tftpd, NULL);
	if(SUCCESS != ret)
    {
        DEBUG("thread create tftpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }

	ret = pthread_create(&pthread_dhcpd, NULL, thread_dhcpd, NULL);
	if(SUCCESS != ret)
    {
        DEBUG("thread create dhcpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }

	ret = pthread_create(&pthread_ui, NULL, thread_ui, NULL);
	if(SUCCESS != ret)
    {
        DEBUG("thread create ui ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }

	close_pipe();

	ret = save_configs();
	if(SUCCESS != ret)
	{
		DEBUG("save configs error");
	}

	do_exit();
	unload_wsa();
	close_logs();
	return 0;
}
