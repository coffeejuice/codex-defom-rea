// defcmd/objpos_2d_tree.hpp: the roots of the 1401fd240 subtree (2D branch of 1401fc730) that
// census packets other than C09 own (docs/re/census-2026-10-21.md on claude/census):
//   C11  1403dedd0  build the OBJ_MESH2D boundary topology (calls 1403deee0, 1403df940)
//   C08  1403df940  curves (1403df970) then loops (1403e0070)
//   C10  1403e0070  loops
// Binary argument order; namespace def::cmd::objpos. The bodies live in
// src/objpos/tree2d_pending.cpp until the owners replace them. At the time this header was
// written they forward to a first port of the whole subtree in
// defmodel/obj_mesh2d/topology.hpp (p67-1-2d worker, unverified against the oracle); owners
// replace the forwarders (or adopt / review that code) when their packets land.
#pragma once

namespace def::model {
class OBJ_MESH2D;
} // namespace def::model

namespace def::cmd::objpos {

// 1403dedd0: clear the topology arrays (+0x1588, +0x1590, +0x1598, +0x15a8, +0x15b0, +0x15b8),
// then sides / regions (1403deee0) and curves / loops (1403df940)
void fun_1403dedd0(model::OBJ_MESH2D* m);
// 1403df940: 1403df970 (curves), then 1403e0070 (loops)
void fun_1403df940(model::OBJ_MESH2D* m);
// 1403e0070: loops of curves per region
void fun_1403e0070(model::OBJ_MESH2D* m);

} // namespace def::cmd::objpos
