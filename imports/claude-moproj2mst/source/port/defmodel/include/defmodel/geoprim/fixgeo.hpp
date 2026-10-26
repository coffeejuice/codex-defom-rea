// defmodel/geoprim/fixgeo.hpp: FIXGEO, the external profile clean-up the GEOPRIM revolve (p9-05,
// 1402da250) and the CNVT3D build (p9-03, 14041e270) run on a 2D outline, with its option record
// and the outline file reader/writer. Owner: packet p9-01 (docs/phase5_8_9_10-packets.md,
// "Ownership"). Spec: docs/re/commands/DXF_IGES_READERS.md section 5.4, CNVT3D.md section 5.1.
//
// FIXGEO writes the outline to FIXGEO_BUF.GEO and its options to FIXGEO.INI, runs
// "FIXGEO.EXE > DEF_GEO.TMP" through the launcher (140224100, dirKind 2, owner p8-07 in defcmd)
// and reads NEWGEO.GEO back when the program wrote one. defmodel cannot call defcmd, so the
// launch goes through g_fixgeo_launch, which defcmd sets (def::cmd::cnvt3d::bind_fixgeo_launch,
// called by the CNVT3D entry points); unset it stops in DEF_NOT_PORTED(0x140224100).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {
class OBJ_GEO2D;
}

namespace def::model::geoprim {

// The FIXGEO option record (0x30 bytes on the callers' stacks).
struct FixgeoOptions {
    // @src 140220390  ctor: tol_a = tol_b = 0.001 (DAT_14051f1e8), angle_a = 20.0 (DAT_14051f1f0),
    //                 angle_b = 300.0 (DAT_140530c00), i20 = i24 = 0, b28 = b29 = 0
    FixgeoOptions();
    // @src 140220420  dtor (empty)
    ~FixgeoOptions();

    double tol_a;        // 0x00 written when i20 == 0
    double tol_b;        // 0x08 written when i20 != 0
    double angle_a;      // 0x10 degrees; written as angle_a * 0.5 * (pi/180)
    double angle_b;      // 0x18 degrees; written as angle_b * (pi/180)
    std::int32_t i20;    // 0x20
    std::int32_t i24;    // 0x24
    std::uint8_t b28;    // 0x28
    std::uint8_t b29;    // 0x29
    std::byte pad_2a[6];
};
static_assert(sizeof(FixgeoOptions) == 0x30);
DEF_LAYOUT_OFFSET(FixgeoOptions, angle_b, 0x18);
DEF_LAYOUT_OFFSET(FixgeoOptions, i20, 0x20);
DEF_LAYOUT_OFFSET(FixgeoOptions, b28, 0x28);

// @src 14027ece0  FIXGEO: 0 when NEWGEO.GEO was not produced, 1 when it was read back into `geo`,
// -1 (FIXGEO_BUF.GEO not written), -2 (FIXGEO.INI not opened), -3 (NEWGEO.GEO unreadable).
// del_pntgeo != 0 also deletes PNTGEO.GEO; with check != 0, *neg = fun_14027efa0(geo).
std::int32_t fixgeo(OBJ_GEO2D* geo, const FixgeoOptions* opt, std::int32_t* neg,
                    std::int32_t del_pntgeo, char check);

// @src 14027cc00  write the outline file ("wt"): "  %d\n" type+1, "  %d   %d\n" n, 1, then per
// row " %d" and the values " %20.10e" (type 2: the first value as an int, the last in degrees).
// -2 no name, -3 open failed, 0 ok.
std::int32_t write_geo_file(const OBJ_GEO2D* geo, const char* name);
// @src 14027ce90  read an outline file written by 14027cc00 / FIXGEO ("rt"); -2 / -3 / 0 as above
std::int32_t read_geo_file(OBJ_GEO2D* geo, const char* name);
// @src 14027efa0  0 when no point has x < 0; 1 when some do but all within 0.0005 * the box
// width; -1 when one lies further left. Width <= 1e-6: 0.
std::int32_t negative_x_check(OBJ_GEO2D* geo);
// @src 14027f450  the point array (+0x18)
double* geo_points(const OBJ_GEO2D* geo);

// The launcher 140224100 (mode, command line): set by defcmd.
using FixgeoLaunch = void (*)(std::int32_t mode, const char* cmdline);
inline FixgeoLaunch g_fixgeo_launch = nullptr;

} // namespace def::model::geoprim
