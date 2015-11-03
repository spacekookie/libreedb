![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/logo.png)

[![Coverity Status](https://scan.coverity.com/projects/6872/badge.svg)](https://scan.coverity.com/projects/reepass-reedb)
[![Build Status](https://travis-ci.org/reepass/reedb.svg?branch=master)](https://travis-ci.org/reepass/reedb)

:lock: Reedb is a document oriented database that allows you to build applications that handle user data securely – with whatever language you want.

It does so by encrypting all data that is stored in a database folder (callded vault) with nested symmetric ciphers provided by libgcrypt (which also powers software like GPG) which allows for multi-user access and selective sharing.

**The library is not currently deemed stable enough to use in production!**

The issue tracker has migrated to the [Lonely Robot Redmine](https://bugs.lonelyrobot.io/projects/reedb/issues).
Please also note that the Wiki is horribly out-of-date and should generally be ignored. A new one will be written soon-ish.

Build
----------

The only way to work with Reedb at the moment is to build it from source. Clone this repository for an up-to-date copy of the codebase.

Compilation dependencies
`gcc cmake libgcrypt libmicrohttp`

An out-of-source build is recommended

```bash
mkdir build && cd build/
cmake ..
cmake --build .
```

A static object of the library will be compiled as well as the `reedbd-http` extention

Install
-------

Installing Reedb to the root filesystem is currently not recommended.
Please don't do it! Thanks.

License
-------

Reedb is licensed under the LGPL v3.0. It is free software and comes with no warranty of usefulness.
