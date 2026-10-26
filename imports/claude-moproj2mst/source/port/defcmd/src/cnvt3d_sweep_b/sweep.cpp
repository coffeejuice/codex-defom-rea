// p9-03b: the in-process brick sweeps (CNVT3D_SWEEP.md 3, 4, 5).
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <list>
#include <vector>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/libm.hpp>

#include "common.hpp"
#include "ext.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;

namespace {

constexpr double kZero = 0.0;                  // DAT_14051bed8
constexpr double k360 = 360.0;                 // DAT_14051f210
constexpr double kPi = 3.141592653589793;      // DAT_14051f230
constexpr double k180 = 180.0;                 // DAT_14051f228
constexpr double k09 = 0.9;                    // DAT_14051f220

// C remainder of k by 4 as the binary computes it (k >= 0 here)
inline std::int32_t mod4(std::int32_t k) { return k % 4; }

} // namespace

// @src 140148f70  revolve sweep: n (even) sections over min(ang, 360), axis nodes (r < c) merged
// into layer 0, axis elements as one hexahedron per section pair, NBCD.OUT for a partial revolve,
// then 14014c730
void fun_140148f70(MeshGenBase* cvt, std::int32_t nsec, double start, double ang, double c,
                   LLOBJ_DATA* obj)
{
    ext::g.fun_140155340(cvt, obj, cvt_c(cvt));
    dbio::gendb::remove_if_exists("NBCD.OUT");  // 1401b85d0
    std::int32_t s = 0;                         // local_2fc
    char no_axis = 1;                           // local_300
    double ang_e = ang;
    if (k360 < ang) ang_e = k360;
    std::int32_t n = nsec;
    if (n == 0) n = 2;
    if (n % 2 == 1) n = n + 1;
    const double dth = ((ang_e * kPi) / k180) / static_cast<double>(n);
    const double th0 = (start * kPi) / k180;
    const std::int32_t NN = obj_nn(obj);
    const std::int32_t NE = obj_ne(obj);
    std::vector<double> P(static_cast<std::size_t>((n + 1) * NN) * 3);           // local_2d8
    std::vector<std::int32_t> E(static_cast<std::size_t>(n * NE) * 8);           // local_2e0
    std::vector<std::int32_t> axis(static_cast<std::size_t>(NN));                // local_2f0
    std::vector<char> scaled(static_cast<std::size_t>((n + 1) * NN));             // local_2d0
    std::list<std::int32_t> keep, unused, node2d, elem2d;  // local_2e8, local_308, local_2c0, local_2f8
    auto X = [&](std::int32_t i) -> double& { return P[static_cast<std::size_t>(i) * 3]; };
    auto Y = [&](std::int32_t i) -> double& { return P[static_cast<std::size_t>(i) * 3 + 1]; };
    auto Zc = [&](std::int32_t i) -> double& { return P[static_cast<std::size_t>(i) * 3 + 2]; };

    for (std::int32_t p = 1; p <= NN; ++p) {
        const double r = coords(obj)[p * 2 - 2];
        if (c < r || c == r) {
            axis[p - 1] = 0;
            X(p - 1) = coords(obj)[p * 2 - 2] * rt::libm::crt_cos(th0);  // 140bb3490
            Y(p - 1) = coords(obj)[p * 2 - 2] * rt::libm::crt_sin(th0);  // 140bb3000
        } else {
            axis[p - 1] = 1;
            X(p - 1) = 0.0;
            Y(p - 1) = 0.0;
            no_axis = 0;
        }
        Zc(p - 1) = coords(obj)[p * 2 - 1];
        keep.push_back(p - 1);
        node2d.push_back(p);
        for (std::int32_t L = 1; L <= n + 1; ++L) scaled[NN * (L - 1) - 1 + p] = 0;
    }
    for (s = 1; s <= n; ++s) {
        for (std::int32_t p = 1; p <= NN; ++p) {
            const std::int32_t q = s * NN - 1 + p;
            if (X(p - 1) == kZero && Y(p - 1) == kZero) {
                X(q) = 0.0;
                Y(q) = 0.0;
            } else {
                X(q) = coords(obj)[p * 2 - 2] * rt::libm::crt_cos(dth * static_cast<double>(s) + th0);
                Y(q) = coords(obj)[p * 2 - 2] * rt::libm::crt_sin(dth * static_cast<double>(s) + th0);
            }
            Zc(q) = Zc(p - 1);
            if (axis[p - 1] == 0) {
                if (k360 <= ang_e) {
                    if (s < n) {
                        keep.push_back(q);
                        node2d.push_back(p);
                    }
                } else {
                    keep.push_back(q);
                    node2d.push_back(p);
                }
            }
        }
        for (std::int32_t e = 1; e <= NE; ++e) {
            const std::int32_t base = s * NN - 1;
            auto cn = [&](std::int32_t k) { return conn(obj)[e * 4 - 4 + k]; };  // 140183cb0
            auto on = [&](std::int32_t nd) { return X(base + nd) == kZero && Y(base + nd) == kZero; };
            std::int32_t* Ee = &E[static_cast<std::size_t>((s - 1) * NE - 1 + e) * 8];
            if (no_axis == 0 &&
                (on(cn(3)) || on(cn(2)) || on(cn(1)) ||
                 (X(base + cn(0)) == kZero && X(base + cn(0)) == kZero))) {  // x tested twice
                if (s % 2 == 1) {
                    for (std::int32_t i = 0; i < 8; ++i) Ee[i] = 0;
                    continue;
                }
                std::int32_t k = 0;
                while (on(cn(mod4(k))) || X(base + cn(mod4(k + 1))) != kZero ||
                       Y(base + cn(mod4(k + 1))) != kZero) {
                    k = k + 1;
                }
                for (std::int32_t j = 0; j < 4; ++j) {
                    const std::int32_t i = base + cn(mod4(k + j));
                    if (scaled[i] == 0 && (X(i) != kZero || Y(i) != kZero)) {
                        X(i) = X(i) * k09;
                        Y(i) = Y(i) * k09;
                        scaled[i] = 1;
                    }
                }
                if (s == n) {
                    for (std::int32_t j : {0, 3}) {
                        const std::int32_t i = cn(mod4(k + j)) - 1;
                        if (scaled[i] == 0) {
                            X(i) = X(i) * k09;
                            Y(i) = Y(i) * k09;
                            scaled[i] = 1;
                        }
                    }
                }
                Ee[0] = (s - 1) * NN + cn(mod4(k));
                Ee[1] = s * NN + cn(mod4(k));
                Ee[2] = s * NN + cn(mod4(k + 3));
                Ee[3] = (s - 1) * NN + cn(mod4(k + 3));
                for (std::int32_t j = 0; j < 4; ++j) Ee[4 + j] = (s - 2) * NN + cn(mod4(k + j));
                for (std::int32_t i = 0; i < 8; ++i) {
                    if (X(Ee[i] - 1) == kZero && Y(Ee[i] - 1) == kZero) Ee[i] = Ee[i] % NN;
                    if (Ee[i] == 0) Ee[i] = NN;
                }
                elem2d.push_back(e);
            } else {
                for (std::int32_t i = 0; i < 4; ++i) Ee[i] = (s - 1) * NN + cn(3 - i);
                for (std::int32_t i = 0; i < 4; ++i) Ee[4 + i] = s * NN + cn(3 - i);
                elem2d.push_back(e);
            }
        }
    }
    if (k360 <= ang_e) {
        for (s = (n - 1) * NE; s <= n * NE; ++s) {
            for (std::int32_t i = 0; i < 8; ++i) {
                std::int32_t& v = E[static_cast<std::size_t>(s - 1) * 8 + i];
                if (n * NN < v) v = (v % NN == 0) ? NN : v % NN;
            }
        }
    }
    if (ang_e < k360) {
        std::FILE* f = std::fopen("NBCD.OUT", "w");  // 1401b1880 / 1401b18f0, mode str 14051e11c
        if (f != nullptr) {
            // 1401b1bd0 wraps the FILE in an ostream; every line ends with 140007a50 (put('\n'),
            // flush); doubles go through num_put with left | scientific and precision 8, which
            // the VS2008 CRT formats as "%.8e" (3-digit exponent)
            std::int32_t a = 0;
            for (s = 1; s <= NN; ++s) a = a + axis[s - 1];
            const std::int32_t cnt = (NN - a) + NN;
            std::fprintf(f, "\t%d\n", cnt);
            std::fflush(f);
            for (s = 1; s <= NN; ++s) {
                const std::int32_t i = s - 1;
                if (X(i) == kZero && Y(i) == kZero) {
                    rt::crt::fprintf(f, "\t%.8e\t%.8e\t%.8e\t0\t602\t601\t0\t%d\n", X(i), Y(i), Zc(i), s);
                    std::fflush(f);
                } else {
                    rt::crt::fprintf(f, "\t%.8e\t%.8e\t%.8e\t0\t0\t601\t0\t%d\n", X(i), Y(i), Zc(i), s);
                    std::fflush(f);
                    const std::int32_t j = NN * n - 1 + s;
                    rt::crt::fprintf(f, "\t%.8e\t%.8e\t%.8e\t0\t0\t602\t0\t%d\n", X(j), Y(j), Zc(j), NN * n + s);
                    std::fflush(f);
                }
            }
            std::fclose(f);
        }
    }
    fun_14014c730(cvt, obj, no_axis, th0, dth, n, P.data(), E.data(), axis.data(), keep, unused, node2d,
                  elem2d);
}

// @src 14014c0a0  extrude sweep: nsec node layers z = start + dz*(s-1), elements in 2D order
void fun_14014c0a0(MeshGenBase* cvt, std::int32_t nsec, double start, double len, LLOBJ_DATA* obj)
{
    if (1 < nsec) {
        len = len / static_cast<double>(nsec - 1);
        const std::int32_t NN = obj_nn(obj);
        const std::int32_t NE = obj_ne(obj);
        std::vector<double> X(static_cast<std::size_t>(nsec * NN * 3));
        std::vector<std::int32_t> C(static_cast<std::size_t>((nsec - 1) * NE * 8));
        for (std::int32_t s = 1; s <= nsec; ++s) {
            for (std::int32_t p = 1; p <= obj_nn(obj); ++p) {
                const std::size_t i = static_cast<std::size_t>((s - 1) * obj_nn(obj) - 1 + p) * 3;
                X[i] = coords(obj)[p * 2 - 2];
                X[i + 1] = coords(obj)[p * 2 - 1];
                X[i + 2] = start + len * static_cast<double>(s - 1);
            }
            if (1 < s) {
                for (std::int32_t e = 1; e <= obj_ne(obj); ++e) {
                    std::int32_t* c8 = &C[static_cast<std::size_t>(((s - 2) * obj_ne(obj) + e) * 8 - 8)];
                    for (std::int32_t j = 0; j < 4; ++j) c8[j] = (s - 2) * obj_nn(obj) + conn(obj)[e * 4 - 4 + j];
                    for (std::int32_t j = 0; j < 4; ++j) c8[4 + j] = (s - 1) * obj_nn(obj) + conn(obj)[e * 4 - 4 + j];
                }
            }
        }
        fun_14014fae0(cvt, nsec * obj_nn(obj), (nsec - 1) * obj_ne(obj), X.data(), C.data(), obj, len);
    }
}

// @src 14014c730  compaction: with axis nodes, drop the empty elements, number the kept nodes in
// list order and map the connectivity; without, pass the arrays through; then 14014d730
void fun_14014c730(MeshGenBase* cvt, LLOBJ_DATA* obj, char no_axis, double th0, double dth,
                   std::int32_t n, double* P, std::int32_t* E, std::int32_t* axis,
                   std::list<std::int32_t> keep, std::list<std::int32_t> /*unused*/,
                   std::list<std::int32_t> node2d, std::list<std::int32_t> elem2d)
{
    const std::int32_t NN = obj_nn(obj);
    const std::int32_t NE = obj_ne(obj);
    if (no_axis == 0) {
        const std::int32_t nE = n * NE;
        std::vector<std::int32_t> valid(static_cast<std::size_t>(nE));
        std::vector<std::int32_t> map(static_cast<std::size_t>((n + 1) * NN));
        std::vector<std::int32_t> newE(static_cast<std::size_t>(nE));
        for (std::int32_t i = 1; i <= (n + 1) * NN; ++i) map[i - 1] = 0;
        for (std::int32_t i = 1; i <= nE; ++i) newE[i - 1] = 0;
        for (std::int32_t i = 1; i <= nE; ++i) {
            const std::int32_t* e = &E[static_cast<std::size_t>(i - 1) * 8];
            valid[i - 1] = (e[0] + e[1] + e[2] + e[3] + e[4] + e[5] + e[6] + e[7] < 1) ? 0 : 1;
            newE[i - 1] = (i < 2) ? valid[i - 1] : newE[i - 2] + valid[i - 1];
        }
        std::int32_t a = 0;
        for (std::int32_t p = 1; p <= NN; ++p) a = a + axis[p - 1];
        const std::int32_t nn3 = static_cast<std::int32_t>(keep.size());  // 140159ab0
        const std::int32_t ne3 = newE[nE - 1];
        std::vector<double> X3(static_cast<std::size_t>(nn3) * 3);
        std::vector<std::int32_t> C3(static_cast<std::size_t>(ne3) * 8);
        std::vector<std::int32_t> N2(node2d.size());
        std::vector<std::int32_t> E2(elem2d.size());
        for (std::int32_t j = 1; j <= nn3; ++j) {
            const std::int32_t v = keep.front();
            X3[(j - 1) * 3] = P[v * 3];
            X3[(j - 1) * 3 + 1] = P[v * 3 + 1];
            X3[(j - 1) * 3 + 2] = P[v * 3 + 2];
            map[v] = j;
            keep.push_back(keep.front());
            keep.pop_front();
        }
        for (std::int32_t j = 1; !node2d.empty(); ++j) {
            N2[j - 1] = node2d.front();
            node2d.pop_front();
        }
        for (std::int32_t i = 1; i <= nE; ++i) {
            if (valid[i - 1] == 1) {
                const std::int32_t b = newE[i - 1];
                for (std::int32_t k = 0; k < 8; ++k)
                    C3[static_cast<std::size_t>(b * 8 - 8 + k)] = map[E[static_cast<std::size_t>(i - 1) * 8 + k] - 1];
            }
        }
        for (std::int32_t j = 1; !elem2d.empty(); ++j) {
            E2[j - 1] = elem2d.front();
            elem2d.pop_front();
        }
        fun_14014d730(cvt, nn3, ne3, X3.data(), C3.data(), a, th0, dth, N2.data(), E2.data());
    } else {
        const std::int32_t nn3 = static_cast<std::int32_t>(keep.size());
        const std::int32_t ne3 = static_cast<std::int32_t>(elem2d.size());
        std::vector<double> X3(static_cast<std::size_t>(nn3) * 3);
        std::vector<std::int32_t> C3(static_cast<std::size_t>(ne3) * 8);
        std::vector<std::int32_t> N2(static_cast<std::size_t>(nn3));
        std::vector<std::int32_t> E2(static_cast<std::size_t>(ne3));
        for (std::int32_t j = 1; j <= nn3; ++j) {
            X3[(j - 1) * 3] = P[(j - 1) * 3];
            X3[(j - 1) * 3 + 1] = P[(j - 1) * 3 + 1];
            X3[(j - 1) * 3 + 2] = P[(j - 1) * 3 + 2];
            N2[j - 1] = node2d.front();
            node2d.pop_front();
        }
        for (std::int32_t j = 1; j <= ne3; ++j) {
            for (std::int32_t k = 0; k < 8; ++k)
                C3[static_cast<std::size_t>(j * 8 - 8 + k)] = E[static_cast<std::size_t>(j - 1) * 8 + k];
            E2[j - 1] = elem2d.front();
            elem2d.pop_front();
        }
        fun_14014d730(cvt, nn3, ne3, X3.data(), C3.data(), 0, th0, dth, N2.data(), E2.data());
    }
}

} // namespace def::cmd::cnvt3d_sweep
