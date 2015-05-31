Reedb
=====

:lock: Spicy and secure Ruby file-based database.

Reedb is a system daemon that brings its own libraries and manages databases (Reevaults) fast and securely.
Made for native app development, to protect sensitive information that are worth protecting with AES and Twofish.

Proudly powering [Reepass] - (and others).


Installation
------------

ReeDB is a system daemon that developers can talk to via a JSON powered RESTful API. This interface allows access management, remote vault management and easy integration into projects of different languages. As long as the ReeDB daemon is installed on the system your application can use secure vaults. (See details below)

End-users packages and binary installers are being provided (check the wiki [here])

Usage
-----
In order to develop applications that use ReeDB 

In order to develop applications that use ReeDB you need to register your application with the vault daemon and get an authentication token in order to insert or get data from a secure vault.

Alternatively (if you develop in Ruby) you can use Reedb as a gem dependency. This way you can run your own instance of Reedb in a custom directory with a lot more control over what the daemon does in the background.

To find out how to use Reedb in either of those ways, please check the [wiki] for details.


FAQ
---

Here are some questions that we get asked frequently.

---

**Q: How does Reedb work?**

A: Reedb is a very complex daemon. It allows you to securely write information in databases from your own applications without having to worry about where it is exactly written, how it is serialised and how it is kept secure.

---

**Q: What kind of database is Reedb?**

A: Reedb is a file-based database that is addressed by the daemon with a UUID and found under a name and path on the filesystem. This way it makes integration into other services easy and convenient.

---

**Q: What encryption does Reedb use?**

A: Reedb uses 256-bit AES (less commonly known as Rijndael) by default but can be setup very easily (with one parameter) to use the 256-bit Twofish, a runnerup of the AES contenst.
For integrety checking it uses SHA-256 hashes of files and filenames are created with the 192-bit Tiger2 hashing algorithm.

When communicating to Reedb via HTTP the weak link is of course the connection. A feature to add RSA encryption as a transport-layer security is planned, however not currently implemented. When dealing with Reedb via a Gem dependency this problem is less promiment.

---
**Q: Shouldn't it be camel-case?**

A: No. Reedb is written without camel case. Because the combination of the letters 'd' and 'b' looks too awesome. 'db' => Reedb. End of discussion.

---
**Q: I want to contribute to Reedb**

A. Great. Check out the wiki first to really understand what Reedb does, maybe poke around the issue tracker for ideas of what is currently still missing. Fork this repo, implement and then create a merge-request. So far all of them have been accepted :)


[Reepass]: https://github.com/2RSoftworks/reepass
[here]: https://github.com/tr-lonelyrobot/reedb/wiki/binaries
[wiki]: https://github.com/tr-lonelyrobot/reedb/wiki