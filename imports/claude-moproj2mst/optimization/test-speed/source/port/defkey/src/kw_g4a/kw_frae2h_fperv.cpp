// FRAE2H (id 93) and FPERV (id 104): docs/re/keywords/FRAE2H.md, FPERV.md.
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>

#include <cstring>

namespace def::key::g4a {

using io::kfprintf;

// ---- FRAE2H -----------------------------------------------------------------------------------

// part of @src 140384a50  case 0x1: FUN_140414990(fp, mat+0x1ec8)
int read_frae2h(std::FILE* fp, Material& mat)
{
    const int st = io::line_d(fp, &mat.frae2h);
    return st == 0 ? 0 : 2;
}

// @src 1403cf3f0  FRAE2H write handler
int write_frae2h(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_frae2h_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403cf3f0  after the lookup
int write_frae2h_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    kfprintf(fp, "%-6s %7i %20.10E\n", def::model::keyword_name(0x5d), number, mat.frae2h);
    return 0;
}

// ---- FPERV ------------------------------------------------------------------------------------

// part of @src 140384a50  case 0xc: 4 doubles v1 t1 v2 t2, two DYNARYs (const or 1-D in var 1)
int read_fperv(std::FILE* fp, Material& mat)
{
    double v1 = 0.0, t1 = 0.0, v2 = 0.0, t2 = 0.0;
    int st = io::line_d4(fp, &v1, &t1, &v2, &t2);
    if (st != 0) return 2;
    io::dyn_reinit_keep(&mat.fperv1, 0);
    io::dyn_reinit_keep(&mat.fperv2, 0);
    const std::int32_t k1 = static_cast<std::int32_t>(t1);
    if (k1 == 0) {
        mat.fperv1.ndim = 0;
        mat.fperv1.value = v1;
    } else if (k1 == 1) {
        mat.fperv1.ndim = 1;
        mat.fperv1.var[0] = 1;
        mat.fperv1.n[0] = static_cast<std::int32_t>(v1);
        st = io::read_body(fp, &mat.fperv1);
    }
    const std::int32_t k2 = static_cast<std::int32_t>(t2);
    if (k2 == 0) {
        mat.fperv2.ndim = 0;
        mat.fperv2.value = v2;
    } else if (k2 == 1) {
        mat.fperv2.ndim = 1;
        mat.fperv2.var[0] = 1;
        mat.fperv2.n[0] = static_cast<std::int32_t>(v2);
        st = io::read_body(fp, &mat.fperv2);
    }
    return st == 0 ? 0 : 2;
}

// @src 1403cf830  FPERV write handler
int write_fperv(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_fperv_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403cf830
int write_fperv_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    const def::model::DYNARY& d1 = mat.fperv1;
    const def::model::DYNARY& d2 = mat.fperv2;
    if (d1.ndim == 0 && d2.ndim == 0) {
        kfprintf(fp, "%-6s %7i %20.10E %7i %20.10E %7i\n", def::model::keyword_name(0x68), number, d1.value, 0,
                 d2.value, 0);
    } else if (d1.ndim == 1 && d2.ndim == 0) {
        if (d1.var[0] != 1) return 3;
        kfprintf(fp, "%-6s %7i %7i %7i %20.10E %7i\n", def::model::keyword_name(0x68), number, d1.n[0], 1, d2.value,
                 0);
        io::write_body(fp, &d1, 0);
    } else if (d1.ndim == 0 && d2.ndim == 1) {
        if (d2.var[0] != 1) return 3;
        kfprintf(fp, "%-6s %7i %20.10E %7i %7i %7i\n", def::model::keyword_name(0x68), number, d1.value, 0, d2.n[0],
                 1);
        io::write_body(fp, &d2, 0);
    } else if (d1.ndim == 1 && d2.ndim == 1) {
        if (d1.var[0] != 1) return 3;
        if (d2.var[0] != 1) return 3;
        kfprintf(fp, "%-6s %7i %7i %7i %7i %7i\n", def::model::keyword_name(0x68), number, d1.n[0], 1, d2.n[0], 1);
        io::write_body(fp, &d1, 0);
        io::write_body(fp, &d2, 0);
    }
    return 0;
}

// @src 1403cfbf0  FPERV accessor: "Body Force" -> first table, any other label -> second; always 0
int accessor_fperv(DB_STEP_DATA* md, const char* label, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) {
        if (std::strcmp(label, "Body Force") == 0)
            *out = &mat->fperv1;
        else
            *out = &mat->fperv2;
    }
    return 0;
}

} // namespace def::key::g4a
