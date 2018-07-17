// Copyright (c) 2018 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#define SHA256_HASH_SIZE (512/8)
// buffer must include space for padding (at least 9 bytes)
// buffer must be a multiple 64 bytes (512 bits)
#define sha256_size(x) (((x)+9+63)&(~0x3f))

void sha256 (uint8_t *out, uint8_t *buffer, int length);

#ifndef HEADER_ONLY

#include <string.h>

#define S(x, n) ((x << (32-n)) | (x >> n))
#define Ch(x, y, z) (z ^ (x & (y ^ z)))
#define Maj(x, y, z) (((x | y) & z) | (x & y))
#define Sig_0(x) (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sig_1(x) (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define sig_0(x) (S(x, 7) ^ S(x, 18) ^ (x >> 3))
#define sig_1(x) (S(x, 17) ^ S(x, 19) ^ (x >> 10))
#define compress(a,b,c,d,e,f,g,h,i) \
  T0 = h + Sig_1(e) + Ch(e, f, g) + K[i] + W[i]; \
  T1 = Sig_0(a) + Maj(a, b, c);	\
  d += T0; h = T0 + T1

#define SHA256_HASH_SIZE (512/8)
// buffer must include space for padding (at least 9 bytes)
// buffer must be a multiple 64 bytes (512 bits)
#define sha256_size(x) (((x)+9+63)&(~0x3f))

void sha256 (uint8_t *out, uint8_t *buffer, int length) {
  int pad = 55 - (length % 64);
  int i, n = (length+9+63) >> 6;
  uint64_t l = length * 8;
  uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		   0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
  const uint32_t K[64] = 
    {0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1,
     0x923F82A4, 0xAB1C5ED5, 0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
     0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 0xE49B69C1, 0xEFBE4786,
     0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA, 
     0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147,
     0x06CA6351, 0x14292967, 0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
     0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 0xA2BFE8A1, 0xA81A664B,
     0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
     0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A,
     0x5B9CCA4F, 0x682E6FF3, 0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
     0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};
  uint32_t h[8], W[64], T0, T1; uint8_t *m = buffer;
  buffer[length] = 0x80; buffer += length + 1;
  memset (buffer, 0, pad); buffer += pad;
  PACK64 (buffer, l);
  while (n--) {
    memcpy (h, H, sizeof (H));
    // compute W[0..63]
    for (i = 0; i < 16; i++) { W[i] = UNPACK32 (m); m += 4; }
    while (i < 64) {
      W[i] = sig_1(W[i-2]) + W[i-7] + sig_0(W[i-15]) + W[i-16]; i++;
    }
    // apply compress function (loop unrolled)
    for (i = 0; i < 64; i += 8) {
      compress (h[0],h[1],h[2],h[3],h[4],h[5],h[6],h[7],i+0);
      compress (h[7],h[0],h[1],h[2],h[3],h[4],h[5],h[6],i+1);
      compress (h[6],h[7],h[0],h[1],h[2],h[3],h[4],h[5],i+2);
      compress (h[5],h[6],h[7],h[0],h[1],h[2],h[3],h[4],i+3);
      compress (h[4],h[5],h[6],h[7],h[0],h[1],h[2],h[3],i+4);
      compress (h[3],h[4],h[5],h[6],h[7],h[0],h[1],h[2],i+5);
      compress (h[2],h[3],h[4],h[5],h[6],h[7],h[0],h[1],i+6);
      compress (h[1],h[2],h[3],h[4],h[5],h[6],h[7],h[0],i+7);
    }
    for (i = 0; i < 8; i++) H[i] += h[i];
  }
  for (i = 0; i < 8; i++) { PACK32 (out, H[i]); out += 4; }
}

#endif
