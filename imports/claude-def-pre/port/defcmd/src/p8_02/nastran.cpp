// p8-02: _DEF_NASTRAN, the NASTRAN bulk-data reader of GFREAD (type scan, CTRIA3 / CTETRA /
// CHEXA readers, surface store and mesh store). Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md 4.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

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

// 14047b6c0: strtol(s, NULL, 10)
i32 atol10(const char* s)
{
    return static_cast<i32>(std::strtol(s, nullptr, 10));
}

// 140220810: push_back of an int COW vector (std::vector growth; @src tag in defmodel curve_model.cpp)
void push_int(def::model::CowVector<i32>* v, const i32* x)
{
    v->resize(v->size() + 1, *x);
}

// fields k = 0..n-1 of the 8-column line: line[8k .. 8k+7] copied into a 9-byte NUL-terminated slot,
// whatever the line length (bytes after the line's end are the buffer's earlier contents)
void split_fields(const char* line, char* fields, i32 first, i32 n, i32 src_offset)
{
    for (i32 f = first; f < first + n; f = f + 1) {
        for (i32 c = 0; c < 8; c = c + 1) fields[c + f * 9] = line[c + (f - first + src_offset) * 8];
        fields[f * 9 + 8] = '\0';
    }
}

}  // namespace

// @src 14024f0a0
NasObj* fun_14024f0a0(NasObj* n)
{
    ::new (static_cast<void*>(&n->nodes)) def::model::CowVector<double>();  // 14019a6b0
    ::new (static_cast<void*>(&n->conn)) def::model::CowVector<i32>();      // 140159ad0
    n->mode = 1;
    n->npe = 4;
    n->nn = 0;
    n->ne = 0;
    return n;
}

// @src 14024f110
void fun_14024f110(NasObj* n)
{
    fun_14024f160(n);
    n->conn.~CowVector();   // 14019a720 (~_Timevec) on +0x18
    n->nodes.~CowVector();  // on +0x10
}

// @src 14024f160
void fun_14024f160(NasObj* n)
{
    n->nn = 0;
    n->ne = 0;
    n->nodes.clear();  // 1402568b0
    n->conn.clear();   // 14019a7d0
}

// @src 14024f1b0
i32 fun_14024f1b0(const NasObj* n)
{
    return n->mode;
}

// @src 14024f1d0
bool fun_14024f1d0(NasObj* n, const char* file)
{
    bool r = true;  // local_18: the scan failing still returns 1
    if (fun_140250280(n, file)) {
        if (n->mode == 1) {
            r = fun_14024f270(n, file);
        } else if (n->mode == 2) {
            r = fun_14024f620(n, file);
        } else if (n->mode == 4) {
            r = fun_14024f670(n, file);
        }
    }
    return r;
}

// @src 14024f270
bool fun_14024f270(NasObj* n, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 140533318
    if (f == nullptr) return false;
    char line[256] = {};    // local_198 (stack garbage in the binary before the first line)
    char fields[0x50 + 8];  // acStack_88
    i32 ids[8];             // local_1b8
    double xyz[3];          // auStack_30
    while (std::fgets(line, 0xff, f) != nullptr) {
        if (std::strstr(line, "$") != nullptr) continue;
        if (std::strstr(line, "GRID") == nullptr) {
            if (std::strstr(line, "CTRIA3") != nullptr) {
                split_fields(line, fields, 0, 6, 0);
                for (i32 k = 0; k < 3; k = k + 1) {
                    ids[k] = atol10(fields + (k + 3) * 9);
                    push_int(&n->conn, ids + k);
                }
                push_int(&n->conn, ids);
            }
        } else {
            split_fields(line, fields, 0, 7, 0);
            for (i32 k = 0; k < 3; k = k + 1) {
                xyz[k] = std::atof(fields + (k + 3) * 9);  // 14048754c
                ext::fun_140256800(&n->nodes, xyz + k);
            }
        }
    }
    std::fclose(f);
    n->nn = static_cast<i32>(n->nodes.size() / 3);
    n->ne = static_cast<i32>(n->conn.size() / 4);
    return true;
}

// @src 14024f620
bool fun_14024f620(NasObj* n, const char* file)
{
    bool r = fun_14024fa50(n, file);
    if (!r) r = fun_14024f6a0(n, file);
    return r;
}

// @src 14024f670
bool fun_14024f670(NasObj* n, const char* file)
{
    return fun_14024fe90(n, file);
}

// @src 14024f6a0
bool fun_14024f6a0(NasObj* n, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 140533330
    if (f == nullptr) return false;
    char line[256] = {};
    char fields[0x50 + 8];
    i32 ids[8];
    float xyz[4];
    while (std::fgets(line, 0xff, f) != nullptr) {
        if (std::strstr(line, "$") != nullptr) continue;
        if (std::strstr(line, "GRID") == nullptr) {
            if (std::strstr(line, "CTETRA") != nullptr) {
                split_fields(line, fields, 0, 7, 0);
                for (i32 k = 0; k < 4; k = k + 1) {
                    ids[k] = atol10(fields + (k + 3) * 9);
                    push_int(&n->conn, ids + k);
                }
            }
        } else {
            split_fields(line, fields, 0, 7, 0);
            for (i32 k = 0; k < 3; k = k + 1) {
                xyz[k] = static_cast<float>(std::atof(fields + (k + 3) * 9));
                const double d = static_cast<double>(xyz[k]);  // local_18
                ext::fun_140256800(&n->nodes, &d);
            }
        }
    }
    std::fclose(f);
    n->nn = static_cast<i32>(n->nodes.size() / 3);
    n->ne = static_cast<i32>(n->conn.size() / 4);
    return true;
}

// @src 14024fa50
bool fun_14024fa50(NasObj* n, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 140533348
    if (f == nullptr) return false;
    char line[256] = {};  // local_1e8
    // comma positions: the binary's int[4] (+ the next local) for GRID and int[10] for CTETRA; more
    // commas than that overwrite neighbouring locals (undefined). The port keeps every position.
    i32 gc[256] = {};  // aiStack_70
    i32 tc[256] = {};  // aiStack_48
    i32 ids[256];      // auStack_218
    float xyz[4];      // afStack_90
    while (std::fgets(line, 0xff, f) != nullptr) {
        if (std::strstr(line, "$") != nullptr) continue;
        if (std::strstr(line, "GRID") == nullptr) {
            if (std::strstr(line, "CTETRA") != nullptr) {
                const i32 len = static_cast<i32>(std::strlen(line));
                i32 nc = 0;  // local_50
                for (i32 i = 0; i < len; i = i + 1) {
                    if (line[i] == ',') tc[nc++] = i;
                }
                if (nc < 2) {
                    std::fclose(f);
                    return false;
                }
                n->npe = nc - 2;
                for (i32 k = 0; k < n->npe; k = k + 1) {
                    ids[k] = atol10(line + tc[k + 2] + 1);
                    push_int(&n->conn, ids + k);
                }
            }
        } else {
            const i32 len = static_cast<i32>(std::strlen(line));
            i32 nc = 0;  // local_7c
            for (i32 i = 0; i < len; i = i + 1) {
                if (line[i] == ',') gc[nc++] = i;
            }
            if (nc < 2) {
                std::fclose(f);
                return false;
            }
            const char* v[3] = {line + gc[2] + 1, line + gc[3] + 1, line + gc[4] + 1};  // local_d8
            for (i32 k = 0; k < 3; k = k + 1) {
                xyz[k] = static_cast<float>(std::atof(v[k]));
                const double d = static_cast<double>(xyz[k]);  // local_20
                ext::fun_140256800(&n->nodes, &d);
            }
        }
    }
    std::fclose(f);
    n->nn = static_cast<i32>(n->nodes.size() / 3);
    n->ne = static_cast<i32>(n->conn.size() / static_cast<std::size_t>(static_cast<std::int64_t>(n->npe)));
    return true;
}

// @src 14024fe90
bool fun_14024fe90(NasObj* n, const char* file)
{
    std::FILE* f = std::fopen(file, "rt");  // str 140533360
    if (f == nullptr) return false;
    char line[256] = {};  // local_248
    char next[256] = {};  // local_128 (continuation line)
    char fields[11 * 9 + 8];  // acStack_2b8 (+ local_267)
    i32 ids[8];
    float xyz[4];
    while (std::fgets(line, 0xff, f) != nullptr) {
        if (std::strstr(line, "$") != nullptr) continue;
        if (std::strstr(line, "GRID") == nullptr) {
            if (std::strstr(line, "CHEXA") != nullptr) {
                std::fgets(next, 0xff, f);
                split_fields(line, fields, 0, 9, 0);
                split_fields(next, fields, 9, 2, 1);  // next[8 .. 23]: fields 1, 2 of the continuation
                for (i32 k = 0; k < 8; k = k + 1) {
                    ids[k] = atol10(fields + (k + 3) * 9);
                    push_int(&n->conn, ids + k);
                }
            }
        } else {
            split_fields(line, fields, 0, 7, 0);
            for (i32 k = 0; k < 3; k = k + 1) {
                xyz[k] = static_cast<float>(std::atof(fields + (k + 3) * 9));
                const double d = static_cast<double>(xyz[k]);
                ext::fun_140256800(&n->nodes, &d);
            }
        }
    }
    std::fclose(f);
    n->nn = static_cast<i32>(n->nodes.size() / 3);
    n->ne = static_cast<i32>(n->conn.size() / 8);
    return true;
}

// @src 140250280
bool fun_140250280(NasObj* n, const char* file)
{
    char seps[8];
    std::memcpy(seps, " ,\t\n\r", 6);  // local_38 (DAT_140533378), unused
    (void)seps;
    bool found = false;  // local_40
    TokenReader rdr;     // local_3c
    ext::fun_140411920(&rdr);
    std::FILE* f = std::fopen(file, "rt");  // str 140533380
    if (f == nullptr) {
        def::rt::crt::fprintf(stdout, "*** failed to open file %s  ***\n", file);
        ext::fun_140411940(&rdr);
        return false;
    }
    char tok[208];  // local_118
    do {
        ext::fun_140411ad0(&rdr, f, 2, tok, -5000);
        if (std::strncmp(tok, "CTETRA", 0x50) == 0) {
            n->mode = 2;
            n->npe = 4;
            found = true;
        } else if (std::strncmp(tok, "CTRIA3", 0x50) == 0) {
            n->mode = 1;
            n->npe = 3;
            found = true;
        } else if (std::strncmp(tok, "CHEXA", 0x50) == 0) {
            n->mode = 4;
            n->npe = 8;
            found = true;
        }
    } while (std::strncmp(tok, "DONE", 0x50) != 0);
    std::fclose(f);
    if (!found) {
        def::rt::crt::fprintf(stdout, "*** Supported mesh type not found           ***\n");
        def::rt::crt::fprintf(stdout, "*** Current supported two mesh types are    ***\n");
        def::rt::crt::fprintf(stdout, "*** CTRIA3 --> triangle surface mesh        ***\n");
        def::rt::crt::fprintf(stdout, "*** CHEXA -->  hex solid mesh               ***\n");
    }
    ext::fun_140411940(&rdr);
    return found;
}

// @src 140251be0
bool fun_140251be0(NasObj* n, LLOBJ_DATA* obj)
{
    if (ext::fun_140158d00(obj)) {
        def::rt::crt::fprintf(stdout, "*** _DEF_NASTRAN::setDataToObjectGeo() *** not implemented in 2D\n");
        return false;
    }
    if (n->mode != 1) return false;
    if (n->nn == 0 || n->ne == 0) return false;
    ext::fun_140163340(obj, 1);
    ext::fun_1402009f0(obj, n->nn, n->ne, 0);
    def::model::OBJ_GEO3D* surf = def::model::geo_kw::surface(*obj, 0);  // 140198620
    if (surf == nullptr) return false;
    const i32 nn = n->nn;
    double* dst = def::model::geo_kw::geo3d_coords(surf);  // 140006410
    const double* src = &n->nodes.mutable_at(0);           // 140005e70
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(nn * 3) << 3));
    const i32 ne = n->ne;
    i32* edst = def::model::geo_kw::geo3d_elems(surf);  // 140005510
    const i32* esrc = &n->conn.mutable_at(0);           // 140005100
    std::memcpy(edst, esrc, static_cast<std::size_t>(static_cast<std::int64_t>(ne << 2) << 2));
    def::model::geo_kw::set_surface_flag(*obj, true);  // 1401985e0
    return true;
}

// @src 140251d60
bool fun_140251d60(NasObj* n, LLOBJ_DATA* obj, const i32* comp)
{
    if (n->mode != 2 && n->mode != 4) return false;
    if (n->nn == 0 || n->ne == 0) return false;
    const i32 kind = n->npe == 4 ? 2 : 4;  // local_28
    ext::fun_140202700(obj, n->nn, n->ne, kind, const_cast<i32*>(comp), 0);
    const i32 nn = n->nn;
    const i32 w = comp[0];
    const double* src = &n->nodes.mutable_at(0);  // 140005e70
    std::memcpy(ext::fun_1401808d0(reinterpret_cast<unsigned char*>(obj) + 0x8a0), src,
                static_cast<std::size_t>(static_cast<std::int64_t>(nn * w) << 3));
    const i32 ne = n->ne;
    const i32 npe = n->npe;
    const i32* esrc = &n->conn.mutable_at(0);  // 140005100
    std::memcpy(ext::fun_140183cb0(reinterpret_cast<unsigned char*>(obj) + 0x9e0), esrc,
                static_cast<std::size_t>(static_cast<std::int64_t>(ne * npe) << 2));
    def::model::geo_kw::build_boundary(*obj);  // 1401fc290
    return true;
}

}  // namespace def::cmd::stlnas
