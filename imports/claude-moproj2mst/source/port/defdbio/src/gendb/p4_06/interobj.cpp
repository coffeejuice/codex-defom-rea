// GENDB inter-object list writer and the 2D boundary search of packet p4-06
// (defdbio/gendb/p4_06_interobj.hpp).
#include <defdbio/gendb/p4_06_interobj.hpp>

#include <cmath>
#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::gendb {

using model::Cls_1401f46e0;
using model::Cls_1401fa020;
using model::DB_STEP_DATA;
using model::field;
using model::LLOBJ_DATA;
using model::OBJ_GEO2D;

// @src 1401fa4f0  inter-object list writer
void write_interobj_list(Cls_1401fa020* list, double* llbrt, const std::int32_t* nbnd, std::int32_t* iifnc,
                         double* llfnc, double* llwin, double* llwrz, const std::int32_t* w_llbrt,
                         const std::int32_t* w_llwin, double* llbnd, const std::int32_t* w_llbnd)
{
    Cls_1401f46e0* io = nullptr;
    LLOBJ_DATA* oi = nullptr;
    LLOBJ_DATA* oj = nullptr;
    const std::int32_t n = ext::f1401f4610(list->owner);
    std::int32_t j = 0;
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        oi = ext::f1401d6ea0(list->owner, i);
        if (oi == nullptr || field<std::int32_t>(oi, 0xc) == 7) {
            continue;
        }
        for (j = i; j <= n; j = j + 1) {
            oj = ext::f1401d6ea0(list->owner, j);
            if (oj == nullptr || field<std::int32_t>(oj, 0xc) == 7) {
                continue;
            }
            io = static_cast<Cls_1401f46e0*>(ext::f1401facf0(list, i, j));
            if (io == nullptr || io->u170 == 0) {
                continue;
            }
            std::int32_t bm = 0;
            std::int32_t bs = 0;
            choose_boundary_records(list, oi, oj, &io->u170, &bm, &bs, llbnd, w_llbnd);
            // 1401fa6bb: W * ((bs - 1) * N + bm - 1), int32
            double* rec = llbrt + *w_llbrt * ((bs - 1) * *nbnd + bm - 1);
            write_interobj_record(io, &i, &j, rec, iifnc, llfnc, llwin, llwrz, w_llwin);
            if (bm != bs) {
                double* mirror = llbrt + *w_llbrt * ((bm - 1) * *nbnd + bs - 1);
                if (*rec == 1.0) {          // DAT_140509c90
                    *mirror = 2.0;          // DAT_140509fb8
                }
                if (*rec == 2.0) {
                    *mirror = 1.0;
                }
                std::memcpy(mirror + 1, rec + 1,
                            static_cast<std::size_t>(static_cast<std::int64_t>(*w_llbrt - 1) << 3));
            }
        }
    }
}

// @src 1401fa840  choose the boundary records of master and slave
void choose_boundary_records(Cls_1401fa020* /*list*/, LLOBJ_DATA* master, LLOBJ_DATA* slave,
                             const std::int32_t* flag, std::int32_t* bm, std::int32_t* bs, double* /*llbnd*/,
                             const std::int32_t* /*w_llbnd*/)
{
    *bm = 0;
    *bs = 0;
    if (field<std::int32_t>(master, 0x3588) == field<std::int32_t>(master, 0x358c)) {
        *bm = field<std::int32_t>(master, 0x3588);
    }
    if (field<std::int32_t>(slave, 0x3588) == field<std::int32_t>(slave, 0x358c)) {
        *bs = field<std::int32_t>(slave, 0x3588);
    }
    if (*bm != 0 && *bs != 0) {
        return;
    }
    double pt[2] = {0.0, 0.0};
    std::int32_t found = 0;
    if (*bm != 0 && *bs == 0) {
        const std::int32_t k = 0;
        boundary_first_point(master, pt, &k);
        if (nearest_boundary(slave, pt[0], pt[1], &found, nullptr)) {
            *bs = found;
        }
    } else if (*bm == 0 && *bs != 0) {
        const std::int32_t k = 0;
        boundary_first_point(slave, pt, &k);
        if (nearest_boundary(master, pt[0], pt[1], &found, nullptr)) {
            *bm = found;
        }
    } else {
        std::int32_t a = 0;
        std::int32_t b = 0;
        bool ok = false;
        if (*flag == 1) {
            ok = nearest_boundary_pair(master, slave, &a, &b);
        } else {
            ok = nearest_boundary_pair(slave, master, &b, &a);
        }
        if (ok) {
            *bm = a;
            *bs = b;
        }
    }
}

// @src 1401667e0  first point of a boundary
void boundary_first_point(LLOBJ_DATA* obj, double* pt, const std::int32_t* idx)
{
    if (field<std::int32_t>(obj, 0x87c) == 0) {
        if (!ext::f140160d20(obj)) {
            pt[0] = 0.0;
            pt[1] = 0.0;
        } else {
            const std::int32_t c = 0;
            const auto* g = static_cast<const OBJ_GEO2D*>(ext::f1401982c0(obj, &c));
            if (g->curve_type == 1) {
                pt[0] = g->points[0];
                pt[1] = g->points[1];
            } else {
                pt[0] = g->points[1];
                pt[1] = g->points[2];
            }
        }
    } else if (-1 < *idx && *idx < field<std::int32_t>(obj, 0xbd8)) {
        const double* xy = field<double*>(obj, 0x8a8);
        if (xy != nullptr) {
            // 140166849: boundary *idx is the Cls_14017e740 at 0xbe0 + 0x30 * idx; node list at +8
            const std::int32_t node = *field<std::int32_t*>(obj, 0xbe0 + static_cast<std::size_t>(*idx) * 0x30 + 8) - 1;
            pt[0] = xy[node * 2];
            pt[1] = xy[node * 2 + 1];
        }
    }
}

// @src 140166980  nearest boundary pair of a meshed object and another object
bool nearest_boundary_pair(LLOBJ_DATA* a, LLOBJ_DATA* b, std::int32_t* ia, std::int32_t* ib)
{
    if (field<std::int32_t>(a, 0x87c) == 0) {
        return false;
    }
    if (field<std::int32_t>(a, 0xbd8) < 1) {
        return false;
    }
    if (field<std::int32_t>(a, 0x880) < 1) {
        return false;
    }
    double pt[2] = {0.0, 0.0};
    std::int32_t other = 0;
    double best = 0.0;
    double dist = 0.0;
    bool first = true;
    for (std::int32_t k = 0; k < field<std::int32_t>(a, 0xbd8); k = k + 1) {
        boundary_first_point(a, pt, &k);
        if (!nearest_boundary(b, pt[0], pt[1], &other, &dist)) {
            continue;
        }
        if (first) {
            best = dist;
            *ia = field<std::int32_t>(a, 0x3588) + k;
            *ib = other;
            first = false;
        } else if (dist < best) {   // 140166af0: comisd best, dist; jbe
            best = dist;
            *ia = field<std::int32_t>(a, 0x3588) + k;
            *ib = other;
        }
    }
    return true;
}

// @src 140166b40  nearest boundary of an object and its distance
bool nearest_boundary(LLOBJ_DATA* obj, double x, double y, std::int32_t* bnd, double* dist)
{
    bool ok = false;
    double px = 0.0;    // rsp+0x50
    double py = 0.0;    // rsp+0x48
    *bnd = 0;
    if (field<std::int32_t>(obj, 0x87c) != 0) {
        std::int32_t a = 0;
        std::int32_t b = 0;
        ok = ext::f140166e60(obj, x, y, &a, &b, &px, &py);
        if (ok) {
            *bnd = field<std::int32_t>(obj, 0x3588) + b;
        }
    } else {
        std::int32_t idx = 0;
        // 140166c1a: &px / &py are passed as the 5th / 6th arguments; 140166d30 never writes them
        ok = nearest_curve(obj, x, y, &idx, &px, &py);
        if (ok) {
            *bnd = field<std::int32_t>(obj, 0x3588) + idx;
        }
    }
    if (dist != nullptr && ok) {
        const double dx = x - px;   // 140166c97
        const double dy = y - py;
        *dist = dx * dx + dy * dy;  // 140166cc1: mulsd, mulsd, addsd
        if (0.0 < *dist) {          // DAT_14051bed8
            *dist = std::sqrt(*dist);
        }
    }
    return ok;
}

// @src 140166d30  nearest rigid curve
bool nearest_curve(LLOBJ_DATA* obj, double x, double y, std::int32_t* idx, double* /*px*/, double* /*py*/)
{
    bool first = true;
    double best = 0.0;   // rsp+0x30
    double dist = 0.0;   // rsp+0x38
    double cx = 0.0;     // rsp+0x40 (own nearest point, discarded)
    double cy = 0.0;     // rsp+0x48
    if (!ext::f140160cc0(obj)) {
        return false;
    }
    for (std::int32_t k = 0; k < ext::f140198310(obj); k = k + 1) {
        auto* g = static_cast<OBJ_GEO2D*>(ext::f1401982c0(obj, &k));
        if (!geo2d_nearest_point(g, x, y, &dist, &cx, &cy)) {
            continue;
        }
        if (first) {
            best = dist;
            *idx = k;
            first = false;
        } else if (dist < best) {   // 140166e1f: comisd best, dist; jbe
            best = dist;
            *idx = k;
        }
    }
    return true;
}

// @src 14027b620  nearest point of an outline
bool geo2d_nearest_point(OBJ_GEO2D* g, double x, double y, double* dist, double* px, double* py)
{
    bool first = true;
    double best = 0.0;   // rsp+0x28
    *dist = 0.0;
    const std::int32_t dof = g->dof();   // 1402760e0
    if (g->n_points < 1 || g->points == nullptr) {
        return false;
    }
    const double* p = g->points;
    if (g->curve_type == 1) {
        for (std::int32_t i = 0; i < g->n_points; i = i + 1) {
            const double dx = x - p[i * dof];
            const double dy = y - p[i * dof + 1];
            const double d = dx * dx + dy * dy;   // 14027b73b: mulsd, mulsd, addsd
            if (first) {
                best = d;
                *px = p[i * dof];
                *py = p[i * dof + 1];
                first = false;
            } else if (d < best) {   // 14027b7c8: comisd best, d; jbe
                best = d;
                *px = p[i * dof];
                *py = p[i * dof + 1];
            }
        }
    } else {
        for (std::int32_t i = 0; i < g->n_points; i = i + 1) {
            // 14027b877: (int) of the row's segment type word, stored and never used (omitted)
            for (std::int32_t e = 0; e < 3; e = e + 2) {
                const double dx = x - p[i * dof + e + 1];
                const double dy = y - p[i * dof + e + 2];
                const double d = dx * dx + dy * dy;
                if (first) {
                    best = d;
                    *px = p[i * dof + e + 1];
                    *py = p[i * dof + e + 2];
                    first = false;
                } else if (d < best) {
                    best = d;
                    *px = p[i * dof + e + 1];
                    *py = p[i * dof + e + 2];
                }
            }
        }
    }
    if (0.0 < best) {   // DAT_14051bed8
        *dist = std::sqrt(best);
    }
    return true;
}

} // namespace def::dbio::gendb
