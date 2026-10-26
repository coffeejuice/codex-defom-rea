// defcmd/cmd_p9_01.hpp: the CNVT3D command (registry 287, case 0x11f of the group 6 dispatcher
// FUN_140378e00), the entry the p5-01 dispatcher calls. Packet p9-01; the packet's own API is
// defcmd/cnvt3d.hpp. Spec: docs/re/commands/CNVT3D.md section 2.
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// part of @src 140378e00  case CNVT3D: "%i" n on the keyword line (reader error -> 2); n <= 0 ->
// 0; else the id line (tokens, at most n, strtol), MeshGenBase ctor, 14013e880(cvt, 1), the
// converter 1401467f0(cvt, m+0x1748, m, ids, count), MeshGenBase dtor; 0 whatever the result.
int cmd_cnvt3d(def::model::DB_STEP_DATA* m, std::FILE* f);

} // namespace def::cmd
