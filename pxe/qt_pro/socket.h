#ifndef __SOCKET_H__
#define __SOCKET_H__


#define MAXHADDRLEN 16


#define INVALID_SOCKET -1

#define HADDRCMP(a, b) (a[0] == b[0] &&  a[1] == b[1])
#define HADDRCPY(a, b) (a[0] = b[0], a[1] = b[1])


enum UDP_TYPE
{
    UDP_NORMAL = 0,			//udp
    UDP_ICMP,			//ICMP
    UDP_BOARDCAST,		//广播
    UDP_MREQ,			//组播
};


extern int max_connections;
int load_wsa();
void unload_wsa();
void close_fd(int fd);
char *haddrtoa(const unsigned char *haddr, int len, char sep);


#endif




