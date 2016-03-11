#ifndef SRC_RCRY_UTILS_H
#define SRC_RCRY_UTILS_H

#include <string>

class rcry_utils {

public:

    /**
    * Generate random numbers with AutoSeededX917RNG pool.
    * Takes a length as parameter and mallocs a string on
    * secmem
    *
    * @param length: The char-length of the required random string
    */
    char *generate_random(unsigned int length);

    /** Generates something close to a hash-id */
    char *generate_minitoken();

    /**
     * Takes an input and a salt which should be cryptographically
     * strong randomness. This makes it impossible (or very very hard)
     * to compute rainbow-tables to guess a passphrase from the password
     * hash, even if it is possible to leak it somehow.
     *
     * @param salt: A cryptographically unique salt
     * @param input: The input that should be hashed...tiger style!
     */
    char *salted_tiger2_hash(char *salt, std::string *input);

    /**
     * Takes an input and hashes it...tiger style. Doesn't apply a salt.
     * Do NOT use in any cryptographic operations. This function is intended to
     * be used for filename hashing or validations.
     *
     * @param input: The input that should be hashed
     */
    char *insecure_tiger2_hash(std::string *input);

    /**
     * Takes an input and a salt which should be cryptographically
     * strong randomness. This makes it impossible (or very very hard)
     * to compute rainbow-tables to guess a passphrase from the password
     * hash, even if it is possible to leak it somehow.
     *
     * @param salt: A cryptographically unique salt
     * @param input: The input that should be hashed
     */
    char *salted_sha256_hash(char *salt, std::string *input);

    /**
     * Takes an input and hashes it...NSA style. Doesn't apply a salt.
     * Do NOT use in any cryptographic operations. This function is intended to
     * be used for filename hashing or validations.
     *
     * @param input: The input that should be hashed
     */
    std::string *insecure_sha1_hash(std::string *input);
};

#endif /* SRC_RCRY_UTILS_H */
