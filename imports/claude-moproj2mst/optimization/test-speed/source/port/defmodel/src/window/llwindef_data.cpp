// LLWINDEF_DATA: contact window (packet kw-g3). Layout and ownership:
// defmodel/window/llwindef_data.hpp. Spec: docs/re/keywords/FRCWIN.md.
#include <defrt/ftn/helpers.hpp>
#include <defmodel/window/llwindef_data.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/port.hpp>
#include <defmodel/window/inter_object_ops.hpp>

namespace def::model {

namespace {

// constant at 140509fb8: 2.0
constexpr double kHalfDivisor = 2.0;
// constant at 140522fa0: 1e-20
constexpr double kTiny20 = 1e-20;
// constant at 14051f1e0: 1e-10
constexpr double kTiny10 = 1e-10;
// constant at 14051f1b8: 1e-6
constexpr double kMinRadius = 1e-6;
// constant at 140509c90: 1.0
constexpr double kOne = 1.0;
// constant at 14051bed8: 0.0
constexpr double kZero = 0.0;

// 140158e90 (owned by p1-04): DB_WRITE_VAR::dimension getter (+0x1c)
std::int32_t owner_dimension(const DB_STEP_DATA* m) { return m->dimension; }

// @src 1402635d0  geometry kind of a window (4 = shape 3, whose points live in _OBJ_GEO3D)
std::int32_t window_geometry_kind(const LLWINDEF_DATA* w)
{
    std::int32_t k = 0; // uninitialised in the original for other shapes
    if (!window_is_2d(w)) {
        switch (w->shape) {
        case 1: k = 1; break;
        case 2: k = (kTiny20 < w->u1b8 || kTiny20 == w->u1b8) ? 3 : 2; break;
        case 3: k = 4; break;
        case 0xb: k = 0xb; break;
        case 0xc: k = 0xc; break;
        case 0xd: k = 0xd; break;
        case 0xe: k = 0xe; break;
        case 0xf: k = 0xf; break;
        default: break;
        }
    } else if (w->shape == 1) {
        k = 1;
    } else if (w->shape == 2) {
        k = 2;
    } else if (w->shape == 3) {
        k = 3;
    }
    return k;
}

// @src 140263fc0  address of point i (1-based)
double* window_point(LLWINDEF_DATA* w, std::int32_t i)
{
    if (!window_is_2d(w)) {
        if (window_geometry_kind(w) == 4) {
            // 140006410(w+0x220): &coords[0] of the _OBJ_GEO3D for writing (kw-g2geo's port)
            return geo_kw::geo3d_coords(&w->geo3d) + (i + -1) * 3;
        }
        return w->pts + (i + -1) * 3;
    }
    return w->pts + (i + -1) * 2;
}

// @src 140264850  set face centre k
void window_set_face(LLWINDEF_DATA* w, std::int32_t k, double x, double y, double z)
{
    w->faces[k * 3] = x;
    w->faces[k * 3 + 1] = y;
    w->faces[k * 3 + 2] = z;
}

// @src 140264750  face centre k = midpoint of a and b
void window_set_face_mid(LLWINDEF_DATA* w, std::int32_t k, const double* a, const double* b)
{
    double c[3];
    const std::int32_t n = 3;
    for (std::int32_t i = 0; i < n; i = i + 1) c[i] = (a[i] + b[i]) / kHalfDivisor;
    window_set_face(w, k, c[0], c[1], c[2]);
}

// @src 140264800  face centre k = p
void window_set_face_at(LLWINDEF_DATA* w, std::int32_t k, const double* p)
{
    window_set_face(w, k, p[0], p[1], p[2]);
}

// 14023b1f0 c = a - b (p1-12's def::rt::ftn::vec3_sub)
void vsub(const double* a, const double* b, double* c) { def::rt::ftn::vec3_sub(a, b, c); }

// @src 14023b110  c = a + b
void vadd(const double* a, const double* b, double* c)
{
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
}

// @src 14023b3a0  dot product
double vdot(const double* a, const double* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

// 14023b610 length (p1-12's def::rt::ftn::vec3_length)
double vlen(const double* a) { return def::rt::ftn::vec3_length(a); }

// 14023b720 normalise in place, return the length (p1-12's def::rt::ftn::vec3_normalize)
double vnormalize(double* a) { return def::rt::ftn::vec3_normalize(a); }

// @src 1402640f0  unit axis from face 0 to face 1; false when it is shorter than 1e-10
bool window_axis(LLWINDEF_DATA* w, double* dir, bool check)
{
    const double* f0 = window_face(w, 0);
    const double* f1 = window_face(w, 1);
    vsub(f1, f0, dir);
    if (check && vnormalize(dir) < kTiny10) return false;
    return true;
}

// @src 140267490  index of the largest |component| (ties go to the later index)
std::int32_t largest_component(const double* v)
{
    std::int32_t k;
    if (std::fabs(v[0]) <= std::fabs(v[1])) { // 140484558 = fabs
        k = std::fabs(v[1]) <= std::fabs(v[2]) ? 2 : 1;
    } else {
        k = std::fabs(v[0]) <= std::fabs(v[2]) ? 2 : 0;
    }
    return k;
}

// @src 140267560  point at distance r from face 1, perpendicular to the window axis
double* window_rim_point(LLWINDEF_DATA* w, double r)
{
    double* g = g_dat_1406feac0;
    double dir[4];
    if (r < kTiny10) r = kMinRadius;
    if (!window_axis(w, dir, true)) {
        g[0] = r;
        g[1] = 0.0;
        g[2] = 0.0;
    } else {
        const std::int32_t k = largest_component(dir);
        g[0] = kOne;
        g[1] = kOne;
        g[2] = kOne;
        g[k] = 0.0;
        const double d = vdot(dir, g);
        g[k] = -d / dir[k]; // 1402675d0: sign flip by xor with -0.0
        dir[3] = g[0] * g[0] + g[1] * g[1] + g[2] * g[2];
        if (dir[3] != kZero) {
            dir[3] = std::sqrt(dir[3]); // 140484644
            for (std::int32_t i = 0; i < 3; i = i + 1) g[i] = (g[i] / dir[3]) * r;
        }
    }
    const double* f1 = window_face(w, 1);
    vadd(g, f1, g);
    return g;
}

} // namespace

// @src 140263450  set the 3D window shape from a shape code
void window_set_shape(LLWINDEF_DATA* w, std::int32_t code)
{
    if (!window_is_2d(w)) {
        switch (code) {
        case 1: w->shape = 1; break;
        case 2:
        case 3: w->shape = 2; break;
        case 4: w->shape = 3; break;
        case 0xb: w->shape = 0xb; break;
        case 0xc: w->shape = 0xc; break;
        case 0xd: w->shape = 0xd; break;
        case 0xe: w->shape = 0xe; break;
        case 0xf: w->shape = 0xf; break;
        default: break;
        }
    } else if (code == 1) {
        w->shape = 1;
    } else if (code == 2) {
        w->shape = 2;
    } else if (code == 3) {
        w->shape = 3;
    }
}

// ---- window -----------------------------------------------------------------------------------

// @src 14025c100  ctor (fric, the _OBJ_GEO3D at 0x220 (1402f9020) and the DYNARY members are
// constructed as members, in the original's order)
LLWINDEF_DATA::LLWINDEF_DATA(DB_STEP_DATA* owner_)
{
    // 1404797d8 (operator new) does not zero memory; the port zeroes the fields the original leaves
    // unset so that padding is deterministic
    index = 0;
    u174 = 0;
    npts = 0;
    pts = nullptr;
    std::memset(pad_848, 0, sizeof(pad_848));
    dim = 0;
    owner = owner_;
    window_init(this, 0);
}

// @src 14025c6e0  dtor: window_free, the ten DYNARY dtors (0x7c0 down to 0x380, as member
//                 destruction), 1402f91f0 on the _OBJ_GEO3D, 1401fb1a0 (members)
// @src 14026a380  scalar deleting dtor (vtable slot 0): `delete window`
LLWINDEF_DATA::~LLWINDEF_DATA() { window_free(this); }

// @src 14025c350  init
// @src 14025c870  (owner-is-3D test, inlined: both outcomes set shape 1)
// @src 14025c8b0  (w+0x898 setter, inlined)
void window_init(LLWINDEF_DATA* w, std::int32_t kind)
{
    w->next = nullptr;
    w->index = 0;
    w->u174 = kind;
    w->npts = 0;
    w->pts = nullptr;
    w->u838 = 0;
    w->nface = 0;
    w->faces = nullptr;
    w->u890 = 0;
    if (w->owner == nullptr) {
        w->shape = 1;
    } else {
        w->shape = 1; // 14025c3df: same value whether the owner is 3D or not
        const std::int32_t d = owner_dimension(w->owner);
        w->dim = d;
    }
    for (std::int32_t i = 0; i < 2; i = i + 1) {
        for (std::int32_t j = 0; j < 2; j = j + 1) w->u188[j * 2 + i] = 0.0;
        w->u1a8[i] = 0.0;
    }
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        for (std::int32_t j = 0; j < 2; j = j + 1) w->u1c8[j * 3 + i] = 0.0;
        w->u1f8[i] = 0.0;
    }
    w->u1b8 = 0.0;
    w->u1c0 = 0.0;
    w->geo3d.reset(); // 14025c57d: _OBJ_GEO3D vtable slot 2 (1402f92b0) on 0x220
    for (std::int32_t i = 0; i < 3; i = i + 1) w->u350[i] = 0.0;
    w->orient = 0;
    w->orient_id = 0;
    w->u370 = 0;
    w->u378 = 0;
    w->u560 = 0;
    for (DYNARY& t : w->t380) def::model::dynary_init(&t, 0); // 14012d2a0
    for (DYNARY& t : w->t568) def::model::dynary_init(&t, 0);
}

// @src 14025d0a0  release the point/face arrays and the tables' arrays
void window_free(LLWINDEF_DATA* w)
{
    window_free_points(w); // 140263360
    window_free_faces(w);  // 1402633d0
    w->geo3d.clear(); // 1402f9340 on 0x220
    w->u838 = 0;
    w->u560 = 0;
    for (DYNARY& t : w->t380) def::model::dynary_free_arrays(&t); // 14012f480
    for (DYNARY& t : w->t568) def::model::dynary_free_arrays(&t);
}

// @src 14025d1a0  free and re-init, keeping the index and the list link
void window_reset(LLWINDEF_DATA* w, std::int32_t kind)
{
    const std::int32_t index = w->index;
    LLWINDEF_DATA* next = w->next;
    window_free(w);
    window_init(w, kind);
    w->index = index;
    w->next = next;
}

// @src 14025c9b0  copy (the faces are not copied: dst->nface is 0 after the free, so the memcpy
// moves 0 bytes and dst->faces stays NULL until window_face rebuilds them)
void window_copy(LLWINDEF_DATA* dst, const LLWINDEF_DATA* src)
{
    window_free(dst);
    dst->index = src->index;
    dst->u174 = src->u174;
    dst->dim = src->dim;
    dst->u890 = src->u890;
    const std::int32_t bytes = window_alloc_points(dst, src->npts);
    if (bytes != 0) {
        std::memcpy(dst->pts, src->pts, static_cast<std::size_t>(bytes));
        if (window_is_3d(dst) && src->faces != nullptr && dst->nface * 0x18 != 0) {
            std::memcpy(dst->faces, src->faces, static_cast<std::size_t>(dst->nface * 0x18));
        }
    }
    // 14025cacd: 3D and the source _OBJ_GEO3D has nodes and elements (1402f9360): 1402f9b60
    if (window_is_3d(dst) && geo_kw::surface_has_mesh(const_cast<OBJ_GEO3D*>(&src->geo3d)))
        dst->geo3d.assign(const_cast<OBJ_GEO3D*>(&src->geo3d));
    dst->shape = src->shape;
    for (std::int32_t i = 0; i < 2; i = i + 1) {
        for (std::int32_t j = 0; j < 2; j = j + 1) dst->u188[j * 2 + i] = src->u188[j * 2 + i];
        dst->u1a8[i] = src->u1a8[i];
    }
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        for (std::int32_t j = 0; j < 2; j = j + 1) dst->u1c8[j * 3 + i] = src->u1c8[j * 3 + i];
        dst->u1f8[i] = src->u1f8[i];
    }
    if (window_is_3d(dst)) dst->u1b8 = src->u1b8;
    dst->u1c0 = src->u1c0;
    for (std::int32_t i = 0; i < 3; i = i + 1) dst->u350[i] = src->u350[i];
    dst->orient = src->orient;
    dst->orient_id = src->orient_id;
    dst->fric.u000 = src->fric.u000;
    dynary_copy(&dst->fric.u018, &src->fric.u018); // 14012eff0
    dynary_copy(&dst->fric.u090, &src->fric.u090);
    dst->fric.u004 = src->fric.u004;
    dst->fric.u005 = src->fric.u005;
    dst->fric.u008 = src->fric.u008;
    dst->fric.u00c = src->fric.u00c;
    dst->fric.u010 = src->fric.u010;
    dst->fric.u014 = src->fric.u014;
    for (int i = 0; i < 12; i = i + 1) dst->fric.u108[i] = src->fric.u108[i]; // 0x110..0x168
    dst->u370 = src->u370;
    dst->u378 = src->u378;
    for (int i = 0; i < 4; i = i + 1) dynary_copy(&dst->t380[i], &src->t380[i]);
    dynary_copy(&dst->t568[0], &src->t568[0]);
    dst->u560 = src->u560;
    for (int i = 1; i < 6; i = i + 1) dynary_copy(&dst->t568[i], &src->t568[i]);
    dst->u838 = src->u838;
}

// @src 14025c930  2D window (+0x898 == 2)
bool window_is_2d(const LLWINDEF_DATA* w) { return w->dim == 2; }
// @src 14025c970  3D window (+0x898 == 3)
bool window_is_3d(const LLWINDEF_DATA* w) { return w->dim == 3; }

// @src 140263cb0  (re)allocate n zeroed points; returns the byte size
std::int32_t window_alloc_points(LLWINDEF_DATA* w, std::int32_t n)
{
    window_free_points(w);
    w->npts = n;
    if (w->npts == 0) return 0;
    const std::int32_t per = window_is_2d(w) ? 2 : 3;
    const std::int32_t bytes = per * w->npts * 8;
    w->pts = static_cast<double*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(bytes))));
    for (std::int32_t i = 0; i < w->npts; i = i + 1) {
        for (std::int32_t j = 0; j < per; j = j + 1) w->pts[i * per + j] = 0.0;
    }
    return bytes;
}

// @src 140263360  free the points
void window_free_points(LLWINDEF_DATA* w)
{
    if (w->pts != nullptr) std::free(w->pts);
    w->pts = nullptr;
    w->npts = 0;
    w->pts = nullptr;
}

// @src 1402633d0  free the face centres (only when nface != 0)
void window_free_faces(LLWINDEF_DATA* w)
{
    if (w->nface != 0) {
        if (w->faces != nullptr) std::free(w->faces);
        w->faces = nullptr;
        w->nface = 0;
        w->faces = nullptr;
    }
}

// @src 140263dc0  allocate the face centres of a 3D window (n == 0: count from the shape)
std::int32_t window_alloc_faces(LLWINDEF_DATA* w, std::int32_t n)
{
    std::int32_t bytes = 0;
    const std::int32_t per = window_is_2d(w) ? 2 : 3;
    const std::int32_t s = w->shape;
    if (window_is_3d(w) && 0 < s && (s < 4 || (0xd < s && s < 0x10))) {
        window_free_faces(w);
        if (n == 0) {
            const std::int32_t np = w->npts;
            if (np == 0) {
                w->nface = 0;
                return 0;
            }
            if (np == 2) {
                w->nface = (kTiny20 < w->u1b8 || kTiny20 == w->u1b8) ? 4 : 3;
            } else if (np == 8) {
                w->nface = 6;
            } else {
                w->nface = 6;
            }
        } else {
            w->nface = n;
        }
        bytes = per * w->nface * 8;
        w->faces = static_cast<double*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(bytes))));
        for (std::int32_t i = 0; i < w->nface; i = i + 1) {
            for (std::int32_t j = 0; j < per; j = j + 1) w->faces[i * per + j] = 0.0;
        }
    }
    return bytes;
}

// @src 140267760  face centres of a box (shape 1: midpoints of opposite corners) or a cylinder
// (shape 2: the two axis points and the rim points at radius +0x1c0 and +0x1b8)
void window_build_faces(LLWINDEF_DATA* w)
{
    if (w->shape != 1 && w->shape != 2) return;
    if (window_alloc_faces(w, 0) == 0) return;
    const std::int32_t s = w->shape;
    if (s == 1 || s == 0xe) {
        window_set_face_mid(w, 0, window_point(w, 2), window_point(w, 7));
        window_set_face_mid(w, 1, window_point(w, 1), window_point(w, 8));
        window_set_face_mid(w, 2, window_point(w, 3), window_point(w, 8));
        window_set_face_mid(w, 3, window_point(w, 2), window_point(w, 5));
        window_set_face_mid(w, 4, window_point(w, 5), window_point(w, 7));
        window_set_face_mid(w, 5, window_point(w, 1), window_point(w, 3));
    } else if (s == 2 || s == 0xf) {
        window_set_face_at(w, 0, window_point(w, 1));
        window_set_face_at(w, 1, window_point(w, 2));
        window_set_face_at(w, 2, window_rim_point(w, w->u1c0));
        if (w->nface == 4) window_set_face_at(w, 3, window_rim_point(w, w->u1b8));
    }
}

// @src 140264090  face centre i (built on first use)
const double* window_face(LLWINDEF_DATA* w, std::int32_t i)
{
    if (w->faces == nullptr) window_build_faces(w);
    return w->faces + i * 3;
}

} // namespace def::model
