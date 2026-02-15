#ifndef BOOMED_UTILS_OVERLOADED_H_
#define BOOMED_UTILS_OVERLOADED_H_

// Until this finally makes its way into the standard...
template <typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


#endif // ifndef BOOMED_UTILS_OVERLOADED_H_
