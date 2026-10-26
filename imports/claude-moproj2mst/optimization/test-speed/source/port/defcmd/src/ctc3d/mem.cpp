// Static image of the 3D contact engine (defcmd/ctc3d/common.hpp).
#include <defcmd/ctc3d/common.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "mem_init.hpp"

namespace def::cmd::ctc3d {

namespace {

alignas(4096) std::byte s_data[k_data_hi - k_data_lo];
alignas(4096) std::byte s_ro[k_ro_hi - k_ro_lo];

void apply_init()
{
    for (std::size_t i = 0; i < detail::k_mem_init_count; ++i) {
        const detail::MemInit& e = detail::k_mem_init[i];
        std::memcpy(image_addr(e.addr), e.bytes, e.len);
    }
}

// The initial values are in place before main (static storage starts zeroed).
const bool s_init_done = (apply_init(), true);

} // namespace

std::byte* const g_image_data = s_data;
std::byte* const g_image_ro = s_ro;

void image_bad_address(std::uint64_t a)
{
    std::fprintf(stderr, "def_pre port: ctc3d image access outside the static sections: 0x%llx\n",
                 static_cast<unsigned long long>(a));
    std::abort();
}

void reset_image()
{
    std::memset(s_data, 0, sizeof(s_data));
    std::memset(s_ro, 0, sizeof(s_ro));
    apply_init();
    (void)s_init_done;
}

} // namespace def::cmd::ctc3d
