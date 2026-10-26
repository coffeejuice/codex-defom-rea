// defdbio/gendb/checks_p4_04.hpp: GENDB input checks, packet p4-04 (simulation control and
// inter-object data).
//
// FUN_1401bb550 (GENDB, p4-01) runs the check chain 1401bebf0 when its `check` argument is 0 or 2
// (docs/re/dbmap/stepdriver.md section 5). The chain runs checks 1..5 in order:
//   1 simulation control  1401becf0 (this packet)
//   2 material properties 1401bfdf0 (p4-03)
//   3 inter-material data 1401bfe60 (p4-03)
//   4 object data         1401bfed0 (p4-03)
//   5 inter-object data   1401bff40 (this packet)
// then the summary 14001cdc0 (an empty function in this build). Every check prints its messages
// to stdout (__iob_func() + 0x30); `err` is set to 1 by an "ERROR" message and `warn` by most
// "WARNING" messages (some warnings set nothing, as in the original). Some checks also repair
// the model: PDIE(0) := 1, DTMAX := 0 when DSMAX is set too, STNOUT(5) := 1.
//
// The functions take the objects the binary passes as `this` first. Calls into functions other
// packets own go through src/gendb/p4_04/ext_p4_04.hpp.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::dbio::gendb {

using def::model::Cls_1401e7b10;
using def::model::Cls_1401f46e0;
using def::model::Cls_1401fa020;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;

// ---- check chain ---------------------------------------------------------------------------

// @src 1401bebf0  checks 1..5 and the summary; clears *err and *warn first
void check_chain(DB_STEP_DATA& s, std::int32_t* err, std::int32_t* warn);
// @src 1401becf0  check 1: "INFO   : Checking simulation controls", then 1401ee940(s+0x750)
void check_sim_control(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn);
// @src 1401bff40  check 5: "INFO   : Checking inter-object data", then 1401fa310(s+0x13c8)
void check_inter_object(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn);

// ---- simulation control ---------------------------------------------------------------------

// @src 1401ee940  simulation control checks (STYPE .. STNOUT), then 1401ef470
void sim_ctrl_check(Cls_1401e7b10& sim, std::int32_t* err, std::int32_t* warn);
// @src 1401ef470  iteration / solver method checks against the objects
void sim_ctrl_check_solver(Cls_1401e7b10& sim, std::int32_t* err, std::int32_t* warn);
// @src 1401ee5d0  false when no step size is defined (DTTYPE constant and DSMAX, DTMAX(1) <= 0,
//                 or a DTTYPE table that is all zero)
bool sim_ctrl_step_size_defined(const Cls_1401e7b10& sim);
// @src 1401d9010  simulation type name for STYPE (table 1405d4680)
const char* sim_type_name(const Cls_1401e7b10& sim);

// @data 1406e84d4  14 method codes compared with ITRMTH / SOLMTD / SOLMTT (set per dimension)
struct MethodCodes {
    std::int32_t itr_unset;    // 1406e84d4
    std::int32_t itr_direct;   // 1406e84d8
    std::int32_t itr_newton;   // 1406e84dc
    std::int32_t sol_unset;    // 1406e84e0
    std::int32_t sol_cg;       // 1406e84e4
    std::int32_t c14;          // 1406e84e8 (3 for 2D, 1 for 3D)
    std::int32_t c18;          // 1406e84ec
    std::int32_t c1c;          // 1406e84f0
    std::int32_t sol_explicit; // 1406e84f4
    std::int32_t solt_unset;   // 1406e84f8
    std::int32_t sol_sparse;   // 1406e84fc (3 for 2D, 1 for 3D; the TET/Brick advice)
    std::int32_t c2c;          // 1406e8500
    std::int32_t c30;          // 1406e8504
    std::int32_t c34;          // 1406e8508
};
inline MethodCodes g_method_codes = {};

// @src 1401c7d70  method codes for 2D
void method_codes_2d();
// @src 1401c7e10  method codes for 3D
void method_codes_3d();
// @src 1401c7eb0  *dim == 2 -> 2D codes, else 3D
void method_codes_select(const std::int32_t* dim);

// ---- object queries used by the simulation control checks (s = the step model) --------------

// @src 1401bed60  number of objects of type 4 (elasto-plastic)
std::int32_t count_elasto_plastic(DB_STEP_DATA& s);
// @src 1401bee00  an object has movement type 5 (sliding die)
bool any_movement_type_5(DB_STEP_DATA& s);
// @src 1401bee90  an object has movement type 0 with 1401792d0 true (force control)
bool any_force_movement(DB_STEP_DATA& s);
// @src 1401bef30  an object has torque control (1401797e0 with rotation type 0)
bool any_torque_movement(DB_STEP_DATA& s);
// @src 1401bf230  *any_relation: an inter-object record has +0x170 set; *rigid_moving: a rigid
//                 object with 140179280 true (only looked at when the inter-object list is not empty)
void inter_obj_and_rigid_movement(DB_STEP_DATA& s, std::uint8_t* any_relation, std::uint8_t* rigid_moving);
// @src 1401bf380  the primary die (PDIE 1) has 1401792d0 true
bool primary_die_moves(DB_STEP_DATA& s);
// @src 1401bf460  the primary die follows an object (+0x22c0) that moves, with a ratio (+0x22c8) != 0
bool primary_die_follows(DB_STEP_DATA& s, std::int32_t* followed);
// @src 1401bf730  number of objects of the given type
std::int32_t count_objects_of_type(DB_STEP_DATA& s, const std::int32_t* type);
// @src 1401bf8c0  a meshed object with +0x890 == 4 (flag 0) or < 4 (flag != 0)
bool any_mesh_kind(DB_STEP_DATA& s, bool below_4);
// @src 1401bf990  an elasto-plastic meshed object with +0x2154 == *kind
bool any_elasto_plastic_mesh_type(DB_STEP_DATA& s, const std::int32_t* kind);
// @src 1401bfbc0  3D only: a meshed object has a BCC code 7 or 8
bool any_rotational_symmetry_bcc(DB_STEP_DATA& s);
// @src 1401c5500  an object of type 4 (no type-7 test)
bool any_type_4(DB_STEP_DATA& s);
// @src 1401c5590  an object with 14017a130 true
bool any_mechanical_press(DB_STEP_DATA& s);
// @src 14017a130  movement type 4 with 1401792d0 true
bool movement_type_4_defined(LLOBJ_DATA* obj);
// @src 1401fb840  distinct values of the object's element material array (+0x9f8, +0x884 entries):
//                 *n = number of distinct values, the first min(*n, *cap) go to out; false when the
//                 object has no mesh or no array
bool object_material_ids(LLOBJ_DATA* obj, std::int32_t* n, std::int32_t* out, const std::int32_t* cap,
                         DB_STEP_DATA* owner);
// @src 140202a00  int at +0x3980
std::int32_t object_u3980(const LLOBJ_DATA* obj);

// ---- inter-object data ----------------------------------------------------------------------

// @src 1401fa310  every relation between two (non type 7) objects; warns when there are
//                 several objects and no relation
void inter_obj_check(Cls_1401fa020& list, std::int32_t* err, std::int32_t* warn);
// @src 1401f4ce0  one relation: FRCFAC, IHTCOF, INTRST, FRCWIN; *count += 1 when +0x170 != 0
void inter_obj_check_record(Cls_1401f46e0& rec, std::int32_t* count, std::int32_t* err, std::int32_t* warn);
// @src 1401f4fa0  "WARNING: %s between Objects ... is ZERO" for an undefined or all-zero curve
void inter_obj_check_curve(Cls_1401f46e0& rec, const char* name, bool active, const DYNARY* curve,
                           std::int32_t code, std::int32_t* warn);
// @src 1401f5060  "Error: %s between Objects ... is not valid" when 14025d220 is false
void inter_obj_check_window(Cls_1401f46e0& rec, const char* name, const void* window, std::int32_t code,
                            std::int32_t* err);
// @src 1401f5100  friction (FRCFAC) checks for one or both friction definitions
void inter_obj_check_friction(Cls_1401f46e0& rec, bool active, std::int32_t* warn);
// @src 1401f3cd0  HTMTHD == 1 (1401f3cb0)
bool sim_htmthd_is_1(const Cls_1401e7b10& sim);
// @src 14025d220  a contact window (LLWINDEF_DATA) is valid
bool window_valid(const void* window);
// @src 14001cda0  message hook (an empty function in this build)
void message_hook_14001cda0();

// ---- DYNARY tests (not in the ledger; needed by the functions above and by p4-03's 1401792d0) --

// @src 14012fe90  all values zero (ndim 0: the constant)
bool dynary_values_zero(const DYNARY* t);
// @src 14012fd80  all abscissae and all values zero (ndim 0: the constant)
bool dynary_all_zero(const DYNARY* t);

void reset_globals_p4_04();

} // namespace def::dbio::gendb
