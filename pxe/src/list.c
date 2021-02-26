#include "base.h"
#include "list.h"

list *new_list(void)
{
	struct list *list;
	if((list = (struct list *)malloc(sizeof(* list))) == NULL)
		return NULL;
	
	list->head = list->tail = NULL;
	list->len = 0;
	list->dup = NULL;
	list->free = NULL;
	list->match = NULL;
	return list;
}

void empty_list(list *list)
{
	unsigned long len;
	list_node *current, *next;
	
	current = list->head;
	len = list->len;
	while(len --)
	{
		next = current->next;
		if(list->free)
			list->free(current->value);
		free(current);
		current = next;
	}
	list->head = list->tail = NULL;
	list->len = 0;
}

void release_list(list *list)
{
	empty_list(list);
	free(list);
}

list *add_node_head_list()
{


}

list *add_node_tail_list()
{

}

list *insert_node_list()
{

}

void del_node_list()
{

}

list_iter *get_list_iterator()
{

}

void release_iterator(list_iter *iter)
{

}

void rewind_list()
{

}

void rewind_tail_list()
{

}

list_node *next_list()
{

}

list *dup_list()
{

}

list_node *search_key_list()
{


}

list_node *index_list(list *list, long index)
{

}

void join_list()
{


}

