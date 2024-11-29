#ifndef BOOMED_ARENA_H_
#define BOOMED_ARENA_H_

#include <stdint.h>


typedef struct arena_t arena_t;

struct arena_t {
    void *base;
    uint32_t next;
};

enum arena_type_t {
    arena_type_fixed,
    arena_type_chained,
    arena_type_growable
};

arena_t make_arena(uint32_t initial_size);
void arena_init(arena_t *arena, uint32_t initial_size);
void arena_deinit(arena_t *arena);
void *arena_alloc(arena_t *arena, uint32_t size);
void *arena_realloc(arena_t *arena, void *old_ptr);
void arena_free(arena_t *arena, void *ptr);
void arena_reset(arena_t *arena);


#endif // ifndef BOOMED_ARENA_H_
