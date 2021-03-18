#include "base.h"
#include "dhcpd.h"
#include "tftpd.h"
#include "configs.h"


struct configs conf = {0};

void init_configs()
{
	char buf[128] = {0};

    netcard_conf *net = &(conf.netcard);
    server_conf *server = &(conf.server);
	dhcp_conf *dhcp = &(conf.dhcp);
	http_conf *http = &(conf.http);
	tftp_conf *tftp = &(conf.tftp);

	/* base */
	conf.type = read_profile_int(BASE_SECTION, BASE_TYPE_KEY, 1, CONFIG_FILE);

	/* network */
    if(read_profile_string(NET_SECTION, NET_IP_KEY, buf, sizeof(buf), NULL, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(net->ip, buf);
        }   
    }   

   if(read_profile_string(NET_SECTION, NET_NETMASK_KEY, buf, sizeof(buf), net->netmask, CONFIG_FILE))
   {   
       if(strlen(buf) != 0)
       {   
			strcpy(net->netmask, buf);
       }
   }
   
   if(read_profile_string(NET_SECTION, NET_GATEWAY_KEY, buf, sizeof(buf), net->gateway, CONFIG_FILE))
   {   
       if(strlen(buf) != 0)
       {   
			strcpy(net->gateway, buf);
       }
   }

    if(read_profile_string(NET_SECTION, NET_DNS1_KEY, buf, sizeof(buf), net->dns1, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
            strcpy(net->dns1, buf);
        }
    }

	/* server */
    if(read_profile_string(SERVER_SECTION, SERVER_IP_KEY, buf, sizeof(buf), server->ip, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(server->ip, buf);
        }   
    } 
	server->port = read_profile_int(SERVER_SECTION, SERVER_PORT_KEY, SERVER_PORT, CONFIG_FILE);


	/* httpd */
	http->port = read_profile_int(HTTPD_SECTION, HTTPD_PORT_KEY, 80, CONFIG_FILE);
 	if(read_profile_string(HTTPD_SECTION, HTTPD_DIR_KEY, buf, sizeof(buf), NULL, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(http->dir, buf);
        }   
    } 
	http->hfs = read_profile_int(HTTPD_SECTION, HTTPD_HFS_KEY, 1, CONFIG_FILE);

	/* dhcpd */
 	if(read_profile_string(DHCPD_SECTION, DHCPD_IP_POOL_KEY, buf, sizeof(buf), NULL, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(dhcp->pool_ip, buf);
        }   
    } 

	dhcp->pool_size = read_profile_int(DHCPD_SECTION, DHCPD_POOL_SIZE_KEY, 0, CONFIG_FILE);
	dhcp->lease = read_profile_int(DHCPD_SECTION, DHCPD_LEASE_KEY, 2880, CONFIG_FILE);
	dhcp->again = read_profile_int(DHCPD_SECTION, DHCPD_AGAIN_KEY, 0, CONFIG_FILE);

	strcpy(dhcp->tftp_ip, net->ip);
	strcpy(dhcp->mask, net->netmask);
	strcpy(dhcp->local_ip, net->ip);

 	if(read_profile_string(DHCPD_SECTION, DHCPD_PXE_BOOT_KEY, buf, sizeof(buf), NULL, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(dhcp->pxe_boot, buf);
        }   
    } 
 	if(read_profile_string(DHCPD_SECTION, DHCPD_IPXE_BOOT_KEY, buf, sizeof(buf), NULL, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(dhcp->ipxe_boot, buf);
        }   
    } 

	/* tftpd */
	tftp->timeout = read_profile_int(TFTPD_SECTION, TFTPD_TIMEOUT_KEY, 0, CONFIG_FILE);
 	if(read_profile_string(TFTPD_SECTION, TFTPD_DIR_KEY, buf, sizeof(buf), buf, CONFIG_FILE))
    {   
        if(strlen(buf) != 0)
        {   
			strcpy(tftp->dir, buf);
        }   
    }

	/* voi */

	/* vdi */


	DEBUG("net->ip %s net->netmask %s net->gateway %s net->dns1 %s", net->ip, net->netmask, net->gateway, net->dns1);


	DEBUG("server->ip %s server->port %d", server->ip, server->port);

	DEBUG("http->port %d http->dir %s http->hfs %d", http->port, http->dir, http->hfs);

	DEBUG("dhcp->poo_ip %s dhcp->pool_size %d dhcp->lease %d dhcp->again %d", dhcp->pool_ip, 
			dhcp->pool_size, dhcp->lease, dhcp->again );
	DEBUG("dhcp->pxe_boot %s dhcp->ipxe_boot %s", dhcp->pxe_boot, dhcp->ipxe_boot);

	DEBUG("tftp->timeout %d tftp->dir %s", tftp->timeout, tftp->dir);
	DEBUG("conf.dhcp.tftp_ip %s", conf.dhcp.tftp_ip);
	
}

int save_configs()
{


}


