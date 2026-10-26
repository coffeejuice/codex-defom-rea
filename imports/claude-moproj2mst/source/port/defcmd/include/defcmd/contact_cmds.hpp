// defcmd/contact_cmds.hpp: the contact command keywords INICTC (registry id 5) and GENCTC (id 6),
// cases 5 and 6 of the command dispatcher FUN_140378e00, and the C++ functions they call.
// Specs: docs/re/commands/INICTC.md, GENCTC.md (branch claude/project-thread-6cvah9).
// The dispatcher itself is wired at the P6-7 integration gate; it calls cmd_inictc / cmd_genctc.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Cls_1401f46e0;
} // namespace def::model

namespace def::cmd {

// part of @src 140378e00, case 5: "INICTC n". Reads the rest of the keyword line with "%i"
// (140413de0, no blank-line skip), looks object n up (1401d6ea0) and clears its contact codes.
// Returns 0, or 2 (read error or no such object: "Reading Error").
int cmd_inictc(def::model::DB_STEP_DATA* model, std::FILE* f);
// part of @src 140378e00, case 6: "GENCTC tol". Reads "%Flf" (140414990); 2D models run
// genctc_2d, others genctc_3d. Returns 0, or 2 on a read error.
int cmd_genctc(def::model::DB_STEP_DATA* model, std::FILE* f);

// @src 1402049b0  set every negative BCCDEF code of the object to 0 (2 codes per node in 2D, 3 in 3D)
void inictc_clear(def::model::LLOBJ_DATA* obj);

// @src 1401a2e90  2D GENCTC over the inter-object pairs; returns -2 when some pair could not be
// committed without a warning, else 0 (unused by the dispatcher)
std::int32_t genctc_2d(def::model::DB_STEP_DATA* model, double tol);
// @src 1401a3160  one pair (master m, slave s); returns true when the moved nodes and new codes
// were committed
bool genctc_2d_pair(def::model::DB_STEP_DATA* model, def::model::LLOBJ_DATA* m,
                    def::model::LLOBJ_DATA* s, def::model::Cls_1401f46e0* pair, double tol,
                    std::int32_t* fail, std::int32_t unused);
// @src 1401a6c80  3D GENCTC over the objects (search by packet p67-7, defcmd/genctc3d_ext.hpp)
void genctc_3d(def::model::DB_STEP_DATA* model, double tol);

// @src 140202a20  smallest element edge length of the object (2D 1403046a0, 3D 140202a70)
double min_edge_length(def::model::LLOBJ_DATA* obj);
// @src 1403046a0  2D: smallest element edge of the quad mesh (0 without mesh)
double min_edge_length_2d(def::model::LLOBJ_DATA* obj);
// @src 140202a70  3D: smallest edge of the boundary faces of surface 0 (0 without mesh)
double min_edge_length_3d(def::model::LLOBJ_DATA* obj);
// @src 140202da0  smallest element bounding-box size (3D 140202e00; 2D prints a message, 0)
double element_box_min(def::model::LLOBJ_DATA* obj);
// @src 140202e00  3D: min over elements of the largest bounding-box extent (-1 without mesh)
double element_box_min_3d(def::model::LLOBJ_DATA* obj);

// @src 1401808d0  the coordinate array of a node set (obj+0x8a0), allocated / read from the DB on
// demand
double* node_set_coords(void* set);
// @src 140183cb0  the connectivity array of an element set (obj+0x9e0), read from the DB on demand
std::int32_t* element_set_conn(void* set);

} // namespace def::cmd
