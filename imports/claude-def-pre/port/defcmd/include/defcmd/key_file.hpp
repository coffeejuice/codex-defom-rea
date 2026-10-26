// defcmd/key_file.hpp: the KEY file loader (FUN_1401b5f00), owner p8-07; used by p5-01 (DEF_MULTI.INI,
// DEF_PRE.INI, menu 2) and the 2D mesher read-back (DEFAMG/REMESH). The KEY loop 1401b6520 is
// def::key::read_key_stream; its model side (ModelKeyHooks, <defcmd/dispatch.hpp>) is p5-01's.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// @src 1401b5f00  read keyword file `name` into model m. *reset != 0: 1401d7f10 before and
// 1401d7f40 after the read. m+0x2f8a = name while reading, " " afterwards; m+0x1740 = *flags while
// reading. Returns 1 when the file cannot be opened (message on stdout), else the loop's status.
int read_keyword_file(def::model::DB_STEP_DATA* m, const char* name, const char* reset,
                      const std::int32_t* flags);

// Test seam: when set, runs the KEY loop (1401b6520) on the open file instead of ModelKeyHooks
// (default null = the real loop; the program build never sets it).
using KeyStreamHook = int (*)(def::model::DB_STEP_DATA* m, std::FILE* f);
inline KeyStreamHook g_key_stream_hook = nullptr;

} // namespace def::cmd
