// Calls from p1-10 (and p1-03, which declares the DYNARY ones too) into other packets
// (members/ext_deps.hpp), wired to the owners at the Phase 1 gate. Each forwarder names the
// function it stands for; the owners carry the @src tags.
#include <defmodel/members/ext_deps.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/topo/ftn_geom.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftn/units.hpp>
#include <defrt/stdlib/libm.hpp>

#include <algorithm>
#include <new>

namespace def::model::ext {

namespace {

double factor_fn(int kind, int mode) { return rt::ftn::unit_factor(kind, mode); }
double temperature_fn(int mode, double t) { return rt::ftn::convert_temperature(mode, t); }
const UnitFunctions kUnits{factor_fn, temperature_fn};

using SvList = CowList<StateVarBase*>;
using KeySet = CowList<std::int32_t>;

SvList* sv_list(void* self) { return static_cast<SvList*>(self); }

} // namespace

// ---- p1-04: furnace curve list (Cls_1401364a0) ----
// part of @src 140136670  append a new curve
DYNARY* fun_140136670(Cls_1401364a0* list) { return list->add(); }
// part of @src 140136860  i-th curve, 1-based
DYNARY* fun_140136860(Cls_1401364a0* list, std::int32_t i) { return list->at(i); }
// part of @src 14013c1b0  count
std::int32_t fun_14013c1b0(Cls_1401364a0* list) { return list->size(); }
// part of @src 140136910  delete all curves
void fun_140136910(Cls_1401364a0* list) { list->clear(); }

// ---- DYNARY (key-common, p1-04) ----
// part of @src 14012d2a0
void fun_14012d2a0(DYNARY* c, std::int32_t type) { dynary_init(c, type); }
// part of @src 14012fad0
void fun_14012fad0(DYNARY* c) { dynary_alloc(c); }
// part of @src 140130720
double fun_140130720(DYNARY* c, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l)
{
    return llobj::dynary_get(*c, i, j, k, l);
}
// part of @src 140130800
void fun_140130800(DYNARY* c, double v, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l)
{
    llobj::dynary_set(*c, v, i, j, k, l);
}
// part of @src 14012e400
void fun_14012e400(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes)
{
    dynary_convert_units(c, kind, mode, axes, kUnits);
}
// part of @src 14012e6d0
void fun_14012e6d0(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes)
{
    dynary_convert_units_inverse(c, kind, mode, axes, kUnits);
}

// ---- units (p1-12, in defrt) ----
// part of @src 140235c10
std::int32_t fun_140235c10() { return rt::ftn::unit_system(); }
// part of @src 140235d00
double fun_140235d00(std::int32_t kind, std::int32_t mode) { return rt::ftn::unit_factor(kind, mode); }
// part of @src 140235d80
double fun_140235d80(std::int32_t mode, double t) { return rt::ftn::convert_temperature(mode, t); }

// ---- Fortran kernels ----
// part of @src 1404b3b30  (p1-01 libm)
void fun_1404b3b30(double a, double* s, double* c) { rt::libm::ps_sincos(a, s, c); }
// part of @src 1404cf5c0  (p1-11)
void fun_1404cf5c0(const double* dir, double* rot) { topo::direction_matrix(dir, rot); }
// part of @src 1404ded0c  (p1-11)
void fun_1404ded0c(double* pts, double* out, const double* origin, const double* rot,
                   const std::int32_t* n, const std::int32_t* mode)
{
    topo::transform_points(pts, out, origin, rot, n, mode);
}
// part of @src 140437aa9  (p1-12, in defrt)
void fun_140437aa9(const double* a, const std::int32_t* ld, const std::int32_t* n,
                   const std::int32_t* comp, double* mn, double* mx, const std::int32_t* init)
{
    rt::ftn::column_min_max(a, ld, n, comp, mn, mx, init);
}

// ---- StateVar collection handle: CowList<StateVarBase*> at offset 0 (p1-05 cow.hpp) ----
// part of @src 14032a430  handle ctor
void fun_14032a430(void* self) { ::new (self) SvList(); }
// part of @src 14032a4a0  handle dtor
void fun_14032a4a0(void* self) { sv_list(self)->~SvList(); }
// part of @src 14032a140  count
std::int32_t fun_14032a140(void* self) { return static_cast<std::int32_t>(sv_list(self)->size()); }
// part of @src 14019a860  element reference for writing (detaches)
StateVarBase** fun_14019a860(void* self, std::int64_t i)
{
    return &sv_list(self)->mutable_at(static_cast<std::size_t>(i));
}
// part of @src 14032a160  element i, or null when i < 0 or i >= count
StateVarBase* fun_14032a160(void* self, std::int32_t i)
{
    const std::int32_t n = fun_14032a140(self);
    if (i < 0 || n <= i) {
        return nullptr;
    }
    return sv_list(self)->at(static_cast<std::size_t>(i)); // 14032a540
}
// part of @src 14032a680  clear (new body when shared)
void fun_14032a680(void* self) { sv_list(self)->clear(); }
// part of @src 14032a4e0  insert at the end
void fun_14032a4e0(void* self, void*, StateVarBase* const* item) { sv_list(self)->push_back(*item); }
// part of @src 14032a570  occurrences of *key in the key set (14032a840)
std::int32_t fun_14032a570(void* set, const std::int32_t* key)
{
    const auto& items = static_cast<KeySet*>(set)->items();
    return static_cast<std::int32_t>(std::count(items.begin(), items.end(), *key));
}
// part of @src 14019acf0  key-set dtor
void fun_14019acf0(void* set) { static_cast<KeySet*>(set)->~KeySet(); }
// part of @src 140300e30  StateVarBase: reset the values
void fun_140300e30(StateVarBase* v) { v->clear(); }

} // namespace def::model::ext
