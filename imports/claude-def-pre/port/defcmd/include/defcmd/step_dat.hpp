// defcmd/step_dat.hpp: the STEP.DAT writer FUN_1402ac8a0 (packet p9-ac8a0).
//
// STEP.DAT is DEF_INT's input when it interpolates without a DB: the 2D source object's quad mesh
// and its state variables. 1402ac8a0 also writes DEF_INT_NO_DB.DAT (six component widths).
// Only caller: the CNVT3D member 140146050 ("Interpolate SV On 2D Quad Mesh ..."), reached from
// the CNVT3D driver 140147b70 through 140148960 (brick path) and 140148a20 (tet path), with
// m = cvt+0x3f48 (the 2D model) and obj = cvt+0x10 (the 2D source object).
//
// The arrays come from the allocate-on-access accessors of the object's node set (obj+0x8a0)
// and element set (obj+0x9e0); the bare matrices from the KEY writer's 1402f7490 / 1402a7b60
// (defkey/kw_g2sv.hpp). Status: docs/re/status/p9-ac8a0.tsv.
#pragma once

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}  // namespace def::model

namespace def::cmd::step_dat {

// @src 1402ac8a0  STEP.DAT ("wt"): db name, m+0x840, m+0x744, object number, then the mesh and
// state-variable blocks; DEF_INT_NO_DB.DAT ("wt"): six widths. Nothing when obj is null or
// STEP.DAT cannot be opened.
void write_step_dat(def::model::DB_STEP_DATA* m, const char* db, def::model::LLOBJ_DATA* obj);

}  // namespace def::cmd::step_dat
