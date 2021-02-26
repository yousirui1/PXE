#ifndef __DICT_H__
#define __DICT_H__

typedef struct dict_entry {
	void *key;
	union {
		void *val;
		uint64_t u64;
		int64_t s64;
		double d;
	}v;
	struct dict_entry *next;
}dict_entry;

typedef struct dict_type{



}dict_type;

typedef struct dictht {
	dict_entry **table;
	unsigned long size;
	unsigned long sizemask;
	unsigned long used;
}dictht;

typedef struct dict {
	dict_type *type;
	void *privdata;
	dictht ht[2];
	long rehashidx;
	unsigned long iterators;
}dict;


typedef struct dict_iterator{
	dict *d;
	long index;
	int table, safe;
	dict_entry *entry, *next_entry;
	long long fingerprint;
}dict_iterator;


#endif //__DICT_H__
