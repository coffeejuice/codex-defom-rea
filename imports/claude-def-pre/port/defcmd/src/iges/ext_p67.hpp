// p8-03 seam to the fatal-error printer 14039d450 (claude/p67-7, defcmd/ifd/cube_lib.hpp,
// namespace def::cmd::cube). Reached only from the "Error in type" branches of the text-route
// helpers, which no caller takes.
#pragma once

#include <cstdint>

#include <defrt/port.hpp>

#if __has_include(<defcmd/ifd/cube_lib.hpp>)
#include <defcmd/ifd/cube_lib.hpp>
#define DEF_P803_HAVE_P67 1
#endif

namespace def::cmd::iges::ext {

[[noreturn]] inline void f14039d450(const char* fmt, std::int32_t v)
{
#ifdef DEF_P803_HAVE_P67
    def::cmd::cube::fun_14039d450(fmt, v);
#else
    (void)fmt;
    (void)v;
    DEF_NOT_PORTED(0x14039d450);
#endif
}

} // namespace def::cmd::iges::ext
