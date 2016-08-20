#ifndef RDB_ENCODING_H
#define RDB_ENCODING_H

/* Also make sure we're _always_ interpreted as a C file */
#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
 *
 * BASE64 ENCODERS
 *
 **********************************************************************/

/**
 *
 * @param encoded
 * @param plain_src
 * @param plain_len
 * @return
 */
int rdb_coding_base64_encode(char *(*encoded), const char *plain_src, int plain_len);

int rdb_coding_base64_decode(char *(*decoded), const char *enc_src, int enc_len);

int rdb_coding_base64enclen(int length);

int rdb_coding_base64enc(char *coded_dst, const char *plain_src, int len_plain_src);

int rdb_coding_base64declen(const char *coded_src);

int rdb_coding_base64dec(char *plain_dst, const char *coded_src);

/**********************************************************************
 *
 * HEX ENCODERS
 *
 **********************************************************************/

int rdb_coding_hexenc(char *(*encoded), const char *plain_src, int src_len);

int rdb_coding_hexdec(char *(*decoded), const char *hex_src, int src_len);

#ifdef __cplusplus
}
#endif
#endif // RDB_ENCODING_H
