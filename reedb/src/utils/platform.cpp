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
long reedb_platform_getos() {
#ifdef __linux__
    return RDB_PLATFORM_LINUX;
#elif __unix || __unix__
    return RDB_PLATFORM_UNIX;
#elif __FreeBSD__
    return RDB_PLATFORM_FREEBSD;
#endif
}

int reedb_platform_machine(char *(*buffer)) {
    char host[128];
    gethostname(host, 128);

    int host_len = strlen(host);
    *buffer = (char *) malloc(sizeof(char) * host_len);
    if(*buffer == NULL) {
        free((*buffer));
        return MALLOC_FAILED;
    }

    printf("Buffer size: %d", strlen(*buffer));

    memcpy((*buffer), host, host_len);
    return 0;
}

int reedb_platform_usrname(char *(*buffer)) {
    char b[128];
}