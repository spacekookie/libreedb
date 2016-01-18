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

private:
  uuid_type type;
  char id[32];
  
public:
  rdb_uuid *generate();
  rdb_uuid *generate(uuid_type type);
  
};

// typedef enum uuid_type {
//   ONE, TWO, THREE
// } uuid_type;
// 
// typedef struct rdb_uuid {
//   char id[32];
//   uuid_type type;
// } rdb_uuid;
// 
// /** Generates a standard UUID */
// rdb_uuid rdb_generate_uuid();
// 
// /* */
// rdb_uuid *rdb_generate_uuid_with_type(uuid_type type);

#endif /* __UUID_H__ */
