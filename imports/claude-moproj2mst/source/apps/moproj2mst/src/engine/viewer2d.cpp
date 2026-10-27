// The 2D items of the 3D viewer: curve item and mesh item point lists and their rects (packet V
// extension for SfDispView, packet moproj). Spec: docs/re/moproj2mst/moproj/dispview.md.
// Decompiles: gui_mo/s1/moproj/v40c.c .. v44c.c, v52c.c (scratch copies of the session).
//
// sin / cos / acos / sqrt are the C runtime's (the binary calls 0x14528d000 / 0x14528d490 /
// 0x141472490 / 0x141471bbc); the values reach the .moproj only through QString::number(.., 'g', 6).
#include "viewer2d.hpp"

#include <cmath>
#include <cstdint>
#include <cstdlib>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/node_access.hpp>

namespace m2m::engine {

namespace {

template <class T>
T* at(void* base, std::ptrdiff_t off)
{
    return reinterpret_cast<T*>(static_cast<char*>(base) + off);
}

constexpr double kItemTolFactor = 0.05;            // DAT_1421ec2e0
constexpr double kRadToDeg = 57.29577951308232;    // DAT_1421ec2d8
constexpr double kDegToRad = 0.017453292519943295; // DAT_1426992e8
constexpr double kTiny = 1e-20;                    // DAT_1427aaac0

// SfGL2DCurveItem vertex list (+400 doubles, +0x188 count): rows (index, x, y)
struct CurveRows {
    std::vector<double> v;

    // @src 0x1412689c0  append a row (the list grows by 100 rows when full: 0x141268970)
    void add(int index, double x, double y)
    {
        v.push_back(static_cast<double>(index));
        v.push_back(x);
        v.push_back(y);
    }
};

// @src 0x141269a90  number of chords for an arc of radius r over `angle` with chord error tol;
// at least 20
int chordCount(double tol, double r, double angle)
{
    double d = 1.0 - tol / r;
    int n;
    if (d <= 1.0) {
        if (-1.0 <= d) {
            d = std::acos(d);
            double step = 2.0 * d * kRadToDeg;
            if (std::fabs(step) < kTiny) step = 1.0;
            n = static_cast<int>(std::fabs(angle) / step + 0.5);
            if (n < 0x14) n = 0x14;
        } else {
            n = 0x14;
        }
    } else {
        n = 0x14;
    }
    return n;
}

// @src 0x141269180  points of the arc from p (tangent 1) to q (tangent 2) around c
std::vector<double> arcPoints(const double* p, const double* q, const double* c, double tol)
{
    const double ax = p[0] - c[0];
    const double ay = p[1] - c[1];
    const double qx = q[0];
    const double cx = c[0];
    const double qy = q[1];
    const double cy = c[1];
    const double r = std::sqrt(ax * ax + ay * ay);
    double cosv = (ax * (qx - cx) + ay * (qy - cy)) / (r * r);
    const double sg = (ax * (qy - cy) - (qx - cx) * ay) / (r * r) <= 0.0 ? -1.0 : 1.0;
    if (1.0 < std::fabs(cosv)) cosv = cosv <= 0.0 ? -1.0 : 1.0;
    const double ang = std::acos(cosv);
    const int n = chordCount(tol, r, ang);
    std::vector<double> pts;
    if (n < 2) {
        pts = {p[0], p[1], q[0], q[1]};
        return pts;
    }
    pts.assign(static_cast<std::size_t>(n * 2 + 2), 0.0);
    double st = ang / static_cast<double>(n);
    if (ang <= 0.0) st = -st;
    const double sn = std::sin(st);
    const double cs = std::cos(st);
    int k = 2;
    pts[0] = p[0];
    pts[1] = p[1];
    for (int i = 1; i < n; ++i) {
        const double x0 = pts[static_cast<std::size_t>(i * 2 - 2)];
        const double y0 = pts[static_cast<std::size_t>(i * 2 - 1)];
        pts[static_cast<std::size_t>(i * 2)] = (c[0] + (x0 - c[0]) * cs) - sg * (y0 - c[1]) * sn;
        pts[static_cast<std::size_t>(i * 2 + 1)] = c[1] + sg * (x0 - c[0]) * sn + (y0 - c[1]) * cs;
        ++k;
    }
    pts[static_cast<std::size_t>(k * 2 - 2)] = q[0];
    pts[static_cast<std::size_t>(k * 2 - 1)] = q[1];
    pts.resize(static_cast<std::size_t>(k * 2));
    return pts;
}

// tangent points and centre of the fillet of radius p2[2] at p2 between p1 and p3; false when the
// legs are degenerate, parallel or the radius is 0
bool fillet(const double* p1, const double* p2, const double* p3, double t1[2], double t2[2], double c[2])
{
    const double dx1 = p2[0] - p1[0];
    const double dy1 = p2[1] - p1[1];
    const double dx2 = p3[0] - p2[0];
    const double dy2 = p3[1] - p2[1];
    const double l1 = std::sqrt(dx1 * dx1 + dy1 * dy1);
    const double l2 = std::sqrt(dx2 * dx2 + dy2 * dy2);
    if (l1 == 0.0 || l2 == 0.0) return false;
    const double ux = dx1 / l1, uy = dy1 / l1;
    const double vx = dx2 / l2, vy = dy2 / l2;
    const double cr = ux * vy - vx * uy;
    if (cr == 0.0) return false;
    const double sg = cr <= 0.0 ? -1.0 : 1.0;
    const double R = p2[2];
    if (R == 0.0) return false;
    const double Rs = cr <= 0.0 ? -R : R;
    c[0] = p2[0] + (Rs * (vx - ux)) / cr;
    c[1] = p2[1] + (Rs * (vy - uy)) / cr;
    t1[0] = c[0] - R * sg * -uy;
    t1[1] = c[1] - R * sg * ux;
    t2[0] = c[0] - R * sg * -vy;
    t2[1] = c[1] - R * sg * vx;
    return true;
}

// @src 0x141268cd0  fillet arc points (null when fillet() fails)
bool filletPoints(const double* p1, const double* p2, const double* p3, double tol, std::vector<double>& out)
{
    double t1[2], t2[2], c[2];
    if (!fillet(p1, p2, p3, t1, t2, c)) return false;
    out = arcPoints(t1, t2, c, tol);                  // 0x141269180
    return true;
}

// @src 0x141269680  the two tangent points only (the fallback when no arc came out)
bool tangentPoints(const double* p1, const double* p2, const double* p3, std::vector<double>& out)
{
    const double dx1 = p2[0] - p1[0];
    const double dy1 = p2[1] - p1[1];
    const double dx2 = p3[0] - p2[0];
    const double dy2 = p3[1] - p2[1];
    const double l1 = std::sqrt(dx1 * dx1 + dy1 * dy1);
    const double l2 = std::sqrt(dx2 * dx2 + dy2 * dy2);
    if (l1 == 0.0 || l2 == 0.0) return false;
    const double ux = dx1 / l1, uy = dy1 / l1;
    const double vx = dx2 / l2, vy = dy2 / l2;
    const double cr = ux * vy - vx * uy;
    if (cr == 0.0) return false;
    const double sg = cr <= 0.0 ? -1.0 : 1.0;
    const double R = p2[2];
    const double Rs = cr <= 0.0 ? -R : R;
    const double cx = p2[0] + (Rs * (vx - ux)) / cr;
    const double cy = p2[1] + (Rs * (vy - uy)) / cr;
    out = {cx - R * sg * -uy, cy - R * sg * ux, cx - R * sg * -vy, cy - R * sg * vx};
    return true;
}

// @src 0x141268af0  neighbours of point i of an n-point outline (closed when the last row equals
// the first)
void neighbours(int i, int n, int& prev, int& next, const double* pts)
{
    const bool closed = pts[(n - 1) * 3] == pts[0] && pts[(n - 1) * 3 + 1] == pts[1] && pts[(n - 1) * 3 + 2] == pts[2];
    prev = i == 0 ? (closed ? n - 2 : n - 1) : i - 1;
    if (i < n - 1) next = i + 1;
    else next = closed ? 1 : 0;
    if (prev < 1) prev = 0;
    if (!(next < n - 1)) next = n - 1;
}

// @src 0x1412681f0  curve type 1 (rows x, y, r): straight points, filleted corners as arcs
void pointRows(CurveRows& rows, def::model::OBJ_GEO2D* c, double tol)
{
    const int n = c->n_points;
    const double* pts = c->points;
    for (int i = 0; i < n; ++i) {
        if (pts[i * 3 + 2] == 0.0 || n < 3) {
            rows.add(i + 1, pts[i * 3], pts[i * 3 + 1]);
            continue;
        }
        int prev = 0, next = 0;
        neighbours(i, n, prev, next, pts);
        std::vector<double> arc;
        if (filletPoints(pts + prev * 3, pts + i * 3, pts + next * 3, tol, arc) && !arc.empty()) {
            for (std::size_t k = 0; k + 1 < arc.size(); k += 2) rows.add(i + 1, arc[k], arc[k + 1]);
            continue;
        }
        std::vector<double> t;
        if (tangentPoints(pts + prev * 3, pts + i * 3, pts + next * 3, t)) {
            const std::size_t m = t.size() / 2;
            rows.add(i + 1, t[0], t[1]);
            rows.add(i + 1, t[m * 2 - 2], t[m * 2 - 1]);
        }
    }
}

// @src 0x141269bd0  p rotated about c by `deg` degrees (0x140b4e4e0 rotation)
void rotateAbout(const double* c, const double* p, double deg, double* out)
{
    const double dx = p[0] - c[0];
    const double dy = p[1] - c[1];
    const double a = deg * kDegToRad;
    const double sn = std::sin(a);
    const double cs = std::cos(a);
    out[0] = cs * dx - sn * dy;
    out[1] = sn * dx + cs * dy;
    out[0] = out[0] + c[0];                           // 0x140b4e0f0
    out[1] = out[1] + c[1];
}

// @src 0x141268530  curve type 2 (rows kind, x1, y1, x2, y2, a): lines by their ends, arcs from
// their start (x1, y1) about (x2, y2) through a radians in chordCount steps
void segmentRows(CurveRows& rows, def::model::OBJ_GEO2D* c, double tol)
{
    const int n = c->n_points;
    const double* s = c->points;
    for (int i = 0; i < n; ++i) {
        const int kind = static_cast<int>(s[i * 6]);
        if (kind == 1) {
            rows.add(i + 1, s[i * 6 + 1], s[i * 6 + 2]);
            rows.add(i + 1, s[i * 6 + 3], s[i * 6 + 4]);
        } else if (kind == 2) {
            const double ctr[2] = {s[i * 6 + 3], s[i * 6 + 4]};
            const double st[2] = {s[i * 6 + 1], s[i * 6 + 2]};
            const double deg = s[i * 6 + 5] * kRadToDeg;
            rows.add(i + 1, st[0], st[1]);
            const double d[2] = {st[0] - ctr[0], st[1] - ctr[1]};   // 0x140b4e1a0
            const double sq = d[0] * d[0] + d[1] * d[1];            // 0x140b4e400
            const double len = kTiny <= sq ? std::sqrt(sq) : 0.0;
            if (kTiny <= std::fabs(deg) && kTiny <= std::fabs(len)) {
                const int m = chordCount(tol, len, deg);
                for (int k = 1; k <= m; ++k) {
                    double p[2];
                    rotateAbout(ctr, st, (deg / static_cast<double>(m)) * static_cast<double>(k), p);
                    rows.add(i + 1, p[0], p[1]);
                }
            }
        }
    }
}

}  // namespace

void rectReset(Rect2& r)
{
    r = Rect2{};
}

namespace {

// @src 0x141239eb0 / 0x141239fb0  componentwise min / max of pairs (second first; ties take b /
// a's other side as the binary compares)
void pairMin(double* out, const double* a, const double* b)
{
    out[1] = b[1] <= a[1] ? b[1] : a[1];
    out[0] = b[0] <= a[0] ? b[0] : a[0];
}

void pairMax(double* out, const double* a, const double* b)
{
    out[1] = a[1] <= b[1] ? b[1] : a[1];
    out[0] = a[0] <= b[0] ? b[0] : a[0];
}

void extendPair(Rect2& r, const double* q)
{
    if (!r.valid) {
        r.mn[0] = q[0];
        r.mn[1] = q[1];
        r.mx[0] = q[0];
        r.mx[1] = q[1];
        r.valid = true;
        return;
    }
    double lo[2], hi[2];
    pairMin(lo, r.mn, q);
    r.mn[0] = lo[0];
    r.mn[1] = lo[1];
    pairMax(hi, r.mx, q);
    r.mx[0] = hi[0];
    r.mx[1] = hi[1];
}

}  // namespace

void rectExtend(Rect2& r, const double* p)
{
    const double q[2] = {p[0], p[1]};                 // 0x14123a1c0
    extendPair(r, q);
}

void rectExtend(Rect2& r, const float* p)
{
    const double q[2] = {static_cast<double>(p[0]), static_cast<double>(p[1])};   // 0x14123a200
    extendPair(r, q);
}

Rect2 rectUnion(const Rect2& a, const Rect2& b)
{
    if (a.valid) {
        if (b.valid) {
            Rect2 out;
            pairMin(out.mn, a.mn, b.mn);
            pairMax(out.mx, a.mx, b.mx);
            out.valid = true;                         // 0x141238bf0
            return out;
        }
        return a;
    }
    return b;
}

std::vector<double> curveItemPoints(def::model::OBJ_GEO2D* c, double tol)
{
    CurveRows rows;                                   // 0x1412673d0: reset, 0x141268920 sizing
    if (c) {
        if (c->curve_type == 1) pointRows(rows, c, tol * kItemTolFactor);
        else if (c->curve_type == 2) segmentRows(rows, c, tol * kItemTolFactor);
        // 0x141269c50 per-row flags and the item vt+0x48 rect: the rect is rebuilt on read
    }
    return rows.v;
}

Rect2 curveItemRect(const std::vector<double>& rows)
{
    Rect2 r;                                          // 0x141238d00
    for (std::size_t i = 0; i + 2 < rows.size(); i += 3) rectExtend(r, &rows[i + 1]);
    // 0x141239060 derived centre / radius: not read
    return r;
}

void CurveItemState::bind(def::model::OBJ_GEO2D* curve, double tol)
{
    // Preserve the existing converter failure order: build before replacing rows.
    // The production View creates a fresh owner after unbind on each bind.
    vertexRows_ = curveItemPoints(curve, tol);
    vertexCount_ = static_cast<std::int32_t>(vertexRows_.size() / 3);
}

void CurveItemState::reset() noexcept
{
    vertexCount_ = 0;
    vertexRows_.clear();
}

Rect2 CurveItemState::rectangle() const
{
    Rect2 r;
    // 0x141267530 reads this item's signed count and owned vertex rows.
    for (std::int32_t i = 0; i < vertexCount_; ++i)
        rectExtend(r, &vertexRows_[static_cast<std::size_t>(i) * 3 + 1]);
    return r;
}

std::vector<float> meshItemPoints(def::model::LLOBJ_DATA* obj)
{
    std::vector<float> xy;                            // 0x14124bdd0: reset
    if (!obj) return xy;
    // 0x14124bec0(item, 4, elements obj+0x884, nodes obj+0x880, connectivity, coordinates):
    // only the node positions (x, y as floats) reach the rect
    const std::int32_t nElem = *at<std::int32_t>(obj, 0x884);
    const std::int32_t nNode = *at<std::int32_t>(obj, 0x880);
    if (0 < nElem && 0 < nNode) {
        const double* coords =
            def::model::fun_1401808d0(reinterpret_cast<def::model::Cls_14017ef20*>(obj->s8a0));   // 0x140854af0
        xy.reserve(static_cast<std::size_t>(nNode) * 2);
        for (std::int32_t i = 0; i < nNode; ++i)
            for (int k = 0; k < 2; ++k) xy.push_back(static_cast<float>(coords[k + i * 2]));
    }
    return xy;
}

Rect2 meshItemRect(const std::vector<float>& xy)
{
    Rect2 r;
    for (std::size_t i = 0; i + 1 < xy.size(); i += 2) rectExtend(r, &xy[i]);
    return r;
}

}  // namespace m2m::engine
