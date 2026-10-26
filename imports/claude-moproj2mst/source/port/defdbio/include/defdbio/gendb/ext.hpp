// gendb/ext.hpp: what the p4-01 driver calls in other packets, with the address each stands for.
// Bodies are DEF_NOT_PORTED stubs (ext_stubs.cpp) until the owner lands; tests replace them through
// g_hooks. Owners per docs/phase4-packets.md.
#pragma once

#include <cstdint>
#include <cstdio>

#include <defmodel/layout/db_step_data.hpp>

namespace def::dbio::gendb::ext {

using model::DB_STEP_DATA;

// ---- p4-02b (counts) / p4-03 / p4-04 (checks) ---------------------------------------------------
void input_counts(DB_STEP_DATA& m, std::int32_t* err);                              // 1401bc230
bool object_checks(DB_STEP_DATA& m);                                                // 1401b9d50
void db_checks(DB_STEP_DATA& m, std::int32_t* err, std::int32_t* warn);            // 1401bebf0

// ---- class writers (p4-05, p4-06) and the step header (p4-07) --------------------------------
void write_sim_control(DB_STEP_DATA& m, std::int32_t* iisim, double* llsim, double* llcpu,
                       double* llfnc, std::int32_t* iifnc, std::int32_t* iiuvl);   // 1401c4950
void write_materials(DB_STEP_DATA& m, double* llmpr, double* llfnc, std::int32_t* iifnc); // 1401c49b0
void write_objects(DB_STEP_DATA& m, std::int32_t* mar, double* a, std::int32_t* mar_start,
                   std::int32_t* a_start, double* llfnc, std::int32_t* iifnc, double* llwin,
                   double* llwrz);                                                 // 1401c4d90
void write_inter_objects(DB_STEP_DATA& m, double* llbrt, double* llfnc, std::int32_t* iifnc,
                         double* llwin, double* llwrz, double* llbnd);             // 1401c4ea0
void write_material_pairs(DB_STEP_DATA& m, double* lltfm, double* llfnc,
                          std::int32_t* iifnc);                                    // 1401c4fa0
void write_step_header(DB_STEP_DATA& m, const char* path, std::int32_t* prev, std::int32_t* nh,
                       void* info, const char* keep_version);                     // 1401c5020

// ---- earlier phases, not ported yet -----------------------------------------------------------
// 1401d0fc0 (kw-g4b, defmodel/material/state_remap.hpp on claude/project-thread-hca1wm)
void step_remap_material_state(DB_STEP_DATA& m, bool remap);
// 1401d7160 (P2): -9 without phase data (+0x864 == 0), else 14029bb50(materials, k)
std::int32_t fun_1401d7160(DB_STEP_DATA& m, std::int32_t* k);
// 14025b310..: DYNARY_DATA getters / the SfLinkedList walk (kw-g2mg, defmodel/mesh_gen/sf_list_ops.hpp
// on claude/kw-g2mg); the dynamic-array collection here declares what it needs locally.

// Test hooks: when set, the stub calls the hook instead of ending in DEF_NOT_PORTED.
struct Hooks {
    void (*input_counts)(DB_STEP_DATA&, std::int32_t*) = nullptr;
    bool (*object_checks)(DB_STEP_DATA&) = nullptr;
    void (*db_checks)(DB_STEP_DATA&, std::int32_t*, std::int32_t*) = nullptr;
    void (*write_sim_control)(DB_STEP_DATA&, std::int32_t*, double*, double*, double*, std::int32_t*,
                              std::int32_t*) = nullptr;
    void (*write_materials)(DB_STEP_DATA&, double*, double*, std::int32_t*) = nullptr;
    void (*write_objects)(DB_STEP_DATA&, std::int32_t*, double*, std::int32_t*, std::int32_t*, double*,
                          std::int32_t*, double*, double*) = nullptr;
    void (*write_inter_objects)(DB_STEP_DATA&, double*, double*, std::int32_t*, double*, double*,
                                double*) = nullptr;
    void (*write_material_pairs)(DB_STEP_DATA&, double*, double*, std::int32_t*) = nullptr;
    void (*write_step_header)(DB_STEP_DATA&, const char*, std::int32_t*, std::int32_t*, void*,
                              const char*) = nullptr;
    void (*step_remap_material_state)(DB_STEP_DATA&, bool) = nullptr;
    std::int32_t (*fun_1401d7160)(DB_STEP_DATA&, std::int32_t*) = nullptr;
};
inline Hooks g_hooks{};

} // namespace def::dbio::gendb::ext
