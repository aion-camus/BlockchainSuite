#ifndef BLOCKCHAINSUITE_BYTE_UTIL_H
#define BLOCKCHAINSUITE_BYTE_UTIL_H

#include <string>
#include <vector>

std::string to_hex_string(const unsigned char bytes[], unsigned int len);
std::vector<uint8_t> from_hex_string(const std::string &input);

#endif //BLOCKCHAINSUITE_BYTE_UTIL_H
