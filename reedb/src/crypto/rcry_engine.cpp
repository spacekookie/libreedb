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
#include <gcrypt.h>

using namespace std;

int my_id;

rcry_engine::rcry_engine(int id) {
    cout << "Creating rcry_engine with ID " << id << endl;
    this->context_map = new map<rcry_token *, byte[AES::MAX_KEYLENGTH]>();
    my_id = id;
}

int rcry_engine::query_id() { return my_id; }

void rcry_engine::init(rcry_token *token) {
    /* Generate STRONG_RANDOM key of size 32 byte */
    char *random = (char *) gcry_random_bytes_secure(AES::MAX_KEYLENGTH, GCRY_STRONG_RANDOM);

    /* Then copy from stack into token map space */
    memcpy((*this->context_map)[token], random, AES::MAX_KEYLENGTH);
}

/** Simple utility function to encrypt a C++ string */
string rcry_engine::encrypt_string(string data, crycontext *context) {
    string plain = "Hello Reedb! :)";
    string cipher, encoded, recovered;

    try {
        //        StringSource(data, true, new HexEncoder(new StringSink(encoded)));
        //        cout << "plain text: " << encoded << endl;

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(data, true, new StreamTransformationFilter(e, new StringSink(cipher)));

        encoded.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
        //        cout << "Encrypted: " << encoded << endl;
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return cipher;
}

string rcry_engine::decrypt_string(string data, crycontext *context) {
    string encoded, recovered;

//    StringSource(data, true, new HexEncoder(new StringSink(encoded)));
//    cout << "cipher text: " << encoded << endl;

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

void rcry_engine::switch_context(rcry_token *token) {
    map<rcry_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);

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

string rcry_engine::get_encrypted_key(char *(*salt), char *(*iv), rcry_token *token, string *passphrase) {
    string encoded;

    /* Check that we're actually allowed to access this token by comparing it to the current context */
    map<rcry_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);
//    if (token == nullptr) goto release;
//    if ((*this->context_map)[token] != context_key) goto release;

    /* Save the current key, generate a salt and salt-hash the current user passphrase */
    byte buffer[AES::MAX_KEYLENGTH];
    memcpy(buffer, this->context_key, AES::MAX_KEYLENGTH);

    *salt = rcry_utils::generate_random(8, true);
    byte *user_key = (byte *) rcry_utils::md_sha256_salted(*salt, passphrase->c_str(), false);

    /* Now manually overwrite the key! */
    memcpy(this->context_key, user_key, AES::MAX_KEYLENGTH);

    /* Create a new context object for our crypto to work on */
    crycontext *context = new crycontext();
    context->size = AES::MAX_KEYLENGTH;

    /* Now generate an IV and store it in the context */
    rcry_utils::generate_normal_rand(iv, AES::BLOCKSIZE * sizeof(char));
    memcpy(context->iv, *iv, AES::BLOCKSIZE);

    /* Then clean it up for future generations */
    encoded.clear();
    StringSource((byte *) (*iv), AES::BLOCKSIZE, true, new HexEncoder(new StringSink(encoded)));
    memcpy((char *) (*iv), encoded.c_str(), AES::BLOCKSIZE * sizeof(char));

    /* Now go and encrypt our buffer with the new context key */
    string buffered_key = string((char *) buffer);
    string encrypted_key = this->encrypt_string(buffered_key, context);

    return encoded;

//    release:
    /* We only reach this when an invalid token was used */
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

bool rcry_engine::confirm_passphrase(char **salt, char **iv, char *encrypted, std::string *passphrase) {
//    /* Hash our passphrase with the predetermined salt */
//    byte *user_key = (byte *) rcry_utils::md_sha256_salted(*salt, passphrase->c_str(), false);
//
//    /* Then overwrite the key TODO: Change this to work via a context switch */
//    memcpy(this->context_key, user_key, AES::MAX_KEYLENGTH);
//
//    /* Create a new context object for our crypto to work on */
//    crycontext *context = new crycontext();
//    context->size = AES::MAX_KEYLENGTH;
//
//    /* Now generate an IV and store it in the context */
//    rcry_utils::generate_normal_rand(iv, AES::BLOCKSIZE * sizeof(char));
//    memcpy(context->iv, *iv, AES::BLOCKSIZE);
//
//    /* Then clean it up for future generations */
//    string encoded;
//    StringSource((byte *) (*iv), AES::BLOCKSIZE, true, new HexEncoder(new StringSink(encoded)));
//    memcpy((char *) (*iv), encoded.c_str(), AES::BLOCKSIZE * sizeof(char));
//
    return false;
}
