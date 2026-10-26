// ANISO (id 123): docs/re/keywords/ANISO.md. Block Material::aniso (Cls_140289fe0 at
// material+0x1e40).
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>

#include <cstdlib>

namespace def::key::g4a {

// part of @src 140384a50  case 0x1f (ANISO)
int read_aniso(std::FILE* fp, Material& mat)
{
    def::model::Cls_140289fe0& an = mat.aniso;
    def::model::DYNARY& d = an.d;
    std::int32_t t = 0, nc = 0, tv = 0;
    int st = io::line_iii(fp, &t, &nc, &tv);
    if (st != 0) return 2;
    if (0 <= t && t <= 4) {
        an.t = t;
    } else if (t < 0) {
        an.t = 5;
        an.ref = -t;
        return 0;
    }
    if (t == 1) {
        io::dyn_reinit_keep(&d, 0x68);
        d.ncomp = nc;
    } else if (t == 2) {
        io::dyn_reinit_keep(&d, 0x69);
        d.ncomp = nc;
    } else if (t == 3) {
        io::dyn_reinit_keep(&d, 0x67);
        d.ncomp = nc;
    }
    if (t == 4) {
        io::dyn_reinit_keep(&d, 0x65);
        d.ncomp = nc;
    }
    if (tv == 1) {
        d.ndim = 1;
        d.var[0] = 3;
        st = io::line_i(fp, &d.n[0], false);
    } else if (tv == 2) {
        d.ndim = 1;
        d.var[0] = 7;
        st = io::line_i(fp, &d.n[0], false);
    } else if (tv == 3) {
        d.ndim = 2;
        d.var[0] = 7;
        d.var[1] = 3;
        st = io::line_ii(fp, &d.n[0], &d.n[1]);
    } else if (tv == 4) {
        d.ndim = 2;
        d.var[0] = 3;
        d.var[1] = 0x1e;
        st = io::line_ii(fp, &d.n[0], &d.n[1]);
    }
    if (st != 0) return 2;
    if (io::read_body(fp, &d) != 0) return 2;
    return 0;
}

// @src 1403d0a10  ANISO write handler
int write_aniso(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_aniso_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d0a10
int write_aniso_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    const def::model::Cls_140289fe0& an = mat.aniso;
    const def::model::DYNARY& d = an.d;
    if (an.t == 5) {
        const std::int32_t r = std::abs(an.ref);  // FUN_140484ab8
        io::kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x7b), number, -r);
    } else if (0 < an.t) {
        std::int32_t tv;
        if (d.ndim == 1 && d.var[0] == 3) {
            tv = 1;
        } else if (d.ndim == 1 && d.var[0] == 7) {
            tv = 2;
        } else if (d.ndim == 2 && d.var[0] == 7) {
            tv = 3;
        } else {
            // 1403d0b46 compares var[0] (not var[1]) with 0x1e: what the reader stores for tv 4
            // (var 3, 30) never matches, so such a material aborts the KEY write with 3.
            if (d.ndim != 2 || d.var[0] != 0x1e) return 3;
            tv = 4;
        }
        io::kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(0x7b), number, an.t, d.ncomp, tv);
        io::write_body(fp, &d, 1);
    }
    return 0;
}

// @src 1403d0c00  ANISO accessor: the table for any label (0 even if the material is missing)
int accessor_aniso(DB_STEP_DATA* md, const char* /*label*/, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) *out = &mat->aniso.d;
    return 0;
}

} // namespace def::key::g4a
