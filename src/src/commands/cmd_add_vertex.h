#ifndef BOOMED_COMMANDS_CMD_ADD_VERTEX_H_
#define BOOMED_COMMANDS_CMD_ADD_VERTEX_H_

#include "commands/cmd.h"


namespace cmd {
    struct add_vertex : public base {
        explicit add_vertex(std::pmr::memory_resource* mr) {}

        auto apply() -> void override {
        }

        auto undo() -> void override {
        }
    };
}


#endif // BOOMED_COMMANDS_CMD_ADD_VERTEX_H_
