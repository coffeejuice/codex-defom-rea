// _DEF_AMGGEO (RTTI .?AV_DEF_AMGGEO@@, no base): the GEO file reader / store object of GFREAD
// (2D curves and 3D surface facets), used by GFREAD (p8-01), the STL/NASTRAN readers (p8-02) and
// BRDEXT (p8-05). Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 3 and 5.
//
//   vtable   0x140534cd0 (1 slot: scalar deleting dtor 1402566d0)
//   ctor     140246d00 (vptr, 140246d70 curve fields, 140246dd0 surface fields)
//   dtor     140246d40 (vptr, 140246e10 = 140246e40 + 140246ef0: free everything)
//   size     0x40 (last field 0x38; callers keep it on the stack)
//   note     the curve reader (2D) and the surface reader (3D) share +0x20 (point / node count)
//            and +0x28 (the double array): 2D points use stride 3 or 6, 3D nodes stride 3.
//
// Owner: packet p8-09 (bodies in port/defcmd/src/geolib/, API in defcmd/geolib.hpp).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DEF_AMGGEO {
public:
    // @src 140246d00  ctor
    DEF_AMGGEO();
    // @src 140246d40  dtor; slot 0 is the scalar deleting dtor 1402566d0
    virtual ~DEF_AMGGEO();

    DEF_AMGGEO(const DEF_AMGGEO&) = delete;
    DEF_AMGGEO& operator=(const DEF_AMGGEO&) = delete;

    // @src 140246d70  curve fields: type 1, nc 0, start/code null, npoint 0, points null
    void init_curves();
    // @src 140246dd0  surface fields: npoint 0, nfacet 0, points/facets null (no free)
    void init_surface();
    // @src 140246e10  reset_curves then reset_surface
    void release();
    // @src 140246e40  nc 0, free start, code, points; npoint 0
    void reset_curves();
    // @src 140246ef0  npoint 0, nfacet 0, free points and facets
    void reset_surface();

    std::int32_t curve_type = 1;       // +0x08 1 = 3 values per point, 2 = 6 values (file type 3)
    std::int32_t curve_count = 0;      // +0x0c nc
    std::int32_t* curve_start = nullptr;  // +0x10 1-based first point of each curve (calloc)
    std::int32_t* curve_code = nullptr;   // +0x18 boundary code of each curve (calloc)
    std::int32_t npoint = 0;           // +0x20 2D: points read; 3D: nodes (nn)
    std::int32_t pad24 = 0;
    double* points = nullptr;          // +0x28 2D points / 3D nodes (x, y, z [, 3 more])
    std::int32_t nfacet = 0;           // +0x30 3D facets (ne)
    std::int32_t pad34 = 0;
    std::int32_t* facets = nullptr;    // +0x38 3D facets, 4 ints each
};

static_assert(sizeof(DEF_AMGGEO) == 0x40);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, curve_type, 0x08);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, curve_count, 0x0c);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, curve_start, 0x10);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, curve_code, 0x18);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, npoint, 0x20);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, points, 0x28);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, nfacet, 0x30);
DEF_LAYOUT_OFFSET(DEF_AMGGEO, facets, 0x38);

} // namespace def::model
