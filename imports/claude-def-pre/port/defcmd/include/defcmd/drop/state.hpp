// defcmd/drop/state.hpp: shared state of the OBJPOS drop settling solver (packet p67-3).
// Spec: docs/re/commands/OBJPOS_drop_solver.md §2 (arguments, body tables, COMMON, scalars).
//
// The solver module (0x140359a70..0x140375400, Fortran) keeps all its locals static. State that
// several routines share, or that survives from one call to the next, lives here as globals
// tagged with their address. Locals that a routine always writes before it reads them stay C++
// locals in that routine. Fortran allocatable arrays (descriptors at 0x140704390..) are vectors.
//
// Fortran conventions used in the drop code: arrays column-major, 1-based in comments;
// X(:,n) = X + 3*(n-1) doubles; F(:,f) = F + 4*(f-1) ints.
#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

#include <defrt/ftn/geoutl_tri.hpp>

namespace def::cmd::drop {

using i32 = std::int32_t;

// Fortran NINT as compiled (listing: |x| >= 2^52 ? x : x + copysign(0.5, x), then cvttsd2si).
inline i32 ftn_nint(double x)
{
    double y = x;
    if (!(std::fabs(x) >= 4503599627370496.0)) // DAT_140bafc28 = 2^52
        y = x + (x >= 0.0 ? 0.5 : -0.5);
    return static_cast<i32>(y);
}

// Fortran INT (cvttsd2si, truncation).
inline i32 ftn_int(double x) { return static_cast<i32>(x); }

// @data 140b6cca0  body state COMMON (0x140b6cca0..0x140b6ce78), §2.2
struct BodyCommon {
    double c0[3];      // +0x000 cca0: centre of mass at the start (C0)
    double ct[3];      // +0x018 ccb8: tracked centre (CT)
    double a[3][3];    // +0x030 ccd0: tracked points A1, A2, A3 (start C0 + e_k)
    double pad_78[5];  // +0x078 cd18..cd40: not used by the drop
    double inertia[9]; // +0x0a0 cd40: I(3,3) about the centre, column-major
    double massm[9];   // +0x0e8 cd88: m*I3
    double mass;       // +0x130 cdd0: m (volume, density 1)
    double c[3];       // +0x138 cdd8: centre of mass (recomputed from the geometry)
    double ang[3];     // +0x150 cdf0: rotation increment of the last rigid update
    double pad_168[3]; // +0x168 ce08
    double v[3];       // +0x180 ce20: velocity
    double w[3];       // +0x198 ce38: angular velocity
    double acc[6];     // +0x1b0 ce50: acceleration (linear, angular)
};
static_assert(sizeof(BodyCommon) == 0x1e0);
inline BodyCommon g_body{};

// @data 140b6cba0  Gauss points GP(4,4); @data 140b6cc20  weights W(4,4) (§5.1)
inline double g_gauss_p[16]{};
inline double g_gauss_w[16]{};

// COMMON 140b84720 (u00 = 1e-4 projection tolerance, u08 = 0.01, u10 = characteristic length) is
// the shared object def::rt::geoutl::g_common_140b84720; these names refer to its members.
inline double& g_tol_b84720 = rt::geoutl::g_common_140b84720.u00;
inline double& g_tol_b84728 = rt::geoutl::g_common_140b84720.u08;
inline double& g_len_b84730 = rt::geoutl::g_common_140b84720.u10;

// Solver scalars (§2.2). Only those shared between routines or carried between calls.
inline double g_dt = 0.0;     // @data 140704cc8  time step (0.001)
inline i32 g_it = 0;          // @data 140704cd0  iteration counter
inline i32 g_ins = 0;         // @data 140704cf0  "insert partners" flag of initialDrop (0)
inline double g_vel6[6]{};    // @data 140704d30  V(6) unprojected velocity
inline double g_vnorm = 0.0;  // @data 140704d60  |V|
inline double g_weight = 0.0; // @data 140704d68  W = m * 9800
inline double g_kc = 0.0;     // @data 140704d78  stiffness per contact
inline double g_damp = 0.0;   // @data 140704d80  damping (never used)
inline i32 g_nc = 0;          // @data 140704d88  contact node count
inline i32 g_redrop = 0;      // @data 140704d8c
inline double g_k = 0.0;      // @data 140704d90  stiffness
inline i32 g_stop = 0;        // @data 140704d98
inline i32 g_shift = 0;       // @data 140704d9c
inline double g_h = 0.0;      // @data 140704da0  current time step h
inline i32 g_interrupt = 0;   // @data 140704ec4  argument of the empty stub 14001cd40 (always 0)
inline i32 g_nv = 0;          // @data 1407047e0  number of non-zero mode-9 vectors

// Arrays allocated by the setup 140359a70 (descriptor addresses) and by the solver (§2.1).
struct DropArrays {
    std::vector<double> rt;  // @data 140704410  RT(200, nb) real body table
    std::vector<i32> it;     // @data 1407046d0  IT(20, nb) int body table
    std::vector<double> pt;  // @data 140704470  PT(20, nb*nb) pair table
    std::vector<i32> nodes;  // @data 140704620  node list 1..nn
    std::vector<double> nrm; // @data 140704390  NRM(3,3,nn) contact normal per node and slot
    std::vector<double> xe;  // @data 140704730  XE(2,3,nn) (xi, eta) per slot
    std::vector<i32> bc;     // @data 1407045c0  BC(3,nn) contact codes (negative = -body)
    std::vector<i32> fid;    // @data 140704670  FID(3,nn) contact face of the fixed body
    std::vector<i32> flags;  // @data 140704570  node flags (all > 0)
    std::vector<i32> sw;     // @data 1407044f0  SW(2,3,nn), zero, never written
    std::vector<double> fv;  // @data 1407049f0  face vectors FV(3,nf)
    std::vector<double> vn;  // @data 140704a40  node velocities VN(3,nn)
    std::vector<double> vnp; // @data 140704a90  previous VN
    std::vector<i32> l840;   // @data 140704840  node list (contact nodes)
    std::vector<i32> l890;   // @data 140704890  node list (trial contacts)
    std::vector<i32> l8e0;   // @data 1407048e0  node list (fresh contacts)
    std::vector<double> fn;  // @data 140704930  FN(6,nn) contact force/moment (write only)
    std::vector<double> xb;  // @data 140704990  coordinate backup (3*nn)
    std::vector<double> gap; // @data 1407047f0  GAP(nn) signed gap
};
inline DropArrays g_arr{};

// Resets every global of this header (tests).
inline void reset_state()
{
    g_body = {};
    for (double& x : g_gauss_p) x = 0.0;
    for (double& x : g_gauss_w) x = 0.0;
    g_tol_b84720 = g_tol_b84728 = g_len_b84730 = 0.0;
    g_dt = 0.0;
    g_it = g_ins = g_nc = g_redrop = g_stop = g_shift = g_interrupt = g_nv = 0;
    for (double& x : g_vel6) x = 0.0;
    g_vnorm = g_weight = g_kc = g_damp = g_k = g_h = 0.0;
    g_arr = {};
}

} // namespace def::cmd::drop
