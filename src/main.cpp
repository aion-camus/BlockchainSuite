#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#include "suite.h"

#define SUITE_TEST      false
#define CASES_TEST      false
#define P2P_ENABLE      false

#define HELP_INFO   \
"Usage: \nBlochainChain [args]\n\
regiserStaker [identityAddr] [signingAddr] [coinbaseAddr]\n\
sendRawTx   [params]\n\
sign    [json format transaction]"

static void show_help() {
    std::cout << HELP_INFO << std::endl;
}

int main(int argc, char *argv[]) {
#if SUITE_TEST
    suite_test();
#endif
#if CASES_TEST
    run_test();
#endif
#if P2P_ENABLE
    start_p2p();
#endif

    if (argc == 5 && argv[1] == "registerStaker") {
        std::string identityAddr = argv[2];
        std::string signingAddr = argv[3];
        std::string coinbaseAddr = argv[4];
        // TODO: encode params
        (void)identityAddr;
        (void)signingAddr;
        (void)coinbaseAddr;
    } else if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        show_help();
    } else if (argc == 3 && !strcmp(argv[1], "sign")) {
        std::ifstream fin;
        fin.open(argv[2]);
        if (!fin) {
            std::cout << "file open error" << std::endl;
            exit(-1);
        }
        std::string file_name(argv[2]);
        fin.seekg(0, std::ios::end);
        int length = fin.tellg();
        fin.seekg(0, std::ios::beg);
        auto buff = (char *)malloc(length);
        memset(buff, 0, length);
        fin.read(buff, length);
        invoke_tx(buff);
        fin.close();
        free(buff);
    } else {
        show_help();
    }

    return 0;
}