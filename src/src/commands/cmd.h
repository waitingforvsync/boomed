#ifndef BOOMED_COMMANDS_CMD_H_
#define BOOMED_COMMANDS_CMD_H_

#include <concepts>
#include <memory>
#include <memory_resource>


namespace cmd {
    // Interface for all commands
    struct base {
        virtual ~base() = default;
        virtual auto apply() -> void = 0;
        virtual auto undo() -> void = 0;
        virtual auto is_end() const -> bool { return false; }
    };
}


template <typename T>
concept CmdType = std::derived_from<T, cmd::base>;


class cmd_factory {
public:
    // Construct factory with pool resource
    explicit cmd_factory(std::pmr::unsynchronized_pool_resource* p) : pool(p) {}

    struct deleter {
        using destroy_fn = void (*)(cmd::base*, std::pmr::memory_resource*) noexcept;
        std::pmr::memory_resource* mr;
        destroy_fn destroy{};

        auto operator ()(cmd::base* c) const noexcept -> void {
            if (c) destroy(c, mr);
        }
    };

    using unique_cmd = std::unique_ptr<cmd::base, deleter>;


    // Make a cmd of the given template argument type
    template <CmdType T, typename... Args>
    unique_cmd make(Args&&... args) {
        auto alloc = std::pmr::polymorphic_allocator<T>{pool};
        T* c = alloc.allocate(1);
        try {
            std::construct_at(c, pool, std::forward<Args>(args)...);
        }
        catch (...) {
            alloc.deallocate(c, 1);
            throw;
        }
        return unique_cmd{c, deleter{pool, destroy<T>}};
    }

private:
    // Destroy a cmd of the given template argument type
    template <CmdType T>
    static void destroy(cmd::base* base, std::pmr::memory_resource* mr) noexcept {
        auto* c = static_cast<T*>(base);
        std::destroy_at(c);
        auto alloc = std::pmr::polymorphic_allocator<T>{mr};
        alloc.deallocate(c, 1);
    }

    std::pmr::unsynchronized_pool_resource* pool;
};


#endif // BOOMED_COMMANDS_CMD_H_
