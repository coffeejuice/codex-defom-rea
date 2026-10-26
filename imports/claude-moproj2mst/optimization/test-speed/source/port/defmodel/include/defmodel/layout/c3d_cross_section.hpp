// C3dCrossSection (RTTI .?AVC3dCrossSection@@): abstract base of GEO2D_AND_MESH2D. Only a vptr
// (GEO2D_AND_MESH2D's own members start at 0x08). Class owned by p1-08; header written by p1-07
// because GEO2D_AND_MESH2D (p1-07 ctor/dtor) derives from it.
//
//   vtable   0x140538b70 (2 slots: scalar deleting dtor 1402848b0, _purecall)
//   ctor     140284870 (vptr only); also 140284940 (the copy-ctor form GEO2D_AND_MESH2D's
//            copy ctor 14027f7a0 calls; vptr only)
//   dtor     140284890 (vptr only)
//   size     0x8
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class C3dCrossSection : public geo::MallocObject {
public:
    // @src 140284870  ctor (vptr only)
    C3dCrossSection();
    // @src 140284940  ctor used by the derived copy ctor (vptr only)
    C3dCrossSection(const C3dCrossSection&);
    C3dCrossSection& operator=(const C3dCrossSection&) = delete;

    // @src 1402848b0  slot 0: scalar deleting dtor (dtor body 140284890)
    virtual ~C3dCrossSection();
    // slot 1: pure (_purecall)
    virtual const char* class_name() = 0;

    // vptr at 0x000
};

static_assert(sizeof(C3dCrossSection) == 0x8);

} // namespace def::model
