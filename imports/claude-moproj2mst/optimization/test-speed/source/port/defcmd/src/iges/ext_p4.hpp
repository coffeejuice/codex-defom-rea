// p8-03 seam to the Phase 4 GENDB helpers (claude/p4-gate, merged into claude/p8-03): file removal
// 1401b85d0 (defdbio/gendb/gendb.hpp) and the Fortran string helpers 14044a4b1 / 1404b33e4
// (defdbio/src/gendb/db_header_io.hpp, packet p4-01).
#pragma once

#include <cstdint>

#include <defrt/port.hpp>

#if __has_include(<defdbio/gendb/gendb.hpp>) && __has_include("../../../defdbio/src/gendb/db_header_io.hpp")
#include <defdbio/gendb/gendb.hpp>
#include "../../../defdbio/src/gendb/db_header_io.hpp"
#define DEF_P803_HAVE_P4 1
#endif

namespace def::cmd::iges::ext {

// 1401b85d0 remove the file if it can be opened
inline std::int32_t remove_if_exists(const char* name)
{
#ifdef DEF_P803_HAVE_P4
    return def::dbio::gendb::remove_if_exists(name);
#else
    (void)name;
    DEF_NOT_PORTED(0x1401b85d0);
#endif
}
// 14044a4b1 ADJUSTL of s(1:n) (NULs count as blanks), returns the trimmed length
inline std::int32_t adjust_left(char* s, std::int32_t n)
{
#ifdef DEF_P803_HAVE_P4
    return def::dbio::gendb::adjust_left(s, n);
#else
    (void)s;
    (void)n;
    DEF_NOT_PORTED(0x14044a4b1);
#endif
}
// 1404b33e4 CHARACTER assignment dst(1:dlen) = src(1:slen)
inline void char_assign(char* dst, const char* src, std::int32_t dlen, std::int32_t slen)
{
#ifdef DEF_P803_HAVE_P4
    def::dbio::gendb::char_assign(dst, src, dlen, slen);
#else
    (void)dst;
    (void)src;
    (void)dlen;
    (void)slen;
    DEF_NOT_PORTED(0x1404b33e4);
#endif
}

} // namespace def::cmd::iges::ext
