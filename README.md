![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/extras/logo.png)

[![Coverity Status](https://scan.coverity.com/projects/6872/badge.svg)](https://scan.coverity.com/projects/reepass-reedb)
[![Build Status](https://travis-ci.org/reepass/reedb.svg?branch=master)](https://travis-ci.org/reepass/reedb)

:lock: Fully encrypted, document-oriented database with per-user encryption, nested ciphers and zones to allow for selective sharing, rights management and even revocation of rights.

Build
-----

Compilation dependencies

`gcc cmake libgcrypt libmicrohttp`

An out-of-source build is recommended

```console
mkdir build && cd build/
cmake ..
cmake --build .
```

A static object of the library will be compiled as well as the reedbd-http extention


How to use
----------

```C++
void main(int argc, char **args) {

    /* Create a reedb handler object */
    reedb *rdb = new reedb();
    rdb->set_os(LINUX);
    rdb->set_distro(FEDORA);
    rdb->set_verbose(true);
    rdb->finalise();

    /* Create a vault interface. Interfaces can handle a number of vaults */
    rdb_vaults *v = new rdb_vaults();
    rdb->register_vinterface(v);

    /* Create a new vault with name, location and default master passphrase */
    v->create("default", "~/Documents/", "foobar32");

    /* Shut it down */
    rdb->terminate();
    return 0;
}
```

More use cases of the native interface will be added. There is also a RESTful API available via reedbd-http.


License
-------

Reedb is licensed under the LGPL v3.0. It is free software and comes with no warranty of usefulness.
