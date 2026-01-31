#ifndef BOOMED_WORLD_QUADTREE_H_
#define BOOMED_WORLD_QUADTREE_H_

#include <array>
#include <optional>
#include <immer/array.hpp>
#include <immer/box.hpp>
#include "math/box2.h"


template <typename Item, Scalar T>
class quadtree {
public:
    // Constructors
    explicit quadtree(box2<T> box);

    // Add an item into the quadtree
    auto add(Item item) -> quadtree;

private:
    static constexpr auto threshold = std::size_t{16};
    static constexpr auto max_depth = std::size_t{8};

    struct node {
        immer::array<Item> values{};
        std::optional<std::array<immer::box<node>, 4>> children{};
    };

    immer::box<node> root{};
    box2<T> world_box;
};


template <typename Item, Scalar T>
quadtree<Item, T>::quadtree(box2<T> box) : world_box{box} {}

template <typename Item, Scalar T>
auto quadtree<Item, T>::add(Item item) -> quadtree {
    return *this;
}


#endif // ifndef BOOMED_WORLD_QUADTREE_H_
