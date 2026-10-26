// Globals used by p1-09 code (docs/port-conventions.md section 5). Move to the owner's header
// when another packet turns out to own them.
#pragma once

#include <cstdint>

#include <defrt/ftn/common_140b84720.hpp>

namespace def::model {

// COMMON 140b84720 (three doubles read by the 2D curve kernels 14032b34b / 14032c4a0 and by
// the geometry kernels): one object, owned by defrt (def::rt::geoutl::g_common_140b84720 in
// defrt/ftn/common_140b84720.hpp; members u00 @140b84720, u08 @140b84728, u10 @140b84730). A
// zero entry is replaced by its default on first use: 1e-6, 1e-6, 1.0.
using Common140b84720 = def::rt::geoutl::Common140b84720;
inline Common140b84720& g_common_140b84720 = def::rt::geoutl::g_common_140b84720;

// @data 1405f3c00  SAVE variable of 14032c4a0: min(scale * u08, 1e-6); .data initial 1e-6
inline double dat_1405f3c00 = 1e-06;
// @data 1405f3c08  SAVE variable of 14032c4a0: min(scale * u00, 1e-6); .data initial 0.001
inline double dat_1405f3c08 = 0.001;

// 1403c4504, called by 14032ac75 after its "TOO LARGE SWEEPING ANGLE" message, is the licence /
// process-monitor routine: excluded (licence code, SFTC/user rule), never ported, analysed or
// wired. The hook stays null and the port stops the run there through def::rt::not_ported.
inline void (*g_stop_1403c4504)() = nullptr;

inline void reset_globals_p1_09()
{
    g_common_140b84720 = {0.0, 0.0, 0.0};
    dat_1405f3c00 = 1e-06;
    dat_1405f3c08 = 0.001;
}

} // namespace def::model
