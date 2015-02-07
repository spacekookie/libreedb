#ReeDB
======

Spicy Ruby database. Json document based, low dependencies, fast and secure. Made for native app development. Protects information that is worth protecting with AES, Twofish and GnuPG/ PGP.

Proudly powering [Reepass](https://github.com/2RSoftworks/reepass) (and others).

* json, yaml and marshal files.
* AES, Twofish, OpenPGP, Sha256 and Tiger2 security.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'reedb'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install reedb

## Usage

```ruby
# Create a ReeDB vault by just giving it a name, specifying 
# a path, generating a key with a password and then calling
# "create". All these functions return themselves so 
# you can daisychain them into one line.

db = Reedb.vault(name='db_name', path='/my/path/')
vault.keygen('super_secure_password')
vault.create()

# Or create a datafile to hold thousands of fields.
vault.insert(Datafile.new(name='name', category='category', url='url'))

# Force syncs data and closes the db.
# Dumps the headers into an encrypted file for the vault to pick up next time.
vault.sync.close
```

Or load a vault from file
```ruby
vault = Reedb.vault(path='/my/path/db_name').load('super_secure_password')
vault.get('cool entry') #	=> Returns 'Reedb is awesome'
vault.get('custom object') # => Returns my_custom_object_here

vault.close # No force sync.
```

And much much more.

## Contribute

Like what you see? Want to help out? Great!
Reedb is published under the ```LGPL 3.0```. So feel free to fork the project, make improvements or changes and create a pull request. All further limitations please get from the license file included in this repository!

## HTTP Socket

Not developing in Ruby? Not to worry! If you make sure that ReeDB is installed on your target system you can use the Vaults with virtually any framework, any language from ANY platform.
Request data from a vault, store new files, don't worry about having to manage rights and permissions. ReeDB will do it for you.

*** HTTP specification to follow here ***