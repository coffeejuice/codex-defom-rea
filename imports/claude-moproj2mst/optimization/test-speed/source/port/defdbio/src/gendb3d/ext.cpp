// gendb3d/ext.cpp: dispatch of the p4-05 external-call wrappers (see ext.hpp).
#include "gendb3d/ext.hpp"

#include <defrt/port.hpp>

#include <vector>

namespace def::dbio::g3d::ext {

bool hooked(std::uint64_t addr, std::initializer_list<std::uint64_t> args, std::uint64_t* result)
{
    if (!g_hook) return false;
    std::vector<std::uint64_t> v(args);
    *result = 0;
    return g_hook(addr, v.data(), static_cast<int>(v.size()), result);
}

std::uint64_t call(std::uint64_t addr, std::initializer_list<std::uint64_t> args)
{
    std::uint64_t r = 0;
    if (hooked(addr, args, &r)) return r;
    DEF_NOT_PORTED(addr);
}

} // namespace def::dbio::g3d::ext
