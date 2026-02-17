#ifndef BOOMED_APP_APP_H_
#define BOOMED_APP_APP_H_

#include "platform/app.h"
#include "math/vec2.h"


class app : public platform::app {
public:
    app(const char* title, vec2<std::int32_t> size);
    auto render() const -> void;
};


#endif // ifndef BOOMED_APP_APP_H_
