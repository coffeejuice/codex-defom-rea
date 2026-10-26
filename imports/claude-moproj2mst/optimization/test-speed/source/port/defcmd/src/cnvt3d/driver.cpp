// CNVT3D: the dispatcher case, the converter FUN_1401467f0 and the per-object property copy with
// the mesh-window conversion. Packet p9-01. Spec: docs/re/commands/CNVT3D.md sections 2, 5, 6.4,
// 6.5, 7. Floating point and comparisons follow the listing (deform-rea-work/out/w2/p9-01/mine.asm).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cmd_p9_01.hpp>
#include <defcmd/cnvt3d.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defcmd/geolib.hpp>
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "internal.hpp"

namespace def::cmd {

// part of @src 140378e00  case 0x11f (CNVT3D)
int cmd_cnvt3d(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t n = 0;
    if (key::read_i(f, &n, false) != 0) return 2;  // 140413de0(f, &n, 0)
    if (n <= 0) return 0;
    int cnt = 0;
    if (key::read_tokens(f, &cnt, n) != 0) return 2;  // 140413900
    const std::int32_t count = n > cnt ? cnt : n;
    // operator new(count * 4) (14047780c), freed with free (140479908)
    auto* ids = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(count)) * 4));
    for (std::int32_t i = 0; i < count; i = i + 1) ids[i] = key::token_int(i);  // 140413a50, 14047b6c0
    {
        MeshGenBase cvt;                              // 14013e790
        cnvt3d::bind_fixgeo_launch();
        cnvt3d::ext().exchange_cleanup(cvt, true);    // 14013e880(cvt, 1)
        cnvt3d::convert(cvt, m->u1748, m, ids, static_cast<std::uint32_t>(count));
        std::free(ids);
    }                                                 // 14013e840
    return 0;
}

} // namespace def::cmd

namespace def::cmd::cnvt3d {

using namespace detail;
using model::LLWINDEF_DATA;
using model::linked::Cls_140258580;
using model::linked::Cls_140258580List;

namespace {

// 14019ccc0: find or create object `id` in the new model
LLOBJ_DATA* obj3d(MeshGenBase& cvt, std::int32_t id) { return model::llobj_ctor::get_or_create(model3d(cvt), id); }

Cls_140258580List* wpaxis_list(LLOBJ_DATA* o) { return reinterpret_cast<Cls_140258580List*>(add(o, 0x20a0)); }

model::Cls_140269250* winlist(LLOBJ_DATA* o) { return reinterpret_cast<model::Cls_140269250*>(add(o, 0x2d50)); }

} // namespace

// @src 1401467f0
void convert(MeshGenBase& cvt, const char* db_name, DB_STEP_DATA* m, const std::int32_t* ids, std::uint32_t n)
{
    bind_fixgeo_launch();
    if (db_name == nullptr || *db_name == '\0') std::strcpy(cvt.u4360, "DEFORM");  // 140484f90
    else std::strcpy(cvt.u4360, db_name);
    cvt.u3f48 = m;
    if (cvt.u3f48 == nullptr) return;
    db_object(cvt) = m->db;                          // 140159010
    model3d(cvt) = new DB_STEP_DATA(3);              // operator new(0x3408), 1401c8950(p, 3)
    copy_model_data(model3d(cvt), cvt.u3f48);        // 1401a91d0
    model::call_1401d0fc0(model3d(cvt), 0);          // 1401d0fc0(new, 0)
    cnv23_file(0, 0);
    delete_status_file();
    log_begin();
    log_message("Conversion has been started...", cvt.u4360);
    g_obj_count = static_cast<std::int32_t>(n);
    char buf[0x100];
    std::sprintf(buf, "No. of Objects = %d", n);
    log_message(buf, " ");
    for (std::int32_t i = 1; i <= static_cast<std::int32_t>(n); i = i + 1) {
        const std::int32_t id = ids[i - 1];
        std::sprintf(buf, "Object = %d", id);
        log_message(buf, " ");
        g_obj_index = i;
        LLOBJ_DATA* obj = model::object_list_find(&cvt.u3f48->objects, id);  // 1401d6ea0
        if (obj == nullptr) continue;
        const bool c2 = at<std::int32_t>(obj, 0x2c38) == 2;
        if (!geo23_active(&at<std::int32_t>(obj, 0x2be8))) continue;
        double start = 0.0;
        double ang = 0.0;
        std::int32_t kind = 5;
        if (at<std::int32_t>(obj, 0x2be8) == 1) {
            start = at<double>(obj, 0x2bf8);
            ang = at<double>(obj, 0x2bf0);
            set_geo23_c(cvt, at<double>(obj, 0x2c00));
            rt::crt::sprintf(buf, "RevolveStartAngle = %f", start);
            log_message(buf, " ");
            rt::crt::sprintf(buf, "RevolveAngle = %f", ang);
            log_message(buf, " ");
        } else if (at<std::int32_t>(obj, 0x2be8) == 2) {
            start = at<double>(obj, 0x2c18);
            ang = at<double>(obj, 0x2c10);
            cvt.u4770 = 0.0;  // 8 zero bytes
            rt::crt::sprintf(buf, "ExtrudeStartLocation = %f", start);
            log_message(buf, " ");
            rt::crt::sprintf(buf, "ExtrudeLength = %f", ang);
            log_message(buf, " ");
        }
        if (at<std::int32_t>(obj, 0x87c) != 0) {
            if (0 < at<std::int32_t>(obj, 0x2c24)) {
                if (!objpos::has_geometry(obj)) {  // 140160cc0
                    char one = 1;
                    objpos::fun_1401fc730(obj, &one);
                }
                kind = 5;
                std::sprintf(buf, "Generate geometry: ");
                log_message(buf, " ");
                std::sprintf(buf, "NoGeometrySections = %d", at<std::int32_t>(obj, 0x2c24));
                log_message(buf, " ");
                ext().generate(cvt, at<std::int32_t>(obj, 8), at<std::int32_t>(obj, 0x2c24),
                               at<std::int32_t>(obj, 0x2d8c), start, ang, geo23_c(cvt), kind);
                std::sprintf(buf, "Generate geometry - done ");
                log_message(buf, " ");
            }
            std::sprintf(buf, "OutputMesh = %d", static_cast<std::int32_t>(at<std::uint8_t>(obj, 0x2c20)));
            log_message(buf, " ");
            if (at<std::uint8_t>(obj, 0x2c20) != 0 && 0 < at<std::int32_t>(obj, 0x2c28)) {
                if (at<std::int32_t>(obj, 0x2d94) == 0) kind = 2;
                else if (at<std::int32_t>(obj, 0x2d94) == 1) kind = 4;
                std::sprintf(buf, "Generate mesh: ");
                log_message(buf, " ");
                ext().generate(cvt, at<std::int32_t>(obj, 8), at<std::int32_t>(obj, 0x2c28),
                               at<std::int32_t>(obj, 0x2d8c), start, ang, geo23_c(cvt), kind);
                std::sprintf(buf, "Generate mesh - done ");
                log_message(buf, " ");
            }
        }
        if (objpos::has_geometry(obj) && at<std::int32_t>(obj, 0x87c) == 0) {
            kind = 5;
            ext().generate(cvt, at<std::int32_t>(obj, 8), at<std::int32_t>(obj, 0x2c24),
                           at<std::int32_t>(obj, 0x2d8c), start, ang, geo23_c(cvt), 5);
        }
        if (kind == 2 && at<std::int32_t>(obj, 0x2be8) == 2) {
            auto* d = static_cast<double*>(std::malloc(0x18));  // thunk 1404797d8
            d[0] = 0.0;
            d[1] = 0.0;
            d[2] = start;
            const char flag_b = 1;
            const char flag_a = 0;
            objpos::translate(obj3d(cvt, id), d, &flag_a, &flag_b);  // 14021d0b0
            std::free(d);
        }
        if (kind == 2) {
            if (at<std::int32_t>(obj3d(cvt, id), 0x87c) != 0 && ext().has_patch_ids(obj3d(cvt, id))) {
                LLOBJ_DATA* saved = cvt.u008;
                cvt.u008 = obj3d(cvt, id);
                if (at<std::int32_t>(cvt.u008, 0x87c) != 0 && at<std::int32_t>(cvt.u008, 0xbf0) == 0)
                    model::geo_kw::build_boundary(*cvt.u008);  // 1401fc290
                ext().read_nbcd(cvt);                             // 140141580
                cvt.u008 = saved;
            }
        } else if (kind == 4 && at<std::int32_t>(obj3d(cvt, id), 0x87c) != 0) {
            LLOBJ_DATA* saved = cvt.u008;
            cvt.u008 = obj3d(cvt, id);
            at<std::int32_t>(cvt.u008, 0x2d94) = 1;
            if (at<std::int32_t>(cvt.u008, 0x87c) != 0 && at<std::int32_t>(cvt.u008, 0xbf0) == 0) {
                model::geo_kw::build_boundary(*cvt.u008);
                char one = 1;
                objpos::fun_1401fc730(cvt.u008, &one);
            }
            if (at<std::int32_t>(obj, 0x2be8) == 1 && 360.0 > ang) mark_cut_faces(cvt.u008, start, ang);
            ext().read_nbcd(cvt);
            cvt.u008 = saved;
        }
        LLOBJ_DATA* o3 = obj3d(cvt, id);
        Cls_140258580* rec = model::geo_kw::wpaxis_get(*wpaxis_list(o3), 1);  // 140259bc0
        if (at<std::int32_t>(obj, 0x2be8) == 1) {
            at<std::int32_t>(rec, 4) = 0;
            if (ang == 360.0) {  // ucomisd + jp: NaN is not equal
                at<std::int32_t>(rec, 0xb0) = 1;
            } else {
                at<std::int32_t>(rec, 0xb0) = 0;
                at<double>(rec, 0xb8) = ang;
                at<double>(rec, 0xc0) = start;
            }
        } else if (at<std::int32_t>(obj, 0x2be8) == 2) {
            at<std::int32_t>(rec, 4) = 1;
            const double a[3] = {0.0, 0.0, at<double>(obj, 0x2c18)};
            const double b[3] = {0.0, 0.0, at<double>(obj, 0x2c10) + at<double>(obj, 0x2c18)};
            wpaxis_set_08(rec, a);
            wpaxis_set_20(rec, b);
        }
        Cls_140258580* r4;
        if ((c2 && at<std::int32_t>(obj, 0x2be8) == 1) || (!c2 && at<std::int32_t>(obj, 0x2be8) == 2)) {
            auto* origin = static_cast<double*>(std::malloc(0x18));
            auto* dir = static_cast<double*>(std::malloc(0x18));
            origin[0] = 0.0;
            origin[1] = 0.0;
            origin[2] = 0.0;
            dir[0] = 1.0;
            dir[1] = 0.0;
            dir[2] = 0.0;
            if (at<std::int32_t>(obj, 0x2be8) == 2) objpos::rotate(o3, origin, dir, 90.0);  // 14021d2f0
            else objpos::rotate(o3, origin, dir, -90.0);
            std::free(origin);
            std::free(dir);
            const double v68[3] = {1.0, 0.0, 0.0};
            const double v80[3] = {0.0, 1.0, 0.0};
            const double v98[3] = {0.0, 0.0, 0.0};
            const char create = 1;
            r4 = wpaxis_find_kind4(wpaxis_list(o3), &create);
            wpaxis_set_68(r4, v68);
            wpaxis_set_80(r4, v80);
            wpaxis_set_98(r4, v98);
        } else {
            const double v68[3] = {1.0, 0.0, 0.0};
            const double v80[3] = {0.0, 0.0, 1.0};
            const double v98[3] = {0.0, 0.0, 0.0};
            const char create = 1;
            r4 = wpaxis_find_kind4(wpaxis_list(o3), &create);
            wpaxis_set_68(r4, v68);
            wpaxis_set_80(r4, v80);
            wpaxis_set_98(r4, v98);
        }
        if (at<std::int32_t>(obj, 0x2be8) == 1) {
            at<double>(r4, 0xb8) = ang;
            at<double>(r4, 0xc0) = start;
        } else if (at<std::int32_t>(obj, 0x2be8) == 2) {
            at<double>(r4, 0xb8) = 0.0;
            at<double>(r4, 0xc0) = 0.0;
        }
        copy_object_props(cvt, obj);  // 140148750
        LLOBJ_DATA* o3b = obj3d(cvt, at<std::int32_t>(obj, 8));
        if (c2) {
            const std::int32_t axis = 2;
            movement_copy_axis(add(o3b, 0x2160), add(obj, 0x2160), &axis);
        } else {
            const std::int32_t axis = 3;
            movement_copy_axis(add(o3b, 0x2160), add(obj, 0x2160), &axis);
        }
        props_copy_axis(add(o3b, 0x1ea0), add(obj, 0x1ea0), c2 ? 1 : 0);
    }
    log_message("Saving data...", cvt.u4360);
    const std::uint32_t flags = 0;
    // 1401c5810(new, 140159000(m + 0x140), &0); m+0x140 is the ACTOUT name
    const char* out = key_name(static_cast<model::ACTION_DB_STEP*>(cvt.u3f48)->text);
    const bool ok = ext().write_key(model3d(cvt), out, &flags) == 0;
    log_message("Saving data... done", cvt.u4360);
    if (!ok) log_message("Conversion failed", cvt.u4360);
    else log_message("Conversion succeed", cvt.u4360);
    if (model3d(cvt) != nullptr) geolib::fun_140158fc0(model3d(cvt), 1);  // delete the 3D model
    log_message("Conversion is done.", cvt.u4360);
    log_end();
    cnv23_file(1, at<std::int32_t>(cvt.u3f48, 0x834));
}

// @src 140148750
void copy_object_props(MeshGenBase& cvt, LLOBJ_DATA* obj)
{
    if (obj == nullptr) return;
    LLOBJ_DATA* o3 = obj3d(cvt, at<std::int32_t>(obj, 8));
    if (o3 == nullptr) return;
    const bool has_mesh = at<std::int32_t>(obj, 0x87c) != 0;
    const bool kind0 = at<std::int32_t>(obj, 0x2d94) == 0;
    const char windows = (!kind0 && has_mesh) ? 1 : 0;
    copy_object(o3, obj, &windows);  // 14021d4c0
    if (!kind0) convert_mesh_windows(cvt, obj, o3);
}

// @src 140147f90
void convert_mesh_windows(MeshGenBase& cvt, LLOBJ_DATA* obj, LLOBJ_DATA* o3)
{
    if (obj == nullptr || !geo23_active(&at<std::int32_t>(obj, 0x2be8))) return;
    if (o3 == nullptr) {
        o3 = obj3d(cvt, at<std::int32_t>(obj, 8));
        if (o3 == nullptr) return;
    }
    model::geoprim::DEF_GEOPRIM prim;                         // 1402b7ad0
    const std::int32_t n = model::winlist_count(winlist(obj));  // 140189580(obj + 0x2d10)
    DB_STEP_DATA* scratch = new DB_STEP_DATA(3);
    LLOBJ_DATA* so = model::llobj_ctor::get_or_create(scratch, 1);
    for (std::int32_t k = 0; k < n; k = k + 1) {
        LLWINDEF_DATA* w2 = model::winlist_nth(winlist(obj), k + 1);    // 140269900
        LLWINDEF_DATA* w3 = model::winlist_extend(winlist(o3), k + 1);  // 140269bc0
        w3->index = k + 1;
        w3->u174 = 2;
        if (w2 == nullptr || w3 == nullptr) continue;
        if (at<std::int32_t>(obj, 0x2be8) == 1) {
            const double f = at<double>(obj, 0x2bf0) * 0.03;  // DAT_14051f218
            const std::int32_t n_list = 0, ne = 0, nn = 0;
            const double angle = at<double>(obj, 0x2bf0) + 2.0 * f;
            const double st = at<double>(obj, 0x2bf8) - f;
            const double axis_dir[3] = {0.0, 0.0, 1.0};
            const double axis_pt[3] = {0.0, 0.0, 0.0};
            prim.setup_revolve(1, 0, &st, &angle, axis_dir, axis_pt, &at<std::int32_t>(obj, 0x2c28), &w2->npts,
                               w2->pts, &nn, nullptr, &ne, nullptr, &n_list, nullptr);  // 1402b8980
        } else {
            // the original also computes length * 0.03 here and does not use it
            const double d2450 = 0.0, d2448 = 0.0, v2498 = 0.0, v2490 = 0.0, d2388 = 1.0;
            const std::int32_t i23cc = 0, n_list = 0, ne = 0, nn = 0;
            const char b23c9 = 0, b23c8 = 0;
            const double dir[3] = {0.0, 0.0, 1.0};
            prim.setup_extrude(1, 0, &at<double>(obj, 0x2c10), &at<std::int32_t>(obj, 0x2c28), &w2->npts,
                               w2->pts, &nn, nullptr, &ne, nullptr, &n_list, nullptr, &b23c8, &b23c9, &i23cc,
                               &d2388, &v2490, &v2498, &d2448, &d2450, dir);  // 1402b8520
        }
        const char write_key = 0;
        ext().build_primitive(&prim, so, &write_key, scratch);  // 1402ba500
        if (wpaxis_axis(wpaxis_list(obj)) == 2) {
            const double origin[3] = {0.0, 0.0, 0.0};
            const double dir[3] = {1.0, 0.0, 0.0};
            objpos::rotate(so, origin, dir, -90.0);  // 14021d2f0
        }
        w3->u370 = w2->u370;
        window_set_shape3(w3);  // 140266860
        ext().window_geo3d_assign(w3, model::geo_kw::surface(*so, 0));  // w3+0x220 vtable +0x20
    }
    if (scratch != nullptr) geolib::fun_140158fc0(scratch, 1);
}  // 1402b8090

// @src 140266860
void window_set_shape3(LLWINDEF_DATA* w) { w->shape = 3; }

} // namespace def::cmd::cnvt3d
