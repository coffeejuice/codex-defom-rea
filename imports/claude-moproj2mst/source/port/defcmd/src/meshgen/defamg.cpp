// DEFAMG handler 1401a9be0 and its dispatcher case (packet p8-07). Spec: docs/re/commands/DEFAMG.md.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

#include <defcmd/cmd_p8_07.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/p8_07_ext.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "mg_internal.hpp"
#include "mg_util.hpp"

namespace def::cmd {

using namespace mg;

// @src 1401a9be0  generate a new mesh for object *id (2D DEF_AMG, 3D DEF_AMG3_GUI + p8-06's
// volume step); false when the object is missing or has neither geometry nor a mesh
bool defamg_object(DB_STEP_DATA& m, const std::uint32_t* id)
{
    LLOBJ_DATA* obj = model::object_list_find(&m.objects, static_cast<std::int32_t>(*id));  // 1401d6ea0
    if (obj == nullptr) return false;
    rt::crt::fprintf(stdout, "INFO   : Generating mesh - Object %d...\n", *id);  // 14047903c()+0x30
    if (!rt::dimension_is_3(&m)) {  // 1400015f0
        MeshGenBase g;                    // 14013e790
        meshgen_bind(g, obj, &m, m.u340);  // 140148840
        if (!meshgen_prepare(g)) return false;  // 140144620; dtor 14013e840
        exchange_cleanup(g, !obj_amg_dxt_ok(obj));  // 14013e880 (p8-08)
        if (!dbio::gendb::object_has_geometry(obj)) {  // 140160cc0
            const char one = 1;
            ext::fun_1401fc730(obj, &one);
        }
        const char no = 0;
        mesh2d_run(g, &no);  // 140144a70
    } else {
        MeshGenBase g;
        meshgen_bind(g, obj, &m, m.u340);
        if (!meshgen_prepare(g)) return false;
        exchange_cleanup(g, !obj_amg_dxt_ok(obj));
        if (!dbio::gendb::object_has_geometry(obj)) {
            const char one = 1;
            ext::fun_1401fc730(obj, &one);
        }
        const char yes = 1;
        size_scaling(obj, -1.0, 0, &yes);  // 1401559d0, DAT_140522fc8
        g.surface_step(0);                 // 14013ee00
        g.volume_step(1);                  // 140140110 (p8-06)
    }
    m.u2f89 = 1;
    if (has_mesh(obj) == 0) {  // 140160bf0
        rt::crt::fprintf(stdout, "INFO   : Generating mesh - Failed\n");
    } else {
        rt::crt::fprintf(stdout, "         Success\n");
        rt::crt::fprintf(stdout, "INFO   : Generating mesh - Done\n");
        if (0.0 < at<double>(obj, 0x2090)) {
            double v = 0.0;
            if (!ext::fun_140158ca0(obj)) {
                v = dbio::objcheck::obj_volume(obj, 1);  // 140163540
            } else {
                v = dbio::objcheck::obj_volume(obj, 0);
            }
            if (0.0 < v) {
                at<double>(obj, 0x2090) = v;
                rt::crt::fprintf(stdout, "INFO   : New target volume (%g) has been assigned.\n", v);
            }
        }
    }
    return true;
}

// part of @src 140378e00  case 0x11f+6 DEFAMG
int cmd_defamg(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;
    if (key::read_i(f, &id, false) != 0) return 2;  // 140413de0(f, &id, 0)
    if (model::object_list_find(&m->objects, id) == nullptr) return 2;  // 1401d6ea0
    const std::uint32_t u = static_cast<std::uint32_t>(id);
    defamg_object(*m, &u);  // result ignored
    return 0;
}

} // namespace def::cmd
