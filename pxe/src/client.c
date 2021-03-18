#include "base.h"
#include "client.h"
#include "dhcpd.h"
#include "tftpd.h"
#include "configs.h"

extern struct configs conf;

#define HADDRCMP(a, b) (a[0] == b[0] &&  a[1] == b[1])
#define HADDRCPY(a, b) (a[0] = b[0], a[1] = b[1])

struct client **clients = NULL;

static int ip2index(char *ip)
{
	int index = -1;
	
	unsigned int s_ip, d_ip;
	
	s_ip = ip2int(ip);
	d_ip = ip2int(conf.dhcp.pool_ip);

	DEBUG("ip %s", ip);
	//DEBUG("s_ip %d d_ip %d ", s_ip[3], d_ip[3]);
    return s_ip - d_ip;
}

void free_clients()
{
	int i;
	for(i = 0; i < conf.dhcp.pool_size; i++)
	{
		if(clients[i])
			free(clients[i]);
	}	
	free(clients);
}

int init_clients()
{
	int i;
	
	clients = (struct client **)malloc(sizeof(struct client*) * (conf.dhcp.pool_size + 1));
	if(clients)
	{
		memset(clients, 0, sizeof(struct client *) * (conf.dhcp.pool_size + 1));
		return SUCCESS;	
	}
	else
		return ERROR;
}




struct client* add_client(char *ip, unsigned long *mac)
{
	int ret, ip_index = -1, mac_index = -1;
	struct client *cli = NULL;

	DEBUG("add_client ip %s", ip);
	ip_index = ip2index(ip);
	mac_index = get_client(mac);

#if 0
	if(mac_index >= 0) //已存在
	{
		if(mac_index == ip_index)
		{
			HADDRCPY(clients[mac_index]->mac, mac);
			return clients[mac_index];
		}
		else
		{
			if()
			{

			}

		}
	}
	else		//不存在
	{
		cli = (struct client *)malloc(sizeof(struct client));
		if(

	}
#endif
    if(ip_index >= 0 && mac_index >= 0)
    {
        if(ip_index == mac_index)
        {
            DEBUG("client is find update");
            return clients[ip_index];
        }
        else
        {
            DEBUG("client is find  but ip diff update");
            /* 存在 client mac ip 不同 */
            cli = clients[mac_index];
			if(clients[ip_index])
				free(clients[ip_index]);
			clients[ip_index] = cli;		
            strcpy(cli->ip, ip);
			cli->index = ip_index;
        }
    }
    else if(mac_index < 0)  /* 不存在mac */
    {
        if(clients[ip_index])
        {
            DEBUG("client is find but mac diff update");
            HADDRCPY(clients[ip_index]->mac, mac);
        }
        else
        {
            /* new client */
            DEBUG("new client");
            cli = (struct client *)malloc(sizeof(struct client));
            memset(cli, 0, sizeof(struct client));
            HADDRCPY(cli->mac, mac);
            strcpy(cli->ip, ip);
            clients[ip_index] = cli;
        }
        return clients[ip_index];
    }
    else
    {
        DEBUG("add client error");
        return NULL;
    }
	return cli;

#if 0
	if(clients[ip_index])
		free(clients[ip_index]);
	clients[ip_index] = NULL;

	DEBUG("11111111111111");
	/* 已存在 mac */
	if(mac_index >= 0)
	{
		DEBUG("11111111111111");
		if(ip_index != mac_index)
		{
			clients[mac_index] = NULL;
		}
		cli = clients[mac_index];
	}
	else
	{
		cli = (struct client *)malloc(sizeof(struct client));
	}
	clients[ip_index] = cli;
	cli->index = ip_index;
	HADDRCPY(cli->mac, mac);
#endif 
	//return cli;
}

struct client* alloc_client(unsigned long *mac, int index)
{
	int ret;
	struct client *cli = NULL;
	ret = get_free_index(index);
	cli = (struct client *)malloc(sizeof(struct client));
	clients[ret] = cli;
	cli->index = ret;
	HADDRCPY(cli->mac, mac);
	return cli;
}


int get_client(unsigned long *mac)
{
	int i, ret;
	for(i = 0 ; i < conf.dhcp.pool_size; i++)
	{
		if(clients[i] && HADDRCMP(clients[i]->mac, mac))
			return i;
	}
	return -1;
}

int get_free_index(int index)
{
	int i;
	for(i = index; i < conf.dhcp.pool_size; i++)
	{
		if(!clients[i])
		{
			return i;
		}
	}
	return -1;
}

int get_total_client()
{
	int ret = 0, i;
	for(i = 0 ; i < conf.dhcp.pool_size; i++)
	{
		if(clients[i])
			ret ++;
	}
	return ret;
}

void set_client()
{

}

void del_client()
{

}
