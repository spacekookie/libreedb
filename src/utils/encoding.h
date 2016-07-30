#ifndef RDB_ENCODING_H
#define RDB_ENCODING_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

int rdb_coding_base64enclen(int length);

int rdb_coding_base64enc(char *coded_dst, const char *plain_src, int len_plain_src);

int rdb_coding_base64declen(const char *coded_src);

int rdb_coding_base64dec(char *plain_dst, const char *coded_src);

#ifdef __cplusplus
}
#endif
#endif // RDB_ENCODING_H
