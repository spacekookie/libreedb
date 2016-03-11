#include "rcry_utils.h"
#include "malloc.h"

// Crypto++ includes
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/tiger.h>
#include <cryptopp/des.h>

#include <cryptopp/hex.h>

using CryptoPP::HexEncoder;

#include <cryptopp/sha.h>
using CryptoPP::SHA;

#include <string>
#include <iostream>

using namespace CryptoPP;;
using namespace std;

char *rcry_utils::generate_random(unsigned int length) {
    char *random = (char *) malloc(sizeof(char) * length);

    /** Temporary string for our random sink */
    std::string encoded;

    /** Create a generation scratch */
    const unsigned int BLOCKSIZE = length;
    byte pcbScratch[BLOCKSIZE];

    /** Using a ANSI approved Cipher */
    CryptoPP::AutoSeededX917RNG<CryptoPP::DES_EDE3> rng;
    rng.GenerateBlock(pcbScratch, BLOCKSIZE);

    /** Base64 encode and strip newlines from random block */
    StringSource ss(pcbScratch, sizeof(pcbScratch), true, new Base64Encoder(new StringSink(encoded)));
    std::string::iterator end_pos = std::remove(encoded.begin(), encoded.end(), '\n');
    encoded.erase(end_pos, encoded.end());

    /** Save our token in the struct */
    std::memcpy(random, encoded.c_str(), sizeof(char) * length);

    return random;
}

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
