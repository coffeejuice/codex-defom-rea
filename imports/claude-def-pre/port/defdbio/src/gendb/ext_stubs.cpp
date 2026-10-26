// Stubs for the calls of gendb/ext.hpp, one per address, until the owning packet is merged
// (port-conventions: a call into another packet ends in DEF_NOT_PORTED until it lands).
#include <defdbio/gendb/ext.hpp>
#include <defdbio/gendb/step_header.hpp>
#include <defdbio/gendb/checks_p4_04.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/p4_06_frame.hpp>

#include <defmodel/material/state_remap.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/port.hpp>

namespace def::dbio::gendb::ext {

// 1401bc230 is ported (p4-02b, gendb/p4_02b/frame.cpp): forwarded
void input_counts(DB_STEP_DATA& a0, std::int32_t* a1)
{
    if (g_hooks.input_counts != nullptr) {
        g_hooks.input_counts(a0, a1);
        return;
    }
    counts_frame(&a0, a1); // p4-02b
}
// 1401b9d50 is ported (p4-03): forwarded; was stub for 1401b9d50 (p4-03)
bool object_checks(DB_STEP_DATA& a0)
{
    if (g_hooks.object_checks != nullptr) {
        return g_hooks.object_checks(a0);
    }
    return ::def::dbio::objcheck::gendb_model_checks(&a0); // p4-03
}
// 1401bebf0 is ported (p4-04): forwarded; was stub for 1401bebf0 (p4-04)
void db_checks(DB_STEP_DATA& a0, std::int32_t* a1, std::int32_t* a2)
{
    if (g_hooks.db_checks != nullptr) {
        g_hooks.db_checks(a0, a1, a2);
        return;
    }
    check_chain(a0, a1, a2); // p4-04
}
// 1401c4950 is ported (p4-06, gendb/p4_06/frame.cpp): forwarded
void write_sim_control(DB_STEP_DATA& a0, std::int32_t* a1, double* a2, double* a3, double* a4, std::int32_t* a5, std::int32_t* a6)
{
    if (g_hooks.write_sim_control != nullptr) {
        g_hooks.write_sim_control(a0, a1, a2, a3, a4, a5, a6);
        return;
    }
    write_step_sim_ctrl(&a0, a1, a2, a3, a4, a5, a6); // p4-06
}
// 1401c49b0 is ported (p4-06, gendb/p4_06/frame.cpp): forwarded
void write_materials(DB_STEP_DATA& a0, double* a1, double* a2, std::int32_t* a3)
{
    if (g_hooks.write_materials != nullptr) {
        g_hooks.write_materials(a0, a1, a2, a3);
        return;
    }
    write_step_materials(&a0, a1, a2, a3); // p4-06
}
// 1401c4d90 is ported (p4-06, gendb/p4_06/frame.cpp): forwarded
void write_objects(DB_STEP_DATA& a0, std::int32_t* a1, double* a2, std::int32_t* a3, std::int32_t* a4, double* a5, std::int32_t* a6, double* a7, double* a8)
{
    if (g_hooks.write_objects != nullptr) {
        g_hooks.write_objects(a0, a1, a2, a3, a4, a5, a6, a7, a8);
        return;
    }
    write_step_objects(&a0, a1, a2, a3, a4, a5, a6, a7, a8); // p4-06
}
// 1401c4ea0 is ported (p4-06, gendb/p4_06/frame.cpp): forwarded
void write_inter_objects(DB_STEP_DATA& a0, double* a1, double* a2, std::int32_t* a3, double* a4, double* a5, double* a6)
{
    if (g_hooks.write_inter_objects != nullptr) {
        g_hooks.write_inter_objects(a0, a1, a2, a3, a4, a5, a6);
        return;
    }
    write_step_inter_objects(&a0, a1, a2, a3, a4, a5, a6); // p4-06
}
// 1401c4fa0 is ported (p4-06, gendb/p4_06/frame.cpp): forwarded
void write_material_pairs(DB_STEP_DATA& a0, double* a1, double* a2, std::int32_t* a3)
{
    if (g_hooks.write_material_pairs != nullptr) {
        g_hooks.write_material_pairs(a0, a1, a2, a3);
        return;
    }
    write_step_inter_materials(&a0, a1, a2, a3); // p4-06
}
// 1401c5020 is ported (p4-07): forwarded; was stub for 1401c5020 (p4-07)
void write_step_header(DB_STEP_DATA& a0, const char* a1, std::int32_t* a2, std::int32_t* a3, void* a4, const char* a5)
{
    if (g_hooks.write_step_header != nullptr) {
        g_hooks.write_step_header(a0, a1, a2, a3, a4, a5);
        return;
    }
    ::def::dbio::gendb::write_step_header(&a0, const_cast<char*>(a1), a2, a3, static_cast<model::Cls_1401dbfe0*>(a4), a5); // p4-07
}
// 1401d0fc0 is ported (kw-g4b, defmodel/material/state_remap.cpp): forwarded
void step_remap_material_state(DB_STEP_DATA& a0, bool a1)
{
    if (g_hooks.step_remap_material_state != nullptr) {
        g_hooks.step_remap_material_state(a0, a1);
        return;
    }
    ::def::model::step_remap_material_state(a0, a1);
}
// 1401d7160 is ported (kw-g2sv, defmodel/statevar_kw): forwarded
std::int32_t fun_1401d7160(DB_STEP_DATA& a0, std::int32_t* a1)
{
    if (g_hooks.fun_1401d7160 != nullptr) {
        return g_hooks.fun_1401d7160(a0, a1);
    }
    return ::def::model::svkw::grain_model(a0, reinterpret_cast<const bool*>(a1)); // kw-g2sv
}

} // namespace def::dbio::gendb::ext
