// SRCNT4 helpers (GENCTC 3D interference subtree), GENCTC_3d_srcnt4.md.
// Packet p67-3 (helper worker for p67-7 group "srcnt"). FP order from the listings
// (deform-rea-work/out/p677/lst/L_<addr>.txt); every SAVE local lives in the static image (m<>/mp<>).
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/srcnt.hpp>

#include <defrt/ftn/geoutl.hpp>
#include <defrt/ftn/geoutl_pending.hpp>
#include <defrt/ftn/sse_ops.hpp>

#include <cstdint>
#include <vector>

namespace def::cmd::ctc3d {

namespace sse = def::rt::sse;
using i64 = std::int64_t;

namespace {

// MOV r,x; NEG r; CMOVS r,x: |x| with INT_MIN left as it is.
i32 iabs(i32 v)
{
    const i32 n = static_cast<i32>(0u - static_cast<std::uint32_t>(v));
    return n < 0 ? v : n;
}

// Fortran ALLOCATE of an INTEGER work array (descriptor in the image, data on the heap in the
// binary). The descriptor is never read outside the routine, so the port keeps the data in a
// vector; at least one element so that data() is never null (an access beyond n is out of
// bounds in both).
std::vector<i32> alloc_i32(i32 n) { return std::vector<i32>(static_cast<std::size_t>(n > 1 ? n : 1), 0); }

} // namespace

// @src 140030ae1  box of points: mode 1 init (1e4 / -1e4), else min/max over x(:,|idx(1..n)|),
// widened by *margin unless mode 0
void fun_140030ae1(double* x, double* box, double* margin, i32* n, i32* idx, i32* mode)
{
    i32& c = m<i32>(0x14064f5d0);
    if (*mode == 1) {
        c = 1;
        do {
            box[c - 1] = 10000.0; // DAT_140bad5a0
            box[c + 2] = -10000.0; // DAT_140bad5a8
            c = c + 1;
        } while (c <= 3);
        return;
    }
    i32& i = m<i32>(0x14064f5d4);
    i32& k = m<i32>(0x14064f5d8);
    i = 1;
    if (*n >= 1) {
        do {
            k = iabs(idx[i - 1]);
            if (k > 0) {
                c = 1;
                do {
                    const double v = x[static_cast<i64>(c) + static_cast<i64>(k) * 3 - 4];
                    if (box[c - 1] > v) { // 140030c29 COMISD min,v; SETA
                        box[c - 1] = v;
                    }
                    if (v > box[c + 2]) { // 140030c3d COMISD v,max; SETA
                        box[c + 2] = v;
                    }
                    c = c + 1;
                } while (c <= 3);
            }
            i = i + 1;
        } while (i <= *n);
    }
    if (*mode != 0) {
        const double d = *margin;
        box[2] = sse::sub(box[2], d);
        box[1] = sse::sub(box[1], d);
        box[0] = sse::sub(box[0], d);
        box[3] = sse::add(d, box[3]); // 140030cbc ADDSD XMM0(margin),[max]
        box[5] = sse::add(d, box[5]);
        box[4] = sse::add(d, box[4]);
    }
}

// @src 140032a3e  flag(p) = 1 for listed polygons with EI2 == 1; *found = first polygon not
// flagged whose EI2 is 1 (0 if none)
void fun_140032a3e(i32* obj, i32* inst, i32* list, i32* n, i32* found, i32* npoly, i32* flag)
{
    i32& i = m<i32>(0x140650a34);
    i32& p = m<i32>(0x140650a38);
    i = 1;
    *found = 0;
    if (*n >= 1) {
        do {
            p = list[i - 1];
            if (p > 0) {
                i32 f = 2;
                fun_140228260(obj, inst, &p, &f, mp<i32>(0x140650a3c), mp<i32>(0x140650a40));
                fun_140227e00(obj, inst, &p, mp<i32>(0x140650a50), mp<i32>(0x140650a60));
                m<i32>(0x140650a64) = m<i32>(0x140650a60);
                if (m<i32>(0x140650a3c) == 1) {
                    flag[static_cast<i64>(p) - 1] = 1;
                }
            }
            i = i + 1;
        } while (i <= *n);
    }
    i = 1;
    if (*npoly >= 1) {
        do {
            if (flag[static_cast<i64>(i) - 1] == 0) {
                i32 e[2] = {i, 2};
                fun_140228260(obj, inst, &e[0], &e[1], mp<i32>(0x140650a3c), mp<i32>(0x140650a40));
                if (m<i32>(0x140650a3c) == 1) {
                    *found = i;
                    return;
                }
            }
            i = i + 1;
        } while (i <= *npoly);
    }
}

// @src 140032ce4  keep listed polygons with EI2 == 1, >= 2 nodes with a negative NI4 contact
// code and a centroid box (+-tol) overlapping box; compact the list, *n = kept count
void fun_140032ce4(i32* obj, i32* inst, i32* list, i32* n, double* box, double* tol)
{
    i32& i = m<i32>(0x140650a68);
    i = 1;
    if (*n >= 1) {
        do {
            const i64 ii = i;
            i32& e = m<i32>(0x140650a6c);
            e = list[ii - 1];
            i32 f = 2;
            fun_140228260(obj, inst, &e, &f, mp<i32>(0x140650a70), mp<i32>(0x140650a74));
            if (m<i32>(0x140650a70) == 1) {
                fun_140227e00(obj, inst, &e, mp<i32>(0x140650a80), mp<i32>(0x140650a90));
                const i32 cnt = m<i32>(0x140650a90);
                i32& nv = m<i32>(0x140650a94);
                nv = cnt;
                i32& nneg = m<i32>(0x140650a98);
                nneg = 0;
                m<i32>(0x140650a9c) = 0;
                if (iabs(m<i32>(0x140650a7c + 4 * static_cast<i64>(cnt))) == iabs(m<i32>(0x140650a80))) {
                    nv = cnt - 1;
                }
                double& sx = m<double>(0x140650aa0);
                double& sy = m<double>(0x140650aa8);
                double& sz = m<double>(0x140650ab0);
                i32& k = m<i32>(0x140650ac0);
                k = 1;
                sx = 0.0;
                sy = 0.0;
                sz = 0.0;
                if (nv >= 1) {
                    do {
                        i32& node = m<i32>(0x140650ac4);
                        node = iabs(m<i32>(0x140650a7c + 4 * static_cast<i64>(k)));
                        fun_140227fc0(obj, inst, &node, mp<double>(0x140650ad0));
                        sx = sse::add(m<double>(0x140650ad0), sx); // 1400331d5 ADDSD p,sum
                        sy = sse::add(m<double>(0x140650ad8), sy);
                        sz = sse::add(m<double>(0x140650ae0), sz);
                        i32 f4 = 4;
                        fun_140228830(obj, inst, &node, &f4, mp<i32>(0x140650af0), mp<i32>(0x140650b10));
                        if (m<i32>(0x140650af0) < 0 || m<i32>(0x140650af4) < 0 || m<i32>(0x140650af8) < 0) {
                            nneg = nneg + 1;
                        }
                        k = k + 1;
                    } while (k <= nv);
                }
                const double dn = static_cast<double>(nv); // 14003304b CVTSI2SD
                sz = sse::div(sz, dn);
                sy = sse::div(sy, dn);
                sx = sse::div(sx, dn);
                i32 one_n = 1;
                i32 one_idx = 1;
                def::rt::geoutl::fun_1404d1b1c(mp<double>(0x140650aa0), mp<double>(0x140650b20), tol, &one_n,
                                               &one_idx);
                m<i32>(0x140650b50) = 0;
                def::rt::geoutl::fun_1404d1ed7(box, mp<double>(0x140650b20), mp<i32>(0x140650b50));
                if (nneg < 2 || m<i32>(0x140650b50) == 0) {
                    list[ii - 1] = 0;
                }
            }
            i = i + 1;
        } while (i <= *n);
    }
    // Compaction (unrolled by four in the binary; same reads and writes in the same order).
    i32& kept = m<i32>(0x140650b54);
    kept = 0;
    i = 1;
    while (i <= *n) {
        const i32 v = list[static_cast<i64>(i) - 1];
        if (v != 0) {
            kept = kept + 1;
            list[static_cast<i64>(kept) - 1] = v;
        }
        i = i + 1;
    }
    *n = kept;
}

// @src 14003329f  re-validate (EI2 -2000 -> 1) polygons of obj that face the slave sobj
void fun_14003329f(i32* obj, i32* inst, i32* sobj, i32* sinst, double* x, i32* npoly, i32* nnode, double* margin,
                   double* box, i32* found)
{
    i32& j = m<i32>(0x140650b5c);
    const double& n1 = m<double>(0x140650c00); // ER1 of polygon j
    const double& n2 = m<double>(0x140650c08);
    const double& n3 = m<double>(0x140650c10);

    auto set_valid = [&]() {
        m<i32>(0x140650b60) = 1;
        i32 e = j;
        i32 f = 2;
        fun_1402280d0(obj, inst, &e, &f, mp<i32>(0x140650b60), mp<i32>(0x140650b64));
        *found = j;
    };

    // part of @src 14003329f: one polygon (return = label 14003336f, next polygon)
    auto one_polygon = [&]() {
        i32 e = j;
        fun_140227e00(obj, inst, &e, mp<i32>(0x140650b70), mp<i32>(0x140650b80));
        i32& nv = m<i32>(0x140650b80);
        i32& k = m<i32>(0x140650b84);
        k = 1;
        if (nv >= 1) {
            do {
                i32& node = m<i32>(0x140650b88);
                node = iabs(m<i32>(0x140650b6c + 4 * static_cast<i64>(k)));
                fun_140227fc0(obj, inst, &node, mp<double>(0x140650b78 + 0x18 * static_cast<i64>(k)));
                k = k + 1;
            } while (k <= nv);
        }
        i32& nva = m<i32>(0x140650bf0);
        nva = iabs(nv);
        const i32 last = m<i32>(0x140650b6c + 4 * static_cast<i64>(nva));
        if (last < 0) {
            return;
        }
        if (iabs(m<i32>(0x140650b70)) == iabs(last)) {
            nv = nva - 1;
        }
        i32 one = 1;
        e = j;
        fun_140228540(obj, inst, &e, &one, mp<double>(0x140650c00), mp<i32>(0x140650c20));
        def::rt::geoutl::fun_1404d1b1c(mp<double>(0x140650b90), mp<double>(0x140650c30), margin,
                                       mp<i32>(0x140650b80), mp<i32>(0x1405c6840));
        def::rt::geoutl::fun_1404d1ed7(box, mp<double>(0x140650c30), mp<i32>(0x140650c60));
        i32& c = m<i32>(0x140650c90);
        if (m<i32>(0x140650c60) == 1) {
            c = 1;
            double& cx = m<double>(0x140650c70);
            double& cy = m<double>(0x140650c78);
            double& cz = m<double>(0x140650c80);
            cy = 0.0;
            cx = 0.0;
            cz = 0.0;
            if (nv >= 1) {
                // 140033a3e CVTSI2SS + CVTSS2SD: the count goes through REAL*4
                const double dn = static_cast<double>(static_cast<float>(nv));
                do {
                    const double* p = mp<double>(0x140650b78 + 0x18 * static_cast<i64>(c));
                    cx = sse::add(sse::div(p[0], dn), cx);
                    cy = sse::add(sse::div(p[1], dn), cy);
                    c = c + 1;
                    cz = sse::add(sse::div(p[2], dn), cz);
                } while (c <= nv);
            }
            i32& i = m<i32>(0x140650c94);
            i = 1;
            if (*nnode >= 1) {
                do {
                    i32 a = i;
                    i32 f = 1;
                    fun_140228b10(sobj, sinst, &a, &f, mp<double>(0x140650ca0), mp<i32>(0x140650cc0));
                    const double dot = sse::add(sse::mul(n3, m<double>(0x140650cb0)),
                                                sse::add(sse::mul(n2, m<double>(0x140650ca8)),
                                                         sse::mul(n1, m<double>(0x140650ca0))));
                    m<double>(0x140650cc8) = dot;
                    if (-0.1 > dot) { // 140033be5 COMISD DAT_140bad620,dot; JP/JBE skip
                        const double* xi = x + 3 * (static_cast<i64>(i) - 1);
                        // 140033c3f-140033c49: the binary subtracts the centroid's z from all
                        // three components
                        const double dx = sse::sub(xi[0], cz);
                        const double dy = sse::sub(xi[1], cz);
                        const double dz = sse::sub(xi[2], cz);
                        const double len = sse::sqrt(
                            sse::add(sse::mul(dz, dz), sse::add(sse::mul(dy, dy), sse::mul(dx, dx))));
                        const double cs = sse::div(
                            sse::add(sse::mul(dz, n3), sse::add(sse::mul(dy, n2), sse::mul(dx, n1))), len);
                        m<double>(0x140650cd0) = dx;
                        m<double>(0x140650cd8) = dy;
                        m<double>(0x140650ce0) = dz;
                        m<double>(0x140650ce8) = len;
                        m<double>(0x140650cf0) = cs;
                        if (cs > 0.10000000149011612) { // DAT_140bad2d0
                            set_valid();
                            return;
                        }
                    }
                    i = i + 1;
                } while (i <= *nnode);
            }
        }
        m<i32>(0x140650cf8) = 0;
        c = 1;
        if (nv < 1) {
            return;
        }
        do {
            double* pt = mp<double>(0x140650b78 + 0x18 * static_cast<i64>(c));
            fun_14022d770(sobj, sinst, pt, mp<double>(0x140650d00), mp<std::uint32_t>(0x140650d08),
                          mp<i32>(0x140650b58));
            i32 f = 2;
            fun_140228830(sobj, sinst, mp<i32>(0x140650d08), &f, mp<i32>(0x140650d10), mp<i32>(0x140651cb0));
            i32& q = m<i32>(0x140650c94);
            q = 1;
            if (m<i32>(0x140651cb0) >= 1) {
                do {
                    i32& nb = m<i32>(0x140651cb4);
                    nb = m<i32>(0x140650d0c + 4 * static_cast<i64>(q));
                    def::rt::geoutl::fun_1404d68dd(pt, x + 3 * (static_cast<i64>(nb) - 1), mp<double>(0x140651cc0),
                                                   mp<double>(0x140651ce0));
                    const double dot = sse::add(sse::mul(m<double>(0x140651cd0), n3),
                                                sse::add(sse::mul(m<double>(0x140651cc8), n2),
                                                         sse::mul(m<double>(0x140651cc0), n1)));
                    m<double>(0x140651ce8) = dot;
                    if (dot > 0.0) {
                        set_valid();
                        return;
                    }
                    q = q + 1;
                } while (q <= m<i32>(0x140651cb0));
            }
            f = 2;
            fun_140228b10(sobj, sinst, mp<i32>(0x140650d08), &f, mp<double>(0x140651cf0), mp<i32>(0x140651cf8));
            const double r = sse::sqrt(m<double>(0x140651cf0));
            m<double>(0x140651d00) = r;
            if (r > m<double>(0x140650d00)) { // 140033702 COMISD r,dist; JA
                i32 g = 1;
                fun_140228b10(sobj, sinst, mp<i32>(0x140650d08), &g, mp<double>(0x140651cc0),
                              mp<i32>(0x140651d08));
                const double dot = sse::add(sse::mul(m<double>(0x140651cd0), n3),
                                            sse::add(sse::mul(m<double>(0x140651cc8), n2),
                                                     sse::mul(m<double>(0x140651cc0), n1)));
                m<double>(0x140651ce8) = dot;
                if (-0.5 > dot) { // DAT_140bad628; no exit, the vertex loop continues
                    set_valid();
                }
            }
            c = c + 1;
        } while (c <= nv);
    };

    m<i32>(0x140650b58) = 0;
    j = 1;
    *found = 0;
    if (*npoly < 1) {
        return;
    }
    do {
        i32 e = j;
        i32 f = 2;
        fun_140228260(obj, inst, &e, &f, mp<i32>(0x140650b60), mp<i32>(0x140650b64));
        if (m<i32>(0x140650b60) == -2000) {
            one_polygon();
        }
        j = j + 1;
    } while (j <= *npoly);
}

// @src 140033d76  flood fill from *start: EI2 := *value over polygons reachable by 140039ef9
// (mode 1); flag(j) = 1 beforehand for every polygon with EI2 != 1
void fun_140033d76(i32* obj, i32* inst, i32* npoly, i32* start, double* dir, i32* value, i32* flag)
{
    std::vector<i32> s = alloc_i32(*npoly); // ALLOCATE (descriptor DAT_140651d10)
    const i32 np = *npoly;
    i32& j = m<i32>(0x140651d60);
    j = 1;
    if (np >= 1) {
        do {
            i32 e[2] = {j, 2};
            fun_140228260(obj, inst, &e[0], &e[1], mp<i32>(0x140651d64), mp<i32>(0x140651d68));
            const i64 jj = j;
            if (m<i32>(0x140651d64) != 1) {
                flag[jj - 1] = 1;
            }
            j = j + 1;
            s[static_cast<std::size_t>(jj - 1)] = 0;
        } while (j <= np);
    }
    i32& cnt = m<i32>(0x140651d6c);
    cnt = 0;
    flag[static_cast<i64>(*start) - 1] = 1;
    for (;;) {
        i32 a[2] = {2, 1};
        fun_1402280d0(obj, inst, start, &a[0], value, &a[1]);
        i32 mode = 1;
        fun_140039ef9(obj, inst, start, s.data(), &cnt, flag, dir, &mode);
        const i32 t = cnt - 1;
        if (t < 0) {
            cnt = t;
            break;
        }
        m<i32>(0x140651d70) = cnt;
        const i64 top = cnt;
        cnt = t;
        *start = s[static_cast<std::size_t>(top - 1)];
    }
}

// @src 140034138  re-enable (EI2 := *value) the region of obj around the polygon onto which
// the first node of sobj with exactly one negative BC code, -obj, projects
void fun_140034138(i32* obj, i32* inst, i32* npoly, i32* sobj, i32* sinst, i32* nnode, double* dir, i32* value,
                   i32* flag, i32* bcc, i32* ndof)
{
    const i64 ld = *ndof > 0 ? *ndof : 0;
    std::vector<i32> s = alloc_i32(*npoly); // ALLOCATE (descriptor DAT_140651d80)
    const i32 np = *npoly;
    i32& c = m<i32>(0x140651dd0);
    c = 1;
    if (np >= 1) {
        do {
            s[static_cast<std::size_t>(c - 1)] = 0;
            flag[static_cast<i64>(c) - 1] = 0;
            c = c + 1;
        } while (c <= np);
    }
    m<i32>(0x140651dd4) = 1;
    m<i32>(0x140651dd8) = 0;
    m<i32>(0x140651ddc) = 1;
    c = 1;
    if (*nnode < 1) {
        return;
    }
    do {
        i32 a = c;
        i32 f = 4;
        fun_140228830(sobj, sinst, &a, &f, mp<i32>(0x140651de0), mp<i32>(0x140651e00));
        i32& nn = m<i32>(0x140651e04);
        i32& k = m<i32>(0x140651e08);
        i32& code = m<i32>(0x140651e0c);
        nn = 0;
        k = 1;
        do {
            const i32 v = bcc[static_cast<i64>(k) + (static_cast<i64>(c) - 1) * ld - 1];
            if (v < 0) {
                code = static_cast<i32>(0u - static_cast<std::uint32_t>(v));
                nn = nn + 1;
            }
            k = k + 1;
        } while (k <= 3);
        if (nn == 1 && *obj == code) {
            a = c;
            fun_140227fc0(sobj, sinst, &a, mp<double>(0x140651e10));
            i32 three = 3;
            m<double>(0x140651e30) = 100000.0; // DAT_140bad3c8
            fun_14006798b(obj, inst, &three, npoly, mp<double>(0x140651e10), mp<double>(0x140651e40),
                          mp<i32>(0x140651e60), mp<double>(0x140651e30), mp<i32>(0x140651e64),
                          mp<double>(0x140651e70), mp<double>(0x1405c6850), mp<i32>(0x140651dd4),
                          mp<i32>(0x140651dd8));
            const i32 p = m<i32>(0x140651e60);
            if (p > 0 && flag[static_cast<i64>(p) - 1] == 0) {
                i32& cur = m<i32>(0x140651e90);
                i32& cnt = m<i32>(0x140651e94);
                cur = p;
                m<i32>(0x140651ddc) = c + 1;
                flag[static_cast<i64>(p) - 1] = 1;
                cnt = 0;
                for (;;) {
                    i32 a2[2] = {2, 1};
                    fun_1402280d0(obj, inst, &cur, &a2[0], value, &a2[1]);
                    i32 mode = 0;
                    fun_140039ef9(obj, inst, &cur, s.data(), &cnt, flag, dir, &mode);
                    const i32 t = cnt - 1;
                    if (t < 0) {
                        cnt = t;
                        break;
                    }
                    m<i32>(0x140651e98) = cnt;
                    cur = s[static_cast<std::size_t>(cnt - 1)];
                    cnt = t;
                }
                return;
            }
        }
        c = c + 1;
    } while (c <= *nnode);
}

// @src 140039ef9  push the unvisited edge neighbours of *poly onto stack(1..*cnt) (mode 0:
// edges whose EI6 entry is 0; mode 1: neighbours with EI2 >= 0 and dir.ER1 >= -0.1)
void fun_140039ef9(i32* obj, i32* inst, i32* poly, i32* stack, i32* cnt, i32* flag, double* dir, i32* mode)
{
    i32 f = 1;
    fun_140228260(obj, inst, poly, &f, mp<i32>(0x140655800), mp<i32>(0x140655830)); // EI1
    fun_140227e00(obj, inst, poly, mp<i32>(0x140655840), mp<i32>(0x140655850));
    f = 6;
    fun_140228260(obj, inst, poly, &f, mp<i32>(0x140655860), mp<i32>(0x140655880)); // EI6
    i32& nv = m<i32>(0x140655850);
    if (m<i32>(0x14065583c + 4 * static_cast<i64>(nv)) == m<i32>(0x140655840)) { // no abs here
        nv = nv - 1;
    }
    f = 1;
    fun_140228540(obj, inst, poly, &f, mp<double>(0x140655890), mp<i32>(0x1406558b0)); // ER1
    i32& k = m<i32>(0x1406558b4);
    k = 1;
    if (nv < 1) {
        return;
    }
    const double d1 = dir[0];
    const double d2 = dir[1];
    const double d3 = dir[2];
    i32 kn = 2; // EBX = k + 1
    do {
        const i64 kk = k;
        const i32 nb = m<i32>(0x140655800 + 4 * (kk - 1));
        if (nb != 0) {
            i32& cur = m<i32>(0x1406558b8);
            cur = nb;
            if (flag[static_cast<i64>(nb) - 1] != 1) {
                i32& edge = m<i32>(0x1406558bc);
                edge = m<i32>(0x140655860 + 8 * (kk - 1));
                i32 two = 2;
                fun_140228260(obj, inst, &cur, &two, mp<i32>(0x1406558c0), mp<i32>(0x1406558c4));
                const i32 e = m<i32>(0x1406558c0);
                bool push = false;
                if (e == 0 || e == -1000 || e >= 600) {
                    flag[static_cast<i64>(cur) - 1] = 1;
                } else if (*mode == 1 && e < 0) {
                    flag[static_cast<i64>(cur) - 1] = 1;
                } else if (*mode == 0) {
                    if (edge == 0) {
                        push = true;
                    } else {
                        flag[static_cast<i64>(cur) - 1] = 1;
                    }
                } else {
                    i32 one = 1;
                    fun_140228540(obj, inst, &cur, &one, mp<double>(0x1406558d0), mp<i32>(0x1406558b0));
                    const double dot = sse::add(sse::mul(d3, m<double>(0x1406558e0)),
                                                sse::add(sse::mul(d2, m<double>(0x1406558d8)),
                                                         sse::mul(d1, m<double>(0x1406558d0))));
                    m<double>(0x1406558f0) = dot;
                    // 14003a137 COMISD dot,DAT_140bad750; JP proceeds, JC skips
                    if (!(dot < -0.10000000149011612)) {
                        m<i32>(0x1406558f8) = m<i32>(0x14065583c + 4 * kk);
                        i32& nx = m<i32>(0x1406558fc);
                        nx = kn;
                        if (nv < kn) {
                            nx = 1;
                        }
                        m<i32>(0x140655900) = m<i32>(0x14065583c + 4 * static_cast<i64>(nx));
                        i32 f4 = 4;
                        fun_140228830(obj, inst, mp<i32>(0x1406558f8), &f4, mp<i32>(0x140655910), mp<i32>(0x140655930));
                        f4 = 4;
                        fun_140228830(obj, inst, mp<i32>(0x140655900), &f4, mp<i32>(0x140655940), mp<i32>(0x140655930));
                        // result unused by the binary (the push below is unconditional)
                        fun_14003a4be(mp<i32>(0x140655910), mp<i32>(0x140655940), mp<i32>(0x140655960));
                        m<double>(0x1406558f0) =
                            sse::add(sse::mul(m<double>(0x1406558a0), m<double>(0x1406558e0)),
                                     sse::add(sse::mul(m<double>(0x140655898), m<double>(0x1406558d8)),
                                              sse::mul(m<double>(0x140655890), m<double>(0x1406558d0))));
                        push = true;
                    }
                }
                if (push) { // 14003a2be
                    const i32 c1 = *cnt + 1;
                    flag[static_cast<i64>(cur) - 1] = 1;
                    *cnt = c1;
                    stack[static_cast<i64>(c1) - 1] = cur;
                }
            }
        }
        k = k + 1;
        kn = kn + 1;
    } while (k <= nv);
}

// @src 14003a4be  *found = 1 when a(1..3) and b(1..3) share a negative value
void fun_14003a4be(i32* a, i32* b, i32* found)
{
    i32& i = m<i32>(0x140655964);
    i32& j = m<i32>(0x140655968);
    i = 1;
    *found = 0;
    do {
        j = 1;
        const i32 ai = a[static_cast<i64>(i) - 1];
        do {
            if (b[static_cast<i64>(j) - 1] == ai && ai < 0) {
                *found = 1;
                return;
            }
            j = j + 1;
        } while (j <= 3);
        i = i + 1;
    } while (i <= 3);
}

} // namespace def::cmd::ctc3d
