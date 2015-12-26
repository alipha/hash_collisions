#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#pragma pack(push, 1)

typedef struct hash_element 
{
	unsigned __int32 next_index;
	char data[1];
} hash_element;

typedef struct hash_table
{
	unsigned __int32 buckets;
	unsigned __int32 buckets_used;
	unsigned __int32 size;
	unsigned __int32 max_size;
	unsigned __int32 data_size;
	char elements[1];
} hash_table;

#pragma pack(pop)

typedef struct hash_iterator
{
	hash_table *table;
	unsigned __int32 bucket;
	unsigned __int32 element_index;
} hash_iterator;

typedef enum hash_add_result
{
	HASH_ADD_SUCCESS = 0,
	HASH_ADD_EXISTS = 1,
	HASH_ADD_FULL = 2
} hash_add_result;


hash_table *hash_table_create(unsigned __int32 buckets, unsigned __int32 max_size, unsigned __int32 data_size);

void hash_table_free(hash_table *table);

hash_add_result hash_table_add(hash_table *table, unsigned __int32 bucket, void *data);

void hash_table_clear(hash_table *table);

hash_iterator hash_table_iterator(hash_table *table);

void *hash_iterator_get(hash_iterator it);

hash_iterator hash_iterator_next(hash_iterator it);

#endif
