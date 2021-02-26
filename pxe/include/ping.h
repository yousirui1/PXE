#ifndef __PING_H__
#define __PING_H__

enum {
	PINGAPI_SOCKERROR = -1000,
    PINGAPI_INITERROR,
    PINGAPI_PRIVERROR,
    PINGAPI_TIMEOUT,
    PINGAPI_UNKNOWNPKT,
    PINGAPI_UNREACHABLE,
    PINGAPI_TTLEXPIRE
};


#define PINGAPI_ID 216

/* ICMP packet types */
#define ICMP_ECHO_REPLY 	0
#define ICMP_DEST_UNREACH	3
#define ICMP_ECHO_REQUEST	8
#define ICMP_TTL_EXPIRE		11


/* Mininum ICMP packet size 8 byte */
#define ICMP_MIN 8

#define REQ_DATASIZE 32

typedef struct tagIPHDR
{
	unsigned char vihl;		//version and IHL
	unsigned char tos;		//Type of service
	short tolen;			//Total Length
	short id;				//Identification
	short flag_off;			//Flags and Fragment Offset
	unsigned char ttl;		// Time To Live
	unsigned char protocol;	//Protocol
	unsigned short checksum; //Checksum
	struct in_addr ia_src;	 //Internet Address - Source
	struct in_addr ia_dst;	 //Internet Address - Destination
}IPHDR, *PIPHDR;


typedef struct tagICMPHDR
{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short id;
	unsigned short seq;
	char data;
}ICMPHDR, *PICMPHDR;


struct echo_request
{
	ICMPHDR icmp_hdr;
	unsigned int time;
	char data[REQ_DATASIZE]	
};


struct echo_reply
{
	IPHDR ip_hdr;
	struct echo_request echo_req;
	char filler[256];
};



#endif
