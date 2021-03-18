#ifndef __HTTPD_H__
#define __HTTPD_H__

#define _CRT_SECURE_NO_WARNINGS

#ifdef  FD_SETSIZE
#undef  FD_SETSIZE
#define FD_SETSIZE  1024
#else 
#define FD_SETSIZE  1024
#endif

#define BUFFER_UNIT 4096

#ifdef _DEBUG
#define ASSERT(x)   assert(x)
#else
#define ASSERT(x)
#endif

#include <stdio.h>
#include <Winsock2.h>
#include <Windows.h>
#include <assert.h>
#include <time.h>
#include "types.h"
#include "utils.h"
#include "Logger.h"
#include "network.h"
#include "event.h"
#include "http.h"

struct http_conf
{
	int port;
	char dir[128];	
};



#endif
