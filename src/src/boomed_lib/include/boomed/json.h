#ifndef BOOMED_JSON_H_
#define BOOMED_JSON_H_

#include <stdint.h>
#include <stdlib.h>
#include "boomed/reflect.h"

typedef struct arena_t arena_t;
typedef struct str_t str_t;


str_t json_serialize(const void *data, reflect_desc_t desc, arena_t *arena);
uint32_t json_deserialize(str_t text, void *data, reflect_desc_t desc);


#endif // ifndef BOOMED_JSON_H_
