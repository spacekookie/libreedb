#include "rcry_utils.h"
#include "malloc.h"

// Crypto++ includes
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/des.h>
#include <string>

using namespace CryptoPP;

char *rcry_utils::generate_random(unsigned int length)
{
  char *random = (char*) malloc(sizeof(char) * length);
  
  /** Temporary string for our random sink */
  std::string encoded;
  
  /** Create a generation scratch */
  const unsigned int BLOCKSIZE = length;
  byte pcbScratch[BLOCKSIZE];

  /** Using a ANSI approved Cipher */
  CryptoPP::AutoSeededX917RNG<CryptoPP::DES_EDE3> rng;
  rng.GenerateBlock(pcbScratch, BLOCKSIZE);

  /** Base64 encode and strip newlines from random block */
  StringSource ss(pcbScratch, sizeof(pcbScratch) , true, new Base64Encoder(new StringSink(encoded)));
  std::string::iterator end_pos = std::remove(encoded.begin(), encoded.end(), '\n');
  encoded.erase(end_pos, encoded.end());
 
  /** Save our token in the struct */
  std::memcpy(random, encoded.c_str(), sizeof(char) * length);
  
  return random;
}
