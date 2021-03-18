#ifndef __DHCPD_H__
#define __DHCPD_H__

//#include <stdint.h>

/* DHCP message types. */
#define DHCPDISCOVER  1
#define DHCPOFFER     2
#define DHCPREQUEST   3
#define DHCPDECLINE   4
#define DHCPACK       5
#define DHCPNAK       6
#define DHCPRELEASE   7
#define DHCPINFORM    8

#define DHCP_UDP_OVERHEAD   (14 + /* Ethernet header */     \
                             20 + /* IP header */           \
                              8)   /* UDP header */
#define DHCP_SNAME_LEN        64
#define DHCP_FILE_LEN        128
#define DHCP_FIXED_NON_UDP   236
#define DHCP_FIXED_LEN      (DHCP_FIXED_NON_UDP + DHCP_UDP_OVERHEAD)
                       /* Everything but options. */
#define DHCP_MTU_MAX        1500
#define DHCP_OPTION_LEN     (DHCP_MTU_MAX - DHCP_FIXED_LEN)

#define BOOTP_MIN_LEN        300
#define DHCP_MIN_LEN         548

/* Magic cookie validating dhcp options field (and bootp vendor
   extensions field). */
#define DHCP_OPTIONS_COOKIE    "\143\202\123\143"

/* DHCP Option codes: */

#define DHO_PAD                         0
#define DHO_SUBNET_MASK                 1
#define DHO_TIME_OFFSET                 2
#define DHO_ROUTERS                     3
#define DHO_TIME_SERVERS                4
#define DHO_NAME_SERVERS                5
#define DHO_DOMAIN_NAME_SERVERS         6
#define DHO_LOG_SERVERS                 7
#define DHO_COOKIE_SERVERS              8
#define DHO_LPR_SERVERS                 9
#define DHO_IMPRESS_SERVERS             10
#define DHO_RESOURCE_LOCATION_SERVERS   11
#define DHO_HOST_NAME                   12
#define DHO_BOOT_SIZE                   13
#define DHO_MERIT_DUMP                  14
#define DHO_DOMAIN_NAME                 15
#define DHO_SWAP_SERVER                 16
#define DHO_ROOT_PATH                   17
#define DHO_EXTENSIONS_PATH             18
#define DHO_IP_FORWARDING               19
#define DHO_NON_LOCAL_SOURCE_ROUTING    20
#define DHO_POLICY_FILTER               21
#define DHO_MAX_DGRAM_REASSEMBLY        22
#define DHO_DEFAULT_IP_TTL              23
#define DHO_PATH_MTU_AGING_TIMEOUT      24
#define DHO_PATH_MTU_PLATEAU_TABLE      25
#define DHO_INTERFACE_MTU               26
#define DHO_ALL_SUBNETS_LOCAL           27
#define DHO_BROADCAST_ADDRESS           28
#define DHO_PERFORM_MASK_DISCOVERY      29
#define DHO_MASK_SUPPLIER               30
#define DHO_ROUTER_DISCOVERY            31
#define DHO_ROUTER_SOLICITATION_ADDRESS  32
#define DHO_STATIC_ROUTES               33
#define DHO_TRAILER_ENCAPSULATION       34
#define DHO_ARP_CACHE_TIMEOUT           35
#define DHO_IEEE802_3_ENCAPSULATION     36
#define DHO_DEFAULT_TCP_TTL             37
#define DHO_TCP_KEEPALIVE_INTERVAL      38
#define DHO_TCP_KEEPALIVE_GARBAGE       39
#define DHO_NIS_DOMAIN                  40
#define DHO_NIS_SERVERS                 41
#define DHO_NTP_SERVERS                 42
#define DHO_VENDOR_ENCAPSULATED_OPTIONS  43
#define DHO_NETBIOS_NAME_SERVERS        44
#define DHO_NETBIOS_DD_SERVER           45
#define DHO_NETBIOS_NODE_TYPE           46
#define DHO_NETBIOS_SCOPE               47
#define DHO_FONT_SERVERS                48
#define DHO_X_DISPLAY_MANAGER           49
#define DHO_DHCP_REQUESTED_ADDRESS      50
#define DHO_DHCP_LEASE_TIME             51
#define DHO_DHCP_OPTION_OVERLOAD        52
#define DHO_DHCP_MESSAGE_TYPE           53
#define DHO_DHCP_SERVER_IDENTIFIER      54
#define DHO_DHCP_PARAMETER_REQUEST_LIST 55
#define DHO_DHCP_MESSAGE                56
#define DHO_DHCP_MAX_MESSAGE_SIZE       57
#define DHO_DHCP_RENEWAL_TIME           58
#define DHO_DHCP_REBINDING_TIME         59
#define DHO_VENDOR_CLASS_IDENTIFIER     60
#define DHO_DHCP_CLIENT_IDENTIFIER      61
#define DHO_NWIP_DOMAIN_NAME            62
#define DHO_NWIP_SUBOPTIONS             63
#define DHO_TFTP_SERVER                 66
#define DHO_BOOT_FILE					67
#define DHO_USER_CLASS                  77
#define DHO_FQDN                        81
#define DHO_DHCP_AGENT_OPTIONS          82

/* Options 93, 94, and 97 are define in the Intel Preboot Execution
   Environment (PXE) specification, version 2.1, September 20, 1999,
   page 19. */
#define DHO_PXE_CLIENT_ARCH_ID          93
#define  DHO_PXE_CLIENT_NETIF_ID        94
#define  DHO_PXE_UUID                   97

#define DHO_SUBNET_SELECTION           118 /* RFC3011! */
#define DHO_SIP_SERVERS                120
/* The DHO_AUTHENTICATE option is not a standard yet, so I've
   allocated an option out of the "local" option space for it on a
   temporary basis.  Once an option code number is assigned, I will
   immediately and shamelessly break this, so don't count on it
   continuing to work. */
#define DHO_AUTHENTICATE               210
#define DHO_CUSTOM					   254
#define DHO_END                        255

/* BOOTP (rfc951) message types */
#define BOOTREQUEST 1
#define BOOTREPLY   2

/* Possible values for flags field... */
#define BOOTP_BROADCAST 32768L

#define BOOTPC_PORT  68
#define BOOTPS_PORT  67
#define DHCP_PINGTIMEOUT    500


struct dhcp_packet 
{
    uint8_t  op;                 /* 0: Message opcode/type */
    uint8_t  htype;              /* 1: Hardware addr type (net/if_types.h) */
    uint8_t  hlen;               /* 2: Hardware addr length */
    uint8_t  hops;               /* 3: Number of relay agent hops from client */
    uint32_t xid;                /* 4: Transaction ID */
    uint16_t secs;               /* 8: Seconds since client started looking */
    uint16_t flags;              /* 10: Flag bits */
    struct in_addr ciaddr;        /* 12: Client IP address (if already in use) */
    struct in_addr yiaddr;        /* 16: Client IP address */
    struct in_addr siaddr;        /* 18: IP address of next server to talk to */
    struct in_addr giaddr;        /* 20: DHCP relay agent IP address */
    unsigned char chaddr [16];    /* 24: Client hardware address */
    char sname [DHCP_SNAME_LEN];  /* 40: Server name */
    char file [DHCP_FILE_LEN];    /* 104: Boot filename */
    unsigned char options [DHCP_OPTION_LEN];
};

enum e_services 
{
	TFTPD32_NONE=0,
    TFTPD32_TFTP_SERVER  = 0x0001,
    TFTPD32_TFTP_CLIENT  = 0x0002,
    TFTPD32_DHCP_SERVER  = 0x0004,
    TFTPD32_SYSLOG_SERVER= 0x0008,
    TFTPD32_SNTP_SERVER  = 0x0010,
    TFTPD32_DNS_SERVER   = 0x0020,

    TFTPD32_CONSOLE      = 0x1000,
    TFTPD32_REGISTRY     = 0x2000,
    TFTPD32_SCHEDULER    = 0x4000,
};

#define is_dhcp(x) (*(uint32_t *)(x).options == *(uint32_t *)DHCP_OPTIONS_COOKIE)
#define sizeof_tab(x) (sizeof(x) / sizeof(x[0]))

struct dhcp_conf
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
    char domain_name[128];
	int lease;
    int ignore_bootp;
	int again;
    char pxe_boot[256];
    char ipxe_boot[256];
    struct
    {
        int add_option;
        char add_option_buf[128];
    }t[10];
};

typedef struct dhcp_conf dhcp_conf;
extern struct dhcp_conf dhcp_config;

struct dhcp_type
{
    int type;
    char *desc;
};

static struct dhcp_type dhcp_types[] =
{
    {0,             "BootP",           },
    {DHCPDISCOVER,  "DHCP Discover",   },
    {DHCPOFFER,     "DHCP Offer",      },
    {DHCPREQUEST,   "DHCP Rqst",       },
    {DHCPDECLINE,   "DHCP decline",    },
    {DHCPACK,       "DHCP Ack",        },
    {DHCPNAK,       "DHCP Nak",        },
    {DHCPRELEASE,   "DHCP release",    },
    {DHCPINFORM,    "DHCP inform"      },
    {DHCPINFORM+1,  "Invalid Pkt"      },
};

struct dhcp_option
{
    unsigned opt;
    char len;
    int service;
};

static struct dhcp_option dhcp_opts[] =
{
    DHO_DHCP_MESSAGE_TYPE,      1,    ~TFTPD32_NONE,
    DHO_DHCP_SERVER_IDENTIFIER, 4,    ~TFTPD32_NONE,
    DHO_SUBNET_MASK,            4,    ~TFTPD32_NONE,
    DHO_ROUTERS,                4,    ~TFTPD32_NONE,
    DHO_DOMAIN_NAME_SERVERS,    0,    ~TFTPD32_NONE,
    DHO_DHCP_LEASE_TIME,        4,    ~TFTPD32_NONE,
    DHO_DHCP_RENEWAL_TIME,      4,    ~TFTPD32_NONE,
    DHO_DHCP_REBINDING_TIME,    4,    ~TFTPD32_NONE,
    DHO_NTP_SERVERS,            0,    ~TFTPD32_NONE,
    DHO_SIP_SERVERS,            0,    ~TFTPD32_NONE,
    DHO_BOOT_SIZE,              0,     TFTPD32_TFTP_SERVER,
    DHO_BOOT_FILE,            	0,    ~TFTPD32_NONE,
    DHO_DOMAIN_NAME,            0,    ~TFTPD32_NONE,
    DHO_DHCP_AGENT_OPTIONS,     0,    ~TFTPD32_NONE,
    DHO_CUSTOM,                 0,    ~TFTPD32_NONE,
    DHO_END,                    0,    ~TFTPD32_NONE,
};

#if 0
static struct dhcp_option dhcp_opts[] =
{
    DHO_DHCP_MESSAGE_TYPE,      1,    ~TFTPD32_NONE,
    DHO_DHCP_SERVER_IDENTIFIER, 4,    ~TFTPD32_NONE,
    DHO_SUBNET_MASK,            4,    ~TFTPD32_NONE,
    DHO_ROUTERS,                4,    ~TFTPD32_NONE,
    DHO_DOMAIN_NAME_SERVERS,    0,    ~TFTPD32_NONE,
    DHO_NETBIOS_NAME_SERVERS,   0,    ~TFTPD32_NONE,
    DHO_DHCP_LEASE_TIME,        4,    ~TFTPD32_NONE,
    DHO_DHCP_RENEWAL_TIME,      4,    ~TFTPD32_NONE,
    DHO_DHCP_REBINDING_TIME,    4,    ~TFTPD32_NONE,
    DHO_LOG_SERVERS,            4,     TFTPD32_SYSLOG_SERVER,
    DHO_NTP_SERVERS,            0,    ~TFTPD32_NONE,
    DHO_SIP_SERVERS,            0,    ~TFTPD32_NONE,
    DHO_BOOT_SIZE,              0,     TFTPD32_TFTP_SERVER,
    DHO_BOOT_FILE,            	0,    ~TFTPD32_NONE,
    DHO_TFTP_SERVER,            0,     TFTPD32_TFTP_SERVER,
    DHO_DOMAIN_NAME,            0,    ~TFTPD32_NONE,
    DHO_DHCP_AGENT_OPTIONS,     0,    ~TFTPD32_NONE,
    DHO_CUSTOM,                 0,    ~TFTPD32_NONE,
    DHO_END,                    0,    ~TFTPD32_NONE,
};
#endif


#define MAXHADDRLEN 16
#define CLASS_A_LOOPBACK 127

#endif
