#include <reedb/platform.h>
#include <zconf.h>
#include <stdlib.h>
#include <reedb/reedb.h>


/** Get the current OS type which is supported */
unsigned int rdb_platform_getos()
{
#ifdef __linux__
    return RDB_PLATFORM_LINUX;
#elif __unix || __unix__
    return RDB_PLATFORM_UNIX;
#elif __FreeBSD__
    return RDB_PLATFORM_FREEBSD;
#endif
}

/** Get the local machine name */
const char *rdb_platform_getmachine()
{
    char host[128];
    gethostname(host, 128);

    char *hostname = (char*) malloc(REAL_STRLEN(host));
    strcpy(hostname, host);

    /* Return the pointer to the hostname */
    return hostname;
}

/** Get the current username */
const char *rdb_platform_getuser()
{
    char user[128];
    getlogin_r(user, 128);

    char *username = (char*) malloc(REAL_STRLEN(user));
    strcpy(username, user);

    /* Return the pointer to the username */
    return username;
}
