// Model-level helpers of GENBCC (packet p67-5): the default boundary conditions of a 2D object
// (FUN_1402066e0 and its callees). Free functions over LLOBJ_DATA at the binary's offsets.
// Spec: docs/re/commands/GENBCC.md. Internal header of src/p67_5.
//
// Object fields (obj = LLOBJ_DATA, model m = obj+0x3a98, widths from m+0x15f8):
//   +0x87c has mesh, +0x880 nodes, +0x884 elements, +0x88c boundary edges NBE (sum over the
//   loops of node count - 1, 140165d30), +0x8a0 node set (Cls_14017ef20: +0x8a8 coordinates,
//   +0x8b8 URZ, +0x8c0 FRZ, +0x990 BCCDEF, +0x998 BCCDFN, +0x9a0 BCCTMP, +0x9a8 BCCTFN),
//   +0x9e8 connectivity (4 ints per element in 2D), +0xae8 ECCDEF, +0xaf0 ECDEFN, +0xb00 ECCTMP,
//   +0xb08 ECTMFN, +0xbd8 / +0xbe0 boundary loops, +0x2f40 LOCTMP window list, +0x39c0 object
//   dimension.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::genbcc {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// 1401ac6c0: look object *no up again and regenerate its default BCs; 0 when missing.
bool regenerate_object(DB_STEP_DATA& m, const std::int32_t* no);
// 1402066e0: 3D object (140158ca0) -> 140206730 (returns 0), else the 2D body 1402069a0.
bool regenerate_default_bc(LLOBJ_DATA& obj);
// 1402069a0: the 2D body (always true).
bool regenerate_default_bc_2d(LLOBJ_DATA& obj);

// 140204d20: clear the thermal BCs (BCCTMP/BCCTFN per node, ECCTMP/ECTMFN per edge slot < NBE)
// and empty the LOCTMP window list.
void reset_thermal_bc(LLOBJ_DATA& obj);
// 140204180: clear the deformation BCs (BCCDEF/BCCDFN/URZ per node, negative codes kept in 2D;
// ECCDEF/ECDEFN per edge slot < NBE). Widths of BCCDEF, BCCDFN, URZ, FRZ.
void reset_deformation_bc(LLOBJ_DATA& obj, const std::int32_t* w_def, const std::int32_t* w_dfn,
                          const std::int32_t* w_urz, const std::int32_t* w_frz);

// 140212420: GEOTYP of the object's model (m+0x850, 1401d6d90); 0 without a model.
std::int32_t object_geotyp(const LLOBJ_DATA& obj);
// 140206f60: every edge number 1..NBE appended to list.
void all_edges(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* list);
// 140206d50: nodes on the symmetry axis (|r| < 0.05 * shortest element edge) into axis_nodes,
// edges not on the axis into edges. Returns true.
bool axis_search(LLOBJ_DATA& obj, std::int32_t* n_axis, std::int32_t* axis_nodes, std::int32_t* n_edges,
                 std::int32_t* edges);

// 140202a20: shortest element edge (2D: 1403046a0; 3D: 140202a70, not ported).
double min_edge_length(LLOBJ_DATA& obj);
// 1403046a0: shortest element edge of a 2D mesh (0 without mesh / elements / arrays).
double min_edge_length_2d(LLOBJ_DATA& obj);
// 1403087e0: length of a 2D vector (sqrt of 1403087a0, the dot product with itself).
double vec_length_2d(const double* v);

// 1403054e0 / 140305520: boundary slot of edge number *edge (1-based); -1 when none.
std::int32_t edge_number_slot(const LLOBJ_DATA& obj, const std::int32_t* edge);
std::int32_t edge_to_slot(const std::int32_t* edge, const std::int32_t* nloops, const void* loops);

// 140163490: coordinates of node n (1-based), null without mesh / model or out of 1..nodes.
double* node_coords(LLOBJ_DATA& obj, std::int32_t n);
// 140206fd0: set the node code c / function f / value v at dof of every node of `nodes`
// (mode 0: 140207250; modes 1, 2 call 140207f50, not ported).
void set_node_codes(LLOBJ_DATA& obj, const std::int32_t* nodes, std::int32_t n, std::int32_t* code,
                    std::int32_t* fn, double* val, std::int32_t dof, std::int32_t c, std::int32_t f,
                    double v, const std::int32_t* w_code, const std::int32_t* w_fn, std::int32_t mode,
                    std::int32_t a14, std::int32_t a15, double a16);
// 140207250: mode 0 of the above.
void set_node_codes_direct(LLOBJ_DATA& obj, const std::int32_t* nodes, std::int32_t n, std::int32_t* code,
                           std::int32_t* fn, double* val, std::int32_t dof, std::int32_t c, std::int32_t f,
                           double v, const std::int32_t* w_code, const std::int32_t* w_fn);

// 1402084c0: set the edge code c (function f, value v) of BC kind `kind` on every edge of
// `edges` (mode 0: 1402086c0; modes 1, 2 call 140208920, not ported).
void set_edge_codes(LLOBJ_DATA& obj, std::int32_t kind, const std::int32_t* edges, std::int32_t n,
                    std::int32_t* code, std::int32_t* fn, double* val, std::int32_t c, std::int32_t f,
                    double v, std::int32_t mode, std::int32_t a12, double a13);
// 1402086c0: mode 0 of the above.
void set_edge_codes_direct(LLOBJ_DATA& obj, std::int32_t kind, const std::int32_t* edges, std::int32_t n,
                           std::int32_t* code, std::int32_t* fn, double* val, std::int32_t c, std::int32_t f,
                           double v);
// 140305d70 / 140305e90: store *v into arr[slot of edge] for each edge number of `edges`; range
// messages on stderr. -1 when edges (d70) / arr (e90) is null, else 0.
std::int32_t store_edge_ints(LLOBJ_DATA& obj, std::int32_t* arr, const std::int32_t* edges,
                             const std::int32_t* n, const std::int32_t* v);
std::int32_t store_edge_doubles(LLOBJ_DATA& obj, double* arr, const std::int32_t* edges,
                                const std::int32_t* n, const double* v);

} // namespace def::cmd::genbcc
