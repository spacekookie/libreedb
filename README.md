![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/extras/logo.png)

[![Coverity Status](https://scan.coverity.com/projects/6872/badge.svg)](https://scan.coverity.com/projects/reepass-reedb)
[![Build Status](https://travis-ci.org/reepass/libreedb.svg?branch=master)](https://travis-ci.org/reepass/libreedb)
[![Build Status](https://codeship.com/projects/414c23f0-fa36-0133-64c5-46c303037c21/status?branch=develop)](https://codeship.com/projects/414c23f0-fa36-0133-64c5-46c303037c21/status?branch=develop)

:lock: Fully encrypted, document-oriented database with per-user encryption, nested ciphers and zones to allow for selective sharing, rights management and even revocation of rights.

Build
-----

Compilation dependencies

`gcc cmake libgcrypt libmicrohttp libcrypto++ libboost`

An out-of-source build is recommended

```console
mkdir build && cd build/
cmake ..
cmake --build .
```

A static object of the library will be compiled as well as the reedbd-http extention.


How to use
----------

libreedb provides high level functions to interact with reedb databases. The following section will go into some examples, however do note that this is by no means a complete collection. For more information, please check the wiki.

**Reedb initialisation**

```C++
/* Create a Reedb context */
reedb *rdb = new reedb();
rdb->set_os(LINUX);
rdb->set_verbose(true);
rdb->finalise();

/* Create a vaults interface and attach it to the context */
rdb_vaults *v = new rdb_vaults();
rdb->register_vinterface(v);
```

**Creation and destruct operations**

```C++
vault_meta *vault = v->create("default", "~/Documents/", "foobar32");

/* Destruction of a vault */
v->destroy(vault->id, ...);
```

**Second stage user authentication**

```C++
rdb_token token = v->authenticate(vault->uuid, "foobar32");
```

**Data insertion and update**

The following code outlines data insertions. Via the C++ interface a map is used to transmit a key-value store into a dataset. Update functions work analogue to insert. The map can be cleared after the insert - values are copied out.

```C++
map<string, string> data = map<string, string>();
data["Field A"] = "foobar32"; // Ironically saving the vault pw in itself
data["Field B"] = "My Username";
v->insert(vault->id, &token, "My File", &data);
```

**Vault file meta updates**

You can adjust the meta fields available for filters in datafiles. This can be done via the meta migrate function.

```C++
map<string, string> metas = map<string, string>();
// ... Look into wiki for details
v->migrate_headers(&metas);
```

These were just some basic use cases for libreedb. For more details please check the documentation on the wiki.

Testing
-------

Yea, I should write some tests... :)

License
-------

This program is free software; you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
