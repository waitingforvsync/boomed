#ifndef BOOMED_MATH_VEC2_H_
#define BOOMED_MATH_VEC2_H_

#include <cmath>
#include <cstdint>
#include <type_traits>


template <typename T>
concept Scalar = std::integral<T> || std::floating_point<T>;

template <Scalar T>
struct vec2 {
    T x{};
    T y{};

    using scalar_result = std::conditional_t<std::is_floating_point_v<T>, T, std::int64_t>;

    // Constructors
    vec2() = default;
    vec2(T x, T y);
    template <Scalar U> vec2(vec2<U> v);

    // Binary functions
    static auto dot(vec2 a, vec2 b) -> scalar_result;
    static auto cross(vec2 a, vec2 b) -> scalar_result;
    static auto distancesqr(vec2 a, vec2 b) -> scalar_result;
    static auto distance(vec2 a, vec2 b) -> scalar_result;

    // Unary functions
    auto lensqr() const -> scalar_result;
    auto len() const -> scalar_result;
    auto normalised() const -> vec2 requires std::is_floating_point_v<T>;
};


using vec2f = vec2<float>;
using vec2i = vec2<std::int16_t>;


template <Scalar T> 
inline vec2<T>::vec2(T x, T y) : x{x}, y{y} {}

template <Scalar T>
template <Scalar U>
inline vec2<T>::vec2(vec2<U> v) : x{static_cast<T>(v.x)}, y{static_cast<T>(v.y)} {}

template <Scalar T>
inline auto operator +(vec2<T> a, vec2<T> b) -> vec2<T> {
    return {
        static_cast<T>(a.x + b.x),
        static_cast<T>(a.y + b.y)
    };
}

template <Scalar T>
inline auto operator -(vec2<T> a, vec2<T> b) -> vec2<T> {
    return {
        static_cast<T>(a.x - b.x),
        static_cast<T>(a.y - b.y)
    };
}

template <Scalar T>
inline auto operator *(vec2<T> a, T b) -> vec2<T> {
    return {
        static_cast<T>(a.x * b),
        static_cast<T>(a.y * b)
    };
}

template <Scalar T>
inline auto vec2<T>::dot(vec2 a, vec2 b) -> scalar_result {
    return
        static_cast<scalar_result>(a.x) * static_cast<scalar_result>(b.x) +
        static_cast<scalar_result>(a.y) * static_cast<scalar_result>(b.y);
}

template <Scalar T>
inline auto vec2<T>::cross(vec2 a, vec2 b) -> scalar_result {
    return
        static_cast<scalar_result>(a.x) * static_cast<scalar_result>(b.y) -
        static_cast<scalar_result>(a.y) * static_cast<scalar_result>(b.x);
}

template <Scalar T>
inline auto vec2<T>::distancesqr(vec2 a, vec2 b) -> scalar_result {
    return (b - a).lensqr();
}

template <Scalar T>
inline auto vec2<T>::distance(vec2 a, vec2 b) -> scalar_result {
    return (b - a).len();
}

template <Scalar T>
inline auto vec2<T>::lensqr() const -> scalar_result {
    return vec2::dot(*this, *this);
}

template <Scalar T>
inline auto vec2<T>::len() const -> scalar_result {
    return static_cast<scalar_result>(std::sqrt(vec2::dot(*this, *this)));
}

template <Scalar T>
inline auto vec2<T>::normalised() const -> vec2<T> requires std::is_floating_point_v<T> {
    return *this * (T{1.0} / len());
}

#endif // ifndef BOOMED_MATH_VEC2_H_
