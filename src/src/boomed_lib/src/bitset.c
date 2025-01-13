#include "boomed/bitset.h"


bitset_t make_bitset(uint32_t initial_capacity, arena_t *arena) {
    bitset_t bitset;
    bitset_init(&bitset, initial_capacity, arena);
    return bitset;
}


void bitset_init(bitset_t *bitset, uint32_t initial_capacity, arena_t *arena) {

}


void bitset_deinit(bitset_t *bitset) {

}


void bitset_reserve(bitset_t *bitset, uint32_t capacity, arena_t *arena) {

}


void bitset_resize(bitset_t *bitset, uint32_t size, arena_t *arena) {

}


void bitset_reset(bitset_t *bitset) {

}


void bitset_set(bitset_t *bitset, uint32_t index) {

}


void bitset_clear(bitset_t *bitset, uint32_t index) {

}


bool bitset_is_set(const bitset_t *bitset, uint32_t index) {
    return false;
}


uint32_t bitset_get_first_set(const bitset_t *bitset) {
    return 0;
}


uint32_t bitset_get_next_set(const bitset_t *bitset, uint32_t index) {
    return 0;
}


void bitset_for_each(const bitset_t *bitset, void (*fn)(void *, uint32_t), void *ctx) {

}
