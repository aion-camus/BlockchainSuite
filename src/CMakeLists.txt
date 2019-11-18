AUX_SOURCE_DIRECTORY(ed25519 ED25519)
AUX_SOURCE_DIRECTORY(http_client HTTP_CLIENT)
AUX_SOURCE_DIRECTORY(libeth LIB_ETH)
AUX_SOURCE_DIRECTORY(cryptopp820 LIBCRYPTO)
AUX_SOURCE_DIRECTORY(libeth/secp256k1 LIBSECP256K1)
AUX_SOURCE_DIRECTORY(abi ABI)

add_subdirectory(jsoncpp/1.8.1)
add_subdirectory(p2p)
add_subdirectory(ed25519)
add_library(suite
        rpc.cpp
        aion_sign.cpp
        suite.cpp
        byte_util.cpp
        Transaction.cpp
        ${ED25519} ${HTTP_CLIENT} ${LIB_ETH} ${LIBCRYPTO} ${LIBSECP256K1} ${ABI})

set(CMAKE_C_FLAGS "-std=c99 -DUSE_FIELD_GMP -DUSE_NUM_GMP -DUSE_FIELD_INV_NUM")
target_link_libraries(suite curl pthread jsoncpp gmp p2p ed25519)

target_include_directories(
        suite PRIVATE
        ed25519
        http_client
        libeth
        libeth/secp256k1
        cryptopp820
        abi
        p2p)
target_include_directories(suite PRIVATE jsoncpp/1.8.1/include)