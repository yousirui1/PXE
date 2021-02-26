#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "base.h"

#define IPADDR_LEN 32
#define DATA_SIZE 1472

struct client
{
    int fd;
    struct in_addr addr;

    int index;
    char ip[IPADDR_LEN];
    unsigned long mac[2];
    int flag;

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


extern struct client **clients;

#endif
