// defcmd/src/objpos/centre2d.cpp: own centre of a 2D object (OBJPOS mode 6, OBJPOS.md 8.5,
// OBJPOS_geo_kernels.md 4.4). Decompile: deform-rea-work/out/p67/objpos.c, objpos-geo/geo.c;
// comparisons and the division from the listing (out/p671/centre/L_mp.txt).
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>

#include <cstdint>

#include <defdbio/gendb/counts_frame.hpp>

namespace def::cmd::objpos {

using model::LLOBJ_DATA;

// @src 1401ff4f0  c_i = (min_i + max_i) / 2.0 (DAT_140509fb8), i = 0, 1
bool centre_2d(LLOBJ_DATA* o, double* c)
{
    double b[4];  // local_30: min x, y, max x, y
    bool ok = geometry_bbox_2d(o, b, b + 2);
    if (!ok && mesh_bbox(o, b, b + 2)) ok = true;
    if (ok) {
        for (int i = 0; i < 2; ++i) c[i] = (b[i] + b[i + 2]) / 2.0;
    }
    return ok;
}

// 1401ff600: ported in defdbio/gendb/counts_frame.hpp (object_geo2d_range)
bool geometry_bbox_2d(LLOBJ_DATA* o, double* mn, double* mx) { return dbio::gendb::object_geo2d_range(o, mn, mx); }

// 1401ff5b0: ported in defdbio/gendb/counts_frame.hpp (object_mesh_range)
bool mesh_bbox(LLOBJ_DATA* o, double* mn, double* mx) { return dbio::gendb::object_mesh_range(o, mn, mx); }

} // namespace def::cmd::objpos
