// kw-g2bc window BCs: the KEY writers. write_window_lists is the inline part of the object writer
// FUN_14029e250 for writer-map steps 48-52 (WINPRS 2D loop, WINPRS 3D loop, WINTMP cleanup,
// WINTMP, WINATM, WINRSE); write_winprs_block is FUN_1402ab880 (type-5 objects), whose two loops
// are the same as step 48. Specs: docs/re/keywords/WINPRS.md, WINTMP.md, WINATM.md, WINRSE.md.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <cstdlib>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/bc/window_bc.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g2bc {

namespace {

using model::Cls_140269250;
using model::LLWINDEF_DATA;
namespace crt = def::rt::crt;

constexpr int kWinprs = 0xe2;
constexpr int kWintmp = 0xe3;
constexpr int kWinatm = 0xe4;
constexpr int kWinrse = 0xe5;

constexpr std::size_t kListWinprs = 0x2eb0;
constexpr std::size_t kListWintmp = 0x2ee0;
constexpr std::size_t kListWinatm = 0x2f10;
constexpr std::size_t kListWinrse = 0x2fd0;

Cls_140269250* window_list(LLOBJ_DATA& obj, std::size_t off) { return &at<Cls_140269250>(obj, off); }

// Header code of the attachment mode: +0x368 0 -> 1, 1 -> 2, 2 -> -abs(+0x36c), other -> 0.
std::int32_t mode_code(const LLWINDEF_DATA* w)
{
    std::int32_t c = 0;
    if (w->orient == 0) {
        c = 1;
    } else if (w->orient == 1) {
        c = 2;
    } else if (w->orient == 2) {
        c = std::abs(w->orient_id);  // 140484ab8
        c = -c;
    }
    return c;
}

// Header code of a two-variable table (WINTMP 2/3, WINATM 2): ndim 0 -> 0; ndim 1 -> 1 when the
// variable is 1, else 2; any other ndim -> 3.
std::int32_t table_code(const DYNARY& t)
{
    if (t.ndim == 0) return 0;
    if (t.ndim == 1) return t.var[0] == 1 ? 1 : 2;
    return 3;
}

// The WINPRS records of one object: every window in 2D, then (separate loop) every window in 3D.
// Shared by step 48 of 14029e250 and by 1402ab880, which have the same code.
void write_winprs_entries(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    Cls_140269250* l = window_list(obj, kListWinprs);
    if (is_2d(m)) {
        for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {  // 14019a590
            LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);                 // 140269900
            const std::uint32_t a = w->t568[4].ndim != 0;
            const std::uint32_t b = w->t568[5].ndim != 0;
            const std::int32_t mc = mode_code(w);
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kWinprs), n, w->index,
                         w->npts, a, b, mc);
            crt::fprintf(f, " %20.10E %20.10E\n", w->u350[0], w->u350[1]);
            g3::window_write_points(f, w, 1);         // 1402f4890
            write_table_body(f, &w->t568[4], 1);      // 1402f3130
            write_table_body(f, &w->t568[5], 1);
        }
    }
    if (is_3d(m)) {
        for (std::int32_t k = 0; k < model::winlist_count(l); k = k + 1) {
            LLWINDEF_DATA* w = model::winlist_nth(l, k + 1);
            const std::uint32_t a = w->t568[4].ndim != 0;
            const std::uint32_t b = w->t568[5].ndim != 0;
            const std::int32_t mc = mode_code(w);
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kWinprs), n, w->index,
                         w->shape, a, b, mc);
            crt::fprintf(f, " %20.10E %20.10E %20.10E\n", w->u350[0], w->u350[1], w->u350[2]);
            g3::window_write_points(f, w, 0);
            write_table_body(f, &w->t568[4], 1);
            write_table_body(f, &w->t568[5], 1);
        }
    }
}

}  // namespace

// @src 1402ab880  WINPRS records of a type-5 object (N = n when > 0, else obj+8); returns 0
int write_winprs_block(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    // the original tests obj != null first; a reference is never null
    std::uint32_t no = n;
    if (static_cast<std::int32_t>(n) < 1) no = static_cast<std::uint32_t>(at<std::int32_t>(obj, 8));
    write_winprs_entries(m, f, obj, no);
    return 0;
}

// part of @src 14029e250  writer-map steps 48-52 (N is the object writer's local_708)
void write_window_lists(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    // step 48: WINPRS
    write_winprs_entries(m, f, obj, n);

    // step 49: drop WINTMP windows without shape data. After a removal the index restarts at 0
    // and is then incremented, so the scan goes on at entry 2: entry 1 is tested only once.
    Cls_140269250* lt = window_list(obj, kListWintmp);
    std::int32_t k = 0;
    while (k < model::winlist_count(lt)) {
        LLWINDEF_DATA* w = model::winlist_nth(lt, k + 1);
        if (!model::window_has_shape(w)) {  // 1402632d0
            model::winlist_remove(lt, w);   // 1402696f0
            k = 0;
        }
        k = k + 1;
    }

    // step 50: WINTMP
    for (k = 0; k < model::winlist_count(lt); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(lt, k + 1);
        const std::uint32_t c1 = w->t380[0].ndim != 0;
        const std::int32_t c2 = table_code(w->t380[1]);
        const std::int32_t c3 = table_code(w->t380[2]);
        const std::uint32_t c4 = w->t380[3].ndim != 0;
        const std::int32_t mc = mode_code(w);
        if (!is_2d(m)) {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n", keyword_label(kWintmp), n,
                         w->index, w->shape, c1, c2, c3, mc, c4);
            crt::fprintf(f, " %20.10E %20.10E %20.10E\n", w->u350[0], w->u350[1], w->u350[2]);
            g3::window_write_points(f, w, 0);
        } else {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n", keyword_label(kWintmp), n,
                         w->index, w->npts, c1, c2, c3, mc, c4);
            crt::fprintf(f, " %20.10E %20.10E\n", w->u350[0], w->u350[1]);
            g3::window_write_points(f, w, 1);
        }
        write_table_body(f, &w->t380[0], 1);
        write_table_body(f, &w->t380[1], 1);
        write_table_body(f, &w->t380[2], 1);
        write_table_body(f, &w->t380[3], 1);
    }

    // step 51: WINATM
    Cls_140269250* la = window_list(obj, kListWinatm);
    for (k = 0; k < model::winlist_count(la); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(la, k + 1);
        const std::uint32_t c1 = w->t568[1].ndim != 0;
        const std::int32_t c2 = table_code(w->t568[2]);
        const std::int32_t mc = mode_code(w);
        if (!is_2d(m)) {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kWinatm), n, w->index,
                         w->shape, c1, c2, mc);
            crt::fprintf(f, " %20.10E %20.10E %20.10E\n", w->u350[0], w->u350[1], w->u350[2]);
            g3::window_write_points(f, w, 0);
        } else {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kWinatm), n, w->index,
                         w->npts, c1, c2, mc);
            crt::fprintf(f, " %20.10E %20.10E\n", w->u350[0], w->u350[1]);
            g3::window_write_points(f, w, 1);
        }
        write_table_body(f, &w->t568[1], 1);
        write_table_body(f, &w->t568[2], 1);
    }

    // step 52: WINRSE (nothing when the dimension is unset)
    Cls_140269250* lr = window_list(obj, kListWinrse);
    for (k = 0; k < model::winlist_count(lr); k = k + 1) {
        LLWINDEF_DATA* w = model::winlist_nth(lr, k + 1);
        if (!is_2d(m)) {
            if (is_3d(m)) {
                crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(kWinrse), n, w->index, w->shape);
                g3::window_write_points(f, w, 0);
            }
        } else {
            crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(kWinrse), n, w->index, w->npts);
            g3::window_write_points(f, w, 0);
        }
    }
}

}  // namespace def::key::g2bc
