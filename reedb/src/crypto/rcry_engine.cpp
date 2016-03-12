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
string rcry_engine::encrypt_string(string data, crycontext *context) {
    string plain = "Yo dawg, I heard you like encryption!";
    string cipher, encoded, recovered;

    try {
        cout << "plain text: " << data << endl;

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(data, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return cipher;
}

string rcry_engine::decrypt_string(string data, crycontext *context) {
    string encoded, recovered;

    StringSource(data, true, new HexEncoder(new StringSink(encoded)));
    cout << "cipher text: " << encoded << endl;

    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(data, true, new StreamTransformationFilter(d, new StringSink(recovered)));

        cout << "Recovered Text: " << recovered << endl;

    } catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return recovered;

}

char *rcry_engine::encrypt(void *data, crycontext *context) {
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
        memcpy(this->context_key, (*this->context_map)[token], AES::MAX_KEYLENGTH);

        // string encoded;
        // encoded.clear();
        // StringSource(this->context_key, AES::MAX_KEYLENGTH, true, new HexEncoder(new StringSink(encoded)));

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

    /* Save the current key, generate a salt and salt-hash the current user passphrase */
    byte buffer[AES::MAX_KEYLENGTH];
    memcpy(buffer, this->context_key, AES::MAX_KEYLENGTH);

    salt = rcry_utils::generate_random(8, true);
    byte *user_key = (byte *) rcry_utils::md_sha256_salted(salt, passphrase->c_str(), false);

    /* Now manually overwrite the key! */
    memcpy(this->context_key, user_key, AES::MAX_KEYLENGTH);

    /* Generate an IV for  */
    crycontext *context = new crycontext();

//    string encoded;
//    StringSource(user_key, AES::MAX_KEYLENGTH, true, new HexEncoder(new StringSink(encoded)));
//    cout << "PW Hash:" << encoded << endl;

    char *iv = rcry_utils::generate_random(CryptoPP::AES::BLOCKSIZE, true);
    memcpy(iv, context->iv, CryptoPP::AES::BLOCKSIZE);
    context->size = CryptoPP::AES::BLOCKSIZE;

    /* Now go and encrypt our buffer with the new context key */
    string buffered_key = string((char *) buffer);
    string encrypted_key = this->encrypt_string(buffered_key, context);

    // Used for testing
    // string recovered = this->decrypt_string(foo, context);

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
