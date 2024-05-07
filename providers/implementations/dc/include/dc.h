#ifndef DC_H
#define DC_H

#include <stdint.h>

#define BLOCK_SIZE 16 

typedef struct {
    uint8_t key[BLOCK_SIZE];
} DC_CIPHER_CTX;


extern uint8_t sbox[256];
extern uint8_t rsbox[256];
// Functions
void encrypt_block(const uint8_t *plaintext, uint8_t *ciphertext, const uint8_t *key);
void decrypt_block(const uint8_t *ciphertext, uint8_t *plaintext, const uint8_t *key);
void verify_sbox();

#endif // DC_H