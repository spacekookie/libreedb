ReeDB
=====

Spicy and secure Ruby file-based database.

ReeDB is a system daemon that brings its own libraries and manages databases (Reevaults) fast and securely.
Made for native app development, to protect sensitive information that are worth protecting with AES and Twofish.

Proudly powering [Reepass] (and others).


Installation
------------

ReeDB is a system daemon that developers can talk to via a JSON powered RESTful API. This interface allows access management, remote vault management and easy integration into projects of different languages. As long as the ReeDB daemon is installed on the system your application can use secure vaults. (See details below)

For end-users packages and binary installers are being provided.


Network interface
-----------------

In order to develop applications that use ReeDB you need to register your application with the vault daemon and get an authentication token in order to insert or get data from a secure vault.


[Reepass]: https://github.com/2RSoftworks/reepass
