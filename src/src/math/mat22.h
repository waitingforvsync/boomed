#ifndef BOOMED_MATH_MAT22_H_
#define BOOMED_MATH_MAT22_H_

#include "math/vec2.h"


template <ScalarReal T>
struct mat22 {
    vec2<T> columnx{};
    vec2<T> columny{};

    using scalar_result = T;

    // Constructors
    constexpr mat22() = default;
    constexpr mat22(vec2<T> x, vec2<T> y);
    template <ScalarReal U> constexpr mat22(mat22<U> m);

    // Named constructors
    constexpr auto make_rot(T angle) -> mat22;

    // Nullary functions
    static constexpr auto zero() -> mat22;
    static constexpr auto identity() -> mat22;

    // Member operations
    constexpr auto determinant() const -> T;
    constexpr auto transpose() const -> mat22;
    constexpr auto inverse() const -> mat22;
};


// Construct a mat22 from a vec2 basis
template <ScalarReal T>
constexpr mat22<T>::mat22(vec2<T> x, vec2<T> y) : columnx{x}, columny{y} {}

// Implicitly construct a mat22 from a different type of mat22
template <ScalarReal T>
template <ScalarReal U>
constexpr mat22<T>::mat22(mat22<U> m) : columnx{m.x}, columny{m.y} {}

// Make a mat22 rotation matrix
template <ScalarReal T>
constexpr auto mat22<T>::make_rot(T angle) -> mat22 {
    auto s = std::sin(angle);
    auto c = std::cos(angle);
    return {
        { c, s},
        {-s, c}
    };
}

template <ScalarReal T>
constexpr auto mat22<T>::zero() -> mat22 {
    return mat22{vec2<T>::zero(), vec2<T>::zero()};
}

template <ScalarReal T>
constexpr auto mat22<T>::identity() -> mat22 {
    return mat22{vec2<T>::axisx(), vec2<T>::axisy()};
}

// Add two mat22s
template <ScalarReal T>
constexpr auto operator +(mat22<T> a, mat22<T> b) -> mat22<T> {
    return {
        a.columnx + b.columnx,
        a.columny + b.columny
    };
}

// Subtract two mat22s
template <ScalarReal T>
constexpr auto operator -(mat22<T> a, mat22<T> b) -> mat22<T> {
    return {
        a.columnx - b.columnx,
        a.columny - b.columny
    };
}

// Scale a mat22
template <ScalarReal T>
constexpr auto operator *(mat22<T> m, T s) -> mat22<T> {
    return {
        m.columnx * s,
        m.columny * s
    };
}

// mat22 * vec2
template <ScalarReal T>
constexpr auto operator *(mat22<T> m, vec2<T> v) -> vec2<T> {
    return m.columnx * v.x + m.columny * v.y;
}

// mat22 * mat22 (compose)
template <ScalarReal T>
constexpr auto operator *(mat22<T> a, mat22<T> b) -> mat22<T> {
    return {
        a * b.columnx,
        a * b.columny
    };
}

// Get the determinant of this matrix
template <ScalarReal T>
constexpr auto mat22<T>::determinant() const -> T {
    return vec2<T>::cross(columnx, columny);
}

// Get the transpose of this matrix
template <ScalarReal T>
constexpr auto mat22<T>::transpose() const -> mat22 {
    return {
        {columnx.x, columny.x},
        {columnx.y, columny.y}
    };
}

// Get the inverse of this matrix
template <ScalarReal T>
constexpr auto mat22<T>::inverse() const -> mat22 {
    auto det = determinant();
    auto d = 1 / det;
    return {
        {  columny.y * d, -columnx.y * d},
        { -columny.x * d,  columnx.x * d}
    };
}


#endif // ifndef BOOMED_MATH_MAT22_H_
