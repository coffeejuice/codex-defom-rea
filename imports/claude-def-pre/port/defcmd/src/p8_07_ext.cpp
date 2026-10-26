// The callees in defcmd/p8_07_ext.hpp, forwarded to their owners' ports (wired at the P5 gate).
#include <defcmd/p8_07_ext.hpp>

#include <defcmd/contact_cmds.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/objpos_mirror.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>

namespace def::cmd::ext {

// 1401d7f40: the P4 gate's port (defdbio/src/gendb/pre_steps/pre_steps.cpp)
void fun_1401d7f40(def::model::DB_STEP_DATA* m) { def::dbio::gendb::gendb_command_pre_steps(m); }

// 1401fc730: p67-1's port (objpos/centre3d.cpp); *compact is only read there
void fun_1401fc730(def::model::LLOBJ_DATA* o, const char* compact)
{
    char c = *compact;
    objpos::fun_1401fc730(o, &c);
}
// 140268290: p67-1's port (objpos/transform4x4.cpp window_xf4)
void fun_140268290(def::model::LLWINDEF_DATA* w, const double* m16) { objpos::window_xf4(w, m16); }
// 140202a20: the contact commands' port (contact/contact_cmds.cpp min_edge_length)
double fun_140202a20(def::model::LLOBJ_DATA* obj) { return def::cmd::min_edge_length(obj); }
bool fun_140158ca0(def::model::LLOBJ_DATA* obj) { return def::model::geo_kw::object_dim(*obj) == 3; }  // owner p67-5

} // namespace def::cmd::ext
