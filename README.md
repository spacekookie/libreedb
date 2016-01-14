![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/extras/logo.png)

[![Coverity Status](https://scan.coverity.com/projects/6872/badge.svg)](https://scan.coverity.com/projects/reepass-reedb)
[![Build Status](https://travis-ci.org/reepass/reedb.svg?branch=master)](https://travis-ci.org/reepass/reedb)

:lock: Reedb is a fully encrypted document-oriented database that allows you to build applications that handle data securely - and a load of extentions to go with it...

All data stored in a reedb database (also called a 'vault') is encrypted with symmetric ciphers (see [roadmap](wiki/roadmap)) provided by [crypto++](http://cryptopp.com/). Nesting the ciphers to create zones and groups allows for selective sharing and access management for new revisions of a file.

*Please note that the library (and management daemon) is currently not deemed stable enough to use in production*

If you want to contribute to the project, maybe head over to the wiki and have a look at the roadmap


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
