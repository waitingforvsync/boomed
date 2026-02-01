#ifndef BOOMED_APP_APP_H_
#define BOOMED_APP_APP_H_

#include "platform/app.h"
#include "app/canvas.h"
#include "math/vec2.h"


class app : public platform::app {
public:
    auto render() const -> void;

private:
    canvas app_canvas{"BoomEd", vec2<std::int32_t>{1280, 960}};
};


#endif // ifndef BOOMED_APP_APP_H_
