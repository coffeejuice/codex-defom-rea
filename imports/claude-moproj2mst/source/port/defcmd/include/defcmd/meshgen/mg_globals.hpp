// defcmd/meshgen/mg_globals.hpp: globals of the mesh generators (_MeshGenBase users), owner p8-07.
// They persist between commands (program lifetime); reset_mg_globals() restores the start values.
#pragma once

#include <cstdint>

namespace def::cmd {

// @data 1406e2d74  mesher state: 0 start, 1 surface mesh read, 2 volume mesh read, 3 coating done,
// 4 2D running
inline std::int32_t g_mesher_state = 0;
// @data 1406e2d78  AMG_DXT succeeded (byte: MOVZX at 1401402d0 / 14013f4d8; set by BRDEXT)
inline char g_amg_dxt_ok = 0;
// @data 1406e2d7c  progress code for the empty _MeshGenBase hooks (dword stores 1, 2, 3, 7)
inline std::int32_t g_mesher_progress = 0;
// @data 1406e2d80  material group id of the object being meshed (1401fb270)
inline std::int32_t g_mesher_group_id = 0;
// @data 1406e2d90  density text of a porous object (1401444f0; "%.12g" or "a ~ b"; size not recovered)
inline char g_density_text[0x400] = {};

inline void reset_mg_globals()
{
    g_mesher_state = 0;
    g_amg_dxt_ok = 0;
    g_mesher_progress = 0;
    g_mesher_group_id = 0;
    g_density_text[0] = '\0';
}

} // namespace def::cmd
