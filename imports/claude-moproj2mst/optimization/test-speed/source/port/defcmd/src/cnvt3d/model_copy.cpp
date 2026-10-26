// CNVT3D: copy of the source model's simulation control, materials, phase pairs and inter-object
// data into the new 3D model (1401a91d0 and its simulation-control parts). Packet p9-01.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d.hpp>

#include <cstring>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d {

using namespace detail;
using model::Cls_1401e7b10;

// @src 1401a91d0
void copy_model_data(DB_STEP_DATA* dst, DB_STEP_DATA* src)
{
    sim_ctrl_copy(&dst->sim_ctrl, &src->sim_ctrl, 0);  // 1401f36f0(dst+0x750, src+0x750, 0)
    material_list_copy(&dst->materials, &src->materials);
    phase_pair_copy(&dst->phase_pairs, &src->phase_pairs);
    dst->n_materials = at<std::int32_t>(&dst->materials, 0x10);  // 140199c50: count (+0x10)
    inter_object_copy(&dst->inter_obj, &src->inter_obj);         // 1401fa150 (empty)
}

// @src 14029b6c0
void material_list_copy(void* dst, void* src) { ext().material_list_copy(dst, src); }

// @src 140339dc0
void phase_pair_copy(void* dst, void* src) { ext().phase_pair_copy(dst, src); }

// @src 1401f36f0
void sim_ctrl_copy(Cls_1401e7b10* dst, Cls_1401e7b10* src, char flag)
{
    ext().sim_ctrl_copy_a(dst, src);  // 1401e8040 (p8-08)
    ext().sim_ctrl_copy_b(dst, src);  // 1401e84f0 (p8-08)
    ext().sim_ctrl_copy_c(dst, src);  // 1401ea900 (p8-08)
    sim_ctrl_copy_axes(dst, src, flag);
    sim_ctrl_copy_3d_a(dst, src);
    sim_ctrl_copy_3d_b(dst, src);
    sim_ctrl_copy_3d_c(dst, src);
    at<std::int32_t>(dst, 0xe4) = at<std::int32_t>(src, 0xe4) + 1;
    at<std::int32_t>(dst, 0xf0) = at<std::int32_t>(src, 0xf0) + 1;
}

// @src 1401e91d0
void sim_ctrl_copy_axes(Cls_1401e7b10* d, const Cls_1401e7b10* s, char flag)
{
    at<std::uint64_t>(d, 0x178) = at<std::uint64_t>(s, 0x178);
    if (flag == 0) {
        at<std::uint64_t>(d, 0x180) = at<std::uint64_t>(s, 0x180);
        at<std::uint64_t>(d, 0x188) = at<std::uint64_t>(s, 0x190);
        at<std::uint64_t>(d, 0x190) = at<std::uint64_t>(s, 0x188);
        at<std::uint64_t>(d, 0x198) = at<std::uint64_t>(s, 0x198);
        at<std::uint64_t>(d, 0x1a0) = at<std::uint64_t>(s, 0x1a8);
        at<std::uint64_t>(d, 0x1a8) = at<std::uint64_t>(s, 0x1a0);
        at<std::uint64_t>(d, 0x1b0) = at<std::uint64_t>(s, 0x1b0);
        at<std::uint64_t>(d, 0x1b8) = at<std::uint64_t>(s, 0x1c0);
        at<std::uint64_t>(d, 0x1c0) = at<std::uint64_t>(s, 0x1b8);
        if (at<std::int32_t>(s, 0x290) == 2) at<std::int32_t>(d, 0x290) = 3;
        else if (at<std::int32_t>(s, 0x290) == 3) at<std::int32_t>(d, 0x290) = 2;
        else at<std::int32_t>(d, 0x290) = at<std::int32_t>(s, 0x290);
    } else {
        std::memcpy(add(d, 0x180), add(s, 0x180), 0x18);
        std::memcpy(add(d, 0x198), add(s, 0x198), 0x18);
        std::memcpy(add(d, 0x1b0), add(s, 0x1b0), 0x18);
        at<std::int32_t>(d, 0x290) = at<std::int32_t>(s, 0x290);
    }
    at<std::int32_t>(d, 0x288) = at<std::int32_t>(s, 0x288);
    at<std::int32_t>(d, 0x28c) = at<std::int32_t>(s, 0x28c);
    at<std::uint64_t>(d, 0x298) = at<std::uint64_t>(s, 0x298);
    at<std::uint64_t>(d, 0x170) = at<std::uint64_t>(s, 0x170);
    at<std::uint64_t>(d, 0xbc8) = at<std::uint64_t>(s, 0xbc8);
}

// @src 1401e89c0
void sim_ctrl_copy_3d_a(Cls_1401e7b10* d, const Cls_1401e7b10* s)
{
    if (!rt::dimension_is_3(d->owner)) return;  // 1400015f0(*(d + 0xbc0))
    at<std::int32_t>(d, 0x1c8) = at<std::int32_t>(s, 0x1c8);
    model::linked::set_dsmax(d, at<double>(s, 0x1e0));  // 1401f4390
    at<std::uint64_t>(d, 0x1d0) = at<std::uint64_t>(s, 0x1d0);
    at<std::uint64_t>(d, 0x1d8) = at<std::uint64_t>(s, 0x1d8);
    at<std::uint64_t>(d, 0x268) = at<std::uint64_t>(s, 0x268);
    at<std::uint64_t>(d, 0x270) = at<std::uint64_t>(s, 0x270);
    at<std::uint64_t>(d, 0x278) = at<std::uint64_t>(s, 0x278);
    at<std::uint64_t>(d, 0x280) = at<std::uint64_t>(s, 0x280);
    at<double>(d, 0x260) = 0.0;
    std::memcpy(add(d, 0x2a0), add(s, 0x2a0), 0x10);
    at<double>(d, 0x2c0) = 1.0;  // DAT_140509c90
    model::dynary_copy(reinterpret_cast<model::DYNARY*>(add(d, 0x1e8)),
                       reinterpret_cast<const model::DYNARY*>(add(s, 0x1e8)));  // 14012eff0
}

// @src 1401e9c50  (the remapped values are overwritten with 2 right away, as in the binary)
void sim_ctrl_copy_3d_b(Cls_1401e7b10* d, const Cls_1401e7b10* s)
{
    if (!rt::dimension_is_3(d->owner)) return;
    if (at<std::int32_t>(s, 0x2e4) == 1) at<std::int32_t>(d, 0x2e4) = 0;
    else if (at<std::int32_t>(s, 0x2e4) == 3) at<std::int32_t>(d, 0x2e4) = 1;
    at<std::int32_t>(d, 0x2e4) = 2;
    if (at<std::int32_t>(s, 0x2e0) == 1) at<std::int32_t>(d, 0x2e0) = 1;
    else if (at<std::int32_t>(s, 0x2e0) == 2) at<std::int32_t>(d, 0x2e0) = 2;
    at<std::int32_t>(d, 0x2e0) = 2;
    if (at<std::int32_t>(s, 0x318) == 3) at<std::int32_t>(d, 0x318) = 1;
    else if (at<std::int32_t>(s, 0x318) == 1) at<std::int32_t>(d, 0x318) = 3;
    at<std::int32_t>(d, 0x318) = 2;
    at<double>(d, 0x2d0) = 0.005;  // DAT_14052e870
    at<double>(d, 0x2d8) = 0.05;   // DAT_140524ce0
    at<std::int32_t>(d, 0x310) = at<std::int32_t>(s, 0x310);
    at<std::int32_t>(d, 0x314) = at<std::int32_t>(s, 0x314);
    at<std::int32_t>(d, 0x31c) = at<std::int32_t>(s, 0x31c);
    at<std::int32_t>(d, 0x320) = at<std::int32_t>(s, 0x320);
    at<std::int32_t>(d, 0x324) = at<std::int32_t>(s, 0x324);
}

// @src 1401eb220
void sim_ctrl_copy_3d_c(Cls_1401e7b10* d, const Cls_1401e7b10* s)
{
    if (!rt::dimension_is_3(d->owner)) return;
    at<std::uint64_t>(d, 0x640) = at<std::uint64_t>(s, 0x640);
    at<std::uint64_t>(d, 0x648) = at<std::uint64_t>(s, 0x648);
    at<std::int32_t>(d, 0x650) = at<std::int32_t>(s, 0x650);
    at<std::uint64_t>(d, 0x658) = at<std::uint64_t>(s, 0x658);
    std::memcpy(add(d, 0x660), add(s, 0x660), 0x20);
    for (std::int32_t i = 0; i < 10; i = i + 1)
        std::strcpy(reinterpret_cast<char*>(add(d, 0x688 + static_cast<std::size_t>(i) * 0x51)),
                    reinterpret_cast<const char*>(add(s, 0x688 + static_cast<std::size_t>(i) * 0x51)));  // 140484f90
    at<std::int32_t>(d, 0x9b4) = at<std::int32_t>(s, 0x9b4);
}

} // namespace def::cmd::cnvt3d
