#ifndef BOOMED_STR_H_
#define BOOMED_STR_H_

#include <stdint.h>


typedef struct str_t str_t;


struct str_t {
    const char *data;
    uint32_t len;
};

#define STR(x) {x, sizeof(x)}


#endif // ifndef BOOMED_STR_H_
