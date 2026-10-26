// gendb3d/movement.cpp: LLBND movement / die words of an object (OMV_DATA::WriteToDB,
// FUN_1401912a0) and its helpers (packet p4-05). Mapping: docs/re/dbmap/object.md section 4
// (branch claude/project-thread-bz5k5e). `mv` is the sub-object at LLOBJ_DATA+0x2160
// (Cls_14018b220); the record `rec` is one LLBND row, addressed by byte offset as in the
// decompile (rec word w = byte offset 8 * w).
#include <defdbio/gendb3d/obj3d.hpp>

#include <bit>
#include <cmath>
#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>
#include <defrt/rdata.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_movement.hpp"
#include "gendb3d/movement.hpp"

namespace def::dbio::g3d {

namespace {

// Writer constants (values from the listing).
constexpr double k0 = def::rt::rdata::d_14051bed8;    // rdata 14051bed8 (was 0.0)
constexpr double k1 = def::rt::rdata::d_140509c90;    // rdata 140509c90 (was 1.0)
constexpr double k2 = def::rt::rdata::d_140509fb8;    // rdata 140509fb8 (was 2.0)
constexpr double k3 = def::rt::rdata::d_14051f1c8;    // rdata 14051f1c8 (was 3.0)
const double k4 = 4.0;    // @data 14051f248
constexpr double k5 = def::rt::rdata::d_140522f48;    // rdata 140522f48 (was 5.0)
const double k6 = 6.0;    // @data 140522ff0
constexpr double k7 = def::rt::rdata::d_140522f38;    // rdata 140522f38 (was 7.0)
constexpr double k8 = def::rt::rdata::d_140522f40;    // rdata 140522f40 (was 8.0)
const double k9 = 9.0;    // @data 140522fe8
constexpr double k10 = def::rt::rdata::d_14051bee8;  // rdata 14051bee8 (was 10.0)
const double k11 = 11.0;  // @data 140522fe0
constexpr double km1 = def::rt::rdata::d_140522fc8;  // rdata 140522fc8 (was -1.0)
// rdata 14051f260  1.0E-12
constexpr double kSnapTol = def::rt::rdata::d_14051f260;

// FUN_140484ab8: CRT abs (abs(INT_MIN) == INT_MIN)
std::int32_t crt_abs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}
// 32-bit NEG / SUB with wraparound
std::int32_t neg32(std::int32_t v) { return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)); }
std::int32_t sub32(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) - static_cast<std::uint32_t>(b));
}
std::int32_t add32(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}

// FUN_140484558 is the CRT fabs and FUN_140484644 the CRT sqrt (SQRTSD for x >= 0, error path
// returning NaN otherwise); both give the IEEE result, as std::fabs / std::sqrt do.
double crt_fabs(double x) { return std::fabs(x); }
double crt_sqrt(double x) { return std::sqrt(x); }

} // namespace

// @src 140134de0  speed axis -> time axis of a path table (see movement.hpp)
bool dynary_speed_axis_to_time(model::DYNARY* f)
{
    if (f->type != 0x79 && f->type != 0x7a && f->type != 0x7b) {
        return false;
    }
    if (f->var[0] != 0x1a) {
        return false;
    }
    if (f->n[0] == 0) {
        return false;
    }
    bool result;
    {
        model::DYNARY copy;          // 14012d050
        ext::dynary_copy(&copy, f);  // 14012eff0
        std::int32_t ncoord;
        if (f->ncomp == 4) {
            ncoord = 3;
        }
        else {
            ncoord = 2;
        }
        f->axis[0][0] = 0.0;
        for (std::int32_t i = 1; i < f->n[0]; i = i + 1) {
            const double v = copy.axis[0][i - 1];
            double s = 0.0;
            for (std::int32_t k = 0; k < ncoord; k = k + 1) {
                const double d = copy.values[i * f->ncomp + k] - copy.values[(i - 1) * f->ncomp + k];
                s = s + d * d;
            }
            if (s >= k0) {  // 140134fdc: comisd, JC skips (NaN too)
                s = crt_sqrt(s);
            }
            if (v > k0) {   // 140135006: comisd, JBE to the copy (NaN too)
                f->axis[0][i] = f->axis[0][i - 1] + s / v;
            }
            else {
                f->axis[0][i] = f->axis[0][i - 1];
            }
        }
        f->var[0] = 1;
        result = true;
    }                                // 14012f410
    return result;
}

// @src 14018c850  snap the 2D direction components to 0 / 1
void snap_direction_2d(void* mv)
{
    // 14018c874: comisd tol, |x|; JC skips, so the store happens for |x| <= tol (not for NaN)
    if (crt_fabs(fld<double>(mv, 0x50)) <= kSnapTol) {
        fld<double>(mv, 0x50) = 0.0;
    }
    if (crt_fabs(fld<double>(mv, 0x58)) <= kSnapTol) {
        fld<double>(mv, 0x58) = 0.0;
    }
    if (crt_fabs(k1 - fld<double>(mv, 0x50)) <= kSnapTol) {
        fld<double>(mv, 0x50) = k1;
    }
    if (crt_fabs(k1 - fld<double>(mv, 0x58)) <= kSnapTol) {
        fld<double>(mv, 0x58) = k1;
    }
}

namespace {
// part of @src 14018c940 / 14018ca10 / 14018cb10 (three copies of the same code at different
// offsets): x*x + y*y + z*z; if != 0 (ucomisd, NaN counts as != 0) divide each by its sqrt
void normalize3(void* mv, std::ptrdiff_t off)
{
    double& x = fld<double>(mv, off);
    double& y = fld<double>(mv, off + 8);
    double& z = fld<double>(mv, off + 16);
    double n = x * x + y * y + z * z;
    if (n != k0) {
        n = crt_sqrt(n);
        x = x / n;
        y = y / n;
        z = z / n;
    }
}
} // namespace

// @src 14018c940  normalise mv+0x50..0x60
void normalize_direction_3d(void* mv) { normalize3(mv, 0x50); }

// @src 14018ca10  normalise mv+0x648..0x658
void normalize_axis_648(void* mv) { normalize3(mv, 0x648); }

// @src 14018cb10  normalise mv+0x778..0x788
void normalize_axis_778(void* mv) { normalize3(mv, 0x778); }

// @src 1401d6030  sim-control words into the movement (2D record words 24..29)
void copy_sim_ctrl_to_movement(const model::DB_STEP_DATA* m, void* mv)
{
    fld<double>(mv, 0x1b8) = fld<double>(m, 0x8d0);
    fld<double>(mv, 0x1c0) = fld<double>(m, 0x8d8);
    fld<double>(mv, 0x1c8) = fld<double>(m, 0x8e8);
    fld<double>(mv, 0x1d0) = fld<double>(m, 0x8f0);
    fld<double>(mv, 0x1d8) = fld<double>(m, 0x900);
    fld<double>(mv, 0x1e0) = fld<double>(m, 0x908);
}

// @src 1401912a0  OMV_DATA::WriteToDB: the movement words of one LLBND record. The 2D branch
//                 (owner dimension 2) and the 3D branch (dimension 3) each write their layout.
//                 rec_type is not read; rec_no becomes the id of every function allocated.
//                 Most integer tests read fields of the DYNARY slots (layout/dynary.hpp) of mv:
//                 for a slot at X, X+0x04 is ndim (0: constant, value at X+0x08), X+0x10 var[0]
//                 (the x-variable code), X+0x1c n[0]; e.g. 0x74/0x78/0x80 for the slot at 0x70,
//                 0x458/0x464 for the type-6 curve at 0x448 (so rec word "0x464" is its n[0]).
void write_bnd_movement(void* mv, const std::int32_t* rec_type, double* rec, std::int32_t* iifnc,
                        double* llfnc, DB_STEP_DATA* m, const std::int32_t* rec_no)
{
    (void)rec_type;
    auto R = [rec](std::ptrdiff_t off) -> double& { return fld<double>(rec, off); };
    auto D = [mv](std::ptrdiff_t off) -> double& { return fld<double>(mv, off); };
    auto I = [mv](std::ptrdiff_t off) -> std::int32_t { return fld<std::int32_t>(mv, off); };
    auto fn = [mv](std::ptrdiff_t off) { return reinterpret_cast<model::DYNARY*>(at(mv, off)); };

    std::int32_t pos = 0;  // local_1e4
    std::int32_t len = 0;  // local_1e8
    // The repeated block: allocate a function of DYNARY mv+fn_off, its number into rec+word,
    // its values (140131360) at LLFNC position pos.
    auto curve = [&](std::ptrdiff_t fn_off, std::ptrdiff_t word) {
        pos = 0;
        len = ext::dynary_db_length(fn(fn_off));                    // 140130290
        alloc_function(m, &len, iifnc, rec_no, &R(word), &pos);     // 1401bb3d0
        write_dynary_to_db_array(fn(fn_off), llfnc + sub32(pos, 1)); // 140131360
    };

    void* owner = fld<void*>(mv, 0xa80);
    if (rt::dimension_is_2(owner)) {  // 1400015e0
        rt::to_fortran(reinterpret_cast<char*>(at(rec, 0x1c0)), static_cast<const char*>(mv), 0x40);
        snap_direction_2d(mv);
        R(0x50) = D(0x50);
        R(0x58) = D(0x58);
        switch (I(0x68)) {
        case 0:
            R(0x38) = 0.0;
            if (I(0x74) == 0) {
                R(0x60) = 0.0;
                R(0x48) = D(0x78);
            }
            else if (I(0x74) == 1) {
                R(0x60) = k1;
                if (I(0x80) == 1) {
                    R(0x68) = k2;
                }
                else if (I(0x80) == 2) {
                    R(0x68) = k1;
                }
                curve(0x70, 0x70);
            }
            break;
        case 1:
            R(0x38) = k1;
            if (I(0x160) == 0) {
                if (I(0xec) == 0) {
                    R(0x60) = 0.0;
                    R(0x48) = D(0xf0);
                }
                else if (I(0xec) == 1) {
                    R(0x60) = k1;
                    if (I(0xf8) == 1) {
                        R(0x68) = k2;
                    }
                    else if (I(0xf8) == 2) {
                        R(0x68) = k1;
                    }
                    else if (I(0xf8) == 5) {
                        R(0x68) = k4;
                    }
                    curve(0xe8, 0x70);
                }
            }
            else {
                R(0x60) = k2;
                R(0x70) = static_cast<double>(I(0x160));
                R(0x48) = D(0x168);
            }
            break;
        case 2:
            R(0x38) = k2;
            R(0x48) = D(0x1f0);
            R(0x78) = D(0x1e8);
            R(0x80) = D(0x1f8);
            R(0x88) = static_cast<double>(I(0x200));
            if (I(0x20c) == 0) {
                R(0x228) = 0.0;
            }
            else if (I(0x20c) == 1) {
                if (I(0x218) == 5) {
                    R(0x230) = k4;
                }
                curve(0x208, 0x228);
            }
            break;
        case 3:
            R(0x38) = k3;
            R(0x48) = D(0x280);
            R(0x78) = D(0x290);
            R(0x80) = D(0x288);
            R(0x88) = D(0x298);
            if (I(0x2a4) == 0) {
                R(0x228) = 0.0;
            }
            else if (I(0x2a4) == 1) {
                if (I(0x2b0) == 5) {
                    R(0x230) = k4;
                }
                curve(0x2a0, 0x228);
            }
            break;
        case 4:
            R(0x38) = k4;
            R(0x78) = D(0x318);
            R(0x88) = D(0x320);
            R(0x80) = D(0x328);
            if (fld<char>(mv, 0x330) == '\0') {
                R(0x60) = 0.0;
            }
            else {
                R(0x60) = k1;
                curve(0x338, 0x70);
                R(0x68) = k3;
            }
            break;
        case 5:
            R(0x38) = k5;
            if (I(0x3b4) == 0) {
                R(0x60) = 0.0;
                R(0x88) = D(0x3b8);
            }
            else if (I(0x3b4) == 1) {
                R(0x60) = k1;
                R(0x68) = k1;
                curve(0x3b0, 0x70);
            }
            R(0x190) = D(0x428);
            R(0x78) = D(0x430);
            R(0x180) = D(0x438);
            R(0x178) = static_cast<double>(I(0x440));
            R(0x188) = static_cast<double>(I(0x444));
            break;
        case 6:
            R(0x38) = k6;
            if (I(0x510) == 0) {
                R(0x60) = k1;
                R(0x78) = static_cast<double>(I(0x464));
                pos = 0;
                len = ext::movement_curve_length(mv, m);                // 14018cde0
                alloc_function(m, &len, iifnc, rec_no, &R(0x70), &pos);
                if (I(0x458) == 1) {
                    R(0x68) = k2;
                    write_dynary_to_db_array(fn(0x448), llfnc + sub32(pos, 1));
                }
                else {
                    R(0x68) = k5;
                    model::DYNARY t;                                    // 14012d050
                    ext::dynary_copy(&t, fn(0x448));                    // 14012eff0
                    dynary_speed_axis_to_time(&t);                      // 140134de0
                    write_table_1d_to_db_array(&t, llfnc + sub32(pos, 1)); // 1401317c0
                }                                                       // 14012f410
            }
            else {
                R(0x60) = km1;
                R(0x70) = static_cast<double>(crt_abs(I(0x514)));
            }
            R(0x80) = static_cast<double>(I(0x4c4));
            break;
        case 7:
            R(0x38) = k7;
            if (I(0x51c) == 0) {
                R(0x60) = 0.0;
                R(0x48) = D(0x520);
            }
            else if (I(0x51c) == 1) {
                R(0x60) = k1;
                if (I(0x528) == 1) {
                    R(0x68) = k2;
                }
                else if (I(0x528) == 2) {
                    R(0x68) = k1;
                }
                curve(0x518, 0x70);
            }
            R(0x78) = D(0x608);
            R(0x80) = D(0x610);
            break;
        case 8:
            R(0x38) = k8;
            if (I(0x594) == 0) {
                R(0x60) = 0.0;
                R(0x48) = D(0x598);
            }
            else if (I(0x594) == 1) {
                R(0x60) = k1;
                if (I(0x5a0) == 1) {
                    R(0x68) = k2;
                }
                else if (I(0x5a0) == 2) {
                    R(0x68) = k1;
                }
                curve(0x590, 0x70);
            }
            R(0x78) = D(0x608);
            break;
        case 9:
            R(0x38) = k9;
            R(0x78) = D(0x608);
            break;
        case 10:
            R(0x38) = k10;
            R(0x78) = D(0x608);
            R(0x80) = D(0x618);
            break;
        case 11:
            R(0x38) = k11;
            R(0xa0) = D(0x188);
            R(0xa8) = D(0x190);
            break;
        default:  // 14019137d: JA (unsigned > 11) skips the switch
            break;
        }
        if (I(0x620) != 0) {
            R(0x60) = km1;
            R(0x70) = static_cast<double>(crt_abs(I(0x624)));
        }
        R(0x90) = D(0x170);
        R(0x98) = D(0x178);
        if (I(0x68) == 0 && I(0x74) == 0) {
            R(0xb0) = D(0x78) * D(0x50);
            R(0xb8) = D(0x78) * D(0x58);
        }
        if (I(0x68) == 1 && I(0xec) == 0) {
            R(0xa0) = D(0xf0) * D(0x50);
            R(0xa8) = D(0xf0) * D(0x58);
        }
        copy_sim_ctrl_to_movement(m, mv);  // 1401d6030
        R(0xc0) = D(0x1b8);
        R(0xc8) = D(0x1c0);
        R(0xd0) = D(0x1c8);
        R(0xd8) = D(0x1d0);
        R(0xe0) = D(0x1d8);
        R(0xe8) = D(0x1e0);
        R(0xf0) = k1;
        R(0xf8) = 0.0;
        R(0x118) = 0.0;
        if (I(0x628) == 0) {
            R(0xf0) = 0.0;
            if (I(0x664) == 0) {
                R(0x118) = 0.0;
                R(0x100) = D(0x668);
            }
            else {
                if (I(0x670) == 1) {
                    R(0x118) = k1;
                }
                else if (I(0x670) == 10) {
                    R(0x118) = k2;
                }
                curve(0x660, 0x120);
            }
        }
        else if (I(0x628) == 1) {
            R(0xf0) = k1;
            if (I(0x6dc) == 0) {
                R(0x118) = 0.0;
                R(0xf8) = D(0x6e0);
            }
            else {
                if (I(0x6e8) == 1) {
                    R(0x118) = k1;
                }
                else if (I(0x6e8) == 10) {
                    R(0x118) = k2;
                }
                curve(0x6d8, 0x120);
            }
        }
        R(0x108) = D(0x630);
        R(0x110) = D(0x638);
        R(0x128) = D(0x750);
        if (fld<char>(mv, 0x8b8) != '\0') {
            curve(0x8c0, 0x40);
        }
        R(0x1b0) = D(0x938);
        R(0x1b8) = D(0x940);
        R(0x198) = D(0x948);
        R(0x1a0) = D(0x950);
        R(0x1a8) = D(0x958);
        const std::int32_t mode = fld<std::int32_t>(m, 0x850);
        if (mode == 3 ||
            (mode == 1 && I(0x960) == 1 && rt::dimension_is_2(fld<void*>(mv, 0xa80)))) {
            if (I(0x960) == -1) {
                R(0x130) = 0.0;
            }
            else {
                R(0x130) = k1;
                if (I(0x960) == 0) {
                    R(0x138) = 0.0;
                    if (I(0x96c) == 0) {
                        R(0x140) = 0.0;
                        R(0x148) = D(0x970);
                    }
                    else {
                        if (I(0x978) == 1) {
                            R(0x140) = k1;
                        }
                        else if (I(0x978) == 10) {
                            R(0x140) = k2;
                        }
                        curve(0x968, 0x148);
                    }
                }
                else if (I(0x960) == 1) {
                    R(0x138) = k1;
                    if (I(0x9e4) == 0) {
                        R(0x140) = 0.0;
                        R(0x148) = D(0x9e8);
                    }
                    else {
                        if (I(0x9f0) == 1) {
                            R(0x140) = k1;
                        }
                        else if (I(0x9f0) == 10) {
                            R(0x140) = k2;
                        }
                        curve(0x9e0, 0x148);
                    }
                }
                else if (I(0x960) == 2) {
                    R(0x138) = k2;
                    R(0x148) = D(0xa58);
                    R(0x150) = D(0xa60);
                    R(0x158) = D(0xa68);
                }
                R(0x170) = D(0xa70);
            }
        }
        R(0x208) = D(0x888);
        R(0x210) = D(0x890);
        R(0x218) = D(0x8a0);
        R(0x220) = D(0x8a8);
    }

    if (rt::dimension_is_3(fld<void*>(mv, 0xa80))) {  // 1400015f0
        rt::to_fortran(reinterpret_cast<char*>(at(rec, 0x290)), static_cast<const char*>(mv), 0x40);
        normalize_direction_3d(mv);
        R(0x60) = D(0x50);
        R(0x68) = D(0x58);
        R(0x70) = D(0x60);
        switch (I(0x68)) {
        case 0:
            R(0x78) = 0.0;
            if (I(0x74) == 0) {
                R(0x80) = 0.0;
                R(0x88) = D(0x78);
            }
            else if (I(0x74) == 1) {
                if (I(0x80) == 1) {
                    R(0x88) = k2;
                }
                else if (I(0x80) == 2) {
                    R(0x88) = k1;
                }
                curve(0x70, 0x80);
            }
            break;
        case 1:
            R(0x78) = k1;
            if (I(0x160) == 0) {
                if (I(0xec) == 0) {
                    R(0x80) = 0.0;
                    R(0x88) = D(0xf0);
                }
                else if (I(0xec) == 1) {
                    if (I(0xf8) == 1) {
                        R(0x88) = k2;
                    }
                    else if (I(0xf8) == 2) {
                        R(0x88) = k1;
                    }
                    else if (I(0xf8) == 5) {
                        R(0x88) = k4;
                    }
                    curve(0xe8, 0x80);
                }
            }
            else {
                R(0x80) = static_cast<double>(neg32(add32(I(0x160), 1000)));
                R(0x88) = D(0x168);
            }
            break;
        case 2:
            R(0x78) = k2;
            R(0x50) = D(0x1f0);
            R(0x148) = D(0x1f8);
            R(0x150) = D(0x1e8);
            R(0x158) = static_cast<double>(I(0x200));
            if (I(0x20c) == 0) {
                R(0x338) = 0.0;
            }
            else if (I(0x20c) == 1) {
                if (I(0x218) == 5) {
                    R(0x340) = k4;
                }
                curve(0x208, 0x338);
            }
            break;
        case 3:
            R(0x78) = k3;
            R(0x50) = D(0x280);
            R(0x148) = D(0x288);
            R(0x150) = D(0x290);
            R(0x158) = D(0x298);
            if (I(0x2a4) == 0) {
                R(0x338) = 0.0;
            }
            else if (I(0x2a4) == 1) {
                if (I(0x2b0) == 5) {
                    R(0x340) = k4;
                }
                curve(0x2a0, 0x338);
            }
            break;
        case 4:
            R(0x78) = k4;
            R(0x150) = D(0x318);
            R(0x158) = D(0x320);
            R(0x148) = D(0x328);
            if (fld<char>(mv, 0x330) == '\0') {
                R(0x80) = 0.0;
            }
            else {
                curve(0x338, 0x80);
                R(0x88) = k3;
            }
            break;
        case 5:
            R(0x78) = k5;
            if (I(0x3b4) == 0) {
                R(0x80) = 0.0;
                R(0x158) = D(0x3b8);
            }
            else if (I(0x3b4) == 1) {
                R(0x88) = k1;
                curve(0x3b0, 0x80);
            }
            R(0x260) = D(0x428);
            R(0x150) = D(0x430);
            R(0x1f8) = D(0x438);
            R(0x1f0) = static_cast<double>(I(0x440));
            R(0x200) = static_cast<double>(I(0x444));
            break;
        case 6:
            R(0x78) = k6;
            if (I(0x510) == 0) {
                R(0x148) = static_cast<double>(I(0x464));
                model::DYNARY t;                     // 14012d050
                ext::dynary_copy(&t, fn(0x448));     // 14012eff0
                if (I(0x458) == 1) {
                    R(0x88) = k2;
                }
                else {
                    R(0x88) = k5;
                    dynary_speed_axis_to_time(&t);   // 140134de0
                }
                if (I(0x4c0) == 2) {
                    double frame[10];                // local_c8 (80 bytes)
                    ext::movement_frame(mv, frame);  // 140194d40
                    ext::dynary_to_frame(&t, frame); // 140135bc0
                }
                pos = 0;
                len = ext::movement_curve_length(mv, m);                // 14018cde0
                alloc_function(m, &len, iifnc, rec_no, &R(0x80), &pos);
                double* p = llfnc + sub32(pos, 1);
                p[0] = k10;
                p[1] = D(0x4c8);
                p[2] = D(0x4d0);
                p[3] = D(0x4d8);
                p[4] = D(0x4e0);
                p[5] = D(0x4e8);
                p[6] = D(0x4f0);
                p[7] = D(0x4f8);
                p[8] = D(0x500);
                p[9] = D(0x508);
                write_table_1d_to_db_array(&t, p + 10);                 // 1401317c0
            }                                                           // 14012f410
            else {
                R(0x80) = static_cast<double>(neg32(crt_abs(I(0x514))));
            }
            R(0x150) = static_cast<double>(I(0x4c0));
            R(0x158) = static_cast<double>(I(0x4c4));
            break;
        case 7:
            R(0x78) = k7;
            if (I(0x51c) == 0) {
                R(0x80) = 0.0;
                R(0x88) = D(0x520);
            }
            else if (I(0x51c) == 1) {
                if (I(0x528) == 1) {
                    R(0x88) = k2;
                }
                else if (I(0x528) == 2) {
                    R(0x88) = k1;
                }
                curve(0x518, 0x80);
            }
            R(0x150) = D(0x608);
            R(0x158) = D(0x610);
            break;
        case 8:
            R(0x78) = k8;
            if (I(0x594) == 0) {
                R(0x80) = 0.0;
                R(0x88) = D(0x598);
            }
            else if (I(0x594) == 1) {
                if (I(0x5a0) == 1) {
                    R(0x88) = k2;
                }
                else if (I(0x5a0) == 2) {
                    R(0x88) = k1;
                }
                curve(0x590, 0x80);
            }
            R(0x150) = D(0x608);
            break;
        case 9:
            R(0x78) = k9;
            R(0x150) = D(0x608);
            break;
        case 10:
            R(0x78) = k10;
            R(0x150) = D(0x608);
            R(0x158) = D(0x618);
            break;
        case 11:
            R(0x78) = k11;
            R(0xa8) = D(0x188);
            R(0xb0) = D(0x190);
            R(0xb8) = D(0x198);
            break;
        default:  // 140192f5a: JA (unsigned > 11) skips the switch
            break;
        }
        if (I(0x620) != 0) {
            R(0x80) = static_cast<double>(neg32(crt_abs(I(0x624))));
        }
        R(0x90) = D(0x170);
        R(0x98) = D(0x178);
        R(0xa0) = D(0x180);
        if (I(0x68) == 0 && I(0x74) == 0) {
            R(0xc0) = D(0x78) * D(0x50);
            R(0xc8) = D(0x78) * D(0x58);
            R(0xd0) = D(0x78) * D(0x60);
        }
        if (I(0x68) == 1 && I(0xec) == 0) {
            R(0xa8) = D(0xf0) * D(0x50);
            R(0xb0) = D(0xf0) * D(0x58);
            R(0xb8) = D(0xf0) * D(0x60);
        }
        if (I(0x628) == 0) {
            R(0xd8) = 0.0;
            if (I(0x664) == 0) {
                R(0xe0) = 0.0;
                R(0x130) = D(0x668);
            }
            else {
                if (I(0x670) == 1) {
                    R(0xe0) = k1;
                }
                else if (I(0x670) == 2) {
                    R(0xe0) = k2;
                }
                else if (I(0x670) == 10) {
                    R(0xe0) = k3;
                }
                curve(0x660, 0xe8);
            }
        }
        else if (I(0x628) == 1) {
            R(0xd8) = k1;
            if (I(0x6dc) == 0) {
                R(0xe0) = 0.0;
                R(0x128) = D(0x6e0);
            }
            else {
                if (I(0x6e8) == 1) {
                    R(0xe0) = k1;
                }
                else if (I(0x6e8) == 2) {
                    R(0xe0) = k2;
                }
                else if (I(0x6e8) == 10) {
                    R(0xe0) = k3;
                }
                curve(0x6d8, 0xe8);
            }
        }
        R(0x108) = D(0x630);
        R(0x110) = D(0x638);
        R(0x118) = D(0x640);
        normalize_axis_648(mv);
        R(0xf0) = D(0x648);
        R(0xf8) = D(0x650);
        R(0x100) = D(0x658);
        R(0x120) = D(0x750);
        if (I(0x758) == 0) {
            R(0x190) = 0.0;
            if (I(0x794) == 0) {
                R(0x198) = 0.0;
                R(0x1e8) = D(0x798);
            }
            else {
                if (I(0x7a0) == 1) {
                    R(0x198) = k1;
                }
                else if (I(0x7a0) == 2) {
                    R(0x198) = k2;
                }
                else if (I(0x7a0) == 10) {
                    R(0x198) = k3;
                }
                curve(0x790, 0x1a0);
            }
        }
        else if (I(0x758) == 1) {
            R(0x190) = k1;
            if (I(0x80c) == 0) {
                R(0x198) = 0.0;
                R(0x1e0) = D(0x810);
            }
            else {
                if (I(0x818) == 1) {
                    R(0x198) = k1;
                }
                else if (I(0x818) == 2) {
                    R(0x198) = k2;
                }
                else if (I(0x818) == 10) {
                    R(0x198) = k3;
                }
                curve(0x808, 0x1a0);
            }
        }
        R(0x1c0) = D(0x760);
        R(0x1c8) = D(0x768);
        R(0x1d0) = D(0x770);
        normalize_axis_778(mv);
        R(0x1a8) = D(0x778);
        R(0x1b0) = D(0x780);
        R(0x1b8) = D(0x788);
        R(0x1d8) = D(0x880);
        if (fld<char>(mv, 0x8b8) != '\0') {
            curve(0x8c0, 0x258);
        }
        R(0x280) = D(0x938);
        R(0x288) = D(0x940);
        R(0x268) = D(0x948);
        R(0x270) = D(0x950);
        R(0x278) = D(0x958);
        R(0x2d8) = D(0x888);
        R(0x2e0) = D(0x890);
        R(0x2e8) = D(0x898);
        R(0x2f0) = D(0x8a0);
        R(0x2f8) = D(0x8a8);
        R(0x300) = D(0x8b0);
    }
}

} // namespace def::dbio::g3d
