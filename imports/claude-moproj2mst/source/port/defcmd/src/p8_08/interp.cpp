// Packet p8-08: the interpolation drivers of _MeshGenBase (DEF_INT / DEF_REM3_GUI for DEFINT and
// REMESH, DEF_FRZ / DEF_FRZ3 for DEFFRZ), their exchange files and the read-back of the result
// KEY. Specs docs/re/commands/DEFINT.md, DEFFRZ.md; decompile deform-rea-work/out/p8mg/p8-08/all.c.
//
// The VS2010 std::string pieces of 140155c90 (find_last_of 140159300 / 1401592b0 / 140159dd0 with
// the memchr wrapper 1400ba8c0, substr 140006760, append 140159200 -> 140006730, c_str
// 140002ff0, strlen 1400010c0) are the C++ standard library here.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/interp.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <share.h>
#include <string>

#include <defcmd/file_util.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/key_file.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/p8_08_ext.hpp>
#include <defkey/kw_g2sv.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/node_access.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>
#include <defrt/stdlib/str_ends_with.hpp>

namespace def::cmd {

namespace {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

std::byte* bp(void* p) { return static_cast<std::byte*>(p); }
std::int32_t& i32(void* p, std::size_t off) { return *reinterpret_cast<std::int32_t*>(bp(p) + off); }

// object fields read here (LLOBJ_DATA offsets, DEFINT.md section 3)
std::int32_t obj_no(LLOBJ_DATA* o) { return i32(o, 0x8); }
std::int32_t obj_nnodes(LLOBJ_DATA* o) { return i32(o, 0x880); }
std::int32_t obj_nelem(LLOBJ_DATA* o) { return i32(o, 0x884); }
std::int32_t obj_npe(LLOBJ_DATA* o) { return i32(o, 0x894); }
model::Cls_14017ef20* node_set(LLOBJ_DATA* o) { return reinterpret_cast<model::Cls_14017ef20*>(o->s8a0); }
void* elem_set(LLOBJ_DATA* o) { return o->s9e0; }
// first source object (mg+0x4780 -> [0])
LLOBJ_DATA* source0(MeshGenBase& mg) { return *mg.u4780; }

// 14047b934: fopen = _fsopen(name, mode, _SH_DENYNO)
std::FILE* open_file(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }
// 1401b85d0
void rm(const char* name) { (void)def::dbio::gendb::remove_if_exists(name); }
// 1401b8560
bool readable(const char* name) { return def::dbio::file_exists(name); }

// 1401b5f00 (p8-07, <defcmd/key_file.hpp>)
void read_key(DB_STEP_DATA* m, const char* name, char reset, std::int32_t flags)
{
    (void)read_keyword_file(m, name, &reset, &flags);
}

// model dimension tests on DB_STEP_DATA+0x1c (1400015e0 / 1400015f0)
bool mdl_is_2d(const DB_STEP_DATA* m) { return def::rt::dimension_is_2(m); }

// NSTART (simulation control +0x160 = model+0x8b0)
std::int32_t& model_nstart(DB_STEP_DATA* m) { return i32(m, 0x8b0); }

// part of @src 140156640 / 140157040: the read-back when the source object is another object
// than the target (not reached by DEFINT/DEFFRZ/REMESH, which pass the target itself or use mode
// 2): read the KEY into a temporary model of dimension `dim` and copy its source object onto the
// target, keeping the target's type (+0xc) and name (+0x828, 0x40 bytes) from mg+0x10.
void read_back_other(MeshGenBase& mg, const char* name, std::int32_t dim)
{
    DB_STEP_DATA* model = mg.u3f48;
    DB_STEP_DATA* tmp = new DB_STEP_DATA(dim);  // 1404797d8(0x3408), 1401c8950
    model_copy_data(tmp, model);                // 1401d7390
    model_copy_layout_table(tmp, model->u15f8); // 1401d7320(tmp, model+0x15f8)
    read_key(tmp, name, 1, 0);
    model::llobj_copy::reset_keep_geometry(mg.u008);        // 140158f70
    const bool f9 = true, f8 = false, f7 = true, f6 = true, f5 = true, f4 = true;
    LLOBJ_DATA* src = model::object_list_find(&tmp->objects, obj_no(source0(mg)));  // 1401d6ea0
    model::llobj_copy_from(mg.u008, src, tmp->u15f8, &f4, &f5, &f6, &f7, &f8, &f9);  // 1401613a0
    i32(mg.u008, 0xc) = i32(mg.u010, 0xc);
    std::strncpy(reinterpret_cast<char*>(bp(mg.u008) + 0x828), reinterpret_cast<const char*>(bp(mg.u010) + 0x828), 0x40);
    (void)geolib::fun_140158fc0(tmp, 1);
}

} // namespace

// @src 140155c70
void interp_set_mode(MeshGenBase& mg, std::int32_t mode) { mg.u3f40 = mode; }

// @src 140157610
void interp_set_db_name(MeshGenBase& mg, const char* db)
{
    std::strncpy(mg.u4790, db, 0x400);
    mg.u4790[0x3ff] = '\0';  // +0x4b8f
    std::strcpy(mg.u4360, mg.u4790);
}

// @src 1401575a0
void interp_set_sources(MeshGenBase& mg, std::int32_t n, LLOBJ_DATA** objs, const char* db,
                        std::int32_t step)
{
    mg.u4778 = n;
    mg.u4780 = objs;
    interp_set_db_name(mg, db);
    mg.u4b90 = step;
}

// @src 140157670
void interp_set_value(MeshGenBase& mg, double value) { mg.u4788 = value; }

// @src 140180a40  (1401808d0 with key 3 and the slot at +0x20)
double* node_set_array3(model::Cls_14017ef20* set)
{
    std::int32_t* dims = model::fun_14017f020(set);
    model::fun_14017ffe0(set, set->n, dims, 3);  // n = set+0x12c
    if (set->u130 != nullptr) {
        std::int32_t which = 3;
        model::fun_1401d48d0(static_cast<DB_STEP_DATA*>(set->u130), &which);
    }
    return static_cast<double*>(set->u010[2]);
}

// ---- 2D: DEF_INT ---------------------------------------------------------------------------

// @src 1401562f0  DEF_REM.INI
bool write_def_rem_ini(MeshGenBase& mg)
{
    std::FILE* f = open_file("DEF_REM.INI", "wt");  // str 14051e3ec
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s\n", mg.u4790);
    rt::crt::fprintf(f, "%d\n", mg.u4b90);
    rt::crt::fprintf(f, "%d\n", 2);
    rt::crt::fprintf(f, "%d\n", obj_no(source0(mg)));
    std::fclose(f);
    return true;
}

// @src 1401563c0  2D mesh of mg+8: <prob>.RMS (modes 0/1) or DMESH.DAT (mode 2)
bool write_mesh_2d(MeshGenBase& mg, const char* name)
{
    std::FILE* f = open_file(name, "wt");  // str 14051e494
    if (f == nullptr) return false;
    LLOBJ_DATA* o = mg.u008;
    const bool mode01 = mg.u3f40 == 0 || mg.u3f40 == 1;
    if (mode01) {
        rt::crt::fprintf(f, "%10d %10d\n", obj_nnodes(o), 1);
    } else {
        rt::crt::fprintf(f, "%10d\n", obj_nnodes(o));
    }
    const double* xy = model::fun_1401808d0(node_set(o));
    for (std::int32_t i = 0; i < obj_nnodes(o); i = i + 1) {
        rt::crt::fprintf(f, "%10d %15.7f %15.7f\n", i + 1, xy[i * 2], xy[i * 2 + 1]);
    }
    if (mode01) {
        // str 14051e500. The original's _output_l (14048ccb4, state table __lookuptable
        // 14056fb30, used at 14048ce9d) takes '%' + '\n' as an unknown conversion: state PERCENT,
        // class OTHER -> NORMAL, so '\n' is written and the '%' is dropped (no argument
        // consumed). Bytes: "%10d " of nelem, then "\n" (pinned by test_p8_08_files).
        rt::crt::fprintf(f, "%10d %\n", obj_nelem(o));
    } else {
        rt::crt::fprintf(f, "%10d %5d\n", obj_nelem(o), 4);
    }
    const std::int32_t* con = p8_08_ext::element_connectivity(elem_set(o));  // 140183cb0
    for (std::int32_t e = 0; e < obj_nelem(o); e = e + 1) {
        rt::crt::fprintf(f, "%10d ", e + 1);
        for (std::int32_t k = 0; k < 4; k = k + 1) {
            rt::crt::fprintf(f, "%10d ", con[e * 4 + k]);
        }
        rt::crt::fprintf(f, "\n");
    }
    std::fclose(f);
    return true;
}

// @src 140156640  2D read-back of the result KEY
bool read_back_2d(MeshGenBase& mg, const char* name)
{
    if (!readable(name)) return false;
    if (mg.u3f40 == 2) {
        clear_node_forces(mg);  // 1401573c0
        read_key(mg.u3f48, name, 1, 0);
    } else if (obj_no(source0(mg)) == obj_no(mg.u008)) {
        read_key(mg.u3f48, name, 0, 0xf);
    } else {
        read_back_other(mg, name, 2);
    }
    return true;
}

// @src 140155c90  2D interpolation through DEF_INT
bool interp_2d(MeshGenBase& mg, const char* db)
{
    mg.u3f40 = 1;
    std::string s;  // 140005040
    if (db == nullptr || *db == '\0') {
        std::strcpy(mg.u4360, "DEFORM");
    } else if (db != mg.u4360) {
        std::strcpy(mg.u4360, db);  // 140157780 passes mg+0x4360 itself (strcpy onto itself)
    }
    char rms[1024];
    char act[304];
    rt::crt::sprintf(rms, "%s.RMS", mg.u3f60);
    rt::crt::sprintf(act, "\n5\nY\n1\n%s\n\n6\n\n", rms);
    std::FILE* f = open_file("DEF_INT.ACT", "wt");  // str 14051e3a0
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s", act);
    std::fclose(f);
    if (!write_def_rem_ini(mg)) return false;

    // output name: the DB name after its last '/' or '\', "<stem>_REM.KEY" when it ends in .DB
    s = std::string(mg.u4360);
    std::int32_t idx = static_cast<std::int32_t>(s.find_last_of("/\\"));  // 140159300, str 14051e3b8
    if (idx == -1) {
        idx = static_cast<std::int32_t>(s.find_last_of(fun_1401b17b0()));  // 1401592b0
    }
    if (idx != -1) {
        s = s.substr(static_cast<std::size_t>(idx + 1));  // 140006760
    }
    if (def::rt::str_ends_with(s, ".DB")) {  // 1401b0cc0, str 14051e3bc
        const int n = static_cast<int>(s.size());
        s = fun_1401b1520(s, n - 3);
        s += "_REM.KEY";  // 140159200
    }
    char out[1024];
    std::strcpy(out, s.c_str());

    if (!write_mesh_2d(mg, rms)) {
        rm("DEF_INT.ACT");
        rm("DEF_REM.INI");
        return false;
    }
    rm("DEF_INT.OUT");
    rm(out);
    (void)write_text_file("DEFORM_UIRUNREMESH.DAT", "2D");  // 1400b9130, str 14051e3dc
    char cmd[208];
    rt::crt::sprintf(cmd, "%s < %s > %s", "DEF_INT", "DEF_INT.ACT", "DEF_INT.OUT");
    launch_program_2d(1, cmd);  // 140224100
    rm("DEFORM_UIRUNREMESH.DAT");
    char tmp[208];
    rt::crt::sprintf(tmp, "%s.GEO", mg.u3f60);
    rm(tmp);
    rt::crt::sprintf(tmp, "%s.RES", mg.u3f60);
    rm(tmp);
    rm(rms);
    rm("DEF_INT.ACT");
    rm("DEF_INT.OUT");
    rm("REM_SCR.SCR");
    rm("NEW.MSH");
    rm("STEP.DAT");
    if (!readable(out)) return false;
    const bool ok = read_back_2d(mg, out);
    rm(out);
    return ok;
}

// ---- 3D: DEF_REM3_GUI ----------------------------------------------------------------------

// @src 140156ba0  DEF_REM3.INI
bool write_def_rem3_ini(MeshGenBase& mg)
{
    std::FILE* f = open_file("DEF_REM3.INI", "wt");  // str 14051e544
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s\n", mg.u3f60);
    rt::crt::fprintf(f, "%s\n", mg.u4790);
    rt::crt::fprintf(f, "%d\n", mg.u4b90);
    rt::crt::fprintf(f, "%d\n", obj_no(source0(mg)));
    rt::crt::fprintf(f, "%d\n", 1);
    if (mg.u3f40 == 0) {
        rt::crt::fprintf(f, "%d\n", 2);
    } else if (mg.u3f40 == 1) {
        rt::crt::fprintf(f, "%d\n", 0x66);
    }
    std::fclose(f);
    return true;
}

// @src 140156cd0  3D mesh of mg+8 as a KEY fragment (SCRATCH.KEY, modes 0/1) or DMESH.DAT (mode 2)
bool write_mesh_3d(MeshGenBase& mg, const char* name)
{
    std::FILE* f = open_file(name, "wt");  // str 14051e614
    if (f == nullptr) return false;
    LLOBJ_DATA* o = mg.u008;
    const std::int32_t src_no = obj_no(source0(mg));
    const bool mode01 = mg.u3f40 == 0 || mg.u3f40 == 1;
    if (mode01) {
        rt::crt::fprintf(f, "RZ %10d %10d\n", src_no, obj_nnodes(o));
    } else {
        rt::crt::fprintf(f, "%10d\n", obj_nnodes(o));
    }
    const double* xyz = model::fun_1401808d0(node_set(o));
    for (std::int32_t i = 0; i < obj_nnodes(o); i = i + 1) {
        rt::crt::fprintf(f, "%10d %15.7f %15.7f %15.7f\n", i + 1, xyz[i * 3], xyz[i * 3 + 1], xyz[i * 3 + 2]);
    }
    const std::int32_t npe = obj_npe(o);
    if (mode01) {
        rt::crt::fprintf(f, "ELMCON %10d %10d %5d\n", src_no, obj_nelem(o), npe);
    } else {
        rt::crt::fprintf(f, "%10d %5d\n", obj_nelem(o), npe);
    }
    const std::int32_t* con = p8_08_ext::element_connectivity(elem_set(o));  // 140183cb0
    for (std::int32_t e = 0; e < obj_nelem(o); e = e + 1) {
        rt::crt::fprintf(f, "%10d ", e + 1);
        for (std::int32_t k = 0; k < npe; k = k + 1) {
            rt::crt::fprintf(f, "%10d ", con[npe * e + k]);
        }
        rt::crt::fprintf(f, "\n");
    }
    const std::int32_t* grp = p8_08_ext::element_material_groups(elem_set(o));  // 140183d00
    const char* kw = model::keyword_name(0xae);  // 1402f7720: MTLGRP
    (void)def::key::g2sv::write_node_elem_int(f, kw, static_cast<std::uint32_t>(obj_no(o)), grp,
                                         obj_nelem(o), 1, 1);  // 1402f4df0
    std::fclose(f);
    return true;
}

// @src 140157040  3D read-back (NEWMESH.KEY / SCRATCH.KEY; FLDGEO.KEY after a same-object read)
bool read_back_3d(MeshGenBase& mg, const char* name)
{
    if (!readable(name)) return false;
    if (mg.u3f40 == 2) {
        clear_node_forces(mg);
        read_key(mg.u3f48, name, 1, 0);
    } else if (obj_no(source0(mg)) == obj_no(mg.u008)) {
        read_key(mg.u3f48, name, 0, 0xf);
        if (readable("FLDGEO.KEY")) {
            read_key(mg.u3f48, "FLDGEO.KEY", 0, 0);
            rm("FLDGEO.KEY");
        }
    } else {
        read_back_other(mg, name, 3);
    }
    return true;
}

// @src 140156950  3D interpolation through DEF_REM3_GUI (the second argument is not used). On
// this PC DEF_REM3_GUI cannot start (its 32-bit mpich.dll is missing), so NEWMESH.KEY never
// appears and both the original and the port report Failure.
bool interp_3d(MeshGenBase& mg, const char*)
{
    mg.u3f40 = 1;
    char act[304];
    rt::crt::sprintf(act, "\n");
    std::FILE* f = open_file("DEF_INT.ACT", "wt");  // str 14051e4e4
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s", act);
    std::fclose(f);
    if (!write_def_rem3_ini(mg)) return false;
    if (!write_mesh_3d(mg, "SCRATCH.KEY")) {
        rm("DEF_INT.ACT");
        rm("DEF_REM3.INI");
        return false;
    }
    rm("DEF_INT.OUT");
    char cmd[208];
    rt::crt::sprintf(cmd, "%s < %s > %s", "DEF_REM3_GUI", "DEF_INT.ACT", "DEF_INT.OUT");
    launch_program_3d(1, cmd);  // 140224130
    char tmp[208];
    rt::crt::sprintf(tmp, "%s.GEO", mg.u3f60);
    rm(tmp);
    rt::crt::sprintf(tmp, "%s.RES", mg.u3f60);
    rm(tmp);
    rm("NBCD.DAT");
    rm("SCRATCH.KEY");
    rm("DEF_INT.ACT");
    rm("DEF_INT.OUT");
    if (!readable("NEWMESH.KEY")) return false;
    const bool ok = read_back_3d(mg, "NEWMESH.KEY");
    rm("NEWMESH.KEY");
    return ok;
}

// ---- force interpolation: DEF_FRZ / DEF_FRZ3 -----------------------------------------------

// @src 1401573c0  before the force read-back: for each of nnodes x (model+0x1604) node values,
// a free code (obj+0x990 == 0) with |value| > 1e-6 (DAT_14051f1b8) clears obj+0x998; the value
// (node array 140180a40) is set to 0
void clear_node_forces(MeshGenBase& mg)
{
    LLOBJ_DATA* o = mg.u008;
    const std::int32_t n = obj_nnodes(o);
    const std::int32_t k = mg.u3f48->u15f8[3];  // model+0x1604
    for (std::int32_t i = 0; i < n * k; i = i + 1) {
        std::int32_t* codes = *reinterpret_cast<std::int32_t**>(bp(o) + 0x990);
        if (codes[i] == 0) {
            const double* v = node_set_array3(node_set(o));
            const double a = std::fabs(v[i]);  // 140484558 (CRT fabs; NaN compares false below)
            if (1e-6 < a) {
                (*reinterpret_cast<std::int32_t**>(bp(o) + 0x998))[i] = 0;
            }
        }
        double* v = node_set_array3(node_set(o));
        v[i] = 0.0;
    }
}

// part of @src 1401577e0 / 140157830: 2D (model+0x1c == 2) or 3D routine
namespace {
bool read_back(MeshGenBase& mg, const char* name)  // @src 140157830
{
    return mdl_is_2d(mg.u3f48) ? read_back_2d(mg, name) : read_back_3d(mg, name);
}
} // namespace

// @src 140157890  2D force interpolation (DEF_FRZ)
bool frz_2d(MeshGenBase& mg)
{
    std::FILE* f = open_file("FRZ1.INI", "wt");  // str 14051e634
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s\n", mg.u4790);
    rt::crt::fprintf(f, "%d\n", mg.u4b90);
    rt::crt::fprintf(f, "%d\n", obj_no(mg.u008));
    rt::crt::fprintf(f, "%10d ", mg.u4778);
    for (std::int32_t i = 0; i < mg.u4778; i = i + 1) {
        rt::crt::fprintf(f, "%10d  ", obj_no(mg.u4780[i]));
    }
    rt::crt::fprintf(f, "\n");
    rt::crt::fprintf(f, "%f\n", mg.u4788);
    std::fclose(f);
    if (!write_mesh_2d(mg, "DMESH.DAT")) {
        rm("FRZ1.INI");
        return false;
    }
    rm("DEF_FRZ2.OUT");
    rm("DEF_FRZI.TMP");
    char cmd[208];
    rt::crt::sprintf(cmd, "%s > %s", "DEF_FRZ", "DEF_FRZ2.OUT");
    launch_program_2d(1, cmd);
    rm("DEF_FRZ2.OUT");
    rm("DMESH.DAT");
    rm("FRZ1.INI");
    if (!readable("DEF_FRZI.KEY")) return false;
    bool ok = read_back(mg, "DEF_FRZI.KEY");
    rm("DEF_FRZI.KEY");
    std::FILE* t = open_file("DEF_FRZI.TMP", "rt");  // str 14051e830
    if (t == nullptr) return false;
    (void)std::fscanf(t, "%lf\n", &mg.u4b98[0]);
    (void)std::fscanf(t, "%lf %lf\n", &mg.u4b98[1], &mg.u4b98[2]);
    (void)std::fscanf(t, "%lf %lf\n", &mg.u4b98[3], &mg.u4b98[4]);
    std::fclose(t);
    rm("DEF_FRZI.TMP");
    return ok;
}

// @src 140157be0  3D force interpolation (DEF_FRZ3)
bool frz_3d(MeshGenBase& mg)
{
    const std::int32_t line2 = mg.u4bd0 ? 1 : 0;
    std::FILE* f = open_file("FRZ3.INI", "wt");  // str 14051e880
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%s\n", mg.u3f60);
    rt::crt::fprintf(f, "%d\n", line2);
    rt::crt::fprintf(f, "%s\n", mg.u4790);
    rt::crt::fprintf(f, "%d\n", mg.u4b90);
    rt::crt::fprintf(f, "%d\n", obj_no(mg.u008));
    if (!mdl_is_2d(mg.u3f48)) {
        rt::crt::fprintf(f, "%d\n", obj_no(source0(mg)));
    } else {
        rt::crt::fprintf(f, "%10d ", mg.u4778);
        for (std::int32_t i = 0; i < mg.u4778; i = i + 1) {
            rt::crt::fprintf(f, "%10d  ", obj_no(mg.u4780[i]));
        }
        rt::crt::fprintf(f, "\n");
    }
    rt::crt::fprintf(f, "%f\n", mg.u4788);
    std::fclose(f);
    if (!write_mesh_3d(mg, "DMESH.DAT")) {
        rm("FRZ3.INI");
        return false;
    }
    rm("DEF_FRZ3.OUT");
    rm("SCRATCH.KEY");
    rm("SCRATCH.DAT");
    char cmd[208];
    rt::crt::sprintf(cmd, "%s > %s", "DEF_FRZ3", "DEF_FRZ3.OUT");
    launch_program_3d(1, cmd);
    rm("DEF_FRZ3.OUT");
    rm("DMESH.DAT");
    rm("FRZ3.INI");
    if (!readable("SCRATCH.KEY")) return false;
    const bool ok = read_back(mg, "SCRATCH.KEY");
    rm("SCRATCH.KEY");
    std::FILE* t = open_file("SCRATCH.DAT", "rt");  // str 14051e8c4
    if (t == nullptr) return false;
    (void)std::fscanf(t, "%lf\n", &mg.u4b98[0]);
    (void)std::fscanf(t, "%lf %lf %lf\n", &mg.u4b98[1], &mg.u4b98[2], &mg.u4b98[3]);
    (void)std::fscanf(t, "%lf %lf %lf\n", &mg.u4b98[4], &mg.u4b98[5], &mg.u4b98[6]);
    std::fclose(t);
    rm("SCRATCH.DAT");
    return ok;
}

// @src 140157780  mode 0/1: 2D 140155c90 / 3D 140156950, both with mg+0x4360
// @src 1401577e0  mode 2: 2D 140157890 / 3D 140157be0
// @src 1401576a0
bool object_interpolation(MeshGenBase& mg, bool flag)
{
    mg.u4bd0 = flag;
    if (mg.u4780 == nullptr) {
        rt::crt::fprintf(stdout, "Error in _objectInterpolation: need to set the objects to interpolate from.\n");
        return false;
    }
    const std::int32_t saved = model_nstart(mg.u3f48);
    const std::int32_t mode = mg.u3f40;
    bool r;
    if (mode == 2) {
        r = mdl_is_2d(mg.u3f48) ? frz_2d(mg) : frz_3d(mg);
    } else {
        // modes 0, 1 and every other value
        r = mdl_is_2d(mg.u3f48) ? interp_2d(mg, mg.u4360) : interp_3d(mg, mg.u4360);
    }
    model_nstart(mg.u3f48) = saved;
    return r;
}

} // namespace def::cmd
