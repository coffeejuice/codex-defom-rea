// Unit conversion of an imported material and its phase pairs (packet mdt-keyimport), reached
// from 1401b6ea0 when its convert flag is 1. Decompiles: deform-rea-work/out/mdt1/f1.c, f2.c,
// f3.c; FP order checked against the listing (out/mdt1/asm_conv.txt): x * f, x * (1.0 / f),
// x * pow(f, -y) with -y formed by the sign-bit xor before the factor call.
#include <defcmd/key_import.hpp>

#include <cstddef>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defrt/ftn/units.hpp>
#include <defrt/rdata.hpp>
#include <defrt/stdlib/libm.hpp>

namespace def::cmd {

namespace {

using model::DYNARY;

constexpr double kOne = rt::rdata::d_140509c90;  // 1.0
constexpr double k32 = 32.0;                     // rdata 140522fb0
constexpr double k18 = 1.8;                      // rdata 140531748 (0x3ffccccccccccccd)

double& d(void* p, std::size_t off) { return *reinterpret_cast<double*>(static_cast<std::byte*>(p) + off); }
std::int32_t& i32(void* p, std::size_t off)
{
    return *reinterpret_cast<std::int32_t*>(static_cast<std::byte*>(p) + off);
}
DYNARY* dy(void* p, std::size_t off) { return reinterpret_cast<DYNARY*>(static_cast<std::byte*>(p) + off); }

double F(std::int32_t kind, std::int32_t mode) { return rt::ftn::unit_factor(kind, mode); }        // 140235d00
double T(std::int32_t mode, double t) { return rt::ftn::convert_temperature(mode, t); }           // 140235d80
double pw(double x, double y) { return rt::libm::crt_pow(x, y); }                                 // 140483f60

// 14012e400(t, kind, mode, 1)
void conv(void* p, std::size_t off, std::int32_t kind, std::int32_t mode)
{
    model::ext::fun_14012e400(dy(p, off), kind, mode, true);
}

// 14012fa90 (def_pre's dynary_is_constant, p10-01; inlined here, defcmd cannot call def_pre)
bool is_constant(const DYNARY* t) { return t->ndim == 0; }

std::int32_t mode_of(std::int32_t key_unit, std::int32_t model_unit)
{
    if (key_unit == 2 && model_unit == 1) return 2;
    if (key_unit == 1 && model_unit == 2) return 1;
    return 0;
}

} // namespace

// @src 14019df50
void material_convert_units_mixture(DB_STEP_DATA* m, model::Material* mat, std::int32_t key_unit,
                                    std::int32_t model_unit)
{
    const std::int32_t mode = mode_of(key_unit, model_unit);
    if (mode != 0) material_convert_mixture_mode(m, mat, mode);  // 14019dfc0
}

// @src 14019dfc0
void material_convert_mixture_mode(DB_STEP_DATA* m, model::Material* mat, std::int32_t mode)
{
    if (mat == nullptr) return;
    material_convert_mode(mat, mode);  // 14033ae10
    for (std::int32_t i = 0; i < mat->mstmtr_count; i = i + 1) {
        model::Material* c = model::material_list_find(&m->materials, mat->mstmtr_list[i]);  // 14029c2c0
        if (c != nullptr) material_convert_mode(c, mode);
    }
    for (std::int32_t i = 0; i < model::phase_pair_count(&m->phase_pairs); i = i + 1) {  // 1401b05f0
        model::PhasePair* p = model::phase_pair_nth(&m->phase_pairs, i + 1);              // 140339fe0
        if (model::phase_pair_in_mixture(p, mat)) phase_pair_convert_mode(p, mode);       // 140339ba0, 14033aa30
    }
}

// @src 14033adb0
void material_convert_units(model::Material* mat, std::int32_t key_unit, std::int32_t model_unit)
{
    const std::int32_t mode = mode_of(key_unit, model_unit);
    if (mode != 0) material_convert_mode(mat, mode);  // 14033ae10
}

// @src 14033ae10
void material_convert_mode(model::Material* mat, std::int32_t mode)
{
    if (mode == 0) return;
    void* p = mat;
    d(p, 0x58) = d(p, 0x58) * F(7, mode);
    d(p, 0x70) = d(p, 0x70) * F(7, mode);
    conv(p, 0x78, 7, mode);
    d(p, 0xf0) = d(p, 0xf0) * (kOne / F(7, mode));
    d(p, 0x110) = d(p, 0x110) * F(0x24, mode);
    {
        const double y = -d(p, 0x128);
        d(p, 0x120) = d(p, 0x120) * pw(F(7, mode), y);
    }
    d(p, 0x130) = d(p, 0x130) * F(0x24, mode);
    conv(p, 0x138, 7, mode);
    conv(p, 0x1b0, 7, mode);
    d(p, 0x2a0) = d(p, 0x2a0) * F(7, mode);
    d(p, 0x2a8) = d(p, 0x2a8) * F(7, mode);
    d(p, 0x2f0) = T(mode, d(p, 0x2f0));
    d(p, 0x2f8) = T(mode, d(p, 0x2f8));
    d(p, 0x300) = T(mode, d(p, 0x300));
    {
        const double y = -d(p, 0x2e0);
        d(p, 0x308) = d(p, 0x308) * pw(F(9, mode), y);
    }
    d(p, 0x310) = d(p, 0x310) * F(7, mode);
    d(p, 0x318) = d(p, 0x318) * F(7, mode);
    d(p, 0x320) = d(p, 0x320) * (kOne / F(9, mode));
    d(p, 0x328) = d(p, 0x328) * (kOne / F(9, mode));
    d(p, 0x340) = d(p, 0x340) * F(7, mode);
    d(p, 0x360) = d(p, 0x360) * F(9, mode);
    if (i32(p, 0x1e40) == 3)
        conv(p, 0x1e48, 0xb, mode);
    else
        dynary_convert_units_multi(dy(p, 0x1e48), 0, mode);  // 14012e9b0
    conv(p, 0x958, 0x1a, mode);
    dynary_convert_units_multi(dy(p, 0x9d0), 0x27, mode);
    dynary_convert_units_multi(dy(p, 0xa48), 0x28, mode);
    conv(p, 0xad0, 7, mode);
    conv(p, 0xb48, 7, mode);
    dynary_convert_units_multi(dy(p, 0xbc0), 0x45, mode);
    dynary_convert_units_multi(dy(p, 0xc38), 0x46, mode);
    dynary_convert_units_multi(dy(p, 0xcb0), 0x47, mode);
    dynary_convert_units_multi(dy(p, 0xd28), 0x48, mode);
    dynary_convert_units_multi(dy(p, 0x1088), 0x55, mode);
    dynary_convert_units_multi(dy(p, 0x1100), 0x54, mode);
    dynary_convert_units_multi(dy(p, 0x1178), 0x56, mode);
    dynary_convert_units_multi(dy(p, 0x11f0), 0x57, mode);
    dynary_convert_units_multi(dy(p, 0x1268), 0x58, mode);
    dynary_convert_units_multi(dy(p, 0x12e0), 0x59, mode);
    dynary_convert_units_multi(dy(p, 0x1358), 0x5a, mode);
    dynary_convert_units_multi(dy(p, 0x13d0), 0x5b, mode);
    dynary_convert_units_multi(dy(p, 0x1448), 0x5c, mode);
    d(p, 0x14c0) = T(mode, d(p, 0x14c0));
    conv(p, 0x1b10, 7, mode);
    conv(p, 0x670, 7, mode);
    conv(p, 0x6e8, 0xb, mode);
    conv(p, 0x760, 0x16, mode);
    d(p, 0x7d8) = T(mode, d(p, 0x7d8));
    conv(p, 0x7e0, 0x17, mode);
    if (i32(p, 0x8d0) == 1) {
        conv(p, 0x858, 0x18, mode);
        model::ext::fun_14012e6d0(dy(p, 0x858), 0x2a, mode, false);
    } else {
        conv(p, 0x858, 0x18, mode);
    }
    conv(p, 0x8d8, 0x19, mode);
    conv(p, 0x1fe0, 0x2a, mode);
    conv(p, 0xf10, 0xb, mode);
    conv(p, 0xf88, 1, mode);
    conv(p, 0x1000, 0xb, mode);
    conv(p, 0x1cd8, 0x1b, mode);
    conv(p, 0x1d50, 0x1c, mode);
    conv(p, 0x1dc8, 0x1d, mode);
    conv(p, 0x1968, 0xb, mode);
    conv(p, 0x18f0, 0xb, mode);
    conv(p, 0x19e0, 0xb, mode);
    conv(p, 0x1a58, 0xb, mode);
    conv(p, 0x20d8, 0xb, mode);
    dynary_convert_units_multi(dy(p, 0x2160), 0xaf, mode);
    conv(p, 0x1878, 0xb, mode);
    conv(p, 0x17f0, 0xb, mode);
    if (!is_constant(dy(p, 0x1ef0)))
        conv(p, 0x1ef0, 5, mode);
    else
        d(p, 0x1ef8) = d(p, 0x1ef8) * F(5, mode);
    if (!is_constant(dy(p, 0x1f68)))
        conv(p, 0x1f68, 5, mode);
    else
        d(p, 0x1f70) = d(p, 0x1f70) * F(5, mode);
    d(p, 0x1ed0) = d(p, 0x1ed0) * F(0x1e, mode);
    d(p, 0x1ed8) = T(mode, d(p, 0x1ed8));
    d(p, 0x1ee0) = T(mode, d(p, 0x1ee0));
}

// @src 14033aa30
void phase_pair_convert_mode(model::PhasePair* pp, std::int32_t mode)
{
    if (mode == 0) return;
    void* p = pp;
    dynary_convert_units_multi(dy(p, 0x78), 0x2c, mode);
    dynary_convert_units_multi(dy(p, 0xf8), 0x37, mode);
    dynary_convert_units_multi(dy(p, 0x1f8), 0x30, mode);
    d(p, 0x278) = T(mode, d(p, 0x278));
    d(p, 0x280) = d(p, 0x280) * F(9, mode);
    d(p, 0x290) = T(mode, d(p, 0x290));
    d(p, 0x298) = d(p, 0x298) * F(9, mode);
    d(p, 0x2a8) = d(p, 0x2a8) * (kOne / F(7, mode));
    conv(p, 0x2c8, 0xb, mode);
    d(p, 0x340) = d(p, 0x340) * (kOne / F(7, mode));
    d(p, 0x360) = d(p, 0x360) * (kOne / F(9, mode));
    d(p, 0x370) = d(p, 0x370) * (kOne / F(7, mode));
    d(p, 0x378) = d(p, 0x378) * (kOne / F(7, mode));
    if (mode == 1) {
        d(p, 0x380) = d(p, 0x380) - d(p, 0x360) * k32;
    } else if (mode == 2) {
        d(p, 0x380) = d(p, 0x380) + (d(p, 0x360) * k32) / k18;
    }
    d(p, 0x3a0) = T(mode, d(p, 0x3a0));
    d(p, 0x3a8) = T(mode, d(p, 0x3a8));
    conv(p, 0x478, 0x1e, mode);
    conv(p, 0x4f0, 0xb, mode);
    i32(p, 0x5e8) = 0x2a;  // DYNARY type: 14012e400 then uses 1.0 / factor
    conv(p, 0x5e8, 7, mode);
}

// @src 14012e9b0
void dynary_convert_units_multi(DYNARY* t, std::int32_t kind, std::int32_t mode)
{
    if (mode == 0 || !model::dynary_counts_positive(t) || !(1 < t->ncomp)) return;  // 14012f610
    for (std::int32_t a = 0; a < t->ndim; a = a + 1) {
        const std::int32_t k = model::kVarUnitKind[t->var[a]];  // DAT_14051b7f0
        if (k == 9) {
            for (std::int32_t j = 0; j < t->n[a]; j = j + 1) t->axis[a][j] = T(mode, t->axis[a][j]);
        } else {
            for (std::int32_t j = 0; j < t->n[a]; j = j + 1) {
                const double* ax = t->axis[a];
                const double f = F(k, mode);  // per element, as in the original
                t->axis[a][j] = ax[j] * f;
            }
        }
    }
    if (t->values == nullptr) return;
    double* v = nullptr;
    if (kind == 0x27) {
        for (std::int32_t i = 0; i < t->n[0]; i = i + 1) {
            v = t->values;
            t->values[i * t->ncomp] = v[i * t->ncomp] * F(0x1a, mode);
            v = t->values;
            t->values[i * t->ncomp + 1] = v[i * t->ncomp + 1] * F(0xb, mode);
        }
    } else if (kind == 0x28) {
        for (std::int32_t i = 0; i < t->n[0]; i = i + 1) {
            v = t->values;
            t->values[i * t->ncomp] = v[i * t->ncomp] * F(0x1a, mode);
            v = t->values;
            t->values[i * t->ncomp + 1] = v[i * t->ncomp + 1] * F(9, mode);
        }
    } else if (kind == 0x30) {
        for (std::int32_t i = 0; i < model::dynary_value_count(t); i = i + 1)  // 14012ff20
            t->values[i] = T(mode, t->values[i]);
    } else if (kind == 0x47) {
        for (std::int32_t i = 0; i < t->n[0]; i = i + 1) {
            v = t->values;
            double y = -t->values[i * t->ncomp + 1];
            t->values[i * t->ncomp] = v[i * t->ncomp] * pw(F(7, mode), y);
            v = t->values;
            y = -t->values[i * t->ncomp + 4];
            t->values[i * t->ncomp + 3] = v[i * t->ncomp + 3] * pw(F(7, mode), y);
        }
    } else if (kind == 0x48) {
        for (std::int32_t i = 0; i < t->n[0]; i = i + 1) {
            v = t->values;
            const double y = -t->values[i * t->ncomp + 1];
            t->values[i * t->ncomp] = v[i * t->ncomp] * pw(F(7, mode), y);
            v = t->values;
            t->values[i * t->ncomp + 2] = v[i * t->ncomp + 2] * F(9, mode);
        }
    }
}

} // namespace def::cmd
