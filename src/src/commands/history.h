#ifndef BOOMED_COMMANDS_HISTORY_H_
#define BOOMED_COMMANDS_HISTORY_H_

#include "commands/cmd.h"
#include <cstdint>
#include <string_view>
#include <vector>


class history {
public:
    history();
    auto can_undo() const -> bool;
    auto can_redo() const -> bool;
    auto current_name() const -> std::string_view;
    auto undo() -> void;
    auto redo() -> void;

    class transaction {
    public:
    };

private:
    std::vector<cmd_factory::unique_cmd> commands{};
    std::size_t cursor{0};
    bool transaction_active{false};
};

#endif // ifndef BOOMED_COMMANDS_HISTORY_H_
