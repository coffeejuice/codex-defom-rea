// defcmd/obj_cmds.cpp: WRTOBJ, SVSCAL, CPYOBJ, DOESCL (cases 9, 0xc, 0xd, 0xe of FUN_140378e00).
// Decompile: deform-rea-work/out/bm_cmd.c lines 1442-1665; closures out/p67/{wrtobj,svscal,
// cpyobj,doescl}.c. The dispatcher's status variable local_56d8._4_4_ is 0 on entry; each case
// returns 0 at its end (the dispatcher's "iVar4 == 0 -> 0").
#include <defcmd/obj_cmds.hpp>

#include <share.h>

#include <cstdio>

#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/obj_scale.hpp>
#include <defmodel/strtrim.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::objcmd {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

namespace {

std::byte* sub(LLOBJ_DATA* o, std::size_t off) { return reinterpret_cast<std::byte*>(o) + off; }

// 1401d6ea0 = FUN_14017aaf0(model+0x1380, n): list find, no creation (ported by kw-g1a; the
// public form is kw-g2obj's find_object)
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t n) { return key::g2obj::find_object(m, n); }

} // namespace

bool handles(int id) { return id == 9 || id == 12 || id == 13 || id == 14; }

// part of @src 140378e00  cases 9, 0xc, 0xd, 0xe
int run_case(DB_STEP_DATA& m, std::FILE* f, int id, std::FILE* out)
{
    switch (id) {
    case 9: return wrtobj(m, f);
    case 0xc: return svscal(m, f);
    case 0xd: return cpyobj(m, f);
    case 0xe: return doescl(m, f, out);
    default: return 0;
    }
}

// part of @src 140378e00  case 9 WRTOBJ (bm_cmd.c 1442-1464, listing 14037c4e2..14037c5df)
int wrtobj(DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t obj_no = 0;
    std::int32_t out_no = 0;
    // 14037c4fa: the reader's status is dropped; the case tests local_56d8._4_4_ (still 0)
    key::read_ii(f, &obj_no, &out_no);
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);
    LLOBJ_DATA* obj = key::g2obj::find_object(m, obj_no);  // 14017aaf0 directly
    if (obj != nullptr) {
        // 14047b934: _fsopen(name, "wt", _SH_DENYNO); mode string at 1405545ec
        std::FILE* fp = _fsopen(name, "wt", _SH_DENYNO);
        if (fp == nullptr) return 1;
        key::g2obj::write_object(m, fp, obj, static_cast<std::uint32_t>(out_no), false);  // status ignored
        std::fclose(fp);
    }
    return 0;
}

// part of @src 140378e00  case 0xc SVSCAL (bm_cmd.c 1503-1528)
int svscal(DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t obj_no = 0;
    std::int32_t kind = 0;
    std::int32_t count = 0;
    if (key::read_iii(f, &obj_no, &kind, &count, nullptr) != 0) return 2;
    LLOBJ_DATA* obj = find_object(m, obj_no);
    if (obj == nullptr) return 2;
    model::DYNARY t;                 // 14012d050
    model::dynary_init(&t, 0x81);    // 14012d2a0(t, 0x81)
    if (kind == 1) t.var[0] = 7;
    t.n[0] = count;
    if (key::read_table_body(f, &t, 0) != 0) return 2;  // 14041a800; t freed by 14012f410
    model::scale::remap_strain(obj, &t);                 // 140204bc0
    return 0;                                            // 14012f410 (dtor)
}

// part of @src 140378e00  case 0xd CPYOBJ (bm_cmd.c 1529-1575, listing 14037cf57..14037d14d)
int cpyobj(DB_STEP_DATA& m, std::FILE* f)
{
    std::int32_t dst_no = 0;
    std::int32_t mode = 0;
    std::int32_t src_no = 0;
    if (key::read_iii(f, &dst_no, &mode, &src_no, nullptr) != 0) return 2;
    LLOBJ_DATA* d = find_object(m, dst_no);
    if (d == nullptr) return 2;
    LLOBJ_DATA* s = find_object(m, src_no);
    if (s == nullptr) return 2;
    if (mode == 0) {
        // 14037d000..14037d098: R9 = 1, [rsp+0x20..0x40] = 1, 1, 1, 0 (object number kept), 1
        const bool f4 = true, f5 = true, f6 = true, f7 = true, f8 = false, f9 = true;
        model::llobj_copy_from(d, s, m.u15f8, &f4, &f5, &f6, &f7, &f8, &f9);  // 1401613a0
    } else if (mode == 1) {
        const char shapes = 1, by_dim = 1;
        model::llobj_copy::meshgen_copy(sub(d, 0x2d10), sub(s, 0x2d10), &shapes, &by_dim);  // 140189b70
    } else if (mode == 2) {
        model::llobj_copy::movement_copy(sub(d, 0x2160), sub(s, 0x2160));  // 14018bd30
    } else if (mode == 3) {
        model::llobj_copy::props_copy(sub(d, 0x1ea0), sub(s, 0x1ea0));  // 140186590
    }
    return 0;
}

// @src 1401ac710  DOESCL action
std::int32_t doe_scale(DB_STEP_DATA& m, std::int32_t id, const double* factor, std::int32_t n,
                       std::int32_t* ints, std::FILE* out)
{
    switch (id) {
    case 0x5e:
    case 0x5f:
    case 0x68: {
        model::Material* mat = model::material_list_find(&m.materials, ints[0]);  // 14029c2c0
        if (mat == nullptr) {
            rt::crt::fprintf(out, "Error on reading DOESCL : material # %d is not defined.\n", ints[0]);
            return 0;
        }
        // model+0x1668 = W[0x1c]
        rt::crt::fprintf(out, "VarFieldWidth[NODE_VAR_DAMAG]... %d\n", m.u15f8[0x1c]);
        model::scale::scale_material(mat, id, factor, n, ints);  // 14029b3f0
        break;
    }
    case 0x97:
    case 0xaa:
    case 0xe6: {
        LLOBJ_DATA* obj = find_object(m, ints[0]);
        if (obj == nullptr) {
            rt::crt::fprintf(out, "Error on reading DOESCL : object # %d is not defined.\n", ints[0]);
            return 0;
        }
        model::scale::scale_object(obj, id, factor, n, ints);  // 14021d9c0
        break;
    }
    default:
        rt::crt::fprintf(out, "Error on reading DOESCL : undefined keyword definition\n");
        return 0;
    }
    return 1;
}

// part of @src 140378e00  case 0xe DOESCL (bm_cmd.c 1576-1665, listing 14037c855..14037cc94)
int doescl(DB_STEP_DATA& m, std::FILE* f, std::FILE* out)
{
    double factor = 0.0;
    if (key::read_d(f, &factor) != 0) return 2;  // 140414990
    const double scale = factor;                 // local_5158
    // 140484f90(local_6128, local_6528): strcpy between two uninitialised case-0xb buffers whose
    // result is never read; no-op here.
    int cnt = 0;
    if (key::read_tokens(f, &cnt, 8) != 0) return 2;  // 140413900
    std::int32_t ints[7] = {0, 0, 0, 0, 0, 0, 0};
    int group = 0;
    const std::int32_t id = model::lookup_keyword(key::token(0), &group);  // 1402f76c0
    for (int k = 1; k <= 7; ++k) {
        if (k < cnt) ints[k - 1] = key::token_int(k);  // 14047b6c0 strtol base 10
    }
    switch (id - 0x5e) {
    case 0: rt::crt::fprintf(out, "Scale %lf FSTRES : Mat# %d\n", scale, ints[0]); break;
    case 1: rt::crt::fprintf(out, "Scale %lf YOUNG  : Mat# %d\n", scale, ints[0]); break;
    case 10: rt::crt::fprintf(out, "Scale %lf FPERV  : Mat# %d [%d %d]\n", scale, ints[0], ints[1], ints[2]); break;
    case 0x39:
        if (ints[1] == 0) {
            rt::crt::fprintf(out, "Scale energy %lf MOVCTL : Obj# %d type %d\n", scale, ints[0], 0);
        } else if (ints[1] == 1) {
            rt::crt::fprintf(out, "Assign energy %lf MOVCTL : Obj# %d type %d\n", scale, ints[0], 1);
        } else if (ints[1] == 2) {
            rt::crt::fprintf(out, "Assign efficiency %lf MOVCTL : Obj# %d type %d\n", scale, ints[0], 2);
        }
        break;
    case 0x4c: rt::crt::fprintf(out, "Scale %lf NDTMP  : Obj# %d\n", scale, ints[0]); break;
    case 0x88:
        rt::crt::fprintf(out, "Scale %lf LOCTMP : Obj# %d Win# %d [%d %d %d %d %d]\n", scale, ints[0], ints[1],
                         ints[2], ints[3], ints[4], ints[5], ints[6]);
        break;
    default: break;
    }
    doe_scale(m, id, &scale, 7, ints, out);  // return value ignored
    return 0;
}

} // namespace def::cmd::objcmd
