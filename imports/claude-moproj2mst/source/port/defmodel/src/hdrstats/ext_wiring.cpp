// Adapters from packet p4-07's ext hooks to the owners that are already on this branch
// (claude/kw-merge). Declarations: defmodel/hdrstats/ext_p4_07.hpp, namespace wired.
#include <defmodel/hdrstats/ext_p4_07.hpp>

#include <material/object_deps.hpp>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/step/name_arrays.hpp>

namespace def::model::p407::wired {

// 140158ce0 -> geo_kw::object_dim
std::int32_t fun_140158ce0(const LLOBJ_DATA* o) { return geo_kw::object_dim(*const_cast<LLOBJ_DATA*>(o)); }
// 140160d90 -> geo_kw::has_surface_flag
bool fun_140160d90(const LLOBJ_DATA* o) { return geo_kw::has_surface_flag(*const_cast<LLOBJ_DATA*>(o)); }
// 140160f10 -> llobj_not_kind7 (material/object_deps.hpp)
bool fun_140160f10(const LLOBJ_DATA* o) { return llobj_not_kind7(o); }
// 140198620 -> geo_kw::surface
void* fun_140198620(LLOBJ_DATA* o, const std::int32_t* i) { return geo_kw::surface(*o, *i); }
// 140005510 -> geo_kw::geo3d_elems
std::int32_t* fun_140005510(void* g) { return geo_kw::geo3d_elems(static_cast<OBJ_GEO3D*>(g)); }
// 140006410 -> geo_kw::geo3d_coords
double* fun_140006410(void* g) { return geo_kw::geo3d_coords(static_cast<OBJ_GEO3D*>(g)); }
// 14017ffe0 -> bc::node_set_alloc
void fun_14017ffe0(void* set, std::int32_t n, std::int32_t* dims, std::int32_t key)
{
    bc::node_set_alloc(set, n, dims, key);
}
// 1401836c0 -> bc::element_set_alloc
void fun_1401836c0(void* set, std::int32_t n, std::int32_t* dims, std::int32_t m, std::int32_t key)
{
    bc::element_set_alloc(set, n, dims, m, key);
}
// 1401cbbc0 -> node_name_count
std::int32_t fun_1401cbbc0(DB_STEP_DATA* m) { return node_name_count(m); }
// 1401cbc80 -> element_name_count
std::int32_t fun_1401cbc80(DB_STEP_DATA* m) { return element_name_count(m); }
// 140257150 -> bc::calloc_doubles
double* fun_140257150(std::int32_t n) { return bc::calloc_doubles(n); }
// 14017f020 -> fun_14017f020 (defmodel/node_access.hpp); wired here at the P6-7 gate so that
// node_var_02 (1401809c0, also reached through node_access.cpp) works without defdbio's wiring
std::int32_t* fun_14017f020(void* set) { return model::fun_14017f020(static_cast<Cls_14017ef20*>(set)); }
// 1401d48d0 -> fun_1401d48d0 (defmodel/node_access.hpp), same reason
void fun_1401d48d0(DB_STEP_DATA* m, const std::int32_t* id) { model::fun_1401d48d0(m, id); }
// 140182980 -> svkw::set9e0_dims (kw-g2sv); wired here (p9-ac8a0) so that the element getters
// elem_var_* work without defdbio's wiring, as 14017f020 above
std::int32_t* fun_140182980(void* set) { return svkw::set9e0_dims(set); }

} // namespace def::model::p407::wired
