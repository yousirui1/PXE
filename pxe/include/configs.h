#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#define CONFIG_FILE "config.ini"

#define BASE_SECTION "base"
#define BASE_TYPE_KEY "type"

#define NET_SECTION "network"
#define NET_DHCP_KEY "dhcp"
#define NET_IP_KEY "ip"
#define NET_GATEWAY_KEY "gateway"
#define NET_NETMASK_KEY "netmask"
#define NET_DNS1_KEY "dns1"
#define NET_DNS2_KEY "dns2"
#define NET_MAC_KEY "mac"

#define SERVER_SECTION "server"
#define SERVER_IP_KEY   "server_ip"
#define SERVER_PORT_KEY "server_port"
#define SERVER_PORT 50007


#define DHCPD_SECTION "dhcpd"
#define DHCPD_IP_POOL_KEY "ip_pool"
#define DHCPD_LEASE_KEY "lease"
#define DHCPD_POOL_SIZE_KEY "pool_size"
#define DHCPD_AGAIN_KEY "again"
#define DHCPD_PXE_BOOT_KEY "pxe_boot"
#define DHCPD_IPXE_BOOT_KEY "ipxe_boot"

#define TFTPD_SECTION "tftpd"
#define TFTPD_TIMEOUT_KEY "timeout"
#define TFTPD_DIR_KEY "dir"

#define HTTPD_SECTION "httpd"
#define HTTPD_PORT_KEY "port"
#define HTTPD_DIR_KEY "dir"
#define HTTPD_HFS_KEY "hfs"

#define VERSION_SECTION "version"
#define VER_MAJOR_KEY "major_ver"
#define VER_MINOR_KEY "minor_ver"
#define VER_LINUX_KEY "linux"

#define DEFAULT_IP_VALUE "127.0.0.1"
#define DEFAULT_PORT_VALUE 50007

#define CONFIG_DEFAULT_NULL ""

struct netcard_conf
{
    char is_dhcp;                   //0 不启动 1 启动
    char name[32];
    unsigned char ip[32];
    unsigned char mac[32];
    unsigned char gateway[32];
    unsigned char boardcast_addr[32];
    unsigned char netmask[32];
    unsigned char dns1[32];
    unsigned char dns2[32];
    //char desc[128];               //网卡描述 厂商信息
};


struct server_conf {
    unsigned char ip[32];
    unsigned int port;
};


struct http_conf
{
    int port;
    char dir[128];
	int hfs;
};


typedef struct netcard_conf netcard_conf;
typedef struct server_conf server_conf;

extern struct dhcp_conf;
extern struct tftp_conf;
extern struct http_conf;

typedef struct tftp_conf tftp_conf;
typedef struct http_conf http_conf;

struct configs{
    int type;
    netcard_conf netcard;

    server_conf server;

	struct dhcp_conf dhcp;
	struct tftp_conf tftp;
	struct http_conf http;

    int major_ver;
    int minor_ver;
    int config_ver;
};


#endif //__CONFIGS_H__
