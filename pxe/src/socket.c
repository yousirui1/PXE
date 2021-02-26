#include "base.h"
#include "socket.h"

int max_connections = 0;

void close_fd(int fd)
{
    if (fd)
    {
#ifdef _WIN32
        closesocket(fd);
#else
        close(fd);
#endif
    }
}

int load_wsa()
{
#ifdef _WIN32
    WSADATA wsData = {0};
    if (0 != WSAStartup(0x202, &wsData))
    {   
        DEBUG("WSAStartup  fail");
        WSACleanup();
        return ERROR;
    }   
#endif
    return SUCCESS;
}

void unload_wsa()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

unsigned long addr_inc(struct in_addr in)
{
	unsigned long addr = ntohl(in.s_addr);
	if((addr & 0xff) == 254)
		return addr + 3;		//skip 0 and 1
	if((addr & 0xff) == 255)
		return addr + 2;		//at least skip 0
	return addr + 1;	
}

int addr_fits_pool()
{
	//return ntohl();
}

int is_ip_subnet(struct in_addr *p1, struct in_addr *p2, struct in_addr *mask)
{
	return p1->s_addr & mask->s_addr ==  p2->s_addr & mask->s_addr;
}


unsigned int ip2int(char *ip_str)
{
	unsigned int ip = inet_addr(ip_str);
    unsigned int i_ip[4];
    /* 提取IP：求余取整 */
    i_ip[0]  = ip % 256;       // 提取第一部分IP地址
    ip  = ip / 256;
    i_ip[1] = ip % 256;        // 提取第一部分IP地址
    ip  = ip / 256;
    i_ip[2] = ip % 256;        // 提取第一部分IP地址
    ip = ip / 256;
    i_ip[3]  = ip % 256;       // 提取第一部分IP地址
	
	return i_ip[3];
}

char *haddrtoa(const unsigned char *haddr, int len, char sep)
{
    static char haddrbuf[3 * MAXHADDRLEN + 1];
    char *buf;

    if(len > MAXHADDRLEN)
        len = MAXHADDRLEN;

    buf = haddrbuf;

    while(len > 0)
    {
        sprintf(buf, "%02X%c", (unsigned)(*haddr ++ & 0xFF), sep);
        buf += 3;
        len --;
    }

    buf[-1] = 0;
    return haddrbuf;
}

void atohaddr(const unsigned char *addrstr, unsigned char *haddr, int haddrlen)
{
	if(haddrlen == 6)
	{
		int b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0;

		sscanf(addrstr, "%x:%x:%x:%x:%x:%x", &b0, &b1, &b2, &b3, &b4, &b5);
		haddr[0] = (unsigned char)b0;
		haddr[1] = (unsigned char)b1;
		haddr[2] = (unsigned char)b2;
		haddr[3] = (unsigned char)b3;
		haddr[4] = (unsigned char)b4;
		haddr[5] = (unsigned char)b5;
	}
}

#define  MAXTIMESTR  20  //"mm/dd/yyyy/00:00:00"/0
char *timetoa(time_t t)
{
	static char timebuf[MAXTIMESTR];
	struct tm *tt = localtime(&t);
	if(tt == NULL)
		timebuf[0] = 0;
	else
		sprintf(timebuf, "%02d/%02d/%04d/%02d:%02d:%02d",
			                        tt->tm_mon + 1, tt->tm_mday, tt->tm_year + 1900,
                                    tt->tm_hour, tt->tm_min, tt->tm_sec);
	return timebuf;
}

time_t atotime(char* str)
{
   struct tm t;
   memset(&t, 0, sizeof(t));
   t.tm_isdst = -1;  //For daylight savings calculation
#ifdef MSVC
   sscanf_s (str, "%d/%d/%d/%d:%d:%d", &t.tm_mon, &t.tm_mday, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec);
#else
   sscanf(str, "%d/%d/%d/%d:%d:%d", &t.tm_mon, &t.tm_mday, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec);
#endif
   t.tm_mon -= 1;
   t.tm_year -= 1900;
   return mktime(&t);
}

/* 发送数据 */
int send_msg(const int fd, const char *buf, const int len)
{
    const char *tmp = buf;
    int cnt = 0;
    int send_cnt = 0;
    while (send_cnt != len)
    {   
        cnt = send(fd, tmp + send_cnt, len - send_cnt, 0); 
        if (cnt < 0)
        {   
            if (errno == EINTR || errno == EAGAIN)
                continue;
            return ERROR;
        }   
        send_cnt += cnt;
    }   
    return SUCCESS;
}

/* 接收数据 */
int recv_msg(const int fd, char *buf, const int len)
{
    char *tmp = buf;
    int cnt = 0;
    int read_cnt = 0;
    while (read_cnt != len)
    {   
        cnt = recv(fd, tmp + read_cnt, len - read_cnt, 0); 
        if (cnt == 0)
        {   
            return ERROR;
        }   
        else
        {   
            if (cnt < 0)
            {   
                if (errno == EINTR || errno == EAGAIN)
                {   
                    continue;
                }   
                return ERROR;
            }   
        }   
        read_cnt += cnt;
    }   
    return SUCCESS;
}


int create_boardcast_send(const char *ip, const int port)
{
	int fd = -1;	
	int sock_opt = 0;
	/* 广播 */
	if(ip == NULL)
	{
		fd = create_udp_client("255.255.255.255", port);
		sock_opt = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *)&sock_opt, sizeof(sock_opt)) < 0)
        {   
            DEBUG("setsocksock_sock_sock_opt SO_BROADCAST");
        }
	}
	else			//组播
	{
		fd = create_udp_client(ip, port);
		
  		/* 加入组播 */ 
        struct ip_mreq mreq;
        /* 设置要加入组播的地址 */
        memset(&mreq, 0, sizeof (struct ip_mreq));
        mreq.imr_multiaddr.s_addr = inet_addr(ip);
        /* 设置发送组播消息的源主机的地址信息 */
        mreq.imr_interface.s_addr = htonl (INADDR_ANY);
            
        /* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP , (char *)&mreq,sizeof (struct ip_mreq)) == -1) 
        {   
            DEBUG ("setsockopt IP_ADD_MEMBERSHIP error: %s", strerror(errno));
            goto run_out;
        }
	}
	return fd;

run_out:
	close_fd(fd);
	return -1;
}

int create_boardcast_recv(const char *ip, const int port)
{
	int fd = -1;	
    struct sockaddr_in recv_addr;
	int sock_opt = 0;

	/* 广播 */
	if(ip == NULL)
	{
		fd = create_udp_server(port, &recv_addr);
		sock_opt = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *)&sock_opt, sizeof(sock_opt)) < 0)
        {   
            DEBUG("setsocksock_sock_sock_opt SO_BROADCAST");
        }
	}
	else			//组播
	{
		fd = create_udp_server(port, &recv_addr);
		
  		/* 加入组播 */ 
        struct ip_mreq mreq;
        /* 设置要加入组播的地址 */
        memset(&mreq, 0, sizeof (struct ip_mreq));
        mreq.imr_multiaddr.s_addr = inet_addr(ip);
        /* 设置发送组播消息的源主机的地址信息 */
        mreq.imr_interface.s_addr = htonl (INADDR_ANY);
            
        /* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP , (char *)&mreq,sizeof (struct ip_mreq)) == -1) 
        {   
            DEBUG ("setsockopt IP_ADD_MEMBERSHIP error: %s", strerror(errno));
            goto run_out;
        }
	}
	return fd;

run_out:
	close_fd(fd);
	return -1;

}

int create_udp_client(const char *ip, const int port)
{
    int fd = -1;
    struct sockaddr_in send_addr, recv_addr;
	int sock_opt = 0;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(INVALID_SOCKET == fd)
    {   
        DEBUG("unable to create udp socket");
        return -1;
    }

    memset(&send_addr, 0, sizeof(struct sockaddr_in));
    memset(&recv_addr, 0, sizeof(struct sockaddr_in));
	
	send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(port);
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(NULL != ip)
	{
        send_addr.sin_addr.s_addr = inet_addr(ip);
		recv_addr.sin_port = htons(0);     

	    sock_opt = 1;
	    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) < 0)
	    {   
	        DEBUG("setsocksock_sock_sock_opt SO_REUSEADDR");
	    }   
	    
	    sock_opt = 521 * 1024; //设置为512K
	    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
	    {
	        DEBUG("IP_MULTICAST_LOOP set fail!");
	    }
	    
	    sock_opt = 512 * 1024; //设置为512K
	    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
	    {   
	        DEBUG("IP_MULTICAST_LOOP set fail!");
	    }   
	#ifdef _WIN32
	    sock_opt = 1;
	    if (ioctlsocket(fd, FIONBIO, (u_long *)&sock_opt) == SOCKET_ERROR)
	    {       
	        DEBUG("fcntl F_SETFL fail");
	    }       
	#endif      
	
	    /* 绑定自己的端口和IP信息到socket上 */
		if(bind(fd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
	    {       
	        DEBUG("bind port %d error %s", port, strerror(errno));
	        goto run_out;
	    }
	
	    if(connect(fd, (struct sockaddr *)&send_addr, sizeof(send_addr)) < 0)
	    {   
	        DEBUG("connect connfd : %d ip %s port %d error", fd, ip, port);
	        goto run_out;
	    }   
	}
	return fd;
run_out:
	close_fd(fd);
    return -1;
}

int create_udp_server(const int port, struct sockaddr_in *recv_addr)
{
	int fd = -1;
	int sock_opt = 0;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(INVALID_SOCKET == fd)
	{
        DEBUG("unable to create udp socket");
		return -1;
	}
	memset(recv_addr, 0, sizeof(struct sockaddr_in));
	recv_addr->sin_family = AF_INET;
    recv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr->sin_port = htons(port);

    sock_opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) < 0)
    {   
        DEBUG("setsocksock_sock_sock_opt SO_REUSEADDR");
    }   
    
    sock_opt = 521 * 1024; //设置为512K
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
    {
        DEBUG("IP_MULTICAST_LOOP set fail!");
    }
    
    sock_opt = 512 * 1024; //设置为512K
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
    {   
        DEBUG("IP_MULTICAST_LOOP set fail!");
    }   
#ifdef _WIN32
    sock_opt = 1;
    if (ioctlsocket(fd, FIONBIO, (u_long *)&sock_opt) == SOCKET_ERROR)
    {       
        DEBUG("fcntl F_SETFL fail");
    }       
#endif     

	if(bind(fd, (struct sockaddr *)recv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		DEBUG("bind sockfd %d local port 0 error", fd);
		goto run_out;
	}	
	return fd;
run_out:
	close_fd(fd);
	return -1;	
}

int create_udp_accept(struct sockaddr_in *recv_addr)
{
	int ret, fd = -1;
	struct sockaddr_in send_addr;
	int port = recv_addr->sin_port;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(INVALID_SOCKET == fd)
	{
        DEBUG("unable to create udp socket");
		return -1;
	}

	if(recv_addr->sin_family != AF_INET)
	{
		DEBUG("received address was not AF_INET, please check your inetd config");
	}

	memset(&send_addr, 0, sizeof(struct sockaddr_in));
	//send_addr.sin_port = htons(10000);
	send_addr.sin_port = htons(port);
	recv_addr->sin_family = AF_INET;
	
	if(bind(fd, (struct sockaddr *)&send_addr, sizeof(struct sockaddr_in)) < 0)
	{
		DEBUG("bind connfd: %d error port 0", fd);
		goto run_out;
	}

	if(connect(fd, (struct sockaddr *)recv_addr, sizeof(struct sockaddr_in)) < 0)
	{
		DEBUG("connect connfd : %d error", fd);
		goto run_out;
	}
	return fd;	
run_out:
	close_fd(fd);
	return -1;
}




int create_tcp()
{  
    int fd = -1, sock_opt;
    int keepAlive = 1;    //heart echo open
    int keepIdle = 15;    //if no data come in or out in 15 seconds,send tcp probe, not send ping
    int keepInterval = 3; //3seconds inteval
    int keepCount = 5;    //retry count

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == fd)
    {
        DEBUG("unable to create tcp socket");
        goto run_out;
    }
   
    sock_opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt)) != 0)
        goto run_out;
#ifdef _WIN32

#else
    if (fcntl(fd, F_SETFD, 1) == -1)
    {
        DEBUG("can't set close-on-exec on server socket!");
        goto run_out;
    }
    if ((sock_opt = fcntl(fd, F_GETFL, 0)) < -1)
    {
        DEBUG("can't set close-on-exec on server socket!");
        goto run_out;
    }
    if (fcntl(fd, F_SETFL, sock_opt | O_NONBLOCK) == -1)
    {
        DEBUG("fcntl: unable to set server socket to nonblocking");
        goto run_out;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive)) != 0)
        goto run_out;
    if (setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle)) != 0)
        goto run_out;
    if (setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval)) != 0)
        goto run_out;
#endif
    return fd;
run_out:
    close_fd(fd);
    return -1;
}

int bind_socket(int fd, int port)
{
    DEBUG("tcp bind port %d", port);

    struct sockaddr_in server_sockaddr;
    memset(&server_sockaddr, 0, sizeof server_sockaddr);

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_sockaddr.sin_port = htons(port);

    /* internet family-specific code encapsulated in bind_server()  */
    if (bind(fd, (struct sockaddr *)&server_sockaddr,
             sizeof(server_sockaddr)) == -1)
    {
        DEBUG("unable to bind");
        goto run_out;
    }

	if(max_connections < 1)
	{
#ifndef _WIN32
        struct rlimit rl; 

        /* has not been set explicitly */
        int c = getrlimit(RLIMIT_NOFILE, &rl);
        if (c < 0) 
		{
           	DEBUG("getrlimit");
			goto run_out;
        }   
        max_connections = rl.rlim_cur;
#endif
		if(max_connections < 1)
		{
			max_connections = 1024;
		}
	}

    /* listen: large number just in case your kernel is nicely tweaked */
    if (listen(fd, max_connections) == -1)
    {
        DEBUG("listen max_connect: %d error", max_connections);
        goto run_out;
    }

    return SUCCESS;
run_out:
    close_fd(fd);
    return ERROR;
}

/* 连接服务器 */
int connect_server(int fd, const char *ip, int port)
{
    int count = 10;
    struct sockaddr_in client_sockaddr;
    memset(&client_sockaddr, 0, sizeof client_sockaddr);

    client_sockaddr.sin_family = AF_INET;
    client_sockaddr.sin_addr.s_addr = inet_addr(ip);
    client_sockaddr.sin_port = htons(port);

    while (connect(fd, (struct sockaddr *)&client_sockaddr, sizeof(client_sockaddr)) != 0)
    {
        DEBUG("connection failed reconnection after 1 seconds");
        sleep(1);
        if (!count--)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}


