#ifndef BOOMED_GENERIC_PAIR_H_
#define BOOMED_GENERIC_PAIR_H_

#include <stdint.h>


#define DEF_PAIR(name, type) \
    typedef struct pair_##name##_t { \
        type first; \
        type second; \
    } pair_##name##_t;

DEF_PAIR(int32, int32_t)
DEF_PAIR(uint32, uint32_t)
DEF_PAIR(int64, int64_t)
DEF_PAIR(uint64, uint64_t)
DEF_PAIR(float, float)
DEF_PAIR(double, double)


#define DEF_PAIR2(name, type1, type2) \
    typedef struct pair_##name##_t { \
        type1 first; \
        type2 second; \
    } pair_##name##_t;


#endif // ifndef BOOMED_GENERIC_PAIR_H_
