#include <datastore/encoding.h>
#include <string.h>

static const unsigned char asciilk[256] =
        {
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
                64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
                64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
                64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
        };

static const char base_64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


int rdb_coding_base64declen(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (asciilk[*(bufin++)] <= 63);

    nprbytes = (int) (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int rdb_coding_base64dec(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (asciilk[*(bufin++)] <= 63);
    nprbytes = (int) (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
        *(bufout++) =
                (unsigned char) (asciilk[*bufin] << 2 | asciilk[bufin[1]] >> 4);
        *(bufout++) =
                (unsigned char) (asciilk[bufin[1]] << 4 | asciilk[bufin[2]] >> 2);
        *(bufout++) =
                (unsigned char) (asciilk[bufin[2]] << 6 | asciilk[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
    if (nprbytes > 1) {
        *(bufout++) =
                (unsigned char) (asciilk[*bufin] << 2 | asciilk[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
                (unsigned char) (asciilk[bufin[1]] << 4 | asciilk[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
                (unsigned char) (asciilk[bufin[2]] << 6 | asciilk[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

int rdb_coding_base64enclen(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int rdb_coding_base64enc(char *encoded, const char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
        *p++ = base_64_map[(string[i] >> 2) & 0x3F];
        *p++ = base_64_map[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        *p++ = base_64_map[((string[i + 1] & 0xF) << 2) |
                        ((int) (string[i + 2] & 0xC0) >> 6)];
        *p++ = base_64_map[string[i + 2] & 0x3F];
    }
    if (i < len) {
        *p++ = base_64_map[(string[i] >> 2) & 0x3F];
        if (i == (len - 1)) {
            *p++ = base_64_map[((string[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else {
            *p++ = base_64_map[((string[i] & 0x3) << 4) |
                            ((int) (string[i + 1] & 0xF0) >> 4)];
            *p++ = base_64_map[((string[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}


/*************************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Based on: http://code.google.com/p/bitcoinj/source/browse/core/src/main/java/com/google/bitcoin/core/Base58.java
*/


static const char *ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
static unsigned char INDEXES[128] = { -1 };

unsigned char * getIndexes(){
    int i;

    for (i = 0; i < 58; i++)
        INDEXES[(int)ALPHABET[i]] = i;

    return INDEXES;
}

unsigned char divmod58(unsigned char *in, int inLen, int i){
    int rem = 0;
    for(;i<inLen; i++){
        rem = rem * 256 + in[i];
        in[i] = rem / 58;
        rem = rem % 58;
    }
    return rem & 0xFF;
}

unsigned char divmod256(unsigned char *in, int inLen, int i){
    int rem = 0;
    for(;i<inLen; i++){
        rem = rem * 58 + in[i];
        in[i] = rem / 256;
        rem = rem % 256;
    }
    return rem & 0xFF;
}

unsigned char * NBase58Encode(unsigned char *in, int inLen, int *outLen){
    if(inLen == 0)
        return NULL;

    unsigned char *inCopy = malloc(inLen);
    memcpy(inCopy, in, inLen);

    //count leading zeros
    int z = -1;
    while(z < inLen && inCopy[++z] == 0x00)
        ;

    int j = inLen * 2;
    int inLen_x2 = j;
    unsigned char *temp = malloc(inLen_x2);

    //skip leading zeros and encode from startAt
    int startAt = z;
    while(startAt < inLen){
        unsigned char mod = divmod58(inCopy, inLen, startAt);
        if(inCopy[startAt] == 0)
            ++startAt;

        temp[--j] = ALPHABET[mod];
    }

    free(inCopy);

    while(j<inLen_x2 && temp[j] == '1')		j++;

    while(--z >= 0)
        temp[--j] = '1';


    *outLen = inLen_x2 - j;

    int len = inLen_x2 - j;

    unsigned char *out = malloc(len + 1);
    out[len] = 0;
    memcpy(out, temp + j, len);
    free(temp);

    return out;
}

unsigned char * NBase58Decode(unsigned char *input, int inLen){
    if(inLen == 0)
        return NULL;

    unsigned char *input58 = malloc(inLen);
    unsigned char *indexes = getIndexes();

    int i=0;
    for(; i<inLen; i++){
        input58[i] = indexes[input[i]];
    }

    //count leading zeros
    int z = -1;
    while(z<inLen && input58[++z] == 0x00)
        ;

    unsigned char *temp = malloc(inLen);
    int j = inLen;

    int startAt = z;
    while(startAt < inLen){
        char mod = divmod256(input58, inLen, startAt);
        if(input58[startAt] == 0)
            ++startAt;

        temp[--j] = mod;
    }

    free(input58);

    while(j<inLen && temp[j] == 0)		j++;

    int len = inLen - j + z;
    unsigned char *out = malloc(len + 1);
    out[len] = 0;
    memcpy(out, temp + j - z, len);
    free(temp);

    return out;
}
