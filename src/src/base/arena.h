#ifndef ARENA_H_
#define ARENA_H_

#include <stdint.h>


typedef struct arena_t arena_t;

struct arena_t {
    void *base;
    uint32_t offset;
};

arena_t arena_make(void);
arena_t arena_make_with_size(uint32_t initial_size);
void arena_init(arena_t *arena, uint32_t initial_size);
void arena_deinit(arena_t *arena);
void *arena_alloc(arena_t *arena, uint32_t size);
void *arena_calloc(arena_t *arena, uint32_t size);
void *arena_realloc(arena_t *arena, void *old_ptr, uint32_t old_size, uint32_t new_size);
void arena_free(arena_t *arena, void *ptr, uint32_t size);
void arena_reset(arena_t *arena);
uint32_t arena_get_current_block_size(const arena_t *arena);

#define arena_new(type, arena) ((type *)arena_alloc(arena, sizeof(type)))
#define arena_new_n(type, num, arena) ((type *)arena_alloc(arena, sizeof(type) * (num)))
#define arena_new_zeroed(type, arena) ((type *)arena_calloc(arena, sizeof(type)))
#define arena_new_zeroed_n(type, num, arena) ((type *)arena_calloc(arena, sizeof(type) * (num)))
#define arena_destroy(arena, ptr) arena_free(arena, ptr, sizeof(*ptr))
#define arena_destroy_n(arena, ptr, num) arena_free(arena, ptr, sizeof(*ptr) * (num))


#endif // ifndef ARENA_H_
