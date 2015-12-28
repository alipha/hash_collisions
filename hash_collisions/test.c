#include "hash_table.h"
#include "sha512.h"
#include <stdio.h>
#include <time.h>

#define BITS(x) ((uint64_t)1 << (x))
#define BUCKETS BITS(26)
#define MAX_SIZE BITS(29)

int main()
{
	//hash_table *table = hash_table_create(BUCKETS, MAX_SIZE, 6);

	clock_t start = clock();

	uint8_t hash[64];

	/**/
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

