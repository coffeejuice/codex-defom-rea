// Model UNIT setter (GUI twin 0x140822210 = DEF_PRE 1401c8f70) and the simulation-control setter
// it calls. They live in defkey because the reset they trigger (1401f4100) is ported here
// (src/kw_g1a/sim_ctrl_ops.cpp) and defmodel cannot call down into defkey.
// Header: defkey/item_writers.hpp. Decompile: deform-rea-work/out/mdt3/f.c.
#include <defkey/item_writers.hpp>

#include <defmodel/layout/db_step_data.hpp>

#include "../kw_g1a/sim_ctrl_ops.hpp"

namespace def::key {

namespace {
// @src 1401f40b0  Cls_1401e7b10: UNIT (+0xfc) = unit; on a change with `reset`, 1401f4100
void sim_ctrl_set_unit(model::Cls_1401e7b10& s, std::int32_t unit, bool reset)
{
    if (s.unit != unit && (s.unit = unit, reset)) g1a::ops::reset_unit_defaults(s); // 1401f4100
}
} // namespace

// @src 1401c8f70  UNIT of the model's simulation control (+0x750)
void set_model_unit(model::DB_STEP_DATA& m, std::int32_t unit, bool reset)
{
    sim_ctrl_set_unit(m.sim_ctrl, unit, reset);
}

} // namespace def::key
