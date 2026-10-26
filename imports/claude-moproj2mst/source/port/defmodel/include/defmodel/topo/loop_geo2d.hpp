// defmodel/topo/loop_geo2d.hpp: _DEF_LOOP -> closed _OBJ_GEO2D curve (1404126d0) and the loop
// comparison of slot 4 (140412610), written over the curve type G so that tier-1 tests can run
// them with a recording curve while _OBJ_GEO2D (p1-07) is not ported. DEF_LOOP::to_geo2d and
// DEF_LOOP::inside_bbox_of instantiate them with OBJ_GEO2D.
//
// G must derive from OBJ_GEO_BASE (the edges' curves are dynamic_cast to it) and provide:
// curve_type (int at +0x10), clear_all(), copy_from(OBJ_GEO_BASE*) (virtual slot 4),
// reverse() (virtual slot 5), set_curve_type(int), fun_1402765e0(), append(G*), is_ccw(),
// contains_bbox(G*).
#pragma once

#include <cstdint>

#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/topo/geo_fallback.hpp>

namespace def::model::topo {

template <class G>
G* edge_curve(IEdge* e) {
    // __RTDynamicCast(IEdge -> _DEF_EDGE), then 140284990, then (OBJ_GEO_BASE -> _OBJ_GEO2D)
    return dynamic_cast<G*>(static_cast<DEF_EDGE*>(e)->get_geo());
}

inline DEF_EDGE* as_edge(IEdge* e) { return e == nullptr ? nullptr : dynamic_cast<DEF_EDGE*>(e); }

inline IVertex* vtx(DEF_EDGE* e, std::int32_t i) { return e->vertex(&i); }

// @src 1404126d0  build the closed curve of the loop in out (orders the edges first)
template <class G>
void loop_to_geo2d(DEF_LOOP* loop, G* out) {
    G tmp; // 140275cd0, destroyed by 140275e00 on every exit
    std::int32_t n = loop->edge_count(); // slot 3
    loop->order_edges();                 // 140412da0, result unused
    out->clear_all();                    // 140275e90
    if (n < 1) return;

    std::int32_t i0 = 0;
    DEF_EDGE* e = as_edge(loop->edge_at(&i0));
    G* first = edge_curve<G>(e);
    const std::int32_t first_type = first->curve_type; // first curve's +0x10
    if (first_type == 1) {
        // every curve to type 2 (1402763d0), then the first edge again
        for (std::int32_t i = 0; i < n; ++i) {
            e = as_edge(loop->edge_at(&i));
            if (e != nullptr) edge_curve<G>(e)->set_curve_type(2);
        }
        std::int32_t z = 0;
        e = as_edge(loop->edge_at(&z));
        (void)edge_curve<G>(e); // local_98, unused
    }
    out->copy_from(first); // slot 4

    if (n == 1) {
        if (!out->is_ccw()) out->reverse(); // 1402789f0, slot 5
    } else {
        IVertex* a = vtx(e, 0); // first edge's ends
        IVertex* b = vtx(e, 1);
        IVertex* cur = nullptr;
        std::int32_t i1 = 1;
        e = as_edge(loop->edge_at(&i1));
        if (vtx(e, 0) == a || vtx(e, 1) == a) {
            // the second edge continues from the first edge's start: walk from there, reversed
            cur = a;
            out->reverse(); // slot 5
        } else if (vtx(e, 0) == b || vtx(e, 1) == b) {
            cur = b;
        }
        for (std::int32_t i = 1; i < n; ++i) {
            e = as_edge(loop->edge_at(&i));
            if (e == nullptr) continue;
            G* c = edge_curve<G>(e);
            if (vtx(e, 0) == cur) {
                out->append(c);   // 140276e30
                cur = vtx(e, 1);
            } else if (vtx(e, 1) == cur) {
                tmp.clear_all();   // 140275e90
                tmp.G::copy_from(c); // 140276230, direct call
                tmp.G::reverse();  // 1402567c0, direct call
                out->append(&tmp); // 140276e30
                cur = vtx(e, 0);
            }
        }
    }

    if (first_type == 1) {
        for (std::int32_t i = 0; i < n; ++i) {
            e = as_edge(loop->edge_at(&i));
            if (e != nullptr) edge_curve<G>(e)->set_curve_type(1);
        }
        out->set_curve_type(1);
    }
    if (!out->is_ccw()) out->reverse(); // slot 5
    if (out->curve_type == 1) out->fun_1402765e0();
}

// @src 140412610  slot 4: the box of loop a's curve lies inside that of loop b's curve
template <class G>
bool loop_inside_bbox(DEF_LOOP* a, ILoop* b) {
    G ca; // 140275cd0
    G cb;
    loop_to_geo2d<G>(a, &ca);
    loop_to_geo2d<G>(b == nullptr ? nullptr : dynamic_cast<DEF_LOOP*>(b), &cb);
    return ca.contains_bbox(&cb); // 140278ed0
}

} // namespace def::model::topo
