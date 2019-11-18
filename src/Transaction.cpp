#include "Transaction.h"
#include "blake2.h"
#include "aion_sign.h"

#include <chrono>

using namespace std::chrono;

bytes InvokableTransaction::sign(const bytes private_key) const {
    // Item 1: nonce
    RLPStream s;
    s.append(nonce);
    bytes nonce = s.out();

    // Item 2: target address
    s.clear();
    s.append(target_addr);
    bytes to = s.out();

    // Item 3: value
    s.clear();
    s.append(value);
    bytes value = s.out();

    // Item 4: data
    s.clear();
    s.append(data);
    bytes data = s.out();

    // Item 5: executor
    s.clear();
    s.append(executor);
    bytes executor = s.out();

    s.clear();
    bytes all_in_one;
    all_in_one.insert(all_in_one.end(), nonce.begin(), nonce.end());
    all_in_one.insert(all_in_one.end(), to.begin(), to.end());
    all_in_one.insert(all_in_one.end(), value.begin(), value.end());
    all_in_one.insert(all_in_one.end(), data.begin(), data.end());
    all_in_one.insert(all_in_one.end(), executor.begin(), executor.end());

    bytes full_encoding = {0x00};
    bytes rlp_out = s.appendList(all_in_one).out();
    full_encoding.insert(full_encoding.end(), rlp_out.begin(), rlp_out.end());

    CryptoPP::BLAKE2b blake2b(32U);
    blake2b.Update(&full_encoding[0], full_encoding.size());
    bytes raw_hash(32);
    blake2b.TruncatedFinal(&raw_hash[0], 32);
    AionSignature sig = aion_sign(&private_key[0], &raw_hash[0], 32);

    // Item 6: signature
    s.clear();
    bytes raw_sig;
    raw_sig.insert(raw_sig.end(), &private_key[32], &private_key[64]);
    raw_sig.insert(raw_sig.end(), &sig.data[0], &sig.data[64]);
    s.append(raw_sig);
    bytes pre_sig = s.out();

    all_in_one.insert(all_in_one.end(), pre_sig.begin(), pre_sig.end());

    // Last step: prepend version = 0x00
    s.clear();
    bytes output = {0x00};
    bytes rlp_encoded = s.appendList(all_in_one).out();
    output.insert(output.end(), rlp_encoded.begin(), rlp_encoded.end());

    return output;
}

bytes Transaction::sign(const bytes key) const {
    std::cout << "Unimplemented" << std::endl;
}

bytes Transaction::hash() const {
    CryptoPP::BLAKE2b blake2b(32U);
    bytes rlp_data = rlp_unverified();
    blake2b.Update(&rlp_data[0], rlp_data.size());
    bytes ret(32);
    blake2b.TruncatedFinal(&ret[0], 32);
    return ret;
}

bytes Transaction::rlp_unverified() const {
    RLPStream s(8);
    s.append(nonce);
    if (is_create)
        s.append("");
    else
        s.append(target_addr);
    s.append(value);
    s.append(data);
    bytes timestamp(8);
    unsigned long now = system_clock::now().time_since_epoch().count();
    for (int i = 0; i < 8; i++) {
        timestamp[i] = (now & (0xff << i*8)) >> (i*8);
    }

    s.append("");
    s.append((long)gas);
    s.append((long)gas_price);
    s.append(tx_type);
//    s.append(sig);

    return s.out();
}
