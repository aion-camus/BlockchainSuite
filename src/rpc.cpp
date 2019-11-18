#include "post.h"
#include <malloc.h>
#include <iostream>
#include <json/json.h>

#include "rpc.h"

using namespace std;

RpcResult parse_response(std::string &message)
{
//    string message = "{ \"data\" : { \"username\" : \"test\" }, \"type\" : 6 }";
    Json::Reader reader;
    Json::Value value;
    RpcResult res;
    if(reader.parse(message.c_str(), value)) {
#if 0
        Json::Value temp = value["data"];
        cout << "username:" << temp["username"].asString() << endl;
        cout << "type:" << value["type"].asInt() << endl;
        cout << "id:" << value["id"].asString() << endl;
#endif
        Json::Value temp = value["result"];
        res.public_key = temp["publicKey"].asString();
        res.seal_type = temp["seal_type"].asInt();
    }

    return res;
}

static char get_block_req[] = "{\"jsonrpc\":\"2.0\", \"method\":\"eth_getBlockByNumber\", \"params\":[%d, false], \"id\":\"hello\"}";

int get_blocknumber() {
    int status = post("{\"jsonrpc\":\"2.0\", \"method\":\"eth_blockNumber\", \"id\":\"hello\"}");
    if (status) {
        printf("rpc error: %d\n", status);
        return status;
    }
    // TODO: return real block number
    HttpResponse resp = get_response();
    Json::Reader reader;
    Json::Value value;
    int ret = 0;
    if (reader.parse(resp.data.c_str(), value)) {
        Json::Value temp = value["result"];
        ret = temp.asInt();
    }

    return ret;
}

Block get_block(int number) {
    auto *buffer = (char *)malloc(1024);
    Block ret;
    if (buffer) {
        sprintf(buffer, get_block_req, number);
        int status =  post(buffer);
        free(buffer);
        HttpResponse resp = get_response();
        Json::Reader reader;
        Json::Value value;
        if(reader.parse(resp.data, value)) {
            Json::Value temp = value["result"];
            ret.seal_type = temp["sealType"].asString();
            ret.number = temp["number"].asInt();
            ret.public_key = temp["publicKey"].asString();
        }
    }

    return ret;
}