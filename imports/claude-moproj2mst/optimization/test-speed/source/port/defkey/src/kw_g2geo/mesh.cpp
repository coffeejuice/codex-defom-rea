// kw-g2geo family "mesh": RZ, ELMCON, MTLGRP and the nodal vectors DRZ, URZ, FRZ, PRZ, DRMESH
// (parser cases of FUN_14038b8a0), the RZ / ELMCON block writers and the DRZ..PRZ registry write
// handlers. Decompiles: deform-rea-work/out/g2geo/parser.c (cases 0x0f 0x10 0x15 0x16 0x17 0x1d
// 0x1e 0x77) and fam_mesh.c. Specs: RZ.md, ELMCON.md, MTLGRP.md, DRZ.md, URZ.md, FRZ.md, PRZ.md,
// DRMESH.md.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/node_elem_var.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "../kw_g2obj/dispatch.hpp"

namespace def::key::g2geo::mesh {

namespace gk = def::model::geo_kw;
namespace crt = def::rt::crt;

namespace {

// model+0x15f8: the component table (DB_STEP_DATA::u15f8)
std::int32_t* dims(DB_STEP_DATA& m) { return &at<std::int32_t>(m, 0x15f8); }

// part of @src 14038b8a0  case 0x0f (RZ)
int read_rz(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t n = 0;
    if (read_i(f, &n, false) != 0) return 2;  // 140413de0
    at<std::int32_t>(obj, 0x880) = n;
    model::bc::set_node_count(obj, at<std::int32_t>(obj, 0x880), dims(m));  // 1401652b0
    std::int32_t idx = 0;  // node number, read and ignored
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x880); ++i) {
        double* c = at<double*>(obj, 0x8a8);
        int st;
        if (!is_2d(m)) {
            st = read_i_d3(f, &idx, &c[i * 3], &c[i * 3 + 1], &c[i * 3 + 2]);  // 140415880
        } else {
            st = read_i_dd(f, &idx, &c[i * 2], &c[i * 2 + 1]);  // 140415680
        }
        if (st != 0) return 2;
    }
    return 0;
}

// part of @src 14038b8a0  case 0x1d (ELMCON)
int read_elmcon(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t n = 0;
    if (!is_2d(m)) {
        std::int32_t npe = 0;
        if (read_ii(f, &n, &npe) != 0) return 2;  // 140413ee0
        if (npe == 4) {
            gk::set_element_type(obj, 2);  // 140165500
        } else {
            if (npe != 8) return 2;
            gk::set_element_type(obj, 4);
        }
    } else {
        if (read_i(f, &n, false) != 0) return 2;  // 140413de0
    }
    at<std::int32_t>(obj, 0x884) = n;
    model::bc::set_element_count(obj, at<std::int32_t>(obj, 0x884), dims(m));  // 1401653b0
    std::int32_t idx = 0;  // element number, read and ignored
    for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
        std::int32_t* c = at<std::int32_t*>(obj, 0x9e8);
        int st = 0;  // 3D with another type: nothing read, status stays 0
        if (!is_2d(m)) {
            if (at<std::int32_t>(obj, 0x890) == 2) {
                st = read_i5(f, &idx, &c[e * 4], &c[e * 4 + 1], &c[e * 4 + 2], &c[e * 4 + 3], false);
            } else if (at<std::int32_t>(obj, 0x890) == 4) {
                std::int32_t* const out[9] = {&idx,          &c[e * 8],     &c[e * 8 + 1],
                                              &c[e * 8 + 2], &c[e * 8 + 3], &c[e * 8 + 4],
                                              &c[e * 8 + 5], &c[e * 8 + 6], &c[e * 8 + 7]};
                st = read_i9(f, out, false);  // 140414600
            }
        } else {
            st = read_i5(f, &idx, &c[e * 4], &c[e * 4 + 1], &c[e * 4 + 2], &c[e * 4 + 3], false);  // 140414200
        }
        if (st != 0) return 2;
    }
    at<std::int32_t>(obj, 0x87c) = 1;
    if (gk::step_u1b5c(m) == 0) gk::build_boundary(obj);  // 14039aa20, 1401fc290
    return 0;
}

// part of @src 14038b8a0  case 0x1e (MTLGRP)
int read_mtlgrp(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    const int st = read_int_values(f, at<std::int32_t*>(obj, 0x9f8), at<std::int32_t>(obj, 0x884),
                                   at<std::int32_t>(m, 0x16ec));  // 140419d40
    gk::update_material_group(obj);                                // 140202960, also on failure
    return st;
}

}  // namespace

bool handles(int id)
{
    switch (id) {
    case kRZ: case kDRZ: case kURZ: case kFRZ: case kPRZ: case kELMCON: case kMTLGRP: case kDRMESH:
        return true;
    default:
        return false;
    }
}

// part of @src 14038b8a0  cases 0x0f, 0x10, 0x15, 0x16, 0x17, 0x1d, 0x1e, 0x77; the shell maps
// a nonzero status to 2
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    int st = 0;
    const std::int32_t nn = at<std::int32_t>(obj, 0x880);
    switch (id) {
    case kRZ: st = read_rz(m, f, obj); break;
    // 140416c90: DRZ / URZ mode 6, FRZ / PRZ / DRMESH mode 0
    case kDRZ: st = read_node_values(f, at<double*>(obj, 0x8b0), nn, &at<std::int32_t>(m, 0x15fc), 6); break;
    case kURZ: st = read_node_values(f, at<double*>(obj, 0x8b8), nn, &at<std::int32_t>(m, 0x1600), 6); break;
    case kFRZ: st = read_node_values(f, at<double*>(obj, 0x8c0), nn, &at<std::int32_t>(m, 0x1604), 0); break;
    case kPRZ: st = read_node_values(f, at<double*>(obj, 0x8d0), nn, &at<std::int32_t>(m, 0x160c), 0); break;
    case kELMCON: st = read_elmcon(m, f, obj); break;
    case kMTLGRP: st = read_mtlgrp(m, f, obj); break;
    case kDRMESH:
        // 3D only (1400015f0); in 2D nothing is consumed and the status is 0
        if (is_3d(m)) st = read_node_values(f, at<double*>(obj, 0x930), nn, &at<std::int32_t>(m, 0x165c), 0);
        break;
    default: return 2;
    }
    return st == 0 ? 0 : 2;
}

// ---- writers ------------------------------------------------------------------------------------

namespace {

// @src 1402a2e90  RZ, 3 coordinates per node
int write_nodes_3(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (at<std::int32_t>(obj, 0x87c) != 0 && 0 < at<std::int32_t>(obj, 0x880)) {
        crt::fprintf(f, "%-6s %7i %7i\n", kw(0x9f), n, at<std::int32_t>(obj, 0x880));
        for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x880); ++i) {
            const double* c = at<double*>(obj, 0x8a8);
            crt::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", i + 1, c[i * 3], c[i * 3 + 1], c[i * 3 + 2]);
        }
    }
    return 0;
}

// @src 1402a2fc0  RZ, 2 coordinates per node
int write_nodes_2(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (at<std::int32_t>(obj, 0x87c) != 0 && 0 < at<std::int32_t>(obj, 0x880)) {
        crt::fprintf(f, "%-6s %7i %7i\n", kw(0x9f), n, at<std::int32_t>(obj, 0x880));
        for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x880); ++i) {
            const double* c = at<double*>(obj, 0x8a8);
            crt::fprintf(f, " %7i %20.10E %20.10E\n", i + 1, c[i * 2], c[i * 2 + 1]);
        }
    }
    return 0;
}

// @src 1402a32f0  ELMCON with the nodes-per-element column (tetrahedra / bricks)
int write_elements_typed(std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (at<std::int32_t>(obj, 0x87c) == 0 || at<std::int32_t>(obj, 0x884) == 0) return 0;
    if (at<std::int32_t>(obj, 0x890) == 2) {
        crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xad), n, at<std::int32_t>(obj, 0x884), 4);
        for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
            const std::int32_t* c = at<std::int32_t*>(obj, 0x9e8);
            crt::fprintf(f, " %7i %7i %7i %7i %7i\n", e + 1, c[e * 4], c[e * 4 + 1], c[e * 4 + 2], c[e * 4 + 3]);
        }
    } else if (at<std::int32_t>(obj, 0x890) == 4) {
        crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xad), n, at<std::int32_t>(obj, 0x884), 8);
        for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
            const std::int32_t* c = at<std::int32_t*>(obj, 0x9e8);
            crt::fprintf(f, " %7i %7i %7i %7i %7i %7i %7i %7i %7i\n", e + 1, c[e * 8], c[e * 8 + 1],
                    c[e * 8 + 2], c[e * 8 + 3], c[e * 8 + 4], c[e * 8 + 5], c[e * 8 + 6], c[e * 8 + 7]);
        }
    }
    return 0;
}

}  // namespace

// @src 1402a2db0  RZ writer: 3D, or 2D with object dimension 3 -> 1402a2e90; else 1402a2fc0
int write_nodes(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;
    const std::int32_t d = gk::object_dim(obj);  // 140158ce0
    if (!is_2d(m)) return write_nodes_3(f, obj, n);
    if (d == 3) return write_nodes_3(f, obj, n);
    return write_nodes_2(f, obj, n);
}

// @src 1402a30c0  ELMCON writer
int write_elements(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;
    const std::int32_t d = gk::object_dim(obj);  // 140158ce0
    if (at<std::int32_t>(obj, 0x87c) != 0 && at<std::int32_t>(obj, 0x884) != 0) {
        if (!is_2d(m)) return write_elements_typed(f, obj, n);
        if (d == 3) return write_elements_typed(f, obj, n);
        crt::fprintf(f, "%-6s %7i %7i\n", kw(0xad), n, at<std::int32_t>(obj, 0x884));
        for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
            const std::int32_t* c = at<std::int32_t*>(obj, 0x9e8);
            crt::fprintf(f, " %7i %7i %7i %7i %7i\n", e + 1, c[e * 4], c[e * 4 + 1], c[e * 4 + 2], c[e * 4 + 3]);
        }
    }
    return 0;
}

// ---- registry write handlers ------------------------------------------------------------------

bool handles_write(int id) { return id == kDRZ || id == kURZ || id == kFRZ || id == kPRZ; }

// @src 1402b4f80  DRZ: state-variable writer code 1
// @src 1402b5040  URZ: code 2
// @src 1402b5100  FRZ: code 3
// @src 1402b51c0  PRZ: code 5
// Each: object of ref[0] (14017aaf0, not checked), N = obj+0x3f24 when > 0, then
// 1402a4a80(model, f, obj, code, N, 0) (kw-g2sv); its status is ignored and 0 returned.
int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    int code;
    switch (id) {
    case kDRZ: code = 1; break;
    case kURZ: code = 2; break;
    case kFRZ: code = 3; break;
    case kPRZ: code = 5; break;
    default: return 0;
    }
    const Target t = target(m, ref);
    def::key::g2obj::ext::write_state_var(m, f, *t.obj, code, static_cast<std::uint32_t>(t.n), 0);
    return 0;
}

}  // namespace def::key::g2geo::mesh
