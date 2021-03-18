#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

//#define CAPTUREANDCAST_EXPORTS

#ifdef CAPTUREANDCAST_EXPORTS
#define CAPTUREANDCAST_API  __declspec(dllexport)
#else
#define CAPTUREANDCAST_API  __declspec(dllimport)
#endif


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

struct tftp_conf
{
    char dir[128];
    unsigned int timeout;
};

typedef struct netcard_conf netcard_conf;
typedef struct server_conf server_conf;
typedef struct tftp_conf tftp_conf;
typedef struct http_conf http_conf;
typedef struct dhcp_conf dhcp_conf;

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

CAPTUREANDCAST_API void get_ver(int *major, int *minor);
CAPTUREANDCAST_API void start_pxe(struct configs *conf);
CAPTUREANDCAST_API void stop_pxe();

//CAPTUREANDCAST_API int load_wsa();
//CAPTUREANDCAST_API void unload_wsa();

#endif //__EXTERNAL_H__
