// LLOBJ_DATA sub-object defaults (defmodel/llobj_ctor/obj_defaults.hpp). Decompiles:
// deform-rea-work/out/g2obj/ctor_closure.c and p2_rest.c (closure of 14015ec80).
#include <defmodel/llobj_ctor/obj_defaults.hpp>

#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/step/db_step.hpp>
#include <new>

#include <defrt/ftn/units.hpp>

namespace def::model::llobj_ctor {

namespace {

template <class T>
T& at(std::byte* p, std::size_t off)
{
    return *reinterpret_cast<T*>(p + off);
}
template <class T>
const T& at(const std::byte* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(p + off);
}
DYNARY* tab(std::byte* p, std::size_t off) { return reinterpret_cast<DYNARY*>(p + off); }

// FUN_1400015e0 / FUN_1400015f0 on an owner pointer (null reads as neither)
bool is_2d(const DB_STEP_DATA* m) { return m != nullptr && owner_dimension(m) == 2; }
bool is_3d(const DB_STEP_DATA* m) { return m != nullptr && owner_dimension(m) == 3; }

// rdata 14051f258  0.01   rdata 140509c90  1.0   @data 140522f60  1.0E6
// rdata 14051f1f0  20.0   rdata 14051f1f8  68.0  rdata 140522fc8  -1.0
// @data 140522fc0  0.25   @data 14051f240  0.5   rdata 140509fb8  2.0
// rdata 14051f1c8  3.0    @data 140522fb8  -0.7  @data 140522fb0  32.0
// rdata 14051f1e8  0.001  rdata 140522f48  5.0   @data 140522fa8  210.0
// rdata 14051bee8  10.0   rdata 14051f228  180.0
constexpr double k0_01 = 0.01, k1 = 1.0, k1e6 = 1000000.0, k20 = 20.0, k68 = 68.0, kM1 = -1.0,
                 k0_25 = 0.25, k0_5 = 0.5, k2 = 2.0, k3 = 3.0, kM0_7 = -0.7, k32 = 32.0,
                 k0_001 = 0.001, k5 = 5.0, k210 = 210.0, k10 = 10.0, k180 = 180.0;

} // namespace

std::int32_t owner_dimension(const DB_STEP_DATA* m)
{
    // FUN_140158e90 (p1-04): model+0x1c
    return *reinterpret_cast<const std::int32_t*>(reinterpret_cast<const std::byte*>(m) + 0x1c);
}

// FUN_140235c10 (p1-12, defrt/ftn/units.hpp)
std::int32_t unit_system() { return def::rt::ftn::unit_system(); }

// @src 1401c8f40
std::int32_t model_unit(const DB_STEP_DATA* m)
{
    // FUN_1401f4090(model+0x750): *(int*)(simctrl + 0xfc), the UNIT keyword
    return *reinterpret_cast<const std::int32_t*>(reinterpret_cast<const std::byte*>(m) + 0x750 + 0xfc);
}

// @src 14015f9a0
void object_unit_defaults(std::byte* obj, std::int32_t unit)
{
    if (unit == 0 && at<DB_STEP_DATA*>(obj, 15000) != nullptr) unit = model_unit(at<DB_STEP_DATA*>(obj, 15000));
    if (unit == 1) {
        at<double>(obj, 0x1eb8) = k20;
        at<double>(obj, 0x1ec0) = k1;
    } else {
        at<double>(obj, 0x1eb8) = k68;
        at<double>(obj, 0x1ec0) = k1;
    }
}

// @src 140165500
void set_element_kind(std::byte* obj, std::int32_t kind)
{
    at<std::int32_t>(obj, 0x890) = kind;
    if (at<std::int32_t>(obj, 0x890) == 2) {
        at<std::int32_t>(obj, 0x894) = 4;
        at<std::int32_t>(obj, 0x898) = 4;
        at<std::int32_t>(obj, 0x89c) = 3;
    } else if (at<std::int32_t>(obj, 0x890) == 4) {
        at<std::int32_t>(obj, 0x894) = 8;
        at<std::int32_t>(obj, 0x898) = 6;
        at<std::int32_t>(obj, 0x89c) = 4;
    }
}

// @src 140199a00
void reset_u39d4(std::byte* obj)
{
    at<std::int32_t>(obj, 0x39d4) = 0;
    at<std::uint64_t>(obj, 0x39d8) = 0;
    at<std::int32_t>(obj, 0x39e0) = 0;
    at<std::int32_t>(obj, 0x39e4) = 0;
    at<std::int32_t>(obj, 0x39e8) = 0;
    at<std::int32_t>(obj, 0x39ec) = 0;
    at<std::int32_t>(obj, 0x39f0) = 1;
}

// @src 140199af0
void set_u39c0(std::byte* obj, std::int32_t v) { at<std::int32_t>(obj, 0x39c0) = v; }

// @src 140186200
void props_defaults(std::byte* p)
{
    const DB_STEP_DATA* owner = at<DB_STEP_DATA*>(p, 0x2b8);
    at<double>(p, 0x00) = k0_01;  // LMTSTR
    at<double>(p, 0x08) = k1;     // AVGSTR
    at<double>(p, 0x10) = k1e6;   // PENVOL
    at<double>(p, 0x18) = unit_system() == 1 ? k20 : k68;  // REFTMP
    at<std::int32_t>(p, 0x28) = 0;
    at<std::int32_t>(p, 0x48) = 0;
    for (int i = 0; i < 3; ++i) at<std::uint64_t>(p, 0x30 + 8 * i) = 0;
    at<std::int32_t>(p, 0x4c) = 0;
    at<std::int32_t>(p, 0x50) = 0;
    for (int i = 0; i < 2; ++i) at<std::uint64_t>(p, 0x58 + 8 * i) = 0;
    at<std::int32_t>(p, 0x68) = 0;
    at<std::uint64_t>(p, 0x70) = 0;
    at<std::int32_t>(p, 0x78) = 2;  // ELPSOL
    if (is_2d(owner)) {
        at<std::int32_t>(p, 0x80) = 1;
        at<std::uint64_t>(p, 0x88) = 0;
        at<std::int32_t>(p, 0x90) = 0;
    }
    at<std::int32_t>(p, 0x7c) = 0;
    at<std::int32_t>(p, 0x94) = 0;
    at<std::int32_t>(p, 0x98) = 0;
    at<std::int32_t>(p, 0x9c) = 0;
    at<std::int32_t>(p, 0xa0) = 0;
    at<std::int32_t>(p, 0xa4) = 0;
    at<std::int32_t>(p, 0xa8) = 0;
    at<std::uint64_t>(p, 0xb0) = 0;
    at<std::uint64_t>(p, 0xb8) = 0;
    dynary_init(tab(p, 0xc0), 0);  // CSFREQ
    at<std::int32_t>(p, 0x138) = 0;
    dynary_init(tab(p, 0x140), 0);  // VOLCRG
    if (is_3d(owner)) {
        at<std::uint64_t>(p, 0x1b8) = 0;
        for (int i = 0; i < 3; ++i) {
            at<std::uint64_t>(p, 0x1c0 + 8 * i) = 0;
            at<std::uint64_t>(p, 0x1d8 + 8 * i) = 0;
        }
    }
    at<std::uint64_t>(p, 0x1f0) = 0;
    at<std::int32_t>(p, 0x1f8) = -1;  // TRGVOL mode
    dynary_init(tab(p, 0x238), 0xb5);  // RSEDEF
    at<std::int32_t>(p, 0x2b0) = 0;
    at<std::int32_t>(p, 0x2b4) = 0;
}

// @src 1401861c0
void props_set_owner(std::byte* p, DB_STEP_DATA* m)
{
    at<DB_STEP_DATA*>(p, 0x2b8) = m;
    props_defaults(p);
}

// @src 14018b500
void movement_defaults(std::byte* p)
{
    const DB_STEP_DATA* owner = at<DB_STEP_DATA*>(p, 0xa80);
    at<std::uint8_t>(p, 0) = 0;
    for (int i = 0; i < 3; ++i) {
        if (i == owner_dimension(owner) - 1) {
            at<double>(p, 0x50 + 8 * i) = kM1;
        } else {
            at<std::uint64_t>(p, 0x50 + 8 * i) = 0;
        }
        at<std::uint64_t>(p, 0x170 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x188 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x1a0 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x630 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x648 + 8 * i) = 0;
    }
    if (is_2d(owner)) {
        at<double>(p, 0x50) = k1;
        at<std::uint64_t>(p, 0x58) = 0;
    }
    at<std::int32_t>(p, 0x68) = 1;
    at<std::uint64_t>(p, 0x1e8) = 0;
    at<std::uint64_t>(p, 0x1f0) = 0;
    at<double>(p, 0x1f8) = k1;
    at<std::int32_t>(p, 0x200) = 0;
    dynary_init(tab(p, 0x208), 0);
    at<std::uint64_t>(p, 0x280) = 0;
    at<double>(p, 0x288) = k1;
    at<std::uint64_t>(p, 0x290) = 0;
    at<std::uint64_t>(p, 0x298) = 0;
    dynary_init(tab(p, 0x2a0), 0);
    at<std::uint64_t>(p, 0x318) = 0;
    at<std::uint64_t>(p, 0x320) = 0;
    at<std::uint64_t>(p, 0x328) = 0;
    at<std::uint8_t>(p, 0x330) = 0;
    dynary_init(tab(p, 0x338), 0);
    dynary_init(tab(p, 0x3b0), 0x6f);
    at<std::uint64_t>(p, 0x428) = 0;
    at<std::uint64_t>(p, 0x430) = 0;
    at<std::uint64_t>(p, 0x438) = 0;
    at<std::int32_t>(p, 0x440) = 0;
    at<std::int32_t>(p, 0x444) = 0;
    if (!is_2d(owner)) {
        dynary_init(tab(p, 0x448), 0x7a);
    } else {
        dynary_init(tab(p, 0x448), 0x79);
    }
    at<std::int32_t>(p, 0x4c0) = 1;
    at<std::int32_t>(p, 0x4c4) = 0;
    for (int i = 0; i < 3; ++i) {
        at<std::uint64_t>(p, 0x4c8 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x4e0 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x4f8 + 8 * i) = 0;
    }
    at<std::int32_t>(p, 0x510) = 0;
    at<std::int32_t>(p, 0x514) = 0;
    at<std::int32_t>(p, 0x620) = 0;
    at<std::int32_t>(p, 0x624) = 0;
    dynary_init(tab(p, 0xe8), 0);
    dynary_init(tab(p, 0x70), 0);
    dynary_init(tab(p, 0x590), 0);
    dynary_init(tab(p, 0x518), 0);
    at<std::uint64_t>(p, 0x618) = 0;
    at<std::uint64_t>(p, 0x610) = 0;
    at<std::uint64_t>(p, 0x608) = 0;
    at<std::int32_t>(p, 0x160) = 0;
    at<std::uint64_t>(p, 0x168) = 0;
    at<std::int32_t>(p, 0x628) = 1;
    for (int i = 0; i < 3; ++i) {
        at<std::uint64_t>(p, 0x630 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x648 + 8 * i) = 0;
    }
    dynary_init(tab(p, 0x660), 0);
    dynary_init(tab(p, 0x6d8), 0);
    at<std::uint64_t>(p, 0x750) = 0;
    at<std::uint8_t>(p, 0x8b8) = 0;
    dynary_init(tab(p, 0x8c0), 10);
    at<std::uint64_t>(p, 0x940) = 0;
    at<std::uint64_t>(p, 0x938) = 0;
    at<std::uint64_t>(p, 0x958) = 0;
    at<std::uint64_t>(p, 0x950) = 0;
    at<std::uint64_t>(p, 0x948) = 0;
    for (int i = 0; i < 2; ++i) {
        at<std::uint64_t>(p, 0x1b8 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x1c8 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x1d8 + 8 * i) = 0;
    }
    at<std::int32_t>(p, 0x960) = 1;
    dynary_init(tab(p, 0x968), 0);
    dynary_init(tab(p, 0x9e0), 0);
    at<std::uint64_t>(p, 0xa58) = 0;
    at<std::uint64_t>(p, 0xa60) = 0;
    at<std::uint64_t>(p, 0xa68) = 0;
    at<std::uint64_t>(p, 0xa70) = 0;
    at<std::int32_t>(p, 0x758) = 1;
    for (int i = 0; i < 3; ++i) {
        at<std::uint64_t>(p, 0x760 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x778 + 8 * i) = 0;
    }
    dynary_init(tab(p, 0x790), 0);
    dynary_init(tab(p, 0x808), 0);
    at<std::uint64_t>(p, 0x880) = 0;
    for (int i = 0; i < 3; ++i) {
        at<std::uint64_t>(p, 0x888 + 8 * i) = 0;
        at<std::uint64_t>(p, 0x8a0 + 8 * i) = 0;
    }
    at<std::uint8_t>(p, 0xa78) = 1;
}

// @src 14018b370
void movement_set_owner(std::byte* p, DB_STEP_DATA* m)
{
    at<DB_STEP_DATA*>(p, 0xa80) = m;
    movement_defaults(p);
}

// @src 140189790
void meshgen_defaults(std::byte* p, bool with_windows)
{
    DB_STEP_DATA* owner = at<DB_STEP_DATA*>(p, 0);
    at<std::int32_t>(p, 0x08) = 0;
    at<std::uint64_t>(p, 0x18) = 0;
    at<double>(p, 0x20) = k0_25;
    at<double>(p, 0x28) = k0_25;
    at<std::uint64_t>(p, 0x30) = 0;
    at<double>(p, 0x38) = k0_5;
    // GUI twin 0x14085d9b0 keeps .25/.25/.5 in both dimensions;
    // DEF_PRE 0x140189790 replaces these three weights with 1 in 2D.
    if (!g_gui_engine_variant && is_2d(owner)) {
        at<double>(p, 0x20) = k1;
        at<double>(p, 0x28) = k1;
        at<double>(p, 0x38) = k1;
    }
    if (with_windows) {
        shape_set_owner(p + 0x40, owner);
        shape_reset(p + 0x40);
        shape_set_owner(p + 0x128, owner);
        shape_reset(p + 0x40);  // the binary resets +0x40 a second time, not +0x128
    }
    shape_set_owner(p + 0xf8, owner);
    shape_reset(p + 0xf8);
    at<std::int32_t>(p, 0x70) = 1000;
    if (!is_2d(owner)) {
        at<double>(p, 0x10) = k2;
    } else {
        at<double>(p, 0x10) = k3;
    }
    at<std::int32_t>(p, 0x78) = 8000;
    at<std::int32_t>(p, 0x7c) = 32000;
    if (!is_2d(owner)) {
        at<std::int32_t>(p, 0x80) = 100;
    } else {
        at<std::int32_t>(p, 0x80) = 1000;
    }
    at<std::int32_t>(p, 0x84) = 0;
    at<double>(p, 0x98) = k1;
    at<double>(p, 0x88) = k1;
    at<std::int32_t>(p, 0x90) = 1;
    if (!is_2d(owner)) {
        at<double>(p, 0xb8) = kM0_7;  // RMDPTH
    } else {
        at<std::uint64_t>(p, 0xb8) = 0;
    }
    at<std::uint64_t>(p, 0xc0) = 0;  // RMSTRK
    at<std::uint64_t>(p, 0xc8) = 0;  // RMSTEP
    at<std::uint64_t>(p, 0xd0) = 0;  // RMTIME
    at<double>(p, 0xa0) = k0_01;
    at<double>(p, 0xa8) = k32;
    at<std::int32_t>(p, 0xb0) = 0x19;
    at<std::int32_t>(p, 0xb4) = 0x19;
    at<std::int32_t>(p, 0x74) = 4;
    at<std::int32_t>(p, 0xd8) = 0;
    at<std::uint64_t>(p, 0xe0) = 0;
    at<std::uint64_t>(p, 0xe8) = 0;
    at<std::int32_t>(p, 0xf0) = 0;
    at<std::uint8_t>(p, 0xf5) = 1;
    at<std::uint8_t>(p, 0xf4) = 1;
}

// @src 1401894d0
void meshgen_set_owner(std::byte* p, DB_STEP_DATA* m)
{
    at<DB_STEP_DATA*>(p, 0) = m;
    meshgen_defaults(p, true);
    shape_set_owner(p + 0x40, at<DB_STEP_DATA*>(p, 0));
}

// @src 1401889f0
void geo23_defaults(std::byte* p)
{
    at<std::int32_t>(p, 0x00) = 2;
    at<std::int32_t>(p, 0x04) = 0;
    at<std::uint64_t>(p, 0x10) = 0;
    at<double>(p, 0x18) = k0_001;
    at<std::int32_t>(p, 0x20) = 0;
    at<double>(p, 0x28) = k5;
    at<std::uint64_t>(p, 0x30) = 0;
    at<std::uint8_t>(p, 0x39) = 1;
    at<std::int32_t>(p, 0x3c) = 0x14;
    at<std::uint8_t>(p, 0x38) = 1;
    at<std::int32_t>(p, 0x40) = 0x15;
    at<std::int32_t>(p, 0x48) = 0;
    at<std::int32_t>(p, 0x44) = 4;
    at<std::uint64_t>(p, 0x58) = 0;
    at<std::uint64_t>(p, 0x60) = 0;
    at<std::uint64_t>(p, 0x68) = 0;
    at<double>(p, 0x70) = k1;
    at<double>(p, 0x78) = k1;
    at<std::uint8_t>(p, 0x4c) = 0;
    at<std::int32_t>(p, 0x50) = 3;
    at<std::uint8_t>(p, 0x80) = 1;
    double angle = k210;
    geo23_set_angle(p, &angle);
    at<std::uint8_t>(p, 0x81) = 0;
    at<double>(p, 0x88) = k1;
    for (std::size_t off = 0x90; off <= 0xc0; off += 8) at<std::uint64_t>(p, off) = 0;
    at<double>(p, 0xc8) = k1;
    at<std::uint8_t>(p, 0xd0) = 0;
    at<double>(p, 0xd8) = k1;
    for (std::size_t off = 0xe0; off <= 0xf8; off += 8) at<std::uint64_t>(p, off) = 0;
    at<double>(p, 0x100) = k1;
    at<std::uint64_t>(p, 0x108) = 0;
    at<std::uint64_t>(p, 0x110) = 0;
    at<std::uint64_t>(p, 0x118) = 0;
}

// @src 140189200
void geo23_set_angle(std::byte* p, const double* v)
{
    at<double>(p, 8) = *v;
    std::int32_t n;
    if (*v / k3 <= k10) {
        n = 10;
    } else {
        n = static_cast<std::int32_t>(*v / k3);
    }
    at<std::int32_t>(p, 0x3c) = n;
    if (at<std::uint8_t>(p, 0x80) != 0) at<double>(p, 0x10) = (k180 - at<double>(p, 8)) / k2;
}

// @src 1401890e0
void geo23_set_owner(std::byte* p, DB_STEP_DATA* m) { at<DB_STEP_DATA*>(p, 0x120) = m; }

// Cls_140269250 is p1-06's (layout/cls_140269250.hpp); these forward to it.
void shape_construct(std::byte* q, DB_STEP_DATA* m) { new (q) Cls_140269250(m); }
void shape_reset(std::byte* q) { reinterpret_cast<Cls_140269250*>(q)->reset(); }
void shape_set_owner(std::byte* q, DB_STEP_DATA* m) { reinterpret_cast<Cls_140269250*>(q)->set_owner(m); }
void shape_set_dim(std::byte* q, const std::int32_t* dim) { reinterpret_cast<Cls_140269250*>(q)->set_dimension(dim); }

// @src 140257c20
void list3_reset(std::byte* p)
{
    at<std::uint64_t>(p, 8) = 0;
    at<std::uint64_t>(p, 0) = 0;
    at<std::int32_t>(p, 0x10) = 0;
}

// @src 140259860
void list3_reset_b(std::byte* p)
{
    at<std::uint64_t>(p, 8) = 0;
    at<std::uint64_t>(p, 0) = 0;
    at<std::int32_t>(p, 0x10) = 0;
}

} // namespace def::model::llobj_ctor
