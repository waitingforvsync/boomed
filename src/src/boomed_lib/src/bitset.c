#include "boomed/bitset.h"
#include "boomed/arena.h"
#include "boomed/math/math.h"


bitset_t make_bitset(uint32_t initial_capacity, arena_t *arena) {
    bitset_t bitset = {0};
    bitset_init(&bitset, initial_capacity, arena);
    return bitset;
}


void bitset_init(bitset_t *bitset, uint32_t initial_capacity, arena_t *arena) {
    uint32_t element_num = (initial_capacity + 63) / 64;
    bitset->bits = arena_alloc(arena, element_num * 8);
    bitset->num = 0;
    bitset->capacity = element_num * 64;
}


void bitset_reserve(bitset_t *bitset, uint32_t capacity, arena_t *arena) {
    if (capacity > bitset->capacity) {
        uint32_t element_num = (capacity + 63) / 64;
        bitset->bits = arena_realloc(arena, bitset->bits, bitset->capacity / 8, element_num * 8);
        bitset->capacity = element_num * 64;
    }
}


void bitset_resize(bitset_t *bitset, uint32_t size, arena_t *arena) {
    if (size < bitset->num) {
        // If shrinking, clear the top unused bits of the final element
        bitset->bits[size / 64] &= ~((1ULL << (size & 63)) - 1);
    }
    else {
        // If growing, clear the newly reserved final elements
        bitset_reserve(bitset, size, arena);
        for (uint32_t i = (bitset->num + 63) / 64, max_i = (size + 63) / 64; i < max_i; ++i) {
            bitset->bits[i] = 0;
        }
    }
    bitset->num = size;
}


void bitset_reset(bitset_t *bitset) {
    bitset->num = 0;
}


void bitset_set(bitset_t *bitset, uint32_t index) {
    if (index < bitset->num) {
        bitset->bits[index / 64] |= (1ULL << (index & 63));
    }
}


void bitset_clear(bitset_t *bitset, uint32_t index) {
    if (index < bitset->num) {
        bitset->bits[index / 64] &= ~(1ULL << (index & 63));
    }
}


bool bitset_is_set(const bitset_t *bitset, uint32_t index) {
    return index < bitset->num && (bitset->bits[index / 64] & (1ULL << (index & 63)));
}


void bitset_for_each(const bitset_t *bitset, void (*fn)(void *, uint32_t), void *ctx) {
    for (uint32_t i = 0, max_i = (bitset->num + 63) / 64; i < max_i; ++i) {
        uint64_t b = bitset->bits[i];
        while (b) {
            uint64_t t = b & -b;
            fn(ctx, i * 64 + 63 - clz_uint64(t));
            b ^= t;
        }
    }
}
