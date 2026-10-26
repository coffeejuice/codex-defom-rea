// Packet p67-7, group "pipeline": the Fortran pipeline FUN_1400512aa (GENCTC's entry from the
// C++ driver FUN_1401a6c80), the partner list setters the driver calls before it, and the short
// stages 4 (FUN_14002cc15) and 6 (FUN_140088e99). GENCTC_3d_search.md sec. 2.2, 6.3, 7.7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>
#include <defcmd/ctc3d/srcnt.hpp>
#include <defcmd/ctc3d/surface.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }

} // namespace

// @src 1400511e4  partner list: DAT_14077867c = *n, DAT_140771918[1..n] = partners(1..n)
void fun_1400511e4(std::int32_t* /*one*/, const std::int32_t* n, const std::int32_t* partners)
{
    dat_14077867c = *n;
    I(0x140676700) = 1;
    if (*n >= 1) {
        do {
            const i32 i = I(0x140676700);
            I(0x140676700) = i + 1;
            dat_140771918[i] = partners[i - 1];
        } while (I(0x140676700) <= *n);
    }
}

// @src 140082ed6  DAT_140778684[*obj] = *one (cube instance of the object)
void fun_140082ed6(const std::int32_t* obj, const std::int32_t* one)
{
    m<i32>(0x140778684 + 4 * static_cast<std::int64_t>(*obj)) = *one;
}

// @src 140088e99  stage 6: NR1[4] = 0 for the nodes of partner polygons with EI2 != 1
void fun_140088e99()
{
    const i32 np = dat_14077867c; // 140088eac: read once
    I(0x1406db010) = 1;
    if (np < 1) {
        return;
    }
    do {
        const i32 k = dat_140771918[I(0x1406db010)];
        const i32 inst = dat_140778684[k];
        I(0x1406db014) = k;
        I(0x1406db018) = inst;
        I(0x1406db01c) = dat_14076e588[k * 3 + inst * 300];
        I(0x1406db020) = 1;
        if (I(0x1406db01c) >= 1) {
            do {
                i32 p = I(0x1406db020);
                i32 f2 = 2;
                fun_140228260(P(0x1406db014), P(0x1406db018), &p, &f2, P(0x1406db024), P(0x1406db028));
                if (I(0x1406db024) != 1) {
                    i32 p2 = I(0x1406db020);
                    fun_140227e00(P(0x1406db014), P(0x1406db018), &p2, P(0x1406db030), P(0x1406db040));
                    I(0x1406db044) = 1;
                    if (I(0x1406db040) >= 1) {
                        do {
                            const i32 c = I(0x1406db030 + 4 * (static_cast<std::int64_t>(I(0x1406db044)) - 1));
                            I(0x1406db048) = c < 0 ? -c : c;
                            i32 f1 = 1;
                            fun_140228b10(P(0x1406db014), P(0x1406db018), P(0x1406db048), &f1,
                                          mp<double>(0x1406db050), P(0x1406db028));
                            m<double>(0x1406db068) = 0.0;
                            i32 f1b = 1;
                            i32 four = 4;
                            fun_140228980(P(0x1406db014), P(0x1406db018), P(0x1406db048), &f1b,
                                          mp<double>(0x1406db050), &four);
                            I(0x1406db044) = I(0x1406db044) + 1;
                        } while (I(0x1406db044) <= I(0x1406db040));
                    }
                }
                I(0x1406db020) = I(0x1406db020) + 1;
            } while (I(0x1406db020) <= I(0x1406db01c));
        }
        I(0x1406db010) = I(0x1406db010) + 1;
    } while (I(0x1406db010) <= np);
}

// @src 14008a8c1  empty routine (RET)
void fun_14008a8c1() {}
// @src 14008a8c3  empty routine (RET)
void fun_14008a8c3() {}
// @src 14008a8c4  empty routine (RET)
void fun_14008a8c4() {}

// @src 1400512aa  GENCTC 3D pipeline for slave *obj: stages 1-7, then the per-node decision for
// node = 1..*nnode (GENCTC_3d_search.md sec. 2.2)
void fun_1400512aa(i32* obj, i32* inst, double* coords, i32* bcc, i32* ndof, i32* one2, i32* nnode,
                   i32* nface, i32* one3, i32* zero, i32* ierr, i32* zero2, double* tol)
{
    (void)one2;
    fun_14005c3a7(obj, inst, nface, bcc, ndof);
    fun_140052f49(obj, inst, nnode, bcc, ndof);
    fun_140029d47(obj, inst, nnode, nface);
    fun_14002cc15(obj, inst, nface, bcc, ndof);
    i32 local = 0;
    I(0x140676704) = 0;
    // 1400513db: SRCNT(coords, obj, inst, N, nface, bcc, ndof, &DAT_140676704, tol, arg 11, &0)
    // The argument list agreed with group "srcnt" (arg 9 = double* tol, args 10/11 = i32*);
    // called through that type so that the call compiles before and after srcnt.hpp is
    // corrected (the provisional declaration types arg 9 as i32* and arg 11 as double*).
    using srcnt_fn = void (*)(double*, i32*, i32*, i32*, i32*, i32*, i32*, i32*, double*, i32*, i32*);
    const srcnt_fn srcnt = reinterpret_cast<srcnt_fn>(&fun_14003b304);
    srcnt(coords, obj, inst, nnode, nface, bcc, ndof, P(0x140676704), tol, ierr, &local);
    fun_140088e99();
    local = 0;
    fun_14003fadc(obj, inst, nnode, nface, &local);
    I(0x140676708) = 1;
    if (*nnode >= 1) {
        do {
            I(0x14067670c) = I(0x140676708);
            fun_14005190e(obj, inst, coords, bcc, ndof, P(0x14067670c), one3, zero, ierr, zero2, tol);
            I(0x140676708) = I(0x140676708) + 1;
        } while (I(0x140676708) <= *nnode);
    }
}

} // namespace def::cmd::ctc3d
