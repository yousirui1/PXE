#ifndef __QUEUE_H__
#define __QUEUE_H__

#define MAX_QUEUESIZE 128

typedef struct 
{
	unsigned int size;
	unsigned char *buf;
	unsigned char type;
}queue_index;

typedef struct
{
	unsigned int front;
	unsigned int rear;
	unsigned int max_size;
	unsigned int offset;
	unsigned char *buf;
	queue_index index[MAX_QUEUESIZE];
}queue;








#endif //__QUEUE_H__
