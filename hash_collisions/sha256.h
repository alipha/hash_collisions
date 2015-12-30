#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>

void sha256_hash(const uint8_t *message, uint32_t len, uint8_t hash[32]);

#endif