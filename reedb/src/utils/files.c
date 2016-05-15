#include "files.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

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
    // <SALT>::<IV>::<USER>::<ZONE>::<DATA>
    // Optional fields are either "{}" or "{0}"

    FILE *fp;
    fp = fopen(file_path, "w+");
    fprintf(fp, concat);
    fclose(fp);

    return 0;
}

int rdb_files_dfdump(const char *name, const char *path, char *data)
{
    printf("Writing datafile...");
    /* Expand our path, add the name as a folder and data dir */
    size_t path_size = strlen(path) + strlen(name);
    char file_path[sizeof(char) * path_size];

    strcpy(file_path, path);
    strcat(file_path, name);
    strcat(file_path, ".rdf");

    FILE *f = fopen(file_path, "w+");
    if (f == NULL) {
        printf("FAILED\n");
        return 55;
    }

    fprintf(f, data);
    fclose(f);
    printf("done\n");
}

void rdb_write_dataf(const char *data, const char *filename) {
    FILE *fp;

    char fn[strlen(filename) + 4];
    strcpy(fn, filename);
    strcat(fn, ".rdf");

    fp = fopen(fn, "w+");
    fprintf(fp, data);
    fclose(fp);
}

char *rdb_read_dataf(const char *filename) {
    char ch;

    char fn[strlen(filename) + 4];
    strcpy(fn, filename);
    strcat(fn, ".rdf");

    int fd = open(fn, O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);

    /* Read data via management memory space */
    char *data = (char *) mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
    return data;
}
