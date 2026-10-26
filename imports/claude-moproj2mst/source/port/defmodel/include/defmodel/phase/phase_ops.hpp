// Phase-pair list operations and the DIM model routing (packet kw-g57). Bodies in
// src/phase/phase_ops.cpp. The PhasePairList ctor/reset/clear/dtor and the PhasePair dtor are
// p1-10's; the PhasePair ctor is in src/phase/phase_pair_ctor.cpp.
// Specs: docs/re/keywords/_common_g457.md section 5, PHASTF.md, DIM.md.
#pragma once

#include <cstdint>

#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>

namespace def::model {

class DB_STEP_DATA;

std::int32_t phase_pair_count(const PhasePairList* l);                              // 1401b05f0
PhasePair* phase_pair_append(PhasePairList* l);                                     // 140339df0
PhasePair* phase_pair_find(PhasePairList* l, std::int32_t from, std::int32_t to);   // 14033a040
PhasePair* phase_pair_nth(PhasePairList* l, std::int32_t n);                        // 140339fe0
void count_active_phase_pairs(DB_STEP_DATA* m);                                     // 1401d6450

// DIM routing (DB_STEP_DATA +0x33e8 companion, +0x33f0 flag)
DB_STEP_DATA* companion_model(DB_STEP_DATA* m);                   // 1401d8120
DB_STEP_DATA* keyword_target_model(DB_STEP_DATA* m);              // 1401d81b0
void sync_companion_dimension(DB_STEP_DATA* m);                   // 1401d8050
void set_dim_routing(DB_STEP_DATA* m, const std::int32_t* dim);   // 1401d8140

} // namespace def::model
