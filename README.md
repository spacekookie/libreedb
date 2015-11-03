Reedb
=====

[![Coverity Status](https://scan.coverity.com/projects/6872/badge.svg)](https://scan.coverity.com/projects/reepass-reedb)

<!-- ![Reedb Logo](https://)
 [![NPM version](https://img.shields.io/npm/v/crypton-server.svg?style=flat)](https://npmjs.org/package/crypton-server)
[![Build Status](https://img.shields.io/travis/SpiderOak/crypton.svg?style=flat)](https://travis-ci.org/SpiderOak/crypton)
[![Test coverage](https://img.shields.io/coveralls/SpiderOak/crypton.svg?style=flat)](https://coveralls.io/r/SpiderOak/crypton?branch=master)
 -->

:lock: Reedb is a document oriented database that allows you to build applications that handle user data securely – with whatever language you want.

It does so by creating vaults where datafiles are encrypted before being written to disk and never cached for longer than needed to perform operations. It uses libgcrypt (which also powers software like GPG) and nested keys to even allow for multi-user access to vaults.

The issue tracker has migrated to the [Lonely Robot Redmine](https://bugs.lonelyrobot.io/projects/reedb/issues)

How to use
----------

Reedb is primarily a library that allows you to develop applications that deal with user information **safely**. In addition to that there are binary extentions that, for example, host a simple http server and use a REST interface to interact with vaults.

This allows you to use vaults from multiple applications at the same time (such as a desktop GUI and a browser plugin for a password manager).

More information will be provided as the newdawn branch becomes more...complete.

FAQ
---

Here are some questions that we get asked frequently.

---

**Q: How does Reedb work?**

A: Reedb takes information (usually json), encrypts it and writes it to a file in a vault. At it's core, that's it. In addition to that it also provides interfaces to get the data back ( which is sometimes non-optional ) and do data integrety checking as well as data versioning and merge conflict resolution (the way git does just a *lot* simpler)

---

**Q: What kind of database is Reedb?**

A: Reedb is a file-based database that is addressed by a UUID by applications but can be found under a name and path on the filesystem. (Vaults are folder that have a .reevault suffix). This way it makes integration into other services easy and convenient.

---

**Q: What encryption does Reedb use?**

A: Primarily Reedb uses the 256-bit version of Rijndael but can easily be setup to use other ciphers (e.g. Twofish or Serpent). The encryption is handled by the GCrypt library which provides the backend to the GnuPG. Asymmetric encryption (RSA) is in the planning stage.

Integrety checking and file control is done with SHA-256 hashes and filenames are salted 192-bit Tiger hashes.
When using Reedb via the HTTP interface HTTPs keys and certificates are generated by the 'server'.

---
**Q: Shouldn't it be camel-case?**

A: No. Reedb is written without camel case. Because the combination of the letters 'd' and 'b' looks too awesome. 'db' => Reedb. End of discussion.

---
**Q: I want to contribute to Reedb**

A. Great. Check out the wiki first to really understand what Reedb does, maybe poke around the issue tracker for ideas of what is currently still missing. Fork this repo, implement and then create a merge-request. So far all of them have been accepted :)


[Reepass]: https://github.com/tr-lonelyrobot/reepass
[here]: https://github.com/tr-lonelyrobot/reedb/wiki/binaries
[wiki]: https://github.com/tr-lonelyrobot/reedb/wiki
