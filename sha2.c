#include "sha2.h"

uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t init_h[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

uint32_t padded_len(uint32_t len) {
    //we need 1 byte for the extra bit and 8 bytes for L
    return ((len + 9 + 63) / 64) * 64;
}

void pad(uint8_t *src, uint32_t len, uint8_t *dest){
    uint32_t target_len = padded_len(len);
    
    for(int i = 0; i < target_len; i++){
        dest[i] = 0;
    }

    for(int i = 0; i < len; i++){
        dest[i] = src[i];
    }

    dest[len] = 0x80; //append a single '1' bit

    uint64_t L = len * 8; //length in bits
    for(int i = 0; i < 8; i++){
        dest[target_len - 1 - i] = (L >> (i * 8)) & 0xff;
    }
}

uint32_t rotr(uint32_t n, uint8_t d) {
    return (n >> d) | (n << (32 - d)); 
}

void sha2(uint8_t *padded_src, uint32_t num_chunks, uint32_t dest[8]) {
    uint32_t ha[8];
    for(int i = 0; i < 8; i++){
        ha[i] = init_h[i];
    }

    for(int i = 0; i < num_chunks; i++){
        uint32_t w[64];
        uint8_t *wb = (uint8_t *)w;
        int start = i * 64;
        for(int j = 0; j < 64; j++){
            int dest = (j / 4) * 4 + (3 - (j % 4));
            wb[dest] = padded_src[start + j];
        }

        for(int i = 16; i < 64; i++){
            uint32_t s0 = rotr(w[i-15], 7) ^ rotr(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32_t s1 = rotr(w[i-2], 17) ^ rotr(w[i-2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

        uint32_t a = ha[0];
        uint32_t b = ha[1];
        uint32_t c = ha[2];
        uint32_t d = ha[3];
        uint32_t e = ha[4];
        uint32_t f = ha[5];
        uint32_t g = ha[6];
        uint32_t h = ha[7];

        for(int i = 0; i < 64; i++){
            uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + k[i] + w[i];
            uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        ha[0] += a;
        ha[1] += b;
        ha[2] += c;
        ha[3] += d;
        ha[4] += e;
        ha[5] += f;
        ha[6] += g;
        ha[7] += h;
    }

    for(int i = 0; i < 8; i++){
        dest[i] = ha[i];
    }
}