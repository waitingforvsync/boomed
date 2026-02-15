#include "app/app.h"
#include "commands/cmd_add_vertex.h"
#include "commands/cmd_sentinel.h"

auto app::render() const -> void
{
    auto pool = std::pmr::unsynchronized_pool_resource{};
    auto cf = cmd_factory{&pool};
    auto c = cf.make<cmd::add_vertex>();
    auto s = cf.make<cmd::sentinel>("hello");
    app_canvas.clear(0xFF998C72);
    app_canvas.present();
}
