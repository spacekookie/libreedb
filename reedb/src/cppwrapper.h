/**
 * This file wraps C++ into C-callable format. This way C files can still be compiled 
 * with C flags and not have to use features from C++ std=11 to break existing features
 * with older libraries.
 * 
 * This also allows for a C binding to the outside which makes it easier to write wrappers
 * for different languages.
 */

/* Include protectors */
#ifndef CPPWRAPPER_H
#define CPPWRAPPER_H

/* Then import requirements */
#include "reedb/defs.h"

/* Define our C-Section if we're compiling with C++ */
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* Expose the function prototype to the outside */
EXTERNC ree_err_t cpp_datafile_testing(); 

/* End our external Section */
#undef EXTERNC

ree_err_t cpp_datafile_testing();

#endif // CPPWRAPPER_H
