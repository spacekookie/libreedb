#include "files.h"

#include <stdio.h>
#include <string.h>

int rdb_files_crydump(char *salt, char *iv, char *data, char *vault_path, char *subcat, char *name, rdb_file_t type) {

    /* Concatinate the file path out of the path, the filename and the type ending */
    size_t path_size;
    char file_path[path_size * sizeof(char)];



    /* Concatinate the unique and data and seperate them via "::" that can be reparsed */
    size_t catsize = 2 + strlen(salt) + strlen(data) + strlen(iv);
    char concat[catsize * sizeof(char)];
    strcpy(concat, salt);
    strcat(concat, "::");
    strcat(concat, iv);
    strcat(concat, "::");
    strcat(concat, data);

    // Data format is as following
    // <SALT>::<IV>::<ZONE>::<USER>::<DATA>

    FILE *fp;
    fp = fopen(file_path, "w+");
    fprintf(fp, concat);
    fclose(fp);

    return 0;
}
