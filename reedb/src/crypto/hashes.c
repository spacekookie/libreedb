/* reedb - crypto/hashes.c
 *
 * Header file to use the hash functions implemented by libgcrypt and others
 * I don't know what else this is supposed to say. It's pretty damn fucking
 * self explanatory :) It makes dealing with shitty libraries less shitty.
 *
 * (c) 2015           Lonely Robot.
 * Authors:           Katharina 'spacekookie' Sabel
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 3 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-3.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * -------------------------------------------
 *
 */

#include <gcrypt.h>
#include "reedb/defs.h"

/* Used for file hashing */
ree_err_t rcry_hash_tiger2(unsigned char *word, unsigned char *(*hash), unsigned char *salt)
{
  /* Gather some length metadata */
  int msg_length = strlen(word) + strlen(salt);
  int hash_length = gcry_md_get_algo_dlen(GCRY_MD_TIGER2);

  /* Allocate memory for our tmp hash array */
  unsigned char *tmp = malloc(sizeof(unsigned char) + hash_length + strlen(salt));
  if(tmp == NULL) return MALLOC_FAILED;

  /* Concat the salt and the word to our message */
  unsigned char *message = rdb_concat_fname_simple(salt, word);
  if(message == NULL) return MALLOC_FAILED;

  /* This is needed to make the string formattable as pretty shit*/
  (*hash) = (char *) malloc(sizeof(char) * ((hash_length * 2) + 1));
  unsigned char *p = (*hash);

  if((*hash) == NULL) return MALLOC_FAILED;

  /* Call the actual hash function with our buffer */
  gcry_md_hash_buffer(GCRY_MD_TIGER2, tmp, message, msg_length);

  /* Do a preliminary cleanup */
  // if(message)  free(message);
  // if(word)       free(word);

  /* Loop through the binary string and make it human readable */
  int i; 
  for (i = 0; i < hash_length; i++, p += 2) 
    snprintf (p, 3, "%02x", tmp[i]);

  /* Clean up after ourselves again */
  // free(tmp);
  // p = NULL;

  /* Return for success */
  return SUCCESS;
}

/* Used for integrety checking */
ree_err_t rcry_hash_sha256(unsigned char *word, unsigned char *hash, unsigned int salt)
{
  /* Gather some length metadata */
  int msg_length = strlen(word) + strlen(salt);
  int hash_length = gcry_md_get_algo_dlen(GCRY_MD_TIGER2);

  /* Allocate memory for our tmp hash array */
  unsigned char *tmp = malloc(sizeof(char) + hash_length + strlen(salt));
  if(tmp == NULL) return MALLOC_FAILED;

  /* Concat the salt and the word to our message */
  unsigned char *message = rdb_concat_fname_simple(salt, word);
  if(message == NULL) return MALLOC_FAILED;

  /* This is needed to make the string formattable as pretty shit*/
  (*hash) = (char *) malloc(sizeof(char) * ((hash_length * 2) + sizeof(char)));
  unsigned char *p = (*hash);

  if((*hash) == NULL) return MALLOC_FAILED;

  /* Call the actual hash function with our buffer */
  gcry_md_hash_buffer(GCRY_MD_SHA256, tmp, word, msg_length);

  /* Do a preliminary cleanup */
  free(message);
  free(word);

  /* Loop through the binary string and make it human readable */
  int i; 
  for (i = 0; i < hash_length; i++, p += 2) 
    snprintf (p, 3, "%02x", tmp[i]);

  /* Clean up after ourselves again */
  p = NULL;

  /* Return for success */
  return SUCCESS;
}

/* Used for key padding */
ree_err_t rcry_hash_sphinx(unsigned char *word, unsigned char *hash, unsigned int salt)
{


  return SUCCESS;
}
