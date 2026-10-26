// Inter-object record operations that no Phase 1 packet owns (packet kw-g3): the list add/find/nth
// of Cls_1401fa020 (DB_STEP_DATA +0x13c8, ctor/clear by p1-06), the mirror copy of
// Cls_1401f46e0 used after every group 3 keyword, and the DYNARY copy 14012eff0 it needs.
// Bodies in src/window/inter_object_ops.cpp. Spec: docs/re/keywords/CNTACT.md (mirroring).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/cls_1401fb020.hpp>
#include <defmodel/layout/dynary.hpp>

namespace def::model {

class DB_STEP_DATA;

bool model_is_2d(const DB_STEP_DATA* m);  // 1400015e0 (dimension == 2)
bool model_is_3d(const DB_STEP_DATA* m);  // 1400015f0 (dimension == 3)

std::int32_t pair_count(const Cls_1401fa020* l);                                       // 1400015d0
Cls_1401f46e0* pair_append(Cls_1401fa020* l);                                          // 1401faaa0
Cls_1401f46e0* pair_nth(Cls_1401fa020* l, std::int32_t n);                             // 1401fac90
Cls_1401f46e0* pair_find(Cls_1401fa020* l, std::int32_t master, std::int32_t slave);   // 1401facf0

void pair_copy_keep_ids(Cls_1401f46e0* dst, const Cls_1401f46e0* src);  // 1401f9e80
void pair_copy(Cls_1401f46e0* dst, const Cls_1401f46e0* src);           // 1401f9900
void friction_copy(Cls_1401fb020* dst, const Cls_1401fb020* src);       // 1401f9d50
void ihtcof_copy(Cls_1401f46e0* dst, const Cls_1401f46e0* src);         // 1401f9e10

// FUN_14012eff0: copy shape, constant and arrays of `src` into `dst` (re-allocating dst). Part of
// the DYNARY family; can move next to dynary_init.hpp at merge.
void dynary_copy(DYNARY* dst, const DYNARY* src);

} // namespace def::model
