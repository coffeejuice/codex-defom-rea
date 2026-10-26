// defmodel/statevar_kw/statevar_kw.hpp: model functions the group 2 state-variable keywords need
// that no other packet owns (packet kw-g2sv). Bodies in src/statevar_kw/.
//
//   * StateVarCustom (vtable 0x14054ecb0) and the custom-variable list of an object (obj+0xb58,
//     Cls_140185c60 with its Cls_140329c70 collection, p1-04 / p1-10);
//   * accessors of the object's two state-variable sets that (re)size one variable and return
//     its array (obj+0x8a0 Cls_14017ef20, obj+0x9e0 Cls_140182880, p1-04): TIMERC, NRECVL,
//     GRAIN2, NWEAR, USRNOD (set 0x8a0), SIZESH, TXTODF, ERECVL, MATAXI, USRELM (set 0x9e0);
//   * the phase-column map of VOLFC/VOLFS and the grain-model query of GRAIN2.
//
// The collection's element access is p1-09/p1-05 code reached through p1-10's
// members/ext_deps.hpp (wired to the owners at the Phase 1 gate, src/members/ext_deps_wiring.cpp).
#pragma once

#include <cstdint>

#include <defmodel/layout/state_var_base.hpp>

namespace def::model {

class DB_STEP_DATA;
class LLOBJ_DATA;

// StateVarCustom: StateVarBase (id 0x4e) plus the custom-variable id at 0x38.
class StateVarCustom : public StateVarBase {
public:
    // @src 14032ebd0  ctor(id, p): StateVarBase(id, p), kind 0, custom id 0
    StateVarCustom(std::int32_t id, void* p);
    // @src 14032ec40  dtor (slot 0 is the scalar deleting dtor 14032eca0)
    ~StateVarCustom() override;

    std::int32_t custom_id;   // 0x38 14032ec70 / 14032ec90
    std::byte pad_3c[4];      // 0x3c
};
static_assert(sizeof(StateVarCustom) == 0x40);

// FUN_1401d48d0(model, &id): marks the model's DB output variables of a state variable (DB
// writer bookkeeping, no KEY output). With the hook null the port of 1401d48d0 runs
// (def::model::mark_state_var_outputs, step/name_arrays.hpp: returns at once, as the original);
// tests may install a recorder.
inline void (*g_hook_1401d48d0)(DB_STEP_DATA* m, const std::int32_t* id) = nullptr;

namespace svkw {

// ---- custom variables (obj+0xb58) ------------------------------------------------------------
std::int32_t custom_count(LLOBJ_DATA& obj);                          // 14032a140(obj+0xb58)
StateVarCustom* custom_at(LLOBJ_DATA& obj, std::int32_t i);          // 140185f80
StateVarCustom* custom_find(LLOBJ_DATA& obj, std::int32_t id);       // 140185e00
StateVarCustom* custom_get(LLOBJ_DATA& obj, std::int32_t id);        // 140185ed0
StateVarCustom* custom_new(LLOBJ_DATA& obj, std::int32_t id);        // 140186010
StateVarCustom* custom_define(LLOBJ_DATA& obj, std::int32_t id, std::int32_t type,
                              std::int32_t m, std::int32_t nr);       // 140185d20
void custom_fill(StateVarBase* v, double d);                         // 140301620
std::int32_t* model_custom_registry(DB_STEP_DATA& m);                // 14019a4e0 (+0x33b8)
void set_first_int(std::int32_t* p, std::int32_t v);                 // 14032f630

// ---- state-variable set accessors -------------------------------------------------------------
// `set` is obj+0x8a0 (Cls_14017ef20) or obj+0x9e0 (Cls_140182880).
std::int32_t* set8a0_dims(void* set);                                         // 14017f020
std::int32_t* set9e0_dims(void* set);                                         // 140182980
void set8a0_alloc(void* set, std::int32_t n, std::int32_t* dims, std::int32_t key); // 14017ffe0
void set9e0_alloc(void* set, std::int32_t n, std::int32_t* dims, std::int32_t m,
                  std::int32_t key);                                          // 1401836c0
void set8a0_resize(void* set, std::int32_t id, std::int32_t w);               // 140180310
void set9e0_resize(void* set, std::int32_t id, std::int32_t w);               // 140183840
double* nwear_values(void* set8a0, const bool* mark);     // 140181190 (id 0x18, slot 0x88)
double* usrnod_values(void* set8a0);                      // 1401815b0 (id 0x11, slot 0xc8)
double* timerc_values(void* set8a0);                      // 1401814b0 (id 0x22, slot 0xb8)
double* nrecvl_values(void* set8a0);                      // 140181530 (id 0x23, slot 0xc0)
double* grain2_values(void* set8a0, const bool* no_mark); // 140181420 (id 0x1d, slot 0xb0)
double* mataxi_values(void* set9e0, const bool* mark);    // 140184150 (id 0x2e, slot 0x60)
double* usrelm_values(void* set9e0);                      // 140184a30 (id 0x3c, slot 0xe0)
double* sizesh_values(void* set9e0);                      // 1401845b0 (id 0x36, slot 0xa0)
double* txtodf_values(void* set9e0);                      // 140184640 (id 0x37, slot 0xa8)
double* erecvl_values(void* set9e0);                      // 1401849a0 (id 0x3f, slot 0xd8)
double* damage_values(void* set9e0);                      // 1401840d0 (id 0x2d, slot 0x58)
double* grain_values(void* set9e0);                       // 140184520 (id 0x35, slot 0x98)
double* damage_node_values(void* set8a0);                 // 1401812a0 (id 0x1c, slot 0xa0)
// added by packet p9-ac8a0 (STEP.DAT writer 1402ac8a0, defcmd/step_dat.hpp):
double* dtmp_values(void* set8a0);                        // 140180eb0 (id 0x0c, slot 0x60)
double* stress_node_values(void* set8a0);                 // 1401813a0 (id 0x1a, slot 0xa8)
double* dvolf_values(void* set9e0);                       // 140184400 (id 0x33, slot 0x88)
double* ticf_values(void* set9e0);                        // 140184490 (id 0x34, slot 0x90)
double* ylds_values(void* set9e0);                        // 140184760 (id 0x39, slot 0xb8)

// ---- phases and grain model -------------------------------------------------------------------
// FUN_140398350: map[global phase index] = local column or -1, *count = columns, for the
// mixture materials used by the object's elements.
void phase_column_map(DB_STEP_DATA& m, LLOBJ_DATA& obj, std::int32_t* map, std::int32_t* count);
// FUN_1401d7160: the model's grain model code (-9 when model+0x864 == 0).
std::int32_t grain_model(DB_STEP_DATA& m, const bool* only_used);
// FUN_14029bb50: grain model over the material list (+0x17e8 of each material).
std::int32_t material_grain_model(DB_STEP_DATA& m, const bool* only_used);

} // namespace svkw
} // namespace def::model
