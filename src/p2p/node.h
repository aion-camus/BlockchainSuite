
#ifndef BLOCKCHAINSUITE_NODE_H
#define BLOCKCHAINSUITE_NODE_H

#include <string>
#include <pthread.h>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

using namespace std;

class Node {
public:
    Node(const string &id, const string &ip, const uint16_t port);
    ~Node();

    // Server API: return status after listening
    void listen();
    void send(const Node &remote, const std::string &buff);
    void add_node(const int conn, const Node &node) {nodes.insert(pair<int, Node>(conn, node));}
    void del_node(const int conn) {auto iter = nodes.find(conn);nodes.erase(iter);}
    void set_server(const string server_addr);
    operator bool() const {
        return false;
    }

    // socket that is responsible to handle connections
    int ss;

private:
    std::string id, ip;
    uint16_t port;
    struct sockaddr_in sock_addr;
    // <key, value> = <conn, Node>
    std::map<int, Node> nodes;
    pthread_t listener;
    pthread_t sender;
    bool listener_enabled;
    bool sender_enabled;
};

#endif //BLOCKCHAINSUITE_NODE_H
