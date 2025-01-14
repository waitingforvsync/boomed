#ifndef BOOMED_BITSET_H_
#define BOOMED_BITSET_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct arena_t arena_t;
typedef struct bitset_t bitset_t;

struct bitset_t {
    uint64_t *bits;
    uint32_t num;
    uint32_t capacity;
};


bitset_t make_bitset(uint32_t initial_capacity, arena_t *arena);
void bitset_init(bitset_t *bitset, uint32_t initial_capacity, arena_t *arena);
void bitset_reserve(bitset_t *bitset, uint32_t capacity, arena_t *arena);
void bitset_resize(bitset_t *bitset, uint32_t size, arena_t *arena);
void bitset_reset(bitset_t *bitset);
void bitset_set(bitset_t *bitset, uint32_t index);
void bitset_clear(bitset_t *bitset, uint32_t index);
bool bitset_is_set(const bitset_t *bitset, uint32_t index);
//uint32_t bitset_get_first_set(const bitset_t *bitset);
//uint32_t bitset_get_next_set(const bitset_t *bitset, uint32_t index);
void bitset_for_each(const bitset_t *bitset, void (*fn)(void *, uint32_t), void *ctx);


#endif // ifndef BOOMED_BITSET_H_
