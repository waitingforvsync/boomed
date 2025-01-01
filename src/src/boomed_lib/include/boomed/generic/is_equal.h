#ifndef BOOMED_GENERIC_IS_EQUAL_H_
#define BOOMED_GENERIC_IS_EQUAL_H_

#include <stdbool.h>
#include <stdint.h>


#define DEF_IS_EQUAL(name, type) \
    static inline bool is_equal_##name(const type *a, const type *b) { \
        return *a == *b; \
    }

DEF_IS_EQUAL(int32, int32_t)
DEF_IS_EQUAL(uint32, uint32_t)
DEF_IS_EQUAL(int64, int64_t)
DEF_IS_EQUAL(uint64, uint64_t)
DEF_IS_EQUAL(float, float)
DEF_IS_EQUAL(double, double)


#endif // ifndef BOOMED_GENERIC_IS_EQUAL_H_
