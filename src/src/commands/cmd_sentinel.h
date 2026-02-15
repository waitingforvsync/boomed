#ifndef BOOMED_COMMANDS_CMD_SENTINEL_H_
#define BOOMED_COMMANDS_CMD_SENTINEL_H_

#include "commands/cmd.h"
#include <string>
#include <string_view>


namespace cmd {
    struct sentinel : public base {
        explicit sentinel(std::pmr::memory_resource* mr, std::string_view cmd_name) :
            name{cmd_name, mr}
        {}

        auto apply() -> void override {}
        auto undo() -> void override {}
        auto is_end() const -> bool override { return true; }

        std::pmr::string name{};
    };
}


#endif // BOOMED_COMMANDS_CMD_SENTINEL_H_
