ReeDB
======

Spicy Ruby database. File based, local. Low dependencies.
Encryption from the core.

Proudly powering [Reepass](https://github.com/2RSoftworks/reepass) (and others).

* json, yaml and marshal files.
* AES, Twofish, Sha256 and Tiger2 security.

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
db = Reedb.vault(name='db_name', path='/my/path/').create
db.keygen('super_secure_password')

# Now add files to your db.
db.insert('cool entry').data('Reedb is awesome')
db.insert('custom object').data(my_custom_object_here)
'
db.sync.close # Force syncs data and closes the db.
```

Or load a vault from file
```ruby
db = Reedb.vault(path='/my/path/db_name').load('super_secure_password')
db.get('cool entry') #	=> Returns 'Reedb is awesome'
db.get('custom object') # => Returns my_custom_object_here

db.close # No force sync.
```

And much much more.

## Contribute
Like what you see? Want to help out? Great!
Reedb is published under the $FOSS_LICENSE_HERE_HAVENT_DECIDED_YET.
So feel free to fork the project, make improvements or changes and create a pull request.
