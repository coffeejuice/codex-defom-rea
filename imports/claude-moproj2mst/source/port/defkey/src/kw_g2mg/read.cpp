// Group 2 parser cases 0x28..0x33 (MG*), 0x79 (OPSTOP) and 0x7a (DYNARY) of FUN_14038b8a0,
// packet kw-g2mg. Each returns what the parser returns: a helper status != 0 becomes 2.
// Specs: docs/re/keywords/<KW>.md; decompile lines 1923-2063, 3435-3570 of p_main.c.
#include <defkey/kw_g2mg.hpp>

#include <cstdint>
#include <cstring>

#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/opstop_data.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>

#include "common.hpp"

namespace def::key::g2mg {

using model::DYNARY_DATA;
using model::LLWINDEF_DATA;
using model::OPSTOP_DATA;

namespace {

constexpr int kFirstId = 0x90; // the switch is on id - 0x90

// part of @src 14038b8a0  case 0x28 MGNELM
int read_mgnelm(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    int st = 0;
    if (!is_2d(m)) {
        st = read_i4(f, &at<std::int32_t>(obj, 0x2d88), &at<std::int32_t>(obj, 0x2d8c),
                     &at<std::int32_t>(obj, 0x2d90), &at<std::int32_t>(obj, 0x2d94)); // 140414100
        std::int32_t& flag = at<std::int32_t>(obj, 0x2d94);
        if (flag < 0 || 1 < flag) {
            flag = 0;
        }
    }
    else {
        int n = 0;
        st = read_tokens(f, &n, 4); // 140413900
        if (st != 0) {
            return 2;
        }
        at<std::int32_t>(obj, 0x2d80) = token_int(0); // 140413a50, 14047b6c0
        if (1 < n) {
            at<std::int32_t>(obj, 0x2d8c) = token_int(1);
            at<std::int32_t>(obj, 0x2d94) = token_int(2);
        }
    }
    if (obj_type(obj) == 5) {
        at<std::uint8_t>(obj, 0x2e05) = 0;
    }
    return st;
}

// part of @src 14038b8a0  case 0x32 MGUSER
int read_mguser(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t win = 0;
    std::int32_t np = 0;
    double size = 0.0, p1 = 0.0, p2 = 0.0, p3 = 0.0, shape = 0.0, opt = 0.0, flag = 0.0;
    int st = 0;
    if (!is_2d(m)) {
        double* const d[7] = {&size, &p1, &p2, &p3, &shape, &opt, &flag};
        st = read_i_d7(f, &win, d); // 140415d70
    }
    else {
        double* const d[4] = {&size, &p1, &p2, &opt};
        st = read_ii_d4(f, &win, &np, d); // 140416220
    }
    LLWINDEF_DATA* w = model::winlist_extend(&mesh_windows(obj), win); // 140269bc0
    model::window_reset(w, 2);                                        // 14025d1a0
    std::memcpy(&w->u370, &size, 8);                                  // +0x370 size
    if (!is_2d(m)) {
        w->u350[0] = p1;
        w->u350[1] = p2;
        w->u350[2] = p3;
        const std::int32_t s = static_cast<std::int32_t>(shape);
        if (s == 1 || s == 0) {
            w->shape = 1;
        }
        else if (s == 2 || s == 3 || s == 0xb || s == 0xc || s == 0xd) {
            w->shape = s;
        }
        else {
            return 2;
        }
        st = g3::window_read_points_3d(f, w, true); // 14041b7d0
        model::window_build_faces(w);          // 140267760
    }
    else {
        w->u350[0] = p1;
        w->u350[1] = p2;
        st = g3::window_read_points_2d(f, w, np, true); // 14041b6a0
    }
    const std::int32_t o = static_cast<std::int32_t>(opt);
    if (o == 1 || o == 0) {
        w->orient = 0;
    }
    else if (o == 2) {
        w->orient = 1;
    }
    else if (o < 0) {
        w->orient = 2;
        w->orient_id = -o;
    }
    if (w->shape == 0xb || w->shape == 0xc || w->shape == 0xd) {
        const std::int32_t fl = static_cast<std::int32_t>(flag);
        if (fl == 0) {
            w->u378 = 0;
        }
        else if (fl == 1) {
            w->u378 = 1;
        }
    }
    return st;
}

// part of @src 14038b8a0  case 0x79 OPSTOP
int read_opstop(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t idx = 0;
    std::int32_t type = 0;
    if (read_ii(f, &idx, &type) != 0) { // 140413ee0
        return 2;
    }
    model::OpStopLinkedList& l = opstop_list(obj);
    if (idx == 0) {
        l.clear(); // 14025ab20
        return 0;
    }
    OPSTOP_DATA* e = OPSTOP_DATA::cast(model::sf_list_get_or_create(&l, idx)); // 14025a990, 14025ade0
    if (e != nullptr) {
        model::sf_data_set_active(e, 1); // 14025a640
        e->u0c = type;
        if (e->u0c == 1 && read_d(f, &e->value) != 0) { // 140414990
            return 2;
        }
    }
    return 0;
}

// part of @src 14038b8a0  case 0x7a DYNARY
int read_dynary(std::FILE* f, LLOBJ_DATA& obj)
{
    int n = 0;
    if (read_tokens(f, &n, 10) != 0) { // 140413900
        return 2;
    }
    const std::int32_t idx = token_int(0);
    std::int32_t kind = token_int(1);
    const std::int32_t sub = token_int(2);
    const std::int32_t count = token_int(3);
    const std::int32_t ncomp = token_int(4);
    const std::int32_t nrec = n < 6 ? count : token_int(5);
    const double dflt = 6 < n ? token_double(6) : 0.0; // 14048754c
    DYNARY_DATA* rec = DYNARY_DATA::cast(model::sf_list_get_or_create(&dynary_list(obj), idx)); // 14025b1d0
    if (rec == nullptr) {
        return 0;
    }
    if (kind < 1 || 1 < kind) {
        kind = 1;
    }
    model::dynary_data_set_header(rec, kind, sub, count, ncomp); // 14025b220
    const bool is_int = model::dynary_data_is_int(rec) != 0;     // 14025b960
    const std::int32_t cls = model::dynary_data_class(rec);      // 14025b3f0
    std::int32_t idx_tok = 0;
    if (cls == 1 && !is_int) {
        double* v = model::dynary_data_doubles(rec); // 14025b470
        for (std::int32_t i = 0; i < count; i = i + 1) {
            int st = 0;
            if (ncomp == 2) {
                st = read_i_dd(f, &idx_tok, &v[i * 2], &v[i * 2 + 1]); // 140415680
            }
            else if (ncomp == 3) {
                st = read_i_d3(f, &idx_tok, &v[i * 3], &v[i * 3 + 1], &v[i * 3 + 2]); // 140415880
            }
            if (st != 0) {
                return 2;
            }
        }
    }
    else if (cls == 2 && is_int) {
        std::int32_t* v = model::dynary_data_ints(rec); // 14025b4a0
        for (std::int32_t i = 0; i < count; i = i + 1) {
            int st = 0;
            if (ncomp == 3) {
                st = read_i4(f, &idx_tok, &v[i * 3], &v[i * 3 + 1], &v[i * 3 + 2]); // 140414100
            }
            else if (ncomp == 4) {
                st = read_i5(f, &idx_tok, &v[i * 4], &v[i * 4 + 1], &v[i * 4 + 2], &v[i * 4 + 3],
                             false); // 140414200
            }
            else if (ncomp == 8) {
                std::int32_t* const out[9] = {&idx_tok, &v[i * 8], &v[i * 8 + 1], &v[i * 8 + 2],
                                              &v[i * 8 + 3], &v[i * 8 + 4], &v[i * 8 + 5],
                                              &v[i * 8 + 6], &v[i * 8 + 7]};
                st = read_i9(f, out, false); // 140414600
            }
            if (st != 0) {
                return 2;
            }
        }
    }
    else if (cls == 3 && !is_int) {
        model::dynary_data_fill(rec, dflt); // 14025b580
        double* v = model::dynary_data_doubles(rec);
        return read_value_records(f, v, nrec, count, ncomp) == 0 ? 0 : 2; // 1404199a0
    }
    return 0;
}

} // namespace

bool handles(int id)
{
    return (184 <= id && id <= 195) || id == 265 || id == 266;
}

// part of @src 14038b8a0  cases 0x28..0x33, 0x79, 0x7a
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    int st = 0;
    double v = 0.0;
    switch (id - kFirstId) {
    case 0x28: // MGNELM
        st = read_mgnelm(m, f, obj);
        break;
    case 0x29: // MGTELM: (int) of a double, also on failure
        st = read_d(f, &v);
        at<std::int32_t>(obj, 0x2d84) = static_cast<std::int32_t>(v);
        break;
    case 0x2a: // MGSIZR
        st = read_d(f, &at<double>(obj, 0x2d20));
        break;
    case 0x2b: // MGWCUV
        st = read_d(f, &at<double>(obj, 0x2d48));
        break;
    case 0x2c: // MGWTMP
        st = read_d(f, &at<double>(obj, 0x2d28));
        break;
    case 0x2d: // MGWSTN
        st = read_d(f, &at<double>(obj, 0x2d30));
        break;
    case 0x2e: // MGWSTR
        st = read_d(f, &at<double>(obj, 0x2d38));
        break;
    case 0x2f: // MGWUSR
        if (!is_2d(m)) {
            st = read_d3(f, &at<double>(obj, 0x2d40), &at<double>(obj, 0x2d98), &v); // 140414c20
            at<std::int32_t>(obj, 0x2da0) = static_cast<std::int32_t>(v);
        }
        else {
            st = read_d(f, &at<double>(obj, 0x2d40));
        }
        break;
    case 0x30: { // MGGRID
        double a = 0.0;
        double b = 0.0;
        st = read_dd(f, &a, &b); // 140414a40
        at<std::int32_t>(obj, 0x2dc0) = static_cast<std::int32_t>(a);
        at<std::int32_t>(obj, 0x2dc4) = static_cast<std::int32_t>(b);
        break;
    }
    case 0x31: // MGERR
        st = read_dd(f, &at<double>(obj, 0x2db0), &at<double>(obj, 0x2db8));
        break;
    case 0x32: // MGUSER
        st = read_mguser(m, f, obj);
        break;
    case 0x33: // MGSIZM: 3D only; in 2D nothing is consumed and the status stays 0
        if (is_3d(m)) {
            st = read_d(f, &at<double>(obj, 0x2da8));
        }
        break;
    case 0x79:
        return read_opstop(f, obj);
    case 0x7a:
        return read_dynary(f, obj);
    default:
        return 2;
    }
    return st == 0 ? 0 : 2;
}

} // namespace def::key::g2mg
