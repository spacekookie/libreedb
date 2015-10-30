Reedb naming conventions
========================

There are a lot of variables and interfaces in this Library.
To that end this is supposed to be a naming convention guide to make things more..."clear".

### `rdb_` is a prefix used for outside functions and variables. Namely:

 - `rdb_vault_create`			Used to create vaults
 - `RDB_DEBUG`						Debug macro
 - `rdb_set_os`						Function to set the OS (To be replaced with macros...maybe)

### The second prefix is `ree` is used only for types:

 - `ree_os`				OS enum
 - `ree_err_t` 		Error enum
 - `ree_vault`		Simple vault handle

### Internal datastructures are just named after their type.

 - `vault`
 - `datafile`
 - `cry_context`
 - etc.

### Exceptions

`reedb_c`						The master container
`reedb_init`				Core init
`reedb_terminate`		Core termination

---

Peace.