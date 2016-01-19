//
// Created by spacekookie on 22/11/15.
//

#include "rcry_engine.h"

#include <iostream>

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;


rcry_engine::rcry_engine()
{

}

void rcry_engine::init()
{

}

void rcry_engine::master_keygen(unsigned char *(*key), rdb_uuid *uuid)
{
  AutoSeededRandomPool prng;
  
  /* Malloc the keysize first and check it was successful... */
  (*key) = (unsigned char*) malloc(AES::MAX_KEYLENGTH);
  if((*key) == NULL)
  {
    std::cout << "Failed to malloc key size!" << std::endl;
    throw 0;
  }
  
  /* ...then generate random noise in it*/
  prng.GenerateBlock((*key), sizeof(*key));
}
