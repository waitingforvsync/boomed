#ifndef BOOMED_APP_DISPLAY_H_
#define BOOMED_APP_DISPLAY_H_

#include "platform/display.h"


class display : public platform::display {
public:
    display(const char* title, vec2<std::int32_t> size);
};


#endif // ifndef BOOMED_APP_DISPLAY_H_
