#include "ed25519.h"
#include "byte_util.h"
#include "aion_sign.h"
#include "rpc.h"
#include <json/json.h>
#if _WIN32
#error "No support on WIN32 yet"
#else
#include "pthread.h"
#endif

#include <iostream>
#include <cassert>
#include <cstring>
#include <unistd.h>

#define TX_TEST     true

static void byte_util_test() {
    const unsigned char data[10] = {0x12, 0x34, 0x1A, 0x2b};
    assert(to_hex_string(data, 10) == "12341A2B000000000000");
}

static void keypair_test() {
    const unsigned char seed[] = "12345678901234567890123456789012";
    unsigned char public_key[32], private_key[32];
    ed25519_create_keypair(public_key, private_key, seed);
    assert(to_hex_string(public_key, 32) == "2F8C6129D816CF51C374BC7F08C3E63ED156CF78AEFB4A6550D97B87997977EE");
    assert(to_hex_string(private_key, 32) == "F0106660C3DDA23F16DAA9AC5B811B963077F5BC0AF89F85804F0DE8E424F050");
}

static void signature_test() {
    unsigned char signature[64];
    const unsigned char seed[] = "12345678901234567890123456789012";
    unsigned char public_key[32], private_key[32];
    ed25519_create_keypair(public_key, private_key, seed);
    const unsigned char message[5] = {0x1, 0x2, 0x3, 0x4, 0x5};
    ed25519_sign(signature, message, 5, public_key, private_key);
    assert(to_hex_string(signature, 64) == "3B7A1C1FB9CB3F50B780C691F3C14E5A401DD37D267078D6E67513561A4E8849B9FEDFEACA0E0216E68F115EB8A4C4A7AFE37B07B7A594200171DF7D4D82E107");
}

static void aion_sign_test() {
    const unsigned char seed[32] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02};//"12345678901234567890123456789012";
    unsigned char public_key[32], private_key[32];
    ed25519_create_keypair(public_key, private_key, seed);
    unsigned char aion_sk[64];
    memcpy((void *)aion_sk, seed, 32);
    memcpy((void *)(aion_sk+32), public_key, 32);
    const unsigned char message[] = "hello, aion";
    std::cout << __func__ << std::endl;
    AionSignature sig = aion_sign(aion_sk, message, sizeof(message));
    std::cout << "Aion Signature: ";
    for (int i = 0; i < 64; i++) {
        printf("%.2x", sig.data[i]);
    }
    std::cout << std::endl;
}

static void rpc_test() {
    int block_number = get_blocknumber();
    assert(block_number >= 0);
    printf("best block: %d\n", block_number);
    Block block = get_block(10);
    assert(block.number == 10);
}

static int current_block = 0;
static int total_block = 0;
static bool stop = false;
static int total_pos_block = 0;
static int my_pos_block = 0;
static int waiting_gap = 10;
static int previous_span = 0;

static std::string mykeys[] = {"0x424bce35af1f6154c4d1eefcc4975864eb18646fe9d48c16c6fc9797d2b10210"};

bool is_produced_by_us(std::string &got_key) {
    for (int i = 0; i < mykeys->size(); i++) {
        if (mykeys[i] == got_key) {
            return true;
        }
    }
    return false;
}

void *pos_listener(void *args) {
    int report_id = 0;
    while (!stop) {
        total_block = get_blocknumber();
        if (current_block < total_block - waiting_gap) {
            Block block = get_block(current_block++);
            if (block.seal_type == "0x2") {
                total_pos_block++;
                if (is_produced_by_us(block.public_key)) {
                    printf("block from sh: %ld\n", block.number);
                    my_pos_block++;
                }
            }
        }
        if (current_block == previous_span + 100) {
            previous_span = current_block;
            printf(
                    "Report id %d:\n\t"
                   "Total Blocks: %d\n\t"
                   "Total PoS Blocks: %d\n\t"
                   "My PoS Blocks: %d\n\t"
                   "Waiting Gap: %d\n",
                    report_id++, total_block, total_pos_block, my_pos_block, waiting_gap);
        }
    }
}

#if TX_TEST
#include "Transaction.h"
static bytes generate_inner_tx() {
    bytes sk = from_hex_string("0x04af28dedc829c6e4220a8d5dab7feea41df6c33152ba5610ab6e987ba65979d7703668a89ef15099f1102b193a3db4661e9052e244a3fe2062664929ed15555");
    bytes data;
    bytes executor = from_hex_string("0xa0eaf32b302ecf3b931b675ae8b923d737173704a2041f13af8a3821efcedd52");
    u256 to("0xa04e251db6f1d5c91fcb6cf46b69698d4c64439e71606e0f961653ca5a397c85");
    InvokableTransaction invokable_tx(data,executor,to,21000L,1);
    return invokable_tx.sign(sk);
}

static void tx_test() {
    bytes sk = from_hex_string("0x04af28dedc829c6e4220a8d5dab7feea41df6c33152ba5610ab6e987ba65979d7703668a89ef15099f1102b193a3db4661e9052e244a3fe2062664929ed15555");
    bytes data;
    bytes executor(32);//= from_hex_string("0xa0eaf32b302ecf3b931b675ae8b923d737173704a2041f13af8a3821efcedd52");
    u256 to("0xa04e251db6f1d5c91fcb6cf46b69698d4c64439e71606e0f961653ca5a397c85");
    InvokableTransaction invokable_tx(data,executor,to,21000L,0);
    bytes inner_tx = invokable_tx.sign(sk);
}
#endif

#define ABI_TEST    true

#if ABI_TEST
#include "Abi.h"
static void abi_test() {
    Abi abi;
    std::string method("invoke");
    abi.append(method);
    bytes data = generate_inner_tx();
    abi.append(data);
    for (byte item: abi.out()) {
        printf("%.2x", item);
    }
    std::cout << std::endl;
}
#endif

void suite_test() {
#if BYTE_TEST
    byte_util_test();
#endif
#if KEYPAIR_TEST
    keypair_test();
#endif
#if SIGNATURE_TEST
    signature_test();
    aion_sign_test();
#endif
#if RPC_TEST
    rpc_test();
#endif
#if TX_TEST
    tx_test();
#endif
//    parse_response();
#if POS_LISTENER
    pthread_t invoker;
    pthread_create(&invoker, nullptr, pos_listener, nullptr);
    pthread_join(invoker, nullptr);
#endif
#if ABI_TEST
    abi_test();
#endif
}

void invoke_tx(char *buff) {
    Json::Reader reader;
    Json::Value value;

    if(reader.parse(buff, value)) {
        Json::Value objs = value["transactions"];
        for (int i = 0; i < objs.size(); i++) {
            Json::Value temp = objs[i];
            bytes data = from_hex_string(temp["data"].asString());
            bytes value = from_hex_string(temp["executor"].asString());
            u256 to(temp["to"].asString());
            long gas = temp["gas"].asInt();
            bytes private_key = from_hex_string(temp["sk"].asString());
            InvokableTransaction tx(data,value,to,gas);
            bytes inner_tx = tx.sign(private_key);

            for (byte item: inner_tx) {
                printf("%.2x", item);
            }
            std::cout << std::endl;

            std::cout << tx << std::endl;
        }
    }
}

#if P2P_ENABLE

#define MSG_LENGTH      2 * 1024//*1024
static char hello_msg[MSG_LENGTH];

#include "node.h"

void start_p2p() {
    Node server("server", "127.0.0.1", 30303);
    server.listen();

    Node lin2("lin2", "45.79.112.46", 30303);
    Node local("local", "192.168.50.169", 30303);
    Node farb("farb", "137.116.86.182", 30303);

    int count = 0;
    for (int i = 0; i < MSG_LENGTH; i++)
        hello_msg[i] = 'c';
    while (true) {
        sleep(1);
        std::cout << "Round " << count++ << std::endl;
        Node client("client", "127.0.0.1", 1234);
        client.send(farb, hello_msg);
        close(client.ss);
    }
}
#endif
