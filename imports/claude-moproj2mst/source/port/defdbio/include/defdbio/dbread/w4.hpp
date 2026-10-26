// defdbio/dbread/w4.hpp: DBREAD (FUN_1401cc420) helper W4 of packet p-dbread: the object arrays
// (ReadArrays 1401d2be0 and its per-object readers), the 3D fold meshes (1401d25d0), the dynamic
// arrays (1401d2760), the inter-material records (1401d0180 -> 140336740) and the inter-object
// records (1401d5c10 -> 1401f58a0). Field maps: docs/re/dbmap/objarrays.md, object.md,
// material.md, interobj.md (branch claude/project-thread-bz5k5e); the decompile is the reference.
//
// Arguments are the binary's: MAR is the int array DB_STEP_DATA+0x1b88, A the double array +0x1b78,
// mar_tab / a_tab the address tables +0x1b90 / +0x1dc0 (starts, then widths), w the state-variable
// width table +0x15f8 (W[id]). Region slots are looked up through def::model::g_mar_index /
// g_a_index exactly where the binary reads DAT_1406e81fc.. / DAT_1406e82d0...
#pragma once

#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class OBJ_GEO3D;
class Material;
class PhasePair;
class Cls_1401f46e0;
class DYNARY_DATA;
class DynAryLinkedList;
namespace linked {
class Cls_140257340;
}
} // namespace def::model

namespace def::dbio::dbread {

// ---- entries called by DBREAD 1401cc420 -------------------------------------------------------

// 1401d0180  ReadInterMatlInfo: for every ordered pair (i, j) of existing materials a new phase
//            pair (from = material i, to = material j) read from LLTFM record j * NMAT + i
void read_inter_matl_info(model::DB_STEP_DATA* m);
// 1401d25d0  ReadObjFoldInfoFromDB: 3D fold meshes of every object when m+0x1734 != 0
void read_obj_fold_info(model::DB_STEP_DATA* m);
// 1401d2760  ReadDynamicArrayFromDB: every real / int dynamic array of the step into the list
//            obj+0xba8 of the object named in the array's row
void read_dynamic_arrays(model::DB_STEP_DATA* m);
// 1401d2be0  ReadArrays: per-object node / element / edge-face arrays and BC functions, then the
//            boundary, material and NBCD renumbering
void read_arrays(model::DB_STEP_DATA* m);
// 1401d5c10  ReadInterObjInfo: one inter-object record per ordered object pair (i == j included)
void read_inter_obj_info(model::DB_STEP_DATA* m);

// ---- per-object readers -----------------------------------------------------------------------

// 14016d300  fold meshes FLDGEO (obj+0x3328) / FLWNET (obj+0x3458) from LLFLD / IIFLD rows given by
//            IIOBJ words 15..18
void read_fold_info(model::LLOBJ_DATA* obj, const std::int32_t* iiobj, std::int32_t* mar, double* a,
                    const std::int32_t* mar_tab, const std::int32_t* a_tab);
// 1402fa100  split src into geo (nodes before *first_node, faces before *first_face) and net (the
//            rest, renumbered; with *negate the 4th node is abs(.) - nodes of geo)
void split_fold_mesh(model::OBJ_GEO3D* src, model::OBJ_GEO3D* geo, model::OBJ_GEO3D* net,
                     const std::int32_t* first_node, const std::int32_t* first_face, const char* negate);
// 140170410  node BC arrays (NBCD 0x990, BC refs 0x998, 0x9a0, 0x9a8, 0x9b0, 0x9b8) from MAR
void read_node_bc_info(model::LLOBJ_DATA* obj, const std::int32_t* mar, const std::int32_t* mar_tab,
                       const std::int32_t* w, std::int32_t numnp);
// 140174130  ReadElemIntegerInfoFromDB: connectivity (0x9e8) and material groups (0x9f8)
void read_elem_integer_info(model::LLOBJ_DATA* obj, const std::int32_t* mar, const std::int32_t* mar_tab,
                            const std::int32_t* w, const std::int32_t* iiobj, const std::int32_t* which);
// 140176df0  ReadEdgeFaceInfoFromDB: edge / face BC arrays 0xae8..0xb40 (version gates 4000 / 7000
//            / 11000)
void read_edge_face_info(model::LLOBJ_DATA* obj, const std::int32_t* mar, const double* a,
                         const std::int32_t* mar_tab, const std::int32_t* a_tab, std::int32_t version);
// 140177ba0  ReadBdryCndFuncFromDB (3D, and 2D from 7000): BC function numbers -> IIFNC owner words,
//            BCCFNC table obj+0x2e68 and SYMSUF points obj+0x2e80 rebuilt
void read_bc_functions(model::LLOBJ_DATA* obj, const std::int32_t* iifnc, double* llfnc, std::int32_t nfn,
                       const std::int32_t* w);
// 140178250  ReadNodeBdryCndFuncFromDB: node BC function numbers -> IIFNC owner words only
void read_node_bc_functions(model::LLOBJ_DATA* obj, const std::int32_t* iifnc, double* llfnc,
                            std::int32_t nfn, const std::int32_t* w);
// 140257490  SYMSUF point node: point = p[0..2], direction = p[3..5]
void axis_point_set(model::linked::Cls_140257340* node, const double* p);

// ---- records ----------------------------------------------------------------------------------

// 140336740  one LLTFM record into a phase pair
void read_phase_pair_record(model::PhasePair* pp, const double* rec, std::int32_t* iifnc, double* llfnc);
// 1401f58a0  one LLBRT record into an inter-object record (2D and 3D layouts)
void read_inter_obj_record(model::Cls_1401f46e0* e, const double* rec, std::int32_t* iifnc, double* llfnc,
                           double* llwin, std::int32_t w_llwin, double* llwrz);

// ---- dynamic arrays ---------------------------------------------------------------------------

// 1403bc272  NI, max int size, NR, max real size of the step's dynamic-array descriptor
void dynamic_array_counts(std::int32_t* ni, std::int32_t* ni_max, std::int32_t* nr, std::int32_t* nr_max);
// 14025bdc0  new DYNARY_DATA in list (kind, subtype, n / ncomp rows, ncomp); a real array gets the
//            values of v
void dynary_list_add_real(model::DynAryLinkedList* list, std::int32_t kind, std::int32_t subtype,
                          std::int32_t n, std::int32_t ncomp, model::CowVector<double>* v);
// 14025bd10  the same for an int array
void dynary_list_add_int(model::DynAryLinkedList* list, std::int32_t kind, std::int32_t subtype,
                         std::int32_t n, std::int32_t ncomp, model::CowVector<std::int32_t>* v);
// 14025b5f0  d's double array (its size) = v[0..]
void dynary_data_copy_doubles(model::DYNARY_DATA* d, model::CowVector<double>* v);
// 14025b680  d's int array (its size) = v[0..]
void dynary_data_copy_ints(model::DYNARY_DATA* d, model::CowVector<std::int32_t>* v);
// 1401c57c0  detach, then &v[0] (4-byte elements)
std::int32_t* cow_ints_begin(model::CowVector<std::int32_t>* v);

// ---- DB_STEP_DATA helpers ---------------------------------------------------------------------

// 1401cec20  grow the A buffer (+0x1b78, +0x1b74 doubles) to *n (contents dropped)
void step_reserve_a(model::DB_STEP_DATA* m, const std::int32_t* n);
// 1401d7090  material n (1-based) of the material list
model::Material* step_material_nth(model::DB_STEP_DATA* m, std::int32_t n);

} // namespace def::dbio::dbread
