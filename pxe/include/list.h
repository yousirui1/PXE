#ifndef __LIST_H__
#define __LIST_H__

typedef struct list_node {
	struct list_node *prev;
	struct list_node *next;
	
	void *value;
}list_node;

typedef struct list_iter {


}list_iter;

typedef struct list {
	list_node *head;
	list_node *tail;
	void *(*dup) (void *ptr);
	void (*free) (void *ptr);
	int (*match) (void *ptr, void *key);
	unsigned long len;
}list;


/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)


#endif	//__LIST_H__
