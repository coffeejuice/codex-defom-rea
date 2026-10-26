// The on-demand DB readers the node and element set getters call (contact_ext.hpp). In this build
// both bodies are behind a test that always succeeds (1401d2b70 / 1401d2bb0 return 1), so they
// never read anything; the reading branches are kept as DEF_NOT_PORTED in case a later build of
// the guard differs.
#include "contact_ext.hpp"
#include <defmodel/node_access.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include <defrt/port.hpp>

namespace def::cmd::ext {

namespace {
// 1401d2b70: tests the step's byte +7000 (and calls the side-effect-free 1400015f0), returns 1
bool step_guard_1401d2b70(void*) { return true; }
// 1401d2bb0: returns 1
bool step_guard_1401d2bb0(void*) { return true; }
} // namespace

// 1401d48d0: ported in defmodel/node_access.hpp (fun_1401d48d0)
void fun_1401d48d0(void* step, std::int32_t* key)
{
    model::fun_1401d48d0(static_cast<model::DB_STEP_DATA*>(step), key);
}

// @src 1401d5820  read element arrays from the DB step (no-op: guard 1401d2bb0 always true)
void fun_1401d5820(void* step, std::int32_t* /*key*/)
{
    if (!step_guard_1401d2bb0(step)) DEF_NOT_PORTED(0x1401d5820);
}

} // namespace def::cmd::ext
