// CREEP (id 106): docs/re/keywords/CREEP.md. Block Material::creep (Cls_140289b80 at
// material+0xac8).
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/detail.hpp"
#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>

#include <cstring>

namespace def::key::g4a {

using def::model::DYNARY;
using io::kfprintf;

// part of @src 140384a50  case 0xe (CREEP)
int read_creep(std::FILE* fp, Material& mat)
{
    def::model::Cls_140289b80& c = mat.creep;
    std::int32_t t = 0, n = 0, x = 0;  // x (4th field) is read and ignored
    int st = io::line_iii(fp, &t, &n, &x);
    if (st != 0) return 2;
    st = 0;
    if (t == 0) {
        c.model = 0;
    } else if (1 <= t && t <= 4) {
        DYNARY* const d[4] = {&c.d1, &c.d2, &c.d3, &c.d4};
        c.model = t;
        io::dyn_reinit_keep(d[t - 1], 0x44 + t);  // 0x45..0x48
        d[t - 1]->n[0] = n;
        st = io::read_body(fp, d[t - 1]);
    } else if (t == 5 || t == 6) {
        DYNARY* const d = (t == 5) ? &c.d5 : &c.d6;
        c.model = t;
        std::int32_t n0 = 0, n1 = 0, n2 = 0;
        st = io::line_iii(fp, &n0, &n1, &n2);
        if (st != 0) return 2;
        io::dyn_reinit_keep(d, (t == 5) ? 0x44 : 0x43);
        d->n[0] = n0;
        d->n[1] = n1;
        d->n[2] = n2;
        st = io::read_body(fp, d);
    } else if (t == 7 || t == 8) {
        c.model = t;
        if (detail::read_nvar_table(fp, &c.cnt, &c.a, 0xa9, &c.b, 0xaa, &c.c, 0xab) != 0) return 2;
        st = 0;
    } else if (t < 0) {
        c.model = 9;
        c.user_routine = -t;
    }
    return st == 0 ? 0 : 2;
}

// @src 1403d22a0  CREEP write handler
int write_creep(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_creep_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d22a0
int write_creep_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    def::model::Cls_140289b80& c = mat.creep;
    switch (c.model) {
    case 0:
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6a), number, 0);
        break;
    case 1:
    case 2:
    case 3:
    case 4: {
        DYNARY* const d[4] = {&c.d1, &c.d2, &c.d3, &c.d4};
        const DYNARY& dm = *d[c.model - 1];
        kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(0x6a), number, c.model, dm.n[0],
                 dm.ncomp + 1);
        io::write_body(fp, &dm, 0);
        break;
    }
    case 5:
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6a), number, 5);
        io::write_body(fp, &c.d5, 1);
        break;
    case 6:
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6a), number, 6);
        io::write_body(fp, &c.d6, 1);
        break;
    case 7:
    case 8:
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6a), number, c.model);
        detail::write_nvar_table(fp, &c.cnt, c.a, c.b, c.c);
        break;
    case 9:
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6a), number, -c.user_routine);
        break;
    default:
        return 3;
    }
    return 0;
}

// @src 1403d2a00  CREEP accessor: DYNARY by label for an existing material, else 3
int accessor_creep(DB_STEP_DATA* md, const char* label, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) {
        def::model::Cls_140289b80& c = mat->creep;
        if (std::strcmp(label, "Creep - Prezyna") == 0) { *out = &c.d1; return 0; }
        if (std::strcmp(label, "Creep - Power Law") == 0) { *out = &c.d2; return 0; }
        if (std::strcmp(label, "Creep - GE") == 0) { *out = &c.d3; return 0; }
        if (std::strcmp(label, "Creep - WG") == 0) { *out = &c.d4; return 0; }
        if (std::strcmp(label, "Creep - TableDataStr") == 0) { *out = &c.d5; return 0; }
        if (std::strcmp(label, "Creep - TableData") == 0) { *out = &c.d6; return 0; }
    }
    return 3;
}

} // namespace def::key::g4a
