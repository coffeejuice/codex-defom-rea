// OBJPOS: case 7 of the command dispatcher FUN_140378e00 (out/bm_cmd.c lines 582-1433).
// Spec: docs/re/commands/OBJPOS.md sections 2-5, 7, 8. The interference modes (2, 12, 102, 112)
// are packet p67-2's (defcmd/objpos_ifd.hpp) and the drop modes (7, 8, 9) p67-3's
// (defcmd/objpos_drop.hpp); until their headers are on the branch those modes stop in
// DEF_NOT_PORTED.
#include <defcmd/cmd_objpos.hpp>
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/objpos_parts.hpp>
#include <defcmd/vec3.hpp>

#include <cmath>
#include <cstdint>
#include <cstdlib>

#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#if __has_include(<defcmd/objpos_ifd.hpp>)
#include <defcmd/objpos_ifd.hpp>
#define DEF_P67_IFD 1
#endif
#if __has_include(<defcmd/objpos_drop.hpp>)
#include <defcmd/objpos_drop.hpp>
#define DEF_P67_DROP 1
#endif

#include "internal.hpp"

namespace def::cmd::objpos {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using namespace detail;
namespace crt = def::rt::crt;

namespace {

// 1401d6ea0: object `n` of the object list (lookup only)
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t n) { return model::object_list_find(&m.objects, n); }

std::int32_t obj_no(const LLOBJ_DATA* o) { return at<std::int32_t>(o, 8); }

// part of @src 140378e00  reference-object lookup of modes 13/14 (OBJPOS.md section 4 step 5):
// null -> 3 silently, type 7 -> message and 3, no geometry and no mesh -> message and 3
std::int32_t check_ref(LLOBJ_DATA* r)
{
    if (r == nullptr) return 3;
    if (at<std::int32_t>(r, 0xc) == 7) {
        crt::printf("ERROR  : Object %d Undefined\n", obj_no(r));
        return 3;
    }
    if (!has_geometry(r) && at<std::int32_t>(r, 0x87c) == 0) {
        crt::printf("ERROR  : No Mesh or Die Defined for Object %d\n", obj_no(r));
        return 3;
    }
    return 0;
}

// part of @src 140378e00  bounding box of an object for modes 13/14: geometry 0 when +0x3990
// (1402faca0), else the nodes (140181ae0, result ignored)
void object_box(LLOBJ_DATA* o, double* mn, double* mx)
{
    if (!model::geo_kw::has_surface_flag(*o)) {
        node_bbox(ptr(o, 0x8a0), mn, mx);
    } else {
        surface_bbox(model::geo_kw::surface(*o, 0), mn, mx);
    }
}

// part of @src 140378e00  3D mode 13 (OBJPOS.md 7.5); returns 0 or 3
std::int32_t mode13(DB_STEP_DATA& m, LLOBJ_DATA* obj, Args& a, State& st)
{
    LLOBJ_DATA* ref = find_object(m, std::abs(static_cast<std::int32_t>(a.v[4])));  // 140484ab8
    if (const std::int32_t rc = check_ref(ref)) return rc;
    double D[3] = {a.v[1], a.v[2], a.v[3]};  // local_5330
    double t = 0.0;                          // local_5338
    double d2 = 0.0;                         // local_5398
    const std::int32_t sub = static_cast<std::int32_t>(a.v[5]);  // local_5418
    const double val = a.v[6];                                    // local_5390
    double rmin[3] = {}, rmax[3] = {};                            // local_5388, local_5358
    if (!node_bbox(ptr(ref, 0x8a0), rmin, rmax)) {
        crt::printf("Object %d to Object %d reference positioning failed.\n", obj_no(obj), obj_no(ref));
        st.moved = false;
        return 0;
    }
    double& gx = g_last_disp[0];
    double& gy = g_last_disp[1];
    double& gz = g_last_disp[2];
    double bmin[3] = {}, bmax[3] = {};  // local_53c8, local_53f8 (reused for a and b below)
    object_box(obj, bmin, bmax);
    double C[3];  // local_53e0
    C[0] = (bmin[0] + bmax[0]) * 0.5;  // DAT_14051f240
    C[1] = (bmin[1] + bmax[1]) * 0.5;
    C[2] = (bmin[2] + bmax[2]) * 0.5;
    double* pa = bmin;
    double* pb = bmax;
    v3::copy(rmin, pa);       // a = rmin
    v3::add(rmin, D, pb);     // b = rmin + D
    double Pc[3];             // local_5430
    double Pm[3];             // local_5370
    v3::project(C, pa, pb, &t, &d2, Pc);
    v3::project(rmax, pa, pb, &t, &d2, Pm);
    double B[3];  // local_53b0
    if (t < 0.0) v3::copy(Pm, B);
    else v3::copy(rmin, B);
    if (obj_no(obj) == 2 && !node_bc_code_7_or_8(ref)) {  // 140203fa0
        const double my = (rmax[1] + rmin[1]) / 2.0;     // DAT_140509fb8
        const double mz = (rmax[2] + rmin[2]) / 2.0;
        if (0.001 < std::fabs(my) || 0.001 < std::fabs(mz)) {  // DAT_14051f1e8
            gx = 0.0;
            gy = -my;
            gz = -mz;
            const char f1 = 1, f2 = 1;
            translate(ref, g_last_disp, &f1, &f2);
            crt::printf("Object %d has been centered on YZ plane.\n", obj_no(ref));
            crt::printf("Object %d has been moved by (%g, %g, %g).\n", obj_no(ref), gx, gy, gz);
        }
    }
    double tmp[3];  // local_5410
    if (sub == 0) {
        const double diag = v3::dist(rmin, Pm);  // 14023b8f0 (rmin to the projection of rmax)
        v3::scale(D, val * diag, tmp);
        v3::add(B, tmp, B);
    } else if (sub == 1) {
        unsigned char* ax = wpaxis_find_or_create(ptr(ref, 0x20a0), 1);  // 140259f90
        if (ax != nullptr) {
            if (val == 0.0) wpaxis_vector_a(ax, B);  // 140258f10
            else wpaxis_vector_b(ax, B);             // 140258f90
            v3::project(B, pa, pb, &t, &d2, B);
        }
    } else {
        v3::scale(D, val, tmp);
        v3::add(B, tmp, B);
    }
    gx = B[0] - Pc[0];
    gy = B[1] - Pc[1];
    gz = B[2] - Pc[2];
    const char f1 = 1, f2 = 1;
    translate(obj, g_last_disp, &f1, &f2);
    crt::printf("Object %d has been reference positioned w.r.t. Object %d.\n", obj_no(obj), obj_no(ref));
    crt::printf("Object %d has been moved by (%g, %g, %g).\n", obj_no(obj), gx, gy, gz);
    st.moved = true;
    return 0;
}

// part of @src 140378e00  3D mode 14 (OBJPOS.md 7.6); returns 0 or 3
std::int32_t mode14(DB_STEP_DATA& m, LLOBJ_DATA* obj, Args& a, State& st)
{
    (void)st;
    LLOBJ_DATA* ob = find_object(m, static_cast<std::int32_t>(a.v[0]));  // local_52c8, no abs
    if (const std::int32_t rc = check_ref(ob)) return rc;
    const std::int32_t sub = static_cast<std::int32_t>(a.v[1]);  // local_52c0
    double p = a.v[2];                                            // local_52a0
    double mn[3] = {}, mx[3] = {};                                // local_5298, local_5280
    object_box(obj, mn, mx);
    double ca[3];  // local_52b8
    ca[0] = (mn[0] + mx[0]) * 0.5;
    ca[1] = (mn[1] + mx[1]) * 0.5;
    ca[2] = (mn[2] + mx[2]) * 0.5;
    object_box(ob, mn, mx);
    double cb[3];  // local_52f8
    cb[0] = (mn[0] + mx[0]) * 0.5;
    cb[1] = (mn[1] + mx[1]) * 0.5;
    cb[2] = (mn[2] + mx[2]) * 0.5;
    double v[3];  // local_52e0
    v3::sub(cb, ca, v);
    LLOBJ_DATA* ref = find_object(m, std::abs(static_cast<std::int32_t>(a.v[3])));
    if (const std::int32_t rc = check_ref(ref)) return rc;
    if (sub == 0) {
#ifdef DEF_P67_IFD
        double out5cc8[3] = {};
        char tchg = 0;     // local_5598
        std::int32_t one = 1;
        fun_14019fa00(&m, obj, ref, v, &p, out5cc8, g_last_disp, 1, &a.n_coupled, a.coupled, &tchg, &one);
        v3::scale(v, -1.0, v);  // DAT_140522fc8
        one = 1;
        fun_14019fa00(&m, ob, ref, v, &p, out5cc8, g_last_disp, 1, &a.n_coupled, a.coupled, &tchg, &one);
#else
        DEF_NOT_PORTED(0x14019fa00);
#endif
    } else {
        node_bbox(ptr(ref, 0x8a0), mn, mx);
        const double h = mx[2] - mn[2];  // local_5268
        v3::normalize(v);
        v3::scale(v, -h, v);
        v3::copy(v, g_last_disp);
        const char f1 = 1, f2 = 1;
        translate(obj, g_last_disp, &f1, &f2);
        v3::scale(v, -1.0, v);
        const char f3 = 1, f4 = 1;
        translate(ob, v, &f3, &f4);
    }
    return 0;
}

} // namespace

// @src 140378e00  case 7 (OBJPOS) of the command dispatcher; the other cases are other p67 packets'
int run(DB_STEP_DATA& m, std::FILE* f)
{
    Args a;
    State st;
    std::int32_t ntok = 0;
    if (def::key::read_tokens(f, &ntok, 10) != 0) return 2;  // 140413900
    a.obj_no = def::key::token_int(0);                        // 14047b6c0
    a.mode = def::key::token_int(1);
    const bool is2d = m.dimension == 2;  // 1400015e0
    double count;
    if (!is2d) {
        for (int k = 0; k < 8; ++k) a.v[k] = def::key::token_double(k + 2);  // 14048754c
        count = a.v[7];
    } else {
        for (int k = 0; k < 5; ++k) a.v[k] = def::key::token_double(k + 2);
        count = a.v[4];
    }
    a.n_coupled = static_cast<std::int32_t>(count);
    if (0 < a.n_coupled) {
        std::int32_t m2 = 0;
        if (def::key::read_tokens(f, &m2, 100) != 0) return 2;
        a.n_coupled = m2 < a.n_coupled ? m2 : a.n_coupled;
        for (std::int32_t i = 0; i < a.n_coupled; ++i) {
            a.coupled[i] = static_cast<std::int32_t>(def::key::token_double(i));
        }
    }
    double centre[3] = {0.0, 0.0, 0.0};  // local_5678
    LLOBJ_DATA* obj = find_object(m, a.obj_no);
    if (obj == nullptr) return 3;
    if (!usable(obj)) return 3;
    const char f1 = 1, f2 = 1;
    double* v = a.v;
    if (!is2d) {
        switch (a.mode - 1) {
        case 0:
        case 10:
            translate(obj, v, &f1, &f2);
            g_last_disp[0] = v[0];
            g_last_disp[1] = v[1];
            g_last_disp[2] = v[2];
            crt::printf("Object %d has been moved by (%g, %g, %g).\n", obj_no(obj), v[0], v[1], v[2]);
            st.moved = true;
            break;
        case 1:
        case 0xb:
        case 0x65:
        case 0x6f: {
#ifdef DEF_P67_IFD
            const std::int32_t rc = interference_mode(m, *obj, a, st);
            if (rc != 0) return rc;
#else
            DEF_NOT_PORTED(0x14019fa00);
#endif
            break;
        }
        case 2:
            rotate(obj, &v[0], &v[3], v[6]);
            crt::printf("Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n",
                        obj_no(obj), v[6], v[0], v[1], v[2], v[3], v[4], v[5]);
            st.rotated = true;
            break;
        case 3:
            mirror_3d(obj, &v[0], &v[3]);
            crt::printf("Object %d has been flip w.r.t. to plane defined by point (%g, %g, %g) and normal (%g, %g, %g).\n",
                        obj_no(obj), v[0], v[1], v[2], v[3], v[4], v[5]);
            st.mirrored = true;
            break;
        case 5: {
            own_centre(obj, centre);  // 1401ff360
            const double ang = v[3];
            v[6] = v[3];   // angle := v5
            v[3] = v[0];   // direction := (v2, v3, v4)
            v[4] = v[1];
            v[5] = v[2];
            v[0] = centre[0];  // origin := C
            v[1] = centre[1];
            v[2] = centre[2];
            rotate(obj, &v[0], &v[3], ang);
            crt::printf("Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n",
                        obj_no(obj), v[6], v[0], v[1], v[2], v[3], v[4], v[5]);
            st.rotated = true;
            break;
        }
        case 6:
        case 7:
        case 8: {
#ifdef DEF_P67_DROP
            const std::int32_t rc = drop_mode(m, *obj, a, st);
            if (rc != 0) return rc;
#else
            DEF_NOT_PORTED(0x1401a7d60);
#endif
            break;
        }
        case 0xc:
            if (const std::int32_t rc = mode13(m, obj, a, st)) return rc;
            break;
        case 0xd:
            if (const std::int32_t rc = mode14(m, obj, a, st)) return rc;
            break;
        default:
            break;
        }
        // 5.1 STROKE (main object)
        bool upd = (a.mode == 0xb || a.mode == 0xc || a.mode == 0x70) && st.moved;  // local_5684
        if (at<std::int32_t>(obj, 0x21c8) == 4) upd = false;
        if (upd) {
            at<double>(obj, 0x22d0) = at<double>(obj, 0x22d0) + g_last_disp[0];
            at<double>(obj, 0x22d8) = at<double>(obj, 0x22d8) + g_last_disp[1];
            at<double>(obj, 0x22e0) = at<double>(obj, 0x22e0) + g_last_disp[2];
        }
        // 5.2 / 5.3 coupled objects
        bool retr = false;  // local_54b0
        double disp[3] = {g_last_disp[0], g_last_disp[1], g_last_disp[2]};  // local_54c8
        if (a.mode == 2 || a.mode == 0xc || a.mode == 0x66 || a.mode == 0x70) {
            const double zero[3] = {0.0, 0.0, 0.0};
            const double dir[3] = {v[1], v[2], v[3]};
            if (v3::near(dir, zero, 1e-12)) retr = true;  // DAT_14051f260
        }
        for (std::int32_t i = 0; i < a.n_coupled; ++i) {
            LLOBJ_DATA* c = find_object(m, std::abs(a.coupled[i]));
            if (c == nullptr) std::abort();  // the original crashes in 1401611f0 (access violation)
            if (!usable(c)) continue;
            if (st.moved) {
                if (retr) {
                    const double l = v3::len(v3::make(g_last_disp));  // 140239a30
                    move_along_movctl(ptr(c, 0x2160), 0.0, l, disp);
                }
                const char c1 = 1, c2 = 1;
                translate(c, disp, &c1, &c2);
                crt::printf("Object %d has been moved by (%g, %g, %g).\n", obj_no(c), disp[0], disp[1], disp[2]);
                if (upd) {
                    at<double>(c, 0x22d0) = at<double>(c, 0x22d0) + disp[0];
                    at<double>(c, 0x22d8) = at<double>(c, 0x22d8) + disp[1];
                    at<double>(c, 0x22e0) = at<double>(c, 0x22e0) + disp[2];
                }
            }
            if (st.rotated) {
                rotate(c, &v[0], &v[3], v[6]);
                crt::printf("Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n",
                            obj_no(c), v[6], v[0], v[1], v[2], v[3], v[4], v[5]);
            }
            if (st.mirrored) {
                mirror_3d(c, &v[0], &v[3]);
                crt::printf("Object %d has been flip w.r.t. to plane defined by point (%g, %g, %g) and normal (%g, %g, %g).\n",
                            obj_no(c), v[0], v[1], v[2], v[3], v[4], v[5]);
            }
            if (st.dropped) {
                transform_4x4(c, st.drop_m);  // 1401fe240
                crt::printf("Object %d has been moved by drop positioning.\n", obj_no(c));
            }
        }
    } else {
        switch (a.mode - 1) {
        case 0:
        case 10:
            translate(obj, v, &f1, &f2);
            g_last_disp[0] = v[0];
            g_last_disp[1] = v[1];
            crt::printf("Object %d has been moved by (%g, %g).\n", obj_no(obj), v[0], v[1]);
            st.moved = true;
            break;
        case 1:
        case 0xb:
        case 0x65:
        case 0x6f: {
#ifdef DEF_P67_IFD
            const std::int32_t rc = interference_mode(m, *obj, a, st);
            if (rc != 0) return rc;
#else
            DEF_NOT_PORTED(0x14019fa00);
#endif
            break;
        }
        case 2:
            rotate(obj, &v[0], nullptr, v[2]);
            crt::printf("Object %d has been rotated by %g w.r.t. (%g, %g).\n", obj_no(obj), v[2], v[0], v[1]);
            st.rotated = true;
            break;
        case 3:
            mirror_2d(obj, &v[0], &v[2]);
            crt::printf("Object %d has been flip w.r.t. to (%g, %g)-(%g, %g).\n", obj_no(obj), v[0], v[1], v[2], v[3]);
            st.mirrored = true;
            break;
        case 4:
            translate(obj, g_last_disp, &f1, &f2);
            crt::printf("Object %d has been moved by (%g, %g).\n", obj_no(obj), g_last_disp[0], g_last_disp[1]);
            st.moved = true;
            break;
        case 5: {
            own_centre(obj, centre);
            const double ang = v[0];
            v[2] = v[0];  // angle := v2
            v[0] = centre[0];
            v[1] = centre[1];
            rotate(obj, &v[0], nullptr, ang);
            crt::printf("Object %d has been rotated by %g w.r.t. (%g, %g).\n", obj_no(obj), v[2], v[0], v[1]);
            st.rotated = true;
            break;
        }
        default:
            break;
        }
        bool upd = a.mode == 0xb || a.mode == 0xc || a.mode == 0x70;  // 2D: moved not tested
        if (at<std::int32_t>(obj, 0x21c8) == 4) upd = false;
        if (upd) {
            at<double>(obj, 0x22d0) = at<double>(obj, 0x22d0) + g_last_disp[0];
            at<double>(obj, 0x22d8) = at<double>(obj, 0x22d8) + g_last_disp[1];
        }
        bool retr = false;  // local_5528
        double disp[3] = {g_last_disp[0], g_last_disp[1], 0.0};  // local_5540
        if (a.mode == 2 || a.mode == 0xc || a.mode == 0x66 || a.mode == 0x70) {
            const double zero[2] = {0.0, 0.0};
            const double dir[2] = {v[2], v[3]};
            if (v3::near2(dir, zero, 1e-12)) retr = true;
        }
        for (std::int32_t i = 0; i < a.n_coupled; ++i) {
            LLOBJ_DATA* c = find_object(m, std::abs(a.coupled[i]));
            if (c == nullptr) std::abort();  // the original crashes in 1401611f0
            if (!usable(c)) continue;
            if (st.moved) {
                if (retr) {
                    const double l = v3::len(v3::make2(g_last_disp));  // 1403087e0
                    move_along_movctl(ptr(c, 0x2160), 0.0, l, disp);
                }
                const char c1 = 1, c2 = 1;
                translate(c, disp, &c1, &c2);
                crt::printf("Object %d(coupled) has been moved by (%g, %g).\n", obj_no(c), disp[0], disp[1]);
                if (upd) {
                    at<double>(c, 0x22d0) = at<double>(c, 0x22d0) + disp[0];
                    at<double>(c, 0x22d8) = at<double>(c, 0x22d8) + disp[1];
                }
            }
            if (st.rotated) {
                rotate(c, &v[0], nullptr, v[2]);
                crt::printf("Object %d has been rotated by %g w.r.t. (%g, %g).\n", obj_no(c), v[2], v[0], v[1]);
            }
            if (st.mirrored) {
                mirror_2d(c, &v[0], &v[2]);
                crt::printf("Object %d has been flip w.r.t. to (%g, %g)-(%g, %g).\n", obj_no(c), v[0], v[1], v[2], v[3]);
            }
        }
    }
    return 0;
}

// 1401952e0: ported in defdbio/objcheck/objcheck.hpp (press_stroke_vector)
void move_along_movctl(const void* mv, double p2, double p3, double* out)
{
    dbio::objcheck::press_stroke_vector(const_cast<void*>(mv), p2, p3, out);
}

} // namespace def::cmd::objpos

namespace def::cmd {

// part of @src 140378e00  dispatcher entry for case 7 (defcmd/cmd_objpos.hpp)
int cmd_objpos(model::DB_STEP_DATA* m, std::FILE* f) { return objpos::run(*m, f); }

} // namespace def::cmd
