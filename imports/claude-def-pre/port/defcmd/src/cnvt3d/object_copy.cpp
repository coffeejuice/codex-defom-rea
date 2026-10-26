// CNVT3D: 2D -> 3D object property copy with the axis mapping, TRGVOL, WPAXIS records (CNVT3D.md
// section 6.4). Packet p9-01.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d.hpp>
#include <defcmd/p67_5.hpp>

#include <cmath>
#include <cstring>

#include <defcmd/objpos.hpp>
#include <defcmd/vec3.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defrt/ftn/helpers.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d {

using namespace detail;
using model::linked::Cls_140258580;
using model::linked::Cls_140258580List;

// @src 14021d4c0
void copy_object(LLOBJ_DATA* o3, LLOBJ_DATA* o2, const char* windows)
{
    std::strncpy(o3->name, o2->name, 0x40);
    at<std::int32_t>(o3, 0xc) = at<std::int32_t>(o2, 0xc);
    std::byte tmp[0x128];  // copied through a stack buffer (rep movsb twice)
    std::memcpy(tmp, add(o2, 0x2be8), 0x128);
    std::memcpy(add(o3, 0x2be8), tmp, 0x128);
    const std::int32_t axis = wpaxis_axis(reinterpret_cast<Cls_140258580List*>(add(o3, 0x20a0)));  // 14025a360
    movement_copy_axis(add(o3, 0x2160), add(o2, 0x2160), &axis);
    props_copy_axis(add(o3, 0x1ea0), add(o2, 0x1ea0), 0);
    if (at<std::int32_t>(o3, 0x87c) != 0 && at<double>(o2, 0x2090) > 1e-6) update_target_volume(o3);
    if (*windows != 0) meshwin_copy(add(o3, 0x2d10), add(o2, 0x2d10));
}

// @src 14021d630
std::int32_t update_target_volume(LLOBJ_DATA* o)
{
    if (model::p407::obj_kind_is_2(o)) return 0;  // 140158d00
    double vmesh = 0.0;
    if (at<std::int32_t>(o, 0x87c) != 0) vmesh = dbio::objcheck::obj_volume(o, 1);  // 140163540(o, 1)
    double vgeo = 0.0;
    if (objpos::has_geometry(o)) vgeo = dbio::objcheck::obj_volume(o, 0);  // 140160cc0, 140163540(o, 0)
    else vgeo = vmesh;
    double pct;
    if (std::fabs(vgeo) > 1e-6) pct = (100.0 * (vmesh - vgeo)) / vgeo;  // DAT_140524cd8
    else pct = 0.0;
    at<std::int32_t>(o, 0x2098) = 0;
    if (20.0 > pct) at<double>(o, 0x2090) = vgeo;  // COMISD 20, pct; JBE: NaN takes the mesh volume
    else at<double>(o, 0x2090) = vmesh;
    return 1;
}

// @src 14021ef80
void movement_copy_axis(std::byte* d, const std::byte* s, const std::int32_t* axis)
{
    model::llobj_copy::movement_copy(d, s);  // 14018bd30
    if (at<std::uint64_t>(d, 0xa80) == 0) return;
    const double ax = std::fabs(at<double>(s, 0x50));
    const double ay = std::fabs(at<double>(s, 0x58)) - 1.0;
    // COMISD 1e-12, x; JC: taken when 1e-12 < x or x is NaN
    if (1e-12 < ax || std::isnan(ax) || 1e-12 < ay || std::isnan(ay)) {
        at<double>(d, 0x50) = at<double>(s, 0x50);
        at<double>(d, 0x58) = at<double>(s, 0x58);
        at<double>(d, 0x60) = 0.0;
        return;
    }
    if (*axis == 3) {
        at<double>(d, 0x50) = 0.0;
        at<double>(d, 0x58) = 0.0;
        at<double>(d, 0x60) = at<double>(s, 0x58);
    } else if (*axis == 2) {
        at<double>(d, 0x50) = 0.0;
        at<double>(d, 0x58) = at<double>(s, 0x58);
        at<double>(d, 0x60) = 0.0;
    }
    if (*axis == 3) {
        at<double>(d, 0x170) = at<double>(s, 0x170);
        at<double>(d, 0x178) = 0.0;
        at<double>(d, 0x180) = at<double>(s, 0x178);
    } else if (*axis == 2) {
        at<double>(d, 0x170) = at<double>(s, 0x170);
        at<double>(d, 0x178) = at<double>(s, 0x178);
        at<double>(d, 0x180) = 0.0;
    }
}

// @src 14021f160
void props_copy_axis(std::byte* d, const std::byte* s, char flag)
{
    at<std::uint64_t>(d, 0x18) = at<std::uint64_t>(s, 0x18);
    at<std::int32_t>(d, 0x28) = at<std::int32_t>(s, 0x28);
    if (flag == 0) {
        at<std::uint64_t>(d, 0x30) = at<std::uint64_t>(s, 0x30);
        at<std::uint64_t>(d, 0x38) = at<std::uint64_t>(s, 0x40);
        at<std::uint64_t>(d, 0x40) = at<std::uint64_t>(s, 0x38);
    } else {
        std::memcpy(d + 0x30, s + 0x30, 0x18);
    }
}

// @src 14021f760
void meshwin_copy(std::byte* d, const std::byte* s)
{
    const char shapes = 0;
    const char by_dim = 0;
    model::llobj_copy::meshgen_copy(d, s, &shapes, &by_dim);  // 140189b70
    at<std::int32_t>(d, 0x7c) = at<std::int32_t>(s, 0x7c);
    at<std::int32_t>(d, 0x78) = at<std::int32_t>(s, 0x78);
}

// @src 14025a270
Cls_140258580* wpaxis_find_kind4(Cls_140258580List* l, const char* create)
{
    Cls_140258580* r = l->head;
    while (r != nullptr && at<std::int32_t>(r, 4) != 4) r = r->next;
    if (r == nullptr && *create != 0) {
        r = model::geo_kw::wpaxis_add(*l, 0);  // 140259950(l, 0)
        at<std::int32_t>(r, 4) = 4;
    }
    return r;
}

// @src 14025a360
std::int32_t wpaxis_axis(Cls_140258580List* l)
{
    const char create = 0;
    Cls_140258580* r = wpaxis_find_kind4(l, &create);
    if (r == nullptr) return 0;
    return wpaxis_record_axis(r);
}

// @src 140259170  (ucomisd + jp: a NaN product is not 1)
std::int32_t wpaxis_record_axis(const Cls_140258580* r)
{
    const double ex[3] = {1.0, 0.0, 0.0};
    const double ey[3] = {0.0, 1.0, 0.0};
    const double ez[3] = {0.0, 0.0, 1.0};
    const double* v = &at<double>(r, 0x80);
    if (v3::dot(v, ez) == 1.0) return 3;  // 14023b3a0
    if (v3::dot(v, ey) == 1.0) return 2;
    if (v3::dot(v, ex) == 1.0) return 1;
    return 0;
}

// 140258ed0 / 140258f50: owner p67-5 (defcmd/p67_5.hpp, src/p67_5/genaxs.cpp)
void wpaxis_set_08(Cls_140258580* r, const double* v) { wpaxis_set_origin(r, v); }
void wpaxis_set_20(Cls_140258580* r, const double* v) { wpaxis_set_direction(r, v); }
// 140258fd0 / 140259060 / 1402590f0: one body each, owner DBREAD W1 (defdbio/dbread/w1.hpp
// body_force_set_68 / _80 / _98; p9/p10 integration)
void wpaxis_set_68(Cls_140258580* r, const double* v) { def::dbio::dbread::body_force_set_68(r, v); }
void wpaxis_set_80(Cls_140258580* r, const double* v) { def::dbio::dbread::body_force_set_80(r, v); }
void wpaxis_set_98(Cls_140258580* r, const double* v) { def::dbio::dbread::body_force_set_98(r, v); }

} // namespace def::cmd::cnvt3d
