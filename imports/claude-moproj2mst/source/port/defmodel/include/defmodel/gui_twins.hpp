// defmodel/gui_twins.hpp: DEF_PRE_64 model utilities whose byte-identical twins the DEF_GUI_MO_64
// operation loader calls (docs/re/moproj2mst/mdt/L0/spec_A.md, spec_B.md, spec_C.md). None of
// these addresses is in the DEF_PRE batch ledger; status rows in docs/re/status/mdt-model.tsv.
// Bodies in src/gui_twins/. Decompiles: deform-rea-work/out/mdt3/f.c, g.c.
#pragma once

#include <cstdint>

namespace def::model {

class Cls_14017a6c0;
class DB_STEP_DATA;
class LLOBJ_DATA;
class Material;
class PhasePair;

// ---- step model (step_utils.cpp) --------------------------------------------------------------

// 1401cb400: clear the model and initialise it again for its own dimension: dim = +0x1c
// (140158e90), step_free_contents (1401cb440), step_init(dim) (1401c8c00).
void step_clear(DB_STEP_DATA& s);

// 1401cabc0: number (+8) of the first object 1..u1378 (1401d6ed0) that exists and whose kind
// (+0xc) is not 7 (140160f10); 0 when there is none.
std::int32_t first_real_object_number(DB_STEP_DATA& s);

// 14017ac20: the smallest number n >= 1 with no object of number n in the list (14017aaf0).
std::int32_t first_free_object_number(const Cls_14017a6c0* list);

// 1401d6df0: number (+8) of the list's tail object (14017ab50 returns list+8, the tail) when
// u1378 >= 1 and the tail exists, else 0.
std::int32_t last_object_number(const DB_STEP_DATA& s);

// ---- objects (object_utils.cpp) ---------------------------------------------------------------

// 140161770: copy src into dst with 1401613a0 (flags p4..p9 = 1, 1, 1, 1, 0, 1; sizes = the owner's
// table +0x15f8) and keep dst's number (+8). False (nothing done) when dst has no owner.
bool llobj_copy_keep_number(LLOBJ_DATA* dst, const LLOBJ_DATA* src);

// 140160f50: kind 7 becomes 0, then the unit-dependent defaults (14015f9a0) for `unit`.
void llobj_reset_kind7(LLOBJ_DATA* obj, std::int32_t unit);

// 140188c90: assignment of the 0x128-byte block Cls_1401889a0 (LLOBJ_DATA+0x2be8), member by member.
void cls_1401889a0_assign(void* dst, const void* src);

// 1401981f0: copy the list object at +0x3998 of src into dst's (1403019d0; no null check of dst's).
void llobj_copy_u3998_list(LLOBJ_DATA* dst, const LLOBJ_DATA* src);

// ---- BCC groups (bcc_groups.cpp) --------------------------------------------------------------

// 140165eb0: BC code of node `node` (1-based): the first negative entry of the node's row in the
// int array obj+0x990 (row width tbl[0x12], the int at tbl+0x48), negated; 201..299 lose 200;
// anything outside 0..100 gives 0. node < 1 gives 0.
std::int32_t node_bc_code(const LLOBJ_DATA* obj, std::int32_t node, const std::int32_t* tbl);

// 140207f50: clear BC entries of the listed nodes (1-based) by mode `code` in the arrays
// a = obj+0x990-like (row width *w_a), b (row width *w_b), c (doubles). See the source for the
// mode cases (3D owner and code 4..8, 1, 0, < 0, 3/11/12).
void bcc_clear_nodes(const LLOBJ_DATA* obj, const std::int32_t* nodes, std::int32_t n,
                     std::int32_t* a, std::int32_t* b, double* c, std::int32_t code,
                     const std::int32_t* w_a, const std::int32_t* w_b, std::int32_t comp,
                     std::int32_t value);

// 14020a340: group the object's nodes by BC code (140165eb0) and clear the negative codes again:
// 3D objects (140158d00 false) through 140206fd0 mode 1 (= 140207f50 with code -c), 2D objects
// by zeroing every negative entry of the grouped nodes. tbl is the model's +0x15f8 table
// (null: nothing).
void bcc_groups_from_model(LLOBJ_DATA* obj, const std::int32_t* tbl);

// ---- inter-material (phase_mix.cpp) -----------------------------------------------------------

// 140339ba0: true when `mat` is a mixture (+0x1cc8 == 1), the pair is active (+0xc == 1) and
// both of the pair's materials (from, to) are in the mixture's list.
bool phase_pair_in_mixture(const PhasePair* pair, const Material* mat);

} // namespace def::model
