// OBJPOS 3D mirror: the single-precision matrix class and the per-object-part mirror routines that
// use it (OBJPOS.md 6.5). Every routine converts its plane point and normal to float, builds the
// matrix with 140240340, converts each point to float, applies 140240980 and widens the result
// back to double. All float operations are separate statements so each rounds to float exactly
// where the binary's SSE code (listings l_240340, l_240220, l_240980, l_239960) does.
#include <defcmd/objpos_mirror.hpp>
#include <defcmd/vec3.hpp>

#include <cstdint>

#include <defmodel/bc/window_bc.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/rdata.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

namespace {

// rdata 14051f260  1e-12: "normal is zero" tolerance of every mirror routine
constexpr double kTiny = def::rt::rdata::d_14051f260;
// @data 140530be0  2.0f
constexpr float kTwoF = 2.0f;
// @data 14053176c  1.0f
constexpr float kOneF = 1.0f;

// @data 1405e3070  identity (float 4x4)
F44 identity_f()
{
    F44 m;
    for (std::int32_t k = 0; k < 16; k = k + 1) m.m[k] = (k % 5 == 0) ? 1.0f : 0.0f;
    return m;
}

// x ^ 0x80000000 (DAT_14052c5b0): sign flip
inline float neg(float x) { return -x; }

} // namespace

// @src 140239630  Vector3f ctor (the three components as given)
F3 f3(float x, float y, float z) { return F3{x, y, z}; }

// @src 140239960  squared length: (x*x + y*y) + z*z (MULSS / ADDSS)
float f3_len2(const F3* v)
{
    float s = v->x * v->x;
    float t = v->y * v->y;
    s = s + t;
    t = v->z * v->z;
    s = s + t;
    return s;
}

// @src 14023fdf0  m(0,3) += x, m(1,3) += y, m(2,3) += z
void f44_add_translation(F44* m, float x, float y, float z)
{
    m->m[3] = m->m[3] + x;
    m->m[7] = m->m[7] + y;
    m->m[11] = m->m[11] + z;
}

// @src 140240220  out = a * b: acc starts as the zero matrix 0x1406fe9f0 (never written),
// acc(i,j) = acc(i,j) + a(i,k) * b(k,j) for i, j, k ascending
void f44_mul(F44* out, const F44* a, const F44* b)
{
    F44 acc;
    for (std::int32_t k = 0; k < 16; k = k + 1) acc.m[k] = 0.0f;
    for (std::int32_t i = 0; i < 4; i = i + 1) {
        for (std::int32_t j = 0; j < 4; j = j + 1) {
            for (std::int32_t k = 0; k < 4; k = k + 1) {
                const float prod = a->m[i * 4 + k] * b->m[k * 4 + j];
                acc.m[i * 4 + j] = acc.m[i * 4 + j] + prod;
            }
        }
    }
    *out = acc;
}

// @src 140240340  plane mirror: H = I - (2 / n.n) n n^T (float), out = H * T(-p), then the
// translation column += p
void f44_mirror(F44* out, const F3* p, const F3* n)
{
    F44 t = identity_f();  // rsp+0x20
    F44 h = identity_f();  // rsp+0x70
    float k = f3_len2(n);
    k = kTwoF / k;
    float s;
    s = k * n->x;
    s = s * n->x;
    h.m[0] = kOneF - s;
    s = k * n->y;
    s = s * n->y;
    h.m[5] = kOneF - s;
    s = k * n->z;
    s = s * n->z;
    h.m[10] = kOneF - s;
    s = neg(k) * n->x;
    s = s * n->y;
    h.m[1] = s;
    h.m[4] = h.m[1];
    s = neg(k) * n->x;
    s = s * n->z;
    h.m[2] = s;
    h.m[8] = h.m[2];
    s = neg(k) * n->y;
    s = s * n->z;
    h.m[6] = s;
    h.m[9] = h.m[6];
    h.m[14] = 0.0f;
    h.m[13] = h.m[14];
    h.m[12] = h.m[13];
    f44_add_translation(&t, neg(p->x), neg(p->y), neg(p->z));
    f44_mul(out, &h, &t);
    f44_add_translation(out, p->x, p->y, p->z);
}

// @src 140240980  q_i = ((M(i,0)*x + M(i,1)*y) + M(i,2)*z) + M(i,3)
void f44_apply(const F3* v, F3* q, const F44* m)
{
    float r[3];
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        float s = m->m[i * 4 + 0] * v->x;
        float t = m->m[i * 4 + 1] * v->y;
        s = s + t;
        t = m->m[i * 4 + 2] * v->z;
        s = s + t;
        s = s + m->m[i * 4 + 3];
        r[i] = s;
    }
    q->x = r[0];
    q->y = r[1];
    q->z = r[2];
}

F44 mirror_matrix(const double* p, const double* n)
{
    F3 pf = f3(static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]));
    F3 nf = f3(static_cast<float>(n[0]), static_cast<float>(n[1]), static_cast<float>(n[2]));
    F44 m;
    f44_mirror(&m, &pf, &nf);
    return m;
}

void mirror_point_f(double* p, const F44* m)
{
    F3 v = f3(static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]));
    F3 q;
    f44_apply(&v, &q, m);
    p[0] = static_cast<double>(q.x);
    p[1] = static_cast<double>(q.y);
    p[2] = static_cast<double>(q.z);
}

// @src 1402fbbe0  _OBJ_GEO3D mirror: when n is not ~0 and there are faces and points: every point
// (written through 140005e70), then the faces reversed (1402fbf60) and the point list +0x28
// mirrored (140258500)
void geo3d_mirror(model::OBJ_GEO3D* g, const double* p, const double* n)
{
    const double zero[3] = {0.0, 0.0, 0.0};
    if (v3::near(n, zero, kTiny)) return;
    const std::int32_t faces = g->elem_count();   // 140003500
    const std::int32_t points = g->node_count();  // 1400034d0
    if (0 < faces && 0 < points) {
        F44 m = mirror_matrix(p, n);
        for (std::int32_t i = 0; i < points; i = i + 1) {
            const double z = g->coords.mutable_at(static_cast<std::size_t>(i * 3 + 2));
            const double y = g->coords.mutable_at(static_cast<std::size_t>(i * 3 + 1));
            const double x = g->coords.mutable_at(static_cast<std::size_t>(i * 3));
            F3 v = f3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
            F3 q;
            f44_apply(&v, &q, &m);
            g->coords.mutable_at(static_cast<std::size_t>(i * 3)) = static_cast<double>(q.x);
            g->coords.mutable_at(static_cast<std::size_t>(i * 3 + 1)) = static_cast<double>(q.y);
            g->coords.mutable_at(static_cast<std::size_t>(i * 3 + 2)) = static_cast<double>(q.z);
        }
        g->reverse_elements();  // 1402fbf60
        axis_list_mirror(reinterpret_cast<model::linked::Cls_140257340List*>(g->u28), p, n);
    }
}

// @src 1402578d0  axis-point record: point +8 and vector +0x20 mirrored as points (the vector is
// not made relative to the mirrored origin)
void axis_point_mirror(model::linked::Cls_140257340* r, const double* p, const double* n)
{
    const double zero[3] = {0.0, 0.0, 0.0};
    if (v3::near(n, zero, kTiny)) return;
    F44 m = mirror_matrix(p, n);
    mirror_point_f(r->point, &m);
    mirror_point_f(r->direction, &m);
}

// @src 140258500  every record of the list (count 140158e70, record 140258090)
void axis_list_mirror(model::linked::Cls_140257340List* l, const double* p, const double* n)
{
    for (std::int32_t k = 0; k < l->size(); k = k + 1) {
        model::linked::Cls_140257340* r = l->nth(k + 1);
        if (r != nullptr) axis_point_mirror(r, p, n);
    }
}

// @src 140268930  window points (3 doubles each) when the window has a shape (1402632d0), p and n
// are given and n is not ~0. No face refresh (unlike translate / rotate / 4x4).
void window_mirror_3d(model::LLWINDEF_DATA* w, const double* p, const double* n)
{
    const double zero[3] = {0.0, 0.0, 0.0};
    if (!model::window_has_shape(w) || p == nullptr || n == nullptr || v3::near(n, zero, kTiny)) return;
    F44 m = mirror_matrix(p, n);
    for (std::int32_t i = 0; i < w->npts; i = i + 1) mirror_point_f(w->pts + i * 3, &m);
}

// @src 14026a0a0  every window of the list (count 14019a590, window 140269900)
void winlist_mirror_3d(void* list, const double* p, const double* n)
{
    auto* l = static_cast<model::Cls_140269250*>(list);
    for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
        model::LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
        if (w != nullptr) window_mirror_3d(w, p, n);
    }
}

} // namespace def::cmd::objpos
