#ifndef BOOMED_ARENA_H_
#define BOOMED_ARENA_H_

#include <stdint.h>


typedef struct arena_t arena_t;

struct arena_t {
    void *base;
    uint32_t next;
};

enum arena_flags_t {
    arena_flag_growable = (1<<0),   // set if the arena is allowed to grow from its initial allocation size
    arena_flag_chained  = (1<<1),   // set to chain distinct allocations instead of resizing the existing one
    arena_flag_freeable = (1<<2),   // set if we track freed allocations
};

arena_t make_arena(uint32_t initial_size, uint32_t flags);
arena_t make_arena_fixed(uint32_t size);
arena_t make_arena_chained(uint32_t initial_size);
arena_t make_arena_resizable(uint32_t initial_size);
void arena_init(arena_t *arena, uint32_t initial_size, uint32_t flags);
void arena_deinit(arena_t *arena);
void *arena_alloc(arena_t *arena, uint32_t size);
void *arena_realloc(arena_t *arena, void *old_ptr, uint32_t new_size);
void arena_free(arena_t *arena, void *ptr);
void arena_reset(arena_t *arena);


#endif // ifndef BOOMED_ARENA_H_
