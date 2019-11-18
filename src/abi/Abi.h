
#ifndef BLOCKCHAINSUITE_ABI_H
#define BLOCKCHAINSUITE_ABI_H

#include <vector>
#include <string>

using namespace std;

enum TokenType {
    T_UCHAR = 0x11,
    T_STRING = 0x21
};

using bytes = vector<uint8_t>;
using byte = uint8_t;

class Abi {
public:
    Abi();
    Abi &append(std::string& name);
    Abi &append(std::vector<byte>);
    bytes out() { return m_data; }

private:
    bytes m_data;
};

#endif //BLOCKCHAINSUITE_ABI_H
