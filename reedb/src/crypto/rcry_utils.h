#ifndef SRC_RCRY_UTILS_H
#define SRC_RCRY_UTILS_H

#include <string>

/**
 * A class with static members of utility functions used for
 * cryptographic procedures.
 */
class rcry_utils {

public:

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
    char *insecure_tiger2_hash(char *input, bool clear);



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

    static char *generate_random(unsigned int length, bool clear);
    static void generate_super_rand(char *(*data), unsigned int length);
    static void generate_normal_rand(char *(*data), unsigned int length);
    static char *generate_minitoken();

    static char *md_tiger2_salted(char *salt, const char *message, bool clear);
    static char *md_sha256_salted(char *salt, const char *message, bool clear);
    static char *md_blake_salted(char *salt, const char *message, bool clear);

    static char *md_tiger2(const char *message, bool clear);
    static char *md_sha256(const char *message, bool clear);
    static char *md_blake(const char *message, bool clear);
};

#endif /* SRC_RCRY_UTILS_H */
