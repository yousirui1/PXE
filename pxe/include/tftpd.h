#ifndef __TFTPD_H__
#define __TFTPD_H__


static const char * const errmsgs[] =
{
  "Undefined error code",           /* 0 - EUNDEF */
  "File not found",             /* 1 - ENOTFOUND */
  "Access denied",              /* 2 - EACCESS */
  "Disk full or allocation exceeded",       /* 3 - ENOSPACE */
  "Illegal TFTP operation",         /* 4 - EBADOP */
  "Unknown transfer ID",            /* 5 - EBADID */
  "File already exists",            /* 6 - EEXISTS */
  "No such user",               /* 7 - ENOUSER */
  "Failure to negotiate RFC2347 options"    /* 8 - EOPTNEG */
};


/*
 * Trivial File Transfer Protocol (IEN-133)
 */
#define TFTP_SEGSIZE          512     /* data segment size */
#define TFTP_MAXSEGSIZE     16384       /* data segment size */
#define TFTP_MINSEGSIZE         8       /* data segment size */



#define TFTP_RRQ    01          /* read request */
#define TFTP_WRQ    02          /* write request */
#define TFTP_DATA   03          /* data packet */
#define TFTP_ACK    04          /* acknowledgement */
#define TFTP_ERROR  05          /* error code */
#define TFTP_OACK   06          /* option acknowledgement */


struct    tftphdr {
  short   th_opcode;      /* packet type */
  union {
          short   tu_block;   /* block # */
          short   tu_code;    /* error code */
          char    tu_stuff[1];    /* request packet stuff */
  } th_u;
  char    th_data[2];     /* data or error string */
};


#define  th_block    th_u.tu_block
#define  th_code     th_u.tu_code
#define  th_stuff    th_u.tu_stuff
#define  th_msg      th_data

#define TFTP_DATA_HEADERSIZE ( offsetof (struct tftphdr, th_data ) )
#define TFTP_ACK_HEADERSIZE  (  offsetof (struct tftphdr, th_block )  \
                            + sizeof ( ((struct tftphdr *) (0))->th_block) )


/*
 * Error codes.
 */
#define  EUNDEF      0       /* not defined */
#define  ENOTFOUND   1       /* file not found */
#define  EACCESS     2       /* access violation */
#define  ENOSPACE    3       /* disk full or allocation exceeded */
#define  EBADOP      4       /* illegal TFTP operation */
#define  EBADID      5       /* unknown transfer ID */
#define  EEXISTS     6       /* file already exists */
#define  ENOUSER     7       /* no such user */
#define  EBADOPTION  8        /* bad option */
#define  ECANCELLED 99      /* cancelled by administrator */


/* 
 * options 
 */
#define TFTP_OPT_TSIZE     "tsize"
#define TFTP_OPT_TIMEOUT   "timeout"
#define TFTP_OPT_BLKSIZE   "blksize"
#define TFTP_OPT_MCAST     "multicast"
#define TFTP_OPT_PORT      "udpport"

#define IS_OPT(s,opt)   (lstrcmpi (s, opt)==0)

#define PKTSIZE             TFTP_SEGSIZE+4
#define MAXPKTSIZE          TFTP_MAXSEGSIZE+4



#define E_WOULD_BLOCK(x) ((x) == EAGAIN || (x) == EWOULDBLOCK)

#if 0
struct formats;

char *rewrite_access(char *, int , const char **);
int validate_access(char *, int , struct formats *, const char **);
void tftp_sendfile(struct formats *, struct tftphdr *, int );
void tftp_recvfile(struct formats *, struct tftphdr *, int );


void do_opt(char *, char *, char **);

int set_blksize(char *, char **);
int set_blksize2(char *, char **);
int set_tsize(char *, char **);
int set_timeout(char *, char **);
int set_utimeout(char *, char **);


struct formats {
    const char *f_mode;
    char *(*f_rewrite)(char *, int , const char **);
    int (*f_validate)(char *, int, struct formats *, const char **);
    void (*f_send)(struct formats *, struct tftphdr *, int );
    void (*f_recv)(struct formats *, struct tftphdr *, int );
    int f_convert;
}formats[] = {
    {"netascii", rewrite_access, validate_access, tftp_sendfile, tftp_recvfile, 1},
    {"octet",    rewrite_access, validate_access, tftp_sendfile, tftp_recvfile, 0},
    {NULL, NULL, NULL, NULL, NULL, 0}
};


struct options {
    const char *o_opt;
    int (*o_fnc)(char *, char **);
}options[] = {
    { "blksize",    set_blksize},
    { "blksize2",   set_blksize2},
    { "tsize",      set_tsize},
    { "timeout",    set_timeout},
    { "utimeout",   set_utimeout},
    { "NULL",       NULL},
};
#endif

#endif
