// SRCNT stage of the 3D contact engine: FUN_14003b304 and its set-up helpers 14003b130,
// 14003b0c6 (GENCTC_3d_search.md sec. 7.1, 7.2, 11.3). Packet p67-7, group "srcnt".
#include "srcnt_internal.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/srcnt.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::cmd::ctc3d {

using namespace srcnt_detail;
namespace fio = def::rt::ftn::fio;
namespace sse = def::rt::sse;

namespace {

// partner list entry i (DAT_140771918, 1-based) and instance of object k (DAT_140778684)
i32 partner(i32 i) { return I(0x140771918ull + 4 * static_cast<std::int64_t>(i)); }
i32 inst_of(i32 k) { return I(0x140778684ull + 4 * static_cast<std::int64_t>(k)); }
// point / polygon count of the cube (obj, inst): index obj*3 + inst*300
i32 cube_npts(i32 obj, i32 inst) { return I(0x14076b6a8ull + 4 * (static_cast<std::int64_t>(inst) * 300 + static_cast<std::int64_t>(obj) * 3)); }
i32 cube_npoly(i32 obj, i32 inst) { return I(0x14076e588ull + 4 * (static_cast<std::int64_t>(inst) * 300 + static_cast<std::int64_t>(obj) * 3)); }
// bounding box of the cube (obj, inst), 6 doubles at DAT_14075a9b8 + (obj*6 + inst*600)*8
double* cube_box(i32 obj, i32 inst) { return DP(0x14075a9b8ull + 8 * (static_cast<std::int64_t>(obj) * 6 + static_cast<std::int64_t>(inst) * 600)); }

} // namespace

// @src 14003b0c6  diagonal of a box: d = max - min per axis, |d| = sqrt((dx*dx + dy*dy) + dz*dz)
void fun_14003b0c6(const double* box, double* diag)
{
    const double dx = sse::sub(box[3], box[0]);
    D(0x140655a30) = dx;
    const double dy = sse::sub(box[4], box[1]);
    D(0x140655a38) = dy;
    const double dz = sse::sub(box[5], box[2]);
    // 14003b101..14003b119: x2 = dx*dx; y2 = dy*dy; y2 += x2; z2 = dz*dz; z2 += y2
    const double s = sse::add(sse::mul(dz, dz), sse::add(sse::mul(dy, dy), sse::mul(dx, dx)));
    D(0x140655a40) = dz;
    *diag = sse::sqrt(s);
}

// @src 14003b130  *flag = 1 if a polygon of a partner (not obj, not 0) has a negative last node
void fun_14003b130(i32* obj, i32* flag)
{
    const i32 np = dat_14077867c;
    I(0x140655a48) = 1;
    *flag = 0;
    if (np < 1) {
        return;
    }
    do {
        I(0x140655a4c) = partner(I(0x140655a48));
        I(0x140655a50) = inst_of(I(0x140655a4c));
        if (*obj != I(0x140655a4c) && I(0x140655a4c) != 0) {
            I(0x140655a54) = cube_npts(I(0x140655a4c), I(0x140655a50));
            I(0x140655a58) = cube_npoly(I(0x140655a4c), I(0x140655a50));
            I(0x140655a5c) = 1;
            if (I(0x140655a58) > 0) {
                do {
                    i32 p = I(0x140655a5c);
                    fun_140227e00(IP(0x140655a4c), IP(0x140655a50), &p, IP(0x140655a60), IP(0x140655a80));
                    // node list DAT_140655a60(1..4), base DAT_140655a5c: entry count = last node
                    if (I(0x140655a5c + 4 * static_cast<std::int64_t>(I(0x140655a80))) < 0) {
                        *flag = 1;
                        return;
                    }
                    I(0x140655a5c) += 1;
                } while (I(0x140655a5c) <= I(0x140655a58));
            }
        }
        I(0x140655a48) += 1;
    } while (I(0x140655a48) <= np);
}

// @src 14003b304  SRCNT: tolerance, partner preparation, symmetry planes, partner-partner contact,
// SRCNT2/4/5, symmetry die, DIEGEO.DAT (GENCTC_3d_search.md sec. 7)
void fun_14003b304(double* x, i32* obj, i32* inst, i32* n, i32* nface, i32* bc, i32* ndof, i32* p8, double* tol, i32* p10, i32* p11)
{
    if (*p8 == 1) {
        fun_14002cc15(obj, inst, nface, bc, ndof);
        return;
    }
    I(0x140655a84) = 0; // debug flag (handed to SRCNT4, never set)
    I(0x140655a88) = 0; // debug polygon
    I(0x140655a8c) = 0; // debug object
    I(0x140655a90) = 1; // debug instance
    I(0x140655a94) = 0;
    fun_14003b130(obj, IP(0x140655a94));

    // sec. 7.1: m = min NR2 over surface nodes (strict 0 < NR2 < m), start 1e7 (DAT_140bad0f0)
    I(0x140655aa0) = 1;
    D(0x140655a98) = 10000000.0;
    if (*n >= 1) {
        do {
            i32 pt = I(0x140655aa0);
            i32 field = 3;
            fun_140228830(obj, inst, &pt, &field, IP(0x140655aa4), IP(0x140655aa8));
            if (I(0x140655aa4) != 0) {
                pt = I(0x140655aa0);
                field = 2;
                fun_140228b10(obj, inst, &pt, &field, DP(0x140655ab0), IP(0x140655ab8));
                // 14003bd8a: COMISD v,0 (JP/JBE skip); 14003bda8: COMISD m,v (JP/JBE skip)
                if (D(0x140655ab0) > 0.0 && D(0x140655a98) > D(0x140655ab0)) {
                    D(0x140655a98) = D(0x140655ab0);
                }
            }
            I(0x140655aa0) += 1;
        } while (I(0x140655aa0) <= *n);
    }
    const double r = sse::sqrt(D(0x140655a98));
    const double t = *tol;
    D(0x140655ac8) = t;
    D(0x140655ac0) = r;
    D(0x140767500ull + 8 * (static_cast<std::int64_t>(*obj) + static_cast<std::int64_t>(*inst) * 100)) = r;
    // 14003b432: COMISD 1e-10, tol: JP/JC skip
    if (t <= 1.000000013351432e-10) { // DAT_140bad130
        D(0x140655ac8) = sse::mul(r, 0.05000000074505806); // DAT_140bad758
        // 14003b47f: COMISD 0, tol': JP/JC skip
        if (D(0x140655ac8) <= 0.0) {
            D(0x140655ac8) = 1.000000013351432e-10;
        }
    }
    fun_140232090(obj, inst, DP(0x140655ad0), p10);
    fun_14003b0c6(cube_box(*obj, *inst), DP(0x140655af0));
    D(0x140655af8) = sse::mul(D(0x140655af0), 2.0); // DAT_140bad4d0
    fun_14002a857(obj, inst, x, nface, bc, ndof, DP(0x140655b00), DP(0x140656460), DP(0x140656dc0), IP(0x1406570e0),
                  DP(0x140655af8), n);

    // sec. 7.2 step 4: partner preparation, symmetry polygons, partner-partner contact
    {
        const i32 np = dat_14077867c;
        I(0x1406570e4) = 1;
        if (np > 0) {
            do {
                I(0x1406570e8) = partner(I(0x1406570e4));
                I(0x1406570ec) = inst_of(I(0x1406570e8));
                if (*obj != I(0x1406570e8) && I(0x1406570e8) != 0) {
                    I(0x1406570f0) = cube_npts(I(0x1406570e8), I(0x1406570ec));
                    I(0x1406570f4) = cube_npoly(I(0x1406570e8), I(0x1406570ec));
                    if (!(I(0x1406570f0) < 2 || I(0x1406570f4) < 1)) {
                        I(0x1406570f8) = 1;
                        if (I(0x1406570f0) > 0) {
                            do {
                                i32 pt = I(0x1406570f8);
                                i32 field = 4;
                                i32 cnt = 6;
                                fun_140228690(IP(0x1406570e8), IP(0x1406570ec), &pt, &field, IP(0x1405c6880), &cnt);
                                I(0x1406570f8) += 1;
                            } while (I(0x1406570f8) <= I(0x1406570f0));
                        }
                        fun_140029d47(IP(0x1406570e8), IP(0x1406570ec), IP(0x1406570f0), IP(0x1406570f4));
                        if (I(0x14076abb8ull + 4 * static_cast<std::int64_t>(I(0x1406570e8))) == 0) {
                            fun_14003d2ee(IP(0x1406570e8), IP(0x1406570ec), IP(0x1406570f4), DP(0x140655b00), DP(0x140656460),
                                          DP(0x140656dc0), IP(0x1406570e0), DP(0x140655af8));
                        }
                        const i32 nq = dat_14077867c;
                        I(0x1406570fc) = 1;
                        if (nq > 0) {
                            do {
                                I(0x140657100) = partner(I(0x1406570fc));
                                I(0x140657104) = inst_of(I(0x140657100));
                                if (*obj != I(0x140657100) && I(0x1406570e8) != I(0x140657100) && I(0x140657100) != 0) {
                                    I(0x140657108) = cube_npts(I(0x140657100), I(0x140657104));
                                    I(0x14065710c) = cube_npoly(I(0x140657100), I(0x140657104));
                                    if (!(I(0x140657108) < 2 || I(0x14065710c) < 1)) {
                                        fun_14003e7f6(IP(0x1406570e8), IP(0x140657100), IP(0x1406570ec), IP(0x140657104),
                                                      IP(0x1406570f0), IP(0x14065710c), DP(0x140655ac8), p11);
                                    }
                                }
                                I(0x1406570fc) += 1;
                            } while (I(0x1406570fc) <= nq);
                        }
                    }
                }
                I(0x1406570e4) += 1;
            } while (I(0x1406570e4) <= np);
        }
    }

    // step 5: SRCNT2 on every partner with polygons
    {
        const i32 np = dat_14077867c;
        I(0x1406570e4) = 1;
        if (np > 0) {
            const i32 oi = *inst;
            do {
                I(0x1406570e8) = partner(I(0x1406570e4));
                I(0x1406570ec) = inst_of(I(0x1406570e8));
                I(0x140657110) = cube_npts(I(0x1406570e8), I(0x1406570ec));
                I(0x1406570f4) = cube_npoly(I(0x1406570e8), I(0x1406570ec));
                if (*obj != I(0x1406570e8) && 0 < I(0x1406570e8) && 0 < I(0x1406570f4)) {
                    fun_14002cdee(IP(0x1406570e8), IP(0x1406570ec), IP(0x1406570f4), DP(0x140655ac8), cube_box(*obj, oi), obj, p11);
                }
                I(0x1406570e4) += 1;
            } while (I(0x1406570e4) <= np);
        }
    }

    if (I(0x140655a94) == 1) {
        return;
    }
    // step 6: SRCNT4, SRCNT5
    fun_14002f091(obj, inst, bc, ndof, n, nface, x, DP(0x140655ac8), p11, IP(0x140655a84), IP(0x140655a8c),
                  IP(0x140655a90), IP(0x140655a88));
    if (I(0x140655a84) == 1) {
        fio::Write(6).a("After SRCNT4").end();
        i32 unit = 6;
        fun_14003c742(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88), &unit);
    }
    fun_140030d0e(obj, inst, nface, DP(0x140655ac8), bc, ndof, p11);
    if (I(0x140655a84) == 1) {
        fio::Write(6).a("After SRCNT5").end();
        i32 unit = 6;
        fun_14003c742(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88), &unit);
    }
    // step 7: symmetry die (DAT_14076abb8 > 0 never happens in DEF_PRE)
    {
        const i32 np = dat_14077867c;
        if (0 < I(0x14076abb8)) {
            I(0x1406570e4) = 1;
            if (0 < np) {
                const i32 oi = *inst;
                const i32 oo = *obj;
                do {
                    I(0x1406570e8) = partner(I(0x1406570e4));
                    const std::int64_t k = I(0x1406570e8);
                    I(0x1406570ec) = inst_of(I(0x1406570e8));
                    I(0x140657110) = cube_npts(I(0x1406570e8), I(0x1406570ec));
                    I(0x1406570f4) = cube_npoly(I(0x1406570e8), I(0x1406570ec));
                    fun_14003dabe(IP(0x1406570e8), IP(0x1406570ec), IP(0x1406570f4), DP(0x140769768), DP(0x14076a0c8), IP(0x14076abb8),
                                  IP(0x14076aa28), IP(0x140772728ull + 4 * (k + static_cast<std::int64_t>(I(0x1406570ec)) * 100)),
                                  DP(0x140655ac8), cube_box(oo, oi));
                    fun_14003c506(IP(0x1406570e8), IP(0x1406570ec), IP(0x140657110));
                    fun_14005d44f(IP(0x1406570e8), IP(0x1406570ec), IP(0x1406570f4));
                    I(0x1406570e4) += 1;
                } while (I(0x1406570e4) <= np);
            }
        }
    }
    if (I(0x140655a84) == 1) {
        i32 field = 2;
        fun_140228260(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88), &field, IP(0x140657114), IP(0x140655aa8));
        fio::Write(6).a("After SYMDIE4").i(I(0x140655a88)).i(I(0x140657114)).end();
        fun_14003cd08(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88));
        i32 unit = 6;
        fun_14003c742(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88), &unit);
        fio::stop(" ", 1); // STOP (blank code, str 140bad790)
    } else {
        // sec. 11.3: OPEN(23, FILE='DIEGEO.DAT', STATUS='OLD', ERR=...); CLOSE(23)
        I(0x140657118) = 0;
        if (fio::open_old(23, "DIEGEO.DAT")) {
            I(0x140657118) = 1;
        }
        fio::close(23);
        if (I(0x140657118) != 0) {
            fun_14003cd08(IP(0x140655a8c), IP(0x140655a90), IP(0x140655a88));
        }
    }
}

} // namespace def::cmd::ctc3d
