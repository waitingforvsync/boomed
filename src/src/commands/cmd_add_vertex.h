#ifndef BOOMED_COMMANDS_CMD_ADD_VERTEX_H_
#define BOOMED_COMMANDS_CMD_ADD_VERTEX_H_

#include "commands/cmd.h"


namespace cmd {
    struct add_vertex;
}


struct cmd::add_vertex : public cmd::base {
    explicit add_vertex(std::pmr::memory_resource& mr) {}

    void apply() override {
    }

    void undo() override {
    }
};


#endif // BOOMED_COMMANDS_CMD_ADD_VERTEX_H_
