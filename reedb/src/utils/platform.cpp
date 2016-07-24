#include <reedb/platform.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include <reedb/errors.h>
#include <printf.h>

/**
 * A simple utility function that returns the type of OS in a
 * nicely controlled long flag.
 *
 * @return long flag from platform.h
 */
long rdb_platform::get_os() {
#ifdef __linux__
    return RDB_PLATFORM_LINUX;
#elif __unix || __unix__
    return RDB_PLATFORM_UNIX;
#elif __FreeBSD__
    return RDB_PLATFORM_FREEBSD;
#endif
}

string rdb_platform::get_machine() {
    char host[128];
    gethostname(host, 128);

    string s = string(host);
    return s;
}

string rdb_platform::get_username() {
    char b[128];
}