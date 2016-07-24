//
// Created by spacekookie on 23/07/16.
//

#ifndef REEDB_PLATFORM_H
#define REEDB_PLATFORM_H


//#ifdef _WIN32
//return "Windows 32-bit";
//#elif _WIN64
//return "Windows 64-bit";
//#elif __unix || __unix__
//return "Unix";
//#elif __APPLE__ || __MACH__
//return "Mac OSX";
//    #elif __linux__
//    return "Linux";
//    #elif __FreeBSD__
//    return "FreeBSD";
//    #else
//return "foo"

#define RDB_PLATFORM_LINUX      0x0
#define RDB_PLATFORM_FREEBSD    0x1
#define RDB_PLATFORM_UNIX       0x2
#define RDB_PLATFORM_OSX        0x3

long rdb_utls_get_os();

#endif //REEDB_PLATFORM_H
