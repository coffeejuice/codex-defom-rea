// defcmd/meshgen/mesh3d.hpp: the 3D solid (volume) mesh chain of _MeshGenBase, packet p8-06.
// Spec: docs/re/commands/DEFAMG.md §4.2 / §6.3, CNVT3D.md §6; status docs/re/status/p8-06.tsv.
//
// Entry: mesh3d_volume_step (140140110, vtable slot +0x10), declared in p8_seams.hpp. The rest is
// exposed for the tier-1 tests and for CNVT3D (p9-03 calls 140142f10 / 140143900 indirectly
// through the same step).
//
// Not ported: the in-process coating mesh 1401403c0 and its subtree (the DEF_MHCK mesh check
// 14026a4f0 and 45 more); unreachable in batch, the proof is in the status file.
#pragma once

#include <cstdint>

#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/p8_seams.hpp>

namespace def::cmd {

// @src 140140310  coating step: only when the coating layer count obj+0x2de8 > 0 (never in batch)
void mesh3d_coating_step(MeshGenBase& mg);
// @src 1401403c0  in-process coating mesh (not ported: unreachable in batch, see p8-06.tsv)
std::int32_t mesh3d_coating(MeshGenBase& mg, std::int32_t nlayer, const double* thickness,
                            const std::int32_t* ids);
// @src 140141d50  MESH3D_GUI, BRKMSH, CHKVOL or the second DEF_AMG3_GUI run, SCRATCH.KEY read
// back; 0 on success, else a step code (6, 7, 8, 9, 0xc, or 0x11 / 7 from the MESH3D.INP writer)
std::int32_t mesh3d_run_volume_mesher(MeshGenBase& mg);
// @src 140142680  delete the volume mesher's exchange files
void mesh3d_cleanup(MeshGenBase& mg);
// @src 1401427e0  write MESH3D.INP (surface nodes and polygons); 0, 6 (open), 7 (no nodes), 0x11
std::int32_t mesh3d_write_inp(MeshGenBase& mg);
// @src 140142f10  read the RZ / ELMCON blocks of SCRATCH.KEY into the object; true on success
bool mesh3d_read_scratch_key(MeshGenBase& mg);
// @src 140143900  mesher state = 2 (volume mesh read)
void mesh3d_set_volume_read(MeshGenBase& mg);
// @src 1401445f0  set the object's group id (DAT_1406e2d80) on all its elements
void mesh3d_apply_group(MeshGenBase& mg);

} // namespace def::cmd
