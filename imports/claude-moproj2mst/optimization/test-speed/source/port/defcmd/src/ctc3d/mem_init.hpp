// Initial-value table of the static image (defcmd/ctc3d/common.hpp), generated into mem_init.cpp.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::cmd::ctc3d::detail {

struct MemInit {
    std::uint64_t addr;
    std::uint32_t len;
    const char* bytes;
};

extern const MemInit k_mem_init[];
extern const std::size_t k_mem_init_count;

} // namespace def::cmd::ctc3d::detail
