// defcmd/ctc3d/cube.hpp: C cube / locator library (cfnCUBE_*, defLOCD, defCELL, defDPT) and the C++ bucket build, GENCTC_3d_search.md sec. 3.1, sec. 4, sec. 2.1.
// Packet p67-7, group "cube". The library core (table, BuildBucket, box query, defLOCD/defCELL/
// defDPT, vectors, pcube) is ported once by p67-2 (defcmd/ifd/cube.hpp, cube_lib.hpp, namespace
// def::cmd::cube, typed: Cube, Locator, CellList, PointSet, ElemArray, ListNode, Vec3). This
// group adds the rest of the library in the same namespace (point / control-volume / edge /
// element insertion, exact box test, attribute lists, allocation helpers) and the cfnCUBE entry
// points GENCTC 3D calls, plus the C++ driver 1401a7710. All are re-exported into ctc3d.
#pragma once

#include <defcmd/ctc3d/common.hpp>
#include <defcmd/ifd/cube.hpp>
#include <defcmd/ifd/cube_lib.hpp>

namespace def::cmd::cube {

using i32 = std::int32_t;

// attribute node = cube::ListNode: +0x00 field id, +0x04 count, +0x08 data (ints or doubles), +0x10 next
inline i32& node_field(ListNode* n) { return reinterpret_cast<i32*>(&n->u00)[0]; }
inline i32& node_count(ListNode* n) { return reinterpret_cast<i32*>(&n->u00)[1]; }

// @src 140256a60  calloc n bytes ("DEF_BYTE")
void* fun_140256a60(i32 n);
// @src 140256ac0  realloc n bytes ("DEF_BYTE")
void* fun_140256ac0(void* p, i32 n);
// @src 140256b30  calloc n chars ("CHAR")
char* fun_140256b30(i32 n);
// 1402571b0  realloc n doubles ("DOUBLE"); forwards to DBREAD realloc_doubles
double* fun_1402571b0(void* p, i32 n);
// @src 140233ef0  map[|conn[k]| - base] = 'T' for k < nf*nv
void fun_140233ef0(char* map, const i32* conn, i32 nf, i32 nv, i32 base);
// @src 140406540  ((dx*dx + dy*dy) + dz*dz)
double fun_140406540(const double* a, const double* b);

// defLOCD insertion of the cube types p67-2 does not use
// @src 140400330  defLOCD_InsertPoint_Obj
i32 fun_140400330(Locator* l, const double* x, i32 id, i32 verbose);
// @src 140401c40  defLOCD_InsertPolygon_ControlVolume
i32 fun_140401c40(Locator* l, const i32* conn, i32 nv, i32 id, i32 verbose);
// @src 1404021a0  defLOCD_InsertEdge_Obj (conn = the 2-node record)
i32 fun_1404021a0(Locator* l, const i32* conn, i32 verbose);
// @src 140402c40  defLOCD_InsertElement_Obj
i32 fun_140402c40(Locator* l, const i32* conn, i32 nv, i32 id, i32 verbose);
// @src 1404040c0  exact polygon-box test (box [min3, max3])
i32 fun_1404040c0(Locator* l, const double* box, const i32* conn, i32 nv);

// attribute lists (cube +0x180 nodes, +0x190 polygons): per entity an integer and a real list
// @src 140404560  set an integer field of entity (1-based); 0 when out of range
i32 fun_140404560(ElemArray* a, i32 entity, i32 field, const void* data, i32 count);
// @src 1404045e0  get an integer field
i32 fun_1404045e0(ElemArray* a, i32 entity, i32 field, void* buf, i32* count);
// @src 140404660  set a real field
i32 fun_140404660(ElemArray* a, i32 entity, i32 field, const void* data, i32 count);
// @src 1404046f0  get a real field
i32 fun_1404046f0(ElemArray* a, i32 entity, i32 field, void* buf, i32* count);
// @src 140404900  integer list: replace or append the field; new head
ListNode* fun_140404900(ListNode* head, i32 field, const i32* data, i32 count);
// @src 140404a00  new integer node
ListNode* fun_140404a00(i32 field, const i32* data, i32 count);
// @src 140404ab0  replace the data of an integer node
ListNode* fun_140404ab0(ListNode* head, ListNode* node, ListNode* prev, const i32* data, i32 count);
// @src 140404b90  find an integer field (count 0 when missing)
void fun_140404b90(ListNode* head, i32 field, i32* buf, i32* count);
// @src 140404ce0  real list: replace or append the field
ListNode* fun_140404ce0(ListNode* head, i32 field, const double* data, i32 count);
// @src 140404de0  new real node
ListNode* fun_140404de0(i32 field, const double* data, i32 count);
// @src 140404e90  replace the data of a real node
ListNode* fun_140404e90(ListNode* head, ListNode* node, ListNode* prev, const double* data, i32 count);
// @src 140404f70  find a real field
void fun_140404f70(ListNode* head, i32 field, double* buf, i32* count);

} // namespace def::cmd::cube

namespace def::model {
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::ctc3d {

// p67-2's part of the library
using def::cmd::cube::fun_140225640;
using def::cmd::cube::fun_140225820;
using def::cmd::cube::fun_140225af0;
using def::cmd::cube::fun_140225ff0;
using def::cmd::cube::fun_140226150;
using def::cmd::cube::fun_140228c60;
using def::cmd::cube::fun_140231b10;
using def::cmd::cube::fun_140239700;
using def::cmd::cube::fun_140239a30; // 140239a30 |a|: defined (untagged) in p67-2's ifd/cube/vec.cpp
using def::cmd::cube::fun_140239b40;
using def::cmd::cube::fun_140239fb0;
using def::cmd::cube::fun_14023a1b0;
using def::cmd::cube::fun_14023a4b0;
using def::cmd::cube::fun_14023aa90;
using def::cmd::cube::fun_140240b40;
using def::cmd::cube::fun_140240d80;
using def::cmd::cube::fun_140256e20;
using def::cmd::cube::fun_1403fed00;
using def::cmd::cube::fun_1403feec0;
using def::cmd::cube::fun_1403ff870;
using def::cmd::cube::fun_1403ff890;
using def::cmd::cube::fun_1403ff970;
using def::cmd::cube::fun_140401050;
using def::cmd::cube::fun_1404044c0;
using def::cmd::cube::fun_140404780;
using def::cmd::cube::fun_1404047a0;
using def::cmd::cube::fun_1404047f0;
using def::cmd::cube::fun_140404850;
using def::cmd::cube::fun_140404c40;
using def::cmd::cube::fun_140405010;
using def::cmd::cube::fun_1404050b0;
using def::cmd::cube::fun_140405150;
using def::cmd::cube::fun_140405350;
using def::cmd::cube::fun_140405540;
using def::cmd::cube::fun_1404056c0;
using def::cmd::cube::fun_1404056e0;
using def::cmd::cube::fun_140405720;
using def::cmd::cube::fun_1404057a0;
using def::cmd::cube::fun_140405c80;
using def::cmd::cube::fun_1404062e0;
using def::cmd::cube::fun_140406300;
using def::cmd::cube::fun_140406360;
using def::cmd::cube::fun_140406510;
using def::cmd::cube::fun_140453010;
using def::cmd::cube::fun_1404531b0;
using def::cmd::cube::fun_140453300;
using def::cmd::cube::fun_1404535e0;
using def::cmd::cube::fun_1404537f0;
using def::cmd::cube::fun_140453850;
using def::cmd::cube::fun_140453c40;
using def::cmd::cube::fun_140453e80;
using def::cmd::cube::fun_140454110;

// this group's library part
using def::cmd::cube::fun_140256a60;
using def::cmd::cube::fun_140256ac0;
using def::cmd::cube::fun_140256b30;
using def::cmd::cube::fun_1402571b0;
using def::cmd::cube::fun_140233ef0;
using def::cmd::cube::fun_140406540;
using def::cmd::cube::fun_140400330;
using def::cmd::cube::fun_140401c40;
using def::cmd::cube::fun_1404021a0;
using def::cmd::cube::fun_140402c40;
using def::cmd::cube::fun_1404040c0;
using def::cmd::cube::fun_140404560;
using def::cmd::cube::fun_1404045e0;
using def::cmd::cube::fun_140404660;
using def::cmd::cube::fun_1404046f0;
using def::cmd::cube::fun_140404900;
using def::cmd::cube::fun_140404a00;
using def::cmd::cube::fun_140404ab0;
using def::cmd::cube::fun_140404b90;
using def::cmd::cube::fun_140404ce0;
using def::cmd::cube::fun_140404de0;
using def::cmd::cube::fun_140404e90;
using def::cmd::cube::fun_140404f70;

// @src 1401a7710  cubes for every object of the model (GENCTC_3d_search.md sec. 2.1)
void fun_1401a7710(def::model::DB_STEP_DATA* model);
// @src 1401a7980  true: cube from the FE boundary surface, false: from the rigid surface 0
bool fun_1401a7980(def::model::DB_STEP_DATA* model, def::model::LLOBJ_DATA* obj);
// @src 1401a7a30  obj is the slave of some inter-object pair
bool fun_1401a7a30(def::model::DB_STEP_DATA* model, def::model::LLOBJ_DATA* obj);
// @src 140225cd0  cfnCUBE_Clean_ObjEntity
void fun_140225cd0(i32* obj, i32* inst, i32* verbose);
// @src 1402260a0  rebuild a bucket keeping its attribute lists (140226150 with create = 0)
void fun_1402260a0(i32* obj, i32* inst, i32* type, i32* np, double* x, i32* nf, i32* nv, i32* faces, i32* base, i32* verbose);
// @src 140227e00  cfnCUBE_Get_Elem_Conn: node numbers of polygon *poly (minus base plus 1), count = nv
void fun_140227e00(i32* obj, i32* inst, i32* poly, i32* conn, i32* count);
// @src 140227fc0  cfnCUBE_Get_Node_Coord
void fun_140227fc0(i32* obj, i32* inst, i32* point, double* xyz);
// Attribute access (entity 1-based, field id, buffer of ints / doubles, count):
// @src 1402280d0  cfnCUBE_Init_Object_EI
void fun_1402280d0(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228260  cfnCUBE_Get_Object_EI
void fun_140228260(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 1402283b0  cfnCUBE_Init_Object_ER
void fun_1402283b0(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228540  cfnCUBE_Get_Object_ER
void fun_140228540(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228690  cfnCUBE_Init_Object_NI
void fun_140228690(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228830  cfnCUBE_Get_Object_NI
void fun_140228830(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228980  cfnCUBE_Init_Object_NR
void fun_140228980(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 140228b10  cfnCUBE_Get_Object_NR
void fun_140228b10(i32* obj, i32* inst, i32* entity, i32* field, void* buf, i32* count);
// @src 14022a5c0  cfnCUBE_GetPoly3_Obj: unique ascending polygon ids (i32) of the last box query
void fun_14022a5c0(i32* obj, i32* inst, void* list, std::uint32_t* n, i32* verbose);
// @src 14022d770  cfnCUBE_SearchPoint_Obj: nearest cube point (i32 number) and its distance
void fun_14022d770(i32* obj, i32* inst, double* p, double* dist, std::uint32_t* point, i32* verbose);
// @src 14022fcd0  cfnCUBE_SearchPoint_CellPolygon_Obj: initial search radius
void fun_14022fcd0(i32* obj, i32* inst, double* p, double* radius, i32* verbose);
// @src 140232090  cfnCUBE_GetDimension_Obj: cell sizes h[3] of the last record of (obj, inst)
void fun_140232090(i32* obj, i32* inst, double* h, i32* verbose);

} // namespace def::cmd::ctc3d
