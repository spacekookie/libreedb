//
// Created by spacekookie on 22/11/15.
//

#include "rcry_engine.h"

#include <iostream>

#include <cryptopp/aes.h>

using CryptoPP::AES;

#include <cryptopp/osrng.h>

using CryptoPP::AutoSeededRandomPool;

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

#include <string>

using std::string;

#include <cstdlib>

using std::exit;

#include <cryptopp/cryptlib.h>

using CryptoPP::Exception;

#include <cryptopp/hex.h>

using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <cryptopp/filters.h>

using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <cryptopp/aes.h>

using CryptoPP::AES;

#include <cryptopp/ccm.h>

using CryptoPP::CBC_Mode;

#include "assert.h"
#include <iterator>

using namespace std;

rcry_engine::rcry_engine() {
    this->context_map = new map<rdb_token *, byte[AES::MAX_KEYLENGTH]>();
}

void rcry_engine::init(rdb_token *token) {
    /* Initilise a random pool generator */
    AutoSeededRandomPool prng;

    /* Then create a key for this context token in a map */
    prng.GenerateBlock((*this->context_map)[token], sizeof((*this->context_map)[token]));
}

/** Simple utility function to encrypt a C++ string */
string *rcry_engine::encrypt_string(string *data) {
    AutoSeededRandomPool prng;

    byte key[AES::DEFAULT_KEYLENGTH];
    prng.GenerateBlock(key, sizeof(key));

    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    string plain = "CBC Mode Test";
    string cipher, encoded, recovered;

    /*********************************\
    \*********************************/

    // Pretty print key
    encoded.clear();
    StringSource(this->context->key, sizeof(this->context->key), true,
                 new HexEncoder(
                         new StringSink(encoded)
                 ) // HexEncoder
    ); // StringSource
    cout << "key: " << encoded << endl;

    // Pretty print iv
    encoded.clear();
    StringSource(iv, sizeof(iv), true,
                 new HexEncoder(
                         new StringSink(encoded)
                 ) // HexEncoder
    ); // StringSource
    cout << "iv: " << encoded << endl;

    /*********************************\
    \*********************************/

    try {
        cout << "plain text: " << plain << endl;

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(this->context->key, sizeof(this->context->key), iv);

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource s(plain, true,
                       new StreamTransformationFilter(e,
                                                      new StringSink(cipher)
                       ) // StreamTransformationFilter
        ); // StringSource

    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    /*********************************\
    \*********************************/

    // Pretty print
    encoded.clear();
    StringSource(cipher, true,
                 new HexEncoder(
                         new StringSink(encoded)
                 ) // HexEncoder
    ); // StringSource
    cout << "cipher text: " << encoded << endl;
}

/** Simple utility function to decrypt a C++ string */
//   string *decrypt_string(string *data);

void rcry_engine::switch_context(rdb_token *token) {
    if (token == nullptr) goto release;

    cout << "Checking for token availability..." << endl;
    map<rdb_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);
    if (it != (*this->context_map).end()) {
        this->cry_lock = true;
        this->context_key = (*this->context_map)[token];

        return;
    }

    cout << "[ERROR] Token not found!" << endl;
    return;

    release:
    this->cry_lock = false;
    memset(this->context_key, 0, AES::MAX_KEYLENGTH);
    cout << "Crypto Engine now released for new token" << endl;
}

void rcry_engine::get_encrypted_key(unsigned char **key, std::string passphrase) {

    /* First save the current key in a stack array */
    byte raw_key[AES::MAX_KEYLENGTH];
    memcpy(this->context_key, raw_key, AES::MAX_KEYLENGTH);

    
}

void rcry_engine::master_keygen(byte *key, rdb_uuid *uuid) {
    AutoSeededRandomPool prng;

    /* Malloc the keysize first and check it was successful... */
    key = (byte *) malloc(AES::MAX_KEYLENGTH);
    if ((*key) == NULL) {
        std::cout << "Failed to malloc key size!" << std::endl;
        throw 0;
    }

    /* ...then generate random noise in it*/
    prng.GenerateBlock(key, AES::MAX_KEYLENGTH);
}
