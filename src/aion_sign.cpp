#include <vector>
#include <cstring>
#include <iostream>

#include "ed25519.h"
#include "aion_sign.h"
#include "assert.h"
#include "byte_util.h"

using namespace std;

AionSignature aion_sign(const unsigned char sk[SEED_SIZE+PUBLIC_KEY_SIZE], const unsigned char *message, unsigned long length) {
    unsigned char seed[SEED_SIZE];
    unsigned char public_key[PUBLIC_KEY_SIZE], private_key[PRIVATE_KEY_SIZE];

    memcpy((void *)seed, sk, SEED_SIZE);
    ed25519_create_keypair(public_key, private_key, seed);

    assert(to_hex_string(public_key, PUBLIC_KEY_SIZE) == to_hex_string(sk+SEED_SIZE, PUBLIC_KEY_SIZE));
    memcpy((void *)public_key, sk+SEED_SIZE, PUBLIC_KEY_SIZE);

    unsigned char signature[SIGNATURE_SIZE+1] = {0};
    ed25519_sign(signature, message, length, public_key, private_key);

    AionSignature result = {0};
    memcpy(result.data, signature, SIGNATURE_SIZE);

    return result;
}

