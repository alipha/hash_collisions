#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

#define ELEMENT_EMPTY ((uint32_t)-2)
#define NO_NEXT_ELEMENT ((uint32_t)-1)

#define ELEMENT_SIZE(data_size) ((data_size) + 4)
#define ELEMENT_PTR(table_ptr, index, element_size) ((hash_element*)((table_ptr)->elements + (int64_t)(index) * (element_size)))


hash_table *hash_table_create(uint32_t buckets, uint32_t max_size, uint32_t data_size)
{
	const uint32_t element_size = ELEMENT_SIZE(data_size);

	hash_table *table = malloc(5 * sizeof(uint32_t) + (uint64_t)max_size * element_size);
	table->buckets = buckets;
	table->data_size = data_size;
	table->max_size = max_size;

	hash_table_clear(table);

	return table;
}


void hash_table_free(hash_table *table)
{
	free(table);
}


hash_add_result hash_table_add(hash_table *table, uint32_t bucket, void *data)
{
	const uint32_t data_size = table->data_size;
	const uint32_t element_size = ELEMENT_SIZE(table->data_size);
	const uint32_t next_available_index = table->size;

	uint32_t next_index;

	hash_element *element = ELEMENT_PTR(table, bucket, element_size);
	next_index = element->next_index;

	if (next_index == ELEMENT_EMPTY)
	{
		table->buckets_used++;
		element->next_index = NO_NEXT_ELEMENT;
		memcpy(element->data, data, data_size);
		return HASH_ADD_SUCCESS;
	}

	while (memcmp(element->data, data, data_size) != 0)
	{
		if (next_index == NO_NEXT_ELEMENT)
		{
			if (next_available_index == table->max_size)
				return HASH_ADD_FULL;

			element->next_index = table->size++;

			element = ELEMENT_PTR(table, next_available_index, element_size);
			element->next_index = NO_NEXT_ELEMENT;
			memcpy(element->data, data, data_size);
			return HASH_ADD_SUCCESS;
		}

		element = ELEMENT_PTR(table, next_index, element_size);
		next_index = element->next_index;
	}

	return HASH_ADD_EXISTS;
}


void hash_table_clear(hash_table *table)
{
	const uint32_t element_size = ELEMENT_SIZE(table->data_size);
	const uint32_t buckets = table->buckets;

	uint32_t i;

	table->buckets_used = 0;
	table->size = buckets;

	for (i = 0; i < buckets; i++)
		ELEMENT_PTR(table, i, element_size)->next_index = ELEMENT_EMPTY;
}


hash_iterator hash_table_iterator(hash_table *table)
{
	const uint32_t element_size = ELEMENT_SIZE(table->data_size);

	hash_iterator it = { table, 0, 0 };
	hash_element *element = ELEMENT_PTR(table, 0, element_size);

	if (element->next_index == ELEMENT_EMPTY)
		it = hash_iterator_next(it);

	return it;
}


void *hash_iterator_get(hash_iterator it)
{
	hash_table *table = it.table;

	if (it.bucket >= table->buckets)
		return NULL;

	const uint32_t element_size = ELEMENT_SIZE(table->data_size);
	hash_element *element = ELEMENT_PTR(table, it.element_index, element_size);

	if (element->next_index == ELEMENT_EMPTY)
		return NULL;

	return element->data;
}


hash_iterator hash_iterator_next(hash_iterator it)
{
	hash_table *table = it.table;
	const uint32_t buckets = table->buckets;

	if (it.bucket >= buckets)
		return it;

	const uint32_t element_size = ELEMENT_SIZE(table->data_size);
	hash_element *element = ELEMENT_PTR(table, it.element_index, element_size);

	if (element->next_index != NO_NEXT_ELEMENT && element->next_index != ELEMENT_EMPTY)
	{
		it.element_index = element->next_index;
		return it;
	}

	it.bucket++;

	while (it.bucket < buckets && ELEMENT_PTR(table, it.bucket, element_size)->next_index == ELEMENT_EMPTY)
		it.bucket++;

	it.element_index = it.bucket;
	return it;
}
