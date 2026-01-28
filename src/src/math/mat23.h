#ifndef BOOMED_MATH_MAT23_H_
#define BOOMED_MATH_MAT23_H_

#include "math/mat22.h"


template <ScalarReal T>
struct mat23 {
    mat22<T> rot{};
    vec2<T> trans{};

    using scalar_result = T;

    // Constructors
    constexpr mat23() = default;
    constexpr mat23(mat22<T> m, vec2<T> v);
    template <ScalarReal U> constexpr mat23(mat23<U> m);

    // Named constructors
    static constexpr auto make_translation(vec2<T> t) -> mat23;

    // Member operations
    constexpr auto inverse() const -> mat23;
};


// Construct a mat23 from a mat22 and a translation
template <ScalarReal T>
constexpr mat23<T>::mat23(mat22<T> m, vec2<T> t) : rot{m}, trans{t} {}

// Implicitly construct a mat23 from a different type of mat23
template <ScalarReal T>
template <ScalarReal U>
constexpr mat23<T>::mat23(mat23<U> m) : rot{m.rot}, trans{m.trans} {}

// Make a translation matrix
template <ScalarReal T>
constexpr auto mat23<T>::make_translation(vec2<T> t) -> mat23 {
    return {mat22<T>::identity(), t};
}

// mat23 * vec2
template <ScalarReal T>
constexpr auto operator *(mat23<T> m, vec2<T> v) -> vec2<T> {
    return m.rot * v + m.trans;
}

// mat23 * mat23
template <ScalarReal T>
constexpr auto operator *(mat23<T> a, mat23<T> b) -> mat23<T> {
    return {a.rot * b.rot, a * b.trans};
}

// mat22 * mat23
template <ScalarReal T>
constexpr auto operator *(mat22<T> a, mat23<T> b) -> mat23<T> {
    return {a * b.rot, a * b.trans};
}

// mat23 * mat22
template <ScalarReal T>
constexpr auto operator *(mat23<T> a, mat22<T> b) -> mat23<T> {
    return {a.rot * b, a.trans};
}

// Inverse
template <ScalarReal T>
constexpr auto mat23<T>::inverse() const -> mat23<T> {
	auto mi = rot.inverse();
    return {mi, -mi * trans};
}


#endif // ifndef BOOMED_MATH_MAT23_H_
