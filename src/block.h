#ifndef BLOCKCHAINSUITE_BLOCK_H
#define BLOCKCHAINSUITE_BLOCK_H

#include <iostream>

struct Block {
    long number;
    std::string seal_type;
    std::string public_key;
};

#endif //BLOCKCHAINSUITE_BLOCK_H
