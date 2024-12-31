#ifndef BOOMED_GENERIC_SLICE_H_
#define BOOMED_GENERIC_SLICE_H_


#define DEF_SLICE(name, type) \
    typedef struct slice_const_##name##_t { \
        const type *data; \
        uint32_t size; \
    } slice_const_##name##_t; \
    \
    typedef union slice_##name##_t { \
        struct { \
            type *data; \
            uint32_t size; \
        }; \
        slice_const_##name##_t const_slice; \
    } slice_##name##_t;


#define SLICE(type, ...) { \
        .data = (type[]){__VA_ARGS__}, \
        .num = sizeof((type[]){__VA_ARGS__}) / sizeof(type) \
    }


#define INDEX_NONE (0xFFFFFFFFU)


#endif // ifndef BOOMED_GENERIC_SLICE_H_
