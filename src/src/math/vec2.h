#ifndef BOOMED_MATH_VEC2_H_
#define BOOMED_MATH_VEC2_H_

#include <cmath>
#include <cstdint>
#include "math/concepts.h"


template <Scalar T>
struct vec2 {
    T x{};
    T y{};

    using scalar_result = std::conditional_t<std::is_floating_point_v<T>, T, std::int64_t>;

    // Constructors
    constexpr vec2() = default;
    constexpr vec2(T x, T y);
    template <Scalar U> constexpr vec2(vec2<U> v);

    // Constants
    static constexpr auto zero() -> vec2 { return vec2{0, 0}; }
    static constexpr auto axisx() -> vec2 { return vec2{1, 0}; }
    static constexpr auto axisy() -> vec2 { return vec2{0, 1}; }

    // Named constructors
    static constexpr auto make_sincos(T angle) -> vec2 requires ScalarReal<T>;

    // Member operations
    constexpr auto lensqr() const -> scalar_result;
    constexpr auto len() const -> scalar_result;
    constexpr auto normalised() const -> vec2 requires ScalarReal<T>;

    // Binary functions
    static constexpr auto dot(vec2 a, vec2 b) -> scalar_result;
    static constexpr auto cross(vec2 a, vec2 b) -> scalar_result;
    static constexpr auto distancesqr(vec2 a, vec2 b) -> scalar_result;
    static constexpr auto distance(vec2 a, vec2 b) -> scalar_result;
    static constexpr auto min(vec2 a, vec2 b) -> vec2;
    static constexpr auto max(vec2 a, vec2 b) -> vec2;
};


// Construct a vec2 from two scalars
template <Scalar T>
constexpr vec2<T>::vec2(T x, T y) : x{x}, y{y} {}

// Implicitly construct a vec2 from a different type of vec2
template <Scalar T>
template <Scalar U>
constexpr vec2<T>::vec2(vec2<U> v) : x{static_cast<T>(v.x)}, y{static_cast<T>(v.y)} {}

// Unary negation
template <Scalar T>
constexpr auto operator -(vec2<T> a) -> vec2<T> {
    return {
        static_cast<T>(-a.x),
        static_cast<T>(-a.y)
    };
}

// Binary addition
template <Scalar T>
constexpr auto operator +(vec2<T> a, vec2<T> b) -> vec2<T> {
    return {
        static_cast<T>(a.x + b.x),
        static_cast<T>(a.y + b.y)
    };
}

// Binary subtraction
template <Scalar T>
constexpr auto operator -(vec2<T> a, vec2<T> b) -> vec2<T> {
    return {
        static_cast<T>(a.x - b.x),
        static_cast<T>(a.y - b.y)
    };
}

// Multiply by a scalar
template <Scalar T>
constexpr auto operator *(vec2<T> v, T s) -> vec2<T> {
    return {
        static_cast<T>(v.x * s),
        static_cast<T>(v.y * s)
    };
}

// Divide by a scalar
template <Scalar T>
constexpr auto operator /(vec2<T> v, T s) -> vec2<T> {
    return {
        static_cast<T>(v.x / s),
        static_cast<T>(v.y / s)
    };
}

// Compare equal
template <Scalar T>
constexpr auto operator ==(vec2<T> a, vec2<T> b) -> bool {
    return a.x == b.x && a.y == b.y;
}

// Compare not equal
template <Scalar T>
constexpr auto operator !=(vec2<T> a, vec2<T> b) -> bool {
    return a.x != b.x || a.y != b.y;
}

// Make a vec2 from the sin, cos of angle
template <Scalar T>
constexpr auto vec2<T>::make_sincos(T angle) -> vec2 requires ScalarReal<T> {
    return {
        std::sin(angle),
        std::cos(angle)
    };
}


// Get the square of the length of the vec2
template <Scalar T>
constexpr auto vec2<T>::lensqr() const -> scalar_result {
    return vec2::dot(*this, *this);
}

// Get the length of the vec2
template <Scalar T>
constexpr auto vec2<T>::len() const -> scalar_result {
    return static_cast<scalar_result>(std::sqrt(lensqr()));
}

// Return a normalised vec2
template <Scalar T>
constexpr auto vec2<T>::normalised() const -> vec2 requires ScalarReal<T> {
    return *this / len();
}

// Calculate the dot product of a and b
template <Scalar T>
constexpr auto vec2<T>::dot(vec2 a, vec2 b) -> scalar_result {
    return
        static_cast<scalar_result>(a.x) * static_cast<scalar_result>(b.x) +
        static_cast<scalar_result>(a.y) * static_cast<scalar_result>(b.y);
}

// Calculate the cross (wedge) product of a and b
template <Scalar T>
constexpr auto vec2<T>::cross(vec2 a, vec2 b) -> scalar_result {
    return
        static_cast<scalar_result>(a.x) * static_cast<scalar_result>(b.y) -
        static_cast<scalar_result>(a.y) * static_cast<scalar_result>(b.x);
}

// Get the distance squared between a and b
template <Scalar T>
constexpr auto vec2<T>::distancesqr(vec2 a, vec2 b) -> scalar_result {
    return (b - a).lensqr();
}

// Get the distance between a and b
template <Scalar T>
constexpr auto vec2<T>::distance(vec2 a, vec2 b) -> scalar_result {
    return (b - a).len();
}

// Return a vec2 which is the componentwise minimum of a and b
template <Scalar T>
constexpr auto vec2<T>::min(vec2 a, vec2 b) -> vec2 {
    return {
        static_cast<T>(std::min(a.x, b.x)),
        static_cast<T>(std::min(a.y, b.y))
    };
}

// Return a vec2 which is the componentwise maximum of a and b
template <Scalar T>
constexpr auto vec2<T>::max(vec2 a, vec2 b) -> vec2 {
    return {
        static_cast<T>(std::max(a.x, b.x)),
        static_cast<T>(std::max(a.y, b.y))
    };
}

#endif // ifndef BOOMED_MATH_VEC2_H_
