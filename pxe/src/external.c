#include "base.h"
#include "dhcpd.h"
#include "external.h"


static pthread_t pthread_httpd, pthread_dhcpd, pthread_tftpd;

void *thread_httpd(void *param);
void *thread_tftpd(void *param);
void *thread_dhcpd(void *param);


CAPTUREANDCAST_API void init_network()
{
	load_wsa();
}

CAPTUREANDCAST_API void exit_network()
{
	unload_wsa();
}

CAPTUREANDCAST_API struct client ** get_clients(struct dhcp_param *param)
{
	if(param)
		memcpy(&dhcp_config, param, sizeof(struct dhcp_param));	
	else
		return NULL;
	
	if(init_clients() == SUCCESS)
	{
		DEBUG("get_client SUCCESS");
		return clients;
	}
	else
		return NULL;
}


CAPTUREANDCAST_API void start_dhcpd()
{
	int ret;
    ret = pthread_create(&pthread_dhcpd, NULL, thread_dhcpd, NULL);
    if(SUCCESS != ret)
    {
        DEBUG("thread create dhcpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }
	return SUCCESS;
}

CAPTUREANDCAST_API void stop_dhcpd()
{
  	void *tret = NULL;

	DEBUG("stop_dhcpd");
	pthread_cancel(&pthread_dhcpd);
	DEBUG("1111111111111111");
    pthread_join(pthread_dhcpd, &tret);
    DEBUG("thread dhcpd exit ret %d", *(int *)tret);

	close_fd(dhcp_config.fd);
	free_clients();
}

CAPTUREANDCAST_API int start_httpd()
{
	int ret;
    ret = pthread_create(&pthread_tftpd, NULL, thread_httpd, NULL);
    if(SUCCESS != ret)
    {
        DEBUG("thread create tftpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }
	DEBUG("start httpd ok");
	return SUCCESS;
}

CAPTUREANDCAST_API void start_tftpd()
{
	int ret;
    ret = pthread_create(&pthread_httpd, NULL, thread_tftpd, NULL);
    if(SUCCESS != ret)
    {
        DEBUG("thread create httpd ret: %d, error: %s",ret,strerror(ret));
        return ERROR;
    }
	DEBUG("start tftpd ok");
	return SUCCESS;
}


