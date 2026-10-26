// defcmd/objpos_centre.hpp: the own centre of OBJPOS mode 6 (3D volume centroid, 2D box centre),
// the bounding boxes and the WPAXIS lookup of modes 13 / 14, and the object tests behind
// usable() / has_die_geometry() (objpos.hpp). Packet p67-1.
// Specs: docs/re/commands/OBJPOS.md 4, 7.4-7.6, 8.5; OBJPOS_geo_kernels.md 2 and 4.4.
// Decompiles / listings: deform-rea-work/out/p67/objpos.c, objpos-geo/geo.c, geo-kernels/,
// out/p671/centre/L_mp.txt (FnList listing used for every floating-point order below).
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
class OBJ_GEO3D;
class GEO2D_AND_MESH2D;
} // namespace def::model

namespace def::cmd::objpos {

// ---- 3D own centre (centre3d.cpp) ----
// 1401fe7d0: surface mass properties of geometry 0 (a temporary surface built from the mesh when
// the object has a mesh and no rigid geometry); c = CENTER. axis may be null. The binary also
// writes AXIS and returns (ok == 0), both of which depend on the AXIS part of 1403e8510 (not
// ported, see centre_ftn.cpp); the port writes axis = 0 and returns true once the mass-property
// routine ran (false without mesh and geometry, or without geometry 0). No caller uses either
// (OBJPOS 1401ff360, drop 1401a7d60).
bool centre_3d(model::LLOBJ_DATA* o, double* c, double* axis);
// 1401fc730: geometry from the mesh. 3D: temporary surface 0 from the boundary faces of the mesh
// (*compact: drop the unused nodes, 1402fc1b0), surface flag +0x3990 set. 2D (surface2d.cpp):
// 1401fcd50 when some element repeats a node (1401fcc50), else 1401fd240.
void fun_1401fc730(model::LLOBJ_DATA* o, char* compact);
// the 3D path of 1401fc730 (centre3d.cpp; also the 3D branch of 1401fcd50, the same code inlined)
void surface_from_mesh_3d(model::LLOBJ_DATA* o, char* compact);
// 1401ffed0: clear every surface (count +0x3994), then count = 0 and flag +0x3990 = 0
void fun_1401ffed0(model::LLOBJ_DATA* o);
// 1402fa880: append the points and faces of s to u (face node numbers shifted by u's old point
// count; face flags copied)
void fun_1402fa880(model::OBJ_GEO3D* u, model::OBJ_GEO3D* s);
// 1402f9780: 1402f97c0(np) then 1402f9810(nf)
void fun_1402f9780(model::OBJ_GEO3D* g, const std::int32_t* np, const std::int32_t* nf);
// 1402f97c0: point array resized to n points (new ones 0)
void fun_1402f97c0(model::OBJ_GEO3D* g, const std::int32_t* n);
// 1402f9810: face array, face flags and face ids resized to n faces (new ones 0)
void fun_1402f9810(model::OBJ_GEO3D* g, const std::int32_t* n);
// 1402fa940: copy s behind the first *np0 points / *nf0 faces of u
void fun_1402fa940(model::OBJ_GEO3D* u, model::OBJ_GEO3D* s, const std::int32_t* np0,
                   const std::int32_t* nf0);
// 1402fc1b0: renumber the nodes of a surface so that only the nodes faces use remain
void surface_compact_nodes(model::OBJ_GEO3D* s);
// 140163340: 3D: surface count := min(n, 100) (surfaces n..count-1 cleared first); 2D:
// 140198230(o, n)
void set_surface_slots(model::LLOBJ_DATA* o, std::int32_t n);
// 1402009f0: size surface `bdry` (nnode points, nface faces), or print the iBdryNo error
void size_surface(model::LLOBJ_DATA* o, std::int32_t nnode, std::int32_t nface, std::int32_t bdry);

// ---- 2D geometry from the mesh (surface2d.cpp) ----
// 1401fcc50: 2D object with a mesh (+0x87c) and an element whose connectivity (obj+0x9e8, 4 per
// element) repeats a node (a triangle written as a quad)
bool fun_1401fcc50(model::LLOBJ_DATA* o);
// 1401fcd50: 3D: surface_from_mesh_3d (1401fc730 never calls 1401fcd50 for 3D). 2D: edge count
// of geometry 0 := obj+0xbd8 (140163340), then curve b of geometry 0 = the n nodes of boundary
// record b (obj+0xbe0 + b*0x30: count n, node list at +8; records with no list or n < 1 are
// skipped, node numbers > obj+0x880 clamped to the last node) as n type-1 points (x, y, 0). The
// curve is not closed: the binary computes the next node (wrapping to the first) but never
// stores it.
void fun_1401fcd50(model::LLOBJ_DATA* o, char* compact);
// 1401fd240: geometry 0 rebuilt from the mesh: 2D objects copy their mesh (nodes, quads, material
// groups) into a new OBJ_MESH2D, build its boundary topology (1403dedd0) and turn it into faces /
// loops / edges / vertices of geometry 0 (1403e0f20); other dimensions do the same with the mesh
// geometry 0 already holds (1402815a0). `compact` is not read.
void fun_1401fd240(model::LLOBJ_DATA* o, char* compact);
// 140198230: edge count of geometry 0 := n (14027fe20 on geometry 0); calls surface2d_ext()
// .fun_140198230 (p-dbread's dbread::set_curve_count when defdbio/dbread/w1.hpp is present)
void fun_140198230(model::LLOBJ_DATA* o, std::int32_t n);

// Calls into packets that are not on this branch. Default targets stop in DEF_NOT_PORTED.
struct Surface2dExt {
    // 14027fe20 (p-dbread, defdbio dbread w1_geo.cpp geo2d_set_edge_count): add edges with new
    // curves (14027fce0) up to *n or remove the edges from *n on (140280290), then 1402847d0(g, 1)
    void (*fun_14027fe20)(model::GEO2D_AND_MESH2D* g, const std::int32_t* n);
    // 140198230 (p-dbread dbread::set_curve_count); fallback: the local body (14027fe20 above on
    // geometry 0)
    void (*fun_140198230)(model::LLOBJ_DATA* o, std::int32_t n);
};
Surface2dExt& surface2d_ext();
void reset_surface2d_ext();

// ---- 2D own centre (centre2d.cpp) ----
// 1401ff4f0: c[0..1] = (min + max) / 2.0 of the geometry box (1401ff600), else of the node box
// (1401ff5b0); false (c untouched) when neither exists
bool centre_2d(model::LLOBJ_DATA* o, double* c);
// 1401ff600: union of the edge boxes (OBJ_GEO2D::bounding_box 140279040) of geometry 0, only when
// has_geometry(o); x, y only
bool geometry_bbox_2d(model::LLOBJ_DATA* o, double* mn, double* mx);
// 1401ff5b0: node_bbox of obj+0x8a0 when the object has a mesh (+0x87c)
bool mesh_bbox(model::LLOBJ_DATA* o, double* mn, double* mx);

// ---- boxes and WPAXIS of modes 13 / 14 (bbox_wpaxis.cpp) ----
// 1402faca0: box of the points of a surface; init 1e29 / -1e29, strict updates
void surface_bbox(model::OBJ_GEO3D* g, double* mn, double* mx);
// 140203fa0: some node has a BCCDEF code (int[3] per node at obj+0x990) equal to 7 or 8
bool node_bc_code_7_or_8(model::LLOBJ_DATA* o);
// 14025a000: first item of the WPAXIS list (obj+0x20a0) with +4 == 3 and +0x60 == tag, or null
unsigned char* wpaxis_find(unsigned char* list, std::int32_t tag);
// 140259f90: wpaxis_find, else append a new item (140259950, id 0) with +4 = 3, +0x60 = tag
unsigned char* wpaxis_find_or_create(unsigned char* list, std::int32_t tag);
// 140258f10: out = the 3 doubles at item+8 (nothing when out is null)
void wpaxis_vector_a(unsigned char* item, double* out);
// 140258f90: out = the 3 doubles at item+0x20 (nothing when out is null)
void wpaxis_vector_b(unsigned char* item, double* out);

// ---- object tests (predicates.cpp) ----
// 140160bf0: mesh flag +0x87c != 0
bool has_mesh(const model::LLOBJ_DATA* o);
// 140160ba0: has_mesh or has_geometry
bool has_mesh_or_geometry(const model::LLOBJ_DATA* o);
// 1402039d0: 2D geometry 0 has edges and none is degenerate (140304e40(o, 0))
bool has_die_geometry_2d(model::LLOBJ_DATA* o);

// The Fortran mass properties (1403e8510 closure, module block 140b44d80) are p67-3's:
// defcmd/drop/mode8.hpp.

} // namespace def::cmd::objpos
