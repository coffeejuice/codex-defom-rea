// kw-g2obj: the group 2 KEY parser FUN_14038b8a0 (prologue, dispatch, result) and the case
// bodies of this packet's keywords. Case map: docs/re/keywords/group2-parser-map.md; every case
// below cites its p_main.c lines (deform-rea-work/out/g2/p_main.c, the decompile of 14038b8a0).
#include <cmath>
#include <cstdlib>

#include <defkey/crt_scan.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defrt/rdata.hpp>

#include "dispatch.hpp"
#include "internal.hpp"

namespace def::key::g2obj {

namespace {

// rdata 140522fd0  pi/180
constexpr double kDegToRad = def::rt::rdata::d_140522fd0;
// rdata 14051f260  snap threshold for the 2D direction components
constexpr double kTiny = def::rt::rdata::d_14051f260;
// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;

// FUN_140484ab8: abs(int), INT_MIN stays INT_MIN (two's complement negation).
std::int32_t iabs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

// (int)d as the binary's CVTTSD2SI does (truncation; out-of-range gives INT_MIN).
std::int32_t trunc_i(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) return INT32_MIN;
    return static_cast<std::int32_t>(d);
}

// Case 7 sub-mode for the types 0, 7 and 8 (tables T@21d0, T@2678, T@26f0): the 3D branch at
// p_main.c 350-382 / 590-622 / 623-655, the 2D branch at 750-779 / 1004-1036 / 1037-1068.
// Returns the table-read status (0 when no table is read).
int sub_mode_078(std::FILE* f, LLOBJ_DATA& obj, std::size_t toff, std::int32_t sub, double v)
{
    int st = 0;
    DYNARY* t = table_at(obj, toff);
    model::dynary_reinit_keep(t, 0);
    if (sub == 0) {
        t->ndim = 0;
        t->value = v;
    } else if (sub == 1) {
        t->ndim = 1;
        t->var[0] = 2;
        t->n[0] = trunc_i(v);
        st = io::table_read(f, t);
    } else if (sub == 2) {
        t->ndim = 1;
        t->var[0] = 1;
        t->n[0] = trunc_i(v);
        st = io::table_read(f, t);
    }
    if (sub < 0) {
        at<std::int32_t>(obj, 0x2780) = 1;
        at<std::int32_t>(obj, 0x2784) = iabs(sub);
    } else {
        at<std::int32_t>(obj, 0x2780) = 0;
        at<std::int32_t>(obj, 0x2784) = 0;
    }
    return st;
}

// Case 7 type 1 (speed, T@2248): p_main.c 384-428 (3D) / 781-826 (2D).
int sub_mode_speed(std::FILE* f, LLOBJ_DATA& obj, std::int32_t sub, double v)
{
    int st = 0;
    DYNARY* t = table_at(obj, 0x2248);
    model::dynary_reinit_keep(t, 0);
    at<std::int32_t>(obj, 0x2780) = 0;
    at<std::int32_t>(obj, 0x2784) = 0;
    at<std::int32_t>(obj, 0x22c0) = 0;
    at<double>(obj, 0x22c8) = 0.0;
    if (sub == 0) {
        t->ndim = 0;
        t->value = v;
    } else if (sub == 1) {
        t->ndim = 1;
        t->var[0] = 2;
        t->n[0] = trunc_i(v);
        st = io::table_read(f, t);
    } else if (sub == 2) {
        t->ndim = 1;
        t->var[0] = 1;
        t->n[0] = trunc_i(v);
        st = io::table_read(f, t);
    } else if (sub == 4) {
        t->ndim = 1;
        t->var[0] = 5;
        t->n[0] = trunc_i(v);
        st = io::table_read(f, t);
    } else if (sub < 0x3e9 || 1999 < sub) {
        if (sub < 0) {
            at<std::int32_t>(obj, 0x2780) = 1;
            at<std::int32_t>(obj, 0x2784) = iabs(sub);
        }
    } else {
        at<std::int32_t>(obj, 0x22c0) = sub - 1000;
        at<double>(obj, 0x22c8) = v;
    }
    return st;
}

// Case 7 type 5 (spring, T@2510): the token line, p_main.c 536-570 (3D) / 942-976 (2D).
// Returns -1 on tokenizer EOF (the case then returns 2), else the table-read status.
int spring(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t cnt = 0;
    if (io::tokenize_line(f, &cnt, 7) != 0) return -1;
    std::int32_t flag = io::token_int(0);
    at<double>(obj, 0x2588) = io::token_double(2);
    at<double>(obj, 0x2590) = io::token_double(3);
    at<double>(obj, 0x2598) = io::token_double(4);
    at<std::int32_t>(obj, 0x25a0) = io::token_int(5);
    at<std::int32_t>(obj, 0x25a4) = io::token_int(6);
    DYNARY* t = table_at(obj, 0x2510);
    model::dynary_reinit_keep(t, 0);
    if (flag == 0) {
        t->ndim = 0;
        t->value = io::token_double(1);
        return 0;
    }
    t->ndim = 1;
    t->var[0] = 0x13;
    t->n[0] = io::token_int(1);
    return io::table_read(f, t);
}

// Case 7 type 6 (T@25a8) after the template choice: p_main.c 579-588 (3D) / 985-1002 (2D).
// Sets +0x2624, the count and the sub flags; returns the sub value's side effects only.
void type6_head(LLOBJ_DATA& obj, std::int32_t sub, double i, double n)
{
    at<std::int32_t>(obj, 0x2624) = trunc_i(i);
    at<std::int32_t>(obj, 0x25c4) = trunc_i(n);
    at<std::int32_t>(obj, 0x2670) = 0;
    if (sub == 1) {
        at<std::int32_t>(obj, 0x25b8) = 1;
    } else if (sub == 2) {
        at<std::int32_t>(obj, 0x25b8) = 0x1a;
    } else if (sub < 0) {
        at<std::int32_t>(obj, 0x2670) = 1;
        at<std::int32_t>(obj, 0x2674) = iabs(sub);
    } else {
        at<std::int32_t>(obj, 0x25b8) = 1;
    }
}

void set_dir3(LLOBJ_DATA& obj, double x, double y, double z)
{
    at<double>(obj, 0x21b0) = x;
    at<double>(obj, 0x21b8) = y;
    at<double>(obj, 0x21c0) = z;
}

// 2D MOVCTL direction, e.g. 14038c699: movsd xmm0,[a]; mulsd xmm0,[140522fd0]; call 140bb3490
// (MSVC CRT cos, AMD LibM); then the same product into 140bb3000 (sin). The CRT routines differ
// from <cmath> in the last bit (cos(-90.000003 deg) is ...CECA, not the correctly rounded ...CECB).
void set_dir2(LLOBJ_DATA& obj, double a)
{
    at<double>(obj, 0x21b0) = def::rt::libm::crt_cos(a * kDegToRad);  // 140bb3490
    at<double>(obj, 0x21b8) = def::rt::libm::crt_sin(a * kDegToRad);  // 140bb3000
}

// part of @src 14038b8a0  case 7 (MOVCTL), 3D branch: p_main.c 336-728
int read_movctl_3d(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t type = 0;
    double h[7];
    int st = io::read_id7(f, &type, &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6]);
    if (st != 0) return 2;
    int status = st;  // iVar2 = local_108c before the switch
    switch (type) {
    case 0:
        at<std::int32_t>(obj, 0x21c8) = 0;
        set_dir3(obj, h[1], h[2], h[3]);
        status = sub_mode_078(f, obj, 0x21d0, trunc_i(h[0]), h[4]);
        break;
    case 1:
        at<std::int32_t>(obj, 0x21c8) = 1;
        set_dir3(obj, h[1], h[2], h[3]);
        status = sub_mode_speed(f, obj, trunc_i(h[0]), h[4]);
        break;
    case 2: {
        at<std::int32_t>(obj, 0x21c8) = 2;
        set_dir3(obj, h[0], h[1], h[2]);
        double e[6] = {0, 0, 0, 0, 0, 0};
        int l2 = io::read_d6(f, &e[0], &e[1], &e[2], &e[3], &e[4], &e[5]);
        std::int32_t ia = trunc_i(e[4]);
        std::int32_t ib = trunc_i(e[5]);
        DYNARY* t = table_at(obj, 0x2368);
        if (ia == 0 && ib == 0) {
            at<double>(obj, 0x2350) = e[0];
            at<double>(obj, 0x2358) = e[1];
            at<double>(obj, 0x2348) = e[2];
            at<std::int32_t>(obj, 0x2360) = trunc_i(e[3]);
            model::dynary_reinit_keep(t, 0);
            t->ndim = 0;
            status = l2;
        } else if (ia == 0 || ib != 0) {
            status = l2;
            if (ib != 0) {
                at<double>(obj, 0x2350) = e[0];
                at<double>(obj, 0x2358) = e[1];
                at<double>(obj, 0x2348) = e[2];
                at<std::int32_t>(obj, 0x2360) = trunc_i(e[3]);
                model::dynary_reinit_keep(t, 0);
                t->ndim = 1;
                if (trunc_i(e[4]) == 4) t->var[0] = 5;
                t->n[0] = trunc_i(e[5]);
                status = io::table_read(f, t);
                if (status != 0) return 2;
            }
        } else {
            // legacy form: B is the point count, +0x2358 is not stored, the L2 status is lost
            at<double>(obj, 0x2350) = e[0];
            at<double>(obj, 0x2348) = e[2];
            at<std::int32_t>(obj, 0x2360) = trunc_i(e[3]);
            model::dynary_reinit_keep(t, 0);
            t->ndim = 1;
            if (trunc_i(e[4]) == 4) t->var[0] = 5;
            t->n[0] = trunc_i(e[1]);
            status = io::table_read(f, t);
            if (status != 0) return 2;
            status = 0;
        }
        break;
    }
    case 3: {
        at<std::int32_t>(obj, 0x21c8) = 3;
        set_dir3(obj, h[0], h[1], h[2]);
        DYNARY* t = table_at(obj, 0x2400);
        model::dynary_reinit_keep(t, 0);
        double e[6] = {0, 0, 0, 0, 0, 0};
        int l2 = io::read_d6(f, &e[0], &e[1], &e[2], &e[3], &e[4], &e[5]);
        at<double>(obj, 0x23e0) = e[0];
        at<double>(obj, 0x23e8) = e[1];
        at<double>(obj, 0x23f0) = e[2];
        at<double>(obj, 0x23f8) = e[3];
        std::int32_t sub = trunc_i(e[4]);
        status = l2;
        if (sub == 0) {
            t->ndim = 0;
        } else if (sub == 4) {
            t->ndim = 1;
            t->var[0] = 5;
            t->n[0] = trunc_i(e[5]);
            status = io::table_read(f, t);
            if (status != 0) return 2;
        }
        break;
    }
    case 4: {
        at<std::int32_t>(obj, 0x21c8) = 4;
        set_dir3(obj, h[0], h[1], h[2]);
        double n = 0;
        int l2 = io::read_d4(f, &at<double>(obj, 0x2478), &at<double>(obj, 0x2480),
                             &at<double>(obj, 0x2488), &n);
        DYNARY* t = table_at(obj, 0x2498);
        model::dynary_reinit_keep(t, 0);
        if (trunc_i(n) < 1) {
            at<std::uint8_t>(obj, 0x2490) = 0;
            t->ndim = 0;
            t->value = 0.0;
            status = l2;
        } else {
            at<std::uint8_t>(obj, 0x2490) = 1;
            t->ndim = 1;
            t->var[0] = 10;
            t->n[0] = trunc_i(n);
            status = io::table_read(f, t);
        }
        break;
    }
    case 5: {
        at<std::int32_t>(obj, 0x21c8) = 5;
        set_dir3(obj, h[0], h[1], h[2]);
        int r = spring(f, obj);
        if (r < 0) return 2;
        status = r;
        break;
    }
    case 6: {
        at<std::int32_t>(obj, 0x21c8) = 6;
        at<std::int32_t>(obj, 0x2620) = trunc_i(h[0]);
        DYNARY* t = table_at(obj, 0x25a8);
        model::dynary_init(t, at<std::int32_t>(obj, 0x2620) == 1 ? 0x7a : 0x7b);
        type6_head(obj, trunc_i(h[1]), h[2], h[3]);
        if (at<std::int32_t>(obj, 0x2670) == 0) {
            if (at<std::int32_t>(obj, 0x2620) == 2) {
                // both statuses are overwritten by the table read
                io::read_d3(f, &at<double>(obj, 0x2628), &at<double>(obj, 0x2630),
                            &at<double>(obj, 0x2638));
                io::read_d6(f, &at<double>(obj, 0x2640), &at<double>(obj, 0x2648),
                            &at<double>(obj, 0x2650), &at<double>(obj, 0x2658),
                            &at<double>(obj, 0x2660), &at<double>(obj, 0x2668));
            }
            status = io::table_read(f, t);
        }
        break;
    }
    case 7:
        at<std::int32_t>(obj, 0x21c8) = 7;
        set_dir3(obj, h[1], h[2], h[3]);
        status = sub_mode_078(f, obj, 0x2678, trunc_i(h[0]), h[4]);
        at<double>(obj, 0x2768) = h[5];
        at<double>(obj, 0x2770) = h[6];
        break;
    case 8:
        at<std::int32_t>(obj, 0x21c8) = 8;
        set_dir3(obj, h[1], h[2], h[3]);
        status = sub_mode_078(f, obj, 0x26f0, trunc_i(h[0]), h[4]);
        at<double>(obj, 0x2768) = h[5];
        break;
    case 9:
        at<std::int32_t>(obj, 0x21c8) = 9;
        set_dir3(obj, h[0], h[1], h[2]);
        at<double>(obj, 0x2768) = h[3];
        break;
    case 10:
        at<std::int32_t>(obj, 0x21c8) = 10;
        set_dir3(obj, h[0], h[1], h[2]);
        at<double>(obj, 0x2768) = h[3];
        at<double>(obj, 0x2778) = h[4];
        break;
    case 0xb:
        at<std::int32_t>(obj, 0x21c8) = 0xb;
        at<double>(obj, 0x22e8) = h[0];
        at<double>(obj, 0x22f0) = h[1];
        at<double>(obj, 0x22f8) = h[2];
        break;
    default:
        break;
    }
    return status;
}

// part of @src 14038b8a0  case 7 (MOVCTL), 2D branch: p_main.c 729-1100
int read_movctl_2d(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t type = 0;
    double h[7] = {0, 0, 0, 0, 0, 0, 0};
    int st = io::read_id7(f, &type, &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6]);
    if (st != 0) return 2;
    int status = st;  // local_108c; iVar2 takes it after the snap below
    switch (type) {
    case 0:
        at<std::int32_t>(obj, 0x21c8) = 0;
        set_dir2(obj, h[1]);
        status = sub_mode_078(f, obj, 0x21d0, trunc_i(h[0]), h[2]);
        break;
    case 1:
        at<std::int32_t>(obj, 0x21c8) = 1;
        set_dir2(obj, h[1]);
        status = sub_mode_speed(f, obj, trunc_i(h[0]), h[2]);
        break;
    case 2: {
        at<std::int32_t>(obj, 0x21c8) = 2;
        set_dir2(obj, h[0]);
        std::int32_t ia = trunc_i(h[5]);
        std::int32_t ib = trunc_i(h[6]);
        DYNARY* t = table_at(obj, 0x2368);
        if (ia == 0 && ib == 0) {
            at<double>(obj, 0x2350) = h[1];
            at<double>(obj, 0x2358) = h[2];
            at<double>(obj, 0x2348) = h[3];
            at<std::int32_t>(obj, 0x2360) = trunc_i(h[4]);
            model::dynary_reinit_keep(t, 0);
            t->ndim = 0;
        } else if (ia == 0 || ib != 0) {
            if (ib != 0) {
                at<double>(obj, 0x2350) = h[1];
                at<double>(obj, 0x2358) = h[2];
                at<double>(obj, 0x2348) = h[3];
                at<std::int32_t>(obj, 0x2360) = trunc_i(h[4]);
                model::dynary_reinit_keep(t, 0);
                t->ndim = 1;
                if (trunc_i(h[5]) == 4) t->var[0] = 5;
                t->n[0] = trunc_i(h[6]);
                status = io::table_read(f, t);
                if (status != 0) return 2;
            }
        } else {
            at<double>(obj, 0x2350) = h[1];
            at<double>(obj, 0x2348) = h[3];
            at<std::int32_t>(obj, 0x2360) = trunc_i(h[4]);
            model::dynary_reinit_keep(t, 0);
            t->ndim = 1;
            if (trunc_i(h[5]) == 4) t->var[0] = 5;
            t->n[0] = trunc_i(h[2]);
            if (io::table_read(f, t) != 0) return 2;
            status = 0;
        }
        break;
    }
    case 3: {
        at<std::int32_t>(obj, 0x21c8) = 3;
        set_dir2(obj, h[0]);
        at<double>(obj, 0x23e0) = h[1];
        at<double>(obj, 0x23e8) = h[2];
        at<double>(obj, 0x23f0) = h[3];
        at<double>(obj, 0x23f8) = h[4];
        DYNARY* t = table_at(obj, 0x2400);
        model::dynary_reinit_keep(t, 0);
        std::int32_t sub = trunc_i(h[5]);
        if (sub == 0) {
            t->ndim = 0;
        } else if (sub == 4) {
            t->ndim = 1;
            t->var[0] = 5;
            t->n[0] = trunc_i(h[6]);
            status = io::table_read(f, t);
            if (status != 0) return 2;
        }
        break;
    }
    case 4: {
        at<std::int32_t>(obj, 0x21c8) = 4;
        set_dir2(obj, h[0]);
        at<double>(obj, 0x2478) = h[1];
        at<double>(obj, 0x2480) = h[2];
        at<double>(obj, 0x2488) = h[3];
        DYNARY* t = table_at(obj, 0x2498);
        model::dynary_reinit_keep(t, 0);
        if (trunc_i(h[4]) < 1) {
            at<std::uint8_t>(obj, 0x2490) = 0;
            t->ndim = 0;
            t->value = 0.0;
        } else {
            at<std::uint8_t>(obj, 0x2490) = 1;
            t->ndim = 1;
            t->var[0] = 10;
            t->n[0] = trunc_i(h[4]);
            status = io::table_read(f, t);
        }
        break;
    }
    case 5: {
        at<std::int32_t>(obj, 0x21c8) = 5;
        set_dir2(obj, h[0]);
        int r = spring(f, obj);
        if (r < 0) return 2;
        status = r;
        break;
    }
    case 6: {
        at<std::int32_t>(obj, 0x21c8) = 6;
        DYNARY* t = table_at(obj, 0x25a8);
        model::dynary_init(t, 0x79);
        type6_head(obj, trunc_i(h[0]), h[1], h[2]);
        if (at<std::int32_t>(obj, 0x2670) == 0) status = io::table_read(f, t);
        break;
    }
    case 7:
        at<std::int32_t>(obj, 0x21c8) = 7;
        set_dir2(obj, h[1]);
        status = sub_mode_078(f, obj, 0x2678, trunc_i(h[0]), h[2]);
        at<double>(obj, 0x2768) = h[3];
        at<double>(obj, 0x2770) = h[4];
        break;
    case 8:
        at<std::int32_t>(obj, 0x21c8) = 8;
        set_dir2(obj, h[1]);
        status = sub_mode_078(f, obj, 0x26f0, trunc_i(h[0]), h[2]);
        at<double>(obj, 0x2768) = h[3];
        break;
    case 9:
        at<std::int32_t>(obj, 0x21c8) = 9;
        set_dir2(obj, h[0]);
        at<double>(obj, 0x2768) = h[1];
        break;
    case 10:
        at<std::int32_t>(obj, 0x21c8) = 10;
        set_dir2(obj, h[0]);
        at<double>(obj, 0x2768) = h[1];
        at<double>(obj, 0x2778) = h[2];
        break;
    case 0xb:
        at<std::int32_t>(obj, 0x21c8) = 0xb;
        at<double>(obj, 0x22e8) = h[0];
        at<double>(obj, 0x22f0) = h[1];
        break;
    default:
        break;
    }
    // every 2D MOVCTL, whatever the type (FUN_140484558 = fabs)
    if (std::fabs(at<double>(obj, 0x21b0)) <= kTiny) at<double>(obj, 0x21b0) = 0.0;
    if (std::fabs(at<double>(obj, 0x21b8)) <= kTiny) at<double>(obj, 0x21b8) = 0.0;
    return status;
}

// Angular movement (ANGMOV T@27c0/T@2838 with method +0x2788; ANGMO2 T@28f0/T@2968 with method
// +0x28b8): p_main.c 1138-1187 and 3266-3319. `moff` is the method field, `t0`/`t1` the two
// tables.
int read_angular(std::FILE* f, LLOBJ_DATA& obj, std::size_t moff, std::size_t t0, std::size_t t1)
{
    std::int32_t method = 0, kind = 0;
    double v = 0;
    if (io::read_iid(f, &method, &kind, &v) != 0) return -1;
    if (method != 0 && method != 1) return 0;
    at<std::int32_t>(obj, moff) = method;
    DYNARY* t = table_at(obj, method == 0 ? t0 : t1);
    model::dynary_reinit_keep(t, 0);
    if (kind == 0) {
        t->ndim = 0;
        t->value = v;
    } else if (kind == 1) {
        t->ndim = 1;
        t->var[0] = 1;
        t->n[0] = trunc_i(v);
    } else if (kind == 2) {
        t->ndim = 1;
        t->var[0] = 10;
        t->n[0] = trunc_i(v);
    }
    return io::table_read(f, t);
}

} // namespace

bool handles(int id)
{
    switch (id - 0x90) {
    case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
    case 0x08: case 0x09: case 0x0b: case 0x0c: case 0x0d:
    case 0x24: case 0x25: case 0x26: case 0x27:
    case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x44: case 0x47:
    case 0x5a: case 0x5b: case 0x5c: case 0x60: case 0x6d:
    case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
    case 0x7b: case 0x7c: case 0x9f:
        return true;
    default:
        return false;
    }
}

// part of @src 14038b8a0  this packet's case bodies; returns the parser's code (0, 2, 3)
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    int st = 0;  // iVar2
    std::int32_t i1 = 0, i2 = 0;
    double d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    switch (id - 0x90) {
    case 0x00:  // OBJNAM, 270-272
        st = io::read_text_line2(f, &at<char>(obj, 0x828), 0x51);
        break;
    case 0x01:  // OBJTYP, 273-307
        if (io::read_ii(f, &i1, &i2) != 0) return 2;
        if (i1 == 1) {
            at<std::int32_t>(obj, 0xc) = 0;
        } else if (i1 == 2) {
            at<std::int32_t>(obj, 0xc) = 1;
        } else if (i1 == 3) {
            at<std::int32_t>(obj, 0xc) = 2;
        } else if (i1 == 4) {
            at<std::int32_t>(obj, 0xc) = 3;
        } else if (i1 == 5) {
            at<std::int32_t>(obj, 0xc) = 4;
        } else if (i1 != 6) {  // 14038bb3a: code 6 assigns nothing
            at<std::int32_t>(obj, 0xc) = i1 == 7 ? 6 : 7;
        }
        at<std::int32_t>(obj, 0x2154) = i2;
        if (at<std::int32_t>(obj, 0xc) == 7) model::llobj_ctor::make_empty(&obj);  // FUN_140161260
        st = 0;
        break;
    case 0x02:  // AVGSTR
        st = io::read_d(f, &at<double>(obj, 0x1ea8));
        break;
    case 0x03:  // LMTSTR
        st = io::read_d(f, &at<double>(obj, 0x1ea0));
        break;
    case 0x04:  // ZSTR, 2D only
        if (is_2d(m)) st = io::read_id(f, &at<std::int32_t>(obj, 0x1f30), &at<double>(obj, 0x1f28));
        break;
    case 0x05:  // PENVOL, 321-330
        if (!is_2d(m)) {
            st = io::read_d(f, &at<double>(obj, 0x1eb0));
        } else {
            st = io::read_dd(f, &at<double>(obj, 0x1eb0), &d1);
            at<std::int32_t>(obj, 0x1f20) = trunc_i(d1);
        }
        break;
    case 0x06:  // REFTMP
        st = io::read_d(f, &at<double>(obj, 0x1eb8));
        break;
    case 0x07:  // MOVCTL, 334-1101
        return (is_2d(m) ? read_movctl_2d(f, obj) : read_movctl_3d(f, obj)) == 0 ? 0 : 2;
    case 0x08:  // MOVANG, 1102-1108: 2D reads one value and drops it
        if (is_2d(m)) st = io::read_d(f, &d1);
        break;
    case 0x09:  // STROKE, 1109-1117
        if (!is_2d(m)) {
            st = io::read_d3(f, &at<double>(obj, 0x22d0), &at<double>(obj, 0x22d8),
                             &at<double>(obj, 0x22e0));
        } else {
            st = io::read_dd(f, &at<double>(obj, 0x22d0), &at<double>(obj, 0x22d8));
        }
        break;
    case 0x0b: {  // SPDLMT, 1120-1137
        if (io::read_idd(f, &i1, &at<double>(obj, 0x2a98), &at<double>(obj, 0x2aa0)) != 0) return 2;
        at<std::uint8_t>(obj, 0x2a18) = i1 < 1 ? 0 : 1;
        DYNARY* t = table_at(obj, 0x2a20);
        model::dynary_reinit_keep(t, 10);
        t->n[0] = i1;
        st = io::table_read(f, t);
        if (at<std::uint8_t>(obj, 0x2a18) != 0 && at<std::int32_t>(obj, 0x21c8) == 1) {
            at<std::int32_t>(obj, 0x21c8) = 8;
        }
        break;
    }
    case 0x0c: {  // ANGMOV, 1138-1187
        int r = read_angular(f, obj, 0x2788, 0x27c0, 0x2838);
        if (r < 0) return 2;
        st = r;
        break;
    }
    case 0x0d:  // CNTRAX, 1188-1200
        if (!is_2d(m)) {
            st = io::read_d7(f, &at<double>(obj, 0x2790), &at<double>(obj, 0x2798),
                             &at<double>(obj, 0x27a0), &at<double>(obj, 0x27a8),
                             &at<double>(obj, 0x27b0), &at<double>(obj, 0x27b8),
                             &at<double>(obj, 0x28b0));
        } else {
            st = io::read_d3(f, &at<double>(obj, 0x2790), &at<double>(obj, 0x2798),
                             &at<double>(obj, 0x28b0));
        }
        break;
    case 0x24:  // RMDPTH
        st = io::read_d(f, &at<double>(obj, 0x2dc8));
        break;
    case 0x25:  // RMSTRK
        st = io::read_d(f, &at<double>(obj, 0x2dd0));
        break;
    case 0x26:  // RMTIME
        st = io::read_d(f, &at<double>(obj, 0x2de0));
        break;
    case 0x27:  // RMSTEP
        st = io::read_d(f, &at<double>(obj, 0x2dd8));
        break;
    case 0x37:  // FRCSTP
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f34), false);
        break;
    case 0x38:  // FRCNEL
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f38), false);
        break;
    case 0x39:  // FRCMTH
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f3c), false);
        break;
    case 0x3a:  // TMPLMT
        st = io::read_d(f, &at<double>(obj, 0x1f10));
        break;
    case 0x3b: {  // ANGMVY, 2D only, 2143-2208
        if (!is_2d(m)) break;
        if (io::read_iid4(f, &i1, &i2, &d1, &d2, &d3, &d4) != 0) return 2;
        if (i1 == 0 || i1 == 1) {
            at<std::int32_t>(obj, 0x2ac0) = i1;
            at<double>(obj, 0x2bd0) = d2;
            DYNARY* t = table_at(obj, i1 == 0 ? 0x2ac8 : 0x2b40);
            model::dynary_reinit_keep(t, 0);
            if (i2 == 0) {
                t->ndim = 0;
                t->value = d1;
            } else if (i2 == 1) {
                t->ndim = 1;
                t->var[0] = 1;
                t->n[0] = trunc_i(d1);
            } else if (i2 == 2) {
                t->ndim = 1;
                t->var[0] = 10;
                t->n[0] = trunc_i(d1);
            }
            st = io::table_read(f, t);
        } else {
            st = 0;
            if (i1 == 2) {
                at<std::int32_t>(obj, 0x2ac0) = 2;
                at<double>(obj, 0x2bb8) = d1;
                at<double>(obj, 0x2bc0) = d2;
                at<double>(obj, 0x2bc8) = d3;
                at<double>(obj, 0x2bd0) = d4;
            }
        }
        break;
    }
    case 0x44:  // CRPTIM
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f40), false);
        break;
    case 0x47:  // REFPOS, 2266-2280
        st = io::read_id3(f, &at<std::int32_t>(obj, 0x1ec8), &d1, &d2, &d3);
        if (at<std::int32_t>(obj, 0x1ec8) == 1) {
            at<double>(obj, 0x1ed0) = d1;
            at<double>(obj, 0x1ed8) = d2;
            at<double>(obj, 0x1ee0) = d3;
        } else if (at<std::int32_t>(obj, 0x1ec8) == 2) {
            at<std::int32_t>(obj, 0x1ee8) = trunc_i(d1);
        } else {
            at<std::int32_t>(obj, 0x1ee8) = 1;
        }
        break;
    case 0x5a:  // ELPSOL
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f18), false);
        break;
    case 0x5b: {  // CSFREQ, 3058-3086 (table T@1f60)
        if (io::read_id(f, &i1, &d1) != 0) return 2;
        DYNARY* t = table_at(obj, 0x1f60);
        st = 0;
        if (i1 == 0) {
            t->ndim = 0;
            t->value = d1;
        } else if (i1 == 1 || i1 == 2 || i1 == 3) {
            t->ndim = 1;
            t->var[0] = i1 == 1 ? 1 : (i1 == 2 ? 0x1c : 0x1d);
            t->n[0] = trunc_i(d1);
            st = io::table_read(f, t);
        }
        break;
    }
    case 0x5c: {  // VOLCRG, 3087-3115 (class +0x1fd8, table T@1fe0)
        if (io::read_id(f, &i1, &d1) != 0) return 2;
        if (i1 < 2) {
            at<std::int32_t>(obj, 0x1fd8) = 0;
        } else if (i1 < 0xc) {
            at<std::int32_t>(obj, 0x1fd8) = 1;
        } else {
            if (0x15 < i1) return 3;
            at<std::int32_t>(obj, 0x1fd8) = 2;
        }
        DYNARY* t = table_at(obj, 0x1fe0);
        st = 0;
        if (i1 == 0 || i1 == 10 || i1 == 0x14) {
            t->ndim = 0;
            t->value = d1;
        } else if (i1 == 1 || i1 == 0xb || i1 == 0x15) {
            t->ndim = 1;
            t->var[0] = 1;
            t->n[0] = trunc_i(d1);
            st = io::table_read(f, t);
        }
        break;
    }
    case 0x60: {  // OTPRNG, 3131-3162
        std::int32_t cnt = 0;
        if (io::tokenize_line(f, &cnt, 6) != 0) return 2;
        i1 = io::token_int(0);
        at<double>(obj, 0x1ef8) = io::token_double(1);
        at<double>(obj, 0x1f00) = io::token_double(2);
        if (cnt < 4) {
            at<std::int32_t>(obj, 0x1f08) = 0;
        } else {
            at<std::int32_t>(obj, 0x1f08) = io::token_int(3);
        }
        if (i1 < 0) {
            at<std::int32_t>(obj, 0x1eec) = 3;
            at<std::int32_t>(obj, 0x1ef0) = iabs(i1);
        } else {
            at<std::int32_t>(obj, 0x1eec) = i1;
        }
        st = 0;
        break;
    }
    case 0x6d:  // USRSUB
        st = io::read_i(f, &at<std::int32_t>(obj, 0x2150), false);
        break;
    case 0x70:  // PRSTRE
        st = io::read_d3(f, &at<double>(obj, 0x2aa8), &at<double>(obj, 0x2ab0),
                         &at<double>(obj, 0x2ab8));
        break;
    case 0x71:  // PRSNAM
        st = io::read_text_line2(f, &at<char>(obj, 0x2160), 0x50);
        break;
    case 0x72:  // ROTSYM, 3D only, 3244-3259
        if (!is_3d(m)) break;
        if (io::read_d(f, &at<double>(obj, 0x2058)) != 0) return 2;
        if (io::read_d3(f, &at<double>(obj, 0x2078), &at<double>(obj, 0x2080),
                        &at<double>(obj, 0x2088)) != 0) {
            return 2;
        }
        st = io::read_d3(f, &at<double>(obj, 0x2060), &at<double>(obj, 0x2068),
                         &at<double>(obj, 0x2070));
        break;
    case 0x73:  // TRGVOL, 3260-3265
        st = io::read_id(f, &at<std::int32_t>(obj, 0x2098), &at<double>(obj, 0x2090));
        if (at<double>(obj, 0x2090) == kZero) at<std::int32_t>(obj, 0x2098) = -1;
        break;
    case 0x74: {  // ANGMO2, 3D only, 3266-3319
        if (!is_3d(m)) break;
        int r = read_angular(f, obj, 0x28b8, 0x28f0, 0x2968);
        if (r < 0) return 2;
        st = r;
        break;
    }
    case 0x75:  // CNTRA2, 3D only, 3320-3330
        if (!is_3d(m)) break;
        st = io::read_d7(f, &at<double>(obj, 0x28c0), &at<double>(obj, 0x28c8),
                         &at<double>(obj, 0x28d0), &at<double>(obj, 0x28d8),
                         &at<double>(obj, 0x28e0), &at<double>(obj, 0x28e8),
                         &at<double>(obj, 0x29e0));
        break;
    case 0x7b:  // OBJUPD
        st = io::read_i(f, &at<std::int32_t>(obj, 0x1f1c), false);
        break;
    case 0x7c:  // OBJSTP, 3574-3586
        if (!is_2d(m)) {
            st = io::read_d6(f, &at<double>(obj, 0x29e8), &at<double>(obj, 0x29f0),
                             &at<double>(obj, 0x29f8), &at<double>(obj, 0x2a00),
                             &at<double>(obj, 0x2a08), &at<double>(obj, 0x2a10));
        } else {
            st = io::read_d4(f, &at<double>(obj, 0x29e8), &at<double>(obj, 0x29f0),
                             &at<double>(obj, 0x2a00), &at<double>(obj, 0x2a08));
        }
        break;
    case 0x9f:  // FORCE, 3861-3869
        if (!is_2d(m)) {
            st = io::read_d3(f, &at<double>(obj, 0x2300), &at<double>(obj, 0x2308),
                             &at<double>(obj, 0x2310));
        } else {
            st = io::read_dd(f, &at<double>(obj, 0x2300), &at<double>(obj, 0x2308));
        }
        break;
    default:
        return 2;  // switch default (also OBJPHS, id 204, case 0x3c)
    }
    return st == 0 ? 0 : 2;
}

// @src 14038b8a0  group 2 parser: object number, get-or-create, case dispatch, result
int read_object_keyword(DB_STEP_DATA& m, std::FILE* f, int id)
{
    std::int32_t no = 0;
    LLOBJ_DATA* obj = nullptr;
    if (id != 0x11e) {
        if (crt::fscanf(f, "%i", &no) == -1) return 2;
        obj = model::llobj_ctor::get_or_create(&m, no);  // FUN_14019ccc0 (real LLOBJ_DATA ctor via 14017a880)
    }
    if (obj == nullptr) return 2;
    if (handles(id)) return read_case(m, f, *obj, id);
    return ext::read_case(m, f, *obj, id);
}

} // namespace def::key::g2obj
