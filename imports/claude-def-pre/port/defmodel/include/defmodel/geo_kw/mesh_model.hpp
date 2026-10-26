// defmodel/geo_kw/mesh_model.hpp: FE-mesh model operations of packet kw-g2geo, family "mesh"
// (RZ, ELMCON, MTLGRP). The node / element count setters and array allocation (1401652b0,
// 1401653b0, ...) are kw-g2bc's (defmodel/bc/node_arrays.hpp); this header has the rest.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}  // namespace def::model

namespace def::model::geo_kw {

// @src 140165500  element type obj+0x890 = type; type 2 (tetrahedron): +0x894 = 4 nodes,
// +0x898 = 4 faces, +0x89c = 3 nodes per face; type 4 (brick): 8, 6, 4; other types touch nothing
// else.
void set_element_type(LLOBJ_DATA& obj, std::int32_t type);

// @src 1401fb270  material groups of the elements: when obj+0x3980 >= 1, *group = obj+0x3980 and
// *count = number of distinct values in obj+0x9f8[0..nelem-1] (1 without a mesh); otherwise
// 1401fb440.
void material_groups(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* group);

// @src 1401fb440  *count = number of distinct values in obj+0x9f8[0..nelem-1], *group = the most
// frequent one (first seen wins ties); obj+0x3980 = *group when no element has the old obj+0x3980.
// Without a mesh (obj+0x87c == 0) or array: 0 and 0.
void material_groups_counted(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* group);

// @src 140202960  obj+0x3980 = the most frequent element material group, or its old value when
// that is 0 (MTLGRP's reader calls it).
void update_material_group(LLOBJ_DATA& obj);

// @src 14039aa20  model byte +0x1b5c (set: ELMCON does not build the boundary)
std::uint8_t step_u1b5c(const DB_STEP_DATA& m);

}  // namespace def::model::geo_kw
