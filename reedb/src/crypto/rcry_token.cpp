// Reedb internal includes
#include "rcry_token.h"
#include "crypto/rcry_engine.h"

// Runtime includes
#include <string>
#include <iostream>
#include <iomanip>

// Crypto++ includes
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/des.h>
#include <string>

#include <stdlib.h>

using namespace CryptoPP;

void rcry_token_helper::destroy(rcry_token *token) {
    free(token->contents);
    free(token);
}

// TODO: REWRITE THIS FUNCTION!
void rcry_token_helper::create(rcry_token *(*token), unsigned int timed) {
    (*token) = (rcry_token *) malloc(sizeof(rcry_token)); // TODO: USE SECMEM!

    /** Temporary string for our random sink */
    std::string encoded;

    /** Create a generation scratch */
    const unsigned int BLOCKSIZE = 95; // FIXME: What?
    byte pcbScratch[BLOCKSIZE];

    /** Using a ANSI approved Cipher */
    CryptoPP::AutoSeededX917RNG<CryptoPP::DES_EDE3> rng;
    rng.GenerateBlock(pcbScratch, BLOCKSIZE); // FIXME: Don't use AutoSeeded917....

    /** Base64 encode and strip newlines from random block */
    StringSource ss(pcbScratch, sizeof(pcbScratch), true, new Base64Encoder(new StringSink(encoded)));
    std::string::iterator end_pos = std::remove(encoded.begin(), encoded.end(), '\n');
    encoded.erase(end_pos, encoded.end()); // FIXME: Don't copy data in place

    /** Save our token in the struct */
    (*token)->contents = (char *) malloc(sizeof(char) * 127); // FIXME WHAAAT?
    std::memcpy((*token)->contents, encoded.c_str(), sizeof(char) * 127); // I am so broken...
}
