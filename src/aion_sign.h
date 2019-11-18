#ifndef BLOCKCHAINSUITE_AION_SIGN_H
#define BLOCKCHAINSUITE_AION_SIGN_H

#include <string>
#include <vector>

#define SIGNATURE_SIZE      64
#define PUBLIC_KEY_SIZE     32
#define PRIVATE_KEY_SIZE    32
#define SEED_SIZE           32

typedef struct _AionSignature {
    unsigned char data[SIGNATURE_SIZE];
} AionSignature;

AionSignature aion_sign(const unsigned char sk[PUBLIC_KEY_SIZE+PRIVATE_KEY_SIZE], const unsigned char *message, unsigned long size);

#endif //BLOCKCHAINSUITE_AION_SIGN_H
