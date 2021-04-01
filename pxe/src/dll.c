#include "base.h"
#include "dhcpd.h"
#include "dll.h"
#include "tftpd.h"
#include "configs.h"

extern struct configs conf;


static pthread_t pthread_httpd, pthread_dhcpd, pthread_tftpd;

void *thread_httpd(void *param);
void *thread_tftpd(void *param);
void *thread_dhcpd(void *param);

CAPTUREANDCAST_API void get_ver(int *major, int *minor)
{
	get_version(major, minor);
}

CAPTUREANDCAST_API void start_pxe(struct configs *c)
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

	//memcpy(&conf, c, sizeof(struct configs));

	strcpy(conf.netcard.ip, c->netcard.ip);
	strcpy(conf.dhcp.local_ip, c->netcard.ip);
	strcpy(conf.dhcp.pool_ip, c->dhcp.pool_ip);
    conf.dhcp.pool_size = c->dhcp.pool_size;

    strcpy(conf.dhcp.mask, c->dhcp.mask);
    strcpy(conf.dhcp.dns1, c->dhcp.dns1);
    strcpy(conf.dhcp.gateway, c->dhcp.gateway);

	strcpy(conf.netcard.netmask, c->dhcp.mask);
	strcpy(conf.netcard.gateway, c->dhcp.gateway);
	strcpy(conf.netcard.dns1, c->dhcp.dns1);

    strcpy(conf.dhcp.tftp_ip, c->netcard.ip);
    conf.dhcp.lease = c->dhcp.lease;
	
	conf.type = c->type;

	DEBUG("mask %s dns1 %s gateway %s", c->dhcp.mask, c->dhcp.dns1, c->dhcp.gateway);

    DEBUG("conf.netcard.ip %s conf.netcard.netmask %s conf.netcard.gateway %s conf.netcard.dns1 %s", conf.netcard.ip, conf.netcard.netmask, conf.netcard.gateway, conf.netcard.dns1);


    DEBUG("conf.server.ip %s conf.server.port %d", conf.server.ip, conf.server.port);

    DEBUG("conf.http.port %d conf.http.dir %s conf.http.hfs %d", conf.http.port, conf.http.dir, conf.http.hfs);

    DEBUG("conf.dhcp.poo_ip %s conf.dhcp.pool_size %d conf.dhcp.lease %d conf.dhcp.again %d", conf.dhcp.pool_ip,
            conf.dhcp.pool_size, conf.dhcp.lease, conf.dhcp.again );
    DEBUG("conf.dhcp.pxe_boot %s conf.dhcp.ipxe_boot %s", conf.dhcp.pxe_boot, conf.dhcp.ipxe_boot);

    DEBUG("conf.tftp.timeout %d conf.tftp.dir %s", conf.tftp.timeout, conf.tftp.dir);
    DEBUG("conf.dhcp.tftp_ip %s", conf.dhcp.tftp_ip);

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

    return 0;
}

CAPTUREANDCAST_API void stop_pxe()
{
	int ret;
  	close_pipe();

    ret = save_configs();
    if(SUCCESS != ret)
    {
        DEBUG("save configs error");
    }

    unload_wsa();
    close_logs();
    //do_exit();
}

