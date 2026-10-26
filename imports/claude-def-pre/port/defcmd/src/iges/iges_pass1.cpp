// IGES pass 1 (packet p8-03): open/retry loop, D and P sections, parameter parsing.
// Spec: docs/re/commands/IGES_READER.md §4. Fortran conventions: iges_ftn.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include "ext_p4.hpp"
#include "ftn_read.hpp"
#include "iges_ftn.hpp"

#include <cstdio>
#include <cstring>
#include <string>

#include <defrt/ftn/fio.hpp>

namespace def::cmd::iges {

namespace {

constexpr int kUnit = 7; // DAT_140638c50

// IW record i field f (1-based)
inline std::int32_t& IW(std::int32_t* iw, std::int32_t i, std::int32_t f) { return iw[(i - 1) * 5 + (f - 1)]; }

// 1404b4108 with lengths 1/1: CHARACTER*1 equality
inline bool eq1(const char* a, char b) { return *a == b; }

// The value buffer of the P reader (DAT_1407227f0, V(1..5000)) and the counters behind it. SAVE
// data: values of an earlier record stay in V when a later record has fewer.
struct PState {
    double v[5001] = {};        // v[i] = V(i); v[0] overlaps the kind word 1407227e8
    std::int32_t cnt0 = 0;      // 14072c430
    char dummy = ' ';           // 14072c440
};
PState g_p;

// (A72,A1,I7) record: BUF, SEC, SEQ. Returns 0 ok, 1 error (letters in I7), 2 end of file.
int read_a72_a1_i7(char* buf, char* sec, std::int32_t* seq)
{
    std::string rec;
    if (ftnrd::read_record(kUnit, rec) == 2) return 2;
    const std::string b = ftnrd::field(rec, 1, 72);
    std::memcpy(buf, b.data(), 72);
    *sec = ftnrd::field(rec, 73, 1)[0];
    std::int32_t v = 0;
    if (!ftnrd::read_int(ftnrd::field(rec, 74, 7), &v)) return 1;
    *seq = v;
    return 0;
}

// The same READ without ERR= (only END=): a conversion error aborts.
int read_a72_a1_i7_end(char* buf, char* sec, std::int32_t* seq)
{
    const int r = read_a72_a1_i7(buf, sec, seq);
    if (r == 1) ftnrd::runtime_error("Illegal character in numeric input", "7");
    return r;
}

// READ(unit,'(nIw)') without END= / ERR=
void read_ints_unit(std::int32_t* out, int n, int w)
{
    std::string rec;
    if (ftnrd::read_record(kUnit, rec) == 2) ftnrd::runtime_error("Attempt to read past end of file", "7");
    for (int i = 0; i < n; ++i)
        if (!ftnrd::read_int(ftnrd::field(rec, 1 + i * w, w), &out[i]))
            ftnrd::runtime_error("Illegal character in numeric input", "7");
}

// Internal READ(buf,'(8I8,4I2)') without ERR=
void read_d_line1(const char* buf, std::int32_t* f, std::int32_t* s)
{
    const std::string rec(buf, 72);
    for (int i = 0; i < 8; ++i)
        if (!ftnrd::read_int(ftnrd::field(rec, 1 + i * 8, 8), &f[i]))
            ftnrd::runtime_error("Illegal character in numeric input", "INTERNAL");
    for (int i = 0; i < 4; ++i)
        if (!ftnrd::read_int(ftnrd::field(rec, 65 + i * 2, 2), &s[i]))
            ftnrd::runtime_error("Illegal character in numeric input", "INTERNAL");
}

// OPEN(7, FILE=name(1:len), STATUS='OLD', ACTION='READ', ERR=)
bool open7(const char* name, std::size_t len) { return ftnrd::open_old(kUnit, name, len); }

} // namespace

// @src 1404570f0  pass-1 entry: forwards to 1404579fc (name length 200)
void fun_1404570f0(char* nm, std::int32_t*, std::int32_t* na, std::int32_t* nb, std::int32_t* n100,
                   std::int32_t* n110, std::int32_t* n112, std::int32_t* n126, std::int32_t* n124,
                   double*, double* rw, std::int32_t* iw, std::int32_t*, std::int32_t*, std::int32_t* st)
{
    static std::int32_t dc0 = 0; // 140721dc0
    fun_1404579fc(nm, &dc0, rw, na, nb, iw, n100, n110, n112, n126, n124, st, 200);
}

// @src 1404579fc  open (retry with the _TMP_SFTC copy), find the D and P sections, sort, read P
void fun_1404579fc(char* nm, std::int32_t*, double* rw, std::int32_t* na, std::int32_t* nb, std::int32_t* iw,
                   std::int32_t* n100, std::int32_t* n110, std::int32_t* n112, std::int32_t* n126,
                   std::int32_t* n124, std::int32_t* st, std::int32_t nm_len)
{
    FtnState& g = g_ftn;
    static char buf[72];  // 140721f10
    static char sec = ' '; // 140721f60
    static std::int32_t seq = 0; // 140721f70
    g.de8 = 0;
    *st = 0;
    ext::char_assign(g.tmp, " ", 0x100, 1); // str 140bb2140
    ext::char_assign(g.suf, " ", 0x10, 1);  // str 140bb2150
    for (;;) {
        bool ok;
        if (g.de8 == 0) {
            ok = open7(nm, static_cast<std::size_t>(nm_len));
        } else {
            ok = open7(g.tmp, 0x100);
        }
        if (!ok) {
            *st = 1;
            return;
        }
        ftnrd::rewind(kUnit); // 1404b5a10
        bool to_p = false;
        for (;;) {
            const int r = read_a72_a1_i7(buf, &sec, &seq);
            if (r == 1) {
                *st = 1; // unit 7 stays open, as in the original
                return;
            }
            if (r == 2) break;
            if (eq1(&sec, 'D') || eq1(&sec, 'd')) { // strs 140bb2180, 140bb2190
                fun_140460f50(buf, &sec, iw, nb, n110, n100, n124, n112, n126, &g.f74, kUnit);
                if (eq1(&sec, 'P') || eq1(&sec, 'P')) { // strs 140bb21c0, 140bb21d0
                    to_p = true;
                    break;
                }
            } else if (eq1(&sec, 'P') || eq1(&sec, 'P')) { // strs 140bb21a0, 140bb21b0
                to_p = true;
                break;
            }
        }
        if (to_p) {
            const std::int32_t key = 2, asc = 1, ncol = 5;
            fun_140468677(iw, &key, nb, &asc, &ncol);
            fun_140461b95(buf, &sec, rw, iw, nb, na, kUnit);
            ftnrd::close(kUnit);
            return;
        }
        ftnrd::close(kUnit);
        if (g.de8 != 0) {
            *st = 2;
            return;
        }
        g.f78 = 0;
        const std::int32_t l = ext::adjust_left(nm, 200);
        const std::int32_t l1 = l + 1;
        const std::int32_t lc = l < 0 ? 0 : l;
        ext::char_assign(g.tmp, nm, lc, lc);
        g.tmp[l1 - 1] = '\0';
        ext::char_assign(g.suf, "_TMP_SFTC", 0x10, 9);
        const std::int32_t ls = ext::adjust_left(g.suf, 0x10);
        g.suf[ls] = '\0';
        fun_140459c0d(g.tmp, g.suf, &g.f8c, &g.f78);
        if (g.f78 != 0) {
            *st = 2;
            return;
        }
        ext::char_assign(g.tmp, " ", 0x100, 1); // str 140bb21f0
        ext::char_assign(g.tmp, nm, lc, lc);
        ext::char_assign(g.tmp + l1 - 1, g.suf, ls + 1 < 0 ? 0 : ls + 1, ls < 0 ? 0 : ls);
        g.de8 = 1;
    }
}

// @src 140459c0d  count = st = 0, then the C copy 1401b9100 (the lengths are not used)
void fun_140459c0d(char* src, char* suffix, std::int32_t* count, std::int32_t* st)
{
    *st = 0;
    *count = 0;
    fun_1401b9100(src, suffix, count, st);
}

// @src 140460f50  D section: one 5-int record per kept entity (types 100 110 -102 124 112 126)
void fun_140460f50(char* buf, char* sec, std::int32_t* iw, std::int32_t* nb, std::int32_t* n110,
                   std::int32_t* n100, std::int32_t* n124, std::int32_t* n112, std::int32_t* n126,
                   std::int32_t* n102, std::int32_t)
{
    static std::int32_t first = 0;   // 140722790
    static std::int32_t f[8] = {};   // 140722794.. (f[0] is overwritten by the type of line 2)
    static std::int32_t s4[4] = {};  // 1407227b4..
    static std::int32_t l2[4] = {};  // 1407227c4.. rest of line 2
    static std::int32_t cnt = 0;     // 1407227d4 D records read
    static std::int32_t seq = 0;     // 1407227d8
    first = 1;
    *nb = 0;
    *n112 = 0;
    *n124 = 0;
    *n110 = 0;
    *n100 = 0;
    *n126 = 0;
    for (;;) {
        if (first == 1) {
            read_d_line1(buf, f, s4);
            std::int32_t t[5];
            read_ints_unit(t, 5, 8);
            f[0] = t[0];
            l2[0] = t[1];
            l2[1] = t[2];
            l2[2] = t[3];
            l2[3] = t[4];
            cnt = 2;
            first = 0;
        } else {
            if (read_a72_a1_i7_end(buf, sec, &seq) == 2) return;
            cnt = cnt + 1;
            if (!eq1(sec, 'D') && !eq1(sec, 'd')) return; // strs 140bb2270, 140bb2280
            read_d_line1(buf, f, s4);
            std::int32_t t[5];
            read_ints_unit(t, 5, 8);
            f[0] = t[0];
            l2[0] = t[1];
            l2[1] = t[2];
            l2[2] = t[3];
            l2[3] = t[4];
            cnt = cnt + 1;
        }
        const std::int32_t type = f[0];
        const std::int32_t ptr = f[1];
        const std::int32_t trf = f[6];
        if (type == 100) {
            *nb = *nb + 1;
            *n100 = *n100 + 1;
            IW(iw, *nb, 3) = trf;
            IW(iw, *nb, 2) = ptr;
            IW(iw, *nb, 1) = 1;
        } else if (type == 110) {
            *nb = *nb + 1;
            *n110 = *n110 + 1;
            IW(iw, *nb, 3) = trf;
            IW(iw, *nb, 2) = ptr;
            IW(iw, *nb, 1) = 2;
        } else if (type == -102) {
            *nb = *nb + 1;
            *n102 = *n102 + 1;
            IW(iw, *nb, 3) = trf;
            IW(iw, *nb, 2) = ptr;
            IW(iw, *nb, 1) = 3;
        } else if (type == 124) {
            *nb = *nb + 1;
            *n124 = *n124 + 1;
            IW(iw, *nb, 3) = cnt - 1;
            IW(iw, *nb, 2) = ptr;
            IW(iw, *nb, 1) = 4;
        } else if (type == 112 || type == 126) {
            *nb = *nb + 1;
            IW(iw, *nb, 1) = 5;
            if (type == 126) {
                IW(iw, *nb, 1) = 6;
                *n126 = *n126 + 1;
            } else {
                *n112 = *n112 + 1;
            }
            IW(iw, *nb, 2) = ptr;
            IW(iw, *nb, 3) = trf;
        }
    }
}

// @src 140468677  bubble sort of n rows of a(ncol, n) by column key (asc 1: ascending)
void fun_140468677(std::int32_t* a, const std::int32_t* key, const std::int32_t* n, const std::int32_t* asc,
                   const std::int32_t* ncol)
{
    static std::int32_t tmp[64] = {}; // 14072d8ec.. (the swap buffer, column by column)
    const std::int32_t nc = *ncol;
    const std::int32_t nr = *n;
    const std::int64_t ld = nc < 0 ? 0 : nc;
    auto at = [&](std::int32_t col, std::int32_t row) -> std::int32_t& {
        return a[(static_cast<std::int64_t>(row) - 1) * ld + col - 1];
    };
    for (std::int32_t i = 1; i <= nr - 1; ++i) {
        for (std::int32_t j = nr; j >= i + 1; --j) {
            const std::int32_t kj1 = at(*key, j - 1);
            const std::int32_t kj = at(*key, j);
            const bool swap = (*asc == 1) ? (kj < kj1) : (kj1 < kj);
            if (swap) {
                for (std::int32_t c = 1; c <= nc; ++c) {
                    const std::int32_t t = at(c, j - 1);
                    at(c, j - 1) = at(c, j);
                    at(c, j) = t;
                    if (c < 64) tmp[c] = t;
                }
            }
        }
    }
}

// @src 140461b95  P section: values of every sorted record into RW slots; f4 = first slot
void fun_140461b95(char* buf, char*, double* rw, std::int32_t* iw, std::int32_t* nb, std::int32_t* na,
                   std::int32_t unit)
{
    PState& p = g_p;
    double* v = p.v; // v[k] = V(k)
    std::int32_t rd = 0;    // 1407227dc
    std::int32_t i = 1;     // 1407227e0 record
    std::int32_t cur = 1;   // 1407227ec P record last read
    std::int32_t ptr = IW(iw, 1, 2);  // 1407227e4
    std::int32_t kind = IW(iw, 1, 1); // 1407227e8
    std::int32_t want = 0, got = 0;   // 14072c450, 14072c454
    *na = 0;
    if (ptr == 1) {
        fun_140464245(buf, &v[1], &p.cnt0);
    } else if (1 < ptr) {
        rd = 1;
    }
    for (;;) {
        for (std::int32_t k = cur + 1; k <= ptr - 1; ++k) { // 14072c434
            cur = cur + 1;
            std::string rec;
            if (ftnrd::read_record(unit, rec) == 2) ftnrd::runtime_error("Attempt to read past end of file", "7");
            p.dummy = ftnrd::field(rec, 1, 1)[0]; // READ(7,'(A1)')
        }
        if (kind == 2) {
            const std::int32_t n0 = *na;
            IW(iw, i, 4) = n0 + 1;
            if (rd == 0) {
                want = -(p.cnt0 - 7);
                if (0 < want) fun_140463e15(&v[p.cnt0 + 1], &got, &cur, &want, unit);
                rd = 1;
            } else {
                want = 7;
                fun_140463e15(&v[1], &got, &cur, &want, unit);
            }
            const std::int32_t s1 = *na + 1;
            const std::int32_t s2 = *na + 2;
            for (std::int32_t c = 1; c <= 3; ++c) {
                rw[(s1 - 1) * 3 + c - 1] = v[1 + c];
                rw[(s2 - 1) * 3 + c - 1] = v[4 + c];
            }
            *na = *na + 2;
        } else if (kind == 1) {
            const std::int32_t n0 = *na;
            IW(iw, i, 4) = n0 + 1;
            if (rd == 0) {
                want = -(p.cnt0 - 8);
                if (0 < want) fun_140463e15(&v[p.cnt0 + 1], &got, &cur, &want, unit);
                rd = 1;
            } else {
                want = 8;
                fun_140463e15(&v[1], &got, &cur, &want, unit);
            }
            const double zt = v[2];
            const std::int32_t s = *na;
            rw[(s + 0) * 3 + 0] = v[5];
            rw[(s + 0) * 3 + 1] = v[6];
            rw[(s + 0) * 3 + 2] = zt;
            rw[(s + 1) * 3 + 0] = v[3];
            rw[(s + 1) * 3 + 1] = v[4];
            rw[(s + 1) * 3 + 2] = zt;
            rw[(s + 2) * 3 + 0] = v[7];
            rw[(s + 2) * 3 + 1] = v[8];
            rw[(s + 2) * 3 + 2] = zt;
            *na = s + 3;
        } else if (kind == 3) {
            // composite curve (type -102: never stored for a real file)
            const std::int32_t n0 = *na;
            IW(iw, i, 4) = n0 + 1;
            if (rd == 1) {
                want = 0;
                fun_140463e15(&v[1], &p.cnt0, &cur, &want, unit);
            }
            static std::int32_t c45c = 0, c460 = 0;
            c45c = ftn_nint(v[2]);
            want = (c45c - p.cnt0) + 2;
            if (want < 1) {
                got = p.cnt0;
            } else {
                fun_140463e15(&v[p.cnt0 + 1], &got, &cur, &want, unit);
                got = got - 1 + p.cnt0;
            }
            const std::int32_t dir = 1;
            rd = 1;
            fun_140462631(&v[2], &rw[n0 * 3], &got, &c460, &dir);
            *na = *na + c460;
        } else if (kind == 4) {
            const std::int32_t n0 = *na;
            IW(iw, i, 4) = n0 + 1;
            if (rd == 0) {
                want = -(p.cnt0 - 13);
                if (0 < want) fun_140463e15(&v[p.cnt0 + 1], &got, &cur, &want, unit);
                rd = 1;
            } else {
                want = 13;
                fun_140463e15(&v[1], &got, &cur, &want, unit);
            }
            fun_14046414e(&v[2], &rw[n0 * 3]);
            *na = *na + 4;
        } else if (kind == 5) {
            fun_1404627bf(&rw[*na * 3], &v[1], na, &cur, unit, &IW(iw, i, 1), &p.cnt0, &rd);
        } else if (kind == 6) {
            fun_140462b00(&rw[*na * 3], &v[1], na, &cur, unit, &IW(iw, i, 1), &p.cnt0, &rd);
        }
        i = i + 1;
        ptr = IW(iw, i, 2);
        kind = IW(iw, i, 1);
        if (*nb < i) {
            IW(iw, i, 4) = *na + 1;
            return;
        }
    }
}

// @src 140462631  copy n doubles a -> b (dir 1; *nslots = ceil(n/3)) or b -> a (dir 0, n = b(1))
void fun_140462631(double* a, double* b, std::int32_t* n, std::int32_t* nslots, const std::int32_t* dir)
{
    if (*dir == 1) {
        for (std::int32_t i = 1; i <= *n; ++i) b[i - 1] = a[i - 1];
        const std::int32_t q = *n / 3;
        *nslots = q + 1;
        if (*n == q * 3) *nslots = q;
    } else {
        const std::int32_t m = static_cast<std::int32_t>(b[0]) + 1;
        *n = static_cast<std::int32_t>(b[0]);
        for (std::int32_t i = 1; i <= m; ++i) a[i - 1] = b[i - 1];
    }
}

// @src 1404627bf  IGES 112 (parametric spline) parameter block
void fun_1404627bf(double* blk, double* v, std::int32_t* na, std::int32_t* cur, std::int32_t unit,
                   std::int32_t* rec, std::int32_t* cnt, std::int32_t* rd)
{
    std::int32_t want = 0; // 14072c46c
    if (*rd == 0) {
        *rd = 1;
    } else {
        want = 0;
        fun_140463e15(v, cnt, cur, &want, unit);
    }
    const std::int32_t n = ftn_nint(v[4]); // 14072c470 = NINT(V(5))
    const std::int32_t c0 = *cnt;
    rec[3] = *na + 1;
    want = (n * 13 - *cnt) + 6;
    fun_140463e15(&v[c0], cnt, cur, &want, unit);
    const std::int32_t n1 = n + 1;
    const std::int32_t m = n * 4 + n1 / 3;
    blk[0] = static_cast<double>(n);
    rec[3] = *na + 1;
    blk[1] = static_cast<double>(m + 2);
    if (0 < n1) {
        for (std::int32_t k = 1; k <= n1; ++k) blk[k + 2] = v[k + 4]; // b(k+3) = V(k+5)
        std::int32_t pb = (n1 / 3) * 3 + 7; // 14072c478 position in b
        std::int32_t pv = n + 7;            // 14072c47c position in V
        for (std::int32_t s = 1; s <= n1; ++s) {
            fun_14046414e(&v[pv - 1], &blk[pb - 1]);
            pv = pv + 12;
            pb = pb + 12;
        }
    }
    *na = *na + 2 + m;
}

// @src 140462b00  IGES 126 (rational B-spline) parameter block
void fun_140462b00(double* blk, double* v, std::int32_t* na, std::int32_t* cur, std::int32_t unit,
                   std::int32_t* rec, std::int32_t* cnt, std::int32_t* rd)
{
    std::int32_t want = 0; // 14072c480
    if (*rd == 0) {
        *rd = 1;
    } else {
        want = 0;
        fun_140463e15(v, cnt, cur, &want, unit);
    }
    const std::int32_t k = ftn_nint(v[1]); // 14072c484
    const std::int32_t m = ftn_nint(v[2]); // 14072c488
    rec[3] = *na + 1;
    const std::int32_t km = k + m;
    const std::int32_t s = km + k * 4;
    want = (s - *cnt) + 0xf;
    fun_140463e15(&v[*cnt], cnt, cur, &want, unit);
    blk[0] = static_cast<double>(k);
    blk[5] = static_cast<double>(s + 0xd);
    blk[3] = static_cast<double>(km + 9);
    blk[1] = static_cast<double>(m);
    blk[4] = static_cast<double>(k + 10 + km);
    const std::int32_t n8 = s + 9;
    for (std::int32_t j = 1; j <= n8; ++j) blk[j + 5] = v[j + 6]; // b(j+6) = V(j+7)
    *na = *na + 1 + (s + 0xf) / 3;
}

// @src 140463e15  INPSEC: read records until `want` values are in dest(1..); EOF stops the program
void fun_140463e15(double* dest, std::int32_t* got, std::int32_t* cur, const std::int32_t* want, std::int32_t)
{
    static char rec[72];          // 14072c780
    static char sec = ' ';        // 14072c7d0
    static std::int32_t seq = 0;  // 14072c7e0
    std::int32_t k = 1;           // 14072c770
    std::int32_t n = 0;           // 14072c774
    for (;;) {
        if (read_a72_a1_i7_end(rec, &sec, &seq) == 2) {
            def::rt::ftn::fio::Write(6).a("INPSEC : STOPPING").end();
            ftnrd::stop(); // STOP (1404b5acc with ' ')
        }
        *cur = *cur + 1;
        fun_140464245(rec, &dest[k - 1], &n);
        const std::int32_t w = *want;
        k = n + k;
        *got = k - 1;
        if (!(k - 1 < w)) return;
    }
}

// @src 14046414e  4x3 block: slot r component c = v((c-1)*4 + r)
void fun_14046414e(const double* v, double* slots)
{
    for (std::int32_t r = 1; r <= 4; ++r)
        for (std::int32_t c = 1; c <= 3; ++c) slots[(r - 1) * 3 + c - 1] = v[(c - 1) * 4 + r - 1];
}

// @src 140464245  values of one record: READ(rec,'(A72)') L, fields between ',' / ';'
void fun_140464245(const char* rec, double* vals, std::int32_t* n)
{
    static char l[72]; // 14072c7f0
    std::memcpy(l, rec, 72);
    std::int32_t start = 1; // 14072c840
    *n = 0;
    for (std::int32_t pos = 1; pos <= 72; ++pos) { // 14072c844
        const std::int32_t prev = pos == 1 ? 72 : pos - 1; // 14072c848
        const char c = l[pos - 1];
        if (c == ',' || c == ';') { // strs 140bb2300, 140bb2310
            *n = *n + 1;
            if (l[prev - 1] == ',') { // str 140bb2320
                vals[*n - 1] = 0.0;
                start = pos + 1;
            } else {
                const std::int32_t len = pos - start;
                const std::int32_t slen = (pos - 1) - start + 1;
                std::int32_t err = 0; // 14072c858
                fun_140468960(&l[start - 1], &len, &g_ftn.c850, &err, slen < 0 ? 0 : slen);
                if (err == 1) return;
                vals[*n - 1] = g_ftn.c850;
                start = pos + 1;
            }
        }
    }
}

// @src 140468960  value of one field s(1:len): FMT = '(E<len>.0)', READ(s, FMT, ERR=) value
void fun_140468960(const char* s, const std::int32_t* len, double* value, std::int32_t* err, std::int32_t slen)
{
    static char str[0x50]; // 14072d920 CHARACTER*80
    *err = 0;
    ext::char_assign(str, s, 0x50, slen);
    if (*len < 1) return;
    if (*len < 2) {
        if (str[0] == 'E' || str[0] == 'e') { // strs 140bb2370, 140bb2380
            *value = 0.0;
            return;
        }
        if (static_cast<unsigned char>(str[0]) < 0x30 || 0x39 < static_cast<unsigned char>(str[0])) {
            *err = 1;
            return;
        }
    }
    // WRITE(FMT(3:4),'(I2)') len: fails only for len > 99 (not possible, len <= 72)
    const std::int32_t w = *len;
    if (99 < w) {
        *err = 1;
        return;
    }
    const std::string f(str, static_cast<std::size_t>(w < 0x50 ? w : 0x50));
    double x = 0.0;
    if (!ftnrd::read_real_e0(f, &x)) {
        *err = 1;
        return;
    }
    *value = x;
}

void reset_pass1_state() { g_p = PState{}; }

} // namespace def::cmd::iges
