#ifndef STR_H_
#define STR_H_

#include <stdint.h>


typedef struct str_t str_t;
typedef struct str_view_t str_view_t;

struct str_view_t {
    const uint8_t *data;
    uint32_t len;
};

struct str_t {
    union {
        struct {
            const uint8_t *data;
            uint32_t len;
        };
        str_view_t view;
    };
    uint32_t capacity;
};



#define STR(x) {(const uint8_t *)(x), sizeof(x)}


#endif // ifndef BOOMED_STR_H_
