#include "hash_table.h"
#include "sha512.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define snprintf _snprintf

#define BITS(x) ((uint64_t)1 << (x))
#define BUCKETS BITS(27)
#define MAX_SIZE BITS(29)
#define DATA_SIZE 6
#define HASH_SIZE 64

char printbuf[1024];
FILE *logfile;


void check_partial_match(uint32_t plaintext, uint32_t iteration, uint8_t *match_hash);
char *print_hex(char *output, uint8_t *input, size_t len);


uint8_t test_hash[HASH_SIZE];


void hashlog() {
	puts(printbuf);
	fputs(printbuf, logfile);
	fflush(logfile);
}


int main()
{
	logfile = fopen("hashlog.txt", "a");

	clock_t start = clock();

	hash_table *table = hash_table_create(BUCKETS, MAX_SIZE, DATA_SIZE);
	uint8_t hash_a[HASH_SIZE];
	uint8_t hash_b[HASH_SIZE];

	for (uint32_t plaintext = 0;; plaintext++) {
		uint8_t *hash = hash_a;
		uint8_t *other_hash = hash_b;

		sha512_hash((uint8_t*)&plaintext, sizeof plaintext, hash);

		for (uint32_t iteration = 0;; iteration++) {
			uint8_t *temp;
			uint32_t bucket = *(uint32_t*)hash & (BUCKETS - 1);
			hash_add_result result = hash_table_add(table, bucket, hash + sizeof(uint32_t));

			if (result == HASH_ADD_EXISTS) {
				check_partial_match(plaintext, iteration, hash);
			}
			else if (result == HASH_ADD_FULL) {
				snprintf(printbuf, sizeof printbuf, "Performed %d iterations of hash %d after %.2f minutes.\n", iteration, plaintext, (clock() - start) / 60.0 / CLOCKS_PER_SEC);
				hashlog();
				break;
			}

			sha512_hash(hash, HASH_SIZE, other_hash);

			/*
			if (plaintext == 2 && iteration == 100000) {
			memcpy(test_hash, hash, HASH_SIZE);
			test_hash[30] = 3;
			}

			if (plaintext != 2 || iteration != 200000) {
			sha512_hash(hash, HASH_SIZE, other_hash);
			} else {
			memcpy(other_hash, test_hash, HASH_SIZE);
			}
			*/

			temp = other_hash;
			other_hash = hash;
			hash = temp;
		}

		hash_table_clear(table);
	}

	return 0;
}


void check_partial_match(uint32_t plaintext, uint32_t iteration, uint8_t *match_hash) {
	snprintf(printbuf, sizeof printbuf, "Checking partial match found after %d iterations of hash %d.\n", iteration, plaintext);
	hashlog();

	uint32_t match_bucket = *(uint32_t*)match_hash & (BUCKETS - 1);
	char hex[HASH_SIZE * 2 + 1];
	char hex2[HASH_SIZE * 2 + 1];
	uint8_t hash_a[HASH_SIZE];
	uint8_t hash_b[HASH_SIZE];
	uint8_t *hash = hash_a;
	uint8_t *other_hash = hash_b;

	sha512_hash((uint8_t*)&plaintext, sizeof plaintext, hash);

	for (uint32_t iteration = 0; iteration < MAX_SIZE; iteration++) {
		uint8_t *temp;
		uint32_t bucket = *(uint32_t*)hash & (BUCKETS - 1);

		if (memcmp(hash, match_hash, HASH_SIZE) == 0) {
			snprintf(printbuf, sizeof printbuf, "Full hash collision found! %s.\n", print_hex(hex, hash, HASH_SIZE));
			hashlog();
			getchar();
			exit(0);
		}

		if (bucket == match_bucket && memcmp(hash + sizeof(uint32_t), match_hash + sizeof(uint32_t), DATA_SIZE) == 0) {
			snprintf(printbuf, sizeof printbuf, "False positive: %s != %s\n", print_hex(hex, hash, HASH_SIZE), print_hex(hex2, match_hash, HASH_SIZE));
			hashlog();
		}

		sha512_hash(hash, HASH_SIZE, other_hash);
		temp = other_hash;
		other_hash = hash;
		hash = temp;
	}
}


char *print_hex(char *output, uint8_t *input, size_t len) {
	char *p = output;

	for (size_t i = 0; i < len; i++, p += 2)
		snprintf(p, 3, "%02x", input[i]);

	return output;
}


int test() {
	/*
	*  Test hash_table
	*/

	/*
	int bucketValues[] = { 2, 0, 3, 4, 0 };
	int elementValues[] = { 1, 2, 3, 4 };
	int32_t data;

	hash_table *table = hash_table_create(6, 15, 4);

	for (int i = 0; i < 18; i++)
	{
	data = elementValues[i % 4];
	int result = (int)hash_table_add(table, bucketValues[i % 5], &data);
	printf("%d %d %d %d %d %d\n", result, table->buckets, table->buckets_used, table->size, table->max_size, table->data_size);
	}

	hash_iterator it = hash_table_iterator(table);
	void *dataptr;

	while (dataptr = hash_iterator_get(it)) {
	data = *(int32_t*)dataptr;
	printf("%d:%d %d\n", it.bucket, data, it.element_index);
	it = hash_iterator_next(it);
	}

	hash_table_clear(table);
	printf("%d %d %d %d %d\n", table->buckets, table->buckets_used, table->size, table->max_size, table->data_size);

	while (dataptr = hash_iterator_get(it)) {
	data = *(int32_t*)dataptr;
	printf("%d:%d %d\n", it.bucket, data, it.element_index);
	}

	hash_table_free(table);
	getchar();
	return 0;
	*/

	/*
	*  Hash timing
	*/

	clock_t start = clock();

	uint8_t hash[64];

	/*/
	char message[] = "hello";
	sha512_hash(message, 5, hash);

	for (int x = 0; x < 64; x++)
	printf("%02x", hash[x]);

	getchar();
	return 0;
	/**/

	for (uint32_t i = 0; i < (1 << 24); i++)
		sha512_hash((uint8_t*)&i, sizeof(uint32_t), hash);

	clock_t end = clock();
	printf("%f\n", (double)(end - start) / CLOCKS_PER_SEC);

	getchar();
	return 0;
}

