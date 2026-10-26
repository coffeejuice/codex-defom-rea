// defcmd/cmd_p8_08.hpp: packet p8-08, the REMESH, DEFINT and DEFFRZ command keywords (registry
// 291, 294, 296; dispatcher cases 0x11f+4, 0x11f+7, 0x11f+9 of FUN_140378e00). Included by the
// dispatcher (p5-01) through __has_include. Specs: docs/re/commands/REMESH.md, DEFINT.md,
// DEFFRZ.md. Internals (interpolation drivers, exchange files, read-back, model copy) are in
// defcmd/interp.hpp.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// part of @src 140378e00  case 0x11f+4 (REMESH): "%i" (140413de0, skip_blank 0); reader failure
// -> 2; object missing (1401d6ea0) -> 2; otherwise 1401a9a10 (result ignored) and 0.
int cmd_remesh(def::model::DB_STEP_DATA* m, std::FILE* f);
// part of @src 140378e00  case 0x11f+7 (DEFINT): as REMESH, handler 1401aa000.
int cmd_defint(def::model::DB_STEP_DATA* m, std::FILE* f);
// part of @src 140378e00  case 0x11f+9 (DEFFRZ): "%i %i %Flf" (140415f00); reader failure -> 2;
// object A or object B missing -> 2; otherwise 1401aa370 (result ignored) and 0.
int cmd_deffrz(def::model::DB_STEP_DATA* m, std::FILE* f);

// @src 1401a9a10  REMESH handler: BRDEXT (1401a9ac0), DEFAMG (1401a9be0), DEFINT (1401aa000)
bool remesh_object(def::model::DB_STEP_DATA& m, const std::uint32_t* obj);
// @src 1401aa000  DEFINT handler: interpolate the DB state (model+0x1748, step +0x744) onto obj
bool defint_object(def::model::DB_STEP_DATA& m, const std::uint32_t* obj);
// @src 1401aa370  DEFFRZ handler: force interpolation of source object *b onto target *a
bool deffrz_object(def::model::DB_STEP_DATA& m, const std::uint32_t* a, const std::uint32_t* b,
                   const double* value);

} // namespace def::cmd
