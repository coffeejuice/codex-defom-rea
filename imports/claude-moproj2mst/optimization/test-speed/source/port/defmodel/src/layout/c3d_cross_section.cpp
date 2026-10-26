// C3dCrossSection: the vptr-only ctor/dtor bodies (the vptr store is compiler-generated).
// Class owned by p1-08; these three are trivial, written by p1-07 so GEO2D_AND_MESH2D links.
// Layout: defmodel/layout/c3d_cross_section.hpp.
#include <defmodel/layout/c3d_cross_section.hpp>

namespace def::model {

// @src 140284870  ctor: *this = C3dCrossSection::vftable
C3dCrossSection::C3dCrossSection() = default;

// @src 140284940  ctor (copy form): *this = C3dCrossSection::vftable
C3dCrossSection::C3dCrossSection(const C3dCrossSection&) {}

// @src 140284890  dtor: *this = C3dCrossSection::vftable
// @src 1402848b0  slot 0: scalar deleting dtor (free through MallocObject)
C3dCrossSection::~C3dCrossSection() = default;

} // namespace def::model
