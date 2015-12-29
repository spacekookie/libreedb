#ifndef __UUID_H__
#define __UUID_H__

typedef enum uuid_type {
  ONE, TWO, THREE
} uuid_type;

typedef struct rdb_uuid {
  char id[32];
  uuid_type type;
} rdb_uuid;

rdb_uuid generate_uuid();

rdb_uuid generate_uuid_with_type(uuid_type type);

#endif /* __UUID_H__ */
