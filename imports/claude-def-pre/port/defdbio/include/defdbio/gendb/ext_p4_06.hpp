// defdbio/gendb/ext_p4_06.hpp: calls from packet p4-06 (GENDB writers for 2D objects, simulation
// control, materials, inter-materials and inter-objects) into functions other packets own.
//
// Two kinds of wrapper:
//
//   * Local copies: small functions that only read or write model fields (getters, setters, list
//     walks, FUNCTION_DATA length and copy helpers, the FNCLOC function allocator). Their bodies are
//     ported here from the decompile so that the p4-06 writers run end to end in tests. Each names
//     its address and owner with "part of @src" (not counted as a p4-06 port). At the Phase 4 merge
//     the body is replaced by a call to the owner's function; call sites do not change.
//   * Hooked calls: larger functions of other packets (p4-05 object arrays, movement, windows,
//     P2 geometry access). Until the owner lands they go through g_hook (tests) or end in
//     DEF_NOT_PORTED.
//
// Pointers to Fortran-style arrays are passed as the binary passes them (int32 for MAR regions,
// double for A regions); "rec" is one record of a region.
#pragma once

#include <bit>
#include <cstdint>
#include <cstring>
#include <initializer_list>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defkey/gendb_deps.hpp>

namespace def::dbio::gendb::ext {

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::field;
using def::model::LLOBJ_DATA;
using def::model::Material;
using def::model::MaterialList;
using def::model::PhasePair;
using def::model::PhasePairList;

// ---- hook for calls into packets that have not landed ---------------------------------------

// Test hook: called for every hooked wrapper instead of DEF_NOT_PORTED. args are the wrapper's
// arguments in order (pointers as addresses, ints sign-extended, doubles as their bit pattern);
// the return value is the function's result (bit pattern for double, 0/1 for bool).
using Hook = std::uint64_t (*)(std::uint64_t addr, const std::uint64_t* args, int nargs);
inline Hook g_hook = nullptr;

// g_hook when set, otherwise DEF_NOT_PORTED(addr).
std::uint64_t call(std::uint64_t addr, std::initializer_list<std::uint64_t> args);

inline std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }
inline std::uint64_t u(std::int32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(v)); }
inline std::uint64_t u(double v) { return std::bit_cast<std::uint64_t>(v); }

// ---- empty functions (bodies are `ret`) -----------------------------------------------------

// 14001cd90 (p4-02b): message/error reporter, an empty function in this build.
inline void f14001cd90(const void*, const void*, const void*, const void*) {}
// 140174b40 (p4-05): debug consistency check after each object writer step, empty in this build.
inline void f140174b40(const void*, const void*, const void*, const void*, const void*, const char*) {}

// ---- DB_WRITE_VAR getters / setters (local copies) -------------------------------------------
// 140199c10 (p4-02a: forwarded to the owner): number of materials, +0x78
inline std::int32_t f140199c10(const DB_STEP_DATA* m) { return ::def::dbio::gendb::step_u078(m); }
// 140199ce0 (p4-05: forwarded to the owner): running node counter, +0x114
inline std::int32_t f140199ce0(const DB_STEP_DATA* m) { return g3d::node_offset(m); }
// 140199d20 (p4-05: forwarded to the owner): running element counter, +0x118
inline std::int32_t f140199d20(const DB_STEP_DATA* m) { return g3d::elem_offset(m); }
// 140199da0 (p4-05: forwarded to the owner): NUMNP, +0x6c
inline std::int32_t f140199da0(const DB_STEP_DATA* m) { return g3d::numnp(m); }
// 140199db0 (p4-05: forwarded to the owner): NUMEL, +0x70
inline std::int32_t f140199db0(const DB_STEP_DATA* m) { return g3d::numel(m); }
// 14019a0c0 (p4-05: forwarded to the owner): boundary node row cursor, +0x11c
inline std::int32_t f14019a0c0(const DB_STEP_DATA* m) { return g3d::bnd_node_offset(m); }
// 14019a0e0 (P1, ledger todo: forwarded to the owner): set +0x11c
inline void f14019a0e0(DB_STEP_DATA* m, const std::int32_t* v) { g3d::set_bnd_node_offset(m, v); }
// 14019a100 (p4-05: forwarded to the owner): LLGRY row cursor, +0x124
inline std::int32_t f14019a100(const DB_STEP_DATA* m) { return g3d::geo_point_offset(m); }
// 14019a120 (P1, ledger todo: forwarded to the owner): set +0x124
inline void f14019a120(DB_STEP_DATA* m, const std::int32_t* v) { g3d::set_geo_point_offset(m, v); }
// 14019a200 (p4-05: forwarded to the owner): +0x98
inline std::int32_t f14019a200(const DB_STEP_DATA* m) { return g3d::n_objects_meshed(m); }
// 14019a220 (p4-05: forwarded to the owner): LLBND record cursor, +0x110
inline std::int32_t f14019a220(const DB_STEP_DATA* m) { return g3d::record_counter(m); }
// 14019a2d0 (P1, ledger todo: forwarded to the owner): set +0x110
inline void f14019a2d0(DB_STEP_DATA* m, const std::int32_t* v) { g3d::set_record_counter(m, v); }
// 14019a350 (p4-05: forwarded to the owner): boundary count, +0x80
inline std::int32_t f14019a350(const DB_STEP_DATA* m) { return g3d::n_boundaries(m); }
// 14019a5a0 (p4-05: forwarded to the owner): next free LLWIN record, +0x134
inline std::int32_t f14019a5a0(const DB_STEP_DATA* m) { return g3d::window_row(m); }
// 14019a5c0 (p4-02b: forwarded to the owner): set +0x134
inline void f14019a5c0(DB_STEP_DATA* m, const std::int32_t* v) { ::def::dbio::gendb::set_u134(m, v); }
// 14019a5e0 (p4-05: forwarded to the owner): next free LLWRZ row, +0x138
inline std::int32_t f14019a5e0(const DB_STEP_DATA* m) { return g3d::window_pos(m); }
// 14019a600 (p4-02b: forwarded to the owner): set +0x138
inline void f14019a600(DB_STEP_DATA* m, const std::int32_t* v) { ::def::dbio::gendb::set_u138(m, v); }
// 1401f4610 (p4-02b: forwarded to the owner): number of objects, +0x7c
inline std::int32_t f1401f4610(const DB_STEP_DATA* m) { return ::def::dbio::gendb::object_count_field(const_cast<DB_STEP_DATA*>(m)); }

// ---- simulation control (DB_STEP_DATA+0x750) getters (local copies) --------------------------
// 1401f3c10 (kw-g1a: forwarded to the owner): +0x104 (TRANS 1)
inline std::int32_t f1401f3c10(const void* s) { return key::g1a::ops::trans0(*static_cast<const model::Cls_1401e7b10*>(s)); }
// 1401f3c30 (kw-g1a: forwarded to the owner): +0x108 (TRANS 2)
inline std::int32_t f1401f3c30(const void* s) { return key::g1a::ops::trans1(*static_cast<const model::Cls_1401e7b10*>(s)); }
// 1401f3c50 (P4, no packet: forwarded to the owner): +0x10c (TRANS 3)
inline std::int32_t f1401f3c50(const void* s) { return ::def::dbio::gendb::simctrl_u10c(s); }
// 1401f3c70 (P4, no packet: forwarded to the owner): +0x110 (TRANS 4)
inline std::int32_t f1401f3c70(const void* s) { return ::def::dbio::gendb::simctrl_u110(s); }
// 1401f3cb0 (P4, no packet: forwarded to the owner): +0x118 (HTMTHD)
inline std::int32_t f1401f3cb0(const void* s) { return ::def::dbio::gendb::simctrl_u118(s); }
// 1401f3d90 (p4-02a: forwarded to the owner): TRANS = (0, 1, 0, 0) and HTMTHD 0 (heat transfer only)
inline bool f1401f3d90(const void* s)
{
    return ::def::dbio::gendb::simctrl_only_deformation(static_cast<model::Cls_1401e7b10*>(const_cast<void*>(s)));
}

// ---- lists (local copies) --------------------------------------------------------------------
// 14019a280 (P3: forwarded to the owner): object list count (+0x10)
inline std::int32_t f14019a280(const model::Cls_14017a6c0* l) { return model::llobj_ctor::list_count(l); }
// 14017aaf0 (P2: forwarded to the owner): object with id n (int at +8), next link at +0x3940
LLOBJ_DATA* f14017aaf0(const model::Cls_14017a6c0* l, std::int32_t n);
// part of @src 1401d6ea0 (owner P2): object with id n of the model's list (+0x1380)
inline LLOBJ_DATA* f1401d6ea0(DB_STEP_DATA* m, std::int32_t n) { return f14017aaf0(&m->objects, n); }
// 1401facf0 (P2: forwarded to the owner): inter-object node with (+0x168, +0x16c) == (a, b), next +0x390
void* f1401facf0(const model::Cls_1401fa020* l, std::int32_t a, std::int32_t b);
// 1401f98d0 / 14019a590 (p4-02b / P2: forwarded to the owner): window count of the inter-object
// window list at +0x188 (its int at +0x14)
inline std::int32_t f1401f98d0(const void* io) { return ::def::dbio::gendb::inter_object_window_count(static_cast<model::Cls_1401f46e0*>(const_cast<void*>(io))); }
// 140269900 (P2: forwarded to the owner): window n (1-based) of a list (head at +0, next at +0x840)
void* f140269900(const void* list, std::int32_t n);
// 14028aff0 (P2: forwarded to the owner): node n (1-based) of a list (head at +0, next at +0x1e8)
void* f14028aff0(const void* list, std::int32_t n);
// part of @src 14029cd20 (owner P2): int at +0
inline std::int32_t f14029cd20(const void* p) { return field<std::int32_t>(p, 0); }
// part of @src 14029cd30 (owner P2): byte at +0x1e0
inline std::uint8_t f14029cd30(const void* p) { return field<std::uint8_t>(p, 0x1e0); }
// part of @src 1401b0620 (owner P2): material "used" flag (+4 != 0)
inline bool f1401b0620(const Material* mat) { return field<std::int32_t>(mat, 4) != 0; }
// 14029c410 (p4-02b: forwarded to the owner): largest material number in the list (0 if empty)
std::int32_t f14029c410(const MaterialList* l);
// 14029ba10 (p4-05: forwarded to the owner): table[number-1] = dense slot (1..) of each used material,
// 0 otherwise, for numbers 1..*n; returns the number of used materials
std::int32_t f14029ba10(const MaterialList* l, std::int32_t* table, const std::int32_t* n);
// 14033a040 (P3: forwarded to the owner): pair (from, to) of the inter-material list, next at +0x770
PhasePair* f14033a040(const PhasePairList* l, std::int32_t from, std::int32_t to);
// part of @src 140375e70 (owner P2): furnace record length (20)
inline std::int32_t f140375e70(const void*) { return 0x14; }

// ---- FUNCTION_DATA (DYNARY) DB lengths and copies (local copies) ------------------------------
// 14012fff0 -> gendb::dynary_words_3d (gate_ports.hpp): n1+n2+n3 + max(n1,1)*max(n2,1)*max(n3,1)
std::int32_t f14012fff0(const DYNARY* f);
// 140130090 (p4-02b: forwarded to the owner): as 14012fff0 with a factor max(ncomp,1)
std::int32_t f140130090(const DYNARY* f);
// 140130160 -> gendb::dynary_words_2d: n1+n2 + max(n1,1)*max(n2,1)*max(ncomp,1)
std::int32_t f140130160(const DYNARY* f);
// 140130200 -> gendb::dynary_words_1d: type 0x7c: ncomp, else 2*n1
std::int32_t f140130200(const DYNARY* f);
// 140130230 (p4-02a: forwarded to the owner): n1*(ncomp+1) rounded up to even
std::int32_t f140130230(const DYNARY* f);
// 140130290 -> gendb::dynary_words: plain-layout length by ndim
std::int32_t f140130290(const DYNARY* f);
// 140130380 (p4-02b: forwarded to the owner): self-describing length (data + 10)
std::int32_t f140130380(const DYNARY* f);
// 140131360 (p4-05: forwarded to the owner): plain layout writer (WriteFuncToDBArray)
void f140131360(const DYNARY* f, double* out);
// 1401317c0 (p4-05: forwarded to the owner): 1-D interleaved writer (x, y[ncomp]) per point
void f1401317c0(const DYNARY* f, double* out);
// 14028a650 (p4-02b: forwarded to the owner): mesoscale grain record length
std::int32_t f14028a650(const void* rec);
// 14028b990 (p4-02b: forwarded to the owner): GRNDAT mode 1/4 function length; g = Material+0x1078
std::int32_t f14028b990(const void* g);
// 14028e140 (P2: forwarded to the owner): FRCMOD DB code and its 5 reals; false for an unknown type
bool f14028e140(const Material* mat, std::int32_t* code, double* p);
// 140336300 (p4-02b: forwarded to the owner): TTTD function length by model
std::int32_t f140336300(const PhasePair* pp);

// 1401bb3d0 (p4-05: forwarded to the owner): FNCLOC. Allocates the next function number (m+0x10c),
// stores it in *fid as a double, writes IIFNC[2k-1] = *attr, IIFNC[2k] = IIFNC[2k-2] + *len and
// returns the 1-based LLFNC start in *start. Prints the two FNCLOC warnings to stdout.
void f1401bb3d0(DB_STEP_DATA* m, const std::int32_t* len, std::int32_t* iifnc, const std::int32_t* attr,
                double* fid, std::int32_t* start);

// ---- hooked calls -----------------------------------------------------------------------------

// P2 geometry access of an object (_OBJ_GEO2D of the object's geometry collection)
// 1401982c0: curve *idx of the object's 2D geometry, or null
inline void* f1401982c0(LLOBJ_DATA* obj, const std::int32_t* idx)
{
    return reinterpret_cast<void*>(call(0x1401982c0, {u(obj), u(idx)}));
}
// 140198310: number of curves of the object's 2D geometry
inline std::int32_t f140198310(LLOBJ_DATA* obj) { return static_cast<std::int32_t>(call(0x140198310, {u(obj)})); }
// 140160cc0 (p4-02b): object has geometry (2D: curve count > 0)
inline bool f140160cc0(LLOBJ_DATA* obj) { return (call(0x140160cc0, {u(obj)}) & 0xff) != 0; }
// 140160d20 (P2): 2D geometry curve count > 0
inline bool f140160d20(LLOBJ_DATA* obj) { return (call(0x140160d20, {u(obj)}) & 0xff) != 0; }
// 1401808d0 (p4-03): node coordinate array of the mesh sub-object (obj+0x8a0)
inline double* f1401808d0(void* nodes) { return reinterpret_cast<double*>(call(0x1401808d0, {u(nodes)})); }
// 140183cb0 (p4-05): connectivity array of the element sub-object (obj+0x9e0)
inline std::int32_t* f140183cb0(void* elems) { return reinterpret_cast<std::int32_t*>(call(0x140183cb0, {u(elems)})); }
// 140165230 (p4-05): size of the per-object scratch array (boundary items + 1000)
inline std::int32_t f140165230(LLOBJ_DATA* obj) { return static_cast<std::int32_t>(call(0x140165230, {u(obj)})); }
// 140166e60 (p4-05): nearest boundary point of a meshed object
inline bool f140166e60(LLOBJ_DATA* obj, double x, double y, std::int32_t* a, std::int32_t* b, double* px,
                       double* py)
{
    return (call(0x140166e60, {u(obj), u(x), u(y), u(a), u(b), u(px), u(py)}) & 0xff) != 0;
}
// 140203980 (p4-05): rigid object written with boundary record type 2
inline bool f140203980(LLOBJ_DATA* obj) { return (call(0x140203980, {u(obj)}) & 0xff) != 0; }

// p4-05 object writers (called by the 2D object writer 14017cb60 in this order)
inline void f14016f8e0(LLOBJ_DATA* o, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at, std::int32_t* n)
{
    call(0x14016f8e0, {u(o), u(mar), u(a), u(mt), u(at), u(n)});
}
inline void f140174cd0(LLOBJ_DATA* o, std::int32_t* mar, std::int32_t* mt, std::int32_t* p4, std::int32_t* p5,
                       std::int32_t* p6)
{
    call(0x140174cd0, {u(o), u(mar), u(mt), u(p4), u(p5), u(p6)});
}
inline void f1401774f0(LLOBJ_DATA* o, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                       std::int32_t* p6, std::int32_t* p7, std::int32_t* buf, std::int32_t* p9)
{
    call(0x1401774f0, {u(o), u(mar), u(a), u(mt), u(at), u(p6), u(p7), u(buf), u(p9)});
}
inline void f140172c90(LLOBJ_DATA* o, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                       std::int32_t* p6, std::int32_t* p7)
{
    call(0x140172c90, {u(o), u(mar), u(a), u(mt), u(at), u(p6), u(p7)});
}
inline void f1401707e0(LLOBJ_DATA* o, std::int32_t* mar, std::int32_t* mt, std::int32_t* p4, std::int32_t* buf,
                       void* p6, std::int32_t* p7, std::int32_t* p8, std::int32_t* p9, std::int32_t* p10)
{
    call(0x1401707e0, {u(o), u(mar), u(mt), u(p4), u(buf), u(p6), u(p7), u(p8), u(p9), u(p10)});
}
inline void f140178460(LLOBJ_DATA* o, std::int32_t* mar, std::int32_t* mt, std::int32_t* p4, std::int32_t* p5,
                       std::int32_t* p6, std::int32_t* iifnc, double* llfnc, std::int32_t* buf, void* p10,
                       std::int32_t* p11, std::int32_t* p12, std::int32_t* p13)
{
    call(0x140178460, {u(o), u(mar), u(mt), u(p4), u(p5), u(p6), u(iifnc), u(llfnc), u(buf), u(p10), u(p11),
                       u(p12), u(p13)});
}
// 14016cf10 WriteGeneralInfoToDB(obj, IIOBJ rec, LLOBJ rec, mode, bnd_cursor, nbnd, IIFNC, LLFNC,
// material slot table, &meshed)
inline void f14016cf10(LLOBJ_DATA* o, std::int32_t* irec, double* lrec, std::int32_t mode, std::int32_t cursor,
                       std::int32_t nbnd, std::int32_t* iifnc, double* llfnc, std::int32_t* slots, char* meshed)
{
    call(0x14016cf10, {u(o), u(irec), u(lrec), u(mode), u(cursor), u(nbnd), u(iifnc), u(llfnc), u(slots), u(meshed)});
}
// 14016d200 LLOBJ[48..49] (same arguments as 14016cf10)
inline void f14016d200(LLOBJ_DATA* o, std::int32_t* irec, double* lrec, std::int32_t mode, std::int32_t cursor,
                       std::int32_t nbnd, std::int32_t* iifnc, double* llfnc, std::int32_t* slots, char* meshed)
{
    call(0x14016d200, {u(o), u(irec), u(lrec), u(mode), u(cursor), u(nbnd), u(iifnc), u(llfnc), u(slots), u(meshed)});
}
// 14018a760 remesh sub-object (obj+0x2d10) writer
inline void f14018a760(void* sub, std::int32_t* irec, double* lrec, std::int32_t* iifnc, double* llfnc,
                       double* llwin, std::int32_t wwin, double* llwrz, char* flag, DB_STEP_DATA* m,
                       std::int32_t* objid)
{
    call(0x14018a760, {u(sub), u(irec), u(lrec), u(iifnc), u(llfnc), u(llwin), u(wwin), u(llwrz), u(flag), u(m),
                       u(objid)});
}
// 140196840 window sub-object (obj+0x2e68) writer
inline void f140196840(void* sub, std::int32_t* irec, double* lrec, std::int32_t* iifnc, double* llfnc,
                       double* llwin, std::int32_t wwin, double* llwrz, char* flag_a, char* flag_locdef,
                       char* flag_winprs, DB_STEP_DATA* m, std::int32_t* objid)
{
    call(0x140196840, {u(sub), u(irec), u(lrec), u(iifnc), u(llfnc), u(llwin), u(wwin), u(llwrz), u(flag_a),
                       u(flag_locdef), u(flag_winprs), u(m), u(objid)});
}
// 140187a60 process sub-object (obj+0x1ea0) writer
inline void f140187a60(void* sub, std::int32_t* irec, double* lrec, std::int32_t* iifnc, double* llfnc,
                       DB_STEP_DATA* m, std::int32_t* objid)
{
    call(0x140187a60, {u(sub), u(irec), u(lrec), u(iifnc), u(llfnc), u(m), u(objid)});
}
// 14017e190 object-list tail writer (after all objects)
inline void f14017e190(model::Cls_14017a6c0* l, double* a, std::int32_t* mar, double* llbnd, std::int32_t* wbnd)
{
    call(0x14017e190, {u(l), u(a), u(mar), u(llbnd), u(wbnd)});
}
// 1401912a0 OMV_DATA::WriteToDB (movement, obj+0x2160) into one LLBND record
inline void f1401912a0(void* mv, std::int32_t* type, double* rec, std::int32_t* iifnc, double* llfnc,
                       DB_STEP_DATA* m, std::int32_t* cursor)
{
    call(0x1401912a0, {u(mv), u(type), u(rec), u(iifnc), u(llfnc), u(m), u(cursor)});
}
// 140260ea0 LLWINDEF_DATA::WriteToDBArrays into one LLWIN record
inline void f140260ea0(void* win, double* rec, double* llwrz, std::int32_t* iifnc, double* llfnc,
                       std::int32_t* win_cursor, std::int32_t* wrz_cursor, DB_STEP_DATA* m, std::int32_t* p9,
                       std::int32_t objid)
{
    call(0x140260ea0, {u(win), u(rec), u(llwrz), u(iifnc), u(llfnc), u(win_cursor), u(wrz_cursor), u(m), u(p9),
                       u(objid)});
}

} // namespace def::dbio::gendb::ext
