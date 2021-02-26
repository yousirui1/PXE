#include "base.h"
#include "dict.h"

static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;
static uint8_t dict_hash_function_seed[16];

void set_hash_function_seed_dict(uint8_t *seed)
{
	memcpy(dict_hash_function_seed, seed, sizeof(dict_hash_function_seed));
}

uint8_t *get_hash_function_seed_dict(void)
{
	return dict_hash_function_seed;
}

uint64_t gen_hash_function(const void *key, int len)
{

}

uint64_t gen_case_hash_function(const unsigned char *buf, int len)
{

}

static reset_dict(dictht *ht)
{
	ht->table = NULL;
	ht->size = 0;
	ht->sizemask = 0;
	ht->used = 0;
}

int init_dict(dict *d, dict_type *type, void *data_ptr)
{
	reset_dict(&d->ht[0]);
	reset_dict(&d->ht[1]);

	d->type = type;
	d->privdata = data_ptr;
	d->rehashidx = -1;
	d->iterators = 0;
	return SUCCESS;
}

dict *new_dict(dict_type *type, void *data_ptr)
{
	dict *d = (dict *)malloc(sizeof(*d));
	
	init_dict(d, type, data_ptr);
}

/* expand or create the hash table */
int expand_dict()
{

}


int resize_dict(dict *d)
{

}

int rehash_dict(dict *d, int n)
{

}

int rehash_milliseconds_dict()
{

}

static void rehash_step_dict()
{

}

int add_dict()
{

}

dict_entry *add_raw_dict()
{

}
