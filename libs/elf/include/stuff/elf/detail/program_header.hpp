#pragma once

#include <stuff/elf/detail/consumer.hpp>

namespace stf::elf {

enum class program_segment_type : u32 {
    null = 0, //!< PT_NULL
    load = 1, //!< PT_LOAD
    dynamic = 2, //!< PT_DYNAMIC
    interp = 3, //!< PT_INTERP
    note = 4, //!< PT_NOTE
    shlib = 5, //!< PT_SHLIB
    phdr = 6, //!< PT_PHDR
};

struct program_header {
    u32 type;
    u32 flags;

    u64 file_offset;

    u64 virtual_address;
    u64 physical_address;

    u64 file_size;
    u64 memory_size;

    u64 alignment;

    static constexpr auto from_consumer(detail::data_consumer& consumer) -> stf::expected<program_header, std::string_view> {
        auto ret = program_header{
          .type = consumer.consume<u32>(),
        };

        if (consumer.is_64) {
            ret.flags = consumer.consume<u32>();
        }

        ret.file_offset = consumer.consume_address();
        ret.virtual_address = consumer.consume_address();
        ret.physical_address = consumer.consume_address();
        ret.file_size = consumer.consume_address();
        ret.memory_size = consumer.consume_address();

        if (!consumer.is_64) {
            ret.flags = consumer.consume<u32>();
        }

        ret.alignment = consumer.consume_address();

        if (const auto err = ret.get_error(); err) {
            return stf::unexpected{*err};
        }

        return ret;
    }

    constexpr auto is_read() const -> bool { return (flags & 0b100) != 0; }
    constexpr auto is_write() const -> bool { return (flags & 0b010) != 0; }
    constexpr auto is_execute() const -> bool { return (flags & 0b001) != 0; }

private:
    constexpr auto get_error() const -> std::optional<std::string_view> { return std::nullopt; }
};

}
