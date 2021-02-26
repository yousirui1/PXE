#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

#ifdef CAPTUREANDCAST_EXPORTS
#define CAPTUREANDCAST_API extern "C" __declspec(dllexport)
#else
#define CAPTUREANDCAST_API extern "C" __declspec(dllimport)
#endif

CAPTUREANDCAST_API void init_network();

CAPTUREANDCAST_API void exit_network();

CAPTUREANDCAST_API struct client ** get_clients(struct dhcp_param *param);

CAPTUREANDCAST_API void start_dhcpd();

CAPTUREANDCAST_API void stop_dhcpd();
CAPTUREANDCAST_API int start_httpd();
CAPTUREANDCAST_API void start_tftpd();



#endif //__EXTERNAL_H__
