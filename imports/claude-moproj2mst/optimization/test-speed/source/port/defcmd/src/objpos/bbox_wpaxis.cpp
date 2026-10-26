// defcmd/src/objpos/bbox_wpaxis.cpp: bounding boxes and the WPAXIS item lookup of OBJPOS modes 13
// and 14 (OBJPOS.md 7.5, 7.6). Decompile: deform-rea-work/out/p67/objpos.c; comparisons from the
// listing (out/p671/centre/L_mp.txt).
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/objpos_parts.hpp>

#include <cstdint>
#include <cstring>

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/record_e8_list.hpp>

#include "internal.hpp"

namespace def::cmd::objpos {

using model::LLOBJ_DATA;
using detail::at;

// 140181ae0: ported in defdbio/gendb/counts_frame.hpp (node_coord_range); the listing
// 140181c48..140181c7c is `COMISD min,v; JBE` then (else) `COMISD v,max; JBE`, as there
bool node_bbox(const void* node_set, double* mn, double* mx)
{
    return dbio::gendb::node_coord_range(const_cast<void*>(node_set), mn, mx);
}

// @src 1402faca0  box of the points of a surface (init DAT_14052c598 1e29 / DAT_14052c5a0 -1e29)
void surface_bbox(model::OBJ_GEO3D* g, double* mn, double* mx)
{
    mn[0] = mn[1] = mn[2] = 1e29;
    mx[0] = mx[1] = mx[2] = -1e29;
    if (g->node_count() == 0) return;  // 1400034d0
    const std::int32_t n = g->node_count();
    const double* p = g->coords.begin();  // 14026a470(g + 0x10, i)
    for (std::int32_t i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            const double v = p[i * 3 + j];
            if (mn[j] > v) mn[j] = v;
            if (v > mx[j]) mx[j] = v;
        }
    }
}

// 140203fa0: ported in defdbio/objcheck/objcheck.hpp (obj_has_rotsym_code)
bool node_bc_code_7_or_8(LLOBJ_DATA* o) { return dbio::objcheck::obj_has_rotsym_code(o); }

// @src 14025a000  item with +4 == 3 and +0x60 == tag (link +0xe0)
unsigned char* wpaxis_find(unsigned char* list, std::int32_t tag)
{
    unsigned char* it = at<unsigned char*>(list, 0);
    while (it != nullptr && (at<std::int32_t>(it, 4) != 3 || at<std::int32_t>(it, 0x60) != tag))
        it = at<unsigned char*>(it, 0xe0);
    return it;
}

// @src 140259f90  find, else append a new item (140259950(list, 0)) with +4 = 3, +0x60 = tag
unsigned char* wpaxis_find_or_create(unsigned char* list, std::int32_t tag)
{
    unsigned char* it = wpaxis_find(list, tag);
    if (it == nullptr) {
        it = reinterpret_cast<unsigned char*>(
            model::geo_kw::wpaxis_add(*reinterpret_cast<model::linked::Cls_140258580List*>(list), 0));
        at<std::int32_t>(it, 4) = 3;
        at<std::int32_t>(it, 0x60) = tag;
    }
    return it;
}

// 140258f10: ported in defdbio/gendb3d/obj3d.hpp (record_e8_get_08)
void wpaxis_vector_a(unsigned char* item, double* out) { dbio::g3d::record_e8_get_08(item, out); }

// 140258f90: ported in defdbio/gendb3d/obj3d.hpp (record_e8_get_20)
void wpaxis_vector_b(unsigned char* item, double* out) { dbio::g3d::record_e8_get_20(item, out); }

} // namespace def::cmd::objpos
