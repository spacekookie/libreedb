/*
 * tester.cpp
 *
 *  Created on: 1 Jul 2015
 *      Author: spacekookie
 */

#include <cryptopp/config.h>
#include <cryptopp/modes.h>
#include <cstdbool>
#include <cstring>

namespace CryptoPP
{
class AutoSeededRandomPool;
} /* namespace CryptoPP */

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

// C includes!
extern "C"
{
#include "../file_handle.h"
}

#include "bench.hpp";

int main(int argc, char* argv[])
{
	byte *key;
	rcry_generateKey(RCRY_RIJNDAEL, &key);

	string encoded;
	encoded.clear();
	StringSource(key, sizeof(key), true, new HexEncoder(new StringSink(encoded)) // HexEncoder
			);// StringSource
	cout << "key: " << encoded << endl;

	rcry_cryptoInit(RCRY_RIJNDAEL, &key);

	cout << "Init done!" << endl;

	//\\// RAW DATA FROM WRAPPER CODE \\//
	raw_datafile_t *rdf = (struct raw_datafile_t*) malloc(
			sizeof(struct raw_datafile_t));

	cout << "Step 1" << endl;

	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
	char *b =
			(char*) "{'1::12':{'passphrase':'qwertzuiop', 'username':'spacekookie'}, '2::15':{'passphrase':'BetterCarHorseBattery'}}";
	char *h =
			(char*) "{'name'=>'Lonely Robot', 'urls'=>['www.myface.com', 'mobile.myface.com'], 'tags'=>['social media', 'awesome', 'web login']}";
	char *mn = (char*) "Myface";
	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
	//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

	/** Calculate how much space we'll need */
	int bodyLength = strlen(b);
	int headLength = strlen(h);
	int metaNameLength = strlen(mn);

	/** Malloc enough memory for the data */
	char *body = (char*) malloc(bodyLength * sizeof(char));
	char *head = (char*) malloc(headLength * sizeof(char));
	char *metaName = (char*) malloc(metaNameLength * sizeof(char));

	/** Now copy over the values */
	strcpy(body, b);
	strcpy(head, h);
	strcpy(metaName, mn);

	// Now we update the pointers in our struct to work with.
	rdf->header = head;
	rdf->body = body;
	rdf->meta_name = metaName;

	cout << "The raw data file struct is now baked" << endl;

	reefile_t *my_file = (struct reefile_t*) malloc(sizeof(struct reefile_t));
	my_file->context = (struct ree_ccontext_t*) malloc(
			sizeof(struct ree_ccontext_t));
	// An empty context means that an IV needs to be created!

	cout << "Two more mallocs." << endl;

	rcry_setCryptoContext(my_file->context);

	cout << "Just set context! YAY!" << endl;

//	rcry_setCryptoContext(myFile->context);

	//

//	// This is our dummy file for disk handling
//	reefile_t *myFile = (struct reefile_t*) malloc(sizeof(struct reefile_t));
//	myFile->context =  malloc(
//			sizeof(struct ree_ccontext_t));
////
//	cout << "Step 3" << endl;
//
//	rcry_encryptInContext(myFile->head, &rdf->header);
//	rcry_encryptInContext(myFile->body, &rdf->body);

//	cout << "Step 4" << endl;
//
//	string buffer;
//
//	// make it to Base64 for printing!
//	rcry_toBase64Converter(myFile->head, &buffer, true);
//	rcry_toBase64Converter(myFile->body, &buffer, true);
//
//	cout << "Step 5" << endl;
//
//	rcry_cryptoStop();
//
//	cout << "Step 6" << endl;
//
//	cout << "Going down for a nap..." << endl;
//	return 0;
}
