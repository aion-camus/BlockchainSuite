//
// Created by camus on 19-11-6.
//

#include "Abi.h"

Abi::Abi() {

}

Abi& Abi::append(std::string &name) {
    m_data.push_back(TokenType::T_STRING);
    uint16_t len = name.length();
    m_data.push_back((byte)((len & 0xff00) >> 8));
    m_data.push_back((byte)(len & 0xff));

    m_data.insert(m_data.end(), name.begin(), name.end());
}

Abi& Abi::append(std::vector<byte> d_array) {
    m_data.push_back(TokenType::T_UCHAR);
    uint16_t len = d_array.size();
    m_data.push_back((byte)((len & 0xff00) >> 8));
    m_data.push_back((byte)(len & 0xff));
    m_data.insert(m_data.end(), d_array.begin(), d_array.end());
}