#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdbool.h>
#include <stdint.h>
#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef _MSC_VER
#define trap() __debugbreak()
#else
#define trap() __builtin_trap()
#endif

#define assert(cond) ((void)((cond) || (trap(), 0)))
#define always_assert(cond) ((void)((cond) || (trap(), 0)))
#define static_assert(cond, msg) _Static_assert(cond, msg)

#define CONCAT(a, b) CONCAT2_(a, b)
#define CONCAT2_(a, b) a##b


#endif // ifndef DEFINES_H_
