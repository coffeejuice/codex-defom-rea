// kw-g2bc window BCs: the parser cases 0x52..0x55 of FUN_14038b8a0 (WINPRS 226, WINTMP 227,
// WINATM 228, WINRSE 229). The writers are in win_write.cpp.
//
// Each keyword fills one LLWINDEF_DATA (defmodel/window/llwindef_data.hpp, packet kw-g3) of a
// window list Cls_140269250 inside the object's Cls_1401956d0 at obj+0x2e68:
//   WINPRS obj+0x2eb0 (kind 8)   WINTMP obj+0x2ee0 (kind 3)
//   WINATM obj+0x2f10 (kind 4)   WINRSE obj+0x2fd0 (kind 10)
// Tables of the window (DYNARY, 0x78 bytes each): t380[0..3] at +0x380/+0x3f8/+0x470/+0x4e8
// (WINTMP 1..4), t568[1..2] at +0x5e0/+0x658 (WINATM 1..2), t568[4..5] at +0x748/+0x7c0 (WINPRS
// A, B). Specs: docs/re/keywords/WINPRS.md (shared parts), WINTMP.md, WINATM.md, WINRSE.md.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>

namespace def::key::g2bc {
namespace win {

namespace {

using model::Cls_140269250;
using model::LLWINDEF_DATA;

constexpr int kWinprs = 0xe2;  // 226, case 0x52
constexpr int kWinrse = 0xe5;  // 229, case 0x55

constexpr std::size_t kListWinprs = 0x2eb0;  // Cls_1401956d0 +0x48
constexpr std::size_t kListWintmp = 0x2ee0;  // +0x78
constexpr std::size_t kListWinatm = 0x2f10;  // +0xa8
constexpr std::size_t kListWinrse = 0x2fd0;  // +0x168

// The object's window list at `off`.
//
// INTERIM (stand-in, no @src): in the original the list owner (+0x20, which every new window
// copies and takes its dimension from) is set when the object is built (Cls_1401956d0 ctor
// 1401956d0 / 1401957b0 with obj+0x3a98). kw-g2obj's get_or_create_object shim zero-fills the
// object, so the owner is still null here; set it the way 1401957b0 would. Remove when the
// LLOBJ_DATA ctor is ported.
Cls_140269250* window_list(LLOBJ_DATA& obj, std::size_t off)
{
    Cls_140269250* l = &at<Cls_140269250>(obj, off);
    if (l->owner == nullptr) l->set_owner(&owner_of(obj));  // 14026a120
    return l;
}

// Attachment mode of the header (WINPRS.md table): 0/1 -> 0, 2 -> 1, < 0 -> 2 with the object
// number -m; anything else leaves the window unchanged.
void store_mode(LLWINDEF_DATA* w, std::int32_t mode)
{
    if (mode == 1 || mode == 0) {
        w->orient = 0;
    } else if (mode == 2) {
        w->orient = 1;
    } else if (mode < 0) {
        w->orient = 2;
        w->orient_id = -mode;
    }
}

// Function line of a time table (WINPRS A/B, WINTMP 1 and 4, WINATM 1): code 0 constant, 1 a
// table over variable 1 with (int)v points; other codes leave the table as reset.
void set_time_table(DYNARY& t, std::int32_t code, double v)
{
    if (code == 0) {
        t.ndim = 0;
        t.value = v;
    } else if (code == 1) {
        t.ndim = 1;
        t.var[0] = 1;
        t.n[0] = static_cast<std::int32_t>(v);
    }
}

// Function line of WINTMP tables 2/3 (var2 = 3) and WINATM table 2 (var2 = 0xc): 0 constant,
// 1 f(var 1), 2 f(var2), 3 2D table over (1, var2) with ((int)v, (int)v2) points.
void set_two_var_table(DYNARY& t, std::int32_t code, double v, double v2, std::int32_t var2)
{
    if (code == 0) {
        t.ndim = 0;
        t.value = v;
    } else if (code == 1) {
        t.ndim = 1;
        t.var[0] = 1;
        t.n[0] = static_cast<std::int32_t>(v);
    } else if (code == 2) {
        t.ndim = 1;
        t.var[0] = var2;
        t.n[0] = static_cast<std::int32_t>(v);
    } else if (code == 3) {
        t.ndim = 2;
        t.var[0] = 1;
        t.var[1] = var2;
        t.n[0] = static_cast<std::int32_t>(v);
        t.n[1] = static_cast<std::int32_t>(v2);
    }
}

// part of @src 14038b8a0  case 0x52 WINPRS (decompile lines 2334-2479). With the dimension
// neither 2 nor 3 nothing is read and the status stays 0.
int read_winprs(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int st = 0;  // local_108c (0 after the object-number read)
    double v = 0.0;
    if (is_2d(m)) {
        std::int32_t win = 0, npts = 0, ta = 0, tb = 0, mode = 0;
        st = read_i5(f, &win, &npts, &ta, &tb, &mode, false);  // 140414200
        if (st != 0) return 2;
        // 140269bc0: win <= 0 on an empty list gives null and the original crashes in the reset
        LLWINDEF_DATA* w = model::winlist_extend(window_list(obj, kListWinprs), win);
        model::window_reset(w, 8);                               // 14025d1a0
        st = read_d3(f, &w->u350[0], &w->u350[1], &w->u350[2]);  // 140414c20
        if (st != 0) return 2;
        st = g3::window_read_points_2d(f, w, npts, false);  // 14041b6a0(f, w, n, 0)
        if (st != 0) return 2;
        st = read_d(f, &v);  // 140414990
        if (st != 0) return 2;
        set_time_table(w->t568[4], ta, v);
        st = read_table_body(f, &w->t568[4], 0);  // 14041a800
        if (st != 0) return 2;
        st = read_d(f, &v);
        if (st != 0) return 2;
        set_time_table(w->t568[5], tb, v);
        if (read_table_body(f, &w->t568[5], 0) != 0) return 2;
        st = 0;
        store_mode(w, mode);
    }
    if (is_3d(m)) {
        std::int32_t win = 0, shape = 0, ta = 0, tb = 0, mode = 0;
        st = read_i5(f, &win, &shape, &ta, &tb, &mode, false);
        if (st != 0) return 2;
        LLWINDEF_DATA* w = model::winlist_extend(window_list(obj, kListWinprs), win);
        model::window_reset(w, 8);
        st = read_d3(f, &w->u350[0], &w->u350[1], &w->u350[2]);
        if (st != 0) return 2;
        if (shape == 1) {
            w->shape = 1;
        } else {
            if (shape != 2) return 2;
            w->shape = 2;
        }
        st = g3::window_read_points_3d(f, w, true);  // 14041b7d0(f, w, 1)
        model::window_build_faces(w);                // 140267760
        if (st != 0) return 2;
        st = read_d(f, &v);
        if (st != 0) return 2;
        set_time_table(w->t568[4], ta, v);
        st = read_table_body(f, &w->t568[4], 0);
        if (st != 0) return 2;
        st = read_d(f, &v);
        if (st != 0) return 2;
        set_time_table(w->t568[5], tb, v);
        if (read_table_body(f, &w->t568[5], 0) != 0) return 2;
        st = 0;
        store_mode(w, mode);
    }
    return st == 0 ? 0 : 2;
}

// part of @src 14038b8a0  case 0x53 WINTMP (tokenized header, up to 7 tokens; "not 2D" takes
// the 3D path, also for an unset dimension)
int read_wintmp(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int cnt = 0;
    if (read_tokens(f, &cnt, 7) != 0) return 2;  // 140413900
    const std::int32_t win = token_int(0);       // 140413a50 + 14047b6c0 (strtol, base 10)
    const std::int32_t n = token_int(1);
    const std::int32_t t1 = token_int(2);
    const std::int32_t t2 = token_int(3);
    const std::int32_t t3 = token_int(4);
    const std::int32_t mode = token_int(5);
    const std::int32_t t4 = token_int(6);
    LLWINDEF_DATA* w = model::winlist_extend(window_list(obj, kListWintmp), win);
    model::window_reset(w, 3);
    int st = read_d3(f, &w->u350[0], &w->u350[1], &w->u350[2]);
    if (st != 0) return 2;
    if (!is_2d(m)) {
        if (n == 1) {
            w->shape = 1;
        } else {
            if (n != 2) return 2;
            w->shape = 2;
        }
        st = g3::window_read_points_3d(f, w, true);
        model::window_build_faces(w);
    } else {
        st = g3::window_read_points_2d(f, w, n, false);
    }
    if (st != 0) return 2;
    double v = 0.0, v2 = 0.0;
    if (read_d(f, &v) != 0) return 2;
    set_time_table(w->t380[0], t1, v);
    if (read_table_body(f, &w->t380[0], 0) != 0) return 2;
    if (read_dd(f, &v, &v2) != 0) return 2;  // 140414a40
    set_two_var_table(w->t380[1], t2, v, v2, 3);
    if (read_table_body(f, &w->t380[1], 0) != 0) return 2;
    if (read_dd(f, &v, &v2) != 0) return 2;
    set_two_var_table(w->t380[2], t3, v, v2, 3);
    if (read_table_body(f, &w->t380[2], 0) != 0) return 2;
    st = 0;
    if (cnt == 7) {
        if (read_d(f, &v) != 0) return 2;
        set_time_table(w->t380[3], t4, v);
        // a failed table 4 returns here, before the mode is stored (the status test after the
        // mode store is dead code)
        if (read_table_body(f, &w->t380[3], 0) != 0) return 2;
    }
    store_mode(w, mode);
    return st == 0 ? 0 : 2;
}

// part of @src 14038b8a0  case 0x54 WINATM
int read_winatm(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t win = 0, n = 0, t1 = 0, t2 = 0, mode = 0;
    if (read_i5(f, &win, &n, &t1, &t2, &mode, false) != 0) return 2;
    LLWINDEF_DATA* w = model::winlist_extend(window_list(obj, kListWinatm), win);
    model::window_reset(w, 4);
    int st = read_d3(f, &w->u350[0], &w->u350[1], &w->u350[2]);
    if (st != 0) return 2;
    if (!is_2d(m)) {
        if (n == 1) {
            w->shape = 1;
        } else {
            if (n != 2) return 2;
            w->shape = 2;
        }
        st = g3::window_read_points_3d(f, w, true);
        model::window_build_faces(w);
    } else {
        st = g3::window_read_points_2d(f, w, n, false);
    }
    if (st != 0) return 2;
    double v = 0.0, v2 = 0.0;
    if (read_d(f, &v) != 0) return 2;
    set_time_table(w->t568[1], t1, v);
    if (read_table_body(f, &w->t568[1], 0) != 0) return 2;
    if (read_dd(f, &v, &v2) != 0) return 2;
    set_two_var_table(w->t568[2], t2, v, v2, 0xc);
    st = read_table_body(f, &w->t568[2], 0);
    store_mode(w, mode);  // stored even when table 2's body failed
    if (st != 0) return 2;
    return 0;
}

// part of @src 14038b8a0  case 0x55 WINRSE
int read_winrse(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t win = 0, k = 0;
    if (read_ii(f, &win, &k) != 0) return 2;  // 140413ee0
    if (is_3d(m) && k != 1) return 2;          // before the window is created
    LLWINDEF_DATA* w = model::winlist_extend(window_list(obj, kListWinrse), win);
    model::window_reset(w, 10);
    int st;
    if (!is_2d(m)) {
        if (k != 1) return 2;  // unset dimension: after the reset
        w->shape = 1;
        st = g3::window_read_points_3d(f, w, true);
        model::window_build_faces(w);
    } else {
        if (k < 1) return 2;
        st = g3::window_read_points_2d(f, w, k, true);  // 14041b6a0(f, w, k, 1)
    }
    return st == 0 ? 0 : 2;
}

}  // namespace

bool handles(int id) { return id >= kWinprs && id <= kWinrse; }

// part of @src 14038b8a0  cases 0x52..0x55 (ids 226..229)
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    switch (id - 0x90) {
    case 0x52: return read_winprs(m, f, obj);
    case 0x53: return read_wintmp(m, f, obj);
    case 0x54: return read_winatm(m, f, obj);
    case 0x55: return read_winrse(m, f, obj);
    default: return 2;
    }
}

}  // namespace win
}  // namespace def::key::g2bc
