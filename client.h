#ifndef __CLIENT_H__
#define __CLIENT_H__



#define IPADDR_LEN 32
#define DATA_SIZE 1472

struct client
{
    int fd;
    struct in_addr addr;

    char ip[IPADDR_LEN];
    unsigned long mac[2];

    pthread_t pthread_tftp;

    unsigned char *data_buf;
    unsigned int data_size;

    unsigned int blksize;
    off_t tsize;
    unsigned long timeout;
    unsigned long rexmtval;
    unsigned long maxtimeout;
    unsigned long utimeout;

    char ackbuf[DATA_SIZE];

    time_t last_time;

    FILE *file;
};

struct dhcp_param
{
    int fd;
    char tftp_ip[64];
    char local_ip[64];
    char pool_ip[64];
    int pool_size;
    char mask[64];
    char dns1[64];
    char dns2[64];
    char wins[64];
    char gateway[64];
    char opt42[64];
    char opt120[64];
    char boot_file[256];
    char domain_name[128];
    int lease;
    int ignore_bootp;
    struct
    {
        int add_option;
        char add_option_buf[128];
    }t[10];
};

#endif
