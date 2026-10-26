// defcmd/objpos_mirror.hpp: lower-level pieces of the OBJPOS mirror (3D single precision, 2D double)
// and of the drop's 4x4 transform (FUN_1401fe240). Packet p67-1. Specs: docs/re/commands/OBJPOS.md
// 6.5, 6.6, OBJPOS_geo_kernels.md 4 and 5, OBJPOS_interference_kernels.md 1.7.
//
// The 3D mirror matrix is the float class of the binary (row-major 4x4 floats, column-vector
// convention: q = M * (x, y, z, 1)); points are rounded to float, mirrored in float and widened
// back. The 4x4 transform of the drop is the double Matrix4 layout (row-vector convention,
// translation in row 3, element (i, j) at m16[i * 4 + j]).
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
class LLWINDEF_DATA;
class OBJ_GEO3D;
class GEO2D_AND_MESH2D;
class OBJ_GEO2D;
class Cls_14017e740;
namespace linked {
class Cls_140257340;
class Cls_140257340List;
} // namespace linked
} // namespace def::model

namespace def::cmd::objpos {

// ---- single-precision mirror matrix (Vector3f / Matrix4f of the binary) ----------------------
struct F3 {
    float x, y, z;
};
struct F44 {
    float m[16];  // row-major, element (i, j) at m[i * 4 + j]
};

// 140239630: Vector3f(x, y, z)
F3 f3(float x, float y, float z);
// 140239960: (x*x + y*y) + z*z in float
float f3_len2(const F3* v);
// 14023fdf0: m(i, 3) += (x, y, z)[i]
void f44_add_translation(F44* m, float x, float y, float z);
// 140240220: out = a * b (sum over k from the zero matrix at 0x1406fe9f0, k ascending)
void f44_mul(F44* out, const F44* a, const F44* b);
// 140240340: mirror at the plane through p with normal n: T(+p) * H(n) * T(-p)
void f44_mirror(F44* out, const F3* p, const F3* n);
// 140240980: q = M * (v, 1) (float)
void f44_apply(const F3* v, F3* q, const F44* m);

// The sequence every 3D mirror routine inlines: build f44_mirror from (float)p, (float)n.
F44 mirror_matrix(const double* p, const double* n);
// p (3 doubles) := (double)(M * (float)p)
void mirror_point_f(double* p, const F44* m);

// ---- 3D mirror parts ---------------------------------------------------------------------------
// 1402fbbe0: _OBJ_GEO3D mirror (points, face reversal 1402fbf60, point list +0x28)
void geo3d_mirror(model::OBJ_GEO3D* g, const double* p, const double* n);
// 1402578d0: axis-point record: point +8 and vector +0x20 both mirrored as points
void axis_point_mirror(model::linked::Cls_140257340* r, const double* p, const double* n);
// 140258500: every record of an axis-point list
void axis_list_mirror(model::linked::Cls_140257340List* l, const double* p, const double* n);
// 140268930: 3D window points (3 per point) when the window has a shape (1402632d0)
void window_mirror_3d(model::LLWINDEF_DATA* w, const double* p, const double* n);
// 14026a0a0: every window of a window list (Cls_140269250*)
void winlist_mirror_3d(void* list, const double* p, const double* n);
// 1401fe410: element node order after a 3D mirror (tet 1<->2, brick 1<->3 and 5<->7)
void fix_element_orientation(model::LLOBJ_DATA* o);
// 140201fb0: re-orient one boundary record (2D: node list reversed; 3D: per face 2<->3 or 2<->4
// chosen from face 0)
void boundary_record_reorient(model::Cls_14017e740* r);
// 140281710: display placement of a geometry-list entry: M_item = M_item * (double)mirror matrix
void cross_section_mirror(model::GEO2D_AND_MESH2D* g, const double* p, const double* n);

// ---- 2D mirror parts ---------------------------------------------------------------------------
// 140309640: mirror the point at the line a-b (double)
void mirror_point_2d(double* pt, const double* a, const double* b);
// 140278740: mirror an _OBJ_GEO2D edge, then re-order it (1402788a0)
void geo2d_mirror(model::OBJ_GEO2D* e, const double* a, const double* b);
// 140268890: 2D window points mirrored, then orientation 140265bf0(w, &1)
void window_mirror_2d(model::LLWINDEF_DATA* w, const double* a, const double* b);
// 140265bf0: keep the window counter-clockwise (2D); returns the binary's low byte (0 or 1)
bool window_orient(model::LLWINDEF_DATA* w, const char* reverse);
// 140305360 / 140305420: reverse the first *n ints / doubles of v (nothing when *n < 0 or v null
// (ints only))
void reverse_ints(const std::int32_t* n, std::int32_t* v);
void reverse_doubles(const std::int32_t* n, double* v);
// 140305180: 2D boundary-face arrays reversed per loop (ECHFLX twice, ECAFLX never)
void face_arrays_reverse_2d(model::LLOBJ_DATA* o);

// ---- 4x4 transform of the drop (double, Matrix4 layout) ---------------------------------------
// 140241270: p (3 doubles) := (p, 1) * M
void fun_140241270(const double* m16, double* p);
// 14030a170: p (2 doubles) := (p, 0, 1) * M
void xf4_point_2d(const double* m16, double* p);
// 140268290: window points (2D 14030a170, 3D 140241270); 3D windows refresh their faces
void window_xf4(model::LLWINDEF_DATA* w, const double* m16);
// 1401fe290: rigid surfaces (obj+0x3c00[k]) when +0x3990
void fun_1401fe290(model::LLOBJ_DATA* o, double* m16);
// 1401fe310: RZ (3 per node, when +0x87c), then fun_1401fe3a0
void fun_1401fe310(model::LLOBJ_DATA* o, double* m16);
// 1401fe3a0: FLDGEO / FLWNET surfaces (called from 1401fe310)
void fun_1401fe3a0(model::LLOBJ_DATA* o, double* m16);
// 1401fd730: windows, movement, process, GEO23, display placements, inter-object windows
void fun_1401fd730(model::LLOBJ_DATA* o, double* m16);
// 1402fccf0: _OBJ_GEO3D points
void geo3d_xf4(model::OBJ_GEO3D* g, const double* m16);

} // namespace def::cmd::objpos
