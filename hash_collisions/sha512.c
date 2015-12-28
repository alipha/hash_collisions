/*
* SHA-512 hash in C
*
* Copyright (c) 2014 Project Nayuki
* http://www.nayuki.io/page/fast-sha2-hashes-in-x86-assembly
*
* (MIT License)
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
* - The above copyright notice and this permission notice shall be included in
*   all copies or substantial portions of the Software.
* - The Software is provided "as is", without warranty of any kind, express or
*   implied, including but not limited to the warranties of merchantability,
*   fitness for a particular purpose and noninfringement. In no event shall the
*   authors or copyright holders be liable for any claim, damages or other
*   liability, whether in an action of contract, tort or otherwise, arising from,
*   out of or in connection with the Software or the use or other dealings in the
*   Software.
*/

/*
#include "sha512.h"
#include <string.h>

static void sha512_compress(uint64_t state[8], const uint8_t block[128]);


void sha512_hash(const uint8_t *message, uint32_t len, uint64_t hash[8]) {
	hash[0] = UINT64_C(0x6A09E667F3BCC908);
	hash[1] = UINT64_C(0xBB67AE8584CAA73B);
	hash[2] = UINT64_C(0x3C6EF372FE94F82B);
	hash[3] = UINT64_C(0xA54FF53A5F1D36F1);
	hash[4] = UINT64_C(0x510E527FADE682D1);
	hash[5] = UINT64_C(0x9B05688C2B3E6C1F);
	hash[6] = UINT64_C(0x1F83D9ABFB41BD6B);
	hash[7] = UINT64_C(0x5BE0CD19137E2179);

	uint32_t i;
	for (i = 0; len - i >= 128; i += 128)
		sha512_compress(hash, message + i);

	uint8_t block[128];
	uint32_t rem = len - i;
	memcpy(block, message + i, rem);

	block[rem] = 0x80;
	rem++;
	if (128 - rem >= 16)
		memset(block + rem, 0, 120 - rem);
	else {
		memset(block + rem, 0, 128 - rem);
		sha512_compress(hash, block);
		memset(block, 0, 120);
	}

	uint64_t longLen = ((uint64_t)len) << 3;
	for (i = 0; i < 8; i++)
		block[128 - 1 - i] = (uint8_t)(longLen >> (i * 8));
	sha512_compress(hash, block);
}
*/

#include <stdint.h>

static void sha512_compress(uint64_t state[8], const uint8_t block[128]) {
	// 64-bit right rotation
#define ROR(x, i)  \
		(((x) << (64 - (i))) | ((x) >> (i)))

#define LOADSCHEDULE(i)  \
		schedule[i] = (uint64_t)block[i * 8 + 0] << 56  \
		            | (uint64_t)block[i * 8 + 1] << 48  \
		            | (uint64_t)block[i * 8 + 2] << 40  \
		            | (uint64_t)block[i * 8 + 3] << 32  \
		            | (uint64_t)block[i * 8 + 4] << 24  \
		            | (uint64_t)block[i * 8 + 5] << 16  \
		            | (uint64_t)block[i * 8 + 6] <<  8  \
		            | (uint64_t)block[i * 8 + 7];

#define SCHEDULE(i)  \
		schedule[i] = schedule[i - 16] + schedule[i - 7]  \
			+ (ROR(schedule[i - 15], 1) ^ ROR(schedule[i - 15], 8) ^ (schedule[i - 15] >> 7))  \
			+ (ROR(schedule[i - 2], 19) ^ ROR(schedule[i - 2], 61) ^ (schedule[i - 2] >> 6));

#define ROUND(a, b, c, d, e, f, g, h, i, k) \
		h += (ROR(e, 14) ^ ROR(e, 18) ^ ROR(e, 41)) + (g ^ (e & (f ^ g))) + UINT64_C(k) + schedule[i];  \
		d += h;  \
		h += (ROR(a, 28) ^ ROR(a, 34) ^ ROR(a, 39)) + ((a & (b | c)) | (b & c));

	uint64_t schedule[80];
	LOADSCHEDULE(0)
		LOADSCHEDULE(1)
		LOADSCHEDULE(2)
		LOADSCHEDULE(3)
		LOADSCHEDULE(4)
		LOADSCHEDULE(5)
		LOADSCHEDULE(6)
		LOADSCHEDULE(7)
		LOADSCHEDULE(8)
		LOADSCHEDULE(9)
		LOADSCHEDULE(10)
		LOADSCHEDULE(11)
		LOADSCHEDULE(12)
		LOADSCHEDULE(13)
		LOADSCHEDULE(14)
		LOADSCHEDULE(15)
		SCHEDULE(16)
		SCHEDULE(17)
		SCHEDULE(18)
		SCHEDULE(19)
		SCHEDULE(20)
		SCHEDULE(21)
		SCHEDULE(22)
		SCHEDULE(23)
		SCHEDULE(24)
		SCHEDULE(25)
		SCHEDULE(26)
		SCHEDULE(27)
		SCHEDULE(28)
		SCHEDULE(29)
		SCHEDULE(30)
		SCHEDULE(31)
		SCHEDULE(32)
		SCHEDULE(33)
		SCHEDULE(34)
		SCHEDULE(35)
		SCHEDULE(36)
		SCHEDULE(37)
		SCHEDULE(38)
		SCHEDULE(39)
		SCHEDULE(40)
		SCHEDULE(41)
		SCHEDULE(42)
		SCHEDULE(43)
		SCHEDULE(44)
		SCHEDULE(45)
		SCHEDULE(46)
		SCHEDULE(47)
		SCHEDULE(48)
		SCHEDULE(49)
		SCHEDULE(50)
		SCHEDULE(51)
		SCHEDULE(52)
		SCHEDULE(53)
		SCHEDULE(54)
		SCHEDULE(55)
		SCHEDULE(56)
		SCHEDULE(57)
		SCHEDULE(58)
		SCHEDULE(59)
		SCHEDULE(60)
		SCHEDULE(61)
		SCHEDULE(62)
		SCHEDULE(63)
		SCHEDULE(64)
		SCHEDULE(65)
		SCHEDULE(66)
		SCHEDULE(67)
		SCHEDULE(68)
		SCHEDULE(69)
		SCHEDULE(70)
		SCHEDULE(71)
		SCHEDULE(72)
		SCHEDULE(73)
		SCHEDULE(74)
		SCHEDULE(75)
		SCHEDULE(76)
		SCHEDULE(77)
		SCHEDULE(78)
		SCHEDULE(79)

		uint64_t a = state[0];
	uint64_t b = state[1];
	uint64_t c = state[2];
	uint64_t d = state[3];
	uint64_t e = state[4];
	uint64_t f = state[5];
	uint64_t g = state[6];
	uint64_t h = state[7];
	ROUND(a, b, c, d, e, f, g, h, 0, 0x428A2F98D728AE22)
		ROUND(h, a, b, c, d, e, f, g, 1, 0x7137449123EF65CD)
		ROUND(g, h, a, b, c, d, e, f, 2, 0xB5C0FBCFEC4D3B2F)
		ROUND(f, g, h, a, b, c, d, e, 3, 0xE9B5DBA58189DBBC)
		ROUND(e, f, g, h, a, b, c, d, 4, 0x3956C25BF348B538)
		ROUND(d, e, f, g, h, a, b, c, 5, 0x59F111F1B605D019)
		ROUND(c, d, e, f, g, h, a, b, 6, 0x923F82A4AF194F9B)
		ROUND(b, c, d, e, f, g, h, a, 7, 0xAB1C5ED5DA6D8118)
		ROUND(a, b, c, d, e, f, g, h, 8, 0xD807AA98A3030242)
		ROUND(h, a, b, c, d, e, f, g, 9, 0x12835B0145706FBE)
		ROUND(g, h, a, b, c, d, e, f, 10, 0x243185BE4EE4B28C)
		ROUND(f, g, h, a, b, c, d, e, 11, 0x550C7DC3D5FFB4E2)
		ROUND(e, f, g, h, a, b, c, d, 12, 0x72BE5D74F27B896F)
		ROUND(d, e, f, g, h, a, b, c, 13, 0x80DEB1FE3B1696B1)
		ROUND(c, d, e, f, g, h, a, b, 14, 0x9BDC06A725C71235)
		ROUND(b, c, d, e, f, g, h, a, 15, 0xC19BF174CF692694)
		ROUND(a, b, c, d, e, f, g, h, 16, 0xE49B69C19EF14AD2)
		ROUND(h, a, b, c, d, e, f, g, 17, 0xEFBE4786384F25E3)
		ROUND(g, h, a, b, c, d, e, f, 18, 0x0FC19DC68B8CD5B5)
		ROUND(f, g, h, a, b, c, d, e, 19, 0x240CA1CC77AC9C65)
		ROUND(e, f, g, h, a, b, c, d, 20, 0x2DE92C6F592B0275)
		ROUND(d, e, f, g, h, a, b, c, 21, 0x4A7484AA6EA6E483)
		ROUND(c, d, e, f, g, h, a, b, 22, 0x5CB0A9DCBD41FBD4)
		ROUND(b, c, d, e, f, g, h, a, 23, 0x76F988DA831153B5)
		ROUND(a, b, c, d, e, f, g, h, 24, 0x983E5152EE66DFAB)
		ROUND(h, a, b, c, d, e, f, g, 25, 0xA831C66D2DB43210)
		ROUND(g, h, a, b, c, d, e, f, 26, 0xB00327C898FB213F)
		ROUND(f, g, h, a, b, c, d, e, 27, 0xBF597FC7BEEF0EE4)
		ROUND(e, f, g, h, a, b, c, d, 28, 0xC6E00BF33DA88FC2)
		ROUND(d, e, f, g, h, a, b, c, 29, 0xD5A79147930AA725)
		ROUND(c, d, e, f, g, h, a, b, 30, 0x06CA6351E003826F)
		ROUND(b, c, d, e, f, g, h, a, 31, 0x142929670A0E6E70)
		ROUND(a, b, c, d, e, f, g, h, 32, 0x27B70A8546D22FFC)
		ROUND(h, a, b, c, d, e, f, g, 33, 0x2E1B21385C26C926)
		ROUND(g, h, a, b, c, d, e, f, 34, 0x4D2C6DFC5AC42AED)
		ROUND(f, g, h, a, b, c, d, e, 35, 0x53380D139D95B3DF)
		ROUND(e, f, g, h, a, b, c, d, 36, 0x650A73548BAF63DE)
		ROUND(d, e, f, g, h, a, b, c, 37, 0x766A0ABB3C77B2A8)
		ROUND(c, d, e, f, g, h, a, b, 38, 0x81C2C92E47EDAEE6)
		ROUND(b, c, d, e, f, g, h, a, 39, 0x92722C851482353B)
		ROUND(a, b, c, d, e, f, g, h, 40, 0xA2BFE8A14CF10364)
		ROUND(h, a, b, c, d, e, f, g, 41, 0xA81A664BBC423001)
		ROUND(g, h, a, b, c, d, e, f, 42, 0xC24B8B70D0F89791)
		ROUND(f, g, h, a, b, c, d, e, 43, 0xC76C51A30654BE30)
		ROUND(e, f, g, h, a, b, c, d, 44, 0xD192E819D6EF5218)
		ROUND(d, e, f, g, h, a, b, c, 45, 0xD69906245565A910)
		ROUND(c, d, e, f, g, h, a, b, 46, 0xF40E35855771202A)
		ROUND(b, c, d, e, f, g, h, a, 47, 0x106AA07032BBD1B8)
		ROUND(a, b, c, d, e, f, g, h, 48, 0x19A4C116B8D2D0C8)
		ROUND(h, a, b, c, d, e, f, g, 49, 0x1E376C085141AB53)
		ROUND(g, h, a, b, c, d, e, f, 50, 0x2748774CDF8EEB99)
		ROUND(f, g, h, a, b, c, d, e, 51, 0x34B0BCB5E19B48A8)
		ROUND(e, f, g, h, a, b, c, d, 52, 0x391C0CB3C5C95A63)
		ROUND(d, e, f, g, h, a, b, c, 53, 0x4ED8AA4AE3418ACB)
		ROUND(c, d, e, f, g, h, a, b, 54, 0x5B9CCA4F7763E373)
		ROUND(b, c, d, e, f, g, h, a, 55, 0x682E6FF3D6B2B8A3)
		ROUND(a, b, c, d, e, f, g, h, 56, 0x748F82EE5DEFB2FC)
		ROUND(h, a, b, c, d, e, f, g, 57, 0x78A5636F43172F60)
		ROUND(g, h, a, b, c, d, e, f, 58, 0x84C87814A1F0AB72)
		ROUND(f, g, h, a, b, c, d, e, 59, 0x8CC702081A6439EC)
		ROUND(e, f, g, h, a, b, c, d, 60, 0x90BEFFFA23631E28)
		ROUND(d, e, f, g, h, a, b, c, 61, 0xA4506CEBDE82BDE9)
		ROUND(c, d, e, f, g, h, a, b, 62, 0xBEF9A3F7B2C67915)
		ROUND(b, c, d, e, f, g, h, a, 63, 0xC67178F2E372532B)
		ROUND(a, b, c, d, e, f, g, h, 64, 0xCA273ECEEA26619C)
		ROUND(h, a, b, c, d, e, f, g, 65, 0xD186B8C721C0C207)
		ROUND(g, h, a, b, c, d, e, f, 66, 0xEADA7DD6CDE0EB1E)
		ROUND(f, g, h, a, b, c, d, e, 67, 0xF57D4F7FEE6ED178)
		ROUND(e, f, g, h, a, b, c, d, 68, 0x06F067AA72176FBA)
		ROUND(d, e, f, g, h, a, b, c, 69, 0x0A637DC5A2C898A6)
		ROUND(c, d, e, f, g, h, a, b, 70, 0x113F9804BEF90DAE)
		ROUND(b, c, d, e, f, g, h, a, 71, 0x1B710B35131C471B)
		ROUND(a, b, c, d, e, f, g, h, 72, 0x28DB77F523047D84)
		ROUND(h, a, b, c, d, e, f, g, 73, 0x32CAAB7B40C72493)
		ROUND(g, h, a, b, c, d, e, f, 74, 0x3C9EBE0A15C9BEBC)
		ROUND(f, g, h, a, b, c, d, e, 75, 0x431D67C49C100D4C)
		ROUND(e, f, g, h, a, b, c, d, 76, 0x4CC5D4BECB3E42B6)
		ROUND(d, e, f, g, h, a, b, c, 77, 0x597F299CFC657E2A)
		ROUND(c, d, e, f, g, h, a, b, 78, 0x5FCB6FAB3AD6FAEC)
		ROUND(b, c, d, e, f, g, h, a, 79, 0x6C44198C4A475817)
		state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

/**/


/*-
* Copyright 2005,2007,2009 Colin Percival
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*/



#define AS1(x) __asm {x}
#define AS2(x, y) __asm {x, y}
#define AS3(x, y, z) __asm {x, y, z}
#define ASS(x, y, a, b, c, d) __asm {x, y, (a)*64+(b)*16+(c)*4+(d)}
#define ASL(x) __asm {label##x:}
#define ASJ(x, y, z) __asm {x label##y}
#define ASC(x, y) __asm {x label##y}
#define AS_HEX(y) 0x##y
#define W64LIT(x) x##ui64


__declspec(align(16)) static const uint64_t SHA512_K[80] = {
	W64LIT(0x428a2f98d728ae22), W64LIT(0x7137449123ef65cd),
	W64LIT(0xb5c0fbcfec4d3b2f), W64LIT(0xe9b5dba58189dbbc),
	W64LIT(0x3956c25bf348b538), W64LIT(0x59f111f1b605d019),
	W64LIT(0x923f82a4af194f9b), W64LIT(0xab1c5ed5da6d8118),
	W64LIT(0xd807aa98a3030242), W64LIT(0x12835b0145706fbe),
	W64LIT(0x243185be4ee4b28c), W64LIT(0x550c7dc3d5ffb4e2),
	W64LIT(0x72be5d74f27b896f), W64LIT(0x80deb1fe3b1696b1),
	W64LIT(0x9bdc06a725c71235), W64LIT(0xc19bf174cf692694),
	W64LIT(0xe49b69c19ef14ad2), W64LIT(0xefbe4786384f25e3),
	W64LIT(0x0fc19dc68b8cd5b5), W64LIT(0x240ca1cc77ac9c65),
	W64LIT(0x2de92c6f592b0275), W64LIT(0x4a7484aa6ea6e483),
	W64LIT(0x5cb0a9dcbd41fbd4), W64LIT(0x76f988da831153b5),
	W64LIT(0x983e5152ee66dfab), W64LIT(0xa831c66d2db43210),
	W64LIT(0xb00327c898fb213f), W64LIT(0xbf597fc7beef0ee4),
	W64LIT(0xc6e00bf33da88fc2), W64LIT(0xd5a79147930aa725),
	W64LIT(0x06ca6351e003826f), W64LIT(0x142929670a0e6e70),
	W64LIT(0x27b70a8546d22ffc), W64LIT(0x2e1b21385c26c926),
	W64LIT(0x4d2c6dfc5ac42aed), W64LIT(0x53380d139d95b3df),
	W64LIT(0x650a73548baf63de), W64LIT(0x766a0abb3c77b2a8),
	W64LIT(0x81c2c92e47edaee6), W64LIT(0x92722c851482353b),
	W64LIT(0xa2bfe8a14cf10364), W64LIT(0xa81a664bbc423001),
	W64LIT(0xc24b8b70d0f89791), W64LIT(0xc76c51a30654be30),
	W64LIT(0xd192e819d6ef5218), W64LIT(0xd69906245565a910),
	W64LIT(0xf40e35855771202a), W64LIT(0x106aa07032bbd1b8),
	W64LIT(0x19a4c116b8d2d0c8), W64LIT(0x1e376c085141ab53),
	W64LIT(0x2748774cdf8eeb99), W64LIT(0x34b0bcb5e19b48a8),
	W64LIT(0x391c0cb3c5c95a63), W64LIT(0x4ed8aa4ae3418acb),
	W64LIT(0x5b9cca4f7763e373), W64LIT(0x682e6ff3d6b2b8a3),
	W64LIT(0x748f82ee5defb2fc), W64LIT(0x78a5636f43172f60),
	W64LIT(0x84c87814a1f0ab72), W64LIT(0x8cc702081a6439ec),
	W64LIT(0x90befffa23631e28), W64LIT(0xa4506cebde82bde9),
	W64LIT(0xbef9a3f7b2c67915), W64LIT(0xc67178f2e372532b),
	W64LIT(0xca273eceea26619c), W64LIT(0xd186b8c721c0c207),
	W64LIT(0xeada7dd6cde0eb1e), W64LIT(0xf57d4f7fee6ed178),
	W64LIT(0x06f067aa72176fba), W64LIT(0x0a637dc5a2c898a6),
	W64LIT(0x113f9804bef90dae), W64LIT(0x1b710b35131c471b),
	W64LIT(0x28db77f523047d84), W64LIT(0x32caab7b40c72493),
	W64LIT(0x3c9ebe0a15c9bebc), W64LIT(0x431d67c49c100d4c),
	W64LIT(0x4cc5d4becb3e42b6), W64LIT(0x597f299cfc657e2a),
	W64LIT(0x5fcb6fab3ad6faec), W64LIT(0x6c44198c4a475817)
};


__declspec(naked) static void SHA512_SSE2_Transform(uint64_t *state, const uint64_t *data)
{
	AS2(mov edx, [esp + 12])
		AS2(mov ecx, [esp + 8])
		AS1(pop  eax)
		AS2(add esp, 8)
		AS1(push eax)

		AS1(push	ebx)
		AS1(push	esi)
		AS1(push	edi)
		AS2(lea		ebx, SHA512_K)

		AS2(mov		eax, esp)
		AS2(and		esp, 0xfffffff0)
		AS2(sub		esp, 27 * 16)				// 17*16 for expanded data, 20*8 for state
		AS1(push	eax)
		AS2(xor		eax, eax)
		AS2(lea		edi, [esp + 4 + 8 * 8])		// start at middle of state buffer. will decrement pointer each round to avoid copying
		AS2(lea		esi, [esp + 4 + 20 * 8 + 8])	// 16-byte alignment, then add 8

		AS2(movdqa	xmm0, [ecx + 0 * 16])
		AS2(movdq2q	mm4, xmm0)
		AS2(movdqa[edi + 0 * 16], xmm0)
		AS2(movdqa	xmm0, [ecx + 1 * 16])
		AS2(movdqa[edi + 1 * 16], xmm0)
		AS2(movdqa	xmm0, [ecx + 2 * 16])
		AS2(movdq2q	mm5, xmm0)
		AS2(movdqa[edi + 2 * 16], xmm0)
		AS2(movdqa	xmm0, [ecx + 3 * 16])
		AS2(movdqa[edi + 3 * 16], xmm0)
		ASJ(jmp, 0, f)

#define SSE2_S0_S1(r, a, b, c)	\
	AS2(	movq	mm6, r)\
	AS2(	psrlq	r, a)\
	AS2(	movq	mm7, r)\
	AS2(	psllq	mm6, 64-c)\
	AS2(	pxor	mm7, mm6)\
	AS2(	psrlq	r, b-a)\
	AS2(	pxor	mm7, r)\
	AS2(	psllq	mm6, c-b)\
	AS2(	pxor	mm7, mm6)\
	AS2(	psrlq	r, c-b)\
	AS2(	pxor	r, mm7)\
	AS2(	psllq	mm6, b-a)\
	AS2(	pxor	r, mm6)

#define SSE2_s0(r, a, b, c)	\
	AS2(	movdqa	xmm6, r)\
	AS2(	psrlq	r, a)\
	AS2(	movdqa	xmm7, r)\
	AS2(	psllq	xmm6, 64-c)\
	AS2(	pxor	xmm7, xmm6)\
	AS2(	psrlq	r, b-a)\
	AS2(	pxor	xmm7, r)\
	AS2(	psrlq	r, c-b)\
	AS2(	pxor	r, xmm7)\
	AS2(	psllq	xmm6, c-a)\
	AS2(	pxor	r, xmm6)

#define SSE2_s1(r, a, b, c)	\
	AS2(	movdqa	xmm6, r)\
	AS2(	psrlq	r, a)\
	AS2(	movdqa	xmm7, r)\
	AS2(	psllq	xmm6, 64-c)\
	AS2(	pxor	xmm7, xmm6)\
	AS2(	psrlq	r, b-a)\
	AS2(	pxor	xmm7, r)\
	AS2(	psllq	xmm6, c-b)\
	AS2(	pxor	xmm7, xmm6)\
	AS2(	psrlq	r, c-b)\
	AS2(	pxor	r, xmm7)

		ASL(SHA512_Round)
		// k + w is in mm0, a is in mm4, e is in mm5
		AS2(paddq	mm0, [edi + 7 * 8])		// h
		AS2(movq	mm2, [edi + 5 * 8])		// f
		AS2(movq	mm3, [edi + 6 * 8])		// g
		AS2(pxor	mm2, mm3)
		AS2(pand	mm2, mm5)
		SSE2_S0_S1(mm5, 14, 18, 41)
		AS2(pxor	mm2, mm3)
		AS2(paddq	mm0, mm2)			// h += Ch(e,f,g)
		AS2(paddq	mm5, mm0)			// h += S1(e)
		AS2(movq	mm2, [edi + 1 * 8])		// b
		AS2(movq	mm1, mm2)
		AS2(por		mm2, mm4)
		AS2(pand	mm2, [edi + 2 * 8])		// c
		AS2(pand	mm1, mm4)
		AS2(por		mm1, mm2)
		AS2(paddq	mm1, mm5)			// temp = h + Maj(a,b,c)
		AS2(paddq	mm5, [edi + 3 * 8])		// e = d + h
		AS2(movq[edi + 3 * 8], mm5)
		AS2(movq[edi + 11 * 8], mm5)
		SSE2_S0_S1(mm4, 28, 34, 39)			// S0(a)
		AS2(paddq	mm4, mm1)			// a = temp + S0(a)
		AS2(movq[edi - 8], mm4)
		AS2(movq[edi + 7 * 8], mm4)
		AS1(ret)

		// first 16 rounds
		ASL(0)
		AS2(movq	mm0, [edx + eax * 8])
		AS2(movq[esi + eax * 8], mm0)
		AS2(movq[esi + eax * 8 + 16 * 8], mm0)
		AS2(paddq	mm0, [ebx + eax * 8])
		ASC(call, SHA512_Round)
		AS1(inc		eax)
		AS2(sub		edi, 8)
		AS2(test	eax, 7)
		ASJ(jnz, 0, b)
		AS2(add		edi, 8 * 8)
		AS2(cmp		eax, 16)
		ASJ(jne, 0, b)

		// rest of the rounds
		AS2(movdqu	xmm0, [esi + (16 - 2) * 8])
		ASL(1)
		// data expansion, W[i-2] already in xmm0
		AS2(movdqu	xmm3, [esi])
		AS2(paddq	xmm3, [esi + (16 - 7) * 8])
		AS2(movdqa	xmm2, [esi + (16 - 15) * 8])
		SSE2_s1(xmm0, 6, 19, 61)
		AS2(paddq	xmm0, xmm3)
		SSE2_s0(xmm2, 1, 7, 8)
		AS2(paddq	xmm0, xmm2)
		AS2(movdq2q	mm0, xmm0)
		AS2(movhlps	xmm1, xmm0)
		AS2(paddq	mm0, [ebx + eax * 8])
		AS2(movlps[esi], xmm0)
		AS2(movlps[esi + 8], xmm1)
		AS2(movlps[esi + 8 * 16], xmm0)
		AS2(movlps[esi + 8 * 17], xmm1)
		// 2 rounds
		ASC(call, SHA512_Round)
		AS2(sub		edi, 8)
		AS2(movdq2q	mm0, xmm1)
		AS2(paddq	mm0, [ebx + eax * 8 + 8])
		ASC(call, SHA512_Round)
		// update indices and loop
		AS2(add		esi, 16)
		AS2(add		eax, 2)
		AS2(sub		edi, 8)
		AS2(test	eax, 7)
		ASJ(jnz, 1, b)
		// do housekeeping every 8 rounds
		AS2(mov		esi, 0xf)
		AS2(and		esi, eax)
		AS2(lea		esi, [esp + 4 + 20 * 8 + 8 + esi * 8])
		AS2(add		edi, 8 * 8)
		AS2(cmp		eax, 80)
		ASJ(jne, 1, b)

#define SSE2_CombineState(i)	\
	AS2(	movdqa	xmm0, [edi+i*16])\
	AS2(	paddq	xmm0, [ecx+i*16])\
	AS2(	movdqa	[ecx+i*16], xmm0)

		SSE2_CombineState(0)
		SSE2_CombineState(1)
		SSE2_CombineState(2)
		SSE2_CombineState(3)

		AS1(pop		esp)
		AS1(emms)

		AS1(pop		edi)
		AS1(pop		esi)
		AS1(pop		ebx)
		AS1(pop eax)
		AS2(sub esp, 8)
		AS1(push eax)
		AS1(ret)
}











#include "sha512.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Avoid namespace collisions with BSD <sys/endian.h>. */
#define be64dec _sha512_be64dec
#define be64enc _sha512_be64enc

static uint64_t be64dec(const void *pp)
{
	const uint8_t *p = (uint8_t const *)pp;

	return ((uint64_t)(p[7]) + ((uint64_t)(p[6]) << 8) +
		((uint64_t)(p[5]) << 16) + ((uint64_t)(p[4]) << 24) +
		((uint64_t)(p[3]) << 32) + ((uint64_t)(p[2]) << 40) +
		((uint64_t)(p[1]) << 48) + ((uint64_t)(p[0]) << 56));
}

static void be64enc(void *pp, uint64_t x)
{
	uint8_t *p = (uint8_t *)pp;

	p[7] = x & 0xff;
	p[6] = (x >> 8) & 0xff;
	p[5] = (x >> 16) & 0xff;
	p[4] = (x >> 24) & 0xff;
	p[3] = (x >> 32) & 0xff;
	p[2] = (x >> 40) & 0xff;
	p[1] = (x >> 48) & 0xff;
	p[0] = (x >> 56) & 0xff;
}

static void
be64enc_vect(unsigned char *dst, const uint64_t *src, size_t len)
{
	size_t i;

	for (i = 0; i < len / 8; i++) {
		be64enc(dst + i * 8, src[i]);
	}
}

static void
be64dec_vect(uint64_t *dst, const unsigned char *src, size_t len)
{
	size_t i;

	for (i = 0; i < len / 8; i++) {
		dst[i] = be64dec(src + i * 8);
	}
}

#define Ch(x, y, z)     ((x & (y ^ z)) ^ z)
#define Maj(x, y, z)    ((x & (y | z)) | (y & z))
#define SHR(x, n)       (x >> n)
#define ROTR(x, n)      ((x >> n) | (x << (64 - n)))
#define S0(x)           (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define S1(x)           (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define s0(x)           (ROTR(x, 1) ^ ROTR(x, 8) ^ SHR(x, 7))
#define s1(x)           (ROTR(x, 19) ^ ROTR(x, 61) ^ SHR(x, 6))

#define RND(a, b, c, d, e, f, g, h, k)              \
    t0 = h + S1(e) + Ch(e, f, g) + k;               \
    t1 = S0(a) + Maj(a, b, c);                      \
    d += t0;                                        \
    h  = t0 + t1;

#define RNDr(S, W, i, k)                    \
    RND(S[(80 - i) % 8], S[(81 - i) % 8],   \
        S[(82 - i) % 8], S[(83 - i) % 8],   \
        S[(84 - i) % 8], S[(85 - i) % 8],   \
        S[(86 - i) % 8], S[(87 - i) % 8],   \
        W[i] + k)

static void
SHA512_Transform(uint64_t *state, const unsigned char block[128])
{
	uint64_t W[80];
	uint64_t S[8];
	uint64_t t0, t1;
	int i;

	be64dec_vect(W, block, 128);
	for (i = 16; i < 80; i++) {
		W[i] = s1(W[i - 2]) + W[i - 7] + s0(W[i - 15]) + W[i - 16];
	}

	memcpy(S, state, 64);

	RNDr(S, W, 0, 0x428a2f98d728ae22ULL);
	RNDr(S, W, 1, 0x7137449123ef65cdULL);
	RNDr(S, W, 2, 0xb5c0fbcfec4d3b2fULL);
	RNDr(S, W, 3, 0xe9b5dba58189dbbcULL);
	RNDr(S, W, 4, 0x3956c25bf348b538ULL);
	RNDr(S, W, 5, 0x59f111f1b605d019ULL);
	RNDr(S, W, 6, 0x923f82a4af194f9bULL);
	RNDr(S, W, 7, 0xab1c5ed5da6d8118ULL);
	RNDr(S, W, 8, 0xd807aa98a3030242ULL);
	RNDr(S, W, 9, 0x12835b0145706fbeULL);
	RNDr(S, W, 10, 0x243185be4ee4b28cULL);
	RNDr(S, W, 11, 0x550c7dc3d5ffb4e2ULL);
	RNDr(S, W, 12, 0x72be5d74f27b896fULL);
	RNDr(S, W, 13, 0x80deb1fe3b1696b1ULL);
	RNDr(S, W, 14, 0x9bdc06a725c71235ULL);
	RNDr(S, W, 15, 0xc19bf174cf692694ULL);
	RNDr(S, W, 16, 0xe49b69c19ef14ad2ULL);
	RNDr(S, W, 17, 0xefbe4786384f25e3ULL);
	RNDr(S, W, 18, 0x0fc19dc68b8cd5b5ULL);
	RNDr(S, W, 19, 0x240ca1cc77ac9c65ULL);
	RNDr(S, W, 20, 0x2de92c6f592b0275ULL);
	RNDr(S, W, 21, 0x4a7484aa6ea6e483ULL);
	RNDr(S, W, 22, 0x5cb0a9dcbd41fbd4ULL);
	RNDr(S, W, 23, 0x76f988da831153b5ULL);
	RNDr(S, W, 24, 0x983e5152ee66dfabULL);
	RNDr(S, W, 25, 0xa831c66d2db43210ULL);
	RNDr(S, W, 26, 0xb00327c898fb213fULL);
	RNDr(S, W, 27, 0xbf597fc7beef0ee4ULL);
	RNDr(S, W, 28, 0xc6e00bf33da88fc2ULL);
	RNDr(S, W, 29, 0xd5a79147930aa725ULL);
	RNDr(S, W, 30, 0x06ca6351e003826fULL);
	RNDr(S, W, 31, 0x142929670a0e6e70ULL);
	RNDr(S, W, 32, 0x27b70a8546d22ffcULL);
	RNDr(S, W, 33, 0x2e1b21385c26c926ULL);
	RNDr(S, W, 34, 0x4d2c6dfc5ac42aedULL);
	RNDr(S, W, 35, 0x53380d139d95b3dfULL);
	RNDr(S, W, 36, 0x650a73548baf63deULL);
	RNDr(S, W, 37, 0x766a0abb3c77b2a8ULL);
	RNDr(S, W, 38, 0x81c2c92e47edaee6ULL);
	RNDr(S, W, 39, 0x92722c851482353bULL);
	RNDr(S, W, 40, 0xa2bfe8a14cf10364ULL);
	RNDr(S, W, 41, 0xa81a664bbc423001ULL);
	RNDr(S, W, 42, 0xc24b8b70d0f89791ULL);
	RNDr(S, W, 43, 0xc76c51a30654be30ULL);
	RNDr(S, W, 44, 0xd192e819d6ef5218ULL);
	RNDr(S, W, 45, 0xd69906245565a910ULL);
	RNDr(S, W, 46, 0xf40e35855771202aULL);
	RNDr(S, W, 47, 0x106aa07032bbd1b8ULL);
	RNDr(S, W, 48, 0x19a4c116b8d2d0c8ULL);
	RNDr(S, W, 49, 0x1e376c085141ab53ULL);
	RNDr(S, W, 50, 0x2748774cdf8eeb99ULL);
	RNDr(S, W, 51, 0x34b0bcb5e19b48a8ULL);
	RNDr(S, W, 52, 0x391c0cb3c5c95a63ULL);
	RNDr(S, W, 53, 0x4ed8aa4ae3418acbULL);
	RNDr(S, W, 54, 0x5b9cca4f7763e373ULL);
	RNDr(S, W, 55, 0x682e6ff3d6b2b8a3ULL);
	RNDr(S, W, 56, 0x748f82ee5defb2fcULL);
	RNDr(S, W, 57, 0x78a5636f43172f60ULL);
	RNDr(S, W, 58, 0x84c87814a1f0ab72ULL);
	RNDr(S, W, 59, 0x8cc702081a6439ecULL);
	RNDr(S, W, 60, 0x90befffa23631e28ULL);
	RNDr(S, W, 61, 0xa4506cebde82bde9ULL);
	RNDr(S, W, 62, 0xbef9a3f7b2c67915ULL);
	RNDr(S, W, 63, 0xc67178f2e372532bULL);
	RNDr(S, W, 64, 0xca273eceea26619cULL);
	RNDr(S, W, 65, 0xd186b8c721c0c207ULL);
	RNDr(S, W, 66, 0xeada7dd6cde0eb1eULL);
	RNDr(S, W, 67, 0xf57d4f7fee6ed178ULL);
	RNDr(S, W, 68, 0x06f067aa72176fbaULL);
	RNDr(S, W, 69, 0x0a637dc5a2c898a6ULL);
	RNDr(S, W, 70, 0x113f9804bef90daeULL);
	RNDr(S, W, 71, 0x1b710b35131c471bULL);
	RNDr(S, W, 72, 0x28db77f523047d84ULL);
	RNDr(S, W, 73, 0x32caab7b40c72493ULL);
	RNDr(S, W, 74, 0x3c9ebe0a15c9bebcULL);
	RNDr(S, W, 75, 0x431d67c49c100d4cULL);
	RNDr(S, W, 76, 0x4cc5d4becb3e42b6ULL);
	RNDr(S, W, 77, 0x597f299cfc657e2aULL);
	RNDr(S, W, 78, 0x5fcb6fab3ad6faecULL);
	RNDr(S, W, 79, 0x6c44198c4a475817ULL);

	for (i = 0; i < 8; i++) {
		state[i] += S[i];
	}

	memset(W, 0, sizeof W);
	memset(S, 0, sizeof S);
	memset(&t0, 0, sizeof t0);
	memset(&t1, 0, sizeof t1);
}

static unsigned char PAD[128] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void
SHA512_Pad(crypto_hash_sha512_state *state)
{
	unsigned char len[16];
	uint64_t r, plen;

	be64enc_vect(len, state->count, 16);

	r = (state->count[1] >> 3) & 0x7f;
	plen = (r < 112) ? (112 - r) : (240 - r);
	crypto_hash_sha512_update(state, PAD, (unsigned long long) plen);

	crypto_hash_sha512_update(state, len, 16);
}

int
crypto_hash_sha512_init(crypto_hash_sha512_state *state)
{
	static const uint64_t sha512_initstate[8] = {
		0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
		0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
		0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
		0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
	};

	state->count[0] = state->count[1] = (uint64_t)0U;
	memcpy(state->state, sha512_initstate, sizeof sha512_initstate);

	return 0;
}

int
crypto_hash_sha512_update(crypto_hash_sha512_state *state,
const unsigned char *in,
unsigned long long inlen)
{
	uint64_t bitlen[2];
	uint64_t r;
	const unsigned char *src = in;

	r = (state->count[1] >> 3) & 0x7f;

	bitlen[1] = ((uint64_t)inlen) << 3;
	bitlen[0] = ((uint64_t)inlen) >> 61;

	/* LCOV_EXCL_START */
	if ((state->count[1] += bitlen[1]) < bitlen[1]) {
		state->count[0]++;
	}
	/* LCOV_EXCL_STOP */
	state->count[0] += bitlen[0];

	if (inlen < 128 - r) {
		memcpy(&state->buf[r], src, inlen);
		return 0;
	}
	memcpy(&state->buf[r], src, 128 - r);
	//sha512_compress(state->state, state->buf);
	SHA512_SSE2_Transform(state->state, state->buf);
	src += 128 - r;
	inlen -= 128 - r;

	while (inlen >= 128) {
		//sha512_compress(state->state, src);
		SHA512_SSE2_Transform(state->state, src);
		src += 128;
		inlen -= 128;
	}
	memcpy(state->buf, src, inlen);

	return 0;
}

int
crypto_hash_sha512_final(crypto_hash_sha512_state *state,
unsigned char *out)
{
	SHA512_Pad(state);
	be64enc_vect(out, state->state, 64);
	memset(state, 0, sizeof *state);

	return 0;
}

int
crypto_hash_sha512(unsigned char *out, const unsigned char *in,
unsigned long long inlen)
{
	crypto_hash_sha512_state state;

	crypto_hash_sha512_init(&state);
	crypto_hash_sha512_update(&state, in, inlen);
	crypto_hash_sha512_final(&state, out);

	return 0;
}
