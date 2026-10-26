// defcmd/meshgen/p8_seams.hpp: functions that packets p8-05 (BRDEXT), p8-06 (3D solid mesh chain),
// p8-07 (DEFAMG, _MeshGenBase) and p8-08 (DEFINT, DEFFRZ, REMESH) call in each other.
// Settled by the p8-05..p8-08 thread before the packets started; the owner of each function
// defines it in its own sources and deletes its stub file port/defcmd/src/p8_pending/<packet>.cpp
// (a stub and a real definition of one symbol do not link, so the delete cannot be forgotten).
// A signature change goes through the thread, never one packet alone.
#pragma once

#include <cstdint>
#include <string>

#include <defcmd/file_util.hpp>   // 1400b9130 write_text_file (p8-07)
#include <defcmd/launch.hpp>      // 140223790 launcher, 140224100 / 140224130 (p8-07)
#include <defcmd/meshgen/meshgen_base.hpp>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// ---- p8-05 BRDEXT ---------------------------------------------------------------------------
// @src 1401a9ac0  extract the border of object *obj (REMESH step 1); false when it is missing
bool brdext_object(model::DB_STEP_DATA& m, const std::uint32_t* obj);

// ---- p8-06 3D solid mesh chain --------------------------------------------------------------
// @src 140140110  _MeshGenBase slot +0x10: 3D volume step (MESH3D_GUI / BRKMSH / CHKVOL)
int mesh3d_volume_step(MeshGenBase& mg, char flag);

// ---- p8-07 DEFAMG and mesher input files ----------------------------------------------------
// @src 1401a9be0  generate the mesh of object *obj (DEFAMG, REMESH step 2)
bool defamg_object(model::DB_STEP_DATA& m, const std::uint32_t* obj);
// @src 14013f410  (p8-06 caller 140141d50 passes 1, 0)
int fun_14013f410(MeshGenBase& mg, char a, char b);
// @src 140141580  NBCD.OUT (called by 140140110 when its flag is set and DAT_1406e2d78 == 0)
int fun_140141580(MeshGenBase& mg);
// @src 1401443c0
void fun_1401443c0(MeshGenBase& mg);
// @src 1401444f0
void fun_1401444f0(MeshGenBase& mg);

// ---- p8-08 DEFINT, DEFFRZ, REMESH -----------------------------------------------------------
// @src 14013e880  delete the exchange files of DEF_AMG / DEF_INT / DEF_REM (flag: arg 2)
void exchange_cleanup(MeshGenBase& mg, bool flag);
// @src 1401b17b0  std::string from the literal at 140524d24
std::string fun_1401b17b0();
// @src 1401b1520  s truncated to n characters when 0 < n < length
std::string fun_1401b1520(std::string s, int n);

} // namespace def::cmd
