// kw-g2geo: the Packet API entry points (defkey/kw_g2geo.hpp), routed to the four families,
// and writer-map steps 19-22 of FUN_14029e250.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>

namespace def::key::g2geo {

Target target(DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = def::key::g2obj::find_object(m, n);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    return {obj, n};
}

bool handles(int id)
{
    return curve::handles(id) || surface::handles(id) || mesh::handles(id) || misc::handles(id);
}

// part of @src 14038b8a0  the kw-g2geo cases of the group 2 parser
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    if (curve::handles(id)) return curve::read_case(m, f, obj, id);
    if (surface::handles(id)) return surface::read_case(m, f, obj, id);
    if (mesh::handles(id)) return mesh::read_case(m, f, obj, id);
    if (misc::handles(id)) return misc::read_case(m, f, obj, id);
    return 2;
}

bool handles_write(int id) { return mesh::handles_write(id); }

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (mesh::handles_write(id)) return mesh::write_keyword(id, f, m, ref);
    return 0;
}

// part of @src 14029e250  writer-map steps 19-22 (group2-writer-map.md), sub-writer statuses
// ignored as in the original
void write_geometry(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    namespace gk = def::model::geo_kw;
    // 19: DIEGEO curves, not 2D and FUN_140198050(obj, 0)
    if (!is_2d(m) && gk::has_curve(obj, 0)) curve::write_curves(m, f, obj, n);
    // 20: geometry dispatcher; not 2D: FUN_140160d90 (byte +0x3990); 2D: FUN_140160d20 and the
    // type of curve 0 (FUN_1401982c0) != 0
    bool geo20;
    if (!is_2d(m)) {
        geo20 = gk::has_surface_flag(obj);
    } else {
        geo20 = gk::geometry_has_edges(obj) && gk::curve_type0_nonzero(obj);
    }
    if (geo20) surface::write_geometry_dispatch(m, f, obj, n);
    // 21: 2D TPLOGY with the same test
    if (is_2d(m) && gk::geometry_has_edges(obj) && gk::curve_type0_nonzero(obj))
        curve::write_tplogy(m, f, obj, n);
    // 22: FLDGEO / FLWNET, 3D, each if FUN_140160e50 / FUN_140160eb0
    if (is_3d(m)) {
        if (gk::has_fldgeo(obj)) surface::write_surface_mesh(m, f, kw(0x121), &at<std::byte>(obj, 0x3328), n);
        if (gk::has_flwnet(obj)) surface::write_surface_mesh(m, f, kw(0x122), &at<std::byte>(obj, 0x3458), n);
    }
}

int write_nodes(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n) { return mesh::write_nodes(m, f, obj, n); }
int write_elements(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n) { return mesh::write_elements(m, f, obj, n); }
int write_symsuf(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n) { return misc::write_symsuf(m, f, obj, n); }
int write_wpaxis(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n) { return misc::write_wpaxis(m, f, obj, n); }

}  // namespace def::key::g2geo
