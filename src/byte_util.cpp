#include "byte_util.h"
#include <iostream>

std::string to_hex_string(const unsigned char bytes[], unsigned int len) {
    const char hex_chars[] = "0123456789ABCDEF";

    char *hex_string = (char *)malloc(len*2);
    for (int i = 0; i < len; i++) {
        hex_string[2*i] = hex_chars[bytes[i] >> 4];
        hex_string[2*i+1] = hex_chars[bytes[i] & 0xf];
    }

    std::string res(hex_string);
    free(hex_string);

    return res;
}

static char toUpper(char input) {
    if ('a' <= input && 'z' >= input)
        return input-'a'+'A';
    return input;
}

static char toLower(char input) {
    if ('A' <= input && 'Z' >= input)
        return input-'A'+'a';
    return input;
}

static uint8_t convertToUint(char input) {
    char raw = toUpper(input);
    if (raw >= 'A' && raw <= 'Z')
        return raw-'A'+10;
    else if ('0' <= raw && '9' >= raw)
        return raw-'0';
    else
        throw "Unexpected input for Uint Convertion";
}

std::vector<uint8_t> from_hex_string(const std::string &input) {
    std::string raw_data;
    std::vector<uint8_t> output;
    if (input.substr(0, 2) == "0x")
        raw_data = input.substr(2, input.length());
    if (raw_data.length() % 2 != 0)
        throw "odd input length";
    try {
        for (int i = 0; i < raw_data.length(); i+=2) {
            uint8_t tmp = 0;
            tmp |= (convertToUint(raw_data[i]) & 0xf) << 4;
            tmp |= (convertToUint(raw_data[i+1]) & 0xf);
            output.push_back(tmp);
        }

        return output;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}