// defmodel/members/ext_deps.hpp: functions of other packets that p1-10 code calls, declared here
// until their owners land (docs/port-conventions.md section 9, step 3).
//
// Each one is named after its address and defined in src/members/ext_deps_stubs.cpp as
// DEF_NOT_PORTED. When the owner's function is merged, the merge deletes its stub and points
// the call sites (grep "ext::fun_<address>") at the owner's name.
//
// Owners (docs/re/phase1_packet_map.tsv):
//   p1-04  Cls_1401364a0 list: 140136670 add, 140136860 at, 14013c1b0 size, 140136910 clear;
//          DYNARY values: 140130720 get, 140130800 set
//   none   DYNARY core 14012d2a0 init, 14012fad0 alloc, 14012e400 / 14012e6d0 unit
//          conversion: not in the ledger (scope gap, reported to the coordinator)
//   p1-12  units 140235c10, 140235d00, 140235d80 (defdbio; called from defmodel, layering
//          question reported); Fortran 140437aa9 (column min/max)
//   none   Fortran runtime sincos 1404b3b30 (sin/cos core 1404b3960): not in the ledger
//   p1-11  Fortran 1404cf5c0 (direction -> rotation matrix), 1404ded0c (transform points)
//   p1-09  StateVar collection handle 14032a430/14032a4a0/14032a140/14032a160/14032a4e0/
//          14032a570/14032a680, StateVarBase reset 140300e30
//   p1-05  COW detach/at 14019a860, key-set dtor 14019acf0
#pragma once

#include <cstdint>

namespace def::model {

class DYNARY;
class Cls_1401364a0;
class StateVarBase;

namespace ext {

// ---- p1-04: furnace curve list (Cls_1401364a0) ----
DYNARY* fun_140136670(Cls_1401364a0* list);                 // append a new curve, return it
DYNARY* fun_140136860(Cls_1401364a0* list, std::int32_t i); // i-th curve, 1-based
std::int32_t fun_14013c1b0(Cls_1401364a0* list);                   // count
void fun_140136910(Cls_1401364a0* list);                           // delete all curves

// ---- curve (DYNARY) ----
void fun_14012d2a0(DYNARY* c, std::int32_t type);                                // init(type)
void fun_14012fad0(DYNARY* c);                                                   // allocate arrays
double fun_140130720(DYNARY* c, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l);
void fun_140130800(DYNARY* c, double v, std::int32_t i, std::int32_t j, std::int32_t k,
                   std::int32_t l);
void fun_14012e400(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes);
void fun_14012e6d0(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes);

// ---- units (p1-12) ----
std::int32_t fun_140235c10();                            // current unit system: 1 or 2
double fun_140235d00(std::int32_t kind, std::int32_t mode); // conversion factor
double fun_140235d80(std::int32_t mode, double t);       // temperature conversion

// ---- Fortran kernels (p1-11, p1-12); arguments by reference as in the binary ----
void fun_1404b3b30(double a, double* s, double* c);      // s = sin(a), c = cos(a)
void fun_1404cf5c0(const double* dir, double* rot);      // rot(3,3) from a direction vector
void fun_1404ded0c(double* pts, double* out, const double* origin, const double* rot,
                   const std::int32_t* n, const std::int32_t* mode);
void fun_140437aa9(const double* a, const std::int32_t* ld, const std::int32_t* n,
                   const std::int32_t* comp, double* mn, double* mx, const std::int32_t* init);

// ---- StateVar collection handle (p1-09) and COW helpers (p1-05) ----
// `self` is the object holding the handle at offset 0 (Cls_140329c70); `set` a key set passed
// by value (its dtor is 14019acf0).
void fun_14032a430(void* self);                                   // handle ctor
void fun_14032a4a0(void* self);                                   // handle dtor
std::int32_t fun_14032a140(void* self);                           // count
StateVarBase** fun_14019a860(void* self, std::int64_t i);         // element ref (detaches)
StateVarBase* fun_14032a160(void* self, std::int32_t i);          // element or null
void fun_14032a680(void* self);                                   // clear
void fun_14032a4e0(void* self, void* it_out, StateVarBase* const* item); // insert at end
std::int32_t fun_14032a570(void* set, const std::int32_t* key);   // lookup in the key set
void fun_14019acf0(void* set);                                    // key-set dtor
void fun_140300e30(StateVarBase* v);                              // reset values

} // namespace ext
} // namespace def::model
