#ifndef SHA2_H
#define SHA2_H
#include <inttypes.h>

extern uint32_t k[64];

extern uint32_t init_h[8];

//calculates length after padding
uint32_t padded_len(uint32_t len);
//len is in bytes
//dest must be of size padded_len(len);
void pad(uint8_t *src, uint32_t len, uint8_t *dest);

void sha2(uint8_t *padded_src, uint32_t num_chunks, uint32_t dest[8]);

#endif