#include "app/app.h"
#include "commands/cmd_add_vertex.h"
#include "commands/cmd_sentinel.h"


app::app(const char* title, vec2<std::int32_t> size) :
    platform::app{title, size}
{
    // test
    auto pool = std::pmr::unsynchronized_pool_resource{};
    auto cf = cmd_factory{&pool};
    auto c = cf.make<cmd::add_vertex>();
    auto s = cf.make<cmd::sentinel>("hello");
}


auto app::render() const -> void
{
//    app_canvas.clear(0xFF998C72);
//    app_canvas.present();
}
