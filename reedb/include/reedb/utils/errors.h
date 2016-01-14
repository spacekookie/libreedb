#include <stdexcept>
#include <string>

extern "C" {
#include "reedb/utils/helper.h"
#include "reedb/defs.h"
}

/**
 * Errors are important for debugging Reedb and understand what
 * is actually going on under the hood. Thus, several errors are
 * implemented extending the default std::runtime_error.
 * 
 * Each error is thrown and must be handled by a catch-block. 
 * Some catch blocks throw new errors that are passed onto the 
 * user (programmer in this case). 
 * 
 * Please also consult the error-code list in {reedb/defs.h} for 
 * error codes that can be used when returning more data for debugging.
 * 
 * Good luck :)
 */

/** Generic error to throw if no other type fits */
class gen_rdberror : public std::runtime_error {
  gen_rdberror(std::string msg, ree_err_t c) : runtime_error(msg), code(c){};
  
private:
  GETTER(ree_err_t, code);
};

/** Error that can be throw when crypto operations fail */
class crypto_error : public std::runtime_error {
  crypto_error(std::string msg, ree_err_t c) : runtime_error(msg), code(c){};
  
  private:
  GETTER(ree_err_t, code);
};

/** Error that can be thrown when dealing with writing data to disk or parsing files */
class storage_error : public std::runtime_error {
  storage_error(std::string msg, ree_err_t c) : runtime_error(msg), code(c){};
  
  private:
  GETTER(ree_err_t, code);
};

/** An error that can be thrown when reading, parsing or updating the core Reedb config */
class config_error : public std::runtime_error {
  config_error(std::string msg, ree_err_t c) : runtime_error(msg), code(c){};
  
  private:
  GETTER(ree_err_t, code);
};