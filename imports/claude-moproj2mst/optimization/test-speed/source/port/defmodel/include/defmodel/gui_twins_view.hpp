// defmodel/gui_twins_view.hpp: DEF_PRE_64 object tests whose byte-identical twins the DEF_GUI_MO_64
// 3D viewer calls when it binds an object (docs/re/moproj2mst/mdt/V/spec_V.md sections 2.6, 2.7,
// 4.1, 6). None of these addresses is in the DEF_PRE batch ledger; status rows in
// docs/re/status/mdt-viewer.tsv. Bodies in src/gui_twins_view/.
#pragma once

#include <cstdint>

namespace def::model {

class LLOBJ_DATA;
class OBJ_GEO3D;

// 140160060 (GUI twin 0x1408340f0): true when every coordinate of the first min(nodes, 10) nodes
// (obj+0x880 nodes, node_set_coords 1401808d0 of obj+0x8a0, width = the owner's table +0x15f8[0])
// lies in [-1e9, 1e9]. Otherwise the object name (obj+0x828) gets the prefix "*Corrupt* " (name cut
// to 0x3f - 10 = 53 characters) and the result is false. The owner (obj+0x3a98) is not checked
// for null, as in the binary.
bool llobj_mesh_coords_plausible(LLOBJ_DATA* obj);

// 1402fabc0 (GUI twin 0x140929bd0): nodes per face of a surface: 0 without elements; 3 when
// e[0] == abs(e[3]) and (one element only, or e[4] == abs(e[7])); else 4.
std::int32_t geo3d_face_node_count(OBJ_GEO3D* s);

// 140160fe0 (GUI twin 0x140835070): object kind (+0xc) is 0 (rigid).
bool llobj_is_rigid(const LLOBJ_DATA* obj);

}  // namespace def::model
