#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "boomed/arena.h"


typedef union arena_header_t {
    struct {
        void *prev;
        void *next;
        uint32_t size;
    };
    uint8_t reserve[32];
} arena_header_t;


typedef union arena_alloc_header_t {
    struct {
        uint32_t prev;
        uint32_t next;
    };
    uint32_t reserve[16];
} arena_alloc_header_t;


arena_t make_arena(uint32_t initial_size) {
    arena_t arena;
    arena_init(&arena, initial_size);
    return arena;
}


void arena_init(arena_t *arena, uint32_t initial_size) {
    assert(arena);
    if (arena->base) {
        arena_deinit(arena);
    }

    arena_header_t *header = calloc(initial_size, 1);
    if (!header) {
        abort();
    }

    header->size = initial_size;

    arena->base = header;
    arena->next = (uint32_t)sizeof *header;
}


void arena_deinit(arena_t *arena) {
    assert(arena);
    arena_header_t *base = arena->base;
    while (base) {
        arena_header_t *prev = base->prev;
        free(base);
        base = prev;
    }

    arena->base = 0;
    arena->next = 0;
}


static uint32_t get_aligned_size(uint32_t size) {
    return (size + 15) & ~15;
}


void *arena_alloc(arena_t *arena, uint32_t size) {
    assert(arena);
    size = get_aligned_size(size);
    uint32_t end = sizeof(arena_alloc_header_t) + arena->next + size;

    arena_header_t *header = arena->base;
    if (end <= header->size) {

    }

    return 0;
}


void arena_free(arena_t *arena, void *ptr) {

}


void arena_reset(arena_t *arena) {

}
