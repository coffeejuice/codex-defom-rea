// The task KEY file writers DEF_GUI_MO_64's "save current operation" calls (packet S,
// docs/re/moproj2mst/s/spec.md section 2): byte-identical twins of DEF_PRE functions that DEF_PRE
// batch mode never calls. Each opens the file "wt", prints the file banner (1401c5a20) and one
// block writer that is already ported. Header: defkey/item_writers.hpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defkey/item_writers.hpp>

#include <cstdio>
#include <cstring>
#include <share.h>

#include <defkey/group1.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/program_info.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "../kw_g1a/sim_ctrl_ops.hpp"
#include "../kw_g2obj/dispatch.hpp"

namespace def::key {

using model::field;

namespace {

bool no_name(const char* name) { return name == nullptr || std::strlen(name) == 0; }

std::FILE* open_wt(const char* name) { return _fsopen(name, "wt", _SH_DENYNO); } // 14047b934

// 1401c5a20(m, f, &0): product name of the model's dimension and the start-up version
void banner(model::DB_STEP_DATA& m, std::FILE* f)
{
    write_file_banner(f, 0, model::product_name_for_dim(m.dimension), model::version());
}

model::LLOBJ_DATA* object_no(model::DB_STEP_DATA& m, std::int32_t no)
{
    return g1a::ops::find_in_list(m.objects, no); // 14017aaf0(m + 0x1380, no)
}

// 140160f10 kind (+0xc) != 7, 140160bf0 has a mesh (+0x87c != 0)
bool meshed_solid(const model::LLOBJ_DATA* o)
{
    return field<std::int32_t>(o, 0xc) != 7 && field<std::int32_t>(o, 0x87c) != 0;
}

} // namespace

// @src 1401c5de0  SimCtrl.KEY: flags into m+0x1744, banner, every group 1 section (1403d4990)
int write_simctrl_key_file(model::DB_STEP_DATA& m, const char* name, const std::int32_t* flags)
{
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    const int r = write_simctrl_key_stream(m, f, flags);
    std::fclose(f);
    return r;
}

int write_simctrl_key_stream(model::DB_STEP_DATA& m, std::FILE* f, const std::int32_t* flags)
{
    m.u1744 = *flags;
    banner(m, f);
    return write_group1(&m, f);
}

// @src 1401c64f0  InterObject.KEY: banner and the inter-object blocks (1403c9ad0)
int write_interobject_key_file(model::DB_STEP_DATA& m, const char* name)
{
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    const int r = write_interobject_key_stream(m, f);
    std::fclose(f);
    return r;
}

int write_interobject_key_stream(model::DB_STEP_DATA& m, std::FILE* f)
{
    banner(m, f);
    return g3::write_inter_objects(&m, f);
}

// @src 1401c63f0  one object KEY file: flags into m+0x1744, object `no` written as `out_no`
// (0 = `no`) with its own banner (14029e250 withHeader = 1)
int write_object_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no,
                          const std::int32_t* flags, std::int32_t out_no)
{
    if (out_no == 0) out_no = no;
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    const int r = write_object_key_stream(m, f, no, flags, out_no);
    std::fclose(f);
    return r;
}

int write_object_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no,
                            const std::int32_t* flags, std::int32_t out_no)
{
    if (out_no == 0) out_no = no;
    m.u1744 = *flags;
    model::LLOBJ_DATA* obj = object_no(m, no);
    return g2obj::write_object(m, f, obj, static_cast<std::uint32_t>(out_no), true);
}

// @src 1401c6820  MeshSettingsNNNNN.KEY: banner, RM*/MG* block of object `no` (1402a9180);
// 3 without the object
int write_meshsettings_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    const int r = write_meshsettings_key_stream(m, f, no);
    std::fclose(f);
    return r;
}

int write_meshsettings_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no)
{
    banner(m, f);
    model::LLOBJ_DATA* obj = object_no(m, no);
    if (obj == nullptr) return 3;
    return g2obj::ext::write_mesh_block(m, f, *obj, static_cast<std::uint32_t>(no));
}

// @src 1402a9c80  CRDSYS (0x11a), GEO23 (0x11b), GEOSEC (0x11c), MSHSEC (0x11d) of an object from
// its GEO23 block +0x2be8 (GUI twin 0x14097fa30, packet MDT L5); always 0
int write_geo23_block(model::DB_STEP_DATA&, std::FILE* f, model::LLOBJ_DATA* obj, std::uint32_t no)
{
    def::rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x11a), no, field<std::int32_t>(obj, 0x2c38));
    const std::int32_t mode = field<std::int32_t>(obj, 0x2be8);
    if (mode == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_label(0x11b), no, 0);
    } else if (mode == 1) {
        const std::int32_t type = field<std::int32_t>(obj, 0x2c08);
        const double a = field<double>(obj, 0x2bf0);
        const double b = field<double>(obj, 0x2bf8);
        const double c = field<double>(obj, 0x2c00);
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x11b), no, 1, type);
        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", a, b, c);
    } else if (mode == 2) {
        const std::int32_t type = field<std::int32_t>(obj, 0x2c08);
        const double a = field<double>(obj, 0x2c10);
        const double b = field<double>(obj, 0x2c18);
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x11b), no, 2, type);
        def::rt::crt::fprintf(f, " %20.10E %20.10E\n", a, b);
    }
    const std::uint32_t sec = field<std::uint8_t>(obj, 0x2c21) != 0 ? 1u : 0u;
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x11c), no, field<std::int32_t>(obj, 0x2c24), sec);
    const std::uint32_t msh = field<std::uint8_t>(obj, 0x2c20) != 0 ? 1u : 0u;
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(0x11d), no, field<std::int32_t>(obj, 0x2c28), msh);
    return 0;
}

// @src 1401c6920  GeoConvt23DataNNNNN.KEY: banner, 1402a9c80 of object `no` (GUI twin 0x140896460).
// The original passes a missing object on (null dereference); callers pass existing objects.
int write_geoconvt_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    banner(m, f);
    model::LLOBJ_DATA* obj = object_no(m, no);
    if (obj == nullptr) {
        std::fclose(f);
        return 3;
    }
    const int r = write_geo23_block(m, f, obj, static_cast<std::uint32_t>(no));
    std::fclose(f);
    return r;
}

// @src 1402abe30  OTPRNG line (1402abd50) and OPSTOP list (1402a4020); always 0
int write_objproperties_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj,
                              std::uint32_t no)
{
    g2obj::write_otprng_line(m, f, obj, no);
    if (obj != nullptr) g2obj::ext::write_opstop(m, f, *obj, no); // 3 for a null obj, ignored
    return 0;
}

// @src 1401c6740  ObjPropertiesNNNNN.KEY: banner, 1402abe30 of object `no`
int write_objproperties_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    if (no_name(name)) return 1;
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    const int r = write_objproperties_key_stream(m, f, no);
    std::fclose(f);
    return r;
}

int write_objproperties_key_stream(model::DB_STEP_DATA& m, std::FILE* f, std::int32_t no)
{
    banner(m, f);
    model::LLOBJ_DATA* obj = object_no(m, no);
    return write_objproperties_block(m, f, obj, static_cast<std::uint32_t>(no));
}

// @src 1401c6b00  RzElmConNNNNN.KEY: banner, nodes (1402a2db0) and elements (1402a30c0) of a
// meshed object; 3 otherwise. Unlike the original (no name check; the file of a 3 is left open
// with the banner only) the port closes it after the same bytes. A missing object, which the
// original dereferences, also gives 3.
int write_rzelmcon_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    banner(m, f);
    model::LLOBJ_DATA* obj = object_no(m, no);
    if (obj == nullptr || !meshed_solid(obj)) {
        std::fclose(f);
        return 3;
    }
    g2obj::ext::write_nodes(m, f, *obj, static_cast<std::uint32_t>(no));
    g2obj::ext::write_elements(m, f, *obj, static_cast<std::uint32_t>(no));
    std::fclose(f);
    return 0;
}

// @src 1402abe90  edge/face BCs (1402abf20) and local BCs (1402ac560); `no` < 1 means obj+8
int write_bccdef_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA* obj, std::int32_t no)
{
    if (obj != nullptr) {
        const std::int32_t n = no < 1 ? field<std::int32_t>(obj, 8) : no;
        g2obj::ext::write_edge_face_bcs(m, f, *obj, static_cast<std::uint32_t>(n));
        g2obj::ext::write_local_bcs(m, f, *obj, static_cast<std::uint32_t>(n));
    }
    return 0;
}

// @src 1401c6c10  BccDefNNNNN.KEY: banner, 1402abe90 of a meshed object; 3 otherwise (file
// handling as write_rzelmcon_key_file)
int write_bccdef_key_file(model::DB_STEP_DATA& m, const char* name, std::int32_t no)
{
    std::FILE* f = open_wt(name);
    if (f == nullptr) return 1;
    banner(m, f);
    model::LLOBJ_DATA* obj = object_no(m, no);
    if (obj == nullptr || !meshed_solid(obj)) {
        std::fclose(f);
        return 3;
    }
    const int r = write_bccdef_block(m, f, obj, no);
    std::fclose(f);
    return r;
}

} // namespace def::key
