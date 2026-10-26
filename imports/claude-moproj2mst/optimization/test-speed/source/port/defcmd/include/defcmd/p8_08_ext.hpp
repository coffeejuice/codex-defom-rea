// defcmd/p8_08_ext.hpp: callees of packet p8-08 (DEFINT/DEFFRZ/REMESH) owned by packets outside
// the p8-05..p8-08 thread whose port is not usable from this branch. Replaced by the owners'
// functions at integration (src/p8_08/p8_08_ext.cpp).
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::cmd::p8_08_ext {

// 140183cb0 / 140183d00 (owners: defcmd contact_cmds.cpp element_set_conn / defdbio
// gendb3d/node_elem.cpp element_material_groups; both forward there): element set +8
// (connectivity) / +0x18 (material group per element) after the region sync 1401d5820(owner, 1/2),
// which changes nothing (1401d2bb0 always 1; see defdbio test_gendb3d_oracle). The bodies here
// return the fields directly; switch to the owner's export at integration.
std::int32_t* element_connectivity(void* elems);
std::int32_t* element_material_groups(void* elems);

} // namespace def::cmd::p8_08_ext
