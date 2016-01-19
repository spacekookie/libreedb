/* File of helper functions and definitions */

#include "reedb/utils/helper.h"
#include <wordexp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ree_distro rdb_find_distro()
{
  return FEDORA;
}

ree_os rdb_find_os()
{
  return LINUX;
}

char *rdb_expand_path(const char *path) {
  wordexp_t exp_result;
  wordexp(path, &exp_result, 0);
  
  char *result = malloc(sizeof(char) * sizeof(exp_result));
  
  
  
  // memcpy(exp_result.we_wordc, result, sizeof(exp_result));
  
  return result;
}
