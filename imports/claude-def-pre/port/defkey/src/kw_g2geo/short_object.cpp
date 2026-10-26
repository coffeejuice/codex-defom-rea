// The "short" per-object KEY writer FUN_1402a1df0 (packet p5-shortkey): banner, OBJTYP, OBJNAM,
// geometry, RZ, ELMCON. Its only caller is the short KEY writer 1401c5d20 (KFWRIT core 1401c5810
// with flags bit 31); every caller of 1401c5810 in DEF_PRE_64 passes flags 0, so the original
// never reaches it (docs/re/status/p5-shortkey.tsv). Decompile and listing:
// deform-rea-work/out/p5sk/a1df0.c, a1df0_asm.txt.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/rdata.hpp>

namespace def::key::g2geo {

namespace {

namespace gk = def::model::geo_kw;
namespace rtc = def::rt::crt;

// rdata 140522fd8, the factor of the arc angle (1402a22ec MULSD xmm6 = p[5] * [140522fd8])
constexpr double kAngleFactor = def::rt::rdata::d_140522fd8;

// part of @src 1402a1df0  2D models: the DIEGEO curves of geometry 0 when it has edges
// (140160d20). Unlike 1402aaa70 there is no degenerate-curve pass and no null test of curve 0.
void write_curves_inline(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (!gk::geometry_has_edges(obj)) return;  // 140160d20
    const std::int32_t type = gk::curve(obj, 0)->curve_type;  // +0x10
    std::uint32_t k = 1;
    std::int32_t total = 0;
    for (std::int32_t i = 0; i < gk::curve_count(obj); ++i) total += gk::curve(obj, i)->n_points;  // +0x14
    rtc::fprintf(f, "%-6s %7i %7i %7i\n", kw(0x9e), n, type, total);  // str 14053b7e0
    for (std::int32_t i = 0; i < gk::curve_count(obj); ++i) {
        gk::curve(obj, i)->set_curve_type(type);  // 1402763d0, result unused
        if (type == 1) {
            if (0 < i) rtc::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", -1, 0.0, 0.0, 0.0);  // 14053b7f8
            for (std::int32_t j = 0; j < gk::curve(obj, i)->n_points; ++j) {
                const double* p = gk::curve(obj, i)->points;
                rtc::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", k, p[j * 3], p[j * 3 + 1], p[j * 3 + 2]);
                ++k;
            }
        } else {
            if (0 < i) {
                rtc::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", -1, 0, 0.0, 0.0, 0.0,
                             0.0, 0.0);  // 14053b838
            }
            for (std::int32_t j = 0; j < gk::curve(obj, i)->n_points; ++j) {
                const double* p = gk::curve(obj, i)->points;
                const double ang = p[j * 6 + 5] * kAngleFactor;  // first, as in the listing
                rtc::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", k,
                             static_cast<std::int32_t>(p[j * 6]),  // CVTTSD2SI
                             p[j * 6 + 1], p[j * 6 + 2], p[j * 6 + 3], p[j * 6 + 4], ang);  // 14053b870
                ++k;
            }
        }
    }
}

}  // namespace

// @src 1402a1df0
int write_object_short(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA* obj, std::uint32_t out_no)
{
    if (obj == nullptr) return 0;
    at<std::uint32_t>(*obj, 0x3f24) = out_no;
    std::uint32_t n = out_no;
    if (out_no == 0) n = at<std::uint32_t>(*obj, 8);
    char banner[0x400];
    rtc::sprintf(banner, "Data for Object #     %i", n);  // str 14053b7b0
    rtc::fprintf(f, "*\n*  %s\n*\n", banner);            // str 14053b7d0
    if (g2obj::write_objtyp(m, f, *obj, n) != 0) {         // 1402a25a0
        at<std::int32_t>(*obj, 0x3f24) = 0;
        return 3;
    }
    (void)g2obj::write_objnam(m, f, obj, n);  // 1402a2530
    if (is_2d(m)) {                           // 1400015e0 on the model
        write_curves_inline(f, *obj, n);
    } else if (gk::has_surface_flag(*obj) || gk::has_curve(*obj, 0)) {  // 140160d90, 140198050(obj, 0)
        (void)surface::write_geometry_dispatch(m, f, *obj, n);           // 1402a26c0
    }
    (void)mesh::write_nodes(m, f, *obj, n);     // 1402a2db0
    (void)mesh::write_elements(m, f, *obj, n);  // 1402a30c0
    at<std::int32_t>(*obj, 0x3f24) = 0;
    return 0;
}

}  // namespace def::key::g2geo
