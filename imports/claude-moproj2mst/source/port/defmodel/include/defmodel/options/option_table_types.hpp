#pragma once
// Static option-label tables (packet p1-02). In DEF_PRE_64.exe each table is a global array of
// VS2010 std::string (0x28 bytes per element) that a C++ dynamic initialiser fills from .rdata
// literals; the port keeps them as constexpr data generated from the binary
// (port/defmodel/scripts/gen_option_tables.py) and the addresses each label came from, so the
// tier-1 test can re-read the binary and confirm every entry.

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace def::model::options {

inline constexpr int kOptionTableCount = 38;  // initialisers 0x140501bd0-0x1405036a0

struct OptionLabel {
    std::string_view text;        // bytes passed to std::string::assign, with its length
    std::uint64_t object;         // the std::string element in .data/.bss
    std::uint64_t label_addr;     // the literal in .rdata
    std::uint64_t code;           // the instruction that loads the length (mov r8d / lea r8d)
};

struct OptionTable {
    const char* name;             // inferred from the labels; no caller names the table
    std::uint64_t init;           // static initialiser
    std::uint64_t data;           // first element (@data)
    std::span<const OptionLabel> labels;
};

}  // namespace def::model::options
