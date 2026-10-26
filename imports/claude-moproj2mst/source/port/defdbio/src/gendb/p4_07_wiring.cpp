// Phase 4 gate: points the p4-07 step-header hooks (defmodel/hdrstats/ext_p4_07.hpp) at their
// owners. The owners live in defdbio (p4-01, p4-02a, p4-02b, p4-03, p4-05), which defmodel cannot
// call, so the pointers are set here when the program starts (step_header.cpp runs
// wire_p4_07_externals() from a static initializer). Tests may override them afterwards.
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>

#include "../objcheck/ext.hpp"

namespace def::dbio::gendb {

namespace {

namespace x = def::model::p407::ext;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::p407::Vec3;

// @src 140324000  platform code of the step-header text: returns 2
std::int32_t platform_code() { return 2; }

bool skip_mesh(LLOBJ_DATA* o) { return object_skip_mesh(o); }                            // 140160c20
double volume(LLOBJ_DATA* o, std::int32_t mode) { return objcheck::obj_volume(o, mode); } // 140163540
std::int32_t* node_dims(void* set)                                                         // 14017f020
{
    return model::fun_14017f020(static_cast<model::Cls_14017ef20*>(set));
}
double* node_coords(void* set) { return model::fun_1401808d0(static_cast<model::Cls_14017ef20*>(set)); } // 1401808d0
std::int32_t* elem_dims(void* set) { return model::svkw::set9e0_dims(set); }              // 140182980
double* elem_density(void* set) { return objcheck::elem_block_density(set); }             // 140184050
void mark_var(DB_STEP_DATA* m, const std::int32_t* id) { model::fun_1401d48d0(m, id); }   // 1401d48d0
std::int32_t step_version(DB_STEP_DATA* m) { return stored_version(*m); }                 // 1401cc180
std::int32_t fold(DB_STEP_DATA* m) { return g3d::fold_flag(m); }                           // 14019a2f0
Vec3* make(Vec3* o, double a, double b, double c)                                          // 140239700
{
    vec3_make(o->v, a, b, c);
    return o;
}
double length(const Vec3* v) { return vec3_norm(v->v); }                                   // 140239a30 (gendb)
Vec3* add(Vec3* o, const Vec3* a, const Vec3* b)                                           // 14023a1b0
{
    vec3_add(o->v, a->v, b->v);
    return o;
}
Vec3* cross(Vec3* o, const Vec3* a, const Vec3* b)                                         // 14023a4b0
{
    vec3_cross(o->v, a->v, b->v);
    return o;
}

} // namespace

void wire_p4_07_externals()
{
    x::fun_140160c20 = &skip_mesh;
    x::fun_140163540 = &volume;
    x::fun_14017f020 = &node_dims;
    x::fun_1401808d0 = &node_coords;
    x::fun_140182980 = &elem_dims;
    x::fun_140184050 = &elem_density;
    x::fun_1401d48d0 = &mark_var;
    x::fun_1401cc180 = &step_version;
    x::fun_14019a2f0 = &fold;
    x::fun_140324000 = &platform_code;
    x::fun_140239700 = &make;
    x::fun_140239a30 = &length;
    x::fun_14023a1b0 = &add;
    x::fun_14023a4b0 = &cross;
    namespace sv = def::model::svkw;
    x::fun_1401812a0 = &sv::damage_node_values;
    x::fun_1401814b0 = &sv::timerc_values;
    x::fun_140181530 = &sv::nrecvl_values;
    x::fun_1401815b0 = &sv::usrnod_values;
    x::fun_1401840d0 = &sv::damage_values;
    x::fun_140184150 = &sv::mataxi_values;
    x::fun_140184a30 = &sv::usrelm_values;
}

} // namespace def::dbio::gendb
