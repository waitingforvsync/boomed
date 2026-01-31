#include "app/app.h"


auto app::render() const -> void
{
    disp.clear(0xFF998C72);
    disp.present();
}
