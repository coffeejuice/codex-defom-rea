// dbread/w1_geo.cpp: 2D curve / edge-list helpers and 3D die point / edge sets DBREAD uses while
// reading the boundary records (packet p-dbread, helper W1).
#include <defdbio/dbread/w1.hpp>

#include <cstdlib>
#include <cstring>
#include <iterator>
#include <list>
#include <new>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/mesh2d/cow_list.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/sdctl/cow.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

// @src 14027e700
bool curve_drop_bad_segments(model::OBJ_GEO2D* c)
{
    std::int32_t bad = 0;
    if (c->n_points < 1 || c->points == nullptr) {
        return false;
    }
    if (c->curve_type != 2) {
        return false;
    }
    const std::int32_t dof = c->dof(); // 1402760e0
    auto* flag = static_cast<char*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(c->n_points))));
    std::memset(flag, 0, static_cast<std::size_t>(static_cast<std::int64_t>(c->n_points)));
    for (std::int32_t k = 0; k < c->n_points; k = k + 1) {
        const std::int32_t t = static_cast<std::int32_t>(c->points[k * dof]);
        if (t != 1 && t != 2) {
            flag[k] = 1;
            bad = bad + 1;
        }
    }
    if (0 < bad) {
        const std::int64_t n = static_cast<std::int64_t>(c->n_points * dof);
        auto* copy = static_cast<double*>(std::malloc(static_cast<std::size_t>(n) * 8)); // overflow -> SIZE_MAX
        std::memcpy(copy, c->points, static_cast<std::size_t>(n << 3));
        std::int32_t kept = 0;
        for (std::int32_t k = 0; k < c->n_points; k = k + 1) {
            if (flag[k] == 0) {
                std::memcpy(c->points + kept * dof, copy + k * dof, static_cast<std::size_t>(static_cast<std::int64_t>(dof) << 3));
                kept = kept + 1;
            }
        }
        c->n_points = kept;
        std::free(copy);
    }
    std::free(flag);
    c->set_flag28(true); // 14027f410
    return 0 < bad;
}

// @src 14027fe20
void geo2d_set_edge_count(model::GEO2D_AND_MESH2D* g, const std::int32_t* n)
{
    const std::int32_t have = static_cast<std::int32_t>(g->edges.size()); // 140159190
    if (have < *n) {
        while (static_cast<std::int32_t>(g->edges.size()) < *n) {
            g->add_edge_curve(); // 14027fce0
        }
    } else if (*n < have) {
        model::CowVector<std::int32_t> idx; // 140159ad0
        for (std::int32_t k = *n; k < have; k = k + 1) {
            idx.insert(idx.mutable_end(), 1, k); // 140220810
        }
        std::int32_t cnt = static_cast<std::int32_t>(idx.size()); // 1400030b0
        geo2d_remove_edges(g, &idx.mutable_at(0), &cnt);          // 140005100, 140280290
    } // ~CowVector 140159b40
    const char on = 1;
    model::geo2d_set_b8(g, &on); // 1402847d0
}

// The local list of 140280290 is one more CowList instantiation (element: an EdgeList iterator,
// i.e. a node pointer); the port uses a std::list of std::list iterators, which is what
// CowList<T> holds:
// @src 1402859a0  E handle ctor (operator new(0x18), body ctor 140287820)
// @src 140285a10  E handle dtor (release 1402863e0)
// @src 140285a30  E begin (detach, iterator(head->next))
// @src 140285a70  E end (detach, iterator(head))
// @src 140285ab0  E push_back (detach, end(), insert 140287950)
// @src 140285b00  E iterator()
// @src 140285b20  E operator!= (Ghidra: CFileTimeSpan::operator!=)
// @src 140285b60  E operator* (&node->value)
// @src 140285b80  E operator++(int)
// @src 140286020  E detach (when refs > 1: 140286aa0)
// @src 140286050  E iterator(node)
// @src 140286070  E iterator copy
// @src 1402863e0  E release (refs--, at 0 the body deleting dtor 140286540)
// @src 140286540  E body scalar deleting dtor
// @src 140286aa0  E detach: refs--, new body copied from the old one (140288730)
// @src 140287820  E body ctor (refs = 1, sentinel 140287e30, size 0)
// @src 1402878d0  E body dtor (free every node, then the sentinel)
// @src 140287950  E body insert before an iterator, size++
// @src 140287e00  E node ctor (copy the value)
// @src 140287e30  E sentinel ctor
// @src 140288730  E body copy ctor
using EdgeIterList = model::CowList<std::list<model::IEdge*>::iterator>;

// @src 140280290
bool geo2d_remove_edges(model::GEO2D_AND_MESH2D* g, std::int32_t* idx, const std::int32_t* n)
{
    EdgeIterList doomed;
    for (std::int32_t k = 0; k < *n; k = k + 1) {
        const std::int32_t pos = idx[k];
        // 140284ff0: edge list at(pos) as an iterator (detaching, then walking pos nodes)
        auto& edges = g->edges.mutable_items();
        auto it = edges.begin();
        for (std::int32_t s = 0; s < pos; ++s) ++it;
        model::IEdge* e = *it; // 140285820
        auto* d = dynamic_cast<model::DEF_EDGE*>(e);
        if (d != nullptr) {
            model::geo_kw::edge_delete_curve(d); // 14041c4c0
            // free() of the edge object: storage released without running its destructor
            ::operator delete(static_cast<void*>(d));
        }
        doomed.push_back(it);
    }
    auto& items = doomed.mutable_items();
    for (auto i = items.begin(); i != items.end(); i++) {
        // 140284f90: edge list erase (make_unique 140285e20, body erase 1402870e0)
        g->edges.mutable_items().erase(*i);
    }
    model::geo2d_renumber_edges(g); // 140284220
    const char on = 1;
    model::geo2d_set_b8(g, &on);
    return true;
}

// @src 1402f8850
void die_point_set_from_values(model::geo_kw::DiePointSet* s, double* v)
{
    namespace gk = model::geo_kw;
    std::int32_t type = static_cast<std::int32_t>(v[0]);
    const double count = v[1];
    const std::int32_t ncol = static_cast<std::int32_t>(v[2]);
    std::int32_t extra = static_cast<std::int32_t>(v[3]);
    s->set_type(&type); // 1402fe8e0
    if (type == 1) {
        s->set_extra(&extra); // 1402fe900
    }
    model::CowList<gk::DiePoint>& pts = s->points; // 1402b74b0
    gk::points_clear(pts);                         // 1402ff5f0
    if (type == 1 && ncol == 5) {
        for (std::int32_t k = 0; k < static_cast<std::int32_t>(count); k = k + 1) {
            gk::DiePoint p; // 1402b7860: zeroed
            const double x = v[k * ncol + 10];
            const double y = v[(k * ncol + 1) + 10];
            const double z = v[(k * ncol + 2) + 10];
            const double p4 = v[(k * ncol + 3) + 10];
            const double p5 = v[(k * ncol + 4) + 10];
            p.set_xyz(x, y, z); // 1402fe850
            p.set_p4(p4);       // 1402fe8a0
            p.set_p5(p5);       // 1402fe8c0
            gk::points_push_back(pts, p); // 1402fed80
        }
    }
}

// @src 1402f8de0
void die_edge_set_from_values(model::geo_kw::DieEdgeSet* s, double* v)
{
    std::int32_t type = static_cast<std::int32_t>(v[0]);
    const double count = v[1];
    const std::int32_t ncol = static_cast<std::int32_t>(v[2]);
    std::int32_t extra = static_cast<std::int32_t>(v[3]);
    s->set_type(&type); // 1402fe930
    if (type == 1) {
        s->set_extra(&extra); // 1402fe950
    }
    const double* data = v + 10;
    model::CowList<std::int32_t>& nodes = s->nodes; // 1402b7510
    nodes.clear();                                  // 14015a620
    if ((type == 2 || type == 1) && ncol == 1) {
        for (std::int32_t k = 0; k < static_cast<std::int32_t>(count); k = k + 1) {
            const std::int32_t node = static_cast<std::int32_t>(data[k * ncol]);
            nodes.push_back(node); // 140220700
        }
    }
}

// @src 1402f9af0
void geo3d_zero_elem_ids(void* geo3d)
{
    auto* g = static_cast<model::OBJ_GEO3D*>(geo3d);
    const std::int32_t n = g->elem_count(); // 140003500
    if (0 < n) {
        for (std::int32_t k = 0; k < n; k = k + 1) {
            g->elem_ids.mutable_at(static_cast<std::size_t>(k)) = 0; // 140005100
        }
    }
}

} // namespace def::dbio::dbread
