// Half-edges, neighbours, patches and the edge table (GENCTC_3d_search.md sec. 5.6).
// Group "surface", packet p67-7.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/search.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defrt/ftn/fio.hpp>

#include <cstdint>
#include <vector>

#include "surf_util.hpp"

namespace def::cmd::ctc3d {

using namespace surf;

namespace {

// Record r (1-based) of a table with 8 INTEGERs per record, field f 0-based.
inline i32& R8(i32* t, std::int64_t r, int f) { return t[(r - 1) * 8 + f]; }

} // namespace

// @src 14005a6bf  remove repeated entries (first kept, order kept), *n = number left
void fun_14005a6bf(i32* list, i32* n)
{
    i32& i = I(0x140694470);
    i32& j = I(0x140694478);
    i32& out = I(0x14069447c);
    i = 1;
    if (*n > 0) {
        do {
            const i32 v = list[i - 1];
            I(0x140694474) = v;
            if (v != 0) {
                j = i + 1;
                if (j <= *n) {
                    do {
                        const i32 w = list[j - 1];
                        if (w != 0 && v == w) {
                            list[j - 1] = 0;
                        }
                        j = j + 1;
                    } while (j <= *n);
                }
            }
            i = i + 1;
        } while (i <= *n);
        out = 0;
        i = 1;
        do {
            const i32 v = list[i - 1];
            if (v != 0) {
                out = out + 1;
                list[out - 1] = v;
            }
            i = i + 1;
        } while (i <= *n);
        *n = out;
        return;
    }
    *n = 0;
    out = 0;
}

// @src 14006f089  distinct entries of a that occur in b, in the order of a
void fun_14006f089(const i32* na, const i32* a, const i32* nb, const i32* b, i32* nout, i32* out)
{
    i32& i = I(0x1406acb38);
    i32& j = I(0x1406acb40);
    i32& k = I(0x1406acb48);
    i = 1;
    out[0] = 0;
    *nout = 0;
    if (*na > 0) {
        do {
            const i32 v = a[i - 1];
            I(0x1406acb3c) = v;
            j = 1;
            if (*nb > 0) {
                bool next = false;
                do {
                    const i32 w = b[j - 1];
                    if (w != 0) {
                        I(0x1406acb44) = w;
                        if (v == w) {
                            k = 1;
                            if (*nout > 0) {
                                do {
                                    if (out[k - 1] == v) {
                                        next = true; // 14006f11f: on to the next entry of a
                                        break;
                                    }
                                    k = k + 1;
                                } while (k <= *nout);
                            }
                            if (next) {
                                break;
                            }
                            const i32 c = *nout;
                            *nout = c + 1;
                            out[c] = v;
                        }
                    }
                    j = j + 1;
                } while (j <= *nb);
            }
            i = i + 1;
        } while (i <= *na);
    }
}

// @src 1400798bf  half-edge records (n1, n2, poly, k, poly2, k2, count, -) and their pairing
void fun_1400798bf(i32* obj, const i32* npoly, i32* inst, i32* fni1, i32* table, i32* nedge,
                   i32* flag)
{
    i32& p = I(0x1406b3ab0);
    i32* conn = IP(0x1406b3ac0); // conn(k) = I(0x1406b3abc + 4k)
    i32& ccnt = I(0x1406b3af0);
    i32& cflag = I(0x1406b3af4);
    i32& nv = I(0x1406b3af8);
    i32& k = I(0x1406b3b00);
    i32& kk = I(0x1406b3b04);
    i32& nout = I(0x1406b3b08);
    i32& n1 = I(0x1406b3b0c);
    i32& n2 = I(0x1406b3b10);
    i32& pp = I(0x1406b3b14);
    i32* l1 = IP(0x1406b3b30);
    i32& c1 = I(0x1406b8950);
    i32* l2 = IP(0x1406b8960);
    i32& c2 = I(0x1406bd780);
    i32& nc = I(0x1406bd784);
    i32* cand = IP(0x1406bd790);
    i32& nmatch = I(0x1406bd920);
    double& best = D(0x1406bd928);
    double* n0 = DP(0x1406bd930);
    i32& ecnt = I(0x1406bd950);
    i32& ibest = I(0x1406bd954);
    i32& jc = I(0x1406bd958);
    i32& q = I(0x1406bd95c);
    double* nq = DP(0x1406bd960);
    double& dot = D(0x1406bd980);
    auto C = [](i32 i) -> i32 { return I(0x1406b3abc + 4ull * static_cast<std::uint32_t>(i)); };

    // dope 1406b3a60: first record of each polygon (4 * npoly INTEGERs, ALLOCATE)
    std::vector<i32> first(static_cast<std::size_t>((*npoly << 2) > 0 ? (*npoly << 2) : 0), 0);
    *nedge = 0;
    p = 1;
    if (*npoly < 1) {
        nout = 0;
        *nedge = nout;
        return;
    }
    do {
        i32 ent = p;
        fun_140227e00(obj, inst, &ent, conn, &ccnt);
        fun_140062fb2(conn, &ccnt, &cflag);
        if (cflag != 0 && conn[0] != 0) {
            nv = ccnt;
            if (iabs(C(ccnt)) == iabs(conn[0])) {
                nv = ccnt - 1;
            }
            const i32 nvl = nv;
            k = 1;
            I(0x1406b3afc) = p - 1;
            first[p - 1] = *nedge + 1;
            if (nvl > 0) {
                i32 k2 = 2;
                do {
                    const i32 r = *nedge + 1;
                    *nedge = r;
                    kk = k2;
                    R8(table, r, 0) = iabs(C(k));
                    if (nv < k2) {
                        kk = k2 - nv;
                    }
                    R8(table, r, 2) = p;
                    R8(table, r, 1) = iabs(C(kk));
                    R8(table, r, 3) = k;
                    R8(table, r, 4) = 0;
                    R8(table, r, 5) = 0;
                    R8(table, r, 6) = 1;
                    k = k + 1;
                    k2 = k2 + 1;
                } while (k <= nv);
            }
        }
        p = p + 1;
    } while (p <= *npoly);

    const i32 nrec = *nedge;
    p = 1;
    if (nrec > 0) {
        do {
            n1 = R8(table, p, 0);
            n2 = R8(table, p, 1);
            pp = R8(table, p, 2);
            if (n1 != 0) {
                I(0x1406b3b20) = n1;
                I(0x1406b3b24) = n2;
                fun_140228830(obj, inst, &n1, fni1, l1, &c1);
                fun_140228830(obj, inst, &n2, fni1, l2, &c2);
                fun_14006f089(&c1, l1, &c2, l2, &nc, cand);
                i32 one = 1;
                nmatch = 0;
                best = -10.0; // 140badde0
                fun_140228540(obj, inst, &pp, &one, n0, &ecnt);
                ibest = 0;
                jc = 1;
                if (nc > 0) {
                    do {
                        q = cand[jc - 1];
                        fun_140227e00(obj, inst, &q, conn, &ccnt);
                        one = 1;
                        fun_140228540(obj, inst, &q, &one, nq, &ecnt);
                        dot = dot3(n0, nq);
                        fun_140062fb2(conn, &ccnt, &cflag);
                        if (cflag != 0 && conn[0] > 0) {
                            nv = ccnt;
                            if (iabs(C(ccnt)) == iabs(conn[0])) {
                                nv = ccnt - 1;
                            }
                            I(0x1406b3afc) = q - 1;
                            const i32 s = first[q - 1];
                            I(0x1406bd988) = s;
                            const i32 e = s - 1 + nv;
                            I(0x1406bd98c) = e;
                            for (k = s; k <= e; k = k + 1) {
                                const i32 b1 = R8(table, k, 1);
                                const i32 b0 = R8(table, k, 0);
                                I(0x1406bd990) = b1;
                                I(0x1406bd994) = b0;
                                if (b1 == n1 && b0 == n2) {
                                    nmatch = nmatch + 1;
                                }
                                if (b0 == n1 && b1 == n2) {
                                    nmatch = nmatch + 1;
                                }
                                if (b1 == n1 && b0 == n2 && best < dot) {
                                    best = dot;
                                    ibest = k;
                                }
                            }
                        }
                        jc = jc + 1;
                    } while (jc <= nc);
                    if (ibest > 0) {
                        R8(table, ibest, 0) = 0;
                        R8(table, p, 4) = R8(table, ibest, 2);
                        R8(table, p, 6) = R8(table, p, 6) + 1;
                        R8(table, p, 5) = R8(table, ibest, 3);
                    }
                    if (nmatch > 2) {
                        *flag = 1;
                    }
                }
            }
            p = p + 1;
        } while (p <= nrec);
    }
    nout = 0;
    p = 1;
    if (nrec > 0) {
        do {
            I(0x1406bd998) = R8(table, p, 0);
            if (R8(table, p, 0) != 0) {
                nout = nout + 1;
                for (k = 1; k < 8; k = k + 1) { // fields 1..7 (the 8th is not copied)
                    R8(table, nout, k - 1) = R8(table, p, k - 1);
                }
            }
            p = p + 1;
        } while (p <= nrec);
    }
    *nedge = nout;
}

// @src 14007c24c  polygon q of patch *patch with the reversed half-edge (n2 -> n1)
void fun_14007c24c(i32* obj, i32* inst, i32* n1, i32* n2, const i32* patch, i32* poly, i32* k)
{
    i32& n = I(0x1406c3910);
    i32& n2c = I(0x1406c3914);
    i32& nt = I(0x1406c3918);
    i32& j = I(0x1406c391c);
    i32& q = I(0x1406c3920);
    i32& qpatch = I(0x1406c3924);
    i32* conn = IP(0x1406c3930);
    i32& ccnt = I(0x1406c3950);
    i32& kk = I(0x1406c3954);
    i32& k2 = I(0x1406c3958);

    i32 one = 1;
    fun_140228830(obj, inst, n1, &one, IP(0x1406c19d0), &n);
    one = 1;
    // 14007c2e9: NI1(*n2) appended at list(n + 1) (the 2000-entry buffer is not checked)
    fun_140228830(obj, inst, n2, &one, IP(0x1406c19cc + 4ull * static_cast<std::uint32_t>(n + 1)),
                  &n2c);
    nt = n2c + n;
    fun_14005a6bf(IP(0x1406c19d0), &nt);
    const i32 ntot = nt;
    j = 1;
    *poly = 0;
    if (ntot > 0) {
        do {
            q = I(0x1406c19cc + 4ull * static_cast<std::uint32_t>(j));
            i32 f4 = 4;
            fun_140228260(obj, inst, &q, &f4, &qpatch, IP(0x1406c3928));
            if (*patch == qpatch) {
                fun_140227e00(obj, inst, &q, conn, &ccnt);
                kk = 1;
                if (ccnt > 0) {
                    i32 c = 2;
                    do {
                        k2 = c;
                        if (ccnt < c) {
                            k2 = 1;
                        }
                        const i32 a = iabs(I(0x1406c392c + 4ull * static_cast<std::uint32_t>(kk)));
                        const i32 b = iabs(I(0x1406c392c + 4ull * static_cast<std::uint32_t>(k2)));
                        I(0x1406c3960) = b;
                        I(0x1406c395c) = a;
                        *k = kk;
                        if (*n1 == b && *n2 == a) {
                            *poly = q;
                            return;
                        }
                        kk = kk + 1;
                        c = c + 1;
                    } while (kk <= ccnt);
                }
            }
            j = j + 1;
        } while (j <= nt);
    }
}

// @src 14007b1ea  patches (EI4) by flooding over paired edges, neighbour lists Nb -> EI1
void fun_14007b1ea(i32* obj, i32* inst, i32* /*fei1*/, i32* table, const i32* nedge,
                   const i32* npoly, const i32* nvert, i32* e, i32* stack, i32* nb, i32* npatch)
{
    const i32 nv = *nvert;
    const std::int64_t lde = nv < 0 ? 0 : nv;
    const i32 nv2 = nv * 2;
    const std::int64_t ldn = nv2 < 0 ? 0 : nv2;
    auto E = [&](std::int64_t kk, std::int64_t pp) -> i32& { return e[(pp - 1) * lde + kk - 1]; };
    auto NB = [&](std::int64_t kk, std::int64_t pp) -> i32& { return nb[(pp - 1) * ldn + kk - 1]; };
    i32& i = I(0x1406c192c);
    i32& k = I(0x1406c1930);
    i32& p1 = I(0x1406c1934);
    i32& p2 = I(0x1406c1938);
    i32& patch = I(0x1406c1948);
    i32& r = I(0x1406c194c);
    i32& f1 = I(0x1406c1950);
    i32& f2 = I(0x1406c1954);
    i32& sp = I(0x1406c1964);
    i32& q = I(0x1406c1970);
    i32& kq = I(0x1406c1974);

    i = 1;
    if (*npoly > 0) {
        do {
            k = 1;
            if (nv > 0) {
                do {
                    E(k, i) = 0;
                    k = k + 1;
                } while (k <= nv);
            }
            i = i + 1;
        } while (i <= *npoly);
    }
    i = 1;
    if (*nedge > 0) {
        do {
            p1 = R8(table, i, 2);
            p2 = R8(table, i, 4);
            I(0x1406c1940) = R8(table, i, 5);
            I(0x1406c193c) = R8(table, i, 3);
            if (p1 > 0) {
                E(I(0x1406c193c), p1) = i;
            }
            if (p2 > 0) {
                E(I(0x1406c1940), p2) = i;
            }
            i = i + 1;
        } while (i <= *nedge);
    }
    I(0x1406c1944) = 0;
    i = 1;
    if (*npoly > 0) {
        do {
            i32 ent = i;
            i32 f4 = 4;
            i32 one = 1;
            fun_1402280d0(obj, inst, &ent, &f4, IP(0x1406c1944), &one); // EI4 = 0
            i = i + 1;
        } while (i <= *npoly);
    }

    // Flood one polygon's unassigned neighbours across edges with fewer than 3 half-edges.
    auto spread = [&](i32 pc, i32& ecell, i32& fcell, std::uint64_t cntaddr) {
        k = 1;
        if (nv < 1) {
            return;
        }
        do {
            const i32 eid = E(k, pc);
            ecell = eid;
            if (eid != 0) {
                q = R8(table, eid, 2);
                kq = R8(table, eid, 3);
                if (q == pc) {
                    kq = R8(table, eid, 5);
                    q = R8(table, eid, 4);
                }
                if (q < 1) {
                    fcell = 1000;
                } else {
                    i32 f4 = 4;
                    fun_140228260(obj, inst, &q, &f4, &fcell, IP(cntaddr));
                    if (q != 0 && R8(table, eid, 6) < 3 && fcell == 0) {
                        sp = sp + 1;
                        stack[sp - 1] = eid;
                        NB(k, pc) = q;
                        NB(kq, q) = pc;
                    }
                }
            }
            k = k + 1;
        } while (k <= nv);
    };

    i32 last = 0; // R12 at 14007b527: the patch number before the increment
    patch = 0;
    for (;;) {
        last = patch;
        patch = patch + 1;
        r = 1;
        if (*nedge < 1) {
            break;
        }
        bool start = false;
        for (;;) {
            p1 = R8(table, r, 2);
            p2 = R8(table, r, 4);
            f1 = 0;
            f2 = 0;
            I(0x1406c1958) = r;
            if (p1 > 0) {
                i32 f4 = 4;
                fun_140228260(obj, inst, &p1, &f4, &f1, IP(0x1406c195c));
            } else {
                f1 = 1000;
            }
            if (p2 > 0) {
                i32 f4 = 4;
                fun_140228260(obj, inst, &p2, &f4, &f2, IP(0x1406c1960));
            } else if (p1 > 0) {
                f2 = 1000;
            } else {
                // 14007b87f: neither side valid, next record
                r = r + 1;
                f2 = 1000;
                if (*nedge < r) {
                    break;
                }
                continue;
            }
            if (f1 == 0 || f2 == 0) {
                start = true;
                break;
            }
            r = r + 1;
            if (*nedge < r) {
                break;
            }
        }
        if (!start) {
            break;
        }
        sp = 1;
        stack[0] = r;
        do {
            const i32 ec = stack[sp - 1];
            I(0x1406c1968) = ec;
            p1 = R8(table, ec, 2);
            sp = sp - 1;
            if (p1 > 0) {
                i32 f4 = 4;
                fun_140228260(obj, inst, &p1, &f4, &f1, IP(0x1406c195c));
                if (f1 == 0) {
                    i32 fe = 4;
                    i32 one = 1;
                    fun_1402280d0(obj, inst, &p1, &fe, &patch, &one); // EI4 = patch
                    spread(p1, I(0x1406c196c), I(0x1406c1978), 0x1406c195c);
                }
            }
            p2 = R8(table, ec, 4);
            if (R8(table, ec, 4) > 0) {
                i32 f4 = 4;
                fun_140228260(obj, inst, &p2, &f4, &f2, IP(0x1406c1960));
                if (f2 == 0) {
                    i32 fe = 4;
                    i32 one = 1;
                    fun_1402280d0(obj, inst, &p2, &fe, &patch, &one);
                    spread(p2, I(0x1406c197c), I(0x1406c1980), 0x1406c1960);
                }
            }
        } while (sp != 0);
    }
    *npatch = last;
    if (last > 1) {
        // str 140bae390, list-directed
        def::rt::ftn::fio::Write(msg_unit()).a(" Info - Multiple patches detected: ", 35).i(*npatch).end();
    }

    // Edges the flood did not pair: same-patch polygon with the reversed half-edge (14007c24c).
    i = 1;
    if (*npoly < 1) {
        i = 1;
        return;
    }
    i32* conn = IP(0x1406c1990);
    i32& ccnt = I(0x1406c19b0);
    i32& nvp = I(0x1406c19b4);
    do {
        i32 ent = i;
        fun_140227e00(obj, inst, &ent, conn, &ccnt);
        nvp = ccnt;
        if (ccnt == 4 && conn[0] == iabs(conn[3])) {
            nvp = 3;
        }
        k = 1;
        if (nvp > 0) {
            i32 c = 2;
            while (k <= nvp) {
                if (NB(k, i) == 0) {
                    i32 e1 = i;
                    fun_140227e00(obj, inst, &e1, conn, &ccnt);
                    i32 f4 = 4;
                    i32 e2 = i;
                    fun_140228260(obj, inst, &e2, &f4, &patch, IP(0x1406c1960));
                    i32& k2 = I(0x1406c19b8);
                    k2 = c;
                    if (nv < c) {
                        k2 = 1;
                    }
                    I(0x1406c19bc) = iabs(conn[k - 1]);
                    I(0x1406c19c0) = iabs(conn[k2 - 1]);
                    fun_14007c24c(obj, inst, IP(0x1406c19bc), IP(0x1406c19c0), &patch,
                                  IP(0x1406c19c4), IP(0x1406c19c8));
                    const i32 qq = I(0x1406c19c4);
                    if (qq > 0) {
                        NB(I(0x1406c19c8), qq) = i;
                        NB(k, i) = qq;
                    }
                }
                k = k + 1;
                c = c + 1;
            }
        }
        i = i + 1;
    } while (i <= *npoly);
    i = 1;
    do {
        i32 ent = i;
        i32 one = 1;
        i32 n2 = nv2;
        fun_1402280d0(obj, inst, &ent, &one, &NB(1, i), &n2); // EI1 = Nb(1..2nv, i)
        i = i + 1;
    } while (i <= *npoly);
}

// @src 14007aa22  edge table (n1, n2, p, k, q, kq, 0, -) and the edge ids EI1(nv+k)
void fun_14007aa22(i32* obj, i32* inst, i32* table, i32* nedge, const i32* npoly, const i32* nvert)
{
    i32& ne = I(0x1406bda3c);
    i32& p = I(0x1406bda40);
    i32* ei = IP(0x1406bda50);     // EI1(p), ei(j) = I(0x1406bda4c + 4j)
    i32* conn = IP(0x1406bf9a0);   // conn(j) = I(0x1406bf99c + 4j)
    i32& ccnt = I(0x1406bf9c0);
    i32& nvp = I(0x1406bf9c4);
    i32& k2 = I(0x1406bf9c8);
    i32& k = I(0x1406bf9cc);
    i32& q = I(0x1406bf9d0);
    i32* eq = IP(0x1406bf9e0);     // EI1(q), eq(j) = I(0x1406bf9dc + 4j)
    i32& kq = I(0x1406c1924);
    i32& j = I(0x1406c1928);
    auto EI = [](i32 jj) -> i32& { return I(0x1406bda4c + 4ull * static_cast<std::uint32_t>(jj)); };
    auto EQ = [](i32 jj) -> i32& { return I(0x1406bf9dc + 4ull * static_cast<std::uint32_t>(jj)); };
    auto CN = [](i32 jj) -> i32 { return I(0x1406bf99c + 4ull * static_cast<std::uint32_t>(jj)); };

    ne = 0;
    p = 1;
    if (*npoly > 0) {
        const i32 nv = *nvert;
        do {
            i32 one = 1;
            i32 ent = p;
            fun_140228260(obj, inst, &ent, &one, ei, IP(0x1406bf990));
            ent = p;
            fun_140227e00(obj, inst, &ent, conn, &ccnt);
            nvp = ccnt;
            if (ccnt == 4 && iabs(conn[0]) == iabs(conn[3])) {
                nvp = 3;
            }
            k2 = 0;
            k = 1;
            if (*nvert > 0) {
                i32 c = 2;
                i32 idx = nv;
                do {
                    const i32 kc = k;
                    idx = idx + 1;
                    q = EI(k);
                    k2 = c;
                    if (nvp < c) {
                        k2 = 1;
                    }
                    if (q == 0) {
                        ne = ne + 1;
                        EI(idx) = 0;
                        R8(table, ne, 0) = CN(kc);
                        R8(table, ne, 4) = 0;
                        R8(table, ne, 5) = 0;
                        R8(table, ne, 6) = 0;
                        R8(table, ne, 2) = p;
                        R8(table, ne, 3) = kc;
                        R8(table, ne, 1) = CN(k2);
                    } else if (p < q) {
                        i32 o1 = 1;
                        fun_140228260(obj, inst, &q, &o1, eq, IP(0x1406c1920));
                        kq = 1;
                        for (j = 1; j <= nvp; j = j + 1) {
                            if (EQ(j) == p) {
                                kq = j;
                            }
                        }
                        const i32 id = ne + 1;
                        ne = id;
                        R8(table, id, 2) = p;
                        R8(table, id, 1) = CN(k2);
                        R8(table, id, 0) = CN(kc);
                        R8(table, id, 3) = kc;
                        R8(table, id, 4) = q;
                        R8(table, id, 5) = kq;
                        R8(table, id, 6) = 0;
                        EI(idx) = id;
                        EQ(*nvert + kq) = id;
                        o1 = 1;
                        fun_1402280d0(obj, inst, &q, &o1, eq, IP(0x1406c1920));
                    }
                    k = k + 1;
                    c = c + 1;
                } while (k <= *nvert);
            }
            i32 one2 = 1;
            ent = p;
            fun_1402280d0(obj, inst, &ent, &one2, ei, IP(0x1406bf990));
            p = p + 1;
        } while (p <= *npoly);
    }
    *nedge = ne;
}

} // namespace def::cmd::ctc3d
