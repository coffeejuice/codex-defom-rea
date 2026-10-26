// Packet p8-08: the REMESH, DEFINT and DEFFRZ handlers and their dispatcher case bodies.
// Specs docs/re/commands/REMESH.md, DEFINT.md, DEFFRZ.md; decompile
// deform-rea-work/out/p8mg/p8-08/all.c.
#include <defcmd/cmd_p8_08.hpp>

#include <cstdio>
#include <cstring>

#include <defcmd/interp.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::cmd {

namespace {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

LLOBJ_DATA* find(DB_STEP_DATA& m, std::int32_t id) { return model::object_list_find(&m.objects, id); }  // 1401d6ea0

void print(const char* text) { rt::crt::fprintf(stdout, "%s", text); }

} // namespace

// @src 1401aa000  The two branches of the original (3D: 1400015f0 true, mg at local_97f8; else
// local_4c08) differ only in the stack buffer and the routine.
bool defint_object(DB_STEP_DATA& m, const std::uint32_t* obj)
{
    LLOBJ_DATA* target = find(m, static_cast<std::int32_t>(*obj));
    if (target == nullptr) return false;
    rt::crt::fprintf(stdout, "INFO   : Interpolating data - Object %d...\n", *obj);
    const bool is_3d = rt::dimension_is_3(&m);  // 1400015f0
    {
        MeshGenBase mg;                                   // 14013e790
        meshgen_bind(mg, target, &m, m.u340);             // 140148840
        LLOBJ_DATA* objs[1] = {target};                   // local_9808
        interp_set_sources(mg, 1, objs, m.u1748, m.u744); // 1401575a0
        if (std::strlen(m.u1748) == 0) {
            print("         Skip data interpolation\n");
        } else {
            rt::crt::fprintf(stdout, "         Data interpolation...%s\n", m.u1748);
            const bool ok = is_3d ? interp_3d(mg, m.u1748) : interp_2d(mg, m.u1748);
            print(ok ? "         Success\n" : "         Failure\n");
        }
        exchange_cleanup(mg, true);  // 14013e880(mg, 1)
    }                                // 14013e840
    print("INFO   : Interpolating data - Done\n");
    return true;
}

// @src 1401aa370  objB is not checked here (the dispatcher did); the two branches differ only
// in the stack buffer.
bool deffrz_object(DB_STEP_DATA& m, const std::uint32_t* a, const std::uint32_t* b,
                   const double* value)
{
    LLOBJ_DATA* target = find(m, static_cast<std::int32_t>(*a));
    LLOBJ_DATA* source = find(m, static_cast<std::int32_t>(*b));  // local_9818
    if (target == nullptr) return false;
    rt::crt::fprintf(stdout, "INFO   : Force interpolating - Object %d...\n", *a);
    {
        MeshGenBase mg;
        interp_set_mode(mg, 2);                           // 140155c70
        meshgen_bind(mg, target, &m, m.u340);
        LLOBJ_DATA* objs[1] = {source};
        interp_set_sources(mg, 1, objs, m.u1748, m.u744);
        interp_set_value(mg, *value);                     // 140157670
        if (std::strlen(m.u1748) == 0) {
            print("         Skip Force interpolation\n");
        } else {
            rt::crt::fprintf(stdout, "         Force interpolating...%s\n", m.u1748);
            const bool ok = object_interpolation(mg, false);  // 1401576a0(mg, 0)
            print(ok ? "         Success\n" : "         Failure\n");
        }
        exchange_cleanup(mg, true);
    }
    print("INFO   : Interpolating data - Done\n");
    return true;
}

// @src 1401a9a10
bool remesh_object(DB_STEP_DATA& m, const std::uint32_t* obj)
{
    if (find(m, static_cast<std::int32_t>(*obj)) == nullptr) return false;
    rt::crt::fprintf(stdout, "INFO   : Remeshing - Object %d...\n", *obj);
    (void)brdext_object(m, obj);  // 1401a9ac0 (p8-05)
    (void)defamg_object(m, obj);  // 1401a9be0 (p8-07)
    (void)defint_object(m, obj);  // 1401aa000
    print("INFO   : Remeshing - Done\n");
    return true;
}

// part of @src 140378e00  case 0x11f+4 (registry 291 REMESH)
int cmd_remesh(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;
    const int rc = def::key::read_i(f, &id, false);  // 140413de0(f, &id, 0)
    if (rc != 0) return 2;
    if (find(*m, id) == nullptr) return 2;
    const std::uint32_t uid = static_cast<std::uint32_t>(id);
    (void)remesh_object(*m, &uid);
    return 0;
}

// part of @src 140378e00  case 0x11f+7 (registry 294 DEFINT)
int cmd_defint(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;
    const int rc = def::key::read_i(f, &id, false);
    if (rc != 0) return 2;
    if (find(*m, id) == nullptr) return 2;
    const std::uint32_t uid = static_cast<std::uint32_t>(id);
    (void)defint_object(*m, &uid);
    return 0;
}

// part of @src 140378e00  case 0x11f+9 (registry 296 DEFFRZ)
int cmd_deffrz(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t a = 0, b = 0;
    double d = 0.0;
    const int rc = def::key::read_ii_d(f, &a, &b, &d);  // 140415f00 "%i %i %Flf"
    if (rc != 0) return 2;
    if (find(*m, a) == nullptr) return 2;
    if (find(*m, b) == nullptr) return 2;
    const std::uint32_t ua = static_cast<std::uint32_t>(a), ub = static_cast<std::uint32_t>(b);
    (void)deffrz_object(*m, &ua, &ub, &d);
    return 0;
}

} // namespace def::cmd
