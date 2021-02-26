#include "base.h"
#include "dhcpd.h"
#include "client.h"
#include "socket.h"

static int scan_for_leases = 1;
static int dhcp_index = 0;

void close_fd(int fd);

struct dhcp_param dhcp_config;

static unsigned char dhcp_agent_opt[1024];
static int dhcp_agent_len = 0;
static int dhcp_double_answer = 1;
static struct in_addr local_addr;

struct m_addr{
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
};

struct client *dhcp_realloc_item(struct in_addr addr,unsigned long *mac)
{
#if 0
	struct client *cli = NULL;

	cli = (struct client *)malloc(sizeof(struct client));		

	if(cli)
	{
		cli->addr = addr;
		HADDRCPY(cli->mac, mac);	
	}
	return cli;
#endif

}

void dhcp_destroy_item(struct client *cli)
{

}

struct client *dhcp_search_client(unsigned long *mac)
{
	struct client *cli = NULL;
	int ret = get_client(mac);

	if(ret == -1)
	{
		cli = alloc_client(mac, 0);
	}
	else
	{
		cli = clients[ret];
	}
	HADDRCPY(cli->mac, mac);
	return cli;
}

void dhcp_set_time(struct client *cli)
{

}

unsigned char *dhcp_search_options(unsigned char *opt, int field, int *len)
{
	int i;
	unsigned char *p;
	
	if(*(uint32_t *)opt != *(uint32_t *)DHCP_OPTIONS_COOKIE)
		return NULL;

	for(i = 0, p = opt + sizeof(DHCP_OPTIONS_COOKIE) - 1; i < DHCP_OPTION_LEN - 3 && p[i] != field;
		i += (p[i] == DHO_PAD ? 1 : 2 + p[i + 1] ));

	if(i < DHCP_OPTION_LEN - 3 && i + p[i + 1] < DHCP_OPTION_LEN && p[i] == field)
	{
		if(len != NULL)
			*len = p[i + 1];

                return &p[i + 2];
	}
	return NULL;	
}

int dhcp_send(int sockfd, struct sockaddr_in *send_addr, struct sockaddr_in *recv_addr, struct dhcp_packet *pkt, int len, int unicast)
{
	DEBUG("dhcp_send !!!");
	int ret;
	
	if(unicast)
	{
		send_addr->sin_addr.s_addr = htonl (INADDR_BROADCAST); 
	}
	
	ret = sendto(sockfd, (char *)pkt, len, 0, (struct sockaddr *)send_addr, sizeof(*send_addr));
	
	if(ret < len)
		DEBUG("sendto error ret: %d msg %s", ret, strerror(ret));

	return ret;
}

int dhcp_recv(int fd ,struct sockaddr_in *recv_addr, struct sockaddr_in *send_addr, struct dhcp_packet *pkt, int len)
{
    int ret, sock_opt;
    int port = send_addr->sin_port;

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == INVALID_SOCKET)
    {
        DEBUG("create socket error %s", strerror(errno));
        return ERROR;
    }

    /* add send broadcast capacity to DHCP listening socket */
    sock_opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) < 0)
    {
        DEBUG("setsocksock_sock_sock_opt SO_REUSEADDR");
    }

    sock_opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&sock_opt, sizeof(sock_opt)) < 0)
    {
        DEBUG("setsocksock_sock_sock_opt SO_BROADCAST");
    }

    memset(recv_addr, 0, sizeof(*recv_addr));
    recv_addr->sin_family = AF_INET;
	recv_addr->sin_addr.s_addr = INADDR_ANY;
    recv_addr->sin_port = htons(68);

    /* 绑定自己的端口和IP信息到socket上 */
    if (bind(sockfd, (struct sockaddr *)recv_addr, sizeof(struct sockaddr_in)) == -1)
    {
        DEBUG("bind port %d error", port);
        //close_fd(fd);
        //return udp;
    }
	//ret = sendto(sockfd, (char *)&pkt, size, 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
    ret = sendto(fd, (char *)pkt, len, 0, (struct sockaddr *)send_addr, sizeof(*send_addr));
    if(ret < len)
        DEBUG("sendto error ret: %d msg %s", ret, strerror(ret));

    close_fd(sockfd);
    return ret;
}

void dhcp_send_lease(int lease)
{

}

int dhcp_option_reply(struct dhcp_packet *pkt, int type, struct client *cli, char *boot_file)
{
	int i, ret, j;
	unsigned char *opt = (unsigned char *)pkt->options + (sizeof(DHCP_OPTIONS_COOKIE) -1);
	unsigned char *p;	
	int agent_opt = 0;
	unsigned char agent_opt_buf[1024];
	struct in_addr *next_addr;
	struct in_addr client_addr;

	/* save agent options */
	p = dhcp_search_options(pkt->options, DHO_DHCP_AGENT_OPTIONS, NULL);
	if(p != NULL)
	{
		p--;
		agent_opt = *p;
		memcpy(agent_opt_buf, p + 1, agent_opt);
	}

	/* Always pack the magic cookie again, just in case it was corrupted */
	*(uint32_t *)(pkt->options) = *(uint32_t *)DHCP_OPTIONS_COOKIE;

	/* client 获取到ip 之后 下一个提供服务的服务器ip地址 例如tftp */
	//next_addr = find_next_addr();
	next_addr = &local_addr;

	/* If we are the bootp server, we are also the tftpserver */
	if(TFTPD32_TFTP_SERVER)
		pkt->siaddr.s_addr = inet_addr(dhcp_config.tftp_ip);

	pkt->yiaddr.s_addr = inet_addr(dhcp_config.pool_ip);

	struct m_addr *addr = (struct m_addr *)&pkt->yiaddr.s_addr;
	addr->b4 += cli->index;

	for(i = 0; i < sizeof_tab(dhcp_opts); i++)
	{
		/* skip if linked to a service which is not started (change suggested by Colin) */
		//if();
		
		if(dhcp_opts[i].len != 0)
		{
			*opt ++ = (unsigned char)dhcp_opts[i].opt;
			*opt ++ = (unsigned char)dhcp_opts[i].len;
		}
		switch(dhcp_opts[i].opt)
		{
			case DHO_DHCP_MESSAGE_TYPE:
				*opt = (unsigned char)type;
				break;
			case DHO_LOG_SERVERS:
				break;
			case DHO_DHCP_SERVER_IDENTIFIER:
				*(uint32_t *)opt = inet_addr(dhcp_config.tftp_ip);
				break;
			case DHO_TFTP_SERVER:
				*opt++ = DHO_TFTP_SERVER;
				*opt = strlen(dhcp_config.tftp_ip);
				memcpy(opt + 1, dhcp_config.tftp_ip, *opt);
				opt += 1 + *opt;
				break;
				
			case DHO_SUBNET_MASK:
				*(uint32_t *)opt = inet_addr(dhcp_config.mask);
				break;
			case DHO_ROUTERS:
				if((strlen(dhcp_config.gateway) == 0 || inet_addr(dhcp_config.gateway) == 0x0))
					*(uint32_t *)opt = pkt->yiaddr.s_addr;
				else
					*(uint32_t *)opt = inet_addr(dhcp_config.gateway);
				break;	
			case DHO_DOMAIN_NAME_SERVERS:
				if(strlen(dhcp_config.dns1) != 0 && inet_addr(dhcp_config.dns1) != 0x0)
				{
					int bkp_dns = 0;
					*opt ++ = DHO_DOMAIN_NAME_SERVERS;
					*opt ++ = bkp_dns ? 8: 4;
                    *(uint32_t *)opt = inet_addr(dhcp_config.dns1);
					if(bkp_dns)
						*(uint32_t *)(opt + 4) = inet_addr(dhcp_config.dns2);
					opt += bkp_dns ? 8: 4;	
				}
				break;
			case DHO_NETBIOS_NAME_SERVERS:

				*opt ++= DHO_NETBIOS_NAME_SERVERS;
				*opt ++= 4;
				if(!(strlen(dhcp_config.wins) == 0 || inet_addr(dhcp_config.wins) == 0x0))
				{
					*(uint32_t *)opt = inet_addr(dhcp_config.wins);
				}
				else
				{
                    *(uint32_t *)opt = inet_addr(dhcp_config.dns1);
				}
				opt += 4;
				break;
			case DHO_DHCP_LEASE_TIME:
				*(uint32_t *)opt = htonl(dhcp_config.lease * 60);
				break;

			case DHO_DHCP_RENEWAL_TIME:
				*(uint32_t *)opt = htonl(dhcp_config.lease / 2 * 60);
				break;
			case DHO_DHCP_REBINDING_TIME:
				*(uint32_t *)opt = htonl((dhcp_config.lease * 80) / 100 * 60);
				break;
			case DHO_BOOT_SIZE:
				if(0)
				{
					*opt ++ = DHO_BOOT_SIZE;
					*opt ++ =  sizeof(unsigned short);
					*(unsigned short *)opt = htons((unsigned short)26759);
					opt += sizeof(unsigned short);
				}
				break;
			case DHO_BOOT_FILE:
				*opt ++ = DHO_BOOT_FILE;
				*opt =  strlen(boot_file);
				memcpy(opt + 1, boot_file, strlen(boot_file));
				opt += 1 + *opt;
				break;	
			case DHO_NTP_SERVERS:
				if(!strlen(dhcp_config.opt42) == 0 || inet_addr(dhcp_config.opt42) == 0x0)
				{
					*opt ++ = DHO_NTP_SERVERS;
					*opt ++ = 4;
					*(uint32_t *)opt = inet_addr(dhcp_config.opt42);
					opt += 4;
				}
				break;
			case DHO_SIP_SERVERS:
				if(!strlen(dhcp_config.opt120) == 0 || inet_addr(dhcp_config.opt120) == 0x0)
				{
					*opt ++ = DHO_SIP_SERVERS;
					*opt ++ = 5;
					*opt ++ = 1;
					*(uint32_t *)opt = inet_addr(dhcp_config.opt120);
					opt += 4;
				}
				break;
			case DHO_DOMAIN_NAME:
				if(dhcp_config.domain_name[0] != 0)
				{
					*opt ++ = DHO_DOMAIN_NAME;
					*opt = strlen(dhcp_config.domain_name);
					memcpy(opt + 1, dhcp_config.domain_name, *opt);
					opt += 1 + *opt;
				}
				break;
			case DHO_DHCP_AGENT_OPTIONS:
				if(dhcp_agent_len)
				{
					*opt ++ = DHO_DHCP_AGENT_OPTIONS;
					*opt ++ = dhcp_agent_opt;
					opt += dhcp_agent_len;
					/*
					dhcp_agent_opt_sub1[0] = dhcp_agent_opt_sub2[0] = 0;
					while(sub82 != NULL && sub82 < dhcp_agent_opt + dhcp_agent_len -2 )
					{
						if(sub82[0] = 0x01 && sub[1] < 127)
						{
							memcpy(dhcp_agent_opt_sub1, sub82 + 2, sub82[1]);
							dhcp_agent_opt_sub1[sub82[1]] = 0;
							sub82 += sub82[1] + 2;
						}
						else if(sub82[0] == 0x02 && sub82[1] < 127)
						{
							memcpy(dhcp_agent_opt_sub2, sub82 + 2, sub82[1]);	
							dhcp_agent_opt_sub2[sub82[1]] = 0;
							sub82 += sub82[1] + 2;
						}
						else 
							sub82 = NULL;
					}
					*/	
				}
				break;
			case DHO_CUSTOM:
				break;
				for(j = 0; j < sizeof_tab(dhcp_config.t); j++)
				{
					if(dhcp_config.t[j].add_option)
					{
						*opt++ = (unsigned char)dhcp_config.t[j].add_option;
						//*opt = translate_value();
						opt += 1 + *opt;
					}
				}
				break;

			case DHO_END:
				*opt ++ = DHO_END;
				break;
		}
		opt += dhcp_opts[i].len;		//points on next filed
	}
	return (int)(opt - (unsigned char *)pkt);	
}

int process_dhcp_message(struct dhcp_packet *pkt, int *p_size)
{
	int ret, i;
	unsigned char *p = NULL;
	int type = 0;
	struct in_addr send_addr;
	uint8_t major, minor;
	struct client *cli = NULL;
	char boot_file[128] = "pxelinux.0";
	if(is_dhcp(*pkt))
	{
		/* search dhcp message type */
		p = dhcp_search_options(pkt->options, DHO_DHCP_MESSAGE_TYPE, NULL);
		if(p != NULL)
			type = *p;
	}

	if(pkt->yiaddr.s_addr != INADDR_ANY && pkt->yiaddr.s_addr != INADDR_NONE)
		return ERROR;

#if 0
	for(i = 0; i < sizeof_tab(dhcp_types) - 1 && type != dhcp_types[i].type; i++)
	{
        DEBUG("recv %s msg for ip %s, mac %s",dhcp_types[i].desc, inet_ntoa(pkt->ciaddr), 
				haddrtoa(pkt->chaddr, pkt->hlen,':'));
	}
#endif
	
	switch(type)
	{
		case 0:
			if(dhcp_config.ignore_bootp)
			{
				DEBUG("ignoring bootp request");
				break;
			}
		case DHCPDISCOVER:
			send_addr.s_addr = pkt->ciaddr.s_addr;
			p = dhcp_search_options(pkt->options, DHO_DHCP_REQUESTED_ADDRESS, NULL);
			if(NULL != p)
			{
				send_addr = *(struct in_addr *)p;
				DEBUG("Client requested address %s", inet_ntoa(send_addr));
			}		
			cli = dhcp_search_client(pkt->chaddr);
			if(cli == NULL)
			{
				return ERROR;
			}

			/* populate the packet to be returned */
			pkt->op = BOOTREPLY;		
			//pkt->yiaddr = local_addr;
			p = dhcp_search_options(pkt->options, DHO_PXE_CLIENT_NETIF_ID, NULL);
			if(p != NULL)
			{
				major = *(uint8_t *)&p[1];
				minor = *(uint8_t *)&p[2];
				DEBUG("major: %d minor: %d", major, minor);
				if(major == 3)							//IPXE major 3
				{
					strcpy(boot_file, "boot.ipxe");
				}
			}
			memcpy(pkt->file, boot_file, strlen(boot_file));
			*p_size = dhcp_option_reply(pkt, DHCPOFFER, cli, boot_file);
			break;

		case DHCPREQUEST:
			p = dhcp_search_options(pkt->options, DHO_DHCP_REQUESTED_ADDRESS, NULL);
			if(NULL != p)
			{
				send_addr = *(struct in_addr *)p;
			}
			cli = dhcp_search_client(pkt->chaddr);
			if(cli == NULL)
			{
				return ERROR;
			}
			pkt->op = BOOTREPLY;
			pkt->yiaddr.s_addr = inet_addr(dhcp_config.mask);
			
			p = dhcp_search_options(pkt->options, DHO_PXE_CLIENT_NETIF_ID, NULL);

			if(p != NULL)
			{
				major = *(uint8_t *)&p[1];
				minor = *(uint8_t *)&p[2];
				DEBUG("major: %d minor: %d", major, minor);
				if(major == 3)							//IPXE major 3
				{
					strcpy(boot_file, "boot.ipxe");
				}
			}
			memcpy(pkt->file, boot_file, strlen(boot_file));
			*p_size = dhcp_option_reply(pkt, DHCPACK, cli, boot_file);
			break;	
		case DHCPDECLINE:
			DEBUG("DHCPDECLINE");
			cli = dhcp_search_client(pkt->chaddr);
			if(cli)
			{
				p = dhcp_search_options(pkt->options, DHO_DHCP_REQUESTED_ADDRESS, NULL);
				if(NULL != p)
				{
					send_addr = *(struct in_addr *)p;
					if(cli->addr.s_addr == send_addr.s_addr)
					{
						dhcp_destroy_item(cli);
						DEBUG("item destory");
					}	
				}
			}
#if 0
			{
				unsigned long mac[2];
				p = dhcp_search_options(pkt->options, DHO_DHCP_REQUESTED_ADDRESS, NULL);
				if(NULL != p)
				{
					pkt->ciaddr = *(struct in_addr*)p;
				}
				ret = send_arp(pkt->ciaddr.s_addr, mac);
				if(ret == SUCCESS)
				{
					dhcp_set_time(cli);	
					DEBUG("addr lease for existing address %s", inet_ntoa(cli->addr));
				}	
			}
#endif
			break;

		case DHCPRELEASE:
			DEBUG("DHCPRELEASE ");
			cli = dhcp_search_client(pkt->chaddr);
			if(cli)
			{
				dhcp_set_time(cli);	
			}
			break;
		default:
			break;
	}
	return type == 0 || type == DHCPDISCOVER || type == DHCPREQUEST;
}

int dhcpd_loop()
{
	int ret, size, nready, i;
	struct dhcp_packet pkt;
	char host_name[128];
	int sock_opt = 1;
	int sockfd = 1;
	struct sockaddr_in send_addr, recv_addr;

	int maxfd = 0, maxi = 0;

	fd_set reset, allset;
	struct timeval tv;	

    int socklen =  sizeof (struct sockaddr_in);

	local_addr.s_addr=inet_addr(dhcp_config.local_ip);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == INVALID_SOCKET)
    {
        DEBUG("create socket error %s", strerror(errno));
        return ERROR;
    }

	if(scan_for_leases)			//续租扫描 初始化扫描ip段
	{
		ping_range(dhcp_config.pool_ip, dhcp_config.pool_size);
	}

	/* add send broadcast capacity to DHCP listening socket */
	sock_opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) < 0)
    {   
        DEBUG("setsocksock_sock_sock_opt SO_REUSEADDR");
    }   
		
	sock_opt = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&sock_opt, sizeof(sock_opt)) < 0)
	{
        DEBUG("setsocksock_sock_sock_opt SO_BROADCAST");
	}

	sock_opt = 1;
	if(setsockopt(sockfd, IPPROTO_IP, IP_PKTINFO, (char *)&sock_opt, sizeof(sock_opt)) < 0)
	{
        DEBUG("setsocksock_sock_sock_opt SO_BROADCAST");
	}

    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    //recv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(67);

	memset (&send_addr, 0, socklen);
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons (68);
    send_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	
	/* 绑定自己的端口和IP信息到socket上 */
    if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr_in)) == -1)
    {
        DEBUG("bind port %d error", 67);
        close_fd(sockfd);
		return ERROR;
    }

    FD_ZERO(&allset);
    FD_SET(sockfd, &allset);

    maxfd = maxfd > sockfd ? maxfd : sockfd;
	dhcp_config.fd = sockfd;

	for(;;)
	{
		tv.tv_usec = 100;
		reset = allset;
		
		ret = select(maxfd + 1, &reset, NULL, NULL, &tv);
		if(ret == -1)
		{
			if(errno == EINTR)
				continue;
			else if(errno != EBADF)
			{
				DEBUG("select error ret: %d msg %s", ret, strerror(ret));
				break;
			}
		}
		nready = ret;

		DEBUG("dhcp select");
		dhcp_send_lease(dhcp_config.lease);

		if(FD_ISSET(sockfd, &reset))
		{
			memset(&pkt, 0, sizeof(pkt));
            ret = recvfrom(sockfd, (char *)&pkt, sizeof(pkt) , 0,
                            (struct sockaddr*)&(recv_addr), &socklen);
            if(ret < 0 || ret < offsetof(struct dhcp_packet, options))
                continue;

			/* receive on unbound interface */
			
			/* if pool is empty and mac address not statically assigned : ignore request */
			pkt.sname[sizeof(pkt.sname) - 1] = 0;
			pkt.file[sizeof(pkt.file) - 1] = 0;

			if(pkt.op != BOOTREQUEST)
			{
				DEBUG("!= BOOTREQUEST");
				continue;
			}

			ret = (recv_addr.sin_addr.s_addr != htonl(INADDR_NONE) && recv_addr.sin_addr.s_addr != 
				htonl(INADDR_ANY) && recv_addr.sin_addr.s_addr != CLASS_A_LOOPBACK);	//class A 127
			if(process_dhcp_message(&pkt, &size))
			{
				if(ret)
					dhcp_send(sockfd, &send_addr, &recv_addr, &pkt, size, ret);
				else
				{
					ret = sendto(sockfd, (char *)&pkt, size, 0, (struct sockaddr *)&send_addr, 
											sizeof(send_addr));
					if(dhcp_double_answer)
					{
						for(i = 0; i < 10; i++)
							ret = sendto(sockfd, (char *)&pkt, size, 0, (struct sockaddr *)&send_addr, 
											sizeof(send_addr));
					}
					//dhcp_recv(sockfd ,&send_addr, &recv_addr, &pkt, size);
				}
			}
		}
	}
	return ret;
}


void *thread_dhcpd(void *param)
{
    int ret;
    pthread_attr_t st_attr;
    struct sched_param sched;

    //pthread_detach(pthread_self());
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {
        DEBUG("Thread FFmpeg Decode attr init error ");
    }
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {
        DEBUG("Thread FFmpeg Decode set SCHED_FIFO error");
    }
    sched.sched_priority = SCHED_PRIORITY_DHCPD;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);     //线程可以被取消掉
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);//立即退出
    //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);//立即退出  PTHREAD_CANCEL_DEFERRED

	ret = dhcpd_loop();
    return (void *)ret;
}

