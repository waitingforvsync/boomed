#ifndef BOOMED_MATH_BOX2_H_
#define BOOMED_MATH_BOX2_H_

#include "math/vec2.h"


// A box2 is a half-open axis-aligned bounding box.
// i.e. [min, max)
template <Scalar T>
struct box2 {
    vec2<T> min{};
    vec2<T> max{};

    // Constructors
    constexpr box2() = default;
    constexpr box2(vec2<T> a, vec2<T> b);
    template <Scalar U> constexpr box2(box2<U> b);

    // Named constructors
    static constexpr auto make_union(box2 a, box2 b) -> box2;
    static constexpr auto make_union(box2 a, vec2<T> p) -> box2;

    // Member ops
    constexpr auto empty() const -> bool;
    constexpr auto contains(box2 b) const -> bool;
    constexpr auto intersects(box2 b) const -> bool;
    constexpr auto contains_point(vec2<T> p) const -> bool;
};


// Construct a box from two corner points
template <Scalar T>
constexpr box2<T>::box2(vec2<T> a, vec2<T> b) :
    min{vec2<T>::min(a, b)},
    max{vec2<T>::max(a, b)} {}

// Construct a box from one of a different type
template <Scalar T>
template <Scalar U>
constexpr box2<T>::box2(box2<U> b) : min{b.min}, max{b.max} {}

// Is the box empty?
template <Scalar T>
constexpr auto box2<T>::empty() const -> bool {
    return min.x >= max.x || min.y >= max.y;
}

// Does this box contain box b?
template <Scalar T>
constexpr auto box2<T>::contains(box2 b) const -> bool {
    return !b.empty() &&
        min.x <= b.min.x &&
        min.y <= b.min.y &&
        b.max.x <= max.x &&
        b.max.y <= max.y;
}

// Does this box intersect box b?
template <Scalar T>
constexpr auto box2<T>::intersects(box2 b) const -> bool {
    return !empty() && !b.empty() &&
        min.x < b.max.x &&
        min.y < b.max.y &&
        max.x > b.min.x &&
        max.y > b.min.y;
}

// Does this box contain point p?
template <Scalar T>
constexpr auto box2<T>::contains_point(vec2<T> p) const -> bool {
    return min.x <= p.x && min.y <= p.y && p.x < max.x && p.y < max.y;
}

// Make a box which is the union of box a and box b
template <Scalar T>
constexpr auto box2<T>::make_union(box2 a, box2 b) -> box2 {
    return box2{
        vec2<T>::min(a.min, b.min),
        vec2<T>::max(a.max, b.max)
    };
}

// Make a box which is the union of box a and point p
template <Scalar T>
constexpr auto box2<T>::make_union(box2 a, vec2<T> p) -> box2 {
    return box2{
        vec2<T>::min(a.min, p),
        vec2<T>::max(a.max, p)
    };
}


#endif // ifndef BOOMED_MATH_BOX2_H_
