#include "base.h"
#include "queue.h"

void init_queue(queue *p, unsigned char *buf, unsigned int max_size)
{
	p->front = 0;
	p->rear = 0;
	p->buf = buf;
	p->max_size = max_size;	
	p->offset = 0;
}

unsigned char en_queue(queue *p, unsigned char *data, unsigned int size, unsigned char type)
{
	unsigned pos;	
	pos = (p->rear + 1) & 0x7F;		// 0x7F max queue cnt 128
	if(p->front == pos)
	{
		return ERROR;
	}
	else
	{
		if((p->offset + size) > p->max_size)
			p->offset = 0;
		p->index[p->rear].size = size;
		p->index[p->rear].type = type;
		p->index[p->rear].buf = &(p->buf[p->offset]);
		memcpy(&p->buf[p->offset], data, size);
		p->offset += size;
		p->rear = pos;
		return SUCCESS;
	}	
}

queue_index *de_queue(queue *p)
{
	return &(p->index[p->front]);
}

unsigned char de_queue_pos(queue *p)
{
	p->front = ((p->front + 1) & 0x7F);
	return 0;
}

unsigned char empty_queue(queue *p)
{
	if(p->rear == p->front)
		return 0x1;
	else	
		return 0x0;
}

void clear_queue(queue *p)
{
	p->offset = p->rear = p->front = 0;
}

unsigned char full_queue(queue *p)
{
	unsigned int pos;
	pos = ((p->rear + 1) & 0x7F);
	if(p->front == pos)
		return 0;
	else
		return 1;
}

