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


#endif // ifndef BOOMED_GENERIC_SLICE_H_
