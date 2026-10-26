// high case 8 GENBCC (packet p67-5). Spec: docs/re/commands/GENBCC.md. The object-level work is
// in genbcc_bc.cpp.
#include <defcmd/p67_5.hpp>

#include <cstdint>

#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>

#include "genbcc_bc.hpp"

namespace def::cmd {

// part of @src 140378e00: dispatcher high case 8 (listing 14037d376..14037d3fa, epilogue
// 14037e1fd): GENBCC <objNo>
int cmd_genbcc(model::DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t no = 0;                        // RSP+0x64d8
    const int st = key::read_i(f, &no, false);  // 140413de0(f, &no, 0), stored at RSP+0x12d4
    if (st != 0) return 2;
    if (key::g2obj::find_object(m, no) == nullptr) return 2;  // 1401d6ea0
    genbcc::regenerate_object(m, &no);  // 1401ac6c0, result ignored
    return st != 0 ? 2 : 0;             // 14037e1fd: the stored read status (0 here)
}

} // namespace def::cmd
