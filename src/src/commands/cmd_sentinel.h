#ifndef BOOMED_COMMANDS_CMD_SENTINEL_H_
#define BOOMED_COMMANDS_CMD_SENTINEL_H_

#include "commands/cmd.h"


namespace cmd {
    struct sentinel : public base {
        explicit sentinel(std::pmr::memory_resource& mr) {}

        auto apply() -> void override {}
        auto undo() -> void override {}
        auto is_end() const -> bool override { return true; }
    };
}


#endif // BOOMED_COMMANDS_CMD_SENTINEL_H_
