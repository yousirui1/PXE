#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

#include "client.h"

#define CAPTUREANDCAST_EXPORTS

#ifdef CAPTUREANDCAST_EXPORTS
#define CAPTUREANDCAST_API  __declspec(dllexport)
#else
#define CAPTUREANDCAST_API  __declspec(dllimport)
#endif


CAPTUREANDCAST_API void init_network();

CAPTUREANDCAST_API void exit_network();


CAPTUREANDCAST_API struct client **get_clients(struct dhcp_param *param);

CAPTUREANDCAST_API void start_dhcpd();

CAPTUREANDCAST_API void stop_dhcpd();
CAPTUREANDCAST_API int start_httpd();
CAPTUREANDCAST_API void start_tftpd();


//CAPTUREANDCAST_API int load_wsa();
//CAPTUREANDCAST_API void unload_wsa();

#endif //__EXTERNAL_H__
