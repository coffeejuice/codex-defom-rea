// defdbio/gendb/counts_ext.hpp: calls of packet p4-02b into functions that other packets own and
// that are not on its base branch (claude/p1-gate).
//
// One table of function pointers, set to defaults in src/gendb/p4_02b/counts_ext.cpp:
//   * interim copies of small functions (ported from the decompile, tagged "part of @src", to be
//     replaced by the owner's function when the branches meet);
//   * DEF_NOT_PORTED for the larger ones; tier-1 tests replace those entries.
// The merge points each entry at the owner's function (or replaces the call) and nothing else in
// p4-02b changes. reset_gendb_ext() restores the defaults (tests).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>

namespace def::dbio::gendb {

struct GendbExt {
    // ---- p4-02a (object counts, DYNARY lengths) -------------------------------------------
    // 1401beb30(m, &NFN, &LLFNC length, &u0c0, &u0c4, err): the object function counts
    void (*fun_1401beb30)(model::DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                          std::int32_t* nwin, std::int32_t* lwin, std::int32_t* err);
    // 140134120(t): prepares a 2D/3D table before its length is taken (2521 bytes)
    void (*fun_140134120)(model::DYNARY* t);
    // 140130230(t): DB length of a 1D table, n[0] * (ncomp + 1), rounded up to even (interim)
    std::int32_t (*fun_140130230)(const model::DYNARY* t);
    // 140130410(t, n, len): count a table by its ndim (interim)
    void (*fun_140130410)(const model::DYNARY* t, std::int32_t* n, std::int32_t* len);
    // 14025d3b0(window, n, len, nwin, lwin, owner): contact-window function counts
    void (*fun_14025d3b0)(void* window, std::int32_t* n, std::int32_t* len, std::int32_t* nwin,
                          std::int32_t* lwin, model::DB_STEP_DATA* owner);
    // 1401f3d90(sim ctrl): true for the single-object (STYPE/TRANS) mode that skips the mesh check
    bool (*fun_1401f3d90)(model::Cls_1401e7b10* s);

    // ---- Phase 2 / 3 owners (DBREAD, KEY): not on p1-gate ----------------------------------
    // 14012fff0 / 140130160 / 140130200 / 140130290: DYNARY lengths (interim)
    std::int32_t (*fun_14012fff0)(const model::DYNARY* t);
    std::int32_t (*fun_140130160)(const model::DYNARY* t);
    std::int32_t (*fun_140130200)(const model::DYNARY* t);
    std::int32_t (*fun_140130290)(const model::DYNARY* t);
    // 14017aaf0(list, number): object whose number (+8) is `number`, or 0 (interim)
    model::LLOBJ_DATA* (*fun_14017aaf0)(model::Cls_14017a6c0* l, std::int32_t number);
    // 14017f020(mesh +0x8a0): owner's state-variable table (owner + 0x15f8), or 0 (interim)
    std::int32_t* (*fun_14017f020)(void* mesh);
    // 140198310(o): number of 2D geometry curves; 1401982c0(o, &i): curve i (0-based)
    std::int32_t (*fun_140198310)(model::LLOBJ_DATA* o);
    model::OBJ_GEO2D* (*fun_1401982c0)(model::LLOBJ_DATA* o, const std::int32_t* i);
    // 140160d20(o): the object has 2D geometry curves (1401983a0(o) > 0)
    bool (*fun_140160d20)(model::LLOBJ_DATA* o);
    // 140304e40(o, 0): 2D geometry check
    bool (*fun_140304e40)(model::LLOBJ_DATA* o, std::int32_t flag);
    // 1401f3f80(sim ctrl): recompute SMODE (+0xf4)
    void (*fun_1401f3f80)(model::Cls_1401e7b10* s);
    // 1401d2090(m, w) / 1401d21c0(m, w): resize the element / node name arrays to width w
    void (*fun_1401d2090)(model::DB_STEP_DATA* m, std::int32_t w);
    void (*fun_1401d21c0)(model::DB_STEP_DATA* m, std::int32_t w);
    // 1401facf0(list, i, j): inter-object record of objects (i, j), or 0 (interim)
    model::Cls_1401f46e0* (*fun_1401facf0)(model::Cls_1401fa020* l, std::int32_t i, std::int32_t j);
    // 140269900(window list, n): n-th window (1-based), or 0 (interim)
    void* (*fun_140269900)(void* l, std::int32_t n);
    // 14028aff0(grain list, n): n-th record (1-based, link at +0x1e8), or 0 (interim)
    void* (*fun_14028aff0)(void* l, std::int32_t n);
    // 140256dc0(n): calloc(n, 4), 0 for n == 0 (interim)
    std::int32_t* (*fun_140256dc0)(std::int32_t n);
};

GendbExt& gendb_ext();
void reset_gendb_ext();

} // namespace def::dbio::gendb
