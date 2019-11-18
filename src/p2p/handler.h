
#ifndef BLOCKCHAINSUITE_HANDLER_H
#define BLOCKCHAINSUITE_HANDLER_H

#include <vector>
#include <string>

#include "node.h"

enum Version {
    V0 = 0,
    V1,
    UNKNOWN_VER = 255
};

enum Control {
    NET = 0,
    SYNC = 1,
    UNKNOWN_CTRL = 255
};

enum Action {
    DISCONNECT,
    HANDSHAKEREQ,
    HANDSHAKERES,
    ACTIVENODEREQ = 5,
    ACTIVENODERES,
    UNKNOWN_ACT = 255
};

struct ReqHead {
    uint16_t version;
    uint8_t ctrl;
    Action action;
    uint32_t len;   // body length
};

struct ReqBody {
    std::string node_id;
    std::string net_id;
    uint64_t ip;
    uint32_t port;
    std::string revision;
    std::string version;
};

struct ChannelBuffer {
    ReqHead head;
    ReqBody body;
};

struct Handler {
    Node local_node;
    // server: node; message: cb
    bool send(const Node &node, const ChannelBuffer &cb);
};

#endif //BLOCKCHAINSUITE_HANDLER_H
