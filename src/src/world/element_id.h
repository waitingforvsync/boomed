#ifndef WORLD_ELEMENT_ID_H_
#define WORLD_ELEMENT_ID_H_

#include <stdint.h>

typedef uint16_t element_id_t;

// Define element_id_t array type
#define TEMPLATE_NAME element_ids
#define TEMPLATE_TYPE element_id_t
#include "templates/array.h.template"

#define ID_NONE (0xFFFFU)


#endif // ifndef WORLD_ELEMENT_ID_H_
