// GENCTC 3D: contact polygon switch for nodes with two contact codes (packet p67-7, group
// "decide"): FUN_14004522c, GENCTC_3d_decide.md sec. 4.5. Locals are SAVE statics in the image.
#include "decide_util.hpp"

#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl.hpp>

namespace def::cmd::ctc3d {

using namespace decide_detail;
namespace fio = def::rt::ftn::fio;
namespace geo = def::rt::geoutl;

namespace {

// part of @src 14004522c: nearest valid edge of polygon (o, inst, poly) to x. conn at cbase
// (count *cnt), EI6 at ebase; nvv from the wrap test on conn(wrapIdx); results in kOut / dOut.
void nearest_edge(i32* o, i32* inst, i32* poly, u64 cbase, i32* cnt, u64 ebase, i32 wrapIdx, double* x,
                  u64 tout, i32& kOut, double& dOut)
{
    i32& nvv = I4(0x14066e4e0);
    i32& l = I4(0x14066e514);
    i32& ln = I4(0x14066e550);
    auto connk = [cbase](i32 j) -> i32& { return I4(cbase - 4 + 4ull * static_cast<u64>(static_cast<i64>(j))); };
    i32 f = 6;
    fun_140228260(o, inst, poly, &f, PI(ebase), PI(ebase + 0x20));
    kOut = 0;
    nvv = *cnt;
    if (iabs(connk(wrapIdx)) == iabs(connk(1))) {
        nvv = *cnt - 1;
    }
    dOut = 10000.0; // DAT_140bad5a0
    f = 1;
    fun_140228260(o, inst, poly, &f, PI(0x14066e4f0), PI(0x14066e510));
    for (l = 1; l <= nvv; ++l) {
        ln = l + 1;
        if (nvv < ln) {
            ln = 1;
        }
        I4(0x14066e554) = I4(ebase - 4 + 4ull * static_cast<u64>(l)); // EI6(l)
        if (I4(0x14066e554) <= 0) {
            continue;
        }
        I4(0x14066e558) = I4(0x14066e4ecull + 4ull * static_cast<u64>(l)); // EI1(l)
        f = 2;
        fun_140228260(o, inst, PI(0x14066e558), &f, PI(0x14066e55c), PI(0x14066e560));
        if (I4(0x14066e55c) != 1) {
            continue;
        }
        I4(0x14066e564) = iabs(connk(l));
        I4(0x14066e568) = iabs(connk(ln));
        fun_140227fc0(o, inst, PI(0x14066e564), PD(0x14066e520));
        fun_140227fc0(o, inst, PI(0x14066e568), PD(0x14066e538));
        geo::fun_1404d6b13(PD(0x14066e520), PD(0x14066e538), x, PD(0x14066e570), PD(tout), PD(0x14066e598));
        if (R8(0x14066e598) < dOut) {
            kOut = l;
            dOut = R8(0x14066e598);
        }
    }
}

} // namespace

// @src 14004522c  contact polygon switch of a node with exactly two contact codes
void fun_14004522c(i32* obj, i32* inst, double* x, i32* bcrow, i32* node, i32* changed, double* tol)
{
    i32& nneg = I4(0x14066e368);
    i32& r = I4(0x14066e36c);
    auto code = [](i32 rr) -> i32& { return I4(0x14066e36cull + 4ull * static_cast<u64>(rr)); };
    auto ipoly = [](i32 rr) -> i32& { return I4(0x14066e378ull + 4ull * static_cast<u64>(rr)); };
    auto Mcol = [](i32 rr) { return PD(0x14066e388ull + 24ull * static_cast<u64>(rr)); };

    nneg = 0;
    *changed = 0;
    for (r = 1; r <= 3; ++r) {
        if (bcrow[r - 1] < 0) {
            ++nneg;
        }
    }
    if (nneg < 2 || nneg == 3) {
        return;
    }
    i32 f = 4;
    fun_140228830(obj, inst, node, &f, PI(0x14066e370), PI(0x14066e390));
    f = 4;
    fun_140228b10(obj, inst, node, &f, PD(0x14066e3a0), PI(0x14066e3f0));
    i32& j = I4(0x14066e3f4);
    j = 0;
    for (r = 1; r <= 3; ++r) {
        if (code(r) >= 0) {
            continue;
        }
        const i32 pr = ipoly(r);
        const i32 ob = ineg(code(r));
        ++j;
        I4(0x14066e3f8) = ob;
        I4(0x14066e3fc) = pr;
        const double* mc = Mcol(r);
        double* nj = PD(0x14066e408ull + 24ull * static_cast<u64>(j));
        nj[0] = mc[0];
        I4(0x14066e3f8ull + 8ull * static_cast<u64>(j)) = ob;
        nj[1] = mc[1];
        I4(0x14066e3fcull + 8ull * static_cast<u64>(j)) = pr;
        nj[2] = mc[2];
    }
    double& dot = R8(0x14066e470);
    dot = dot_l(PD(0x14066e420), PD(0x14066e438));
    if (fabs_b(dot) < 0.3499999940395355) { // DAT_140bad930
        return;
    }
    i32& o1 = I4(0x14066e3f8);
    i32& o2 = I4(0x14066e478);
    o1 = I4(0x14066e400);
    o2 = I4(0x14066e408);
    if (o1 == o2) {
        return;
    }
    // object 1: nearest valid edge of its polygon
    i32& e1 = I4(0x14066e47c);
    i32& p1 = I4(0x14066e480);
    e1 = inst_of(o1);
    p1 = I4(0x14066e404);
    fun_140227e00(&o1, &e1, &p1, PI(0x14066e490), PI(0x14066e4a0));
    const i32 nv1 = I4(0x14066e4a0); // local_70 = nv1 * 4, also used for polygon 2 below
    i32& k1 = I4(0x14066e4d4);
    double& d1 = R8(0x14066e4d8);
    nearest_edge(&o1, &e1, &p1, 0x14066e490, PI(0x14066e4a0), 0x14066e4b0, nv1, x, 0x14066e590, k1, d1);
    // object 2
    i32& p2 = I4(0x14066e5a0);
    i32& e2 = I4(0x14066e5a4);
    p2 = I4(0x14066e40c);
    e2 = inst_of(I4(0x14066e408));
    fun_140227e00(&o2, &e2, &p2, PI(0x14066e5b0), PI(0x14066e5c0));
    i32& k2 = I4(0x14066e5f4);
    double& d2 = R8(0x14066e5f8);
    nearest_edge(&o2, &e2, &p2, 0x14066e5b0, PI(0x14066e5c0), 0x14066e5d0, nv1, x, 0x14066e600, k2, d2);
    if (k1 == 0 && k2 == 0) {
        return;
    }
    i32& osel = I4(0x14066e608);
    osel = 0;
    if (d1 > *tol && d2 > *tol) {
        return;
    }
    if (d1 > d2) {
        k1 = 0;
    } else if (d2 > d1) {
        k2 = 0;
    }
    i32& psel = I4(0x14066e3fc);
    i32& ksel = I4(0x14066e60c);
    if (k1 != 0) {
        osel = o1;
        psel = p1;
        ksel = k1;
    }
    if (k2 != 0) {
        osel = o2;
        psel = p2;
        ksel = k2;
    }
    i32& esel = I4(0x14066e610);
    esel = inst_of(osel);
    f = 1;
    fun_140228260(&osel, &esel, &psel, &f, PI(0x14066e4f0), PI(0x14066e510));
    i32& nb = I4(0x14066e614);
    nb = I4(0x14066e4ecull + 4ull * static_cast<u64>(ksel));
    f = 1;
    fun_140228540(&osel, &esel, &psel, &f, PD(0x14066e620), PI(0x14066e640));
    f = 1;
    double& val = R8(0x14066e648);
    val = 1.0;
    fun_140228540(&osel, &esel, &nb, &f, PD(0x14066e650), PI(0x14066e640));
    dot = dot_l(PD(0x14066e620), PD(0x14066e650));
    if (fabs_b(dot) < 1.0) { // DAT_140bad938
        p2 = nb;
        val = dot;
    }
    if (!(fabs_b(val) < 0.3499999940395355)) {
        return;
    }
    for (r = 1; r <= 3; ++r) {
        if (code(r) != ineg(osel)) {
            continue;
        }
        esel = inst_of(osel);
        f = 1;
        ipoly(r) = p2;
        fun_140228540(&osel, &esel, &p2, &f, Mcol(r), PI(0x14066e670));
        f = 4;
        fun_140228690(obj, inst, node, &f, PI(0x14066e370), PI(0x14066e390));
        f = 4;
        fun_140228980(obj, inst, node, &f, PD(0x14066e3a0), PI(0x14066e3f0));
        fio::Write(msg_unit()).a("Contact Poly for Object").i(osel).a("has been changed for Node:").i(*node).end();
        *changed = 1;
        return;
    }
}

} // namespace def::cmd::ctc3d
