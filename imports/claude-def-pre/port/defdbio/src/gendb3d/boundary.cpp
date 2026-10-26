// gendb3d/boundary.cpp: one 3D LLBND record (WriteBdryInfoToDB) with the rigid geometry (LLGRY,
// IIDOD) and its surface lists as functions (packet p4-05; docs/re/dbmap/object.md section 5,
// objarrays.md section 4).
#include <defdbio/gendb3d/obj3d.hpp>

#include <cstring>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"

namespace def::dbio::g3d {

using model::OBJ_GEO3D;

// @src 140199fc0  +0x50 item usable: entry count (14019a000) > 0 and values per entry (1402f8760) > 0
static bool item50_valid(void* item)
{
    if (0 < ext::f14019a000(item) && 0 < ext::f1402f8760(item)) return true;
    return false;
}

// @src 14019a020  +0x58 item usable: entry count (14019a060) > 0 and 1402f8ce0 > 0
static bool item58_valid(void* item)
{
    if (0 < ext::f14019a060(item) && 0 < ext::f1402f8ce0(item)) return true;
    return false;
}

// @src 1402feef0  node n of the list at geo+0x50 (1402ffcd0) + 0x10 = its item
static void* item50_at(void* list, std::uint64_t n) { return at(ext::f1402ffcd0(*static_cast<void**>(list), n), 0x10); }
// @src 1402ff040  the same for the list at geo+0x58 (1402fffa0)
static void* item58_at(void* list, std::uint64_t n) { return at(ext::f1402fffa0(*static_cast<void**>(list), n), 0x10); }

// @src 1402f9430  number of usable items in the list at geo+0x50
static std::int32_t count_items50(OBJ_GEO3D* g)
{
    std::int32_t n = 0;
    const std::int32_t k = ext::f14019a080(g);
    for (std::int32_t i = 0; i < k; i = i + 1) {
        if (item50_valid(item50_at(at(g, 0x50), static_cast<std::uint64_t>(static_cast<std::int64_t>(i))))) n = n + 1;
    }
    return n;
}

// @src 1402f94c0  number of usable items in the list at geo+0x58
static std::int32_t count_items58(OBJ_GEO3D* g)
{
    std::int32_t n = 0;
    const std::int32_t k = ext::f14019a0a0(g);
    for (std::int32_t i = 0; i < k; i = i + 1) {
        if (item58_valid(item58_at(at(g, 0x58), static_cast<std::uint64_t>(static_cast<std::int64_t>(i))))) n = n + 1;
    }
    return n;
}

// @src 1402f8a70  +0x50 item as a function: type, entry count, values per entry, (type 1) word 3;
//                 type 1 with 5 values: the 5 doubles of each entry from word 10
static void write_item50(void* item, double* out)
{
    const std::int32_t type = ext::f1402b7470(item);
    const std::int32_t cnt = ext::f14019a000(item);
    const std::int32_t per = ext::f1402f8760(item);
    const std::int32_t w3 = ext::f1402b7480(item);
    out[0] = static_cast<double>(type);
    out[1] = static_cast<double>(cnt);
    out[2] = static_cast<double>(per);
    if (type == 1) out[3] = static_cast<double>(w3);
    void* list = ext::f1402b74b0(item);
    if (type == 1 && per == 5) {
        for (std::int32_t k = 0; k < cnt; k = k + 1) {
            const double* e = ext::f1402b7560(list, k);
            out[k * per + 10] = e[0];     // 1402b7420
            out[k * per + 1 + 10] = e[1]; // 1402b7430
            out[k * per + 2 + 10] = e[2]; // 1402b7440
            out[k * per + 3 + 10] = e[3]; // 1402b7460
            out[k * per + 4 + 10] = e[4]; // 1402b7450
        }
    }
}

// @src 1402f8ef0  +0x58 item as a function: type, count, per, (type 1) word 3; types 1/2 with
//                 per 1: the ints of the entries from word 10
static void write_item58(void* item, double* out)
{
    const std::int32_t type = ext::f1402b74d0(item);
    const std::int32_t cnt = ext::f14019a060(item);
    const std::int32_t per = ext::f1402f8ce0(item);
    const std::int32_t w3 = ext::f1402b74e0(item);
    out[0] = static_cast<double>(type);
    out[1] = static_cast<double>(cnt);
    out[2] = static_cast<double>(per);
    if (type == 1) out[3] = static_cast<double>(w3);
    void* list = ext::f1402b7510(item);
    if ((type == 2 || type == 1) && per == 1) {
        for (std::int32_t k = 0; k < cnt; k = k + 1) {
            out[k + 10] = static_cast<double>(*ext::f140159a70(list, k));
        }
    }
}

// @src 140175db0  WriteBdryInfoToDB (3D only): record word 0 = type; types > 1 (mesh): surface 0's
//                 node count / first IIBND word / face count / first face (running offsets
//                 advance), word 10 nodes per face (3 for tetrahedra, else 4); types < 3 (rigid
//                 geometry u3c00[0]): points into LLGRY, facets into IIDOD (+ facet ids), surface
//                 lists as functions (words 0x62..0x65); then the movement words (1401912a0) and
//                 words 0xb, 0x2c..0x31 from obj+0x2058..0x2088
void write_boundary_info(LLOBJ_DATA* obj, const std::int32_t* rec_type, double* rec, double* llgry,
                         std::int32_t* iidod, std::int32_t* iifnc, double* llfnc,
                         std::int32_t* /*iibdy*/, const std::int32_t* rec_no)
{
    if (!rt::dimension_is_3(owner_of(obj))) return;
    const std::int32_t surf = 0;
    const std::int32_t npe = reinterpret_cast<model::Cls_140182880*>(at(obj, 0x9e0))->width(); // 140185530
    rec[0] = static_cast<double>(*rec_type);
    rec[0x27] = 0.0;
    rec[5] = 0.0;
    rec[6] = 0.0;
    rec[7] = 0.0;
    rec[8] = 0.0;
    if (1 < *rec_type) {
        std::int32_t bnd = bnd_node_offset(owner_of(obj));
        std::int32_t face = face_offset(owner_of(obj));
        const std::int32_t nn = fld<std::int32_t>(obj, 0xbe0 + surf * 0x30);
        const std::int32_t nf = fld<std::int32_t>(obj, 0xbf0 + surf * 0x30);
        rec[1] = static_cast<double>(nn);
        rec[2] = static_cast<double>(bnd + 1);
        rec[3] = static_cast<double>(nf);
        rec[4] = static_cast<double>(face + 1);
        bnd = bnd + nn;
        face = face + nf;
        set_bnd_node_offset(owner_of(obj), &bnd);
        set_face_offset(owner_of(obj), &face);
        rec[10] = 4.0; // DAT_14051f248
        if (npe == 4) rec[10] = 3.0; // DAT_14051f1c8
    }
    if (*rec_type < 3) {
        const std::int32_t k0 = 0;
        OBJ_GEO3D* g = ext::f140198620(obj, &k0);
        const std::int32_t np = g->node_count();           // 1400034d0
        const double* xyz = ext::f140006410(g);
        const std::int32_t nfc = g->elem_count();         // 140003500
        const std::int32_t* fac = ext::f140005510(g);
        const std::int32_t* ids = ext::f140199fa0(g);
        std::int32_t gp = geo_point_offset(owner_of(obj));
        const std::int32_t ext_code = g->is_extension(); // vtable slot 0x58
        const std::uint32_t is_ext = ext_code == 100 ? 1u : 0u;
        rec[0x27] = static_cast<double>(is_ext);
        rec[5] = static_cast<double>(np);
        rec[6] = static_cast<double>(gp + 1);
        std::memcpy(llgry + (static_cast<std::int32_t>(rec[6]) - 1), xyz,
                    static_cast<std::size_t>(static_cast<std::int64_t>(np * 3) << 3));
        gp = gp + np * 3;
        set_geo_point_offset(owner_of(obj), &gp);
        if (rec[0x27] == 1.0) { // DAT_140509c90
            std::int32_t fo = geo_facet_offset(owner_of(obj));
            rec[7] = static_cast<double>(nfc);
            rec[8] = static_cast<double>(fo + 1);
            std::memcpy(iidod + (static_cast<std::int32_t>(rec[8]) - 1), fac,
                        static_cast<std::size_t>(static_cast<std::int64_t>(nfc << 2) << 2));
            fo = fo + nfc * 4;
            set_geo_facet_offset(owner_of(obj), &fo);
            std::int32_t io = geo_facet_id_offset(owner_of(obj));
            const std::int32_t nobj = n_objects_meshed(owner_of(obj));
            rec[0x61] = static_cast<double>(io + 1 + nobj * 4);
            std::memcpy(iidod + (static_cast<std::int32_t>(rec[0x61]) - 1), ids,
                        static_cast<std::size_t>(static_cast<std::int64_t>(nfc) << 2));
            io = io + nfc;
            set_geo_facet_id_offset(owner_of(obj), &io);
            const char rebuild = 1;
            rec[0x66] = static_cast<double>(ext::f1402fcfe0(g, &rebuild));
            const std::int32_t n50 = count_items50(g);
            if (0 < n50) {
                double number = 0.0;
                std::int32_t pos = 0;
                std::int32_t len = 0;
                char first = 1;
                const std::int32_t k = ext::f14019a080(g);
                for (std::int32_t i = 1; i <= k; i = i + 1) {
                    void* item = ext::f1402fd3e0(g, &i);
                    if (item50_valid(item)) {
                        pos = 0;
                        len = ext::f1402f87a0(item);
                        alloc_function(owner_of(obj), &len, iifnc, &i, &number, &pos);
                        write_item50(item, llfnc + (pos - 1));
                        if (first != '\0') {
                            rec[0x62] = static_cast<double>(static_cast<std::int32_t>(number));
                            first = 0;
                        }
                        if (first == '\0') {
                            rec[99] = static_cast<double>(static_cast<std::int32_t>(number));
                        }
                    }
                }
            }
            const std::int32_t n58 = count_items58(g);
            if (0 < n58) {
                double number = 0.0;
                std::int32_t pos = 0;
                std::int32_t len = 0;
                char first = 1;
                const std::int32_t k = ext::f14019a0a0(g);
                for (std::int32_t i = 1; i <= k; i = i + 1) {
                    void* item = ext::f1402fd740(g, &i);
                    if (item58_valid(item)) {
                        pos = 0;
                        len = ext::f1402f8d30(item);
                        alloc_function(owner_of(obj), &len, iifnc, &i, &number, &pos);
                        write_item58(item, llfnc + (pos - 1));
                        if (first != '\0') {
                            rec[100] = static_cast<double>(static_cast<std::int32_t>(number));
                            first = 0;
                        }
                        if (first == '\0') {
                            rec[0x65] = static_cast<double>(static_cast<std::int32_t>(number));
                        }
                    }
                }
            }
        }
    }
    write_bnd_movement(at(obj, 0x2160), rec_type, rec, iifnc, llfnc, owner_of(obj), rec_no);
    rec[0xb] = fld<double>(obj, 0x2058);
    rec[0x2f] = fld<double>(obj, 0x2060);
    rec[0x30] = fld<double>(obj, 0x2068);
    rec[0x31] = fld<double>(obj, 0x2070);
    rec[0x2c] = fld<double>(obj, 0x2078);
    rec[0x2d] = fld<double>(obj, 0x2080);
    rec[0x2e] = fld<double>(obj, 0x2088);
}

} // namespace def::dbio::g3d
