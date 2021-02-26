#include "base.h"
#include "ping.h"

unsigned short in_cksum(unsigned short *addr, int len)
{
	register int left = len;
	register unsigned short *w = addr;
	register unsigned short answer;
	register int sum = 0;

	/* Our */
	while(left > 1)
	{
		sum += *w++;
		left -= 2;
	}

	/* mop up an odd byte if necessary */
	if(left == 1)
	{
		unsigned short u = 0;
		*(unsigned char *)(&u) = *(unsigned char *)w;
		sum += u;
	}
	
	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);		//add hi 16 to low 16
	sum += (sum >> 16);						// add carry
	answer = ~sum;							// truncate to 16 bits
		
	return (answer);
}

unsigned long add_inc(struct in_addr in)
{
	unsigned long addr = ntohl(in.s_addr);
	if((addr & 0xff) == 254)
		return addr + 3;
	if((addr & 0xff) == 255)
		return addr + 2;
	return addr + 1;
}

int send_arp(struct in_addr *in, unsigned long *mac)
{
	//unsigned long mac[2] = {0};
	unsigned int mac_len =  6;

#ifdef _WIN32
	if(NO_ERROR == SendARP(in->s_addr, 0, mac, &mac_len))
	{
		//return haddrtoa((unsigned char *)mac, 6, ':');
		//DEBUG("mac %s", haddrtoa((unsigned char *)mac_1, 6, ':'));
		return SUCCESS;
	}	
#endif
	return ERROR;
}

void *ping_recv(void *param)
{
	struct echo_reply reply;
	struct sockaddr_in recv_addr;
	int socklen = sizeof(struct sockaddr_in);
	int sockfd = *(int *)param;
	int ret;
	unsigned int ip[16];
	unsigned long mac[2] = {0};
	//char* mac = NULL;
	fd_set fdset;
	struct timeval tv;

	for(;;)
	{
		FD_ZERO(&fdset);
		FD_SET(sockfd, &fdset);

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		
		ret = select(sockfd + 1, &fdset, NULL, NULL, &tv);
		if(ret == 1)
		{
			memset(&recv_addr, 0, sizeof(recv_addr));
			ret = recvfrom(sockfd, (char *)&reply, sizeof(reply), 0, (struct sockaddr *)&recv_addr,
					&socklen);
			
			if(ret > 0 && reply.echo_req.icmp_hdr.type == ICMP_ECHO_REPLY)
			{
				memcpy(ip,inet_ntoa(recv_addr.sin_addr), sizeof(ip));	
				send_arp(&recv_addr.sin_addr, mac);
				DEBUG("ping ip %s mac %s", ip, haddrtoa((unsigned char *)mac, 6, ':'));
				add_client(ip, mac);
			}
		}
	}
}

void ping_range(char *start_ip, unsigned short count)
{
	int sockfd = -1;
	struct sockaddr_in dest_addr;
	struct echo_request echo_req;
	unsigned short seq;
	int i;

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	//if(sockfd == SOCKI/
	if(sockfd < 0)
		return ;	

	pthread_t pthread_ping;
	
	pthread_create(&pthread_ping, NULL, ping_recv, (void*)&sockfd);
	

	DEBUG("start ip %s ping sockfd %d", start_ip, sockfd);

	/* We'll do most of the packing once */
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_addr.s_addr = inet_addr(start_ip);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = 0;

	/* Fill in echo request static parts */
	echo_req.icmp_hdr.type = ICMP_ECHO_REQUEST;	
	echo_req.icmp_hdr.code = 0;
	echo_req.icmp_hdr.checksum = 0;
	echo_req.icmp_hdr.id = htons(PINGAPI_ID);

	/* Fill in some data to send */
	for(i = 0; i < REQ_DATASIZE; i++)
		echo_req.data[i] = ' ' + i;	

	DEBUG("REQ_DATASIZE over");
	for(seq = 0; seq < count; ++seq)
	{
		echo_req.icmp_hdr.seq = htons(seq);
		echo_req.time = htons(seq);
		echo_req.icmp_hdr.checksum = 0;
		echo_req.icmp_hdr.checksum = in_cksum((unsigned short *)&echo_req, sizeof(echo_req));

		sendto(sockfd, (char *)&echo_req, sizeof(echo_req), 0,  (struct sockaddr *)(&dest_addr), sizeof(dest_addr));
		
		//sleep(1);
		sendto(sockfd, (char *)&echo_req, sizeof(echo_req), 0,  (struct sockaddr *)(&dest_addr), sizeof(dest_addr));
		dest_addr.sin_addr.s_addr = htonl(add_inc(dest_addr.sin_addr));
		//sleep(10);
		//DEBUG("send icmp ping %d", seq);
	}
	sleep(3);
	close_fd(sockfd);
}


