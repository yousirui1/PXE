#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__

#include "client.h"

#define CAPTUREANDCAST_EXPORTS

#ifdef CAPTUREANDCAST_EXPORTS
#define CAPTUREANDCAST_API  __declspec(dllexport)
#else
#define CAPTUREANDCAST_API  __declspec(dllimport)
#endif

CAPTUREANDCAST_API void start_pxe();
CAPTUREANDCAST_API void stop_pxe();

CAPTUREANDCAST_API void get_ver(int *major, int *minor);

//CAPTUREANDCAST_API int load_wsa();
//CAPTUREANDCAST_API void unload_wsa();

#endif //__EXTERNAL_H__
