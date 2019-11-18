#ifndef BLOCKCHAINSUITE_RPC_H
#define BLOCKCHAINSUITE_RPC_H

#include "block.h"

int get_blocknumber();
Block get_block(int number);
void parse_response();

struct RpcResult {
    int seal_type;
    bool status;
    std::string public_key;
};

#endif //BLOCKCHAINSUITE_RPC_H
