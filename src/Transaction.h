#ifndef BLOCKCHAINSUITE_TRANSACTION_H
#define BLOCKCHAINSUITE_TRANSACTION_H

#include <RLP.h>

using namespace eth;

class Transaction {
public:
    Transaction(
            u256 nonce,
            u256 price,
            long gas,
            u256 value,
            bytes data,
            u256 tx_type,
            bool is_create = false,
            u256 addr = 0
    ) : nonce(nonce),
        gas_price(price),
        gas(gas),
        value(value),
        data(data),
        tx_type(tx_type),
        is_create(is_create),
        target_addr(addr) {}

    virtual bytes sign(const bytes key) const;
    bytes rlp_unverified() const;
    bytes hash() const;

protected:
    u256 nonce, gas_price, value, tx_type, target_addr;
    long gas;
    bytes data;
    bool is_create;
    bytes signature;
};

class InvokableTransaction: public Transaction {
public:
    InvokableTransaction(
            bytes data,
            bytes executor,
            u256 to,
            long gas,
            u256 nonce = 0,
            u256 value = 0,
            u256 price = 0,
            u256 tx_type = 1
    ) : Transaction(nonce,price,gas,value,data,tx_type,false,to),
        executor(executor) {}

    bytes sign(const bytes sk) const;
    void set_seed(bytes seed);
    friend std::ostream &operator<<(std::ostream &__os, InvokableTransaction const &tx) {
        __os << "TODO: implement operator <<";

        return __os;
    }
private:
    bytes executor;
    bytes seed;
};

#endif //BLOCKCHAINSUITE_INVOKABLETRANSACTION_H
