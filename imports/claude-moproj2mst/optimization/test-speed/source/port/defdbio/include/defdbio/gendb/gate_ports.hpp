// defdbio/gendb/gate_ports.hpp: small functions the GENDB packets call that no packet had ported
// when Phase 4 was wired together (Phase 4 gate). One port each; the packets' ext layers (p4-05
// gendb3d/ext*.hpp, p4-02a ObjCountExt, p4-02b counts_ext, p4-06 ext_p4_06) call these.
// Status: docs/re/status/p4-gate.tsv. Decompiles: deform-rea-work/out/p4g3d_new.c.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary.hpp>

namespace def::model {
class DB_STEP_DATA;
class OBJ_GEO3D;
} // namespace def::model

namespace def::dbio::gendb {

// ---- DYNARY lengths in LLFNC words -----------------------------------------------------------
// 140130200  1D: type 0x7c -> ncomp, else n[0] << 1
std::int32_t dynary_words_1d(const model::DYNARY* t);
// 140130160  2D: n[0] + n[1] + max(n[0],1) * max(n[1],1) * max(ncomp,1)
std::int32_t dynary_words_2d(const model::DYNARY* t);
// 14012fff0  3D: n[0] + n[1] + n[2] + max(n[0],1) * max(n[1],1) * max(n[2],1)
std::int32_t dynary_words_3d(const model::DYNARY* t);
// 140130290  by ndim: 0 -> ncomp for the types 0xa7, 0xa8, 0xaa, 0xab, 0xad, 0xb3, 0xb5 (else 0);
//            1 / 2 / 3 -> the functions above; any other ndim -> 0
std::int32_t dynary_words(const model::DYNARY* t);

// ---- Cls_1401860a0 (LLOBJ_DATA+0x1ea0) list counts -------------------------------------------
// 14019a530  count of the body-force list at +0x200 (14019a550: +0x10)
std::int32_t body_force_list_count(void* self);
// 14019a560  count of the SfLinkedList at +0x218 (14019a580: +0x18)
std::int32_t list218_count(void* self);

// ---- LLWINDEF_DATA -----------------------------------------------------------------------------
// 14025c830  the window's owner (+0x888) is set and 2D (1400015e0)
bool window_owner_is_2d(void* w);

// ---- Cls_14018b220 (LLOBJ_DATA+0x2160) ---------------------------------------------------------
// 140194d40  the 3x3 frame at +0x4c8..+0x510, transposed, into frame[0..8]; returns 1
std::uint64_t movement_frame(const void* mv, double* frame);

// ---- DB_STEP_DATA ------------------------------------------------------------------------------
// 1401d2bb0  1 (both branches on the byte at +0x1b59 return 1)
bool step_fun_1401d2bb0(const model::DB_STEP_DATA* m);
// 1401d5820  read MAR regions on demand (-1 all, 0..6 one group); 1401d2bb0 is always 1 in this
//            build, so it does nothing and returns 0
void* step_read_mar_on_demand(model::DB_STEP_DATA* m, const std::int32_t* what);

// ---- OBJ_GEO3D ---------------------------------------------------------------------------------
// 1402f97c0  coords.resize(*n * 3, 0)
void geo3d_resize_coords(model::OBJ_GEO3D* g, const std::int32_t* n);
// 1402f9810  elems.resize(*n << 2, 0), element flags reallocated keeping the old ones,
//            elem_ids.resize(*n, 0)
void geo3d_resize_elements(model::OBJ_GEO3D* g, const std::int32_t* n);
// 1402f9780  1402f97c0(nodes), then 1402f9810(elems)
void geo3d_resize(model::OBJ_GEO3D* g, const std::int32_t* nodes, const std::int32_t* elems);
// 1402fcfe0  surface patch count: rebuild the +0x40 list first (1402fced0) when *rebuild
std::int32_t geo3d_surface_patch_count(model::OBJ_GEO3D* g, const char* rebuild);

} // namespace def::dbio::gendb
