Reedb
===

Spicy and secure Ruby file-based database.

Reedb is a system daemon that brings it's own libraries, manages databases (Reevaults) fast and securely.
Made for native app development. To protect sensitive information that's worth protecting with AES and Twofish.

Proudly powering [Reepass](https://github.com/2RSoftworks/reepass) (and others).

## Installation

Reedb is a system daemon that you (as a developer) can talk to via HTTP and JSON. This interface allows for access management, remote vault management and easy integration into projects of different languages. As long as the Reedb daemon is installed on the system your application can use secure vaults. (See details below)

For end-users packages and binary installers are being provided HERE.

## Network interface
In order to develop applications that use Reedb you need to register your application with the vault daemon and get an authentication token in order to insert or get data from a secure vault.