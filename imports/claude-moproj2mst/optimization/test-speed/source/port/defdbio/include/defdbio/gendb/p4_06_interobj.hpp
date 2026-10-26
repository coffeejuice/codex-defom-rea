// defdbio/gendb/p4_06_interobj.hpp: GENDB writer of the inter-object records (LLBRT, keyword
// group 3) and the 2D boundary search that picks the record of an object pair (packet p4-06).
//
// FUN_1401fa4f0 walks the object pairs i <= j of DB_STEP_DATA::objects, finds the inter-object
// node (i, j) of DB_STEP_DATA::inter_obj (m+0x13c8), chooses one boundary of each object
// (FUN_1401fa840), writes the record rec(bm, bs) with FUN_1401f7730 and, when bm != bs, the mirror
// record rec(bs, bm) (word 0 1.0 <-> 2.0, the other words copied).
//
// Record addressing: rec(bm, bs) = LLBRT + W * ((bs - 1) * N + bm - 1), N = number of boundaries
// (m+0x80), W = LLBRT record width.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>

namespace def::dbio::gendb {

// @src 1401fa4f0  write the inter-object list: list (m+0x13c8), LLBRT, &N (m+0x80), IIFNC, LLFNC,
//                 LLWIN, LLWRZ, &W(LLBRT), &W(LLWIN), LLBND, &W(LLBND) (the last two are only
//                 passed on to 1401fa840, which does not use them)
void write_interobj_list(model::Cls_1401fa020* list, double* llbrt, const std::int32_t* nbnd, std::int32_t* iifnc,
                         double* llfnc, double* llwin, double* llwrz, const std::int32_t* w_llbrt,
                         const std::int32_t* w_llwin, double* llbnd, const std::int32_t* w_llbnd);

// @src 1401fa840  boundary numbers (*bm of master, *bs of slave) of the pair's record: the single
//                 boundary of an object that has one, else a 2D nearest-boundary search; *flag is
//                 the node's contact flag (+0x170) and picks the search direction; 0 when not found
void choose_boundary_records(model::Cls_1401fa020* list, model::LLOBJ_DATA* master, model::LLOBJ_DATA* slave,
                             const std::int32_t* flag, std::int32_t* bm, std::int32_t* bs, double* llbnd,
                             const std::int32_t* w_llbnd);

// @src 1401667e0  first point (x, y) of boundary *idx (0-based) of obj: meshed (+0x87c != 0) the
//                 first node of the boundary; rigid the first point of curve 0 (curve index ignored),
//                 (0, 0) without geometry; pt unchanged for a meshed object with idx out of range
void boundary_first_point(model::LLOBJ_DATA* obj, double* pt, const std::int32_t* idx);

// @src 140166980  over the boundaries k of meshed object a: the boundary of b nearest to the first
//                 point of k; keeps the pair with the smallest distance (*ia = a+0x3588 + k,
//                 *ib = b's boundary number); false when a has no mesh or no boundaries
bool nearest_boundary_pair(model::LLOBJ_DATA* a, model::LLOBJ_DATA* b, std::int32_t* ia, std::int32_t* ib);

// @src 140166b40  boundary number (obj+0x3588 + index) of obj nearest to (x, y); *dist (if dist
//                 != 0) = distance from (x, y) to the nearest point (rigid objects: the nearest point
//                 stays (0, 0), see the source)
bool nearest_boundary(model::LLOBJ_DATA* obj, double x, double y, std::int32_t* bnd, double* dist);

// @src 140166d30  0-based index of the rigid curve of obj nearest to (x, y); px / py are taken
//                 but never written (the caller's nearest point stays as it was); true when obj has
//                 geometry, even if no curve gave a point
bool nearest_curve(model::LLOBJ_DATA* obj, double x, double y, std::int32_t* idx, double* px, double* py);

// @src 14027b620  nearest point (*px, *py) of the outline g to (x, y) and its distance *dist
//                 (curve type 1: the point rows; else the two end points of each segment row);
//                 false for an empty outline
bool geo2d_nearest_point(model::OBJ_GEO2D* g, double x, double y, double* dist, double* px, double* py);

// @src 1401f7730  one LLBRT record of the inter-object node io (2D and 3D layouts): *master /
//                 *slave object numbers (2D: the other object's number goes to the window writer),
//                 rec, IIFNC, LLFNC, LLWIN, LLWRZ, &W(LLWIN). Functions get the next function numbers
//                 (FNCLOC), windows the next LLWIN records (cursor m+0x134 / m+0x138).
void write_interobj_record(model::Cls_1401f46e0* io, const std::int32_t* master, const std::int32_t* slave,
                           double* rec, std::int32_t* iifnc, double* llfnc, double* llwin, double* llwrz,
                           const std::int32_t* w_llwin);

} // namespace def::dbio::gendb
