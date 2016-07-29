![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/extras/logo.png)

:lock: Fully encrypted, document-table-hybrid oriented database with per-user encryption, nested ciphers, groups and zones for selective sharing, rights management and revocation of privilages. Encrypt single datasets, multisets or even filesystem nodes via FUSE.

Build
-----

libreedb is built on a few libraries. Building has been tested on Fedora 24 and Arch Linux. If you have issues compiling the library, please open an issue for it.

`gcc cmake libgcrypt libapr libgdsl protobuf-c`

An out-of-source build is recommended

```console
mkdir build && cd build/
cmake ..
cmake --build .
```

By default libreeedb builds a static object. This behaviour can be changed by providing `--shared` as a parameter to the cmake configuration.