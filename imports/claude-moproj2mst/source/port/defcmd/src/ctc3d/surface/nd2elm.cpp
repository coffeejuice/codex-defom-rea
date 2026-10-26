// ND2ELM: node -> polygon lists NI1 with the hashed bins of 14007e28d (GENCTC_3d_search.md
// sec. 5.7). Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>

#include <cstdint>
#include <vector>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;

// @src 14007e28d  bins(m, *): column b holds m-1 entries and in row m the link -(next column);
// columns 1..npts are the node heads, *nbin the last column in use
void fun_14007e28d(i32* nbin, i32* bins, i32* cnt, const i32* node, const i32* val,
                   const i32* get, i32* out, const i32* m)
{
    const i32 mm = *m;
    const std::int64_t ld = mm < 0 ? 0 : mm;
    const i32 n = cnt[*node - 1];
    auto B = [&](std::int64_t row, std::int64_t col) -> i32& { return bins[(col - 1) * ld + row - 1]; };
    i32& col = I(0x1406d2a24);
    i32& j = I(0x1406d2a28);
    i32& r = I(0x1406d2a1c);
    if (*get != 0) {
        col = *node;
        j = 1;
        I(0x1406d2a34) = n;
        if (n > 0) {
            const i32 m1 = mm - 1;
            do {
                r = imod(j, m1);
                if (r == 0) {
                    r = m1;
                }
                if (r == 1 && m1 < j) {
                    col = iabs(B(mm, col));
                }
                out[j - 1] = B(r, col);
                j = j + 1;
            } while (j <= n);
        }
        return;
    }
    const i32 n1 = n + 1;
    I(0x1406d2a18) = n1;
    const i32 m1 = mm - 1;
    const i32 q = idiv(n1, m1);
    I(0x1406d2a20) = q;
    r = imod(n1, m1);
    cnt[*node - 1] = n1;
    if (r == 0) {
        r = m1;
    }
    col = *node;
    j = 1;
    if (q > 0) {
        do {
            const i32 link = B(mm, col);
            I(0x1406d2a2c) = link;
            if (link < 0) {
                col = -link;
                if (-link < 0) {
                    col = link;
                }
            }
            j = j + 1;
        } while (j <= q);
    }
    i32& dst = I(0x1406d2a30);
    dst = col;
    if (r == 1 && q > 0) {
        const i32 nb = *nbin + 1;
        *nbin = nb;
        B(mm, col) = -nb;
        j = 1;
        if (mm > 0) {
            do {
                B(j, nb) = 0;
                j = j + 1;
            } while (j <= mm);
        }
        dst = *nbin;
    }
    B(r, dst) = *val;
}

// @src 14007c697  ND2ELM: NI(*field) = ascending list of the polygons (flag != 0) at each node
void fun_14007c697(i32* obj, i32* inst, i32* field, const i32* npts, const i32* npoly,
                   const i32* /*zero*/, i32* /*flag*/)
{
    i32& m = I(0x1406c3a10);
    i32& nbcol = I(0x1406c3a14); // columns allocated
    i32& k = I(0x1406c3a1c);
    i32& nnode = I(0x1406c3a24);
    i32& nbin = I(0x1406c3a30);
    i32& p = I(0x1406c3a34);
    i32* conn = IP(0x1406c3a40);
    i32& ccnt = I(0x1406c3a70);
    i32& cflag = I(0x1406c3a74);
    i32* list = IP(0x1406c3a80); // 5000 INTEGERs
    i32& lcnt = I(0x1406c88a0);
    i32& j = I(0x1406c88a4);

    m = 16;
    nbcol = static_cast<i32>(static_cast<float>(*npts) * 1.5f); // 140bae3c0, CVTTSS2SI
    std::vector<i32> cnt;  // dope 1406c39c0: per node
    std::vector<i32> bins; // dope 1406c3970: bins(m, nbcol)
restart:
    {
        const i32 nbins = m * nbcol;
        I(0x1406c3a18) = nbins;
        cnt.assign(static_cast<std::size_t>(*npts > 0 ? *npts : 0), 0);
        bins.assign(static_cast<std::size_t>(nbins > 0 ? nbins : 0), 0);
        // (the zeroing loops of the original leave their counters in k, nnode, 1406c3a28/2c)
        I(0x1406c3a20) = 0;
        nbin = *npts;
        p = 1;
        if (*npoly > 0) {
            do {
                i32 ent = p;
                fun_140227e00(obj, inst, &ent, conn, &ccnt);
                fun_140062fb2(conn, &ccnt, &cflag);
                if (cflag != 0) {
                    k = 1;
                    if (ccnt > 0) {
                        do {
                            nnode = iabs(I(0x1406c3a3c + 4ull * static_cast<std::uint32_t>(k)));
                            if (nnode != 0) {
                                i32 v = p;
                                i32 g = 1;
                                fun_14007e28d(&nbin, bins.data(), cnt.data(), &nnode, &v, &g, list,
                                              &m);
                                lcnt = cnt[nnode - 1];
                                if (lcnt > 4999) {
                                    // str 1405c87e0, unit DAT_14077a7bc, then STOP
                                    def::rt::ftn::fio::Write(
                                        msg_unit(),
                                        "('Error: Local Array size exceeded in ND2ELM ')")
                                        .end();
                                    ftn_stop();
                                }
                                bool have = false;
                                if (lcnt != 0) {
                                    j = 1;
                                    if (lcnt > 0) {
                                        do {
                                            I(0x1406c88a8) = list[j - 1];
                                            if (list[j - 1] == p) {
                                                have = true;
                                                break;
                                            }
                                            j = j + 1;
                                        } while (j <= lcnt);
                                    }
                                }
                                if (!have) {
                                    v = p;
                                    g = 0;
                                    fun_14007e28d(&nbin, bins.data(), cnt.data(), &nnode, &v, &g,
                                                  list, &m);
                                    if (nbcol <= nbin) {
                                        nbcol = nbcol + *npts; // 14007cdab: grow by npts, start over
                                        goto restart;
                                    }
                                }
                            }
                            k = k + 1;
                        } while (k <= ccnt);
                    }
                }
                p = p + 1;
            } while (p <= *npoly);
        }
    }
    I(0x1406c88ac) = 0;
    k = 1;
    if (*npts > 0) {
        do {
            i32 nd = k;
            i32 g = 1;
            fun_14007e28d(&nbin, bins.data(), cnt.data(), &nd, IP(0x1406c88ac), &g, list, &m);
            I(0x1406c88b0) = cnt[k - 1];
            I(0x1406c88b4) = k;
            fun_140228690(obj, inst, IP(0x1406c88b4), field, list, IP(0x1406c88b0)); // NI1
            k = k + 1;
        } while (k <= *npts);
    }
}

} // namespace def::cmd::ctc3d
