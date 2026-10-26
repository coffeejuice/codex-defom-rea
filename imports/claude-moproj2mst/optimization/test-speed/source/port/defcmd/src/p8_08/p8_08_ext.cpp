// Interim bodies of defcmd/p8_08_ext.hpp (see there for the owners).
#include <defcmd/p8_08_ext.hpp>

#include <defcmd/contact_cmds.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>

namespace def::cmd::p8_08_ext {

// 140183cb0: owner defcmd contact/contact_cmds.cpp element_set_conn (p67)
std::int32_t* element_connectivity(void* elems)
{
    return element_set_conn(elems);
}

// 140183d00: owner defdbio gendb3d/node_elem.cpp
std::int32_t* element_material_groups(void* elems)
{
    return dbio::g3d::element_material_groups(elems);
}

} // namespace def::cmd::p8_08_ext
