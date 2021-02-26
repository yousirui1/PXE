#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__


#ifdef __cplusplus
extern "C"
{
#endif

int create_window(int pipe[2], struct client **clients);

#ifdef __cplusplus
}

#endif
#endif
