// SRCNT4 FUN_14002f091 (GENCTC 3D interference), GENCTC_3d_srcnt4.md.
// Packet p67-3 (helper worker for p67-7 group "srcnt"). FP order from the listing
// deform-rea-work/out/p677/lst/L_14002f091.txt; every SAVE local lives in the static image.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <bit>
#include <cstdint>
#include <vector>

namespace def::cmd::ctc3d {

namespace sse = def::rt::sse;
namespace fio = def::rt::ftn::fio;
using i64 = std::int64_t;

namespace {

// DAT_140bad530: the REAL*4 literal 1e-9 widened (9.999999717180685e-10)
const double k_tiny = std::bit_cast<double>(std::uint64_t{0x3e112e0be0000000});

// Box centre of [min x,y,z, max x,y,z] (14002f3b2-14002f41e: z is 0.5*(a+b), x and y (a+b)*0.5)
void centre(const double* b, double* c)
{
    c[0] = sse::mul(sse::add(b[0], b[3]), 0.5);
    c[1] = sse::mul(sse::add(b[1], b[4]), 0.5);
    c[2] = sse::mul(0.5, sse::add(b[2], b[5]));
}

std::vector<i32> alloc_i32(i32 n) { return std::vector<i32>(static_cast<std::size_t>(n > 1 ? n : 1), 0); }

// part of @src 14002f091: the debug lines WRITE(6,*) 'In<k>- SRCNT4', poly, EI2(poly) [, start]
void debug_line(const char* tag, i32* dbg, i32* dbg_obj, i32* dbg_inst, i32* dbg_poly, bool with_start)
{
    if (!(*dbg == 1 && *dbg_obj == m<i32>(0x14064f3e4))) {
        return;
    }
    i32 f = 2;
    fun_140228260(dbg_obj, dbg_inst, dbg_poly, &f, mp<i32>(0x14064f544), mp<i32>(0x14064f548));
    fio::Write w(6);
    w.a(tag).i(*dbg_poly).i(m<i32>(0x14064f544));
    if (with_start) {
        w.i(m<i32>(0x14064f540));
    }
    w.end();
}

} // namespace

// @src 14002f091  SRCNT4: per partner P whose box meets O's, if P has points with contact codes
// to an object other than O: EI2 = -2000 on P's polygons away from the interference
// (140033d76), then re-validation (14003329f, 140034138)
void fun_14002f091(i32* obj, i32* inst, i32* bcc, i32* ndof, i32* nnode, i32* nface, double* x, double* tol,
                   i32* p9, i32* dbg, i32* dbg_obj, i32* dbg_inst, i32* dbg_poly)
{
    (void)nface;
    (void)p9;
    // 14002f0c0-14002f1a0: resets the allocatable descriptors DAT_14064f2e0 / DAT_14064f330 (the
    // port keeps those arrays in vectors).
    // OPEN(UNIT=61, FILE='SKIP_PNTR.DAT', STATUS='OLD', ERR=...): opened -> skip, unit stays open
    if (fio::open_old(61, "SKIP_PNTR.DAT")) {
        return;
    }
    if (m<i32>(0x1405c16fc) == 1) {
        return;
    }
    m<i32>(0x14064f380) = 0;

    double* box_o = mp<double>(0x14064f390);
    double* box_p = mp<double>(0x14064f400);
    double* pt = mp<double>(0x14064f460);
    double* box_x = mp<double>(0x14064f480);
    double* dir = mp<double>(0x14064f510);
    i32* idx2 = mp<i32>(0x1405c6790); // (1, 2)
    i32* idx1 = mp<i32>(0x1405c67a0); // (1, 2)

    i32 two = 2;
    def::rt::geoutl::fun_1404d1b1c(mp<double>(0x14075a9b8 + 8 * (static_cast<i64>(*obj) * 6 +
                                                                  static_cast<i64>(*inst) * 600)),
                                   box_o, tol, &two, idx2);
    const i32 npart = m<i32>(0x14077867c);
    centre(box_o, mp<double>(0x14064f3c0));
    i32& ip = m<i32>(0x14064f3e0);
    ip = 1;
    if (npart < 1) {
        return;
    }
    i32& kobj = m<i32>(0x14064f3e4);
    i32& kinst = m<i32>(0x14064f3e8);
    i32& npts = m<i32>(0x14064f3ec);
    i32& npoly = m<i32>(0x14064f3f0);
    i32& cnt = m<i32>(0x14064f4b0);
    i32& ipt = m<i32>(0x14064f4b4);
    double* c_o = mp<double>(0x14064f3c0);
    double* c_p = mp<double>(0x14064f430);
    double* c_x = mp<double>(0x14064f4f0);
    double& len = m<double>(0x14064f530);

    // part of @src 14002f091: one partner (return = label 14002f6e5)
    auto one_partner = [&]() {
        i32 one = 1;
        i32 mode = 1;
        fun_140030ae1(pt, box_x, tol, &one, idx1, &mode);
        cnt = 0;
        ipt = 1;
        if (npts >= 1) {
            const i32 mobj = static_cast<i32>(0u - static_cast<std::uint32_t>(*obj));
            do {
                i32 a = ipt;
                i32 f = 4;
                fun_140228830(&kobj, &kinst, &a, &f, mp<i32>(0x14064f4c0), mp<i32>(0x14064f4e0));
                i32& c = m<i32>(0x14064f4e4);
                c = 1;
                do {
                    const i32 v = m<i32>(0x14064f4bc + 4 * static_cast<i64>(c));
                    if (v < 0) {
                        cnt = cnt + 1;
                        if (v != mobj) {
                            a = ipt;
                            fun_140227fc0(&kobj, &kinst, &a, pt);
                            i32 n1 = 1;
                            i32 m0 = 0;
                            fun_140030ae1(pt, box_x, tol, &n1, idx1, &m0);
                        }
                    }
                    c = c + 1;
                } while (c <= 3);
                ipt = ipt + 1;
            } while (ipt <= npts);
        }
        if (cnt < 1) {
            return;
        }
        double half = sse::mul(*tol, 0.5);
        i32 n1 = 1;
        i32 mneg = -1;
        fun_140030ae1(pt, box_x, &half, &n1, idx1, &mneg);
        if (m<i32>(0x14075bc84) == 0) {
            fio::Write(6).a("Interference detected, KOBJ=").i(kobj).end();
        }
        c_x[0] = sse::mul(sse::add(box_x[0], box_x[3]), 0.5);
        c_x[1] = sse::mul(sse::add(box_x[1], box_x[4]), 0.5);
        dir[0] = sse::sub(c_p[0], c_x[0]);
        dir[1] = sse::sub(c_p[1], c_x[1]);
        c_x[2] = sse::mul(0.5, sse::add(box_x[2], box_x[5]));
        dir[2] = sse::sub(c_p[2], c_x[2]);
        len = sse::sqrt(sse::add(sse::mul(dir[2], dir[2]),
                                 sse::add(sse::mul(dir[1], dir[1]), sse::mul(dir[0], dir[0]))));
        // 14002f631 COMISD tiny,len; JP/JC -> use it
        if (!(len != len || k_tiny < len)) {
            dir[0] = sse::sub(c_p[0], c_o[0]);
            dir[1] = sse::sub(c_p[1], c_o[1]);
            dir[2] = sse::sub(c_p[2], c_o[2]);
            len = sse::sqrt(sse::add(sse::mul(dir[2], dir[2]),
                                     sse::add(sse::mul(dir[1], dir[1]), sse::mul(dir[0], dir[0]))));
            // 14002f6cc COMISD len,tiny; JP/JA -> use it, else next partner
            if (!(len != len || len > k_tiny)) {
                return;
            }
        }
        dir[2] = sse::div(dir[2], len);
        dir[1] = sse::div(dir[1], len);
        dir[0] = sse::div(dir[0], len);

        i32& nbox = m<i32>(0x14064f538);
        i32 z1 = 0;
        i32 z2 = 0;
        fun_140228c60(&kobj, &kinst, box_o, &nbox, &z1, &z2);
        m<i32>(0x14064f53c) = nbox;
        if (nbox < 1) {
            return;
        }
        std::vector<i32> list = alloc_i32(nbox); // ALLOCATE (descriptor DAT_14064f2e0)
        i32 z = 0;
        fun_14022a5c0(&kobj, &kinst, list.data(), reinterpret_cast<std::uint32_t*>(&nbox), &z);
        std::vector<i32> flag = alloc_i32(npoly); // ALLOCATE (descriptor DAT_14064f330), zeroed
        ipt = 1;
        while (ipt <= npoly) {
            flag[static_cast<std::size_t>(ipt - 1)] = 0;
            ipt = ipt + 1;
        }
        cnt = nbox;
        fun_140032ce4(&kobj, &kinst, list.data(), &cnt, box_o, tol);
        i32& start = m<i32>(0x14064f540);
        fun_140032a3e(&kobj, &kinst, list.data(), &cnt, &start, &npoly, flag.data());
        list.clear(); // DEALLOCATE
        if (start == 0) {
            return;
        }
        debug_line("In0- SRCNT4", dbg, dbg_obj, dbg_inst, dbg_poly, true);
        i32& value = m<i32>(0x14064f54c);
        value = -2000;
        fun_140033d76(&kobj, &kinst, &npoly, &start, dir, &value, flag.data());
        debug_line("In1- SRCNT4", dbg, dbg_obj, dbg_inst, dbg_poly, false);
        double& margin = m<double>(0x14064f550);
        margin = sse::mul(m<double>(0x140767500 + 8 * (static_cast<i64>(*obj) + static_cast<i64>(*inst) * 100)),
                          2.0); // DAT_140bad4d0
        i32& revalid = m<i32>(0x14064f558);
        fun_14003329f(&kobj, &kinst, obj, inst, x, &npoly, nnode, &margin, box_o, &revalid);
        if (revalid != 0) {
            fio::Write(6).a("Info: Checking Invisible definition").end();
            value = 1;
            fun_140034138(&kobj, &kinst, &npoly, obj, inst, nnode, dir, &value, flag.data(), bcc, ndof);
        }
        debug_line("In2- SRCNT4", dbg, dbg_obj, dbg_inst, dbg_poly, false);
    };

    do {
        kobj = m<i32>(0x140771918 + 4 * static_cast<i64>(ip));
        kinst = m<i32>(0x140778684 + 4 * static_cast<i64>(kobj));
        const i64 ix = static_cast<i64>(kobj) * 3 + static_cast<i64>(kinst) * 300;
        npoly = m<i32>(0x14076e588 + 4 * ix);
        npts = m<i32>(0x14076b6a8 + 4 * ix);
        two = 2;
        def::rt::geoutl::fun_1404d1b1c(mp<double>(0x14075a9b8 + 8 * (static_cast<i64>(kobj) * 6 +
                                                                      static_cast<i64>(kinst) * 600)),
                                       box_p, tol, &two, idx2);
        centre(box_p, c_p);
        def::rt::geoutl::fun_1404d1ed7(box_o, box_p, mp<i32>(0x14064f450));
        if (m<i32>(0x14064f450) != 0) {
            one_partner();
        }
        ip = ip + 1;
    } while (ip <= npart);
}

} // namespace def::cmd::ctc3d
