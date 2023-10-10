#pragma once

#include <stuff/core.hpp>

#include <expected>
#include <span>

namespace stf::elf {

enum class elf_class : u8 {
    class_32 = 1,
    class_64 = 2,
};

enum class data_format : u8 {
    le_two = 1,
    be_two = 2,
};

enum class os_abi : u8 {
    system_v = 0x00,
    hp_ux = 0x01,
    net_bsd = 0x02,
    abi_linux = 0x03,  // the abi_ prefix is because of some retarded `linux` define
    solaris = 0x06,
    irix = 0x08,
    free_bsd = 0x09,
    tru_64 = 0x0a,
};

struct identification {
    std::array<u8, 4> magic;          //!< e_ident[EI_MAG0..3]
    elf_class ei_class;               //!< e_ident[EI_CLASS]
    data_format ei_data;              //!< e_ident[EI_DATA]
    u8 version;                       //!< e_ident[EI_VERSION]
    u8 os_abi;                        //!< e_ident[EI_OSABI]
    u8 abi_version;                   //!< e_ident[EI_ABIVERSION]
    std::array<u8, 7> ident_padding;  //!< e_ident[EI_PAD]

    template<usize Extent = std::dynamic_extent>
    static constexpr auto from_bytes(std::span<const u8, Extent> bytes) -> std::expected<identification, std::string_view> {
        if (bytes.size() < 16) {
            return std::unexpected{"not enough bytes for e_ident"};
        }

        auto ret = identification{
          .ei_class = bytes[0x04] == 2 ? elf_class::class_64 : elf_class::class_32,  // TODO: lenient ei_class and ei_data
          .ei_data = bytes[0x05] == 2 ? data_format::be_two : data_format::le_two,
          .version = bytes[0x06],
          .os_abi = bytes[0x07],
          .abi_version = bytes[0x08],
        };

        std::copy_n(bytes.begin(), 4, ret.magic.begin());
        std::copy_n(bytes.begin() + 0x09, 7, ret.ident_padding.begin());

        if (auto err = ret.get_error(); err) {
            return std::unexpected{*err};
        }

        return ret;
    }

private:
    constexpr auto get_error() const -> std::optional<std::string_view> {
        if (magic != std::array<u8, 4>{0x7f, 0x45, 0x4c, 0x46}) {
            return "bad magic value";
        }

        if (version != 1) {
            return "unsupported elf version";
        }

        return std::nullopt;
    }
};

}
