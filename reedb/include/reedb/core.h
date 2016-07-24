#ifndef REEDB_H
#define REEDB_H

#include <string>
#include <list>
#include <map>

/* Some utilities */
extern "C" {
#include "reedb/utils/helper.h"
}

#include <reedb/reedb.h>
#include <reedb/vaults.h>

using namespace std;

/** Core handler class */
class reedb {
public:

    /* Public constructor, destructor and equals operator overloading */
    reedb();

    bool operator==(const reedb &other) const;

    /* Register a vaults interface with this reedb instance. There can be several vaults handlers
     * active at the same time with dfferent rule sets */
    void register_vinterface(rdb_vaults *interface);

    /* Finalise creation and init reedb */
    void finalise();

    /* Terminate the current instance of reedb gracefully */
    void terminate();

    /* Return if this instance is ready to use */
    bool isReady();

private:

    /* Keep the destructor private so we are in control over resources */
    ~reedb();

    /* Where is the master config located */

    /* Rdb vault interfaces that are bound to this reedb instance */
    list<rdb_vaults *> vault_interfaces;

    /* Publicly set parameters for initialisation*/
SETTER(ree_os, os)

SETTER(ree_distro, distro)

SETTER(bool, daemon)

SETTER(bool, verbose)

SETTER(bool, override)

    /* Indicate whether or not this instance of reedb is ready to be used */
    bool finalised;
};

#endif // REEDB_H
