/*
 * utils.h
 *
 *  Created on: 7 Aug 2015
 *      Author: spacekookie
 */

#ifndef SRC_CRYPTO_UTILS_H_
#define SRC_CRYPTO_UTILS_H_

typedef enum ree_crypto_t {
	RIJDAEL = 0xB0, // Default setting
	TWOFISH = 0xB1,
	RSA = 0xB2,
	RIJ_TWO_MULTI = 0xB3, // Experimental
} ree_crypto_t;

#endif /* SRC_CRYPTO_UTILS_H_ */
