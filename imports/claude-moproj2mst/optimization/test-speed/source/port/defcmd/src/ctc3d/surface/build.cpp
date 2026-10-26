// Master surface build FUN_14004f56b (GENCTC_3d_search.md sec. 5) and its counts/box helper.
// Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>
#include <defrt/ftn/geoutl_pending.hpp>

#include <cstdint>
#include <cstdlib>
#include <vector>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;

namespace {

std::uint64_t u64(const void* p) { return reinterpret_cast<std::uint64_t>(p); }

} // namespace

// @src 14005110a  counts, nodes per polygon and bounding box (widened by 1e-10) of (obj, inst)
void fun_14005110a(i32* obj, i32* inst, double* x, i32* /*faces*/, i32* npts, i32* npoly,
                   i32* nvert)
{
    const std::int64_t o = *obj;
    const std::int64_t s = *inst;
    const std::int64_t k = s * 300 + o * 3;
    I(0x14076e588 + 4 * k) = *npoly;
    I(0x14076b6a8 + 4 * k) = *npts;
    D(0x1406766f8) = 1.0e-10; // 140bad4a0
    I(0x140773534 + 4 * (o * 2 + s * 200)) = *nvert;
    // 1400511d0: (x, box, &tol, npts), arguments read in the listing
    def::rt::geoutl::fun_1404d1d05(x, DP(0x14075a9b8 + 8 * (o * 6 + s * 600)), DP(0x1406766f8),
                                   npts);
}

// @src 14004f56b  master surface of one object instance (cube, ER1-3, EI1-6, NI1-3, NR1/2/5)
void fun_14004f56b(i32* obj, i32* inst, double* x, i32* faces, i32* nvert, i32* base, i32* npts,
                   i32* npoly, i32* zero, i32* flag)
{
    // ALLOCATABLE work arrays (dope vectors 140676420 / 470 / 4c0 / 510): heap in the original.
    // 420: edge tables (36 * npoly INTEGERs), 470: E(nv, npoly), 4c0: flood stack, 510: Nb(2nv,
    // npoly).
    std::vector<i32> table;
    if (*npts == 0 || *npoly == 0) {
        return;
    }
    const i32 np = *npts;
    const i32 nf = *npoly;
    *flag = 0;
    const std::int64_t o = *obj;
    const std::int64_t k = o * 3 + static_cast<std::int64_t>(*inst) * 300;
    I(0x140778684 + 4 * o) = *inst;
    I(0x14076b6a8 + 4 * k) = np;
    I(0x14076e588 + 4 * k) = nf;
    const bool fresh = I(0x1405c16f0) == 0;
    I(0x140676560) = 3;
    if (fresh) {
        fun_140225cd0(obj, inst, zero);
        I(0x140676560) = 3;
        fun_140225ff0(obj, inst, IP(0x140676560), npts, x, npoly, nvert, faces, base, zero);
    } else {
        fun_1402260a0(obj, inst, IP(0x140676560), npts, x, npoly, nvert, faces, base, zero);
    }
    fun_14005110a(obj, inst, x, faces, npts, npoly, nvert);
    I(0x140676564) = 1;
    I(0x140676568) = 2;
    fun_1400407e7(obj, inst, IP(0x140676564), IP(0x140676568), npts, npoly, zero, flag);
    fun_140029d47(obj, inst, npts, npoly);
    if (I(0x1405c16f0) == 0) {
        fun_14007c697(obj, inst, IP(0x140676564), npts, npoly, zero, flag);
    }
    I(0x14067656c) = *nvert;
    I(0x140676570) = *npoly * 0x24;
    table.assign(static_cast<std::size_t>(I(0x140676570) > 0 ? I(0x140676570) : 0), 0);
    I(0x140676574) = 1;
    I(0x140676578) = 1;
    i32 tmp_ent = 1;
    i32 tmp_fld = 0x14;
    if (I(0x1405c16f0) == 0) {
        fun_1400798bf(obj, npoly, inst, IP(0x140676574), table.data(), IP(0x14067657c), flag);
        tmp_ent = 1;
        tmp_fld = 0x14;
        fun_1402280d0(obj, inst, &tmp_ent, &tmp_fld, table.data(), IP(0x14067657c)); // EI20(1)
        I(0x140676580) = *npoly * *nvert;
        I(0x140676584) = I(0x140676580) * 2;
        const std::size_t ne = static_cast<std::size_t>(I(0x140676580) > 0 ? I(0x140676580) : 0);
        const std::size_t nb = static_cast<std::size_t>(I(0x140676584) > 0 ? I(0x140676584) : 0);
        std::vector<i32> e(ne, 0);
        std::vector<i32> stack(ne, 0);
        std::vector<i32> nbr(nb, 0); // 14004fd4b..: zeroed after the ALLOCATE
        fun_14007b1ea(obj, inst, IP(0x140676578), table.data(), IP(0x14067657c), npoly,
                      IP(0x14067656c), e.data(), stack.data(), nbr.data(), IP(0x140676588));
        if (I(0x140676588) > 1) {
            *flag = 1;
        }
    } else {
        fun_140228260(obj, inst, &tmp_ent, &tmp_fld, table.data(), IP(0x14067657c));
    }
    fun_14007aa22(obj, inst, table.data(), IP(0x14067657c), npoly, nvert);
    I(0x14067658c) = 3;
    fun_1400610f2(obj, npts, npoly, inst, IP(0x140676568), IP(0x14067658c), IP(0x140676578),
                  table.data(), IP(0x14067657c));
    I(0x140676590) = 3;
    I(0x140676594) = 1;
    fun_14005815c(obj, inst, npts, npoly);
    fun_14007d97c(x, npts, npoly, obj, inst);
    fun_14005bbf2(obj, npoly, npts, nvert, inst, IP(0x14067658c), IP(0x140676578), table.data());
    fun_14003c506(obj, inst, npts);
}

} // namespace def::cmd::ctc3d
