// Link bodies of defcmd/objpos_2d_tree.hpp: roots owned by census packets C08 / C10 / C11. They
// forward to the first port in defmodel/obj_mesh2d/topology.cpp until the owners replace them
// (an owner may instead replace a body with DEF_NOT_PORTED(0x<addr>) and port it here).
#include <defcmd/objpos_2d_tree.hpp>

#include <defmodel/obj_mesh2d/topology.hpp>

namespace def::cmd::objpos {

// 1403dedd0 (owner C11): forwarder
void fun_1403dedd0(model::OBJ_MESH2D* m) { model::fun_1403dedd0(m); }
// 1403df940 (owner C08): the body is model::fun_1403df940 (defmodel/src/obj_mesh2d/
// topology_curves.cpp, reviewed against the decompile); the cmd-side root stays a link to it
void fun_1403df940(model::OBJ_MESH2D* m) { model::fun_1403df940(m); }
// 1403e0070 (owner C10): ported and reviewed against the decompile in
// defmodel/src/obj_mesh2d/topology_loops.cpp (census packet C10); this is its defcmd entry point.
void fun_1403e0070(model::OBJ_MESH2D* m) { model::fun_1403e0070(m); }

} // namespace def::cmd::objpos
