// GENAXS (id 19, case 0x13 of FUN_140378e00), packet p67-5. Spec: docs/re/commands/GENAXS.md.
//
// Cogging set-up for the fixed object layout 7 (1 workpiece, 2 die, 4/5 and 6/7 gripper pairs):
// undo the gripper springs, open every pair that is not clear of the die, and replace the
// workpiece's type-7 WPAXIS records. Floating point: every operation has at most two operands
// (listing 0x1401af680..0x1401b0593); NaN results follow the listing's dest/src order through
// def::rt::sse.
//
// Missing objects: the original looks the six objects up and never tests for NULL. The port
// follows the same path and, at the first instruction that would read through a NULL object,
// ends the process the way the crash does (def::rt::abnormal_exit, STATUS_ACCESS_VIOLATION, no
// stdio flush). A missing object the path never touches (object 7 when pair 6/7 neither springs
// back nor moves) is harmless, as in the original (oracle cases GENAXS/hand_clear, hand_open4).
#include <defcmd/p67_5.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/vec3.hpp>
#include <defcmd/objpos.hpp>

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <defdbio/dbread/w1.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftn/sse_ops.hpp>
#include <defrt/stdlib/crt_emulation.hpp>

namespace def::cmd {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::OBJ_GEO3D;
using model::linked::Cls_140258580;
using model::linked::Cls_140258580List;

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// The original reads through `o` at instruction `addr`; NULL ends the process like the access
// violation does.
LLOBJ_DATA& deref(LLOBJ_DATA* o, std::uint64_t addr)
{
    (void)addr;
    if (o == nullptr) def::rt::abnormal_exit(def::rt::kStatusAccessViolation);
    return *o;
}

// FUN_14021d0b0(obj, v, &flagA, &flagB) on an object pointer the original does not test: the
// callee's first read is obj+0x39c0 (140158d00 -> 140158ce0), before its zero-vector test.
void translate(LLOBJ_DATA* o, const double* v, char flag_a, char flag_b)
{
    objpos::translate(&deref(o, 0x140158ce0), v, &flag_a, &flag_b);
}

} // namespace

// 140181ae0: ported in defcmd/objpos.hpp (objpos::node_bbox; listing 140181c48..c7c: COMISD/JBE, a NaN never enters the box)
// components, ndim = *(14017f020(set)) = owner model (+0x130) +0x15f8. The first node sets lo and
// hi; later ones: v < lo -> lo = v, else v > hi -> hi = v (a NaN goes to lo). Without an owner
// model or coordinates (+8) nothing is written and the result is false.
bool node_set_bbox(void* set, double* lo, double* hi)
{
    return objpos::node_bbox(set, lo, hi);
}

// 1402faca0: ported in defcmd/objpos_centre.hpp (objpos::surface_bbox)
// (DAT_14052c5a0), then per point and axis p < lo -> lo = p, p > hi -> hi = p (points through
// 14026a470 = coords first + index, count 1400034d0 = coords size / 3)
void surface_bbox(const OBJ_GEO3D* s, double* lo, double* hi)
{
    objpos::surface_bbox(const_cast<OBJ_GEO3D*>(s), lo, hi);
}

// 14023b2d0: ported in defcmd/vec3.hpp (v3::scale)
void vec3_scale(const double* a, double s, double* c)
{
    v3::scale(a, s, c);
}

// @src 140259a40  unlink `node` from the list and delete it (14025bf40 = scalar deleting dtor);
// false when it is not in the list. As in the original, removing the head copies the head's own
// link into itself and, when the head is also the tail, leaves the tail on the deleted node (the
// count is then 0, so the next append resets head and tail).
bool wpaxis_remove(Cls_140258580List& l, Cls_140258580* node)
{
    Cls_140258580* prev = l.head;
    Cls_140258580* cur = l.head;
    while (cur != nullptr && cur != node) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    prev->next = cur->next;
    if (l.head == cur) l.head = cur->next;
    if (l.tail == cur) l.tail = prev;
    delete cur;
    --l.count;
    return true;
}

// @src 140259ec0  delete every record of type 7 (+4): count them, then remove the first type-7
// record per pass until the count is used up or a removal fails
void wpaxis_remove_type7(Cls_140258580List& l)
{
    std::int32_t n = 0;
    for (Cls_140258580* r = l.head; r != nullptr; r = r->next)
        if (at<std::int32_t>(r, 4) == 7) ++n;
    while (n != 0) {
        bool ok = true;
        for (Cls_140258580* r = l.head; r != nullptr; r = r->next) {
            if (at<std::int32_t>(r, 4) == 7) {
                ok = wpaxis_remove(l, r);
                --n;
                break;
            }
        }
        if (!ok) return;
    }
}

// @src 140259e60  first record with type (+4) 7 and id (+0) `id`, or null
Cls_140258580* wpaxis_find_type7(Cls_140258580List& l, std::int32_t id)
{
    Cls_140258580* r = l.head;
    while (r != nullptr && (at<std::int32_t>(r, 4) != 7 || r->id != id)) r = r->next;
    return r;
}

// @src 140259df0  the type-7 record `id`; if there is none, append a record (140259950 with id 0,
// so +0 = the new count) and set +4 = 7, +0 = id
Cls_140258580* wpaxis_type7(Cls_140258580List& l, std::int32_t id)
{
    Cls_140258580* r = wpaxis_find_type7(l, id);
    if (r == nullptr) {
        r = model::geo_kw::wpaxis_add(l, 0);
        at<std::int32_t>(r, 4) = 7;
        r->id = id;
    }
    return r;
}

// 140258ed0 / 140258f50: one body each, owner DBREAD W1 (defdbio/dbread/w1.hpp body_force_set_08 /
// body_force_set_20; p9/p10 integration)
void wpaxis_set_origin(Cls_140258580* r, const double* v) { def::dbio::dbread::body_force_set_08(r, v); }
void wpaxis_set_direction(Cls_140258580* r, const double* v) { def::dbio::dbread::body_force_set_20(r, v); }

namespace {

// Bounding box of `o` by its die flag (140160d90): surface 0 (140198620) or the node set.
void object_bbox(LLOBJ_DATA& o, double* lo, double* hi)
{
    if (!model::geo_kw::has_surface_flag(o)) {
        node_set_bbox(&at<std::byte>(&o, 0x8a0), lo, hi);
    } else {
        surface_bbox(model::geo_kw::surface(o, 0), lo, hi);
    }
}

// Undo the spring pre-displacement of the pair (a, b) with a's spring value (+0x2588) and
// movement direction (+0x21b0); v keeps the last vector.
void spring_reset(LLOBJ_DATA* a_p, LLOBJ_DATA* b_p, double* v, std::uint64_t read_addr)
{
    LLOBJ_DATA& a = deref(a_p, read_addr);
    if (std::fabs(at<double>(&a, 0x2588)) > 1e-6) {  // FUN_140484558 = fabs; DAT_14051f1b8; COMISD/JBE
        v[0] = rt::sse::mul(-at<double>(&a, 0x2588), at<double>(&a, 0x21b0));  // XORPD sign mask, MULSD
        v[1] = rt::sse::mul(-at<double>(&a, 0x2588), at<double>(&a, 0x21b8));
        v[2] = rt::sse::mul(-at<double>(&a, 0x2588), at<double>(&a, 0x21c0));
        translate(&a, v, 0, 1);
        at<double>(&a, 0x2588) = 0.0;
        at<double>(&a, 0x2300) = 0.0;  // FORCE
        at<double>(&a, 0x2308) = 0.0;
        at<double>(&a, 0x2310) = 0.0;
        translate(b_p, v, 0, 1);
        at<double>(b_p, 0x2588) = 0.0;
        at<double>(b_p, 0x2300) = 0.0;
        at<double>(b_p, 0x2308) = 0.0;
        at<double>(b_p, 0x2310) = 0.0;
    }
}

} // namespace

// @src 1401af680  GENAXS body (model, layout, tolA, tolB)
void genaxs(DB_STEP_DATA& m, std::int32_t layout, double tol_a, double tol_b)
{
    if (layout != 7) return;
    LLOBJ_DATA* w = key::g2obj::find_object(m, 1);  // 1401d6ea0 for 1, 2, 4, 5, 6, 7
    LLOBJ_DATA* d = key::g2obj::find_object(m, 2);
    LLOBJ_DATA* g4 = key::g2obj::find_object(m, 4);
    LLOBJ_DATA* g5 = key::g2obj::find_object(m, 5);
    LLOBJ_DATA* g6 = key::g2obj::find_object(m, 6);
    LLOBJ_DATA* g7 = key::g2obj::find_object(m, 7);

    // 1. springs: the workpiece follows the last pair reset (v stays 0 otherwise, which the
    //    translation's zero test ignores)
    double v[3] = {0.0, 0.0, 0.0};
    spring_reset(g4, g5, v, 0x1401af7b4);
    spring_reset(g6, g7, v, 0x1401af974);
    translate(w, v, 0, 1);

    // 2. bounding boxes; lo/hi are the same two locals for every call, so an object without a
    //    mesh leaves the previous call's values (the original's first call would see
    //    uninitialised stack; the port starts from 0)
    double lo[3] = {0.0, 0.0, 0.0};
    double hi[3] = {0.0, 0.0, 0.0};
    object_bbox(deref(d, 0x140160d90), lo, hi);
    const double d_lo = lo[0];
    const double d_hi = hi[0];
    object_bbox(*g4, lo, hi);
    double c4[3];
    c4[0] = rt::sse::mul(rt::sse::add(lo[0], hi[0]), 0.5);  // DAT_14051f240
    c4[1] = rt::sse::mul(rt::sse::add(lo[1], hi[1]), 0.5);
    c4[2] = rt::sse::mul(rt::sse::add(lo[2], hi[2]), 0.5);
    const double a4_lo = lo[0];
    const double a4_hi = hi[0];
    object_bbox(*g6, lo, hi);
    const double a6_lo = lo[0];
    const double a6_hi = hi[0];

    // 3. overlap in X (COMISD: a NaN compares false)
    bool blocked_a = false;
    bool blocked_b = false;
    if (a4_hi <= d_hi || a4_hi <= d_lo || a4_lo <= d_hi || a4_lo <= d_lo) blocked_a = true;
    if (d_hi <= a6_hi || d_lo <= a6_hi || d_hi <= a6_lo || d_lo <= a6_lo) blocked_b = true;

    // 4. centre distance
    const double cd = rt::sse::mul(0.5, rt::sse::add(d_hi, d_lo));
    const double c4x = rt::sse::mul(0.5, rt::sse::add(a4_hi, a4_lo));
    const double c6x = rt::sse::mul(0.5, rt::sse::add(a6_hi, a6_lo));
    if (std::fabs(rt::sse::sub(cd, c4x)) <= tol_a) blocked_a = true;
    if (std::fabs(rt::sse::sub(cd, c6x)) <= tol_b) blocked_b = true;

    // 5. workpiece height from its node set
    node_set_bbox(&at<std::byte>(w, 0x8a0), lo, hi);
    const double h = std::fabs(rt::sse::sub(hi[1], lo[1]));

    // 6. opening vectors along the direction from gripper 4 to gripper 5
    object_bbox(deref(g5, 0x140160d90), lo, hi);
    double c5[3];
    c5[0] = rt::sse::mul(rt::sse::add(lo[0], hi[0]), 0.5);
    c5[1] = rt::sse::mul(rt::sse::add(lo[1], hi[1]), 0.5);
    c5[2] = rt::sse::mul(rt::sse::add(lo[2], hi[2]), 0.5);
    double u[3];
    double wv[3];
    rt::ftn::vec3_sub(c5, c4, u);   // 14023b1f0
    rt::ftn::vec3_normalize(u);     // 14023b720
    vec3_scale(u, -h, u);           // 14023b2d0 with h XOR sign mask
    vec3_scale(u, -1.0, wv);        // DAT_140522fc8

    // 7. drop the workpiece's type-7 WPAXIS records
    Cls_140258580List& axes = at<Cls_140258580List>(w, 0x20a0);
    wpaxis_remove_type7(axes);

    // 8. open the blocked pairs, record the clear ones
    if (!blocked_a || !blocked_b) {
        std::int32_t p1a = 0, p1b = 0, p2a = 0, p2b = 0;
        double r1_lo = 0.0, r1_hi = 0.0, r2_lo = 0.0, r2_hi = 0.0;
        if (!blocked_a && !blocked_b) {
            p1a = 4;
            p1b = 5;
            r1_lo = a4_lo;
            r1_hi = a4_hi;
            p2a = 6;
            p2b = 7;
            r2_lo = a6_lo;
            r2_hi = a6_hi;
        } else if (!blocked_a) {
            p1a = 4;
            p1b = 5;
            r1_lo = a4_lo;
            r1_hi = a4_hi;
            translate(g6, u, 1, 1);
            translate(g7, wv, 1, 1);
        } else if (!blocked_b) {
            p1a = 6;
            p1b = 7;
            r1_lo = a6_lo;
            r1_hi = a6_hi;
            translate(g4, u, 1, 1);
            translate(g5, wv, 1, 1);
        }
        // 9. the records
        Cls_140258580* r = wpaxis_type7(axes, 1);
        if (r != nullptr) {
            const double origin[3] = {0.0, 0.0, 0.0};
            const double dir[3] = {1.0, 0.0, 0.0};  // _DAT_140509c90
            wpaxis_set_origin(r, origin);
            wpaxis_set_direction(r, dir);
            at<double>(r, 0x40) = rt::sse::mul(0.5, h);
            at<std::int32_t>(r, 0xc8) = p1a;
            at<std::int32_t>(r, 0xcc) = p1b;
            at<double>(r, 0xd0) = r1_lo;
            at<double>(r, 0xd8) = r1_hi;
            if (0 < p2a) {
                r = wpaxis_type7(axes, 2);
                if (r != nullptr) {
                    wpaxis_set_origin(r, origin);
                    wpaxis_set_direction(r, dir);
                    at<double>(r, 0x40) = rt::sse::mul(0.5, h);
                    at<std::int32_t>(r, 0xc8) = p2a;
                    at<std::int32_t>(r, 0xcc) = p2b;
                    at<double>(r, 0xd0) = r2_lo;
                    at<double>(r, 0xd8) = r2_hi;
                }
            }
        }
    } else {
        translate(g4, u, 1, 1);
        translate(g5, wv, 1, 1);
        translate(g6, u, 1, 1);
        translate(g7, wv, 1, 1);
    }
}

// part of @src 140378e00: dispatcher case 0x13 (listing 0x14037dc56..0x14037dcc0): layout = 0, then
// FUN_140415680 (%i %Flf %Flf, outputs zeroed first); its result is not stored, and the test
// after the call reads the dispatcher status, which is 0 here, so a bad line is never reported
int cmd_genaxs(DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t layout = 0;
    double tol_a = 0.0;
    double tol_b = 0.0;
    key::read_i_dd(f, &layout, &tol_a, &tol_b);
    genaxs(m, layout, tol_a, tol_b);
    return 0;
}

} // namespace def::cmd
