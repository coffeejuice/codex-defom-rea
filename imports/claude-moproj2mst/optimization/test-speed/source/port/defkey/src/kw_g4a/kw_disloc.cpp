// BURGRS (id 110), ALPHA (111), NDISFM (112), RECVRY (113): docs/re/keywords/<KW>.md. Four
// DYNARYs of Material::disloc (Cls_14028bcc0 at material+0x18f0). None of the readers
// re-initialises its DYNARY: unused var/n fields keep earlier values.
#include <defkey/kw_g4a.hpp>

#include "kw_g4a/io.hpp"

#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/material.hpp>

namespace def::key::g4a {

using def::model::DYNARY;
using io::kfprintf;

// part of @src 140384a50  case 0x12 (BURGRS)
int read_burgrs(std::FILE* fp, Material& mat)
{
    DYNARY& d = mat.disloc.burgrs;
    std::int32_t t = 0;
    double a = 0.0, b = 0.0;
    if (io::line_idd(fp, &t, &a, &b) != 0) return 2;
    switch (t) {
    case 0: d.ndim = 0; d.value = a; break;
    case 1: d.ndim = 1; d.var[0] = 3; d.n[0] = static_cast<std::int32_t>(a); break;
    case 2: d.ndim = 1; d.var[0] = 6; d.n[0] = static_cast<std::int32_t>(a); break;
    case 3: d.ndim = 1; d.var[0] = 0x18; d.n[0] = static_cast<std::int32_t>(a); break;
    case 4:
        d.ndim = 2; d.var[0] = 3; d.var[1] = 6;
        d.n[0] = static_cast<std::int32_t>(a); d.n[1] = static_cast<std::int32_t>(b);
        break;
    case 5:
        d.ndim = 2; d.var[0] = 3; d.var[1] = 0x18;
        d.n[0] = static_cast<std::int32_t>(a); d.n[1] = static_cast<std::int32_t>(b);
        break;
    case 6:
        d.ndim = 2; d.var[0] = 6; d.var[1] = 0x18;
        d.n[0] = static_cast<std::int32_t>(a); d.n[1] = static_cast<std::int32_t>(b);
        break;
    case 7: {
        std::int32_t n0 = 0, n1 = 0, n2 = 0;
        if (io::line_iii(fp, &n0, &n1, &n2) != 0) return 2;
        d.ndim = 3; d.var[0] = 3; d.var[1] = 6; d.var[2] = 0x18;
        d.n[0] = n0; d.n[1] = n1; d.n[2] = n2;
        break;
    }
    default:
        return 2;
    }
    return io::read_body(fp, &d) == 0 ? 0 : 2;
}

namespace {

// ALPHA / NDISFM / RECVRY share the reader of cases 0x13..0x15: t 0 const, 1 var 3, 2 var 8,
// 3 (3, 8); other t -> 2.
int read_t_var3_var8(std::FILE* fp, DYNARY& d)
{
    std::int32_t t = 0;
    double a = 0.0, b = 0.0;
    if (io::line_idd(fp, &t, &a, &b) != 0) return 2;
    if (t == 0) {
        d.ndim = 0;
        d.value = a;
    } else if (t == 1) {
        d.ndim = 1;
        d.var[0] = 3;
        d.n[0] = static_cast<std::int32_t>(a);
    } else if (t == 2) {
        d.ndim = 1;
        d.var[0] = 8;
        d.n[0] = static_cast<std::int32_t>(a);
    } else {
        if (t != 3) return 2;
        d.ndim = 2;
        d.var[0] = 3;
        d.var[1] = 8;
        d.n[0] = static_cast<std::int32_t>(a);
        d.n[1] = static_cast<std::int32_t>(b);
    }
    return io::read_body(fp, &d) == 0 ? 0 : 2;
}

// ALPHA / NDISFM / RECVRY writers (1403d2fb0, 1403d3280, 1403d3550: identical but for the
// keyword id and the DYNARY). Nothing is written for an incomplete table.
int write_t_var3_var8(std::FILE* fp, int id, std::int32_t number, const DYNARY& d)
{
    if (!io::dyn_complete(&d)) return 0;
    std::int32_t t = -1;
    if (d.ndim == 0) {
        t = 0;
        kfprintf(fp, "%-6s %7i %7i %20.10E\n", def::model::keyword_name(id), number, t, d.value);
    } else if (d.ndim == 1) {
        if (d.var[0] == 3)
            t = 1;
        else if (d.var[0] == 8)
            t = 2;
        if (0 < t) {
            kfprintf(fp, "%-6s %7i %7i %7i\n", def::model::keyword_name(id), number, t, d.n[0]);
            io::write_body(fp, &d, 0);
        }
    } else if (d.ndim == 2) {
        if (d.var[0] == 3 && d.var[1] == 8) t = 3;
        if (0 < t) {
            kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(id), number, t, d.n[0], d.n[1]);
            io::write_body(fp, &d, 0);
        }
    }
    if (t < 0) kfprintf(fp, "%-6s %7i %7i %20.10E\n", def::model::keyword_name(id), number, 0, 0.0);
    return 0;
}

} // namespace

// part of @src 140384a50  case 0x13 (ALPHA)
int read_alpha(std::FILE* fp, Material& mat) { return read_t_var3_var8(fp, mat.disloc.alpha); }

// part of @src 140384a50  case 0x14 (NDISFM)
int read_ndisfm(std::FILE* fp, Material& mat) { return read_t_var3_var8(fp, mat.disloc.ndisfm); }

// part of @src 140384a50  case 0x15 (RECVRY)
int read_recvry(std::FILE* fp, Material& mat) { return read_t_var3_var8(fp, mat.disloc.recvry); }

// @src 1403d2c20  BURGRS write handler
int write_burgrs(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_burgrs_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d2c20
int write_burgrs_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    const DYNARY& d = mat.disloc.burgrs;
    if (!io::dyn_complete(&d)) return 0;
    std::int32_t t = -1;
    if (d.ndim == 0) {
        t = 0;
        kfprintf(fp, "%-6s %7i %7i %20.10E\n", def::model::keyword_name(0x6e), number, t, d.value);
    } else if (d.ndim == 1) {
        if (d.var[0] == 3)
            t = 1;
        else if (d.var[0] == 6)
            t = 2;
        else if (d.var[0] == 0x18)
            t = 3;
        if (0 < t) {
            kfprintf(fp, "%-6s %7i %7i %7i\n", def::model::keyword_name(0x6e), number, t, d.n[0]);
            io::write_body(fp, &d, 0);
        }
    } else if (d.ndim == 2) {
        if (d.var[0] == 3 && d.var[1] == 6)
            t = 4;
        else if (d.var[0] == 3 && d.var[1] == 0x18)
            t = 5;
        else if (d.var[0] == 6 && d.var[1] == 0x18)
            t = 6;
        if (0 < t) {
            kfprintf(fp, "%-6s %7i %7i %7i %7i\n", def::model::keyword_name(0x6e), number, t, d.n[0], d.n[1]);
            io::write_body(fp, &d, 0);
        }
    } else if (d.ndim == 3) {
        t = 7;  // variables not checked
        kfprintf(fp, "%-6s %7i %7i\n", def::model::keyword_name(0x6e), number, 7);
        io::write_body(fp, &d, 1);
    }
    if (t < 0) kfprintf(fp, "%-6s %7i %7i %20.10E\n", def::model::keyword_name(0x6e), number, 0, 0.0);
    return 0;
}

// @src 1403d2fb0  ALPHA write handler
int write_alpha(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_alpha_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d2fb0
int write_alpha_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    return write_t_var3_var8(fp, 0x6f, number, mat.disloc.alpha);
}

// @src 1403d3280  NDISFM write handler
int write_ndisfm(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_ndisfm_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d3280
int write_ndisfm_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    return write_t_var3_var8(fp, 0x70, number, mat.disloc.ndisfm);
}

// @src 1403d3550  RECVRY write handler
int write_recvry(std::FILE* fp, DB_STEP_DATA* md, Ids ids)
{
    const std::int32_t number = ids[0];
    return write_recvry_mat(fp, number, *io::find_material(md, number));
}

// part of @src 1403d3550
int write_recvry_mat(std::FILE* fp, std::int32_t number, Material& mat)
{
    return write_t_var3_var8(fp, 0x71, number, mat.disloc.recvry);
}

// @src 1403d2f20  BURGRS accessor
int accessor_burgrs(DB_STEP_DATA* md, const char* /*label*/, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) *out = &mat->disloc.burgrs;
    return 0;
}

// @src 1403d31f0  ALPHA accessor
int accessor_alpha(DB_STEP_DATA* md, const char* /*label*/, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) *out = &mat->disloc.alpha;
    return 0;
}

// @src 1403d34c0  NDISFM accessor
int accessor_ndisfm(DB_STEP_DATA* md, const char* /*label*/, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) *out = &mat->disloc.ndisfm;
    return 0;
}

// @src 1403d3790  RECVRY accessor
int accessor_recvry(DB_STEP_DATA* md, const char* /*label*/, Ids ids, void** out)
{
    Material* mat = io::find_material(md, ids[0]);
    if (mat != nullptr) *out = &mat->disloc.recvry;
    return 0;
}

} // namespace def::key::g4a
