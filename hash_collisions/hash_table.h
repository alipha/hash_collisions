#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct hash_element 
{
	uint32_t next_index;
	char data[1];
} hash_element;

typedef struct hash_table
{
	uint32_t buckets;
	uint32_t buckets_used;
	uint32_t size;
	uint32_t max_size;
	uint32_t data_size;
	char elements[1];
} hash_table;

#pragma pack(pop)

typedef struct hash_iterator
{
	hash_table *table;
	uint32_t bucket;
	uint32_t element_index;
} hash_iterator;

typedef enum hash_add_result
{
	HASH_ADD_SUCCESS = 0,
	HASH_ADD_EXISTS = 1,
	HASH_ADD_FULL = 2
} hash_add_result;


hash_table *hash_table_create(uint32_t buckets, uint32_t max_size, uint32_t data_size);

void hash_table_free(hash_table *table);

hash_add_result hash_table_add(hash_table *table, uint32_t bucket, void *data);

void hash_table_clear(hash_table *table);

hash_iterator hash_table_iterator(hash_table *table);

void *hash_iterator_get(hash_iterator it);

hash_iterator hash_iterator_next(hash_iterator it);

#endif
