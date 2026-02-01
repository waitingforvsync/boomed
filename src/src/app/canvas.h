#ifndef BOOMED_APP_CANVAS_H_
#define BOOMED_APP_CANVAS_H_

#include "platform/canvas.h"


class canvas : public platform::canvas {
public:
    canvas(const char* title, vec2<std::int32_t> size);
};


#endif // ifndef BOOMED_APP_CANVAS_H_
