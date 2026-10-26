// defcmd/src/objpos/predicates.cpp: the object tests of OBJPOS (OBJPOS.md 4, _common.md 2).
// Decompile: deform-rea-work/out/p67/objpos.c.
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>

#include <cstdint>

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::LLOBJ_DATA;
using detail::at;

// @src 1401611f0  type +0xc != 7 (140160f10) and mesh or geometry (140160ba0), else the message
bool usable(LLOBJ_DATA* o)
{
    if (at<std::int32_t>(o, 0xc) == 7) {
        rt::crt::printf("ERROR  : Object %d Undefined\n", at<std::int32_t>(o, 8));
        return false;
    }
    if (!has_mesh_or_geometry(o)) {
        rt::crt::printf("ERROR  : No Mesh or Die Defined for Object %d\n", at<std::int32_t>(o, 8));
        return false;
    }
    return true;
}

// 140160bf0: ported in defdbio/gendb/gendb.hpp (has_mesh)
bool has_mesh(const LLOBJ_DATA* o) { return dbio::gendb::has_mesh(o); }

// @src 140160ba0  has_mesh or has_geometry
bool has_mesh_or_geometry(const LLOBJ_DATA* o) { return has_mesh(o) || has_geometry(o); }

// 140160cc0: ported in defdbio/gendb/counts_frame.hpp (object_has_geometry)
bool has_geometry(const LLOBJ_DATA* o) { return dbio::gendb::object_has_geometry(const_cast<LLOBJ_DATA*>(o)); }

// 140203980: ported in defdbio/gendb3d/obj3d.hpp (g3d::object_has_geometry)
bool has_die_geometry(LLOBJ_DATA* o) { return dbio::g3d::object_has_geometry(o); }

// 1402039d0: ported in defdbio/gendb/counts_frame.hpp (object_geo2d_usable)
bool has_die_geometry_2d(LLOBJ_DATA* o) { return dbio::gendb::object_geo2d_usable(o); }

} // namespace def::cmd::objpos
