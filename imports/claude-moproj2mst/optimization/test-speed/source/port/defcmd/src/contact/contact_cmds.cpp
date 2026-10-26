// INICTC and GENCTC (cases 5 and 6 of FUN_140378e00) and their C++ drivers: INICTC.md and
// GENCTC.md sections 2, 3, 4.1, 4.2, 5.1. Object fields are read at their binary offsets
// (layout headers name few of them); see GENCTC.md section 7 and objarrays.md.
#include <defcmd/contact_cmds.hpp>
#include <defmodel/node_access.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defcmd/ctc2d_ftn.hpp>
#include <defcmd/genctc3d.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/port.hpp>

#include "contact_ext.hpp"

namespace def::cmd {

using def::model::Cls_1401f46e0;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

// LLOBJ_DATA fields (objarrays.md, GENCTC.md)
constexpr std::size_t kNumber = 0x8;        // object number
constexpr std::size_t kType = 0xc;          // type (0 rigid, 5, 7 empty slot, ...)
constexpr std::size_t kHasMesh = 0x87c;
constexpr std::size_t kNodes = 0x880;
constexpr std::size_t kElems = 0x884;
constexpr std::size_t kNpe = 0x894;         // nodes per element
constexpr std::size_t kNodeSet = 0x8a0;     // Cls_14017ef20
constexpr std::size_t kCoords = 0x8a8;      // node set +8
constexpr std::size_t kBccdef = 0x990;      // node set +0xf0
constexpr std::size_t kBccdfn = 0x998;      // node set +0xf8
constexpr std::size_t kElemSet = 0x9e0;     // Cls_140182880
constexpr std::size_t kConn = 0x9e8;        // element set +8
constexpr std::size_t kLoops = 0xbd8;       // number of boundary loops
constexpr std::size_t kLoopRec = 0xbe0;     // loop records, 0x30 each: +0 count, +8 node list
constexpr std::size_t kFaces = 0xbf0;       // boundary faces of surface 0 (3D)
constexpr std::size_t kFaceList = 0xbf8;
constexpr std::size_t kOwner = 0x3a98;      // owning model (15000)
// DB_STEP_DATA
constexpr std::size_t kNdof = 0x1640;
// Cls_1401f46e0 (inter-object pair)
constexpr std::size_t kPairFirst = 0x168;
constexpr std::size_t kPairSecond = 0x16c;
constexpr std::size_t kPairDir = 0x170;

bool obj_is_2d(LLOBJ_DATA* o) { return def::model::model_is_2d(at<DB_STEP_DATA*>(o, kOwner)); }

// FUN_1401d6ea0: object number n of the model (lookup only)
LLOBJ_DATA* find_object(DB_STEP_DATA* m, std::int32_t n) { return def::model::object_list_find(&m->objects, n); }

} // namespace

// 1401808d0: ported in defmodel/node_access.hpp (fun_1401808d0)
double* node_set_coords(void* set)
{
    return model::fun_1401808d0(static_cast<model::Cls_14017ef20*>(set));
}

// @src 140183cb0  element set connectivity
std::int32_t* element_set_conn(void* set)
{
    auto* s = static_cast<unsigned char*>(set);
    if (at<void*>(s, 0xf0) != nullptr) {
        std::int32_t one = 1;
        ext::fun_1401d5820(at<void*>(s, 0xf0), &one);
    }
    return at<std::int32_t*>(s, 8);
}

// @src 1402049b0  clear negative codes
void inictc_clear(LLOBJ_DATA* obj)
{
    const std::int32_t d = obj_is_2d(obj) ? 2 : 3;
    std::int32_t* bcc = at<std::int32_t*>(obj, kBccdef);
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, kNodes); ++i) {
        for (std::int32_t k = 0; k < d; ++k) {
            if (bcc[i * d + k] < 0) bcc[i * d + k] = 0;
        }
    }
}

// part of @src 140378e00 (case 5)
int cmd_inictc(DB_STEP_DATA* model, std::FILE* f)
{
    std::int32_t n = 0;
    if (def::key::read_i(f, &n, false) != 0) return 2;  // 140413de0(f, &n, 0)
    LLOBJ_DATA* obj = find_object(model, n);
    if (obj == nullptr) return 2;
    inictc_clear(obj);
    return 0;
}

// part of @src 140378e00 (case 6)
int cmd_genctc(DB_STEP_DATA* model, std::FILE* f)
{
    double tol = 0.0;
    if (def::key::read_d(f, &tol) != 0) return 2;  // 140414990
    if (def::model::model_is_2d(model)) {
        genctc_2d(model, tol);
    } else {
        genctc_3d(model, tol);
    }
    return 0;
}

// @src 1403046a0  2D smallest element edge
double min_edge_length_2d(LLOBJ_DATA* obj)
{
    const std::int32_t* conn = at<std::int32_t*>(obj, kConn);
    const double* xy = at<double*>(obj, kCoords);
    if (at<std::int32_t>(obj, kHasMesh) == 0 || at<std::int32_t>(obj, kElems) < 1 || conn == nullptr ||
        xy == nullptr) {
        return 0.0;
    }
    double best = 1e29;  // 14052c598
    const std::int32_t npe = 4;
    const bool tri = conn[0] == conn[3];  // first element only
    for (std::int32_t e = 0; e < at<std::int32_t>(obj, kElems); ++e) {
        const std::int32_t* ce = conn + npe * e;
        for (std::int32_t j = 0; j < npe; ++j) {
            std::int32_t k;
            if (j == npe - 1) {
                if (!tri) {
                    k = 0;
                } else {
                    continue;
                }
            } else {
                k = j + 1;
            }
            const double* a = xy + 2 * (ce[j] - 1);  // 140308730
            const double* b = xy + 2 * (ce[k] - 1);
            const double v[2] = {a[0] - b[0], a[1] - b[1]};          // 1403089b0
            const double len = std::sqrt(v[0] * v[0] + v[1] * v[1]); // 1403087e0, 1403087a0
            best = (len <= best) ? std::sqrt(v[0] * v[0] + v[1] * v[1]) : best;
        }
    }
    return best;
}

// @src 140202a70  3D smallest boundary-face edge
double min_edge_length_3d(LLOBJ_DATA* obj)
{
    if (at<std::int32_t>(obj, kHasMesh) == 0 || at<std::int32_t>(obj, kLoops) < 1) return 0.0;
    const std::int32_t nf = at<std::int32_t>(obj, kFaces);
    if (nf < 1) return 0.0;
    const std::int32_t* faces = at<std::int32_t*>(obj, kFaceList);
    const double* xyz = at<double*>(obj, kCoords);
    double best = 1e29;  // 14052c598
    const bool tri = faces[0] == faces[3];
    for (std::int32_t f = 0; f < nf; ++f) {
        for (std::int32_t j = 0; j < 4; ++j) {
            std::int32_t k;
            if (j == 3) {
                if (!tri) {
                    k = 0;
                } else {
                    continue;
                }
            } else {
                k = j + 1;
            }
            const double* a = xyz + 3 * (faces[f * 4 + j] - 1);  // 140239800
            const double* b = xyz + 3 * (faces[f * 4 + k] - 1);
            const double v[3] = {a[0] - b[0], a[1] - b[1], a[2] - b[2]};          // 140239fb0
            const double len = std::sqrt((v[0] * v[0] + v[1] * v[1]) + v[2] * v[2]);  // 140239a30
            best = (len <= best) ? std::sqrt((v[0] * v[0] + v[1] * v[1]) + v[2] * v[2]) : best;
        }
    }
    return best;
}

// @src 140202a20  smallest edge, by dimension
double min_edge_length(LLOBJ_DATA* obj)
{
    if (!obj_is_2d(obj)) return min_edge_length_3d(obj);
    return min_edge_length_2d(obj);
}

// @src 140202e00  3D element box
double element_box_min_3d(LLOBJ_DATA* obj)
{
    double best = 1e20;  // 14051f1b0
    if (at<std::int32_t>(obj, kHasMesh) == 0) return -1.0;  // 140522fc8
    const std::int32_t ne = at<std::int32_t>(obj, kElems);
    const std::int32_t npe = at<std::int32_t>(obj, kNpe);
    if (ne == 0 || npe == 0) return -1.0;
    const double* xyz = node_set_coords(&at<unsigned char>(obj, kNodeSet));
    const std::int32_t* conn = element_set_conn(&at<unsigned char>(obj, kElemSet));
    if (xyz == nullptr || conn == nullptr) return -1.0;
    for (std::int32_t e = 0; e < ne; ++e) {
        const double* p = xyz + 3 * (conn[e * npe] - 1);
        double xmax = p[0], ymax = p[1], zmin = p[2];
        double xmin = xmax, ymin = ymax, zmax = zmin;
        for (std::int32_t j = 1; j < npe; ++j) {
            const double* q = xyz + 3 * (conn[e * npe + j] - 1);
            xmin = (q[0] <= xmin) ? q[0] : xmin;
            xmax = (xmax <= q[0]) ? q[0] : xmax;
            ymin = (q[1] <= ymin) ? q[1] : ymin;
            ymax = (ymax <= q[1]) ? q[1] : ymax;
            zmin = (q[2] <= zmin) ? q[2] : zmin;
            zmax = (zmax <= q[2]) ? q[2] : zmax;
        }
        const double dx = xmax - xmin;
        const double dy = ymax - ymin;
        const double dz = zmax - zmin;
        const double mxy = (dx <= dy) ? dy : dx;
        const double m = (dz <= mxy) ? ((dx <= dy) ? dy : dx) : dz;
        best = (m <= best) ? m : best;
    }
    return best;
}

// @src 140202da0  element box by dimension
double element_box_min(LLOBJ_DATA* obj)
{
    if (!obj_is_2d(obj)) return element_box_min_3d(obj);
    std::fprintf(stdout, "LLOBJ_DATA::calculateElementBoxMin() need to be checked in 2D.\n");
    return 0.0;
}

// @src 1401a3160  2D pair worker
bool genctc_2d_pair(DB_STEP_DATA* model, LLOBJ_DATA* m, LLOBJ_DATA* s, Cls_1401f46e0* pair, double tol,
                    std::int32_t* fail, std::int32_t /*unused*/)
{
    if (at<std::int32_t>(pair, kPairDir) == 0) return false;
    if (at<std::int32_t>(pair, kPairDir) != 2) return false;
    if (at<std::int32_t>(s, kType) == 7 || at<std::int32_t>(s, kHasMesh) == 0) return false;
    if (at<std::int32_t>(s, kType) == 5) return false;
    if (at<std::int32_t>(m, kType) == 7) return false;
    if (at<std::int32_t>(m, kType) == 5) return false;

    const std::int32_t ndof = at<std::int32_t>(model, kNdof);
    std::int32_t changed = 0;
    std::int32_t do_codes = 1;   // local_114
    std::int32_t one = 1;        // local_118
    *fail = 0;
    const std::int32_t n = at<std::int32_t>(s, kNodes);
    // operator new[] with the size check of the original (overflow -> all ones)
    auto* bc3 = static_cast<std::int32_t*>(::operator new(static_cast<std::size_t>(static_cast<std::int64_t>(n * 3)) * 4));
    auto* xy = static_cast<double*>(::operator new(static_cast<std::size_t>(static_cast<std::int64_t>(n << 1)) * 8));
    std::int32_t* bcc = at<std::int32_t*>(s, kBccdef);
    if (ndof == 2) {
        for (std::int32_t i = 0; i < n; ++i) {
            std::memcpy(bc3 + i * 3, bcc + ndof * i, static_cast<std::size_t>(ndof) << 2);
            bc3[i * 3 + 2] = 0;
        }
    } else {
        std::memcpy(bc3, bcc, static_cast<std::size_t>(static_cast<std::int64_t>(n * 3)) << 2);
    }
    std::memcpy(xy, node_set_coords(&at<unsigned char>(s, kNodeSet)),
                static_cast<std::size_t>(static_cast<std::int64_t>(n << 1)) << 3);
    std::int32_t mno = at<std::int32_t>(m, kNumber);  // local_10c
    const std::int32_t nloops = at<std::int32_t>(s, kLoops);
    for (std::int32_t k = 0; k < nloops; ++k) {
        auto* rec = &at<unsigned char>(s, kLoopRec + static_cast<std::size_t>(k) * 0x30);
        std::int32_t* nodes = at<std::int32_t*>(rec, 8);
        std::int32_t info[3];
        info[0] = at<std::int32_t>(rec, 0) + -1;  // the last entry repeats the first
        if (at<std::int32_t>(m, kType) == 0) {
            info[2] = def::model::geo_kw::curve_count(*m);  // 140198310
            info[1] = 1;
            for (std::int32_t c = 1; c <= info[2]; ++c) {
                def::model::OBJ_GEO2D* curve = def::model::geo_kw::curve(*m, c + -1);  // 1401982c0
                def::model::OBJ_GEO2D tmp;  // 140275cd0, dtor 140275e00
                tmp.copy_from(curve);       // 140276230
                tmp.set_curve_type(2);      // 1402763d0
                ftn::fun_140350f12(xy, &info[0], nodes, &tmp.n_points, tmp.points, &info[1], &one, &mno,
                                   bc3, &tol, &changed, &do_codes, fail);
            }
        } else {
            double* mxy = node_set_coords(&at<unsigned char>(m, kNodeSet));
            const std::int32_t mloops = at<std::int32_t>(m, kLoops);
            std::int32_t nl = 1;  // local_78
            auto* li = static_cast<std::int32_t*>(::operator new(8));
            for (std::int32_t j = 0; j < mloops; ++j) {
                auto* mrec = &at<unsigned char>(m, kLoopRec + static_cast<std::size_t>(j) * 0x30);
                std::int32_t* mnodes = at<std::int32_t*>(mrec, 8);
                li[0] = 1;
                li[1] = at<std::int32_t>(mrec, 0);
                ftn::fun_14034fc56(xy, &info[0], nodes, mxy, &nl, li, mnodes, &mno, bc3, &tol, &changed,
                                   &do_codes);
            }
            std::free(li);
        }
    }
    bool result = false;
    if (changed != 0) {
        std::int32_t all_ok = 1;
        std::int32_t p5 = 0;
        for (std::int32_t e = 1; e <= at<std::int32_t>(s, kElems); ++e) {
            std::int32_t ok = 1;
            std::int32_t ee = e;
            const std::int32_t* conn = element_set_conn(&at<unsigned char>(s, kElemSet));
            ftn::fun_14032e34e(&ee, conn, xy, &ok, &p5);
            if (ok == 0) all_ok = 0;
        }
        if (all_ok == 1) {
            for (std::int32_t i = 0; i < n; ++i) {
                std::memcpy(bcc + ndof * i, bc3 + i * 3, static_cast<std::size_t>(ndof) << 2);
            }
            std::memcpy(node_set_coords(&at<unsigned char>(s, kNodeSet)), xy,
                        static_cast<std::size_t>(static_cast<std::int64_t>(n << 1)) << 3);
            result = true;
        }
    }
    std::free(bc3);
    std::free(xy);
    return result;
}

// @src 1401a2e90  2D driver
std::int32_t genctc_2d(DB_STEP_DATA* model, double tol)
{
    bool some_failed = false;
    std::int32_t fail = 0;
    for (std::int32_t p = 1; p <= def::model::pair_count(&model->inter_obj); ++p) {  // 1400015d0
        Cls_1401f46e0* pair = def::model::pair_nth(&model->inter_obj, p);              // 1401fac90
        if (pair == nullptr || at<std::int32_t>(pair, kPairDir) == 0) continue;
        std::int32_t sn;
        std::int32_t mn;
        if (at<std::int32_t>(pair, kPairDir) == 1) {
            sn = at<std::int32_t>(pair, kPairFirst);
            mn = at<std::int32_t>(pair, kPairSecond);
        } else if (at<std::int32_t>(pair, kPairDir) == 2) {
            sn = at<std::int32_t>(pair, kPairSecond);
            mn = at<std::int32_t>(pair, kPairFirst);
        } else {
            continue;
        }
        if (sn == mn) continue;
        LLOBJ_DATA* so = find_object(model, sn);
        LLOBJ_DATA* mo = find_object(model, mn);
        if (so == nullptr || at<std::int32_t>(so, kHasMesh) == 0 || mo == nullptr) continue;
        if (at<std::int32_t>(mo, kHasMesh) == 0 && !def::model::geo_kw::geometry_has_edges(*mo)) continue;  // 140160d20
        if (tol < 1e-12) {                    // 14051f260
            tol = min_edge_length(so) * 0.05; // 140202a20, 140524ce0
        }
        const bool r = genctc_2d_pair(model, mo, so, pair, tol, &fail, 0);
        if ((fail != 0 || !r) && (fail == 0 && !r)) some_failed = true;
    }
    return some_failed ? -2 : 0;
}

// @src 1401a6c80  3D driver
void genctc_3d(DB_STEP_DATA* model, double tol)
{
    std::int32_t* partners = nullptr;  // local_a0
    std::int32_t one_a4 = 1;
    std::int32_t one_94 = 1;           // local_94[1]
    std::int32_t one_b4 = 1;
    std::int32_t ierr = 0;             // local_ac
    std::int32_t zero_88 = 0;
    std::int32_t zero_98 = 0;
    std::int32_t ndof = at<std::int32_t>(model, kNdof);  // local_7c
    ctc3d::fun_1401a7710(model);
    for (std::int32_t o = 1;; ++o) {
        if (def::model::llobj_ctor::list_count(&model->objects) < o) {  // 14019a280
            ctc3d::fun_140225820(&ierr);
            return;
        }
        auto* obj = static_cast<LLOBJ_DATA*>(def::model::step_object_at(*model, o));  // 1401d6ed0
        if (obj == nullptr || at<std::int32_t>(obj, kType) == 7 ||    // 140160f10
            at<std::int32_t>(obj, kHasMesh) == 0 || at<std::int32_t>(obj, kType) == 5) {  // 140161020
            continue;
        }
        if (tol < 1e-12) {                   // 14051f260
            tol = element_box_min(obj);      // 140202da0
            tol = tol * 0.05;                // 140524ce0
            if (tol <= 0.0) continue;
        }
        partners = static_cast<std::int32_t*>(
            def::model::llobj_copy::calloc_int(def::model::llobj_ctor::list_count(&model->objects)));  // 140256dc0
        std::int32_t np = 0;  // local_94[0]
        for (std::int32_t p = 1; p <= def::model::pair_count(&model->inter_obj); ++p) {
            Cls_1401f46e0* pair = def::model::pair_nth(&model->inter_obj, p);
            if (pair == nullptr || at<std::int32_t>(pair, kPairDir) == 0) continue;
            const std::int32_t first = at<std::int32_t>(pair, kPairFirst);
            const std::int32_t second = at<std::int32_t>(pair, kPairSecond);
            std::int32_t add;
            if (at<std::int32_t>(pair, kPairDir) == 1 && first == at<std::int32_t>(obj, kNumber)) {
                add = second;
            } else if (at<std::int32_t>(pair, kPairDir) == 2 && second == at<std::int32_t>(obj, kNumber)) {
                add = first;
            } else {
                continue;
            }
            bool seen = false;
            for (std::int32_t i = 0; i < np; ++i) {
                if (partners[i] == add) seen = true;
            }
            if (!seen) {
                partners[np] = add;
                np = np + 1;
            }
        }
        if (0 < np) {
            ctc3d::fun_1400511e4(&one_94, &np, partners);
            for (std::int32_t i = 0; i < np; ++i) ctc3d::fun_140082ed6(partners + i, &one_94);
            ctc3d::fun_140082ed6(&at<std::int32_t>(obj, kNumber), &one_94);
            std::int32_t* bcc = at<std::int32_t*>(obj, kBccdef);
            std::int32_t* bfn = at<std::int32_t*>(obj, kBccdfn);
            for (std::int32_t i = 0; i < at<std::int32_t>(model, kNdof) * at<std::int32_t>(obj, kNodes); ++i) {
                if (bcc[i] == 6) bcc[i] = bcc[i] * 100 + bfn[i];
            }
            ctc3d::fun_1400512aa(&at<std::int32_t>(obj, kNumber), &one_94, at<double*>(obj, kCoords), bcc, &ndof,
                                 &one_b4, &at<std::int32_t>(obj, kNodes), &at<std::int32_t>(obj, kFaces), &one_a4,
                                 &zero_98, &ierr, &zero_88, &tol);
            bcc = at<std::int32_t*>(obj, kBccdef);
            bfn = at<std::int32_t*>(obj, kBccdfn);
            for (std::int32_t i = 0; i < at<std::int32_t>(model, kNdof) * at<std::int32_t>(obj, kNodes); ++i) {
                if (bcc[i] == bfn[i] + 600) bcc[i] = 6;
            }
            if (partners != nullptr) std::free(partners);
            partners = nullptr;
        }
        // 1401a72bd: the second pass (140055162, 14001d99f) is guarded by a constant false
    }
}

} // namespace def::cmd
