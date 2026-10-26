// p9-03b: the 3D object of a sweep and its state variables (CNVT3D_SWEEP.md 6.2, 6.3) and the tet
// path's copy of the interpolated values (CNVT3D.md 6.3, 140151400).
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstring>
#include <vector>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defcmd/geolib.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/stdlib/libm.hpp>

#include "common.hpp"
#include "ext.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;

namespace {

constexpr double kPi = 3.141592653589793;   // DAT_14051f230
constexpr double k180 = 180.0;              // DAT_14051f228
constexpr double kPi2 = 1.5707963267948966; // DAT_14051f238
constexpr double kHalf = 0.5;               // DAT_14051f240
constexpr double kTwo = 2.0;                // DAT_140509fb8
constexpr double kOne = 1.0;                // DAT_140509c90
constexpr double kFour = 4.0;               // DAT_14051f248
constexpr double kZero = 0.0;               // DAT_14051bed8

inline double cosd(double x) { return rt::libm::crt_cos(x); }  // 140bb3490
inline double sind(double x) { return rt::libm::crt_sin(x); }  // 140bb3000

// 1401d6d90: GEOTYP of the model (+0x850)
inline std::int32_t geotyp(DB_STEP_DATA* m) { return at<std::int32_t>(m, 0x850); }

// the accessors of the per-row copies: 1401815b0 (0x11), 140184a30 (0x3c), 1401842f0 (0x31),
// 140184370 (0x32), 140183e50 (0x28)
double* special_array(LLOBJ_DATA* o, std::int32_t id)
{
    switch (id) {
    case 0x11: return fun_1401815b0(node_set(o));
    case 0x3c: return fun_140184a30(elem_set(o));
    case 0x31: return elem_sv(elem_set(o), 0x31, 0x78);  // 1401842f0
    case 0x32: return fun_140184370(elem_set(o));
    default: return elem_sv(elem_set(o), 0x28, 0x30);    // 140183e50
    }
}

// the STNCMP block offsets (140166200 with the tags 3000 .. 0xdac, block 6 + 140212460(src, 6))
void stncmp_block(LLOBJ_DATA* src, std::int32_t b, std::int32_t& kind, std::int32_t& o2, std::int32_t& o3)
{
    std::int32_t tag;
    switch (b) {
    case 0: tag = 3000; break;
    case 1: tag = 0xc1c; break;
    case 2: tag = 0xc80; break;
    case 3: tag = 0xce4; break;
    case 4: tag = 0xd48; break;
    case 5:
        kind = 1;
        tag = 0xdac;
        break;
    default:
        kind = 1;
        tag = 0xdac;
        break;
    }
    o2 = model::p407::strain_block_offset(src, &tag);
    o3 = o2 * 6;
    o2 = o2 << 2;
    if (b == 6) {
        const std::int32_t x = ext::g.fun_140212460(src, 6);
        o2 = o2 + x;
        o3 = o3 + x;
    }
}

// q2/q3 of STRESS (j 2), STRAIN (1), DAMAGE (0)
inline std::int32_t point_index(std::int32_t id) { return id == 0x26 ? 2 : (id == 0x2b ? 1 : 0); }

// the common start of 14014d730 / 14014fae0 / 140151400: find or create the 3D object, copy type,
// name and the node / element name arrays, re-derive the SV widths
LLOBJ_DATA* begin_object(MeshGenBase* cvt)
{
    return model::llobj_ctor::get_or_create(cvt_m3(cvt), obj_id(cvt_bound(cvt)));  // 14019ccc0
}

void copy_names(MeshGenBase* cvt, LLOBJ_DATA* dst)
{
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    DB_STEP_DATA* m3 = cvt_m3(cvt);
    LLOBJ_DATA* b = cvt_bound(cvt);
    at<std::int32_t>(dst, 0xc) = at<std::int32_t>(b, 0xc);
    std::strcpy(dst->name, b->name);  // 140484f90
    std::int32_t i = 0;
    model::resize_node_names(m3, at<std::int32_t>(m2, 0x163c));  // 1401d21c0
    for (i = 0; i < model::node_name_count(m3); ++i) fun_1401cbd40(m3, &i, fun_1401cbc20(m2)[i]);  // 1401cbbc0
    model::resize_element_names(m3, at<std::int32_t>(m2, 0x16e8));  // 1401d2090
    for (i = 0; i < model::element_name_count(m3); ++i) fun_1401cbd90(m3, &i, fun_1401cbce0(m2)[i]);  // 1401cbc80
    fun_1401d1660(m3, obj_id(b));
}

} // namespace

// @src 14014d730  revolve: 3D object of nn3 nodes / ne3 hexahedra, MTLGRP and the 46 state
// variables of the 2D object, vectors and tensors rotated to the section angle
void fun_14014d730(MeshGenBase* cvt, std::int32_t nn3, std::int32_t ne3, double* x3,
                   std::int32_t* c3, std::int32_t a, double th0, double dth, std::int32_t* n2,
                   std::int32_t* e2)
{
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    DB_STEP_DATA* m3 = cvt_m3(cvt);
    (void)model::geo_kw::simctl_geo_kind(&m2->sim_ctrl);  // 1401ee4c0 (result unused)
    const std::int32_t gt = geotyp(m2);
    LLOBJ_DATA* src = static_cast<LLOBJ_DATA*>(model::step_object_at(*m2, obj_id(cvt_bound(cvt))));  // 1401d6ed0
    LLOBJ_DATA* dst = begin_object(cvt);
    geolib::fun_140202700(dst, nn3, ne3, 4, m3->u15f8, 3);
    copy_names(cvt, dst);
    std::memcpy(coords(dst), x3, static_cast<std::size_t>(nn3 * 3) << 3);
    std::memcpy(conn(dst), c3, static_cast<std::size_t>(ne3 << 3) << 2);
    for (std::int32_t i = 1; i <= ne3; ++i) mtlgrp(dst)[i - 1] = mtlgrp(src)[e2[i - 1] - 1];
    const std::int32_t nsv = fun_140161fd0(src);
    for (std::int32_t t = 1; t <= nsv; ++t) {
        const std::int32_t id = kSvIds[t - 1];
        const std::int32_t w2 = m2->u15f8[id];
        bool nodal = false;
        if (w2 == 0 || fun_1401627e0(src, id, &nodal) == nullptr) continue;
        double* s = fun_1401627e0(src, id, &nodal);
        double* d = fun_1401627e0(dst, id, &nodal);
        if (s == nullptr || d == nullptr) continue;
        const std::int32_t cnt = nodal ? nn3 : ne3;
        std::int32_t q2 = 1, q3 = 1;
        if (id == 0x26 || id == 0x2b || id == 0x2d) {
            q2 = ipoints(src, point_index(id));
            q3 = ipoints(dst, point_index(id));
            if (q3 != q2 * 2) {
                q2 = 1;
                q3 = 1;
            }
        }
        if (id == 0x11) {
            for (std::int32_t i = 1; i <= cnt; ++i) {
                const double* sa = special_array(src, id);
                const std::int32_t r = n2[i - 1];
                double* da = special_array(dst, id);
                std::memcpy(da + static_cast<std::ptrdiff_t>((i - 1) * w2), sa + static_cast<std::ptrdiff_t>((r - 1) * w2),
                            static_cast<std::size_t>(w2 << 3));
            }
            continue;
        }
        if (id == 0x3c || id == 0x31 || id == 0x32 || (id == 0x28 && gt == 3)) {
            for (std::int32_t i = 1; i <= cnt; ++i) {
                const double* sa = special_array(src, id);
                const std::int32_t r = e2[i - 1];
                double* da = special_array(dst, id);
                std::memcpy(da + static_cast<std::ptrdiff_t>((i - 1) * w2), sa + static_cast<std::ptrdiff_t>((r - 1) * w2),
                            static_cast<std::size_t>(w2 << 3));
            }
            continue;
        }
        std::int32_t blocks = 1;
        std::int32_t kind = w2;
        if (id == 0x28) {
            if (gt != 3) {
                kind = 4;
                blocks = 7;
            }
        } else {
            blocks = 1;
        }
        for (std::int32_t b = 0; b < blocks; ++b) {
            std::int32_t o2 = 0, o3 = 0;
            if (id == 0x28 && gt != 3) stncmp_block(src, b, kind, o2, o3);
            if (!(-1 < o2 && o2 < w2)) continue;
            for (std::int32_t i = 1; i <= cnt; ++i) {
                double phi = th0;
                if (!nodal) {
                    const double f = std::floor(
                        (static_cast<double>(c3[i * 8 - 5] + c3[i * 8 - 1] + a * -2) /
                         static_cast<double>(obj_nn(src) - a)) / kTwo);  // 140485120
                    phi = th0 + dth * (f + kHalf);
                } else if (obj_nn(src) < i) {
                    const double f = std::floor(static_cast<double>(i - a) / static_cast<double>(obj_nn(src) - a));
                    phi = th0 + dth * f;
                }
                const std::int32_t w3 = m3->u15f8[id];
                for (std::int32_t g = 1; g <= q2; ++g) {
                    if (kind == 1) {
                        if (!nodal) {
                            if (id == 0x2e && model::step_has_array(*m3, 0x2e, nullptr)) {
                                const double sv = s[e2[i - 1] - 1];
                                double* R = d + static_cast<std::ptrdiff_t>((i - 1) * 9);
                                double u = cosd((sv * kPi) / k180);
                                double v = cosd(phi);
                                R[0] = u * v;
                                u = cosd((sv * kPi) / k180);
                                v = sind(phi);
                                R[1] = u * v;
                                R[2] = sind((sv * kPi) / k180);
                                R[3] = cosd(phi + kPi2);
                                R[4] = sind(phi + kPi2);
                                R[5] = 0.0;
                                u = cosd((sv * kPi) / k180 + kPi2);
                                v = cosd(phi);
                                R[6] = u * v;
                                u = cosd((sv * kPi) / k180 + kPi2);
                                v = sind(phi);
                                R[7] = u * v;
                                R[8] = sind((sv * kPi) / k180 + kPi2);
                            } else if (id == 0x28) {
                                d[(i - 1) * w3 + o3] = s[(e2[i - 1] - 1) * w2 + o2];
                            } else {
                                d[(i - 1) * q3 - 1 + g] = s[(e2[i - 1] - 1) * q2 - 1 + g];
                                if (q3 == q2 * 2) d[(i - 1) * q3 + q2 - 1 + g] = d[(i - 1) * q3 - 1 + g];
                            }
                        } else {
                            d[i - 1] = s[n2[i - 1] - 1];
                        }
                    } else if (kind == 2) {
                        const std::int32_t row = nodal ? n2[i - 1] : e2[i - 1];
                        if (m3->u15f8[id] == 3) {
                            d[(i - 1) * 3] = s[(row - 1) * w2] * cosd(phi);
                            d[(i - 1) * 3 + 1] = s[(row - 1) * w2] * sind(phi);
                            d[(i - 1) * 3 + 2] = s[(row - 1) * w2 + 1];
                        } else if (m3->u15f8[id] == 2) {
                            d[i * 2 - 2] = s[(row - 1) * w2];
                            d[i * 2 - 1] = s[(row - 1) * w2 + 1];
                        }
                    } else if (kind == 4) {
                        if (id == 0x28 || w3 == 6) {
                            double r[9];
                            r[0] = cosd(phi);
                            r[1] = -sind(phi);  // XOR with the sign mask DAT_14051bec0
                            r[2] = 0.0;
                            r[3] = sind(phi);
                            r[4] = cosd(phi);
                            r[5] = 0.0;
                            r[6] = 0.0;
                            r[7] = 0.0;
                            r[8] = kOne;
                            double T[9];
                            if (!nodal) {
                                const std::int32_t base = (e2[i - 1] - 1) * w2 * q2 - 1 + g;
                                T[0] = s[o2 * q2 + base];
                                T[2] = s[(o2 + 3) * q2 + base];
                                T[4] = s[(o2 + 2) * q2 + base];
                                T[6] = s[(o2 + 3) * q2 + base];
                                T[8] = s[(o2 + 1) * q2 + base];
                            } else {
                                const std::int32_t base = (n2[i - 1] - 1) * w2 + o2;
                                T[0] = s[base];
                                T[2] = s[base + 3];
                                T[4] = s[base + 2];
                                T[6] = s[base + 3];
                                T[8] = s[base + 1];
                            }
                            T[1] = 0.0;
                            T[3] = 0.0;
                            T[5] = 0.0;
                            T[7] = 0.0;
                            if (id == 0x27 || id == 0x28) {
                                for (std::int32_t k = 0; k < 9; ++k) T[k] = T[k] / kTwo;
                            }
                            double M[9];
                            for (std::int32_t rr = 0; rr < 3; ++rr)
                                for (std::int32_t cc = 0; cc < 3; ++cc)
                                    M[rr * 3 + cc] = r[rr * 3] * T[cc] + r[rr * 3 + 1] * T[3 + cc] + r[rr * 3 + 2] * T[6 + cc];
                            auto dot = [&](std::int32_t mi, std::int32_t ri) {
                                return M[mi * 3] * r[ri * 3] + M[mi * 3 + 1] * r[ri * 3 + 1] + M[mi * 3 + 2] * r[ri * 3 + 2];
                            };
                            const double out[6] = {dot(0, 0), dot(1, 1), dot(2, 2), dot(0, 1), dot(1, 2), dot(0, 2)};
                            const std::int32_t rowb = (i - 1) * w3 * q3 - 1 + g;
                            for (std::int32_t k = 0; k < 6; ++k) d[(o3 + k) * q3 + rowb] = out[k];
                            if (q3 == q2 * 2) {
                                for (std::int32_t k = 1; k <= w3; ++k)
                                    d[(o3 - 1 + k) * q3 + q2 + (i - 1) * w3 * q3 - 1 + g] =
                                        d[(o3 - 1 + k) * q3 - 1 + g + (i - 1) * w3 * q3];
                            }
                            if (id == 0x27 || id == 0x28) {
                                for (std::int32_t k = 1; k < 7; ++k)
                                    d[(i - 1) * w3 + o3 - 1 + k] = d[(i - 1) * w3 + o3 - 1 + k] * kTwo;
                            }
                        }
                    } else if (kind == 6) {
                        for (std::int32_t k = 1; k < 7; ++k) {
                            d[(k - 1) * q3 - 1 + g + (i - 1) * 6 * q3] =
                                s[(k - 1) * q2 - 1 + g + (e2[i - 1] - 1) * 6 * q2];
                            if (q3 == q2 * 2)
                                d[(k - 1) * q3 + q2 + (i - 1) * 6 * q3 - 1 + g] = d[(k - 1) * q3 - 1 + g + (i - 1) * 6 * q3];
                        }
                    }
                }
            }
        }
    }
}

// @src 14014fae0  extrude: as 14014d730 with the 2D row i % count (0 -> count) and no rotation
void fun_14014fae0(MeshGenBase* cvt, std::int32_t nn3, std::int32_t ne3, double* x,
                   std::int32_t* c, LLOBJ_DATA* obj, double /*dz*/)
{
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    DB_STEP_DATA* m3 = cvt_m3(cvt);
    (void)model::geo_kw::simctl_geo_kind(&m2->sim_ctrl);  // 1401ee4c0 (result unused)
    const std::int32_t gt = geotyp(m2);
    LLOBJ_DATA* src = static_cast<LLOBJ_DATA*>(model::step_object_at(*m2, obj_id(cvt_bound(cvt))));  // 1401d6ed0
    LLOBJ_DATA* dst = begin_object(cvt);
    geolib::fun_140202700(dst, nn3, ne3, 4, m3->u15f8, 3);
    copy_names(cvt, dst);
    std::memcpy(coords(dst), x, static_cast<std::size_t>(nn3 * 3) << 3);
    std::memcpy(conn(dst), c, static_cast<std::size_t>(ne3 << 3) << 2);
    for (std::int32_t i = 1; i <= ne3; ++i) {
        if (i % obj_ne(src) == 0) mtlgrp(dst)[i - 1] = mtlgrp(src)[obj_ne(src) - 1];
        else mtlgrp(dst)[i - 1] = mtlgrp(src)[i % obj_ne(src) - 1];
    }
    auto wrap = [](std::int32_t i, std::int32_t n) { return i % n == 0 ? n : i % n; };
    const std::int32_t nsv = fun_140161fd0(obj);
    for (std::int32_t t = 1; t <= nsv; ++t) {
        const std::int32_t id = kSvIds[t - 1];
        const std::int32_t w2 = m2->u15f8[id];
        bool nodal = false;
        if (w2 == 0 || fun_1401627e0(src, id, &nodal) == nullptr) continue;
        double* s = fun_1401627e0(src, id, &nodal);
        double* d = fun_1401627e0(dst, id, &nodal);
        if (s == nullptr || d == nullptr) continue;
        const std::int32_t cnt = nodal ? nn3 : ne3;
        std::int32_t q2 = 1, q3 = 1;
        if (id == 0x26 || id == 0x2b || id == 0x2d) {
            q2 = ipoints(src, point_index(id));
            q3 = ipoints(dst, point_index(id));
            if (q3 != q2 * 2) {
                q2 = 1;
                q3 = 1;
            }
        }
        if (id == 0x11 || id == 0x3c || id == 0x31 || id == 0x32 || (id == 0x28 && gt == 3)) {
            const std::int32_t n = id == 0x11 ? nn3 : ne3;
            const std::int32_t rows = id == 0x11 ? obj_nn(src) : obj_ne(src);
            for (std::int32_t i = 1; i <= n; ++i) {
                const double* sa = special_array(src, id);
                const std::int32_t r = wrap(i, rows);
                double* da = special_array(dst, id);
                std::memcpy(da + static_cast<std::ptrdiff_t>((i - 1) * w2), sa + static_cast<std::ptrdiff_t>((r - 1) * w2),
                            static_cast<std::size_t>(w2 << 3));
            }
            continue;
        }
        std::int32_t blocks = 1;
        std::int32_t kind = w2;
        if (id == 0x28) {
            if (gt != 3) {
                kind = 4;
                blocks = 7;
            }
        } else {
            blocks = 1;
        }
        for (std::int32_t b = 0; b < blocks; ++b) {
            std::int32_t o2 = 0, o3 = 0;
            if (id == 0x28 && gt != 3) stncmp_block(src, b, kind, o2, o3);
            if (!(-1 < o2 && o2 < w2)) continue;
            for (std::int32_t i = 1; i <= cnt; ++i) {
                const std::int32_t r = nodal ? wrap(i, obj_nn(src)) : wrap(i, obj_ne(src));
                const std::int32_t w3 = m3->u15f8[id];
                for (std::int32_t g = 1; g <= q2; ++g) {
                    if (kind == 1) {
                        if (id == 0x2e && model::step_has_array(*m3, 0x2e, nullptr)) {
                            const double sv = s[r - 1];
                            double* R = d + static_cast<std::ptrdiff_t>((i - 1) * 9);
                            R[0] = cosd((sv * kPi) / k180);
                            R[1] = sind((sv * kPi) / k180);
                            R[2] = 0.0;
                            R[3] = cosd((sv * kPi) / k180 + kPi2);
                            R[4] = sind((sv * kPi) / k180 + kPi2);
                            R[5] = 0.0;
                            R[6] = 0.0;
                            R[7] = 0.0;
                            R[8] = kOne;
                        } else if (id == 0x28) {
                            d[(i - 1) * w3 + o3] = s[(r - 1) * w2 + o2];
                        } else {
                            d[(i - 1) * q3 - 1 + g] = s[(r - 1) * q2 - 1 + g];
                            if (q3 == q2 * 2) d[(i - 1) * q3 + q2 - 1 + g] = d[(i - 1) * q3 - 1 + g];
                        }
                    } else if (kind == 2) {
                        if (m3->u15f8[id] == 3) {
                            d[(i - 1) * 3] = s[(r - 1) * w2];
                            d[(i - 1) * 3 + 1] = s[(r - 1) * w2 + 1];
                            d[(i - 1) * 3 + 2] = 0.0;
                        } else if (m3->u15f8[id] == 2) {
                            d[i * 2 - 2] = s[(r - 1) * w2];
                            d[i * 2 - 1] = s[(r - 1) * w2 + 1];
                        }
                    } else if (kind == 4) {
                        if (id == 0x28 || w3 == 6) {
                            const std::int32_t db = (i - 1) * w3 * q3 - 1 + g;
                            const std::int32_t sb = (r - 1) * w2 * q2 - 1 + g;
                            d[o3 * q3 + db] = s[o2 * q2 + sb];
                            d[(o3 + 1) * q3 + db] = s[(o2 + 1) * q2 + sb];
                            d[(o3 + 2) * q3 + db] = s[(o2 + 2) * q2 + sb];
                            d[(o3 + 3) * q3 + db] = 0.0;
                            d[(o3 + 4) * q3 + db] = 0.0;
                            d[(o3 + 5) * q3 + db] = s[(o2 + 3) * q2 + sb];
                            if (q3 == q2 * 2) {
                                for (std::int32_t k = 1; k <= w3; ++k)
                                    d[(o3 - 1 + k) * q3 + q2 + (i - 1) * w3 * q3 - 1 + g] = d[(o3 - 1 + k) * q3 + db];
                            }
                        }
                    } else if (kind == 6) {
                        for (std::int32_t k = 1; k < 7; ++k) {
                            d[(k - 1) * q3 - 1 + g + (i - 1) * 6 * q3] = s[(k - 1) * q2 - 1 + g + (r - 1) * 6 * q2];
                            if (q3 == q2 * 2)
                                d[(k - 1) * q3 + q2 + (i - 1) * 6 * q3 - 1 + g] = d[(k - 1) * q3 - 1 + g + (i - 1) * 6 * q3];
                        }
                    }
                }
            }
        }
    }
}

// @src 140151400  tet path: 3D object = `tet` mesh, state variables of the interpolated 2D object
// cvt+8 (same node / element numbering), vectors and tensors rotated to the node or element
// centroid direction for a revolve, averaged over the 2D integration points
void fun_140151400(MeshGenBase* cvt, LLOBJ_DATA* tet)
{
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    if (m2 == nullptr) return;
    DB_STEP_DATA* m3 = cvt_m3(cvt);
    const std::int32_t kd = model::geo_kw::simctl_geo_kind(&m2->sim_ctrl);  // 1401ee4c0
    const std::int32_t gt = geotyp(m2);
    LLOBJ_DATA* src = cvt_obj(cvt);
    LLOBJ_DATA* dst = begin_object(cvt);
    copy_names(cvt, dst);
    std::memmove(coords(dst), coords(tet), static_cast<std::size_t>(obj_nn(tet) * 3) << 3);
    std::memmove(conn(dst), conn(tet), static_cast<std::size_t>(obj_ne(tet) << 2) << 2);
    std::memmove(mtlgrp(dst), mtlgrp(src), static_cast<std::size_t>(obj_ne(tet) << 2));
    const std::int32_t nsv = fun_140161fd0(src);
    for (std::int32_t t = 1; t <= nsv; ++t) {
        const std::int32_t id = kSvIds[t - 1];
        std::int32_t w2 = m2->u15f8[id];
        bool nodal = false;
        if (w2 == 0 || fun_1401627e0(src, id, &nodal) == nullptr) continue;
        double* s = fun_1401627e0(src, id, &nodal);
        double* d = fun_1401627e0(dst, id, &nodal);
        if (s == nullptr || d == nullptr) continue;
        const std::int32_t cnt = nodal ? obj_nn(tet) : obj_ne(tet);
        std::int32_t q2 = 1, q3 = 1;
        if (id == 0x26 || id == 0x2b || id == 0x2d) {
            q2 = ipoints(src, point_index(id));
            q3 = ipoints(dst, point_index(id));
            if (q3 != 1) {
                q2 = 1;
                q3 = 1;
            }
        }
        (void)q3;
        if (id == 0x11 || id == 0x3c || id == 0x31 || id == 0x32 || (id == 0x28 && gt == 3)) {
            w2 = cnt * w2;
            const double* sa = special_array(src, id);
            double* da = special_array(dst, id);
            std::memcpy(da, sa, static_cast<std::size_t>(w2 << 3));
            continue;
        }
        std::int32_t blocks = 1;
        std::int32_t kind = w2;
        if (id == 0x28) {
            if (gt != 3) {
                kind = 4;
                blocks = 7;
            }
        } else {
            blocks = 1;
        }
        for (std::int32_t b = 0; b < blocks; ++b) {
            std::int32_t o2 = 0, o3 = 0;
            if (id == 0x28 && gt != 3) stncmp_block(src, b, kind, o2, o3);
            if (!(-1 < o2 && o2 < w2)) continue;
            for (std::int32_t i = 1; i <= cnt; ++i) {
                double c1 = kOne;
                double s1 = 0.0;
                if (kd == 1) {
                    if (!nodal) {
                        const std::int32_t* e = conn(dst) + (i * 4 - 4);
                        const double* X = coords(dst);
                        const double xc = (X[(e[0] - 1) * 3] + X[(e[1] - 1) * 3] + X[(e[2] - 1) * 3] + X[(e[3] - 1) * 3]) / kFour;
                        const double yc =
                            (X[(e[0] - 1) * 3 + 1] + X[(e[1] - 1) * 3 + 1] + X[(e[2] - 1) * 3 + 1] + X[(e[3] - 1) * 3 + 1]) / kFour;
                        if (xc != kZero || yc != kZero) {
                            const double L = std::sqrt(xc * xc + yc * yc);  // 140484644
                            c1 = xc / L;
                            s1 = yc / L;
                        }
                    } else {
                        const double* X = coords(dst);
                        if (X[(i - 1) * 3] != kZero || X[(i - 1) * 3 + 1] != kZero) {
                            const double L = std::sqrt(X[(i - 1) * 3] * X[(i - 1) * 3] + X[(i - 1) * 3 + 1] * X[(i - 1) * 3 + 1]);
                            c1 = X[(i - 1) * 3] / L;
                            s1 = X[(i - 1) * 3 + 1] / L;
                        }
                    }
                }
                const std::int32_t w3 = m3->u15f8[id];
                std::vector<double> acc(static_cast<std::size_t>(w3 > 0 ? w3 : 0) + 8);  // malloc(w3 doubles)
                for (std::int32_t g = 1; g <= q2; ++g) {
                    if (g == 1) {
                        for (std::int32_t k = 1; k <= w3; ++k) acc[k - 1] = 0.0;
                    }
                    if (kind == 1) {
                        if (id == 0x2e && model::step_has_array(*m3, 0x2e, nullptr)) {
                            const double sv = s[i - 1];
                            double* R = d + static_cast<std::ptrdiff_t>((i - 1) * 9);
                            R[0] = cosd((sv * kPi) / k180) * c1;
                            R[1] = cosd((sv * kPi) / k180) * s1;
                            R[2] = sind((sv * kPi) / k180);
                            R[3] = -s1;  // XOR with the sign mask
                            R[4] = c1;
                            R[5] = 0.0;
                            R[6] = cosd((sv * kPi) / k180 + kPi2) * c1;
                            R[7] = cosd((sv * kPi) / k180 + kPi2) * s1;
                            R[8] = sind((sv * kPi) / k180 + kPi2);
                        } else if (id == 0x28) {
                            d[(i - 1) * w3 + o3] = s[(i - 1) * w2 + o2];
                        } else {
                            d[i - 1] = s[(i - 1) * q2 - 1 + g];
                            d[i - 1] = acc[0] + d[i - 1] / static_cast<double>(q2);
                            acc[0] = d[i - 1];
                        }
                    } else if (kind == 2) {
                        if (m3->u15f8[id] == 3) {
                            if (kd == 1) {
                                d[(i - 1) * 3] = s[(i - 1) * w2] * c1;
                                d[(i - 1) * 3 + 1] = s[(i - 1) * w2] * s1;
                                d[(i - 1) * 3 + 2] = s[(i - 1) * w2 + 1];
                            } else if (kd == 2) {
                                d[(i - 1) * 3] = s[(i - 1) * w2];
                                d[(i - 1) * 3 + 1] = s[(i - 1) * w2 + 1];
                                d[(i - 1) * 3 + 2] = 0.0;
                            }
                        } else if (m3->u15f8[id] == 2) {
                            d[i * 2 - 2] = s[(i - 1) * w2];
                            d[i * 2 - 1] = s[(i - 1) * w2 + 1];
                        }
                    } else if (kind == 4) {
                        if (id == 0x28 || w3 == 6) {
                            const std::int32_t sb = (i - 1) * w2 * q2 - 1 + g;
                            double* out = d + static_cast<std::ptrdiff_t>((i - 1) * w3 + o3);
                            if (kd == 1) {
                                const double r[9] = {c1, -s1, 0.0, s1, c1, 0.0, 0.0, 0.0, kOne};
                                double T[9];
                                T[0] = s[o2 * q2 + sb];
                                T[1] = 0.0;
                                T[2] = s[(o2 + 3) * q2 + sb];
                                T[3] = 0.0;
                                T[4] = s[(o2 + 2) * q2 + sb];
                                T[5] = 0.0;
                                T[6] = s[(o2 + 3) * q2 + sb];
                                T[7] = 0.0;
                                T[8] = s[(o2 + 1) * q2 + sb];
                                if (id == 0x27 || id == 0x28) {
                                    for (std::int32_t k = 0; k < 9; ++k) T[k] = T[k] / kTwo;
                                }
                                double M[9];
                                for (std::int32_t rr = 0; rr < 3; ++rr)
                                    for (std::int32_t cc = 0; cc < 3; ++cc)
                                        M[rr * 3 + cc] = r[rr * 3] * T[cc] + r[rr * 3 + 1] * T[3 + cc] + r[rr * 3 + 2] * T[6 + cc];
                                auto dot = [&](std::int32_t mi, std::int32_t ri) {
                                    return M[mi * 3] * r[ri * 3] + M[mi * 3 + 1] * r[ri * 3 + 1] + M[mi * 3 + 2] * r[ri * 3 + 2];
                                };
                                out[0] = dot(0, 0);
                                out[1] = dot(1, 1);
                                out[2] = dot(2, 2);
                                out[3] = dot(0, 1);
                                out[4] = dot(1, 2);
                                out[5] = dot(0, 2);
                                if (id == 0x27 || id == 0x28) {
                                    for (std::int32_t k = 0; k < 6; ++k) out[k] = out[k] * kTwo;
                                }
                            } else if (kd == 2) {
                                out[0] = s[o2 * q2 + sb];
                                out[1] = s[(o2 + 1) * q2 + sb];
                                out[2] = s[(o2 + 2) * q2 + sb];
                                out[3] = 0.0;
                                out[4] = 0.0;
                                out[5] = s[(o2 + 3) * q2 + sb];
                            }
                            if ((kd == 1 || kd == 2) && 1 < q2) {
                                for (std::int32_t k = 1; k <= w3; ++k) {
                                    out[k - 1] = acc[k - 1] + out[k - 1] / static_cast<double>(q2);
                                    acc[k - 1] = out[k - 1];
                                }
                            }
                        }
                    } else if (kind == 6) {
                        for (std::int32_t k = 1; k < 7; ++k) {
                            double& o = d[(i - 1) * 6 - 1 + k];
                            o = s[(k - 1) * q2 - 1 + g + (i - 1) * 6 * q2];
                            o = acc[k - 1] + o / static_cast<double>(q2);
                            acc[k - 1] = o;
                        }
                    }
                }
            }
        }
    }
}

} // namespace def::cmd::cnvt3d_sweep
