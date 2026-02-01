#include "app/app.h"


auto app::render() const -> void
{
    app_canvas.clear(0xFF998C72);
    app_canvas.present();
}
