// defcmd/globals_p8_04.hpp: globals the DXF reader (packet p8-04) uses.
//
// Own globals: the DXF tolerance G (DAT_14062bc68).
// Shared with p8-03's IGES reader (owner p8-03, defcmd/iges.hpp): the conversion context
// DAT_14070bc20 (iges::IgesCtx g_iges_ctx), the Fortran work arrays DAT_14070c050 / DAT_14070c058
// with their sizes DAT_140628568 / DAT_140628554, and the SPLINE sample count DAT_140628550. The
// DXF code reaches them through the accessors below: p8-03's variables when iges.hpp is present,
// else a provisional copy here (same layout, same start values, no @data tag).
#pragma once

#include <cstddef>
#include <cstdint>

#if __has_include(<defcmd/iges.hpp>)
#include <defcmd/iges.hpp>
#define DEF_P804_SHARED_FROM_P803 1
#endif

namespace def::cmd::dxf {

// @data 14062bc68  DXF tolerance G (FUN_140428f70 / FUN_1404099c0), process start value 1e-9
// (0x3e112e0be826d695)
inline double dat_14062bc68 = 1e-9;

#ifdef DEF_P804_SHARED_FROM_P803
using ConvContext = def::cmd::iges::IgesCtx;
inline ConvContext& conv_ctx() { return def::cmd::iges::g_iges_ctx; }
inline double*& conv_rw() { return def::cmd::iges::g_iges_rw; }
inline std::int32_t*& conv_iw() { return def::cmd::iges::g_iges_iw; }
inline std::int32_t conv_rw_size() { return def::cmd::iges::g_iges_rw_size; }
inline std::int32_t conv_iw_size() { return def::cmd::iges::g_iges_iw_size; }
inline std::int32_t spline_samples() { return def::cmd::iges::g_iges_nurbs_samples; }
#else
namespace provisional {
// data 14070bc20 (p8-03's g_iges_ctx), 0x430 bytes, zeroed by 14040ba80
struct ConvContext {
    char name[0x400];        // 0x000 not touched by the DXF path
    std::int32_t n_points;   // 0x400 DAT_14070c020 (points of the segment list)
    std::int32_t n_records;  // 0x404 DAT_14070c024 (segments; the Fortran write-back updates it)
    std::int32_t n100, n110, n112, n126, n124;
    std::byte pad_41c[4];
    double tol;              // 0x420 DAT_14070c040
    double* table;           // 0x428 DAT_14070c048 (segment array, 6 doubles per segment)
};
static_assert(sizeof(ConvContext) == 0x430);
static_assert(offsetof(ConvContext, n_points) == 0x400);
static_assert(offsetof(ConvContext, n_records) == 0x404);
static_assert(offsetof(ConvContext, tol) == 0x420);
static_assert(offsetof(ConvContext, table) == 0x428);
inline ConvContext ctx{};
inline double* rw = nullptr;              // data 14070c050
inline std::int32_t* iw = nullptr;        // data 14070c058
inline std::int32_t rw_size = 1500000;    // data 140628568
inline std::int32_t iw_size = 1500000;    // data 140628554
inline std::int32_t samples = 30;         // data 140628550
} // namespace provisional
using ConvContext = provisional::ConvContext;
inline ConvContext& conv_ctx() { return provisional::ctx; }
inline double*& conv_rw() { return provisional::rw; }
inline std::int32_t*& conv_iw() { return provisional::iw; }
inline std::int32_t conv_rw_size() { return provisional::rw_size; }
inline std::int32_t conv_iw_size() { return provisional::iw_size; }
inline std::int32_t spline_samples() { return provisional::samples; }
#endif

} // namespace def::cmd::dxf
