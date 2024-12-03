#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "boomed/arena.h"


// Arenas are coarse allocations from which smaller allocations are made in a stack-like manner.
// Instead of freeing allocations individually, they may be freed in one go by resetting the arena.
// Allocations may optionally be marked as 'freed' but their space will not be reused until 
// there are no allocations after them.
//
// Arenas may be fixed, chainable or resizeable.
// The simplest type of arena is of fixed size, and, once full, will force an abort if an attempt
// is made to allocate more from them.
// Arenas may also optionally grow, either by chaining new separate, allocated blocks, or by
// resizing the one existing block.


typedef union arena_block_header_t arena_block_header_t;
typedef union arena_alloc_header_t arena_alloc_header_t;

union arena_block_header_t {
    struct {
        arena_block_header_t *prev;
        arena_block_header_t *next;
        uint32_t canary;
        uint32_t size;
        uint8_t flags;
    };
    uint8_t reserve[32];
};


union arena_alloc_header_t {
    struct {
        uint32_t prev;
        uint32_t this;
        uint32_t next;
    };
    uint8_t reserve[16];
};


arena_t make_arena(uint32_t initial_size, uint32_t flags) {
    arena_t arena;
    arena_init(&arena, initial_size, flags);
    return arena;
}


arena_t make_arena_fixed(uint32_t size) {
    arena_t arena;
    arena_init(&arena, size, arena_flag_freeable);
    return arena;
}


arena_t make_arena_chained(uint32_t initial_size) {
    arena_t arena;
    arena_init(&arena, initial_size, arena_flag_growable | arena_flag_chained | arena_flag_freeable);
    return arena;
}


arena_t make_arena_resizable(uint32_t initial_size) {
    arena_t arena;
    arena_init(&arena, initial_size, arena_flag_growable | arena_flag_freeable);
    return arena;
}


void arena_init(arena_t *arena, uint32_t initial_size, uint32_t flags) {
    assert(arena);

    if (arena->base) {
        arena_deinit(arena);
    }

    arena_block_header_t *block_header = calloc(initial_size, 1);
    if (!block_header) {
        abort();
    }

    block_header->size = initial_size;
    block_header->canary = (uint32_t)((intptr_t)(&block_header));
    block_header->flags = flags;

    arena->base = block_header;
    arena->next = (uint32_t)sizeof(arena_block_header_t);

    arena_alloc_header_t *alloc_header = (arena_alloc_header_t *)((char *)block_header + arena->next);
    alloc_header->this = arena->next;
}


void arena_deinit(arena_t *arena) {
    assert(arena);
    
    arena_block_header_t *base = arena->base;
    while (base) {
        arena_block_header_t *prev = base->prev;
        free(base);
        base = prev;
    }

    arena->base = 0;
    arena->next = 0;
}


static bool is_alloc_header_valid(const arena_alloc_header_t *alloc_header) {
    const arena_block_header_t *block_header = (const arena_block_header_t *)((const char *)alloc_header - alloc_header->this);
    return (block_header->canary == (uint32_t)((intptr_t)(&block_header)));
}


static arena_block_header_t *get_arena_block_header(arena_alloc_header_t *alloc_header) {
    // Get the header of the block which holds this allocation
    // We can get there via the 'this' offset of the alloc header
    arena_block_header_t *block_header = (arena_block_header_t *)((char *)alloc_header - alloc_header->this);
    assert(block_header->canary == (uint32_t)((intptr_t)(&block_header)));
    return block_header;
}


static arena_alloc_header_t *get_prev_alloc_header(arena_alloc_header_t *alloc_header) {
    if (!alloc_header->prev) {
        // If prev is 0, this is the first allocation in the arena
        return 0;
    }

    if (alloc_header->prev & 0x01) {
        // If the bottom bit of prev is set, the previous alloc is in the previous block
        arena_block_header_t *prev_block_header = get_arena_block_header(alloc_header)->prev;
        assert(prev_block_header);
        return (arena_alloc_header_t *)((char *)prev_block_header + (alloc_header->prev & ~0x01));
    }

    // Otherwise just follow the prev link to the previous alloc (taking care with unsigned subtraction!)
    return (arena_alloc_header_t *)((char *)alloc_header - (alloc_header->this - alloc_header->prev));
}


static arena_alloc_header_t *get_next_alloc_header(arena_alloc_header_t *alloc_header) {
    if (!alloc_header->next) {
        // If next is 0, this is the final allocation in the arena
        return 0;
    }

    if (alloc_header->next & 0x01) {
        // If the bottom bit of next is set, the next alloc is in the next block
        arena_block_header_t *next_block_header = get_arena_block_header(alloc_header)->next;
        assert(next_block_header);
        return (arena_alloc_header_t *)((char *)next_block_header + (alloc_header->next & ~0x01));
    }

    // Otherwise just follow the next link to the next alloc
    return (arena_alloc_header_t *)((char *)alloc_header + (alloc_header->next - alloc_header->this));
}


void *arena_alloc(arena_t *arena, uint32_t size) {
    assert(arena);
    assert(arena->base);

    uint32_t aligned_size = (size + 15) & ~15;

    arena_block_header_t *block_header = arena->base;
    arena_alloc_header_t *alloc_header = (arena_alloc_header_t *)((char *)arena->base + arena->next);

    uint32_t end = arena->next + (uint32_t)sizeof(arena_alloc_header_t) + aligned_size;
    if (end + (uint32_t)sizeof(arena_alloc_header_t) <= block_header->size) {
        // Allocation fits in this arena block.
        // This is the quick path: just add the allocation to the end.
        // Note there must be additional space to fit the trailing alloc header
        arena_alloc_header_t *next_alloc_header = (arena_alloc_header_t *)((char *)arena->base + end);
        next_alloc_header->prev = arena->next;
        next_alloc_header->this = end;
        next_alloc_header->next = 0;
        alloc_header->next = end;
        arena->next = end;
        
        return alloc_header + 1;
    }
    
    return 0;
}


void *arena_realloc(arena_t *arena, void *old_ptr, uint32_t new_size) {
    assert(arena);
    assert(arena->base);
    return 0;
}


void arena_free(arena_t *arena, void *ptr) {
    assert(arena);
    assert(arena->base);

    if (!(((arena_block_header_t *)arena->base)->flags & arena_flag_freeable)) {
        // If arena not flagged as freeable, this does nothing
        return;
    }

    arena_alloc_header_t *alloc_header = (arena_alloc_header_t *)ptr - 1;
}


void arena_reset(arena_t *arena) {
    assert(arena);
    assert(arena->base);
    while (((arena_block_header_t *)arena->base)->prev) {
        arena->base = ((arena_block_header_t *)arena->base)->prev;
    }

    arena->next = (uint32_t)sizeof(arena_block_header_t);

}
