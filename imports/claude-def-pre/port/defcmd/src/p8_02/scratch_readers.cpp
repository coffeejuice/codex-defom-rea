// p8-02: _DEF_PATRAN (PDA/PAT) and _DEF_UNV: GFREAD runs the external converter (PDA2KEY3 /
// STB2KEY3) and reads its SCRATCH.KEY back (surface DIEGEO, or RZ + ELMCON of a volume mesh).
// The two classes are the same code with other file names. Spec: GFREAD.md section 6.1.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defdbio/step/db_info_read.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p67.hpp"
#include "ext_p8_09.hpp"
#include "stl_nas_internal.hpp"

namespace def::cmd::stlnas {

namespace {

ScratchObj* scratch_ctor(ScratchObj* p)
{
    p->mode = 1;
    p->npe = 4;
    p->nn = 0;
    p->ne = 0;
    p->nodes = nullptr;
    p->conn = nullptr;
    return p;
}

void scratch_free(ScratchObj* p)
{
    p->nn = 0;
    p->ne = 0;
    if (p->nodes != nullptr) std::free(p->nodes);
    p->nodes = nullptr;
    if (p->conn != nullptr) std::free(p->conn);
    p->conn = nullptr;
}

struct ScratchNames {
    const char* ini;      // PDA3.INI / STB3.INI
    const char* program;  // PDA2KEY3 / STB2KEY3
    const char* tmp;      // DEF_PDA3.TMP / DEF_STB3.TMP
    const char* mesh;     // PDA3_MESH.TMP / STB3_MESH.TMP
    i32 second;           // the second "%d" line: 1 (PDA literal) / local_1c = 1 (UNV)
};

bool scratch_read(ScratchObj* p, const char* file);

// the body of 14024c290 / 14024d8b0
bool scratch_run(ScratchObj* p, const char* file, const double* scale, const ScratchNames& nm)
{
    std::FILE* f = std::fopen(nm.ini, "wt");
    if (f == nullptr) return false;
    const i32 one = 1;  // local_1c
    def::rt::crt::fprintf(f, "%s\n", file);
    def::rt::crt::fprintf(f, "%d\n", one);
    def::rt::crt::fprintf(f, "%d\n", nm.second);
    def::rt::crt::fprintf(f, "%15.7f\n", *scale);
    std::fclose(f);
    char cmd[208];  // local_f8
    def::rt::crt::sprintf(cmd, "%s > %s", nm.program, nm.tmp);
    ext::fun_140224130(1, cmd);
    ext::fun_1401b85d0(nm.ini);
    ext::fun_1401b85d0(nm.tmp);
    if (!def::dbio::file_exists("SCRATCH.KEY")) return false;  // 1401b8560
    if (!def::dbio::file_exists(nm.mesh)) {
        p->mode = 1;
    } else {
        std::FILE* m = std::fopen(nm.mesh, "rt");
        if (m == nullptr) {
            p->mode = 1;
        } else {
            ext::crt_fscanf(m, "%d", &p->mode);
            std::fclose(m);
        }
        ext::fun_1401b85d0(nm.mesh);
    }
    const bool r = scratch_read(p, "SCRATCH.KEY");
    ext::fun_1401b85d0("SCRATCH.KEY");
    return r;
}

// the body of 14024c4b0 / 14024dad0
bool scratch_read(ScratchObj* p, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");
    if (f == nullptr) return false;
    bool ok = true;     // local_260
    const i32 n = 500;  // local_288
    char line[512];     // local_218
    bool found = false;  // local_230
    for (;;) {
        if (ext::fun_1401b83d0(line, n, f) == 0) break;
        const char* hit = std::strstr(line, p->mode == 1 ? "DIEGEO" : "RZ");
        if (hit != nullptr && hit == line) {
            found = true;
            break;
        }
    }
    if (!found) return false;  // the file stays open
    if (p->mode == 1) {
        ext::crt_sscanf(line, "%*s %*d %*d %d %d\n", &p->nn, &p->ne);
    } else {
        ext::crt_sscanf(line, "%*s %*d %d\n", &p->nn);
    }
    // DAT_14051f1b0 = 1.0E20
    if (p->nn < 1 || 1.0E20 <= static_cast<double>(p->nn)) {
        ok = false;
    } else {
        if (p->nodes != nullptr) std::free(p->nodes);
        p->nodes = nullptr;
        p->nodes = def::model::bc::calloc_doubles(p->nn * 3);  // 140257150
        float x = 0.0f, y = 0.0f, z = 0.0f;  // local_280, local_27c, local_278
        for (i32 i = 0; i < p->nn; i = i + 1) {
            if (ext::crt_fscanf(f, "%*d %f %f %f\n", &x, &y, &z) == -1) {
                ok = false;
                break;
            }
            p->nodes[i * 3] = static_cast<double>(x);
            p->nodes[i * 3 + 1] = static_cast<double>(y);
            p->nodes[i * 3 + 2] = static_cast<double>(z);
        }
    }
    if (p->mode == 2) {
        found = false;
        for (;;) {
            if (ext::fun_1401b83d0(line, n, f) == 0) break;
            const char* hit = std::strstr(line, "ELMCON");
            if (hit != nullptr && hit == line) {
                found = true;
                break;
            }
        }
        if (!found) return false;  // the file stays open
        ext::crt_sscanf(line, "%*s %*d %d %d\n", &p->ne, &p->npe);
    } else {
        p->npe = 4;
    }
    if (p->ne < 1 || 1.0E20 <= static_cast<double>(p->ne)) {
        ok = false;
    } else {
        if (p->conn != nullptr) std::free(p->conn);
        p->conn = nullptr;
        p->conn = def::model::bc::calloc_ints(p->ne * p->npe);  // 140256dc0
        i32 v[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // local_250..local_234
        for (i32 e = 0; e < p->ne; e = e + 1) {
            if (p->npe == 4) {
                if (ext::crt_fscanf(f, "%*d %d %d %d %d\n", &v[0], &v[1], &v[2], &v[3]) == -1) {
                    ok = false;
                    break;
                }
            } else {
                if (ext::crt_fscanf(f, "%*d %d %d %d %d %d %d %d %d\n", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5],
                                &v[6], &v[7]) == -1) {
                    ok = false;
                    break;
                }
            }
            for (i32 k = 0; k < p->npe; k = k + 1) p->conn[p->npe * e + k] = v[k];
        }
    }
    std::fclose(f);
    return ok;
}

// the body of 14024d4f0 / 14024ee10
bool scratch_store_surface(ScratchObj* p, LLOBJ_DATA* obj, const char* msg2d)
{
    if (ext::fun_140158d00(obj)) {
        def::rt::crt::fprintf(stdout, msg2d);
        return false;
    }
    if (p->mode != 1) return false;
    if (p->nodes == nullptr || p->conn == nullptr) return false;
    ext::fun_140163340(obj, 1);
    ext::fun_1402009f0(obj, p->nn, p->ne, 0);
    def::model::OBJ_GEO3D* surf = def::model::geo_kw::surface(*obj, 0);  // 140198620
    if (surf == nullptr) return false;
    const i32 nn = p->nn;
    double* xyz = def::model::geo_kw::geo3d_coords(surf);  // 140006410
    i32* el = def::model::geo_kw::geo3d_elems(surf);        // 140005510
    std::memcpy(xyz, p->nodes, static_cast<std::size_t>(static_cast<std::int64_t>(nn * 3) << 3));
    std::memcpy(el, p->conn, static_cast<std::size_t>(static_cast<std::int64_t>(p->ne << 2) << 2));
    def::model::geo_kw::set_surface_flag(*obj, true);  // 1401985e0
    return true;
}

// the body of 14024d650 / 14024ef70 (the PDA one copies nn * comp[0] doubles, the UNV one nn * 3)
bool scratch_store_mesh(ScratchObj* p, LLOBJ_DATA* obj, const i32* comp, i32 width)
{
    if (p->mode != 2) return false;
    if (p->nodes == nullptr || p->conn == nullptr) return false;
    const i32 kind = p->npe == 4 ? 2 : 4;  // local_18
    ext::fun_140202700(obj, p->nn, p->ne, kind, const_cast<i32*>(comp), 0);
    std::memcpy(ext::fun_1401808d0(reinterpret_cast<unsigned char*>(obj) + 0x8a0), p->nodes,
                static_cast<std::size_t>(static_cast<std::int64_t>(p->nn * width) << 3));
    std::memcpy(ext::fun_140183cb0(reinterpret_cast<unsigned char*>(obj) + 0x9e0), p->conn,
                static_cast<std::size_t>(static_cast<std::int64_t>(p->ne * p->npe) << 2));
    def::model::geo_kw::build_boundary(*obj);  // 1401fc290
    return true;
}

const ScratchNames kPda{"PDA3.INI", "PDA2KEY3", "DEF_PDA3.TMP", "PDA3_MESH.TMP", 1};
const ScratchNames kUnv{"STB3.INI", "STB2KEY3", "DEF_STB3.TMP", "STB3_MESH.TMP", 1};

}  // namespace

// ---- _DEF_PATRAN -------------------------------------------------------------------------------

// @src 14024c170
ScratchObj* fun_14024c170(ScratchObj* p) { return scratch_ctor(p); }
// @src 14024c1d0
void fun_14024c1d0(ScratchObj* p) { fun_14024c1f0(p); }
// @src 14024c1f0
void fun_14024c1f0(ScratchObj* p) { scratch_free(p); }
// @src 14024c270
i32 fun_14024c270(const ScratchObj* p) { return p->mode; }
// @src 14024c290  (PDA3.INI, "PDA2KEY3 > %s" with DEF_PDA3.TMP, PDA3_MESH.TMP)
bool fun_14024c290(ScratchObj* p, const char* file, const double* scale) { return scratch_run(p, file, scale, kPda); }
// @src 14024c4b0
bool fun_14024c4b0(ScratchObj* p, const char* file) { return scratch_read(p, file); }
// @src 14024d4f0
bool fun_14024d4f0(ScratchObj* p, LLOBJ_DATA* obj)
{
    return scratch_store_surface(p, obj, "*** _DEF_PATRAN::setDataToObjectGeo() *** not implemented in 2D\n");
}
// @src 14024d650
bool fun_14024d650(ScratchObj* p, LLOBJ_DATA* obj, const i32* comp) { return scratch_store_mesh(p, obj, comp, comp[0]); }

// ---- _DEF_UNV ----------------------------------------------------------------------------------

// @src 14024d790
ScratchObj* fun_14024d790(ScratchObj* p) { return scratch_ctor(p); }
// @src 14024d7f0
void fun_14024d7f0(ScratchObj* p) { fun_14024d810(p); }
// @src 14024d810
void fun_14024d810(ScratchObj* p) { scratch_free(p); }
// @src 14024d890
i32 fun_14024d890(const ScratchObj* p) { return p->mode; }
// @src 14024d8b0  (STB3.INI, "STB2KEY3 > %s" with DEF_STB3.TMP, STB3_MESH.TMP)
bool fun_14024d8b0(ScratchObj* p, const char* file, const double* scale) { return scratch_run(p, file, scale, kUnv); }
// @src 14024dad0
bool fun_14024dad0(ScratchObj* p, const char* file) { return scratch_read(p, file); }
// @src 14024ee10
bool fun_14024ee10(ScratchObj* p, LLOBJ_DATA* obj)
{
    return scratch_store_surface(p, obj, "*** _DEF_UNV::setDataToObjectGeo() *** not implemented in 2D\n");
}
// @src 14024ef70
bool fun_14024ef70(ScratchObj* p, LLOBJ_DATA* obj, const i32* comp) { return scratch_store_mesh(p, obj, comp, 3); }

}  // namespace def::cmd::stlnas
