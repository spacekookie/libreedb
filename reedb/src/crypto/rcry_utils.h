#ifndef SRC_RCRY_UTILS_H
#define SRC_RCRY_UTILS_H

class rcry_utils {
public:
  
  /**
   * Generate random numbers with AutoSeededX917RNG pool.
   * Takes a length as parameter and mallocs a string on 
   * secmem
   * 
   * @param length: The char-length of the required random string
   */
  static char *generate_random(unsigned int length);
  
  /** Generates something close to a hash-id */
  static char *generate_minitoken();
};

#endif /* SRC_RCRY_UTILS_H */
