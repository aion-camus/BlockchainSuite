#ifndef BLOCKCHAINSUITE_POST_H
#define BLOCKCHAINSUITE_POST_H

#include <iostream>

struct HttpResponse {
    std::string data;
    // 1: success, 0: failure
    int status;
};

int post(const char data[]);
HttpResponse get_response();

#endif //BLOCKCHAINSUITE_POST_H
