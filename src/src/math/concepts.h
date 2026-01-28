#ifndef BOOMED_MATH_CONCEPTS_H_
#define BOOMED_MATH_CONCEPTS_H_

#include <concepts>

template <typename T>
concept ScalarReal = std::floating_point<T>;

template <typename T>
concept Scalar = ScalarReal<T> || std::signed_integral<T>;


#endif // ifndef BOOMED_MATH_CONCEPTS_H_
