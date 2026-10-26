// FSTRES (id 94): flow stress. docs/re/keywords/FSTRES.md. Block Material::fstres
// (Cls_140288ae0 at material+0x50); offsets below are material offsets as in the spec.
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/detail.hpp"
#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>

#include <cstring>

namespace def::key::g4a {

using def::model::DYNARY;
using io::kfprintf;

namespace {

// The "s" selector of t 6 (Y, H) and t 12 (F): 0 const a, 1 var 3 n=(int)a, 2 var `var2`,
// 3 (3, var2) n=(int)a,(int)b; other: left as re-initialised.
void set_selector_shape(DYNARY* d, std::int32_t s, double a, double b, std::int32_t var2)
{
    if (s == 0) {
        d->ndim = 0;
        d->value = a;
    } else if (s == 1) {
        d->ndim = 1;
        d->var[0] = 3;
        d->n[0] = static_cast<std::int32_t>(a);
    } else if (s == 2) {
        d->ndim = 1;
        d->var[0] = var2;
        d->n[0] = static_cast<std::int32_t>(a);
    } else if (s == 3) {
        d->ndim = 2;
        d->var[0] = 3;
        d->var[1] = var2;
        d->n[0] = static_cast<std::int32_t>(a);
        d->n[1] = static_cast<std::int32_t>(b);
    }
}

} // namespace

// part of @src 140384a50  case 0x2 (FSTRES); returns the parser's result (0 or 2)
int read_fstres(std::FILE* fp, Material& mat)
{
    def::model::Cls_140288ae0& f = mat.fstres;
    std::int32_t t = 0, s1 = 0, s2 = 0;
    int st = io::line_iii(fp, &t, &s1, &s2);
    if (st != 0) return 2;
    if (t == 1) {
        f.model = 0;
        st = io::line_d4(fp, &f.c_t1[0], &f.c_t1[1], &f.c_t1[2], &f.c_t1[3]);
    } else if (t == 2 || t == 3) {
        f.model = 1;
        io::dyn_reinit_keep(&f.table, 0x4f);
        st = io::line_iii(fp, &f.table.n[0], &f.table.n[1], &f.table.n[2]);
        if (st != 0) return 2;
        st = io::read_body(fp, &f.table);
        f.table.u50 = (t == 2) ? 1 : 0;  // mat+0xc8
    } else if (t == 4) {
        f.model = 2;
        st = io::line_d5(fp, &f.c_t4[0], &f.c_t4[1], &f.c_t4[2], &f.c_t4[3], &f.c_t4[4]);
    } else if (t == 5) {
        f.model = 3;
        st = io::line_d4(fp, &f.c_t5[0], &f.c_t5[1], &f.c_t5[2], &f.c_t5[3]);
    } else if (t == 6) {
        f.model = 4;
        double a = 0.0, b = 0.0;
        st = io::line_dd(fp, &a, &b);
        if (st != 0) return 2;
        io::dyn_reinit_keep(&f.y, 0x24);
        set_selector_shape(&f.y, s1, a, b, 0xc);
        st = io::read_body(fp, &f.y);
        if (st != 0) return 2;
        st = io::line_dd(fp, &a, &b);
        if (st != 0) return 2;
        io::dyn_reinit_keep(&f.h, 0x25);
        set_selector_shape(&f.h, s2, a, b, 0xc);
        st = io::read_body(fp, &f.h);
    } else if (t == 7 || t == 8) {
        f.model = 5;
        io::dyn_reinit_keep(&f.s, 0x50);
        st = io::line_iii(fp, &f.s.n[0], &f.s.n[1], &f.s.n[2]);
        if (st != 0) return 2;
        st = io::read_body(fp, &f.s);
        f.s.u50 = (t == 7) ? 1 : 0;  // mat+0x278
    } else if (t == 9) {
        f.model = 6;
        double* v1[7] = {&f.c_t9[0], &f.c_t9[1], &f.c_t9[2], &f.c_t9[3], &f.c_t9[4], &f.c_t9[5], &f.c_t9[6]};
        st = io::line_d7(fp, v1);
        double* v2[7] = {&f.c_t9[7], &f.c_t9[8], &f.c_t9[9], &f.c_t9[10], &f.c_t9[11], &f.c_t9[12], &f.c_t9[13]};
        st = io::line_d7(fp, v2);  // only the second line's status counts
    } else if (t == 10) {
        f.model = 7;
        double dropped = 0.0;  // 7th number read into a local and lost
        double* v[7] = {&f.c_t10[0], &f.c_t10[1], &f.c_t10[2], &f.c_t10[3], &f.c_t10[4], &f.c_t10[5], &dropped};
        st = io::line_d7(fp, v);
    } else if (t == 0xb) {
        f.model = 8;
        st = io::line_d5(fp, &f.c_t11[0], &f.c_t11[1], &f.c_t11[2], &f.c_t11[3], &f.c_t11[4]);
    } else if (t == 0xc) {
        f.model = 9;
        double a = 0.0, b = 0.0;
        st = io::line_dd(fp, &a, &b);
        if (st != 0) return 2;
        io::dyn_reinit_keep(&f.f, 0x26);
        set_selector_shape(&f.f, s1, a, b, 8);
        if (io::read_body(fp, &f.f) != 0) return 2;
        st = 0;
    } else if (t == 0xd || t == 0xf) {
        f.model = (t == 0xd) ? 10 : 0xc;
        if (detail::read_nvar_table(fp, &f.cnt, &f.a, 0xa6, &f.b, 0xa7, &f.c, 0xa8) != 0) return 2;
        st = 0;
    } else if (t == 0xe) {
        f.model = 0xb;
        st = io::line_d4(fp, &f.c_t14[0], &f.c_t14[1], &f.c_t14[2], &f.c_t14[3]);
        st = io::read_generic(fp, &f.bmd_n, 0, nullptr);  // only this status counts
    } else if (t == 0x10) {
        f.model = 0xd;
        io::dyn_reinit_keep(&f.p, 0x51);
        st = io::line_i(fp, &f.p.ncomp, false);
        if (st != 0) return 2;
        st = io::line_iii(fp, &f.p.n[0], &f.p.n[1], &f.p.n[2]);
        if (st != 0) return 2;
        st = io::read_body(fp, &f.p);
        f.p.u50 = 0;  // mat+0x638
    } else {
        st = 0;
        if (t < 0) {
            f.model = 0xe;
            f.user_routine = -t;
        } else if (t == 0) {
            f.model = -1;
        }
    }
    return st == 0 ? 0 : 2;
}

// @src 1403d0c90  FSTRES write handler
int write_fstres(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_fstres_mat(fp, number, *io::find_material(md, number));
}

namespace {

// selector of a Y/H/F table for the writer; -1 = no match (handler returns 3)
std::int32_t write_selector(const DYNARY& d, std::int32_t var2)
{
    if (d.ndim == 0) return 0;
    if (d.ndim == 1 && d.var[0] == 3) return 1;
    if (d.ndim == 1 && d.var[0] == var2) return 2;
    if (d.ndim == 2 && d.var[0] == 3 && d.var[1] == var2) return 3;
    return -1;
}

} // namespace

// part of @src 1403d0c90
int write_fstres_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    def::model::Cls_140288ae0& f = mat.fstres;
    const char* const fmt3 = "%-6s %7i %7i\n";
    const char* const e4 = " %20.10E %20.10E %20.10E %20.10E\n";
    const char* const e5 = " %20.10E %20.10E %20.10E %20.10E %20.10E\n";
    const char* const e6 = " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n";
    const char* const e7 = " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n";
    switch (f.model) {
    case 0:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 1);
        kfprintf(fp, e4, f.c_t1[0], f.c_t1[1], f.c_t1[2], f.c_t1[3]);
        break;
    case 1:
        if (f.table.u50 == 1) {
            kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 2);
        } else {
            if (f.table.u50 != 0) return 3;
            kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 3);
        }
        io::write_body(fp, &f.table, 1);
        break;
    case 2:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 4);
        kfprintf(fp, e5, f.c_t4[0], f.c_t4[1], f.c_t4[2], f.c_t4[3], f.c_t4[4]);
        break;
    case 3:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 5);
        kfprintf(fp, e4, f.c_t5[0], f.c_t5[1], f.c_t5[2], f.c_t5[3]);
        break;
    case 4: {
        const std::int32_t sy = write_selector(f.y, 0xc);
        if (sy < 0) return 3;
        const std::int32_t sh = write_selector(f.h, 0xc);
        if (sh < 0) return 3;
        kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(0x5e), number, 6, sy, sh);
        io::write_body(fp, &f.y, 1);
        io::write_body(fp, &f.h, 1);
        break;
    }
    case 5:
        if (f.s.u50 == 1) {
            kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 7);
        } else {
            if (f.s.u50 != 0) return 3;
            kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 8);
        }
        io::write_body(fp, &f.s, 1);
        break;
    case 6:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 9);
        kfprintf(fp, e7, f.c_t9[0], f.c_t9[1], f.c_t9[2], f.c_t9[3], f.c_t9[4], f.c_t9[5], f.c_t9[6]);
        kfprintf(fp, e7, f.c_t9[7], f.c_t9[8], f.c_t9[9], f.c_t9[10], f.c_t9[11], f.c_t9[12], f.c_t9[13]);
        break;
    case 7:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 10);
        kfprintf(fp, e6, f.c_t10[0], f.c_t10[1], f.c_t10[2], f.c_t10[3], f.c_t10[4], f.c_t10[5]);
        break;
    case 8:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 0xb);
        kfprintf(fp, e5, f.c_t11[0], f.c_t11[1], f.c_t11[2], f.c_t11[3], f.c_t11[4]);
        break;
    case 9: {
        const std::int32_t s = write_selector(f.f, 8);
        if (s < 0) return 3;
        // 5th field is a literal 0 (the decompile shows a masked stack slot; checked in the asm)
        kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(0x5e), number, 0xc, s, 0);
        io::write_body(fp, &f.f, 1);
        break;
    }
    case 10:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 0xd);
        detail::write_nvar_table(fp, &f.cnt, f.a, f.b, f.c);
        break;
    case 0xb:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 0xe);
        kfprintf(fp, e4, f.c_t14[0], f.c_t14[1], f.c_t14[2], f.c_t14[3]);
        if (f.bmd_n.ndim == 0) {
            kfprintf(fp, " %7i %20.10E\n", 0, f.bmd_n.value);
        } else {
            kfprintf(fp, " %7i %7i\n", 6, f.bmd_n.n[0]);
            io::write_body(fp, &f.bmd_n, 0);
        }
        break;
    case 0xc:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 0xf);
        detail::write_nvar_table(fp, &f.cnt, f.a, f.b, f.c);
        break;
    case 0xd:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, 0x10);
        kfprintf(fp, " %7i\n", f.p.ncomp);
        io::write_body(fp, &f.p, 1);
        break;
    case 0xe:
        kfprintf(fp, fmt3, def::model::keyword_name(0x5e), number, -f.user_routine);
        break;
    default:
        return 3;
    }
    return 0;
}

// @src 1403d1c50  FSTRES accessor: DYNARY by label for an existing material, else 3
int accessor_fstres(DB_STEP_DATA* md, const char* label, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) {
        def::model::Cls_140288ae0& f = mat->fstres;
        if (std::strcmp(label, "Bird-Mukherjee-Dorn n") == 0) { *out = &f.bmd_n; return 0; }
        if (std::strcmp(label, "Coefficient (Y)") == 0) { *out = &f.y; return 0; }
        if (std::strcmp(label, "Coefficient (H)") == 0) { *out = &f.h; return 0; }
        if (std::strcmp(label, "Initial Flow Stress") == 0) { *out = &f.f; return 0; }
        if (std::strcmp(label, "Flow Stress-f(Stn,Str,Temp)") == 0) { *out = &f.table; return 0; }
        if (std::strcmp(label, "Strain-StrainRate-Temp") == 0) { *out = &f.p; return 0; }
        if (std::strcmp(label, "Strain-Atm-Temp") == 0) { *out = &f.s; return 0; }
    }
    return 3;
}

} // namespace def::key::g4a
