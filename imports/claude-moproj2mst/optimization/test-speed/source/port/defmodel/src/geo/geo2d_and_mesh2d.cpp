// GEO2D_AND_MESH2D: ctors, dtor, reset/clear, edge accessors and the copy (p1-07).
// Layout: defmodel/layout/geo2d_and_mesh2d.hpp. The topology objects (p1-11) and the other
// GEO2D_AND_MESH2D methods (p1-04/p1-08) are reached through defmodel/geo/pending.hpp; the list
// sizes the original reads through p1-04/p1-06 helpers (140159190, 140158f30, 140256780,
// 1402567a0, 140158f10: body->size) are read here directly.
#include <defmodel/layout/geo2d_and_mesh2d.hpp>

#include <cstdint>
#include <cstdlib>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/obj_geo2d.hpp>

namespace def::model {

using geo::op_new;

namespace {

inline std::size_t idx(std::int32_t i) { return static_cast<std::size_t>(static_cast<std::int64_t>(i)); }

// detach, free every element (the objects' destructors do not run), clear
template <class T>
void free_all(CowList<T>& list)
{
    for (auto it = list.mutable_begin(); it != list.mutable_end(); it++) {
        void* p = *it;
        std::free(p);
    }
}

// 140284960: the geometry as _OBJ_GEO2D, 0 unless is_extension() == 0 (slot 11)
OBJ_GEO2D* as_geo2d(OBJ_GEO_BASE* g)
{
    if (g->is_extension() != 0) {
        g = nullptr;
    }
    return static_cast<OBJ_GEO2D*>(g);
}

} // namespace

// @src 14027f6e0  ctor: C3dCrossSection ctor, vptr, faces (140285080), loops (140284c20),
// edges (140284e30), vertices (140285290), mesh = 0, matrix (140220540), uc0 (140159860),
// reset (14027f910)
GEO2D_AND_MESH2D::GEO2D_AND_MESH2D()
{
    mesh = nullptr;
    pending::fun_140220540(matrix);
    reset();
}

// @src 14027f7a0  copy ctor: C3dCrossSection ctor 140284940, the same member ctors, reset,
// copy_from(other) (140280490)
GEO2D_AND_MESH2D::GEO2D_AND_MESH2D(const GEO2D_AND_MESH2D& other) : C3dCrossSection(other)
{
    mesh = nullptr;
    pending::fun_140220540(matrix);
    reset();
    copy_from(const_cast<GEO2D_AND_MESH2D&>(other));
}

// @src 14027f870  dtor: clear_all, uc0 dtor (140159930), matrix dtor (140220560), the four list
// dtors (140285300, 140284ea0, 140284c90, 1402850f0), C3dCrossSection dtor (140284890)
GEO2D_AND_MESH2D::~GEO2D_AND_MESH2D()
{
    clear_all();
    pending::fun_140220560(matrix);
}

// @src 14027f910  reset
std::int32_t GEO2D_AND_MESH2D::reset()
{
    clear_all();
    mesh = nullptr;
    u30 = 0;
    u31 = 0;
    pending::fun_140220570(matrix);
    ub8 = 1;
    return 0;
}

// @src 14027f980  clear_all
void GEO2D_AND_MESH2D::clear_all()
{
    delete_mesh();
    clear_topology();
}

// @src 14027f9b0  delete the mesh
void GEO2D_AND_MESH2D::delete_mesh()
{
    if (mesh != nullptr) {
        OBJ_MESH2D* m = mesh;
        if (m != nullptr) {
            pending::delete_result_set(m); // (**vtable)(mesh, 1)
        }
    }
    mesh = nullptr;
}

// @src 14027fa30  free the topology objects, clear the lists (faces 140286880, loops 140286660,
// edges 140286770, vertices 140286990), then 1402847d0(this, true)
void GEO2D_AND_MESH2D::clear_topology()
{
    free_all(faces);
    faces.clear();
    free_all(loops);
    loops.clear();
    free_all(edges);
    edges.clear();
    free_all(vertices);
    vertices.clear();
    bool t = true;
    pending::fun_1402847d0(this, &t);
}

// @src 14027fce0  new curve on a new edge
OBJ_GEO2D* GEO2D_AND_MESH2D::add_edge_curve()
{
    OBJ_GEO2D* g = new OBJ_GEO2D(); // operator new(0x30), ctor 140275cd0
    if (g == nullptr) {
        return nullptr;
    }
    void* mem = op_new(0x38);
    DEF_EDGE* e = mem == nullptr ? nullptr : pending::fun_14041c300(mem);
    pending::fun_14041c4a0(e, g);
    std::int32_t id = static_cast<std::int32_t>(edges.size()); // 140159190
    id = id + 1;
    pending::fun_14041c5f0(e, &id);
    IEdge* v = reinterpret_cast<IEdge*>(e); // IEdge is _DEF_EDGE's base at offset 0
    edges.push_back(v);                     // 140284f40
    bool t = true;
    pending::fun_1402847d0(this, &t);
    return g;
}

// @src 14027ffe0  curve of edge i (at(i) const 140285040, no detach)
OBJ_GEO2D* GEO2D_AND_MESH2D::edge_curve(std::int32_t i)
{
    std::int32_t n = 0;
    n = static_cast<std::int32_t>(edges.size());
    OBJ_GEO2D* r;
    if (n < 1) {
        r = nullptr;
    } else if (i < 0) {
        r = nullptr;
    } else if (i < n) {
        IEdge* ie = edges.at(idx(i));
        DEF_EDGE* e = pending::cast_def_edge(ie);
        if (e == nullptr) {
            r = nullptr;
        } else {
            OBJ_GEO_BASE* g = pending::fun_140284990(e);
            r = as_geo2d(g);
        }
    } else {
        r = nullptr;
    }
    return r;
}

// @src 1402800a0  edge i (at(i) 140284ff0, detaching)
DEF_EDGE* GEO2D_AND_MESH2D::edge_at(std::int32_t i)
{
    std::int32_t n = 0;
    n = static_cast<std::int32_t>(edges.size());
    DEF_EDGE* r;
    if (n < 1) {
        r = nullptr;
    } else if (i < 0) {
        r = nullptr;
    } else if (i < n) {
        IEdge* ie = edges.mutable_at(idx(i));
        r = pending::cast_def_edge(ie);
    } else {
        r = nullptr;
    }
    return r;
}

// @src 140280490  copy
void GEO2D_AND_MESH2D::copy_from(GEO2D_AND_MESH2D& other)
{
    delete_mesh();
    if (other.mesh != nullptr) {
        void* mem = op_new(0x1700);
        mesh = mem == nullptr ? nullptr : static_cast<OBJ_MESH2D*>(pending::fun_1403ddf20(mem));
        pending::fun_1403de460(mesh, other.mesh);
    }
    clear_topology();
    std::int32_t n = static_cast<std::int32_t>(other.edges.size()); // 140159190
    for (std::int32_t i = 0; i < n; i = i + 1) {
        OBJ_GEO2D* c = other.edge_curve(i);
        if (c != nullptr) {
            OBJ_GEO2D* g = add_edge_curve();
            g->assign(c); // 140276140
        }
    }
    bool has_faces = pending::fun_140158ec0(&other);
    if (has_faces) {
        copy_topology(other);
    }
    std::uint8_t b = other.u30; // 1402849d0
    u30 = b;                    // 1402849b0
    pending::fun_140220670(matrix, other.matrix); // matrix: 1402849a0
    ub8 = other.ub8;
}

// @src 140280760  rebuild faces, loops and vertices with their links
void GEO2D_AND_MESH2D::copy_topology(GEO2D_AND_MESH2D& other)
{
    std::int32_t n_faces = static_cast<std::int32_t>(other.faces.size());     // 140158f30
    std::int32_t n_loops = static_cast<std::int32_t>(other.loops.size());     // 140256780
    std::int32_t n_edges = static_cast<std::int32_t>(edges.size());           // 140159190 (this)
    std::int32_t n_verts = static_cast<std::int32_t>(other.vertices.size());  // 1402567a0
    if (n_edges == 0) {
        return;
    }
    for (std::int32_t v = 0; v < n_verts; v = v + 1) {
        pending::fun_140283a30(this);
    }
    for (std::int32_t l = 0; l < n_loops; l = l + 1) {
        pending::fun_140283980(this);
    }
    for (std::int32_t f = 0; f < n_faces; f = f + 1) {
        pending::fun_1402838d0(this);
    }
    pending::fun_140284190(this);
    pending::fun_140284220(this);
    pending::fun_1402842b0(this);
    pending::fun_140284340(this);

    // vertices: position, flag, incident edges
    for (std::int32_t v = 0; v < n_verts; v = v + 1) {
        DEF_VERTEX* sv = pending::fun_1402840e0(&other, &v);
        if (sv != nullptr) {
            DEF_VERTEX* dv = pending::fun_1402840e0(this, &v);
            void* pos = pending::fun_14041d000(sv);
            pending::fun_14041cf80(dv, pos);
            bool b = pending::fun_140284a90(sv);
            pending::fun_140284a70(dv, b);
            std::int32_t ne = pending::vertex_edge_count(sv); // vtable + 0x10
            for (std::int32_t k = 0; k < ne; k = k + 1) {
                IEdge* ie = pending::fun_14041d020(sv, &k);
                DEF_EDGE* se = pending::cast_def_edge(ie);
                if (se != nullptr) {
                    std::int32_t id = pending::fun_14041c610(se);
                    DEF_EDGE* de = edge_at(id + -1);
                    pending::fun_14041d0b0(dv, de);
                }
            }
        }
    }

    // edges: end vertices, loops, flags
    for (std::int32_t k = 0; k < n_edges; k = k + 1) {
        DEF_EDGE* se = other.edge_at(k);
        DEF_EDGE* de = edge_at(k);
        std::int32_t end0 = 0;
        DEF_VERTEX* sv = pending::cast_def_vertex(pending::fun_14041c590(se, &end0));
        if (sv != nullptr) {
            std::int32_t id0 = pending::fun_140256770(sv);
            id0 = id0 + -1;
            pending::fun_140256730(de, pending::fun_1402840e0(this, &id0));
            std::int32_t end1 = 1;
            sv = pending::cast_def_vertex(pending::fun_14041c590(se, &end1));
            if (sv != nullptr) {
                std::int32_t id1 = pending::fun_140256770(sv);
                id1 = id1 + -1;
                pending::fun_140256750(de, pending::fun_1402840e0(this, &id1));
                std::int32_t nl = pending::fun_14041c5c0(se);
                for (std::int32_t l = 0; l < nl; l = l + 1) {
                    ILoop* il = pending::fun_14041c630(se, &l);
                    DEF_LOOP* sl = pending::cast_def_loop(il);
                    if (sl != nullptr) {
                        std::int32_t lid = pending::fun_140256720(sl);
                        lid = lid + -1;
                        DEF_LOOP* dl = pending::fun_140284030(this, &lid);
                        pending::fun_14041c6c0(de, dl);
                    }
                }
                bool b = pending::fun_14041ccd0(se);
                pending::fun_14041cd40(de, b);
                b = pending::fun_140284a20(se);
                pending::fun_140284a50(de, b);
                b = pending::fun_140284a10(se);
                pending::fun_140284a30(de, b);
                std::uint8_t c = pending::fun_14041cc10(se);
                pending::fun_14041cc70(de, &c);
            }
        }
    }

    // loops: edges, faces, flag
    for (std::int32_t l = 0; l < n_loops; l = l + 1) {
        DEF_LOOP* sl = pending::fun_140284030(&other, &l);
        DEF_LOOP* dl;
        if (sl != nullptr && (dl = pending::fun_140284030(this, &l)) != nullptr) {
            std::int32_t ne = pending::loop_edge_count(sl); // vtable + 0x18
            for (std::int32_t k = 0; k < ne; k = k + 1) {
                IEdge* ie = pending::fun_140412150(sl, &k);
                DEF_EDGE* se = pending::cast_def_edge(ie);
                if (se != nullptr) {
                    std::int32_t id = pending::fun_14041c610(se);
                    DEF_EDGE* de = edge_at(id + -1);
                    pending::fun_1404121e0(dl, de);
                }
            }
            std::int32_t nf = pending::fun_1404125e0(sl);
            for (std::int32_t f = 0; f < nf; f = f + 1) {
                IFace* iface = pending::fun_1404123b0(sl, &f);
                DEF_FACE* sf = dynamic_cast<DEF_FACE*>(iface);
                if (sf != nullptr) {
                    std::int32_t fid = sf->id; // 140256710
                    fid = fid + -1;
                    DEF_FACE* df = pending::fun_140283f80(this, &fid);
                    pending::fun_140412440(dl, df);
                }
            }
            bool b = pending::fun_140284a00(sl);
            pending::fun_1402849e0(dl, b);
        }
    }

    // faces: u40, flags, loops
    for (std::int32_t f = 0; f < n_faces; f = f + 1) {
        DEF_FACE* sf = pending::fun_140283f80(&other, &f);
        DEF_FACE* df;
        if (sf != nullptr && (df = pending::fun_140283f80(this, &f)) != nullptr) {
            std::int32_t v = sf->u40_value();
            df->set_u40(&v);
            df->set_flag18(sf->flag18());
            df->set_flag19(sf->flag19());
            std::int32_t nl = static_cast<std::int32_t>(sf->loops.size()); // 140158f10
            for (std::int32_t l = 0; l < nl; l = l + 1) {
                ILoop* il = sf->loop_at(&l); // vtable + 0x60
                DEF_LOOP* sl = pending::cast_def_loop(il);
                if (sl != nullptr) {
                    std::int32_t lid = pending::fun_140256720(sl);
                    lid = lid + -1;
                    DEF_LOOP* dl = pending::fun_140284030(this, &lid);
                    df->add_loop(reinterpret_cast<ILoop*>(dl)); // ILoop is _DEF_LOOP's base at 0
                }
            }
        }
    }
}

} // namespace def::model
