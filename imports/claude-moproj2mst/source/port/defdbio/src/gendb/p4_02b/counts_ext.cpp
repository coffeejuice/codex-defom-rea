// Defaults of the p4-02b ext table (counts_ext.hpp). Owners on the Phase 4 gate are called through
// the o_* adapters (the DYNARY lengths: gendb/gate_ports.hpp); an interim copy remains for
// 14017f020, DEF_NOT_PORTED for the rest.
#include <defdbio/gendb/counts_ext.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defmodel/node_access.hpp>

#include <cstdlib>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defkey/gendb_deps.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/port.hpp>

namespace def::dbio::gendb {

namespace {

using model::DYNARY;

// 14012fff0 / 140130160 / 140130200 / 140130290: the Phase 4 gate ports (gendb/gate_ports.hpp)

// 14017f020: the p67-3 port (defmodel/node_access)
std::int32_t* d_14017f020(void* mesh) { return model::fun_14017f020(static_cast<model::Cls_14017ef20*>(mesh)); }

// Owners on the Phase 4 gate (adapters for the table's pointer signatures).
model::LLOBJ_DATA* o_14017aaf0(model::Cls_14017a6c0* l, std::int32_t number)
{
    return model::object_list_find(l, number); // 14017aaf0 (kw-g4b)
}
std::int32_t o_140198310(model::LLOBJ_DATA* o) { return model::geo_kw::curve_count(*o); }
model::OBJ_GEO2D* o_1401982c0(model::LLOBJ_DATA* o, const std::int32_t* i) { return model::geo_kw::curve(*o, *i); }
bool o_140160d20(model::LLOBJ_DATA* o) { return model::geo_kw::geometry_has_edges(*o); }
bool o_140304e40(model::LLOBJ_DATA* o, std::int32_t flag)
{
    return model::geo_kw::remove_degenerate_curves(*o, flag != 0) != 0; // 0 or 1
}
void o_1401f3f80(model::Cls_1401e7b10* s) { key::g1a::ops::recompute_smode(*s); }
void o_1401d2090(model::DB_STEP_DATA* m, std::int32_t w) { model::resize_element_names(m, w); }
void o_1401d21c0(model::DB_STEP_DATA* m, std::int32_t w) { model::resize_node_names(m, w); }
model::Cls_1401f46e0* o_1401facf0(model::Cls_1401fa020* l, std::int32_t i, std::int32_t j)
{
    return model::pair_find(l, i, j);
}
void* o_140269900(void* l, std::int32_t n)
{
    return model::winlist_nth(static_cast<const model::Cls_140269250*>(l), n);
}
void* o_14028aff0(void* l, std::int32_t n)
{
    return key::g4::grndat_record_nth(static_cast<const model::Cls_14028b640::RecordList*>(l), n);
}
std::int32_t* o_140256dc0(std::int32_t n) { return model::bc::calloc_ints(n); }

GendbExt defaults()
{
    GendbExt e{};
    e.fun_1401beb30 = count_object_tables;      // p4-02a
    e.fun_140134120 = dynary_sort;              // p4-02a
    e.fun_140130230 = dynary_words_1d_padded;   // p4-02a
    e.fun_140130410 = count_dynary;             // p4-02a
    e.fun_14025d3b0 = count_window;             // p4-02a
    e.fun_1401f3d90 = simctrl_only_deformation; // p4-02a
    e.fun_14012fff0 = dynary_words_3d;
    e.fun_140130160 = dynary_words_2d;
    e.fun_140130200 = dynary_words_1d;
    e.fun_140130290 = dynary_words;
    e.fun_14017aaf0 = o_14017aaf0;
    e.fun_14017f020 = d_14017f020;
    e.fun_140198310 = o_140198310;
    e.fun_1401982c0 = o_1401982c0;
    e.fun_140160d20 = o_140160d20;
    e.fun_140304e40 = o_140304e40;
    e.fun_1401f3f80 = o_1401f3f80;
    e.fun_1401d2090 = o_1401d2090;
    e.fun_1401d21c0 = o_1401d21c0;
    e.fun_1401facf0 = o_1401facf0;
    e.fun_140269900 = o_140269900;
    e.fun_14028aff0 = o_14028aff0;
    e.fun_140256dc0 = o_140256dc0;
    return e;
}

GendbExt g_ext = defaults();

} // namespace

GendbExt& gendb_ext() { return g_ext; }

void reset_gendb_ext() { g_ext = defaults(); }

} // namespace def::dbio::gendb
