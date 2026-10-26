// _DEF_AMGGEO ctor, dtor and field resets (packet p8-09). Layout: defmodel/layout/def_amggeo.hpp.
// Readers and stores: port/defcmd/src/geolib/amggeo.cpp.
#include <defmodel/layout/def_amggeo.hpp>

#include <cstdlib>

namespace def::model {

// @src 140246d00  ctor: vptr 0x140534cd0, 140246d70, 140246dd0
DEF_AMGGEO::DEF_AMGGEO()
{
    init_curves();
    init_surface();
}

// @src 140246d40  dtor: vptr, 140246e10
// @src 1402566d0  scalar deleting dtor (slot 0): dtor, free when (flags & 1) (compiler-generated)
DEF_AMGGEO::~DEF_AMGGEO()
{
    release();
}

// @src 140246d70
void DEF_AMGGEO::init_curves()
{
    curve_type = 1;
    curve_count = 0;
    curve_start = nullptr;
    curve_code = nullptr;
    npoint = 0;
    points = nullptr;
}

// @src 140246dd0
void DEF_AMGGEO::init_surface()
{
    npoint = 0;
    nfacet = 0;
    points = nullptr;
    facets = nullptr;
}

// @src 140246e10
void DEF_AMGGEO::release()
{
    reset_curves();
    reset_surface();
}

// @src 140246e40
void DEF_AMGGEO::reset_curves()
{
    curve_count = 0;
    if (curve_start != nullptr) std::free(curve_start);
    curve_start = nullptr;
    if (curve_code != nullptr) std::free(curve_code);
    curve_code = nullptr;
    npoint = 0;
    if (points != nullptr) std::free(points);
    points = nullptr;
}

// @src 140246ef0
void DEF_AMGGEO::reset_surface()
{
    npoint = 0;
    nfacet = 0;
    if (points != nullptr) std::free(points);
    points = nullptr;
    if (facets != nullptr) std::free(facets);
    facets = nullptr;
}

} // namespace def::model
