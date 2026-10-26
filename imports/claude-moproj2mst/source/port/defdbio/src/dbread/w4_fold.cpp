// dbread/w4_fold.cpp: ReadObjFoldInfoFromDB (1401d25d0) and the 3D fold mesh reader 14016d300
// (helper W4 of packet p-dbread; docs/re/dbmap/objarrays.md section 7, object.md FLDGEO/FLWNET).
// The GENDB mirror is gendb3d/fold.cpp (14016da70).
#include <defdbio/dbread/w4.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w2.hpp>

#include <cstdint>
#include <cstdlib>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/port.hpp>

#include "dbread/w4_access.hpp"

namespace def::dbio::dbread {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using model::OBJ_GEO3D;
using namespace w4d;

namespace {
inline OBJ_GEO3D* geo_at(LLOBJ_DATA* obj, std::size_t off) { return reinterpret_cast<OBJ_GEO3D*>(at(obj, off)); }
} // namespace

// @src 1401cec20  when *n exceeds the capacity +0x1b74: free +0x1b78, capacity = *n,
//                 +0x1b78 = calloc(capacity, 8)
void step_reserve_a(DB_STEP_DATA* m, const std::int32_t* n)
{
    if (m->u1b74 < *n) {
        if (m->u1b78 != nullptr) {
            std::free(m->u1b78);
        }
        m->u1b78 = nullptr;
        m->u1b74 = *n;
        m->u1b78 = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(m->u1b74)), 8);
    }
}

// @src 1402fa100  geo = the first *first_node - 1 nodes and *first_face - 1 faces of src, net = the
//                 rest; net's node numbers lose the node count of geo (4th node: abs first when
//                 *negate). src is read through 140005e70 / 140005100 (detaching accessors).
void split_fold_mesh(OBJ_GEO3D* src, OBJ_GEO3D* geo, OBJ_GEO3D* net, const std::int32_t* first_node,
                     const std::int32_t* first_face, const char* negate)
{
    std::int32_t net_nodes = 0;          // local_20
    std::int32_t net_faces = 0;          // local_4c
    std::int32_t geo_nodes = *first_node - 1; // local_3c
    std::int32_t geo_faces = *first_face - 1; // local_40
    const std::int32_t sn = src->node_count();
    const std::int32_t se = src->elem_count();
    net_nodes = sn - geo_nodes;
    net_faces = se - geo_faces;
    gendb::geo3d_resize(geo, &geo_nodes, &geo_faces); // 1402f9780
    gendb::geo3d_resize(net, &net_nodes, &net_faces);
    double* gx = model::geo_kw::geo3d_coords(geo);           // 140006410
    std::int32_t* ge = model::geo_kw::geo3d_elems(geo);      // 140005510
    double* nx = model::geo_kw::geo3d_coords(net);
    (void)model::geo_kw::geo3d_elems(net);                   // 140005510 (result unused)
    for (std::int32_t k = 0; k < geo->node_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 3; c = c + 1) {
            gx[k * 3 + c] = src->coords.mutable_at(static_cast<std::size_t>(k * 3 + c));
        }
    }
    for (std::int32_t k = 0; k < geo->elem_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 4; c = c + 1) {
            ge[c + k * 4] = src->elems.mutable_at(static_cast<std::size_t>(c + k * 4));
        }
    }
    for (std::int32_t k = 0; k < net->node_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 3; c = c + 1) {
            nx[k * 3 + c] = src->coords.mutable_at(static_cast<std::size_t>((geo_nodes + k) * 3 + c));
        }
    }
    for (std::int32_t k = 0; k < net->elem_count(); k = k + 1) {
        for (std::int32_t c = 0; c < 4; c = c + 1) {
            const std::int32_t v = src->elems.mutable_at(static_cast<std::size_t>(c + (geo_faces + k) * 4));
            std::int32_t r = v - geo->node_count();
            if (c == 3 && *negate != '\0') {
                const std::int32_t a = std::abs(src->elems.mutable_at(static_cast<std::size_t>((geo_faces + k) * 4 + 3))); // 140484ab8
                r = a - geo->node_count();
            }
            net->elems.mutable_at(static_cast<std::size_t>(c + k * 4)) = r;
        }
    }
}

// @src 14016d300  IIOBJ words 15/16 first/last fold node (LLFLD rows of 3 reals), 17/18 first/last
//                 face (IIFLD rows of 4 ints); the mesh is read into a temporary, faces with a
//                 positive 4th node are FLDGEO, negative FLWNET (both kinds: split at the lowest
//                 node of the first FLWNET face)
void read_fold_info(LLOBJ_DATA* obj, const std::int32_t* iiobj, std::int32_t* mar, double* a,
                    const std::int32_t* mar_tab, const std::int32_t* a_tab)
{
    std::int32_t a_off = 0;         // local_5c
    std::int32_t* alloc = nullptr;  // local_38
    const std::int32_t* faces = nullptr; // local_30
    const std::int32_t n0 = iiobj[0x3c / 4];
    const std::int32_t n1 = iiobj[0x40 / 4];
    const std::int32_t f0 = iiobj[0x44 / 4];
    const std::int32_t f1 = iiobj[0x48 / 4];
    std::int32_t nnodes = (n1 - n0) + 1;
    std::int32_t nfaces = (f1 - f0) + 1;
    geo_at(obj, 0x3328)->clear();   // 1402f9340
    geo_at(obj, 0x3458)->clear();
    if (0 < n0 && 0 < f0 && 0 < nnodes && 0 < nfaces && 0 < a_tab[0x268 / 4] && 0 < mar_tab[0x174 / 4]) {
        OBJ_GEO3D tmp;                                          // 1402f9020 .. 1402f91f0
        model::geo_kw::geo3d_size(&tmp, &nnodes, &nfaces);      // 1402fcd90
        if (obj->owner == nullptr || model::names_up_to_date(obj->owner)) {
            a_off = a_tab[0x38 / 4] - 1;
        } else {
            a_off = 0;
        }
        const double* xyz = a + (a_off + (n0 - 1) * 3);
        const std::int32_t cnt = tmp.node_count();
        double* dst = model::geo_kw::geo3d_coords(&tmp);
        for (std::int32_t k = 0; k < cnt * 3; k = k + 1) {
            dst[k] = xyz[k];
        }
        if (!gendb::step_fun_1401d2bb0(obj->owner)) {
            // 14016d5f0: 1401cf040(owner, 0x17) reads IIFLD on demand; 1401d2bb0 always returns 1
            DEF_NOT_PORTED(0x1401cf040);
        } else {
            faces = mar + (mar_tab[0x5c / 4] + f0 * 4 + -5);
        }
        std::int32_t n_geo = 0;     // local_54
        std::int32_t n_net = 0;     // local_70
        std::int32_t min_node = 0;  // local_60
        const std::int32_t base = n0 - 1; // local_84
        const std::int32_t ne = tmp.elem_count();
        std::int32_t* e = model::geo_kw::geo3d_elems(&tmp);
        for (std::int32_t k = 0; k < ne; k = k + 1) {
            e[k << 2] = faces[k << 2] - base;
            e[k * 4 + 1] = faces[k * 4 + 1] - base;
            e[k * 4 + 2] = faces[k * 4 + 2] - base;
            const std::int32_t v = faces[k * 4 + 3];
            if (v < 1) {
                e[k * 4 + 3] = v + base;
            } else {
                e[k * 4 + 3] = v - base;
            }
            const std::int32_t t = e[k * 4 + 3];
            if (t < 1) {
                if (t < 0) {
                    n_net = n_net + 1;
                    if (n_net == 1) {
                        min_node = -t;
                    }
                    for (std::int32_t c = 0; c < 3; c = c + 1) {
                        if (e[c + k * 4] < min_node) {
                            min_node = e[c + k * 4];
                        }
                    }
                }
            } else {
                n_geo = n_geo + 1;
            }
        }
        if (!gendb::step_fun_1401d2bb0(obj->owner)) {
            if (alloc != nullptr) {
                std::free(alloc);
            }
            alloc = nullptr;
        }
        if (n_geo == 0 && n_net == 0) {
            return; // tmp destroyed (1402f91f0)
        }
        if (n_geo < 1 || n_net != 0) {
            if (n_geo == 0 && 0 < n_net) {
                geo_at(obj, 0x3458)->assign(&tmp);              // 140199c20 -> 1402f9b60
            } else {
                const char negate = 1;
                const std::int32_t first_face = n_geo + 1;
                split_fold_mesh(&tmp, geo_at(obj, 0x3328), geo_at(obj, 0x3458), &min_node, &first_face, &negate);
            }
        } else {
            geo_at(obj, 0x3328)->assign(&tmp);                  // 140199c20 -> 1402f9b60
        }
        if (model::geo_kw::surface_has_mesh(geo_at(obj, 0x3458))) { // 1402f9360
            model::geo_kw::geo3d_fun_1402fbb30(geo_at(obj, 0x3458));
        }
    }
}

// @src 1401d25d0  only when m+0x1734 (3D step header fold flag) != 0; IIOBJ rows from MAR slot 3
//                 (m+0x1b9c start, m+0x1cb4 width). The on-demand read below 1401d2b70 == 0 is
//                 unreachable in this build (1401d2b70 always returns 1).
void read_obj_fold_info(DB_STEP_DATA* m)
{
    const std::int32_t iiobj_start = m->u1b90[3];          // +0x1b9c
    std::int32_t* mar = static_cast<std::int32_t*>(m->u1b88);
    if (m->u1734 != 0) {
        if (!model::names_up_to_date(m)) {
            if (fi(m, 0x26fc) != 0) {
                return;
            }
            std::int32_t n = (m->u1dc0[15] - m->u1dc0[14]) + 10; // +0x1dfc - +0x1df8
            step_reserve_a(m, &n);
            std::int32_t what = 0xe;
            read_single_a_array_from_file(m, &what); // 1401cecc0
        }
        for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
            auto* obj = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1)); // 1401d6ed0
            if (fi(obj, 0xc) != 7) {
                read_fold_info(obj, mar + (iiobj_start + -1) + m->u1b90[73] * i, // +0x1cb4
                               static_cast<std::int32_t*>(m->u1b88), static_cast<double*>(m->u1b78), m->u1b90,
                               m->u1dc0);
            }
        }
    }
}

} // namespace def::dbio::dbread
