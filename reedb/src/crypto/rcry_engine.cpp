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

rcry_engine::rcry_engine()
{
  this->context_map = new map<rdb_token*, crycontext*>();
}

void rcry_engine::init(unsigned char *master_key, rdb_token *token)
{
  crycontext *context = new crycontext();
  unsigned char buffer[AES::MAX_KEYLENGTH];
  
  for(int i = 0; i < AES::MAX_KEYLENGTH; i++) {
    buffer[i] = master_key[i];
  }
  
  // memcpy(context->key, master_key, AES::MAX_KEYLENGTH); 
  (*this->context_map)[token] = context;
  cout << "Adding context to engine map" << endl;
}

/** Simple utility function to encrypt a C++ string */
string *rcry_engine::encrypt_string(string* data)
{
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

  try
  {
    cout << "plain text: " << plain << endl;

    CBC_Mode< AES >::Encryption e;
    e.SetKeyWithIV(this->context->key, sizeof(this->context->key), iv);

    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource s(plain, true, 
      new StreamTransformationFilter(e,
	new StringSink(cipher)
      ) // StreamTransformationFilter
    ); // StringSource

  }
  catch(const CryptoPP::Exception& e)
  {
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

void rcry_engine::switch_context(rdb_token* token)
{
  cout << "Checking for token availability..." << endl;
  
  crycontext *context = (*this->context_map)[token];
  cout << "Context key: " << context->key << endl;
}

void rcry_engine::master_keygen(byte *(*key), rdb_uuid *uuid)
{
  AutoSeededRandomPool prng;
  
  /* Malloc the keysize first and check it was successful... */
  (*key) = (byte*) malloc(AES::MAX_KEYLENGTH);
  if((*key) == NULL)
  {
    std::cout << "Failed to malloc key size!" << std::endl;
    throw 0;
  }
  
  /* ...then generate random noise in it*/
  prng.GenerateBlock((*key), AES::MAX_KEYLENGTH);
}
