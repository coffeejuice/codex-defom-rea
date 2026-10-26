// The STNCMP offset getter 140212460 (packet p9-03a). The STEP.DAT writer 1402ac8a0 that lived
// here, with its bare-matrix writers 1402f7490 / 1402a7b60, is owned by p9-ac8a0
// (defcmd/step_dat.hpp write_step_dat; defkey kw_g2sv for the matrices).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d_sweep.hpp>

#include <cstdio>

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;
namespace crt = def::rt::crt;

// @src 140212460  obj+0x3a98 (owner model) null -> 0, else 1401d72e0(owner, i)
std::int32_t fun_140212460(LLOBJ_DATA* obj, std::int32_t i)
{
    if (obj->owner == nullptr) return 0;
    return E().fun_1401d72e0(obj->owner, i);
}

} // namespace def::cmd::cnvt3d_sweep
