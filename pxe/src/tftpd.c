#include "base.h"
#include "tftpd.h"
#include "client.h"

#define STRPREFIX(a,b) (strncmp((a),(b),strlen((b))) == 0)

char *rewrite_access(char *in_filename, int mode)
{
	//char filename[MAX_FILENAMELEN] = "c://tftp/";	
	char filename[MAX_FILENAMELEN] = {0};

	strcat(filename, "c://tftp/");
	strcat(filename, in_filename);
	
	return filename;
}

int validate_access(struct client *cli, char *in_filename, int mode)
{
	struct stat stbuf;
	int fd, wmode, rmode;
	const char **dirp;

	//search_dir();
	
	//filename = rewrite_access(in_filename, mode);

	char filename[MAX_FILENAMELEN] = {0};
	char *dir = NULL;	

	if((dir = getcwd(NULL, 0)) == NULL)
	{
		strcat(filename, "./voi_pxe/");
	}
	else
	{
		strcat(filename, dir);
		strcat(filename, "/voi_pxe/");
		free(dir);
	}

	strcat(filename, in_filename);
	DEBUG("filename %s", filename);

	fd = open(filename, mode == TFTP_RRQ ? O_RDONLY:O_WRONLY, 0666);
	if(fd < 0)
	{
		DEBUG("open file : %s  error", filename);
		return errno;	
	}
	
	if(fstat(fd, &stbuf) < 0)
	{
		DEBUG("file: %s fstat error", filename);
		return ERROR;
	}

	DEBUG("mode %d TFTP_RRQ %d", mode, TFTP_RRQ);
	if(mode == TFTP_RRQ)
	{
		if(stbuf.st_mode & (S_IREAD >> 6) == 0)
		{
			DEBUG("File must have global read permissions");
			return ERROR;
		}
		cli->tsize = stbuf.st_size;
		//cli->file = fdopen(fd, "rb");	
		close(fd);
		cli->file = fopen(filename, "rb");	
		DEBUG("tsize %d", cli->tsize);
	}
	return SUCCESS;
}

static char *tftp_get_option(const char *option, char *buf, int len)
{
    int opt_val = 0;
    int opt_found = 0;
    int k;

    /* buf options to:
     * "opt_name<NUL>opt_val<NUL>opt_name2<NUL>opt_val2<NUL>..." */

    while(len > 0)
    {   
        /* Make sure options are terminated correctly */
        for(k = 0; k < len; k++)
        {   
            if(buf[k] == '\0')
            {   
                goto nul_found;
            }   
        }   
        return NULL;
nul_found:
        if(opt_val == 0)    //it's "name" part
        {   
            if(strcasecmp(buf, option) == 0)
            {   
                opt_found = 1;
            }   
        }   
        else if(opt_found)
        {   
            return buf;
        }   
        k++;
        buf += k;
        len -= k;
        opt_val ^= 1;   
    }   
    return NULL;
}

void tftp_send_error(struct client *cli, uint16_t code, char *err_msg)
{
	char *cp = cli->ackbuf + 2;
	int send_len = 0;

	((struct tftphdr *)cli->ackbuf)->th_opcode = htons(TFTP_ERROR);
	
    cp += sprintf(cp, "%u", code) + 1;
	
	strcpy(cp, err_msg);
    cp += strlen(err_msg);
		
	send_len = cp - cli->ackbuf;
	send(cli->fd, (char *)cli->ackbuf, send_len, 0);
	
}


int tftp_sendfile(struct client *cli)
{
	int ret;
	int size = cli->blksize;
	int block = 1;
	struct tftphdr *hdr = (struct tftpdhr *)cli->ackbuf;
	off_t file_blk = cli->tsize / cli->blksize;

	fd_set fdset;
	uint16_t opcode;
	int count = 0;
	struct timeval tv;
	int timeout = TIMEOUT;

	size = fread(hdr->th_data, 1, cli->blksize, cli->file);
	if(size < 0 )
	{
		DEBUG("read file error");
		goto abort;
	}

	hdr->th_opcode = htons((uint16_t)TFTP_DATA);
	hdr->th_block = htons((uint16_t)block);

	if(send(cli->fd, (char *)hdr, size + 4, 0) != size + 4)
	{
		DEBUG("send file error %d", errno);
		goto abort;
	}
	do{
		FD_ZERO(&fdset);
		FD_SET(cli->fd, &fdset);

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		
		ret = select(cli->fd + 1, &fdset, NULL, NULL, &tv);
		if(ret == 1)
		{
			ret = recv(cli->fd, cli->ackbuf, sizeof(cli->data_buf), 0);
			if(ret <= 0)
			{
				continue;	
			}
			else
			{
				timeout = TIMEOUT;

				opcode = ntohs((uint16_t)hdr->th_opcode);
				block = ntohs((uint16_t)hdr->th_block);
				block++;			
			}

			if(opcode == TFTP_ACK)
			{
				//fseek(cli->file, ((uint64_t)(count * UINT16_MAX)+(block - 1)) * cli->blksize, SEEK_SET);
				//DEBUG("block %d  process %d",block, (uint64_t)(count* UINT16_MAX + block) * 100 / file_blk);
				//DEBUG("count %d block %d blksize %d", count, block , cli->blksize);
				//DEBUG("file_size %llu", cli->tsize);
				//DEBUG("offset %llu ", ((uint64_t)(count * (UINT16_MAX + 1))+(block - 1)) * cli->blksize); 

				//if(block == UINT16_MAX + 1)
                //	count ++;
send_file:
				size = fread(hdr->th_data, 1, cli->blksize, cli->file);
				if(size < 0 )
				{
					DEBUG("read file error");
					goto abort;
				}

				hdr->th_opcode = htons((uint16_t)TFTP_DATA);
				hdr->th_block = htons((uint16_t)block);

				if(send(cli->fd, (char *)hdr, size + 4, 0) != size + 4)
				{
					DEBUG("send file error %d", errno);
					goto abort;
				}
			}
		}
		else
		{
			timeout --;
		}

	}while(size == cli->blksize && timeout);
	DEBUG("send over");
	if(timeout == 0)
	{
		DEBUG("client %s recv timeout ", cli->ip);
	}

abort:
	return ERROR;
}


int tftp_option_reply(struct client *cli)
{
	int ret;
	char *filename, *mode = NULL;
	struct tftphdr *hdr = (struct tftphdr *)cli->data_buf;
	uint16_t opcode = ntohs((uint16_t)hdr->th_opcode);
	char *cp = cli->ackbuf + 2;
	int send_len = 0;
	char *res = NULL;

	filename = (char *)&(hdr->th_stuff);
	if(validate_access(cli, filename, opcode) != SUCCESS)
	{
		DEBUG("validate_access error");
		tftp_send_error(cli, ENOTFOUND, errmsgs[ENOTFOUND]);
		return ERROR;
	}
	res = tftp_get_option("tsize", &cli->data_buf[2], cli->data_size - 2);
	if(res)
	{
		((struct tftphdr *)cli->ackbuf)->th_opcode = htons(TFTP_OACK);
		
        strcpy(cp, "tsize");
        cp += sizeof("tsize");
        cp += sprintf(cp, "%llu", cli->tsize) + 1;
			
		send_len = cp - cli->ackbuf;
		
		if(send(cli->fd, (char *)cli->ackbuf, send_len, 0) != send_len)
		{
			DEBUG("send tsize error");
			return ERROR;
		}	
		if(ret = recv(cli->fd, cli->ackbuf, sizeof(cli->data_buf), 0) < 0);
		{
			DEBUG("recv tsize error");	
		}
	}
	if(opcode == TFTP_WRQ)		//上传
	{

	}
	else						//下载
	{
		ret = tftp_sendfile(cli);	
	}
	return ret;
}


void *thread_tftp(void *param)
{
    int ret = SUCCESS;
    pthread_attr_t st_attr;
    struct sched_param sched;
    
	struct client *cli = (struct client *)param;
	    
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {    
        DEBUG("ThreadUdp attr init warning ");
    }    
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {    
        DEBUG("ThreadUdp set SCHED_FIFO warning");
    }    
    sched.sched_priority = SCHED_PRIORITY_TFTPD;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

	DEBUG("----------- tftp thread start -------------------");

	ret = tftp_option_reply(cli);

	close_fd(cli->fd);	

	if(cli->file)
		fclose(cli->file);

	if(cli->data_buf)
		free(cli->data_buf);
	
	free(cli);	
	DEBUG("----------- tftp thread end -------------------");
    return (void *)ret;
}


int init_tftpd()
{
	int sockfd = -1, connfd = - 1, ret, i, nready;
	struct sockaddr_in recv_addr, send_addr, cli_addr;
	char *address = NULL;				//Addresss to listen to
	fd_set reset, allset;
	int maxfd = 0;
	
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	char buf[DATA_SIZE];
	
	struct client *cli = NULL;
	struct tftphdr *hdr = (struct tftphdr*)buf;
	uint16_t opcode = 0;

    int socklen =  sizeof (struct sockaddr_in);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(IPPORT_TFTP);

	memset(&cli_addr, 0, sizeof(struct sockaddr_in));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(IPPORT_TFTP);
	memset(&cli_addr.sin_addr, 0, sizeof(cli_addr.sin_addr));
	
	if(address)
	{

	}	
	
	if(bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0)
	{
		DEBUG("cannot bind to local socket port %d", IPPORT_TFTP);
		return ERROR;
	}	

	FD_ZERO(&allset);
	FD_SET(sockfd, &allset);
	//FD_SET(pipe_udp[0], &allset);

	maxfd = maxfd > sockfd ? maxfd : sockfd;	
	
	for(;;)
	{
		tv.tv_sec = 1;
		reset = allset;
		ret = select(maxfd + 1, &reset, NULL, NULL, &tv);
		if(ret == -1)
		{
			if(errno == EINTR)
				continue;
			else if(errno != EBADF)
			{
				DEBUG("select ret: %d error: %s", errno, strerror(errno));
				break;
			}
		}
		if(FD_ISSET(sockfd, &reset))
		{
			ret = recvfrom(sockfd, (char *)buf, sizeof(buf), 0, (struct sockaddr*)&send_addr, &socklen);
			
			if(ret < 0)
			{
				if(E_WOULD_BLOCK(errno) || errno == EINTR)
					continue;		
				else
				{
					DEBUG("recvfrom error");
					break;
				}
			}

			if(send_addr.sin_family != AF_INET)
			{
				DEBUG("received address was not AF_INET, please check your inetd config");
			}	

			if(cli_addr.sin_addr.s_addr = INADDR_ANY)
			{
				DEBUG("memcpy sin_addr");
				memcpy(&cli_addr.sin_addr, &recv_addr.sin_addr, sizeof(recv_addr.sin_addr));	
			}
			opcode = ntohs(hdr->th_opcode);

			if(opcode == TFTP_RRQ || opcode == TFTP_WRQ)
			{
				connfd = socket(AF_INET, SOCK_DGRAM, 0);
				if(connfd < 0)
				{
					DEBUG("connfd socket error ");
				}
			
				send_addr.sin_family = AF_INET;
				cli_addr.sin_port = htons(0);		//we want a new local port 

				if(bind(connfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) < 0)
				{
					DEBUG("bind connfd: %d error port 0", connfd);
					close_fd(connfd);
					break;
				}
			
				if(connect(connfd, (struct sockaddr *)&send_addr, sizeof(send_addr)) < 0)
				{
					DEBUG("connect connfd : %d error", connfd);
					close_fd(connfd);
					break;
				}	
			
				cli = (struct client *)malloc(sizeof(struct client));
				if(!cli)
				{
					DEBUG("new connect and malloc struct client error :%s", strerror(errno));
                	continue;
				}	
				memset(cli, 0, sizeof(struct client));
				cli->fd = connfd;
				cli->data_size = ret;
				cli->data_buf = (unsigned char *)malloc(cli->data_size + 1);
				cli->blksize = 512;
				if(!cli->data_buf)
				{
					DEBUG("cli malloc data_buf size  %d error :%s", cli->data_size, strerror(errno));	
					close_fd(connfd);
					free(cli);
					continue;	
				}
				memcpy(cli->data_buf, buf, cli->data_size);

#ifdef _WIN32
				memcpy(cli->ip,inet_ntoa(send_addr.sin_addr), sizeof(cli->ip));
#else
				if(inet_ntop(AF_INET, &send_addr.sin_addr, cli->ip, sizeof(cli->ip)) == NULL)
				{
					DEBUG("inet_ntop error");		
				}
#endif
				pthread_create(&cli->pthread_tftp, NULL, thread_tftp, cli);
			}
		}
	}
}

void *thread_tftpd(void *param)
{
	init_tftpd();
}

