//
// Created by spacekookie on 22/11/15.
//

#include "rcry_engine.h"

#include <iostream>
#include <malloc.h>
#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
using CryptoPP::Base64Encoder;
using CryptoPP::AutoSeededRandomPool;

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
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>

#include "rcry_utils.h"
#include "rcry_token.h"
#include <gcrypt.h>

using namespace std;
using namespace CryptoPP;

int my_id;

rcry_engine::rcry_engine(int id) {
    if (RDB_DEBUG) cout << "Creating rcry_engine with ID " << id << endl;
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
    string cipher, encoded, recovered;

    try {
        //        StringSource(data, true, new HexEncoder(new StringSink(encoded)));
        //        if (RDB_DEBUG) cout << "plain text: " << encoded << endl;

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(data, true, new StreamTransformationFilter(e, new StringSink(cipher)));

        encoded.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
        //        if (RDB_DEBUG) cout << "Encrypted: " << encoded << endl;
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return cipher;
}

string rcry_engine::decrypt_string(string data, crycontext *context) {
    string encoded, recovered;

    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(data, true, new StreamTransformationFilter(d, new StringSink(recovered)));

    } catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        return NULL;
    }
    return recovered;
}

/** Returns a base64 encoded array of cipher text */
char* rcry_engine::encrypt(void *data, rcry_token *token) {
    string cipherbuf, encoded, recovered;

    try {
        string buffer = string((char*) data);

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(buffer, true, new StreamTransformationFilter(e, new StringSink(cipherbuf)));

        encoded.clear();
        StringSource(cipherbuf, true, new HexEncoder(new StringSink(encoded)));
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        return NULL;
    }

    /** Copy the information over into a permanent array */
    char *cipher = (char*) malloc(sizeof(char) * strlen(cipherbuf.c_str()));
    strcpy(cipher, cipherbuf.c_str());

    return cipher;
}

char* rcry_engine::decrypt(void *data, rcry_token *token) {
    string encoded, recbuffer;
    string buffer = string((char*) data);

    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(this->context_key, sizeof(this->context_key), context->iv);
        StringSource s(buffer, true, new StreamTransformationFilter(d, new StringSink(recbuffer)));

    } catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        return NULL;
    }

    /** Copy the data over into a permanent array */
    char *recovered = (char*) malloc(sizeof(char) * strlen(recbuffer.c_str()));
    strcpy(recovered, recbuffer.c_str());

    return recovered;
}

unsigned int rcry_engine::start_batch(rcry_token *token, bool block) {

    if (RDB_DEBUG) cout << "Registering a batch job on cry_engine " << my_id << endl;
    if (RDB_DEBUG) cout << RDB_DEBUG << endl;

    rcry_batch *batch = new rcry_batch();
    batch->num_id = 42;
    batch->self = token;

    this->switch_context(token);
    return 0;

    /** This means there is no currently queued job*/
//    if(this->batch_queue == nullptr)
//    {
//        this->batch_queue = new rcry_batch();
//        this->batch_queue->self = token;
//
//        /** Now actually do the context switch because it's our turn! */
//        this->switch_context(token);
//        return 0;
//
//    /** Walk down the queue and put this batch at the end. Then either wait or return */
//    } else {
//        unsigned int ctr = 0;
//
//        while(this->batch_queue)
//        {
//            /** If the next slot is free */
//            if(!this->batch_queue->next)
//            {
//                this->batch_queue->next = new rcry_batch();
//                this->batch_queue->next->self = token;
//
//                /** Break out the loop */
//                break;
//            }
//
//            /** Increment the "waiting before" counter */
//            ctr++;
//        }
//
//        if(block)
//        {
//            while(this->batch_queue->self != token)
//            {
//                // Sleep for a bit
//            }
//
//            /** SUCCESS! It's our turn now! */
//            this->switch_context(token);
//
//        /** Just return queue length (before self) */
//        } else {
//            return ctr;
//        }
//    }
}

unsigned int rcry_engine::end_batch(rcry_token *token) {
    if (RDB_DEBUG) cout << "Finishing up batch job..." << endl;

//    if(this->batch_queue->self == token)
//    {
        this->switch_context(NULL);
//    } else {
//        if (RDB_DEBUG) cout << "[ERROR] " << endl;
//    }
}


void rcry_engine::switch_context(rcry_token *token) {
    if (RDB_DEBUG) cout << "[WARN] Using dangerous function! Support will be removed "
                    "in the future. Use `start_batch`, `end_batch` instead!" << endl;

    map<rcry_token *, byte[AES::MAX_KEYLENGTH]>::iterator it = (*this->context_map).find(token);

    if (token == nullptr) goto release;

    if (RDB_DEBUG) cout << "Checking for token availability...";
    if (it != (*this->context_map).end()) {
        this->cry_lock = true;
        memcpy(this->context_key, (*this->context_map)[token], AES::MAX_KEYLENGTH);

        // string encoded;
        // encoded.clear();
        // StringSource(this->context_key, AES::MAX_KEYLENGTH, true, new HexEncoder(new StringSink(encoded)));

        if (RDB_DEBUG) cout << "done!" << endl;
        return;
    }

    if (RDB_DEBUG) cout << "[ERROR]" << endl << " Token not found!" << endl;
    return;

    release:
    this->cry_lock = false;
    memset(this->context_key, 0, AES::MAX_KEYLENGTH);
    if (RDB_DEBUG) cout << "Crypto Engine now released for new token" << endl;
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
        if (RDB_DEBUG) cout << "Failed to malloc key size!" << std::endl;
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
