#include "sha2.h"
#include <stdio.h>
#include <stdlib.h>

#define len 134
int main() {
    uint8_t src[len] = "SHA-2 (Secure Hash Algorithm 2) is a set of cryptographic hash functions designed by the United States National Security Agency (NSA).";
    uint32_t plen = padded_len(len);
    uint8_t *padded = (uint8_t *)malloc(plen);
    uint32_t dest[8];
    
    pad(src, len, padded);
    for(int i = 0; i < 64; i++){
        printf("%02x ", padded[i]);
    }
    sha2(padded, plen / 64, dest);

    for(int i = 0; i < 8; i++){
        printf("%08x", dest[i]);
    }
}