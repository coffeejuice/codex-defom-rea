// defcmd/cnvt3d_build.hpp: the CNVT3D per-object generator as the driver (p9-01, defcmd/cnvt3d.hpp,
// Cnvt3dExt::generate) calls it. Packet p9-03a; the port is def::cmd::cnvt3d_sweep::fun_140147b70
// (defcmd/cnvt3d_sweep.hpp), this is the driver's spelling of it.
#pragma once

#include <cstdint>

#include <defcmd/cnvt3d_sweep.hpp>

namespace def::cmd::cnvt3d {

// 140147b70  (forwards to cnvt3d_sweep::fun_140147b70) kind 5 geometry, 2 tet mesh, 4 brick sweep of object id
inline void fun_140147b70(MeshGenBase& cvt, std::int32_t id, std::int32_t nsec, std::int32_t m, double start,
                          double ang, double c, std::int32_t kind)
{
    cnvt3d_sweep::fun_140147b70(&cvt, id, nsec, m, start, ang, c, kind);
}

} // namespace def::cmd::cnvt3d
