#include "rcry_utils.h"


// Crypto++ includes
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/tiger.h>
#include <cryptopp/des.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

// Standardlib include
#include <string>
#include <iostream>
#include "malloc.h"

// libgcrypt for hashing
#include <gcrypt.h>

using namespace CryptoPP;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

char *rcry_utils::generate_minitoken() {
    char *token;

    /* Use gcrypt random number generator */
    token = (char *) gcry_random_bytes(32, GCRY_STRONG_RANDOM);

    /* Use Crypto++ utilities because they are very convenient */
    string encoded; // TODO: Write your own so we can ditch crypto++ ?
    StringSource((byte *) token, 32, true, new HexEncoder(new StringSink(encoded)));

    /* Then copy it back in place */
    memcpy(token, encoded.c_str(), sizeof(long) * 32);
    return token;
}

char *rcry_utils::generate_random(unsigned int length, bool clear) {
    char *buffer = (char *) gcry_random_bytes(length, GCRY_STRONG_RANDOM);

    if (clear) {
        string encoded;
        StringSource((byte *) buffer, length, true, new HexEncoder(new StringSink(encoded)));
        free(buffer);
        char *data = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
        strcpy(data, encoded.c_str());
        return data;
    }
    return buffer;
}

/** Used for UUIDs randomness */
void rcry_utils::generate_weak_rand(char **data, unsigned int l) {
    unsigned int length = l / 2;
    char *w_ptr = *data;

    string encoded;
    char *buffer = (char *) gcry_random_bytes(length, GCRY_WEAK_RANDOM);
    StringSource((byte *) buffer, length, true, new HexEncoder(new StringSink(encoded)));
    free(buffer);
    buffer = NULL;

    w_ptr = (char *) calloc(sizeof(char), strlen(encoded.c_str()));
    memcpy(w_ptr, encoded.c_str(), l);
    cout << "Random Data: " << w_ptr << endl;
    (*data) = w_ptr;
    // strcpy((*data), encoded.c_str());
}

/** Used for tokens & access keys */
void rcry_utils::generate_normal_rand(char **data, unsigned int length) {
    string encoded;
    char *buffer = (char *) gcry_random_bytes(length, GCRY_STRONG_RANDOM);
    StringSource((byte *) buffer, length, true, new HexEncoder(new StringSink(encoded)));
    free(buffer);

    (*data) = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
    strcpy((*data), encoded.c_str());
}

/** Used for permanent or semi-permanent cryptographic keys */
void rcry_utils::generate_super_rand(char **data, unsigned int length) {
    string encoded;
    char *buffer = (char *) gcry_random_bytes(length, GCRY_VERY_STRONG_RANDOM);
    StringSource((byte *) buffer, length, true, new HexEncoder(new StringSink(encoded)));
    free(buffer);

    (*data) = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
    strcpy((*data), encoded.c_str());
}

char *rcry_utils::md_tiger2_salted(char *salt, const char *message, bool clear) {
    int digest_len = gcry_md_get_algo_dlen(GCRY_MD_TIGER2);

    // Malloc some space for a digest and a salty message. The latter one gets freed later.
    char *digest = (char *) malloc(sizeof(char) * digest_len);
    char *salty_message = (char *) malloc(strlen((char *) salt) + strlen(message) + 2);

    // Concatinate our salt with the message and a wonderful seperator named "::"
    strcpy(salty_message, salt);
    strcat(salty_message, "::");
    strcat(salty_message, message);
    unsigned int msg_len = strlen(salty_message);

    gcry_md_hash_buffer(GCRY_MD_TIGER2, digest, message, strlen(message));

    if (clear) {
        string encoded;
        StringSource((byte *) digest, sizeof(char) * digest_len, true, new HexEncoder(new StringSink(encoded)));
        free(digest);

        char *new_digest = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
        strcpy(new_digest, encoded.c_str());
        return new_digest;
    }

    free(salty_message);
    return digest;
}

char *rcry_utils::md_sha256_salted(char *salt, const char *message, bool clear) {
    int digest_len = gcry_md_get_algo_dlen(GCRY_MD_SHA256);

    // Malloc some space for a digest and a salty message. The latter one gets freed later.
    char *digest = (char *) malloc(sizeof(char) * digest_len);
    char *salty_message = (char *) malloc(strlen((char *) salt) + strlen(message) + 2);

    // Concatinate our salt with the message and a wonderful seperator named "::"
    strcpy(salty_message, salt);
    strcat(salty_message, "::");
    strcat(salty_message, message);
    unsigned int msg_len = strlen(salty_message);

    gcry_md_hash_buffer(GCRY_MD_SHA256, digest, message, strlen(message));

    if (clear) {
        string encoded;
        StringSource((byte *) digest, sizeof(char) * digest_len, true, new HexEncoder(new StringSink(encoded)));
        free(digest);

        char *new_digest = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
        strcpy(new_digest, encoded.c_str());
        return new_digest;
    }

    free(salty_message);
    return digest;
}

char *rcry_utils::md_blake_salted(char *salt, const char *message, bool clear) { }

char *rcry_utils::md_tiger2(const char *message, bool clear) {

    int digest_len = gcry_md_get_algo_dlen(GCRY_MD_TIGER2);
    char *digest = (char *) malloc(sizeof(char) * digest_len);
    gcry_md_hash_buffer(GCRY_MD_TIGER2, digest, message, strlen(message));

    if (clear) {
        string encoded;
        StringSource((byte *) digest, sizeof(char) * digest_len, true, new HexEncoder(new StringSink(encoded)));
        free(digest);

        char *new_digest = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
        strcpy(new_digest, encoded.c_str());
        return new_digest;
    }

    return digest;
}

char *rcry_utils::md_sha256(const char *message, bool clear) {
    int digest_len = gcry_md_get_algo_dlen(GCRY_MD_SHA256);
    char *digest = (char *) malloc(sizeof(char) * digest_len);
    gcry_md_hash_buffer(GCRY_MD_SHA256, digest, message, strlen(message));

    if (clear) {
        string encoded;
        StringSource((byte *) digest, sizeof(char) * digest_len, true, new HexEncoder(new StringSink(encoded)));
        free(digest);

        char *new_digest = (char *) malloc(sizeof(char) * strlen(encoded.c_str()));
        strcpy(new_digest, encoded.c_str());
        return new_digest;
    }

    return digest;
}

char *rcry_utils::md_blake(const char *message, bool clear) { }


byte *return_buffer(const std::string &string) {
    byte *return_string = new byte[string.length() + 1];
    strcpy((char *) return_string, string.c_str());

    return return_string;
}

char *rcry_utils::salted_tiger2_hash(char *salt, std::string *input) {
    CryptoPP::Tiger hash;
    byte digest[CryptoPP::Tiger::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte *) input->c_str(), input->length());
    return (char *) digest;
}

char *rcry_utils::salted_sha256_hash(char *salt, string *input) {
    CryptoPP::SHA256 hash;
    byte digest[CryptoPP::SHA256::DIGESTSIZE];
    hash.CalculateDigest(digest, (byte *) input->c_str(), input->length());
    return (char *) digest;
}
