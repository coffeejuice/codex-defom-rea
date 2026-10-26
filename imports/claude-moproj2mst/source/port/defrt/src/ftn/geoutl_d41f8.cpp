// PNTEDG (FUN_1404d41f8, FTN_geoutl): nearest edge of a 3- or 4-node polygon to a point, and the
// natural coordinates of the foot point. Spec: docs/re/commands/geoutl_1404d41f8.md. Operation
// order from the listing (L_extra.txt 1404d41f8..1404d4a0e).
#include <defrt/ftn/geoutl_basic.hpp> // 1404e33b7
#include <defrt/ftn/geoutl_poly.hpp>  // 1404d6b13
#include <defrt/ftn/geoutl_proj.hpp>
#include <defrt/ftn/geoutl_tri.hpp>   // 1404e29fa, 1404e2c31

#include <cstdint>

namespace def::rt::geoutl {

// part of @src 1404d41f8: the failure message (three Fortran WRITEs at 1404d46e6..1404d4941:
// " Program stopped in Sub. PNTEDG. It seems a node failed to", "  a polygon it should rest on.",
// and, when the node variable DAT_140757aa8 > 0, "Please check Node" with that node, on the unit
// held in DAT_140757a84). Not ported yet (no Fortran WRITE runtime in defrt; stub in
// geoutl_pending_d41f8.cpp); see the spec.
void fun_1404d41f8_failure_write();

namespace {

// @data 140643ac0  natural (xi, eta) of the quad corners, (2, 0:4) REAL*8 in .rdata; entry 0 is the
// word pair in front of the 1-based table (read only in the failure path, k = 0).
constexpr double kCorner[5][2] = {{-1.0, 1.0}, {-1.0, -1.0}, {1.0, -1.0}, {1.0, 1.0}, {-1.0, 1.0}};

} // namespace

// @src 1404d41f8  PNTEDG: nearest edge of polygon x(3,4) to p; foot q, distance *dmin, natural
// coordinates (*xi, *eta): quad -> (-1..1)^2 along the edge, triangle -> area coordinates
void fun_1404d41f8(double* x, double* p, double* xi, double* eta, double* q, double* dmin)
{
    std::int32_t k = 0;      // 140730d88 best edge
    double best = 1.0e10;    // 140730d90 (DAT_140bb2760)
    std::int32_t same = 0;   // 140730d98
    double qt[3];            // 140730db0
    double t = 0.0;          // 140730dd0
    // 140730dd8 t(0:4): SAVE array; elements 1..n are written before they are read, element 0 is
    // never written by any routine (bss, 0.0) and is read only in the failure path (k = 0), so a
    // zeroed local array is equivalent.
    double tpar[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

    fun_1404e33b7(x, x + 9, &same);
    std::int32_t n = 4;      // 140730d9c
    if (same == 1) n = 3;

    for (std::int32_t i = 1; i <= n; ++i) {
        std::int32_t j = i + 1; // 140730da4
        if (n < j) j = 1;
        fun_1404d6b13(x + 3 * (i - 1), x + 3 * (j - 1), p, qt, &t, dmin);
        const double d = *dmin;
        tpar[i] = t;
        if (best > d) { // 1404d439e COMISD best, d; JP/JBE skip: strict, NaN never wins
            best = d;
            q[0] = qt[0];
            q[1] = qt[1];
            k = i;
            q[2] = qt[2];
        }
    }

    if (k == 0) {
        fun_1404d41f8_failure_write();
        // 1404d495e: FUN_1404edb2f(-1) is a bare RET, execution goes on with k = 0
    }

    std::int32_t k2 = k + 1; // 140730e00
    *dmin = best;
    if (n < k2) k2 = 1;

    if (n == 4) {
        const double tk = tpar[k];
        const double omt = 1.0 - tk;
        *xi = omt * kCorner[k][0] + kCorner[k2][0] * tk;
        *eta = omt * kCorner[k][1] + tk * kCorner[k2][1];
        return;
    }
    if (n != 3) return;

    std::int32_t m = k - 1; // 140730da4
    if (m < 1) m = 3;
    // 1404d4502: DAT_140730e08 = 0.0 (written, never read)
    fun_1404e29fa(x, xi); // *xi holds the area of the whole triangle
    double a = 0.0;       // 140730e10
    fun_1404e2c31(x + 3 * (m - 1), q, x + 3 * (k2 - 1), &a);
    const double r = a / *xi; // 140730e18
    const double s = 1.0 - r; // 140730e20
    if (k == 1) {
        *xi = r;
        *eta = s;
        return;
    }
    if (k == 2) {
        *xi = 0.0;
        *eta = r;
        return;
    }
    *xi = s;
    *eta = 0.0;
}

} // namespace def::rt::geoutl
