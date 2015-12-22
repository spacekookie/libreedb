#ifndef __UUID_H__
#define __UUID_H__

typedef enum uuid_type {
  ONE, TWO, THREE
} uuid_type;

typedef struct rdb_uuid {
  char id[32];
  uuid_type type;
} uuid;

#endif /* __UUID_H__ */
