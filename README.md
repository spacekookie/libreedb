![Reedb Logo](https://raw.githubusercontent.com/reepass/reedb/develop/extras/logo.png)

:lock: Fully encrypted, document-table-hybrid oriented database with per-user encryption, nested ciphers, groups and zones for selective sharing, rights management and revocation of privilages. Encrypt single datasets, multisets or even filesystem nodes via FUSE.

Bugs are being tracked on this github repository - if you find issues, please open an issue. Exact progress can be tracked on the {Lonely Robot wekan](https://plan.lonelyrobot.io/b/WEDvddNHrxBzmqCLs/reedb#)

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


How to use
----------

To start using reedb you need to create a context. You can either do so on the stack or allocate memory by yourself and pass the pointer instead.

```
rdb_err_t err;
rdb_context ctx;
err = rdb_ctx_init(&ctx);
```

Afterwards the context is available for further configuration or to create a new vault.

```

rdb_vault vault;
err = rdb_ctx_vaultcreate(&ctx, &vault, "default", "~/Documents/");
// Check for errors

rdb_uuid user = rdb_vlts_adduser(vault, "spacekookie", 0);
err = rdb_vlts_setlogin(vault, &user, "mega_passphrase!");
// Check for errors

```

When creating a vault flags need to be provided to control it's behaviour. Most flags are covered under default values, however there is one thing you need to set before you can start using your vault: the type.

The vault type very profoundly changes the way the vault will behave and is thus required to be explicitly defined. An implicitly chosen behaviour might endanger data in your usecase. The following vault types are available:

 - ROOT (The vault is accessable and managed by only one "root" user)
 - MANAGED (The vault has multi-user access however still has an administrator with super-access)
 - MULTI USER (The vault has mutliple users with no administrator but allows for zone management (aka hirarchy))
 - ANARCHY (The vault has multiple users with no administrator. There is NO hirarchy)

You can then bitwise OR together whatever flags you want:

```
err = rdb_vlts_setflags(vault, RDB_FLG_ROOT | ...);
```

More documentation to follow :)