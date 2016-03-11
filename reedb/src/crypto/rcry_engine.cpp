//
// Created by spacekookie on 22/11/15.
//

#include "rcry_engine.h"

#include <iostream>

#include <cryptopp/aes.h>

using CryptoPP::AES;

#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>

using CryptoPP::Base64Encoder;

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

#include "rcry_utils.h"

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
string *rcry_engine::encrypt_string(string data) {

}

char *rcry_engine::encrypt(void *data, size_t size) {
    string CipherText;



//    // Encryptor
//    CryptoPP::CBC_Mode<AES>::Encryption
//            Encryptor( this->context_key, sizeof(AES::MAX_KEYLENGTH), iv );
//
//    // Encryption
//    CryptoPP::StringSource( PlainText, true,
//                            new CryptoPP::StreamTransformationFilter( Encryptor,
//                                                                      new CryptoPP::StringSink( CipherText )
//                            ) // StreamTransformationFilter
//    ); // StringSource

    return nullptr;
}

/** Simple utility function to decrypt a C++ string */
//   string *decrypt_string(string *data);

void rcry_engine::switch_context(rdb_token *token) {
    map<rdb_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);

    if (token == nullptr) goto release;

    cout << "Checking for token availability...";
    if (it != (*this->context_map).end()) {
        this->cry_lock = true;
        this->context_key = (*this->context_map)[token];

        string encoded;
        encoded.clear();
        StringSource(this->context_key, AES::MAX_KEYLENGTH, true,
                     new HexEncoder(
                             new StringSink(encoded)
                     ) // HexEncoder
        ); // StringSource

        cout << "done!" << endl;
        return;
    }

    cout << "[ERROR]" << endl << "Token not found!" << endl;
    return;

    release:
    this->cry_lock = false;
    memset(this->context_key, 0, AES::MAX_KEYLENGTH);
    cout << "Crypto Engine now released for new token" << endl;
}

string *rcry_engine::get_encrypted_key(char *salt, rdb_token *token, string *passphrase) {
    char *encrypted;

    /* Check that we're actually allowed to access this token by comparing it to the current context */
    map<rdb_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);
    // if (token == nullptr) goto release;
    // if ((*this->context_map)[token] != context_key) goto release;

    rcry_utils utils;

    /* Save the current key, generate a salt and salt-hash the current user passphrase */
    byte *buffer = this->context_key;
    salt = utils.generate_random(8);
    byte *user_key = (byte*) utils.salted_tiger2_hash(salt, passphrase);

    /* Now manually overwrite the key! */
    this->context_key = user_key;

    /* Now go and encrypt our buffer with the new context key */
    encrypted = this->encrypt(buffer, AES::MAX_KEYLENGTH);
    return nullptr;
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
