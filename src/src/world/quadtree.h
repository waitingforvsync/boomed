#ifndef BOOMED_WORLD_QUADTREE_H_
#define BOOMED_WORLD_QUADTREE_H_

#include <array>
#include <optional>
#include <vector>
#include "math/box2.h"


template <typename Pred, typename Item, typename T>
concept ContainsFn =
    Scalar<T> &&
    std::predicate<Pred&, box2<T>, const Item&>;


template <typename Item, Scalar T>
class quadtree {
public:
    // Constructors
    explicit quadtree(box2<T> box);

    // Add an item into the quadtree
    template <ContainsFn<Item, T> F>
    auto add(Item item, F contains_fn) -> bool;

    // Remove an item from the quadtree
    auto remove(Item item) -> bool;

private:
    static constexpr auto threshold = std::size_t{16};
    static constexpr auto max_depth = std::size_t{8};

    struct node {
        std::vector<Item> values{};
        std::optional<std::size_t> child_index{};
    };

    node root{};
    std::vector<std::array<node, 4>> nodes{};
    box2<T> world_box{};
};


template <typename Item, Scalar T>
quadtree<Item, T>::quadtree(box2<T> box) : world_box{box} {}

template <typename Item, Scalar T>
template <ContainsFn<Item, T> F>
auto quadtree<Item, T>::add(Item item, F contains_fn) -> bool {
    return false;
}


#endif // ifndef BOOMED_WORLD_QUADTREE_H_
