#ifndef __UUID_H__
#define __UUID_H__

typedef enum uuid_type {
    ONE, TWO, THREE
} uuid_type;

typedef struct rdb_uuid {
    char id[32];
    uuid_type type;
} rdb_uuid;

class uuid_helper {

public:

    /* C like function you should use for UUIDs */
    static int rdb_uuid_generate(rdb_uuid *(*uuid), uuid_type type);

};

#endif /* __UUID_H__ */
