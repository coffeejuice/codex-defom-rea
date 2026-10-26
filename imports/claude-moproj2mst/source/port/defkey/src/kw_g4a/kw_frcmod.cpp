// FRCMOD (id 105): docs/re/keywords/FRCMOD.md. Block Material::frcmod (Cls_14028c040 at
// material+0x1ad0; the UTSDAT DYNARY in it belongs to kw-g4b).
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>
#include <defrt/rdata.hpp>

namespace def::key::g4a {

namespace {

// rdata 140524cd8  100.0 (FRCMOD t 12 forces e)
constexpr double kFrcmodT12E = def::rt::rdata::d_140524cd8;

} // namespace

// part of @src 140384a50  case 0xd (FRCMOD)
int read_frcmod(std::FILE* fp, Material& mat)
{
    def::model::Cls_14028c040& f = mat.frcmod;
    std::int32_t t = 0;
    double a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0;  // d is read and dropped
    double* v[5] = {&a, &b, &c, &d, &e};
    int st = io::line_id5(fp, &t, v);
    if (st != 0) return 2;
    f.a = a;
    f.e = e;
    st = 0;
    switch (t) {
    case 0: case 1: case 3: case 6: case 8: case 9: case 10:
        f.model = t;
        break;
    case 2: f.model = 2; f.b_t2 = b; break;
    case 4: f.model = 4; f.b_t4 = b; break;
    case 5: f.model = 5; f.b_t5 = b; break;
    case 7: f.model = 7; f.b_t7 = b; f.c_t7 = c; break;
    case 0xb: {
        f.model = 0xc;
        // file order: +0x1c80 +0x1c88 +0x1ca0 +0x1ca8 +0x1cb0 +0x1cb8 +0x1cc0 +0x1c90 +0x1c98
        double* w[9] = {&f.c_t11[0], &f.c_t11[1], &f.c_t11[4], &f.c_t11[5], &f.c_t11[6],
                        &f.c_t11[7], &f.c_t11[8], &f.c_t11[2], &f.c_t11[3]};
        if (io::line_d9(fp, w) != 0) return 2;
        break;
    }
    case 0xc:
        f.model = 0xd;
        f.e = kFrcmodT12E;
        // the DYNARY is not re-initialised; n0 is stored twice in the original
        f.g.n[0] = static_cast<std::int32_t>(b);
        f.g.ndim = 1;
        f.g.var[0] = 0x1f;
        f.g.n[0] = static_cast<std::int32_t>(b);
        st = io::read_body(fp, &f.g);
        break;
    case 0xf: f.model = 0xf; f.b_t5 = b; break;
    case 0x10: f.model = 0x10; break;
    case 0x11:
        f.model = 0x11;
        f.k.n[0] = static_cast<std::int32_t>(b);
        st = io::read_body(fp, &f.k);
        break;
    default:
        if (t < 0) {
            f.model = 0xb;
            f.user_routine = -t;
        }
        break;
    }
    return st == 0 ? 0 : 2;
}

// @src 14028e140  FRCMOD model -> KEY type and the five header numbers; false if unwritable
bool frcmod_key_values(const Material& mat, std::int32_t* t, double v[5])
{
    const def::model::Cls_14028c040& f = mat.frcmod;
    bool ok = true;
    *t = 0;
    v[0] = f.a;
    v[3] = 0.0;
    v[2] = 0.0;
    v[1] = 0.0;
    v[4] = f.e;
    switch (f.model) {
    case 0: *t = 0; break;
    case 1: *t = 1; break;
    case 2: *t = 2; v[1] = f.b_t2; break;
    case 3: *t = 3; break;
    case 4: *t = 4; v[1] = f.b_t4; break;
    case 5: *t = 5; v[1] = f.b_t5; break;
    case 6: *t = 6; break;
    case 7: *t = 7; v[1] = f.b_t7; v[2] = f.c_t7; break;
    case 8: *t = 8; break;
    case 9: *t = 9; break;
    case 10: *t = 10; break;
    case 0xb: *t = (f.user_routine < 1) ? f.user_routine : -f.user_routine; break;
    case 0xc: *t = 0xb; break;
    case 0xd: *t = 0xc; v[1] = static_cast<double>(f.g.n[0]); break;
    case 0xf: *t = 0xf; v[1] = f.b_t5; break;
    case 0x10: *t = 0x10; break;
    case 0x11: *t = 0x11; v[1] = static_cast<double>(f.k.n[0]); break;
    default: ok = false; break;
    }
    return ok;
}

// @src 1403d3820  FRCMOD write handler
int write_frcmod(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_frcmod_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d3820
int write_frcmod_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    std::int32_t t = 0;
    double v[5];
    if (!frcmod_key_values(mat, &t, v)) return 3;
    const def::model::Cls_14028c040& f = mat.frcmod;
    io::kfprintf(fp, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", def::model::keyword_name(0x69),
                 number, t, v[0], v[1], v[2], v[3], v[4]);
    if (t == 0xb)
        io::kfprintf(fp, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                     f.c_t11[0], f.c_t11[1], f.c_t11[4], f.c_t11[5], f.c_t11[6], f.c_t11[7], f.c_t11[8],
                     f.c_t11[2], f.c_t11[3]);
    if (t == 0xc) io::write_body(fp, &f.g, 0);
    if (t == 0x11) io::write_body(fp, &f.k, 0);
    return 0;
}

} // namespace def::key::g4a
