// Fortran geometry routines of p1-11 (defmodel/topo/ftn_geom.hpp). The floating-point operation
// order follows the assembly (comments give the instruction addresses where it differs from a
// left-to-right reading of the decompile).
#include <defmodel/topo/ftn_geom.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftn/geoutl_basic.hpp>
#include <defrt/stdlib/libm.hpp>

#include <defmodel/port.hpp>

#include <cmath>
#include <cstdint>

namespace def::model::topo {

namespace {

} // namespace

void reset_globals_p1_11() {
    g_rev_points = RevPointsState{};
    g_dir_matrix = DirMatrixState{};
    g_rot2d = Rot2dState{};
    g_xform = XformState{};
}

// @src 14041f590  reverse the points of a(ncol, npts)
void reverse_point_rows(const std::int32_t* ncol, const std::int32_t* npts, double* a) {
    RevPointsState& g = g_rev_points;
    const std::int32_t nc = *ncol;
    const std::int64_t ld = nc < 0 ? 0 : static_cast<std::int64_t>(nc); // 14041f5d0 cmovge
    if (nc > 6) {
        // the original copies a row into its 6-double SAVE buffer and overruns it
        def::rt::not_ported(0x14041f590, __FILE__, __LINE__);
    }
    if (nc == 6) {
        rt::ftn::degrees_to_radians_if_flagged(a, npts); // 14045a140(a, npts)
        g.i = 1;
        if (1 <= *npts) {
            do {
                double* p = a + static_cast<std::int64_t>(g.i - 1) * ld;
                std::int32_t six = 6;
                rt::ftn::copy_words8_b(p, g.tmp, &six);
                g.type = static_cast<std::int32_t>(p[0]); // cvttsd2si
                if (g.type == 1) {
                    p[3] = g.tmp[1];
                    p[4] = g.tmp[2];
                    p[2] = g.tmp[4];
                    p[1] = g.tmp[3];
                } else {
                    rt::ftn::rotate2d(p + 1, p + 3, p + 5, &g.tmp[1]); // 14043658f
                    p[1] = g.tmp[1];
                    p[2] = g.tmp[2];
                    p[5] = -p[5]; // xorpd with DAT_140bb1440 (sign bit)
                }
                g.i = g.i + 1;
            } while (g.i <= *npts);
        }
    }
    std::int32_t n = *npts;
    const std::int32_t half = n / 2; // sar/and/add/sar: truncation toward zero
    g.i = 1;
    if (1 <= half) {
        do {
            g.j = n;
            double* ri = a + static_cast<std::int64_t>(g.i - 1) * ld;
            rt::ftn::copy_words8_b(ri, g.tmp, ncol);
            double* rj = a + static_cast<std::int64_t>(n - 1) * ld;
            rt::ftn::copy_words8_b(rj, ri, ncol);
            rt::ftn::copy_words8_b(g.tmp, rj, ncol);
            g.i = g.i + 1;
            n = n - 1;
        } while (g.i <= half);
    }
}

// Bodies of 1404cf5c0, 1404d71d7 and 1404ded0c: defrt/src/ftn/geoutl_basic.cpp.
void direction_matrix(const double* v, double* m) { rt::geoutl::fun_1404cf5c0(v, m); }

void rotate_2d(double* p, const double* a) { rt::geoutl::fun_1404d71d7(p, a); }

void transform_points(const double* a, double* b, const double* c, const double* r,
                      const std::int32_t* n, const std::int32_t* mode) {
    rt::geoutl::fun_1404ded0c(a, b, c, r, n, mode);
}

} // namespace def::model::topo
