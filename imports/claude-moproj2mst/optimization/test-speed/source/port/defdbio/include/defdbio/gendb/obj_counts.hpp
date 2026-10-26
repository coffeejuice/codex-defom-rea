// defdbio/gendb/obj_counts.hpp: GENDB's per-object counts (packet p4-02a, root 1401beb30).
//
// 1401bc230 (p4-02b) calls
//     1401beb30(step, step+0x88 NFN, step+0x8c LLFNC length, step+0xc0 windows, step+0xc4 LLWIN
//               length, err)
// which walks the object list (step+0x1380) and adds, for every object that is not type 7, the
// number of property functions (DYNARY curves) and the words they take in the LLFNC region, and
// the number of window definitions (LLWINDEF_DATA) and the words they take in LLWIN. Along the
// way it normalises some of the model in place, as the original does: tables of the window
// definitions are sorted and de-duplicated (Fortran TPR_FNC_2DSORT / TPR_FNC_3DSORT), polygon
// windows are turned counter-clockwise, 2D objects drop degenerate geometry (140304e40) and
// mismatching body forces are deleted.
//
// The objects are reached through raw offsets (LLOBJ_DATA's sub-objects are byte slots in
// defmodel/layout/llobj_data.hpp); each read cites the offset of the binary.
//
// Callees that belong to other packets and are not on claude/p1-gate go through ObjCountExt
// (obj_count_ext()). Its defaults reproduce the callee where the callee is a field read, a list
// walk or a short formula (they carry no @src tag: the owner's port replaces them at merge);
// the others default to DEF_NOT_PORTED and tests install hooks.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary.hpp>

namespace def::model {
class Cls_1401e7b10;
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::dbio::gendb {

using model::DYNARY;

// ---- callees of other packets ------------------------------------------------------------
struct ObjCountExt {
    // 140130200  words of a 1D table: ncomp for type 0x7c, else 2 * n[0] (P2)
    std::int32_t (*dynary_words_1d)(const DYNARY* t);
    // 140130160  words of a 2D table: n0 + n1 + max(n0,1) * max(n1,1) * max(ncomp,1) (P2)
    std::int32_t (*dynary_words_2d)(const DYNARY* t);
    // 14012fff0  words of a 3D table: n0 + n1 + n2 + max(n0,1) * max(n1,1) * max(n2,1) (P2)
    std::int32_t (*dynary_words_3d)(const DYNARY* t);
    // 140130290  words by ndim (0: ncomp for the types 0xa7/0xa8/0xaa/0xab/0xad/0xb3/0xb5) (P2)
    std::int32_t (*dynary_words)(const DYNARY* t);
    // 14012eff0  DYNARY copy: type, shape, constant, then reallocate and copy the arrays (P2)
    void (*dynary_copy)(DYNARY* dst, const DYNARY* src);
    // 14012fd30  DYNARY init(type) (14012d2a0), n[0] = n0, reallocate (14012fad0) (P2)
    void (*dynary_init_n0)(DYNARY* t, std::int32_t type, std::int32_t n0);
    // 140194d40  the 3x3 matrix of Cls_14018b220 at +0x4c8..+0x510 into m (row major) (P2)
    void (*cls_14018b220_matrix)(const void* self, double* m);
    // 1402416d0  w = m * v (3x3 row major) (P2)
    void (*mat3_mul_vec)(const double* v, double* w, const double* m);
    // 140198310  number of 2D geometries of an object (list at LLOBJ_DATA+0x3998) (P2)
    std::int32_t (*obj_geo2d_count)(model::LLOBJ_DATA* obj);
    // 140304e40  2D: drop degenerate geometries of an object; returns 1 when all are valid (P2)
    bool (*obj_drop_bad_geo2d)(model::LLOBJ_DATA* obj, std::int32_t remove);
    // 140259b60  n-th (1-based) node of the 0xe8-record list (link +0xe0) (P2)
    void* (*record_e8_nth)(void* list, std::int32_t n);
    // 140269900  n-th (1-based) LLWINDEF_DATA of Cls_140269250 (link +0x840) (P2)
    void* (*window_nth)(void* list, std::int32_t n);
    // 140239a30  length of a 3-vector: sqrt(x*x + y*y + z*z) (1402399b0, CRT sqrt) (P2)
    double (*vec3_length)(const double* v);
    // 140257150  calloc(n, 8) with the "Error when allocating" message on failure (P2)
    double* (*alloc_doubles)(std::int32_t n);
    // 1402feeb0 / 1402ff000  element i (0-based) of the surface-set COW lists at +0x50 / +0x58 of
    //            the 3D geometry (u3c00[0]): detach (1402ff270 / 1402ff2e0), walk, node + 0x10 (P2)
    void* (*surface_set_a)(void* list50, std::int64_t i);
    void* (*surface_set_b)(void* list58, std::int64_t i);
    // 14019a820 / 14019a840  size of those lists: *(*(void**)list + 0x10) (P2)
    std::int32_t (*surface_list_a_size)(void* list50);
    std::int32_t (*surface_list_b_size)(void* list58);
    // 14019a000 / 14019a060  item count of a surface set: 14019a800 / 140159950 on set+0x10 (P2)
    std::int32_t (*surface_set_a_count)(void* set);
    std::int32_t (*surface_set_b_count)(void* set);
    // 1402b7470 / 1402b74d0  surface set kind (int at +4) (P2)
    std::int32_t (*surface_set_a_kind)(void* set);
    std::int32_t (*surface_set_b_kind)(void* set);
    // 1404b5ef4  Fortran WRITE of a debug line to `unit` (reached only with debug level 3)
    void (*ftn_debug_write)(std::int32_t unit, const char* text, std::int32_t len);
};

ObjCountExt& obj_count_ext();
void reset_obj_count_ext();  // restore the defaults (tests)

// ---- the packet ---------------------------------------------------------------------------
// Root.
// @src 1401beb30  objects of step (list at +0x1380): 14017ad20(step + 0x1380, ...)
void count_object_tables(model::DB_STEP_DATA* step, std::int32_t* nfn, std::int32_t* lfnc,
                         std::int32_t* nwin, std::int32_t* lwin, std::int32_t* err);
// @src 14017ad20  every object of the list whose type (+0xc) is not 7: 140167b50
void count_list_objects(void* list, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                        std::int32_t* lwin, std::int32_t* err);
// @src 140167b50  one object
void count_object(model::LLOBJ_DATA* obj, std::int32_t* nfn, std::int32_t* lfnc,
                  std::int32_t* nwin, std::int32_t* lwin);
// @src 140167ae0  "only deformation mode on" of the owner's simulation control
std::uint8_t only_deformation_mode_on(model::LLOBJ_DATA* obj);

// Simulation control (Cls_1401e7b10, DB_STEP_DATA+0x750).
std::int32_t simctrl_u10c(const void* sc);  // 1401f3c50
std::int32_t simctrl_u110(const void* sc);  // 1401f3c70
std::int32_t simctrl_u118(const void* sc);  // 1401f3cb0
bool simctrl_only_deformation(model::Cls_1401e7b10* sc);  // 1401f3d90

// Cls_14018b220 (LLOBJ_DATA+0x2160): 16 property tables.
void count_props_2160(void* self, std::int32_t* nfn, std::int32_t* lfnc, model::DB_STEP_DATA* owner,
                      model::LLOBJ_DATA* obj);  // 14018cec0
std::int32_t props_2160_table6_words(void* self, model::DB_STEP_DATA* owner);  // 14018cde0

// Cls_1401860a0 (LLOBJ_DATA+0x1ea0): two curves, body forces, a list and one more table.
void count_props_1ea0(void* self, std::int32_t* nfn, std::int32_t* lfnc);  // 1401869c0
std::int32_t count_body_force_records(void* self);                       // 140186d00

// Cls_140189440 (LLOBJ_DATA+0x2d10): windows.
std::int32_t windows_2d10_count(void* self);  // 140189580
void count_windows_2d10(void* self, std::int32_t* nwin, std::int32_t* lwin,
                        model::DB_STEP_DATA* owner);  // 140189f00

// Cls_1401956d0 (LLOBJ_DATA+0x2e68): a DYNARY list, two lists and seven window lists.
std::int32_t c2e68_tables_count(void* self);  // 140195ae0
std::int32_t c2e68_list18_count(void* self);  // 140195b00
std::int32_t c2e68_win48_count(void* self);   // 140195b30
std::int32_t c2e68_win78_count(void* self);   // 140195b60
std::int32_t c2e68_wina8_count(void* self);   // 140195b90
std::int32_t c2e68_wind8_count(void* self);   // 140195bc0
std::int32_t c2e68_win108_count(void* self);  // 140195bf0
std::int32_t c2e68_win138_count(void* self);  // 140195c20
std::int32_t c2e68_win168_count(void* self);  // 140195c50
void count_props_2e68(void* self, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                      std::int32_t* lwin, model::DB_STEP_DATA* owner);  // 140195da0
// The DYNARY list (Cls_1401364a0).
void count_dynary_list(void* list, std::int32_t* nfn, std::int32_t* lfnc);  // 1401365f0
std::int32_t dynary_list_max_id(void* list);                               // 140136a10
void count_dynary(const DYNARY* t, std::int32_t* nfn, std::int32_t* lfnc);  // 140130410
// The list at +0x18 (nodes linked at +0x38).
void count_list38(void* list, std::int32_t* nfn, std::int32_t* lfnc);  // 140257d10
void count_list38_node(void* node, std::int32_t* nfn, std::int32_t* lfnc);  // 140257450
// Window lists (Cls_140269250) and window definitions (LLWINDEF_DATA).
void count_window_list(void* list, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                       std::int32_t* lwin, model::DB_STEP_DATA* owner);  // 140269b40
void count_window(void* win, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                  std::int32_t* lwin, model::DB_STEP_DATA* owner);  // 14025d3b0
std::uint8_t orient_window(void* win, const std::uint8_t* reverse);  // 140265bf0

// The owner's int at +0x78 (DB_STEP_DATA+0x78 = u074[1]).
std::int32_t step_u078(const model::DB_STEP_DATA* step);  // 140199c10

// 3D geometry (LLOBJ_DATA::u3c00[0]) surface sets.
std::int32_t surface_list_a_size(void* geo);  // 14019a080
std::int32_t surface_list_b_size(void* geo);  // 14019a0a0
void count_surface_sets(void* geo, std::int32_t* nfn, std::int32_t* lfnc);  // 1402fe780
std::int32_t surface_a_ncomp(void* set);  // 1402f8760
std::int32_t surface_a_words(void* set);  // 1402f87a0
void count_surface_a(void* set, std::int32_t* nfn, std::int32_t* lfnc);  // 1402f87f0
std::int32_t surface_b_ncomp(void* set);  // 1402f8ce0
std::int32_t surface_b_words(void* set);  // 1402f8d30
void count_surface_b(void* set, std::int32_t* nfn, std::int32_t* lfnc);  // 1402f8d80

// DYNARY sizes and in-place sorting.
std::int32_t dynary_words_1d_padded(const DYNARY* t);  // 140130230
void dynary_sort(DYNARY* t);                            // 140134120
std::uint8_t dynary_rotate_velocity(DYNARY* t, const double* m);  // 140135bc0

// 3-vectors (24-byte values, returned through the first argument as the original does).
double* vec3_make(double* out, double x, double y, double z);    // 140239700
double vec3_dot_self(const double* v);                          // 1402399b0
double vec3_norm(const double* v);                              // 140239a30
double* vec3_normalized(double* out, const double* v);           // 140239b40
double* vec3_sub(double* out, const double* a, const double* b); // 140239fb0
double* vec3_add(double* out, const double* a, const double* b); // 14023a1b0
double* vec3_cross(double* out, const double* a, const double* b);  // 14023a4b0
double* polygon_normal(double* out, const double* pts, std::int32_t n);  // 140240d80

// Fortran table sorting (column-major arrays, 1-based in the comments, all arguments by
// reference as in Fortran).
// 14023829e: stable bubble sort of the ncol columns of a(lda, ncol) by rows 1..nkey in turn,
// then removal of each column equal to its predecessor in rows 1..nkey (ncol shrinks).
void sort_dedupe_columns(double* a, const std::int32_t* lda, const std::int32_t* nkey,
                         std::int32_t* ncol);
void tpr_fnc_2dsort(double* a, const std::int32_t* ndim, const std::int32_t* nkey, std::int32_t* n1,
                    std::int32_t* n2, const std::int32_t* unit, std::int32_t* ierr,
                    const std::int32_t* debug);  // 140236f5c
void actual_2dsort(double* a, double* w1, double* w2, std::int32_t* n1, std::int32_t* n2,
                   std::int32_t* l1, std::int32_t* l2, const std::int32_t* nkey,
                   const std::int32_t* unit, std::int32_t* ierr, const std::int32_t* debug);  // 1402375da
void tpr_fnc_3dsort(double* a, const std::int32_t* ndim, const std::int32_t* nkey, std::int32_t* n1,
                    std::int32_t* n2, std::int32_t* n3, const std::int32_t* unit, std::int32_t* ierr,
                    const std::int32_t* debug);  // 140235ec0
void actual_3dsort(double* a, double* w1, double* w2, const std::int32_t* nkey, std::int32_t* n1,
                   std::int32_t* n2, std::int32_t* n3, const std::int32_t* unit, std::int32_t* ierr,
                   const std::int32_t* debug);  // 140236539

} // namespace def::dbio::gendb
