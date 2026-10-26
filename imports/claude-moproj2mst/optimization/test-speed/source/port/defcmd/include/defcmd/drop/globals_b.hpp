// defcmd/drop/globals_b.hpp: module statics of the drop contact routines (packet p67-3, worker B)
// that a call can read before it writes them (docs/port-conventions.md §5). Everything else these
// routines keep in statics is written before it is read and is a C++ local.
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// ---- 140373650 (result extraction) ----
// @data 140706720  NI4 of the current node: codes c(1..3), polygons p(1..3) (a Get on a node
// without NI4 leaves the previous node's values); @data 140706740  its count
inline i32 g_x650_ni4[6]{};
inline i32 g_x650_ni4_cnt = 0;
// @data 140706760  ER1 of the contact polygon; @data 140706780  its count
inline double g_x650_er1[3]{};
inline i32 g_x650_er1_cnt = 0;
// @data 140706790  Xp(3,4); @data 1407067f0  node numbers; @data 140706800  node count (14004f363)
inline double g_x650_xp[12]{};
inline i32 g_x650_conn[4]{};
inline i32 g_x650_cnt = 0;
// @data 140706808  xi; @data 140706810  eta (kept when the kernels do not write them);
// @data 140706818  bad
inline double g_x650_xi = 0.0;
inline double g_x650_eta = 0.0;
inline i32 g_x650_bad = 0;

// ---- 140374954 (triangle local coordinates) ----
// @data 140706a70  local triangle V'(3,4); node 4 is written only (as a copy of node 1) before
// 1404d41f8, so 1404cc289 / 1404e2d44 may see the previous call's value there
inline double g_x954_v[12]{};

// Resets every global of this header (tests).
inline void reset_globals_b()
{
    for (i32& v : g_x650_ni4) v = 0;
    g_x650_ni4_cnt = g_x650_er1_cnt = g_x650_cnt = g_x650_bad = 0;
    for (double& v : g_x650_er1) v = 0.0;
    for (double& v : g_x650_xp) v = 0.0;
    for (i32& v : g_x650_conn) v = 0;
    g_x650_xi = g_x650_eta = 0.0;
    for (double& v : g_x954_v) v = 0.0;
}

} // namespace def::cmd::drop
