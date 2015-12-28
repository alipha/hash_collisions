#ifndef SHA512_H
#define SHA512_H

#include <stdint.h>

void sha512_hash(const uint8_t *message, uint32_t len, uint8_t hash[64]);

#endif