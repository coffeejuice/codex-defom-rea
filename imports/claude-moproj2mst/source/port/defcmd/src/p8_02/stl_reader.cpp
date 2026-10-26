// p8-02: _DEF_STL, the STL reader of GFREAD (detection, ASCII and binary readers, bounds check,
// vertex merge, the DEF_STL.OUT orientation pass, result copy, store into surface 0).
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md section 2.
#define _CRT_SECURE_NO_WARNINGS
#include <bit>
#include <cstdint>
#include <limits>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p67.hpp"
#include "ext_p8_09.hpp"
#include "stl_nas_internal.hpp"

namespace def::cmd::stlnas {

// @src 140244ea0
StlObj* fun_140244ea0(StlObj* s)
{
    s->tolerant = 1;
    s->binary = 0;
    s->nn = 0;
    s->ne = 0;
    s->nodes = nullptr;
    s->cells = nullptr;
    return s;
}

// @src 140244f00
void fun_140244f00(StlObj* s)
{
    fun_140244f20(s);
}

// @src 140244f20
void fun_140244f20(StlObj* s)
{
    s->nn = 0;
    s->ne = 0;
    if (s->nodes != nullptr) std::free(s->nodes);
    s->nodes = nullptr;
    if (s->cells != nullptr) std::free(s->cells);
    s->cells = nullptr;
}

// @src 140244fa0
bool fun_140244fa0(StlObj* s, const char* file, const float* tol, const char* write_check)
{
    fun_140245b90(s, file);  // result ignored
    ext::fun_140410fd0(&s->raw_points, 5000, 10000);
    ext::fun_1404050b0(&s->raw_cells, 10000, 10000, 3);
    if (s->binary == 0) {
        fun_140245cc0(s, file);
    } else {
        fun_140245ee0(s, file);
    }
    fun_1404116a0(&s->raw_points);
    const float* b = fun_140411850(&s->raw_points);
    const float dx = static_cast<float>(std::fabs(static_cast<double>(b[1] - b[0])));  // local_28
    const double dy = std::fabs(static_cast<double>(b[3] - b[2]));
    const double dz = std::fabs(static_cast<double>(b[5] - b[4]));
    // _DAT_14051f5f0 = 1.0E10
    if (1.0E10 < static_cast<double>(dx) || 1.0E10 < static_cast<double>(static_cast<float>(dy)) ||
        1.0E10 < static_cast<double>(static_cast<float>(dz))) {
        const i32 ncells = ext::fun_1404056c0(&s->raw_cells);
        const i32 npts = ext::fun_140411620(&s->raw_points);
        def::rt::crt::fprintf(stdout, "\tRead file with %d points and %d triangles\n", npts, ncells);
        // The binary passes no arguments to this fprintf (listing 0x140245180): the three %g print
        // whatever the argument slots hold at that call. The original prints "1.#QNAN",
        // "2.65249e-314" (the bits 0x140000000, the image base) and "0" on every oracle case
        // (gfread_stl_bigbounds, gfread_stl_bigbounds_noname), so the port passes those values
        // for byte-identical output instead of the real extents.
        def::rt::crt::fprintf(stdout,
                              "\tBounds for data in the file\n\n\tdx : %g\n\tdy : %g\n\tdz : %g\n\n\tLimits greater than max "
                              "tolerance of 1E10. This may result from\n\ta file with bad data or incorrect format. Please "
                              "check the file\n\tor CAD system for output options and try using ASCII STL files\n\tto minimize "
                              "these problems.\n\n",
                              std::numeric_limits<double>::quiet_NaN(),
                              std::bit_cast<double>(std::uint64_t{0x140000000}), 0.0);
        ext::fun_140411080(&s->raw_points);
        ext::fun_140405150(&s->raw_cells);
        return false;
    }
    fun_140411360(&s->raw_points);
    fun_1404053b0(&s->raw_cells);
    PointList merged;  // local_230
    ext::fun_140410fd0(&merged, ext::fun_140411620(&s->raw_points) / 2, 10000);
    CellList out;  // local_258
    ext::fun_1404050b0(&out, ext::fun_1404056c0(&s->raw_cells), 10000, 3);
    Locator loc;  // local_b8
    fun_14040ce80(&loc, *tol);
    fun_14040d270(&loc, &merged, fun_140411850(&s->raw_points), 0);
    for (i32 i = 0; i < ext::fun_1404056c0(&s->raw_cells); i = i + 1) {
        const i32* rec = ext::fun_140405720(&s->raw_cells, i + 1);  // local_238
        i32 ids[4];                                                  // local_200
        if (s->tolerant == 0) {
            for (i32 k = 0; k < 3; k = k + 1) {
                ids[k] = fun_14040dc00(&loc, fun_140411640(&s->raw_points, rec[k]), 0);
            }
        } else {
            for (i32 k = 0; k < 3; k = k + 1) {
                ids[k] = fun_14040d750(&loc, fun_140411640(&s->raw_points, rec[k]), 0);
            }
        }
        if (ids[0] != ids[1] && ids[0] != ids[2] && ids[1] != ids[2]) {
            ext::fun_140405540(&out, 3, ids);
        }
    }
    ext::fun_140411080(&s->raw_points);
    ext::fun_140405150(&s->raw_cells);
    if (*write_check == 1) {
        std::FILE* f = std::fopen("DEF_STL.OUT", "wt");  // str 1405324cc
        if (f == nullptr) return false;                  // the lists and the locator leak
        alignas(8) unsigned char sc_raw[0x130];          // local_1e8
        SurfCheck* sc = reinterpret_cast<SurfCheck*>(sc_raw);
        ext::fun_14040e620(sc, &merged, &out, 1.0E-9f);  // DAT_140534cc0
        fun_14040f4e0(sc, f, nullptr, 1);
        fun_14040e980(sc, f, 1);
        ext::fun_14040e810(sc);
        std::fclose(f);
        ext::fun_1401b85d0("DEF_STL.OUT");
    }
    const i32* cells = ext::fun_140405720(&out, 1);
    const float* pts = fun_140411640(&merged, 1);
    const i32 ne = ext::fun_1404056c0(&out);
    const i32 nn = ext::fun_140411620(&merged);
    fun_1402469a0(s, nn, ne, pts, cells);  // result ignored
    ext::fun_140411080(&merged);
    ext::fun_140405150(&out);
    fun_14040cf80(&loc);
    return true;
}

// @src 140245b90
bool fun_140245b90(StlObj* s, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 1405325fc
    if (f == nullptr) return false;
    // local_118: not cleared in the binary; the port clears it so that strstr stops at byte n
    unsigned char buf[256] = {};
    const i32 n = static_cast<i32>(std::fread(buf, 1, 0xff, f));
    std::fclose(f);
    for (i32 i = 0; i < n; i = i + 1) {
        if (0x7f < buf[i]) {
            s->binary = 1;
            return true;
        }
        buf[i] = static_cast<unsigned char>(std::tolower(buf[i]));
    }
    if (std::strstr(reinterpret_cast<char*>(buf), "facet normal") == nullptr) {
        s->binary = 1;
    } else {
        s->binary = 0;
    }
    return true;
}

// @src 140245cc0
bool fun_140245cc0(StlObj* s, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 14053260c
    if (f == nullptr) {
        def::rt::crt::fprintf(stdout, "\tError opening file %s\n", file);
        return false;
    }
    char line[256];
    float v[3] = {0.0f, 0.0f, 0.0f};  // local_148..local_140 (stale between scans)
    i32 ids[3];                       // local_20, local_1c, local_18
    std::fgets(line, 0xff, f);
    for (;;) {
        if (ext::crt_fscanf(f, "%*s %*s %f %f %f\n", &v[0], &v[1], &v[2]) == -1) break;
        std::fgets(line, 0xff, f);
        ext::crt_fscanf(f, "%*s %f %f %f\n", &v[0], &v[1], &v[2]);
        ids[0] = ext::fun_1404115d0(&s->raw_points, v);
        ext::crt_fscanf(f, "%*s %f %f %f\n", &v[0], &v[1], &v[2]);
        ids[1] = ext::fun_1404115d0(&s->raw_points, v);
        ext::crt_fscanf(f, "%*s %f %f %f\n", &v[0], &v[1], &v[2]);
        ids[2] = ext::fun_1404115d0(&s->raw_points, v);
        std::fgets(line, 0xff, f);
        std::fgets(line, 0xff, f);
        ext::fun_140405540(&s->raw_cells, 3, ids);
    }
    std::fclose(f);
    return true;
}

// @src 140245ee0
bool fun_140245ee0(StlObj* s, const char* file)
{
    std::FILE* f = std::fopen(file, "rb");  // str 1405326a8
    if (f == nullptr) {
        def::rt::crt::fprintf(stdout, "\tError opening file %s\n", file);
        return false;
    }
    unsigned char hdr[96];   // local_78
    unsigned char cnt[12];   // local_84
    float rec[12] = {};      // local_c8..local_9c: normal, v1, v2, v3
    unsigned char attr[4];   // local_88
    i32 ids[3];              // local_98, local_94, local_90
    std::fread(hdr, 1, 0x50, f);
    std::fread(cnt, 1, 4, f);
    fun_140246980(s, cnt);
    for (;;) {
        if (static_cast<i32>(std::fread(rec, 0x30, 1, f)) < 1) break;
        std::fread(attr, 2, 1, f);
        for (i32 k = 0; k < 6; k = k + 1) fun_140246980(s, &rec[k]);
        ids[0] = ext::fun_1404115d0(&s->raw_points, &rec[3]);
        for (i32 k = 6; k < 9; k = k + 1) fun_140246980(s, &rec[k]);
        ids[1] = ext::fun_1404115d0(&s->raw_points, &rec[6]);
        for (i32 k = 9; k < 12; k = k + 1) fun_140246980(s, &rec[k]);
        ids[2] = ext::fun_1404115d0(&s->raw_points, &rec[9]);
        ext::fun_140405540(&s->raw_cells, 3, ids);
    }
    std::fclose(f);
    return true;
}

// @src 140246980  byte-swap hook: empty
void fun_140246980(StlObj*, void*) {}

// @src 1402469a0
bool fun_1402469a0(StlObj* s, i32 nn, i32 ne, const float* pts, const i32* cells)
{
    s->nn = nn;
    s->ne = ne;
    s->nodes = def::model::bc::calloc_doubles(nn * 3);  // 140257150
    if (s->nodes == nullptr) return false;
    for (i32 i = 0; i < nn * 3; i = i + 1) s->nodes[i] = static_cast<double>(pts[i]);
    s->cells = def::model::bc::calloc_ints(ne * 3);  // 140256dc0
    if (s->cells == nullptr) return false;
    for (i32 i = 0; i < ne * 3; i = i + 1) s->cells[i] = cells[i];
    return true;
}

// @src 140246ad0
bool fun_140246ad0(StlObj* s, LLOBJ_DATA* obj)
{
    if (ext::fun_140158d00(obj)) {
        def::rt::crt::fprintf(stdout, "*** _DEF_STL::setDataToObjectGeo() ***  not in 2D\n");
        return false;
    }
    if (s->nodes == nullptr || s->cells == nullptr) return false;
    ext::fun_140163340(obj, 1);
    ext::fun_1402009f0(obj, s->nn, s->ne, 0);
    def::model::OBJ_GEO3D* surf = def::model::geo_kw::surface(*obj, 0);  // 140198620
    if (surf == nullptr) return false;
    const i32 nn = s->nn;
    std::memcpy(def::model::geo_kw::geo3d_coords(surf), s->nodes,
                static_cast<std::size_t>(static_cast<std::int64_t>(nn * 3) << 3));  // 140006410
    ext::ElemVec* el = ext::fun_1401b06e0(surf);
    for (i32 i = 0; i < s->ne; i = i + 1) {
        el->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i << 2))) = s->cells[i * 3];  // 140005100
        el->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i * 4 + 1))) = s->cells[i * 3 + 1];
        el->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i * 4 + 2))) = s->cells[i * 3 + 2];
        el->mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i * 4 + 3))) = s->cells[i * 3];
    }
    def::model::geo_kw::set_surface_flag(*obj, true);  // 1401985e0
    return true;
}

}  // namespace def::cmd::stlnas
