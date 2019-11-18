PROJECT(eth)

AUX_SOURCE_DIRECTORY(secp256k1 SECP256K1)

include_directories(secp256k1 secp256k1/impl)

add_library(libeth Common.cpp RLP.cpp ${SECP256K1})