// Group 2 writers of packet kw-g2mg: the mesh-settings block FUN_1402a9180, the OPSTOP list
// FUN_1402a4020, the DYNARY list FUN_1402a41c0 and the MG* registry handlers. Formats are the
// binary's strings, doubles through def::rt::crt::fprintf (the VS2008 CRT float path).
// Specs: docs/re/keywords/MGNELM.md (block order), MGUSER.md, OPSTOP.md, DYNARY.md.
#include <defkey/kw_g2mg.hpp>

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/table_analysis.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/opstop_data.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "common.hpp"

namespace def::key::g2mg {

using model::DYNARY_DATA;
using model::LLWINDEF_DATA;
using model::OPSTOP_DATA;
namespace crt = def::rt::crt;

namespace {

// The handler pattern: the object by ref[0] (14017aaf0, no null test, as in the original), its
// +0x3f24 number when > 0, then one "%-6s %7i ..." line.
LLOBJ_DATA& handler_object(DB_STEP_DATA& m, const KeyRef& ref, std::uint32_t* no)
{
    LLOBJ_DATA* o = g2obj::find_object(m, ref[0]);
    *no = static_cast<std::uint32_t>(ref[0]);
    if (0 < obj_out_no(*o)) {
        *no = static_cast<std::uint32_t>(obj_out_no(*o));
    }
    return *o;
}

int write_scalar(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref, int id, std::size_t off)
{
    std::uint32_t no = 0;
    LLOBJ_DATA& o = handler_object(m, ref, &no);
    crt::fprintf(f, "%-6s %7i %20.10E\n", keyword_label(id), no, at<double>(o, off));
    return 0;
}

} // namespace

// @src 1402b44e0  MGSIZR (+0x2d20)
int write_mgsizr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xba, 0x2d20); }
// @src 1402b45b0  MGWCUV (+0x2d48)
int write_mgwcuv(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xbb, 0x2d48); }
// @src 1402b4680  MGWTMP (+0x2d28)
int write_mgwtmp(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xbc, 0x2d28); }
// @src 1402b4750  MGWSTN (+0x2d30)
int write_mgwstn(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xbd, 0x2d30); }
// @src 1402b4820  MGWSTR (+0x2d38)
int write_mgwstr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xbe, 0x2d38); }

// @src 1402b48f0  MGERR (+0x2db0, +0x2db8)
int write_mgerr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    std::uint32_t no = 0;
    LLOBJ_DATA& o = handler_object(m, ref, &no);
    crt::fprintf(f, "%-6s %7i %20.10E %20.10E\n", keyword_label(0xc1), no, at<double>(o, 0x2db0),
                 at<double>(o, 0x2db8));
    return 0;
}

bool handles_write(int id)
{
    return (186 <= id && id <= 190) || id == 193;
}

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    switch (id) {
    case 186: return write_mgsizr(f, m, ref);
    case 187: return write_mgwcuv(f, m, ref);
    case 188: return write_mgwtmp(f, m, ref);
    case 189: return write_mgwstn(f, m, ref);
    case 190: return write_mgwstr(f, m, ref);
    case 193: return write_mgerr(f, m, ref);
    default: return 0;
    }
}

// @src 1402a9180  mesh-settings block
int write_mesh_block(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
    const KeyRef ref{obj_number(obj)}; // 140159860 / 1402b7530 (obj+8); each handler a copy
    int rc = g2obj::write_rmdpth(f, m, ref); // 1402b49d0
    if (rc != 0) return rc;
    rc = g2obj::write_rmstrk(f, m, ref); // 1402b4aa0
    if (rc != 0) return rc;
    rc = g2obj::write_rmtime(f, m, ref); // 1402b4b70
    if (rc != 0) return rc;
    crt::fprintf(f, "%-6s %7i %20.10E\n", keyword_label(0xb7), no, at<double>(obj, 0x2dd8)); // RMSTEP
    rc = write_mgsizr(f, m, ref);
    if (rc != 0) return rc;
    rc = write_mgwtmp(f, m, ref);
    if (rc != 0) return rc;
    rc = write_mgwstn(f, m, ref);
    if (rc != 0) return rc;
    rc = write_mgwstr(f, m, ref);
    if (rc != 0) return rc;
    if (!is_2d(m)) { // MGNELM
        crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", keyword_label(0xb8), no,
                     at<std::int32_t>(obj, 0x2d88), at<std::int32_t>(obj, 0x2d8c),
                     at<std::int32_t>(obj, 0x2d90), at<std::int32_t>(obj, 0x2d94));
    }
    else {
        crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_label(0xb8), no,
                     at<std::int32_t>(obj, 0x2d80), at<std::int32_t>(obj, 0x2d8c),
                     at<std::int32_t>(obj, 0x2d94));
    }
    crt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0xb9), no, at<std::int32_t>(obj, 0x2d84)); // MGTELM
    rc = write_mgwcuv(f, m, ref);
    if (rc != 0) return rc;
    crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0xc0), no, at<std::int32_t>(obj, 0x2dc0),
                 at<std::int32_t>(obj, 0x2dc4)); // MGGRID
    rc = write_mgerr(f, m, ref);
    if (rc != 0) return rc;
    if (!is_2d(m)) { // MGWUSR
        crt::fprintf(f, "%-6s %7i %20.10E %20.10E %7i\n", keyword_label(0xbf), no,
                     at<double>(obj, 0x2d40), at<double>(obj, 0x2d98), at<std::int32_t>(obj, 0x2da0));
    }
    else {
        crt::fprintf(f, "%-6s %7i %20.10E\n", keyword_label(0xbf), no, at<double>(obj, 0x2d40));
    }
    // MGUSER: the mesh windows with a positive size; renumbered 1..n in 3D
    std::int32_t n = 0;
    std::int32_t opt_out = 0;  // persists across windows
    std::int32_t shape_out = 0;
    model::Cls_140269250& wl = mesh_windows(obj);
    for (std::int32_t k = 0; k < model::winlist_count(&wl); k = k + 1) { // 14019a590
        LLWINDEF_DATA* w = model::winlist_nth(&wl, k + 1);               // 140269900
        double size = 0.0;
        if (w != nullptr) {
            std::memcpy(&size, &w->u370, 8);
        }
        if (w == nullptr || !(0.0 < size)) {
            continue;
        }
        if (w->orient == 0) {
            opt_out = 1;
        }
        else if (w->orient == 1) {
            opt_out = 2;
        }
        else if (w->orient == 2) {
            opt_out = -std::abs(w->orient_id); // 140484ab8
        }
        if (!is_2d(m)) {
            if (w->shape == 1) {
                shape_out = 1;
            }
            else if (w->shape == 2) {
                shape_out = 2;
            }
            else if (w->shape == 3) {
                shape_out = 3;
            }
            else if (w->shape == 0xb || w->shape == 0xc || w->shape == 0xd) {
                shape_out = 0xb;
            }
            else {
                return 3;
            }
            n = n + 1;
            if (shape_out < 0xb) {
                crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i %7i\n",
                             keyword_label(0xc2), no, n, size, w->u350[0], w->u350[1], w->u350[2],
                             shape_out, opt_out);
            }
            else {
                crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i %7i %7i\n",
                             keyword_label(0xc2), no, n, size, w->u350[0], w->u350[1], w->u350[2],
                             shape_out, opt_out, w->u378);
            }
            g3::window_write_points(f, w, 0); // 1402f4890
        }
        else {
            crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i\n", keyword_label(0xc2),
                         no, w->index, w->npts, size, w->u350[0], w->u350[1], opt_out);
            g3::window_write_points(f, w, 0);
        }
    }
    return 0;
}

// @src 1402a4020  OPSTOP list
int write_opstop(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
    if (f == nullptr) { // obj is a reference here; the original also returns 3 for a null obj
        return 3;
    }
    if (no == 0) {
        no = static_cast<std::uint32_t>(obj_number(obj));
    }
    crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x109), no, 0, 0);
    model::OpStopLinkedList& l = opstop_list(obj);
    for (std::int32_t i = 0; i < model::sf_list_count(&l); i = i + 1) { // 14019a560
        model::SfLinkedData* d = model::sf_list_nth(&l, i + 1);         // 14025a9e0
        if (d == nullptr || (obj_type(obj) == 5 && model::sf_data_active(d) == 0)) { // 140161020, 14025a660
            continue;
        }
        const OPSTOP_DATA* e = OPSTOP_DATA::cast(d); // 14025ade0
        if (e != nullptr && e->u0c == 1) {
            crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x109), no, i + 1, e->u0c);
            crt::fprintf(f, " %20.10E\n", e->value);
        }
    }
    return 0;
}

// @src 1402a41c0  DYNARY list
int write_dynary(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
    if (f == nullptr) { // obj is a reference here; the original also returns 3 for a null obj
        return 3;
    }
    if (no == 0) {
        no = static_cast<std::uint32_t>(obj_number(obj));
    }
    model::DynAryLinkedList& l = dynary_list(obj);
    for (std::int32_t i = 0; i < model::sf_list_count(&l); i = i + 1) { // 1402b73e0
        DYNARY_DATA* rec = model::dynary_list_nth(&l, i + 1); // 14025bb50, then 14025b1d0 again
        if (rec == nullptr) {
            continue;
        }
        const std::int32_t num = i + 1;
        const std::int32_t kind = model::dynary_data_kind(rec);
        const std::int32_t sub = model::dynary_data_subtype(rec);
        std::int32_t count = model::dynary_data_count(rec);
        std::int32_t ncomp = model::dynary_data_ncomp(rec);
        const bool is_int = model::dynary_data_is_int(rec) != 0;
        const std::int32_t cls = model::dynary_data_class(rec);
        if (cls == 1 && !is_int) {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(0x10a), no, num, kind, sub,
                         count, ncomp);
            const double* v = model::dynary_data_doubles(rec);
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (ncomp == 2) {
                    crt::fprintf(f, " %7i %20.10E %20.10E\n", r + 1, v[r * 2], v[r * 2 + 1]);
                }
                else {
                    if (ncomp != 3) {
                        return 3;
                    }
                    crt::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", r + 1, v[r * 3], v[r * 3 + 1],
                                 v[r * 3 + 2]);
                }
            }
        }
        else if (cls == 2 && is_int) {
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(0x10a), no, num, kind, sub,
                         count, ncomp);
            const std::int32_t* v = model::dynary_data_ints(rec);
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (ncomp == 3) {
                    crt::fprintf(f, " %7i %7i %7i %7i\n", r + 1, v[r * 3], v[r * 3 + 1], v[r * 3 + 2]);
                }
                else if (ncomp == 4) {
                    crt::fprintf(f, " %7i %7i %7i %7i %7i\n", r + 1, v[r * 4], v[r * 4 + 1],
                                 v[r * 4 + 2], v[r * 4 + 3]);
                }
                else {
                    if (ncomp != 8) {
                        return 3;
                    }
                    const std::int32_t* p = &v[r * 8];
                    crt::fprintf(f, " %7i %7i %7i %7i %7i %7i %7i %7i %7i\n", r + 1, p[0], p[1], p[2],
                                 p[3], p[4], p[5], p[6], p[7]);
                }
            }
        }
        else {
            if (cls != 3 || is_int) {
                return 3;
            }
            const double* v = model::dynary_data_doubles(rec);
            double reference = 0.0;
            const std::int32_t mode = 1;
            char uniform = 1;
            std::int32_t nrec = 0;
            double* defaults = static_cast<double*>(std::calloc(ncomp, 8));
            analyze_table(v, &count, &ncomp, &reference, &mode, &uniform, &nrec, defaults); // 1402f5360
            crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %20.10E\n", keyword_label(0x10a), no, num,
                         kind, sub, count, ncomp, nrec, defaults[0]);
            if (uniform == 0) {
                for (std::int32_t r = 0; r < count; r = r + 1) {
                    bool any = false;
                    for (std::int32_t k = 0; k < ncomp; k = k + 1) {
                        if (v[r * ncomp + k] != reference) {
                            any = true;
                            break;
                        }
                    }
                    if (any) {
                        crt::fprintf(f, " %7i", r + 1);
                        if (write_doubles(f, ncomp, &v[r * ncomp], 5, false) != 0) { // 1402f2e80
                            break;
                        }
                    }
                }
            }
            if (defaults != nullptr) {
                std::free(defaults);
            }
        }
    }
    return 0;
}

} // namespace def::key::g2mg
