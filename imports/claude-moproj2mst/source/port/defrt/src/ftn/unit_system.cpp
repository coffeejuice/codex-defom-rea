// Unit system selection and the conversion tables. See defrt/ftn/unit_system.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defrt/ftn/unit_system.hpp>

#include <bit>
#include <cstdio>
#include <cstdlib>

#include <defrt/ftn/units.hpp>

namespace def::rt::ftn {

namespace {

struct ConvFacSrc {
    std::int32_t id;
    const char* name;
    std::uint64_t f[4];
};
struct ConvUnitSrc {
    std::int32_t id;
    const char* label[5];
};

#include "unit_tables.inc"

// 14039d3a0(fmt, "ERROR    ", what, n): the message goes to stdout (14039d570(0, fmt, va))
void alloc_error(const char* what, std::int32_t n)
{
    std::printf("%s Error when allocating %s memory of size %d\n", "ERROR    ", what, n);  // str 140626108
}

// sprintf(dst, "%s", src) as the original does (strings 1406212e4, 14062139c, 140625aa0..abc)
void copy_label(char* dst, const char* src) { std::sprintf(dst, "%s", src); }

} // namespace

// @src 140235c30  unit system: 1 -> flag 0, 2 -> flag 1, other -> flag 0, then 1403f8b70
void set_unit_system(std::int32_t unit)
{
    if (unit == 1) {
        g_units.unit_flag = 0;
    } else if (unit == 2) {
        g_units.unit_flag = 1;
    } else {
        g_units.unit_flag = 0;
    }
    set_conversion_unit_system(unit);
}

// @src 1403f8b70  conversion-table flag, then the rebuild
void set_conversion_unit_system(std::int32_t unit)
{
    if (unit == 1) {
        g_conv.english = 0;
    } else if (unit == 2) {
        g_conv.english = 1;
    } else {
        g_conv.english = 0;
    }
    build_conversion_tables();
}

// @src 1403fb360  CONV_FAC table
ConvFac* alloc_conv_factors(std::int32_t n)
{
    if (n == 0) return nullptr;
    auto* p = static_cast<ConvFac*>(std::calloc(static_cast<std::size_t>(n), 0x88));
    if (p == nullptr) alloc_error("CONV_FAC", n);  // str 1406260e8
    return p;
}

// @src 1403fb3c0  CONV_UNIT table
ConvUnit* alloc_conv_units(std::int32_t n)
{
    if (n == 0) return nullptr;
    auto* p = static_cast<ConvUnit*>(std::calloc(static_cast<std::size_t>(n), 0x1f8));
    if (p == nullptr) alloc_error("CONV_UNIT", n);  // str 140626138
    return p;
}

// @src 1403f9f20  rebuild both tables from the static records 1406214a0 / 140622380
void build_conversion_tables()
{
    g_conv.u8c = 6000;
    g_conv.count = 0x1c;
    g_conv.u94 = 0;
    g_conv.u90 = 4;
    if (g_conv.factors == nullptr) g_conv.factors = alloc_conv_factors(0x1c);
    if (g_conv.units == nullptr) g_conv.units = alloc_conv_units(0x1c);
    for (std::int32_t i = 0; i <= g_conv.count - 1; i = i + 1) {
        ConvFac& d = g_conv.factors[i];
        const ConvFacSrc& s = kConvFacSrc[i];
        d.id = s.id;
        copy_label(d.name, s.name);
        d.f[0] = std::bit_cast<double>(s.f[0]);
        d.f[1] = std::bit_cast<double>(s.f[1]);
        d.f[2] = std::bit_cast<double>(s.f[2]);
        d.f[3] = std::bit_cast<double>(s.f[3]);
        d.f[3] = std::bit_cast<double>(s.f[0]);  // the original stores +0x80 twice, the second from +0x68
    }
    for (std::int32_t i = 0; i <= g_conv.count - 1; i = i + 1) {
        ConvUnit& d = g_conv.units[i];
        const ConvUnitSrc& s = kConvUnitSrc[i];
        d.id = s.id;
        for (int k = 0; k < 5; ++k) copy_label(d.label[k], s.label[k]);
        if (g_conv.english == 0) {
            copy_label(d.label[1], d.label[3]);
            copy_label(d.label[4], d.label[3]);
        } else {
            copy_label(d.label[1], d.label[2]);
            copy_label(d.label[4], d.label[2]);
        }
    }
}

} // namespace def::rt::ftn
