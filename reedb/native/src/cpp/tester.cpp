/*
 * tester.cpp
 *
 *  Created on: 1 Jul 2015
 *      Author: spacekookie
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

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
using CryptoPP::CTR_Mode;

#include <assert.h>

#include "rcry_wrapper.hpp"

int main(int argc, char* argv[])
{
	byte *key;

	rcry_generateKey(RCRY_TWOFISH, &key);

	string encoded;
	encoded.clear();
	StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)) // HexEncoder
			);// StringSource
	cout << "key: " << encoded << endl;

	rcry_cryptoInit(RCRY_RIJNDAEL, &key);

	//\\// RAW DATA FROM WRAPPER CODE \\//
	raw_datafile_t *rdf = (struct raw_datafile_t*) malloc(
			sizeof(struct raw_datafile_t));

	rdf->body =
			"{'1::1237823':{'passphrase':'qwertzuiop', 'username':'spacekookie'}, '2::234878':{'passphrase':'BetterCarHorseBattery'}}";
	rdf->header =
			"{'name'=>'Lonely Robot', 'urls'=>['www.myface.com', 'mobile.myface.com'], 'tags'=>['social media', 'awesome', 'web login']}";
	rdf->meta_name = "Myface";

	//\\// RAW DATA FROM WRAPPER CODE END \\//

	// This is our dummy file for disk handling
	reefile_t *myFile = (struct reefile_t*) malloc(sizeof(struct reefile_t));
	myFile->context = (struct ree_ccontext_t*) malloc(
			sizeof(struct ree_ccontext_t));

	rcry_setCryptoContext(myFile->context);

	rcry_encryptInContext(myFile->head, &rdf->header);
	rcry_encryptInContext(myFile->body, &rdf->body);

	rcry_cryptoStop();

	cout << "Going down for a nap..." << endl;
	return 0;
}
