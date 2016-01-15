# Crypto layout

The crypto engine for Reedb works by holding a crypto context. That context is stored in secure RAM (elevated mem space).

With an active context very high level crypto functions can be invoked. The following are absolutely neccesary:

 - Generate master key (vault)
 - Generate zone key (vault, zone)
 - Encrypt file (entire file)
 - Decrypt file (entire file)
 - init (vault)
 - close (vault)

The init function should generate some entropy (depending on the master reedb config) to prevent multiple operations that require /dev/urandom to loose secureness by "not being random enough" :p

The crypto engine has a queue of tasks it has to go through. These tasks are basically mapped to engine functions. So

<"SWITCH", this> would tell the crypto engine to switch into the apropriate context to do crypto. When writing things into the queue it is locked to prevent race conditions.

Example:

Two users need to do crypto:

 - A switch context
 - A encrypt file

 - B switch context
 - B encrypt file

--> switch A --> switch B --> encrypt A (ERROR)

While Reedb at this time is not fully multi-threaded yet (only if using a multithreaded hook system - partially) the engine should be designed in a way that it can handle a lot of threads.

Future planning:

Crypto load balancer where multiple instances if the crypto engine (on several KLTs) get given crypto tasks that can be performed at the same time without inverfering with each othtell the crypto engine to switch into the apropriate context to do crypto. When writing things into the queue it is locked to prevent race conditions.

Example:

Two users need to do crypto:

 - A switch context
 - A encrypt file

 - B switch context
 - B encrypt file

--> switch A --> switch B --> encrypt A (ERROR)

While Reedb at this time is not fully multi-threaded yet (only if using a multithreaded hook system - partially) the engine should be designed in a way that it can handle a lot of threads.

Future planning:

Crypto load balancer where multiple instances if the crypto engine (on several KLTs) get given crypto tasks that can be performed at the same time without inverfering with each other..
