// defkey/gendb_deps.hpp: defkey functions that the GENDB counts frame (defdbio, packet p4-02b)
// calls. They are defined in the keyword packets' sources (kw-g1a sim_ctrl_ops.cpp, kw-g4
// group4_writer.cpp); this header only makes the declarations public (Phase 4 gate).
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/db_step_data.hpp>

namespace def::key::g1a::ops {
// 1401f3f80  SMODE recomputed from TRANS[0..1]
void recompute_smode(model::Cls_1401e7b10& s);
// 1401f3c30  TRANS[1] (S+0x108); called by GENDB 3D (packet p4-05)
std::int32_t trans1(const model::Cls_1401e7b10& s);
// 1401f3c10  TRANS[0]
std::int32_t trans0(const model::Cls_1401e7b10& s);
// 1401ee580  time-step flag S+0x240
std::int32_t time_flag(const model::Cls_1401e7b10& s);
// 1401d6ea0  object `id` of the model's list (+0x1380)
model::LLOBJ_DATA* find_object(model::DB_STEP_DATA& m, std::int32_t id);
} // namespace def::key::g1a::ops

namespace def::key::g4 {
// 14028aff0  1-based record i of a GRNDAT record list (i <= 1 gives the head)
model::Cls_14028a4c0* grndat_record_nth(const model::Cls_14028b640::RecordList* l, std::int32_t i);
} // namespace def::key::g4
