/* inclusion guard */
#ifndef __RDB_UTILS_H__
#define __RDB_UTILS_H__

/** Auto ACCESS, GETTER and SETTER macros to generate functions on the fly*/
#define ACCESS(type, var) \
private: \
   type var; \
public: \
   type get_##var() \
   {\
      return this->var; \
   }\
   void set_##var(type val) \
   {\
      this->var = val; \
   }

#define GETTER(type, var) \
private: \
   type var; \
public: \
   type get_##var() \
   {\
      return this->var; \
   }

#define SETTER(type, var) \
private: \
   type var; \
public: \
   void set_##var(type val) \
   {\
      this->var = val; \
   }

/** Define what OS Reedb is running on */
typedef enum ree_os {
  LINUX     = 0xE1, // Most Linux distributions
  OSX       = 0xE2, // Mac OS X
  WINDOWS   = 0xE3, // Windows
  ANDROID   = 0xE4, // Mobile: Android
  IOS       = 0xE5, // Mobile: iOS
  BSD       = 0xE6, // BSD
} ree_os;

typedef enum ree_distro {
  FEDORA,
  DEBIAN
} ree_distro;

/* 
 * Helper function that determines the specific distribution of Linux
 * that is running on the current machine. Applies certain path and 
 * permission changes to avoid errors for very specific functions.
 * 
 * @return ree_distro
 */
ree_distro rdb_find_distro();

/*
 * Helper function that determines the general operating system running
 * on the current machine. This is used to determine file paths, permissions
 * and specific implementations for things.
 */ 
ree_os rdb_find_os();

#endif /* __RDB_UTILS_H__ */
