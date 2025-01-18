#ifndef BOOMED_REFLECT_H_
#define BOOMED_REFLECT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum reflect_type_t {
    reflect_type_none,
    reflect_type_uint8_t,
    reflect_type_uint16_t,
    reflect_type_int32_t
};


typedef struct reflect_desc_t reflect_desc_t;
typedef struct reflect_member_desc_t reflect_member_desc_t;

struct reflect_desc_t {
    const reflect_member_desc_t *member_descs;
    uint32_t num_member_descs;
};

struct reflect_member_desc_t {
    const char *name;
    reflect_desc_t desc;
    uint32_t desc_size;
    uint32_t num_elements;
    uint32_t offset;
    uint32_t element_size;
    uint32_t type;
};



static inline bool reflect_member_is_array(const reflect_member_desc_t *desc) {
    return desc->num_elements == 0;
}


#define REFLECT_DESC_NAME(name) reflect_desc_##name

#define REFLECT_DESC_SIZE(name) \
    ((uint32_t)(sizeof REFLECT_DESC_NAME(name) / sizeof(reflect_member_desc_t)))

#define REFLECT_DESC(name) ((reflect_desc_t){REFLECT_DESC_NAME(name), REFLECT_DESC_SIZE(name)})

#define REFLECT_SIZEOF_FIELD(struct_name, field_name) \
    ((uint32_t)sizeof(((struct_name *)0)->field_name))

#define REFLECT_SIZEOF_ELEMENT_TYPE(type) \
    ((uint32_t)sizeof(type))

#define REFLECT_NUM_FIELD_ELEMENTS(struct_name, field_name, type) \
    (REFLECT_SIZEOF_FIELD(struct_name, field_name) / REFLECT_SIZEOF_ELEMENT_TYPE(type))

#define REFLECT_OFFSETOF_FIELD(struct_name, field_name) \
    ((uint32_t)offsetof(struct_name, field_name))

#define REFLECT_DEF_STRUCT(name) \
    const reflect_member_desc_t REFLECT_DESC_NAME(name)[] =

#define REFLECT(struct_name, field_name, type) { \
    #field_name, \
    {0}, \
    REFLECT_NUM_FIELD_ELEMENTS(struct_name, field_name, type), \
    REFLECT_OFFSETOF_FIELD(struct_name, field_name), \
    REFLECT_SIZEOF_ELEMENT_TYPE(type), \
    reflect_type_##type \
    }

#define REFLECT_STRUCT(struct_name, field_name, type) { \
    #field_name, \
    {REFLECT_DESC_NAME(type), REFLECT_DESC_SIZE(type)}, \
    REFLECT_NUM_FIELD_ELEMENTS(struct_name, field_name, type), \
    REFLECT_OFFSETOF_FIELD(struct_name, field_name), \
    REFLECT_SIZEOF_ELEMENT_TYPE(type), \
    0 \
    }

#define REFLECT_ARRAY(struct_name, field_name, type) { \
    #field_name, \
    {0}, \
    0, \
    REFLECT_OFFSETOF_FIELD(struct_name, field_name), \
    REFLECT_SIZEOF_ELEMENT_TYPE(type), \
    reflect_type_##type \
    }

#define REFLECT_STRUCT_ARRAY(struct_name, field_name, type) { \
    #field_name, \
    {REFLECT_DESC_NAME(type), REFLECT_DESC_SIZE(type)}, \
    0, \
    REFLECT_OFFSETOF_FIELD(struct_name, field_name), \
    REFLECT_SIZEOF_ELEMENT_TYPE(type), \
    0 \
    }


#endif // ifndef BOOMED_REFLECT_H_
