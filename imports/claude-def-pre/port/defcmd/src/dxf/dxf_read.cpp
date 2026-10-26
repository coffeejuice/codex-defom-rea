// defcmd/src/dxf/dxf_read.cpp: Uic2CObjectDxfFile and the DXF read entry 1404093a0, the
// conversion 14040ba80 (p8-04). Spec: docs/re/commands/DXF_READER.md sections 2, 9, 12.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstring>

#include <defcmd/dxf.hpp>
#include <defcmd/globals_p8_04.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>

#include "dxf_internal.hpp"
#include "ext.hpp"

namespace def::cmd {

// ctor 1404092b0 / dtor 140409370: bodies are p8-01's (see dxf.hpp). The C++ base ctor runs
// 140454900; the member stores follow; 140409330 (p8-01) repeats them when p8-01 is present.
Uic2CObjectDxfFile::Uic2CObjectDxfFile(const char* name) : Uic2CObject(name), u18(0), u1c(0), u20(0.0)
{
    dxf::ext::f140409330(this);
}
Uic2CObjectDxfFile::~Uic2CObjectDxfFile() = default;

// @src 140409880  slot 1
const char* Uic2CObjectDxfFile::class_name() const { return "Uic2CObjectDxfFile"; }
// @src 140409890  slot 0: scalar deleting dtor (dtor 140409370, then free when flags & 1); the
// C++ compiler generates it from the virtual dtor.

} // namespace def::cmd

namespace def::cmd::dxf {

using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

// @src 1404099a0  dxf+0x20 = v
static void fun_1404099a0(Uic2CObjectDxfFile* d, double v) { d->u20 = v; }
// @src 140409970  dxf+0x1c = v
static void fun_140409970(Uic2CObjectDxfFile* d, std::int32_t v) { d->u1c = v; }
// @src 140409990  dxf+0x20
static double fun_140409990(const Uic2CObjectDxfFile* d) { return d->u20; }
// @src 140409950  (object)+0x20 = v; used on the IGES helper
static void fun_140409950(ext::IgesHelper* h, double v) { std::memcpy(h->raw + 0x20, &v, sizeof v); }

// 140198230: DBREAD's port owns it (defdbio/src/dbread/w1_objects.cpp set_curve_count)
static void fun_140198230(LLOBJ_DATA* obj, std::int32_t n) { def::dbio::dbread::set_curve_count(obj, n); }

// @src 14040ba80  segment list -> curve 0 of obj (through the Fortran conversion 14040c220,
// mode 700 = duplicate removal + chaining). Returns 0 or the conversion's error code; -1 when
// there is nothing to convert or an allocation fails.
static std::int32_t fun_14040ba80(ext::IgesHelper* iges, LLOBJ_DATA* obj, double tol, std::int32_t n_pts,
                                  std::int32_t n_seg, double* data, std::int32_t mode)
{
    std::int32_t rc = 0;
    char name_buf[0x400];  // local_4c8: not initialised in the original (unused in mode 700)
    std::memset(name_buf, 0, sizeof name_buf);
    ConvContext& ctx = conv_ctx();
    std::memset(&ctx, 0, sizeof ctx);  // 140478d30
    if (obj == nullptr) return -1;
    if (conv_rw() != nullptr) {
        std::free(conv_rw());
        conv_rw() = nullptr;
    }
    if (conv_iw() != nullptr) {
        std::free(conv_iw());
        conv_iw() = nullptr;
    }
    conv_rw() = static_cast<double*>(std::malloc(static_cast<std::size_t>(conv_rw_size()) * 8));  // 1404797d8
    if (conv_rw() == nullptr) {
        conv_rw() = nullptr;
        return -1;
    }
    std::memset(conv_rw(), 0, static_cast<std::size_t>(conv_rw_size()) << 3);
    conv_iw() = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(conv_iw_size()) * 4));
    if (conv_iw() == nullptr) {
        std::free(conv_rw());
        conv_rw() = nullptr;
        return -1;
    }
    std::memset(conv_iw(), 0, static_cast<std::size_t>(conv_iw_size()) << 2);
    auto free_arrays = [] {
        std::free(conv_rw());
        conv_rw() = nullptr;
        std::free(conv_iw());
        conv_iw() = nullptr;
    };
    ctx.n_records = n_seg;
    ctx.n_points = n_pts;
    ctx.tol = tol;
    const std::int32_t values = n_seg * def::model::geo2d_dof(2);  // 140276090
    std::int32_t zero = 0;
    OBJ_GEO2D* curve = def::model::geo_kw::curve(*obj, zero);  // 1401982c0
    while (curve == nullptr) {
        ext::f140198270(obj);
        curve = def::model::geo_kw::curve(*obj, 0);
    }
    ctx.table = values < 1 ? nullptr : data;
    if (ctx.table == nullptr) {
        free_arrays();
        return -1;
    }
    rc = ext::f14040c220(name_buf, &ctx, 2, mode);
    if (rc == 200 || rc == 0) {
        curve->n_points = ctx.n_records;
        curve->curve_type = 2;
        (void)(curve->n_points * def::model::geo2d_dof(2));
        curve->free_points();                        // 140276030
        ext::f14040cde0(curve, ctx.table);  // curve+0x18 = data
        if (ctx.table == nullptr) {
            fun_140198230(obj, 0);
            ext::f14019a650(obj, 0);
            ext::f14040ce60(iges, 0);
            free_arrays();
            ctx.table = nullptr;
            return -1;
        }
        if (rc == 200) {
            ext::f14040ce40(iges);
            fun_140198230(obj, 1);
            if (n_seg < 2) {
                ext::f14019a650(obj, 1);
                ext::f14040ce60(iges, 1);
            } else {
                ext::f14019a650(obj, n_seg);
                ext::f14040ce60(iges, n_seg);
            }
        } else {
            fun_140198230(obj, 1);
            ext::f14019a650(obj, 1);
            ext::f14040ce60(iges, 1);
        }
        free_arrays();
        ctx.table = nullptr;
        return 0;
    }
    free_arrays();
    ctx.table = nullptr;
    return rc;
}

// @src 1404093a0  DXF read entry (see dxf.hpp)
int fun_1404093a0(Uic2CObjectDxfFile* dxf, const char* file, const char* prefix, LLOBJ_DATA* obj,
                  std::int32_t kind, std::int32_t mode)
{
    (void)prefix;
    (void)kind;
    std::int32_t rc = 0;
    ext::IgesHelper iges;
    ext::f1404099d0(&iges);  // Uic2CObjectIgesFile ctor (argument 0)
    std::int32_t n_seg = 0;
    std::int32_t n_pts = 0;
    double* data = nullptr;
    bool free_data = false;
    double tol = 0.0;
    if (obj == nullptr) {
        ext::f140409a90(&iges);
        return -1;
    }
    if (mode == 300) {
        if (file == nullptr) {
            ext::f140409a90(&iges);
            return -2;
        }
        rc = static_cast<std::int32_t>(std::strlen(file));
        if (rc < 1) {
            ext::f140409a90(&iges);
            return -2;
        }
        char name[0x1000];
        std::strcpy(name, file);  // 140484f90
        rc = def::dbio::file_exists(name) ? 1 : 0;  // 1401b8560
        if (rc == 0) {
            ext::f140409a90(&iges);
            return -3;
        }
        ext::f140197bc0(obj);
        rc = fun_140422ab0(file, &n_pts, &n_seg, &data);
        if (rc == 0) {
            if (ext::f1404098f0(obj) == 0) {
                tol = fun_1404099c0();
            } else {
                tol = ext::f140409930(obj);
            }
            ext::f1404098d0(obj, tol);
            fun_1404099a0(dxf, tol);
            if (rc == 0) {
                rc = fun_14040ba80(&iges, obj, tol, n_pts, n_seg, data, 700);
                if (rc != 0) free_data = true;
                n_seg = ext::f1402566b0(&iges);
                fun_140409970(dxf, n_seg);
            }
        }
        if (free_data) fun_140422d90(data);
    } else {
        tol = fun_140409990(dxf);
        fun_140409950(&iges, tol);
        rc = ext::f140409af0(iges.obj(), nullptr, nullptr, obj, &tol, 2, static_cast<char>(0), 1, mode);
    }
    ext::f140409a90(&iges);
    return rc;
}

} // namespace def::cmd::dxf
