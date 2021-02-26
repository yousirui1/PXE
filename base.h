#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <strings.h>
#include <assert.h>


#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windowsx.h>
    #include <process.h>
    #include <iphlpapi.h>

#else
    #include <sys/syscall.h>
    #include <sys/epoll.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/wait.h>
    #include <sys/utsname.h>
    #include <sys/resource.h>
    #include <sys/utsname.h>
    #include <sys/ioctl.h>

    #include <netinet/tcp.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <net/if.h>
    #include <arpa/tftp.h>
#endif

#include <QDebug>

void close_fd(int fd);

#define __DEBUG__
#ifdef __DEBUG__
#define DEBUG(format,...) \
        do { qWarning("File:  Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define FAIL(format,...) \
        do { printf("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
            err_msg("File: "__FILE__", Line: %05d:  " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define DIE(format,...) \
        dok { qWarning("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
            err_msg("File: "__FILE__", Line: %05d:  " format"\r\n", __LINE__, ##__VA_ARGS__); \
            pthread_exit((void *)ERROR); \
        }while(0)
#else
#define DEBUG(format,...)
#define FAIL(format,...)
#define DIE(format,...) pthread_exit((void *)ERROR)
#endif      //__DEBUG__

#ifdef _WIN32
    #define LOG_DIR  "c://Users//Public//Documents//YZYEduClient//Log"
    #define LOG_ERR_FILE "c://Users//Public//Documents//YZYEduClient//Log//remote_monitor_err.log"
    #define TTF_DIR "c://Windows//Fonts//simsunb.ttf"
#else
    #define LOG_DIR  "./log"
    #define LOG_ERR_FILE "./log/screen_cast_err.log"
    #define TTF_DIR "./font/msyh.ttf"
#endif  //WIN32

/* base */
#define SUCCESS 0
#define ERROR_1 1

#define DATA_SIZE 1472
#define MAX_BUFLEN      1024
#define MAX_FILENAMELEN 256

#define SCHED_PRIORITY_TFTPD 1
#define SCHED_PRIORITY_DHCPD 2

#define TIMEOUT 60

#endif //__BASE_H__
