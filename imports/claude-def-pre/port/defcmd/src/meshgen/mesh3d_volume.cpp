// The 3D solid (volume) mesh chain of _MeshGenBase (packet p8-06): the vtable +0x10 step, the
// MESH3D_GUI / BRKMSH / CHKVOL launches with their exchange files and the SCRATCH.KEY read-back.
// Spec: docs/re/commands/DEFAMG.md §4.2, §6.3; decompile deform-rea-work/out/p8mg/p8-06/all.c,
// listing asm.txt there. Strings are copied from 0x14051d35c..0x14051dbc8 (cited per line).
//
// The _MeshGenBase hooks are virtual calls as in the original: +0x48 progress(), +0x50 vslot50(),
// +0x58 vslot58() (owner p8-07; empty / 0x14 in the base class, no subclass in batch).
#include <defcmd/meshgen/mesh3d.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <share.h>
#include <string>
#include <vector>

#include <defcmd/contact_cmds.hpp>
#include <defcmd/file_util.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/p8_06_ext.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/node_access.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/crt_emulation.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/str_helpers.hpp>

namespace def::cmd {

namespace {

using model::LLOBJ_DATA;
namespace gk = model::geo_kw;

template <class T>
T& at(LLOBJ_DATA* obj, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(obj) + off);
}

// 14047b934: _fsopen(name, mode, _SH_DENYNO)
std::FILE* fopen_crt(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }

// 1401b8560 / 1401b85d0 (ported in defdbio)
bool exists(const char* name) { return dbio::file_exists(name); }
void remove_file(const char* name) { dbio::gendb::remove_if_exists(name); }

// 1401808d0 on obj+0x8a0: node coordinates
double* node_coords(LLOBJ_DATA* obj)
{
    return model::fun_1401808d0(reinterpret_cast<model::Cls_14017ef20*>(reinterpret_cast<unsigned char*>(obj) + 0x8a0));
}

// 140183cb0 on obj+0x9e0: connectivity; owner defcmd contact/contact_cmds.cpp element_set_conn (p67)
std::int32_t* connectivity(LLOBJ_DATA* obj)
{
    return element_set_conn(reinterpret_cast<unsigned char*>(obj) + 0x9e0);
}

// @src 140159260  s.find(pattern, pos) (140002810 size, 140002860 c_str, 1400bc080 find)
std::size_t fun_140159260(const std::string& s, const std::string& pattern, std::size_t pos)
{
    return s.find(pattern.c_str(), pos, pattern.size());
}

// part of @src 140142f10: one line of SCRATCH.KEY contains `key` after the trim 1401b1060
// (std::string(line) 1400066a0, copy 140005dd0, str_trim, 140159260 != npos 14051f1d8)
bool line_has(const char* line, const char* key)
{
    const std::string trimmed = rt::str_trim(std::string(line));
    return fun_140159260(trimmed, std::string(key), 0) != std::string::npos;
}

} // namespace

// @src 140140110  _MeshGenBase vtable +0x10: 3D volume step
int mesh3d_volume_step(MeshGenBase& mg, char flag)
{
    LLOBJ_DATA* obj = mg.u008;
    if (obj == nullptr) return 0x14;
    if (!gk::has_surface_flag(*obj) && at<std::int32_t>(obj, 0x87c) != 0) {  // 140160d90
        char one = 1;
        p806_ext::fun_1401fc730(obj, &one);
    }
    // CHK_VOL.INI: TRGVOL int obj+0x2098, real obj+0x2090
    if (exists("CHK_VOL.INI") && at<std::int32_t>(obj, 0x2098) != -1 &&       // str 14051d788
        at<double>(obj, 0x2090) > 0.0) {                                       // 14051bed8
        std::FILE* f = fopen_crt("CHK_VOL.INI", "rt");                         // 14051d798, 14051d35c
        if (f != nullptr) {
            double v;  // uninitialised in the original when the read fails
            std::fscanf(f, "%lf\n", &v);                                       // 14051d7a4
            std::fclose(f);
            if (std::fabs(v - at<double>(obj, 0x2090)) > 1.0e-6) {             // 140484558, 14051f1b8
                f = fopen_crt("CHK_VOL.INI", "wt");                            // 14051d7b0, 14051d3dc
                // 14014025f: not checked; fprintf(NULL) ends in the CRT's invalid-parameter handler
                if (f == nullptr) rt::abnormal_exit(rt::kStatusInvalidCrtParameter);
                rt::crt::fprintf(f, "%g\n", at<double>(obj, 0x2090));          // 14051d3ec
                std::fclose(f);
            }
        }
    }
    if (exists("crossed.faces.0")) remove_file("crossed.faces.0");             // 14051d7c0 / 7d0
    const std::int32_t r = mesh3d_run_volume_mesher(mg);
    if (r != 0) return r;
    remove_file("crossed.faces.0");                                            // 14051d7e0
    remove_file("MESH3D.MSG");                                                 // 14051d7f0
    mesh3d_coating_step(mg);
    if (flag != 0 && g_amg_dxt_ok == 0) fun_140141580(mg);
    fun_1401443c0(mg);
    fun_1401444f0(mg);
    return 0;
}

// @src 140140310  coating step (vtable +0x48 progress around 1401403c0)
void mesh3d_coating_step(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    if (at<std::int32_t>(obj, 0x2de8) <= 0) return;  // always taken in batch (p8-06.tsv)
    g_mesher_progress = 0xb;
    mg.progress();
    const std::int32_t r = mesh3d_coating(mg, at<std::int32_t>(obj, 0x2de8), at<double*>(obj, 0x2df0),
                                          at<std::int32_t*>(obj, 0x2df8));
    g_mesher_progress = r != 0 ? 4 : 0xc;
    mg.progress();
}

// @src 1401403c0  not ported: the coating layer count obj+0x2de8 is never > 0 in batch, so
// 140140310 never calls this (proof in docs/re/status/p8-06.tsv)
std::int32_t mesh3d_coating(MeshGenBase&, std::int32_t, const double*, const std::int32_t*)
{
    DEF_NOT_PORTED(0x1401403c0);
}

// @src 140141d50  volume mesher run and read-back
std::int32_t mesh3d_run_volume_mesher(MeshGenBase& mg)
{
    const char act[] = "5\nN\n6\ne\nY\n";  // 14051d830, 0xb bytes copied
    std::int32_t r = 0;
    mesh3d_cleanup(mg);
    std::FILE* f = fopen_crt("DEF_MGN.ACT", "wt");                             // 14051d840, 14051d45c
    if (f == nullptr) {
        mg.vslot50();
        return 6;
    }
    rt::crt::fprintf(f, "%s", act);                                            // 14051d48c
    std::fclose(f);
    f = fopen_crt("MESH3D.SCR", "wt");                                         // 14051d850, 14051d49c
    if (f == nullptr) return 6;
    rt::crt::fprintf(f, "0\n");                                                // 14051d594
    rt::crt::fprintf(f, "%s\n", "MESH3D.INP");                                 // 14051d5a4, 14051d860
    rt::crt::fprintf(f, "0\n");                                                // 14051d5ec
    std::fclose(f);
    r = mesh3d_write_inp(mg);
    if (r != 0) return r;
    g_mesher_progress = 8;
    mg.progress();
    write_text_file("DEFORM_UIRUNREMESH.DAT", "3D");                           // 14051d870, 14051d5fc
    char cmd[112];
    std::sprintf(cmd, "MESH3D_GUI");                                           // 14051d888
    launch_program_3d(1, cmd);
    remove_file("DEFORM_UIRUNREMESH.DAT");                                     // 14051d898
    if (!exists("mesh.out")) {                                                 // 14051d8b0
        fun_14013f410(mg, 1, 0);
        if (!exists("SCRATCH.KEY")) return 7;                                  // 14051d8c0
        const bool ok = mesh3d_read_scratch_key(mg);
        remove_file("SCRATCH.KEY");                                            // 14051d8d0
        remove_file("MESH3D.MSG");                                             // 14051d8e0
        if (ok) {  // 140141f9e: compared with 1
            mesh3d_set_volume_read(mg);
            return 0;
        }
        return 0xc;
    }
    remove_file("MESH3D.MSG");                                                 // 14051d8f0
    f = fopen_crt("BRKMSH.INI", "wt");                                         // 14051d900, 14051d65c
    if (f == nullptr) return 6;
    rt::crt::fprintf(f, "%10d %10d\n", 4, at<std::int32_t>(mg.u008, 8));       // 14051d910
    rt::crt::fprintf(f, "%s\n", "BRKMSH.KEY");                                 // 14051d68c, 14051d920
    std::fclose(f);
    g_mesher_progress = 9;
    mg.progress();
    std::sprintf(cmd, "BRKMSH > %s", "BRKMSH.MSG");                            // 14051d940, 14051d930
    launch_program_3d(1, cmd);
    remove_file("BRKMSH.KEY");                                                 // 14051d950
    remove_file("BRKMSH.MSG");                                                 // 14051d960
    if (!exists("SCRATCH.KEY")) return 8;                                      // 14051d970
    double target = 0.0;
    f = fopen_crt("CHK_VOL.INI", "rt");                                        // 14051d980, 14051d794
    if (f != nullptr) {
        std::fscanf(f, "%lf", &target);                                        // 14051d7bc
        std::fclose(f);
    }
    if (target > 0.0) {                                                        // 14051bed8
        g_mesher_progress = 10;
        mg.progress();
        std::sprintf(cmd, "CHKVOL > %s", "CHK_VOL.MSG");                       // 14051d9a0, 14051d990
        launch_program_3d(1, cmd);
        remove_file("CHK_VOL.MSG");                                            // 14051d9b0
    }
    if (!exists("SCRATCH.KEY")) {                                              // 14051d9c0
        mesh3d_cleanup(mg);
        return 9;
    }
    const bool ok = mesh3d_read_scratch_key(mg);
    mesh3d_cleanup(mg);
    if (!ok) return 0xc;
    mesh3d_set_volume_read(mg);
    g_mesher_progress = 4;
    mg.progress();
    return 0;
}

// @src 140142680  delete the volume mesher's exchange files
void mesh3d_cleanup(MeshGenBase& mg)
{
    char name[208];
    remove_file("MESH3D.SCR");   // 14051da58
    remove_file("MESH3D.INP");   // 14051da68
    remove_file("MESH3D.MSG");   // 14051da78
    remove_file("BRKMSH.INI");   // 14051da88
    remove_file("SCRATCH.KEY");  // 14051da98
    remove_file("SCRATCH.DAT");  // 14051daa8
    remove_file("DEF_MGN.OUT");  // 14051dab8
    remove_file("BRKMSH.INI");   // 14051dac8
    std::sprintf(name, "%s_%03d.KEY", mg.u3f60, at<std::int32_t>(mg.u008, 8));  // 14051dad8
    remove_file(name);
    std::sprintf(name, "%s.GEO", mg.u3f60);  // 14051dae4
    remove_file(name);
    std::sprintf(name, "%s.DST", mg.u3f60);  // 14051daec
    remove_file(name);
    std::sprintf(name, "%s.RES", mg.u3f60);  // 14051daf4
    remove_file(name);
}

// @src 1401427e0  MESH3D.INP: state 0 the surface geometry 0 (quads split), else the object's
// surface polygons obj+0xbf8 with their nodes renumbered in order of first use
std::int32_t mesh3d_write_inp(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    std::FILE* f = fopen_crt("MESH3D.INP", "wt");  // 14051db40, 14051d84c
    if (f == nullptr) return 6;
    std::int32_t nnode = 0;
    std::int32_t npoly = 0;
    std::vector<double> xyz;
    std::vector<std::int32_t> poly;
    if (g_mesher_state == 0) {
        const std::int32_t h = mg.vslot58();
        if (h == 0x11) return 0x11;  // 140142869: the file stays open (never: +0x58 is 0x14)
        model::OBJ_GEO3D* s = gk::surface(*obj, 0);  // 140198620(obj, &0), four times
        if (s == nullptr) rt::abnormal_exit(rt::kStatusAccessViolation);  // 1400034d0 reads it
        nnode = s->node_count();  // 1400034d0
        npoly = s->elem_count();  // 140003500
        xyz.assign(static_cast<std::size_t>(nnode * 3), 0.0);  // 140257150
        poly.assign(static_cast<std::size_t>(npoly << 2), 0);  // 140256dc0
        if (nnode * 3 != 0) std::memcpy(xyz.data(), gk::geo3d_coords(s), static_cast<std::size_t>(nnode * 3) * 8);
        if ((npoly << 2) != 0) std::memcpy(poly.data(), gk::geo3d_elems(s), static_cast<std::size_t>(npoly << 2) * 4);
        // 140142988: poly[3] of the first polygon; no polygons = a NULL buffer in the original
        if (poly.empty()) rt::abnormal_exit(rt::kStatusAccessViolation);
        if (poly[0] != std::abs(poly[3])) {  // 140484ab8: the first polygon is a quad
            poly.resize(static_cast<std::size_t>((npoly << 1) << 2));  // 140256e20 realloc
            for (std::int32_t k = 0; k < npoly; k = k + 1) {
                std::int32_t* p = poly.data() + (k << 2);
                std::int32_t* q = poly.data() + ((npoly + k) << 2);
                if (p[3] < 1) {
                    q[0] = p[0];
                    q[1] = p[2];
                    q[2] = std::abs(p[3]);
                    q[3] = -p[0];
                    p[3] = -p[0];
                } else {
                    q[0] = p[0];
                    q[1] = p[2];
                    q[2] = p[3];
                    q[3] = p[0];
                    p[3] = p[0];
                }
            }
            npoly = npoly << 1;
        }
    } else {
        const std::int32_t n = at<std::int32_t>(obj, 0x880);
        npoly = at<std::int32_t>(obj, 0xbf0);
        xyz.assign(static_cast<std::size_t>(n * 3), 0.0);       // 140257150
        poly.assign(static_cast<std::size_t>(npoly << 2), 0);   // 140256dc0
        std::vector<std::int32_t> map(static_cast<std::size_t>(n), 0);  // 140159ad0 / 140159b60
        const std::int32_t* src = at<std::int32_t*>(obj, 0xbf8);
        const double* c = node_coords(obj);
        std::int32_t used = 0;
        if (n == 0) return 7;  // 140142a.. : the file stays open, as in the original
        for (std::int32_t k = 0; k < npoly; k = k + 1) {
            for (std::int32_t j = 0; j < 4; j = j + 1) {
                const std::int32_t id = src[j + k * 4];
                if (map[static_cast<std::size_t>(id - 1)] == 0) {
                    std::memcpy(xyz.data() + used * 3, c + (id - 1) * 3, 0x18);
                    used = used + 1;
                    map[static_cast<std::size_t>(id - 1)] = used;
                }
                poly[static_cast<std::size_t>(j + k * 4)] = map[static_cast<std::size_t>(id - 1)];
            }
        }
        nnode = used;
    }
    rt::crt::fprintf(f, "%10d\n", nnode);  // 14051db4c
    for (std::int32_t i = 0; i < nnode; i = i + 1) {
        rt::crt::fprintf(f, "%10d %15.7f %15.7f %15.7f \n", i + 1, xyz[static_cast<std::size_t>(i * 3)],
                         xyz[static_cast<std::size_t>(i * 3 + 1)], xyz[static_cast<std::size_t>(i * 3 + 2)]);  // 14051db00
    }
    rt::crt::fprintf(f, "%10d\n", npoly);  // 14051db54
    for (std::int32_t i = 0; i < npoly; i = i + 1) {
        rt::crt::fprintf(f, "%10d %10d %10d %10d %10d\n", i + 1, poly[static_cast<std::size_t>(i << 2)],
                         poly[static_cast<std::size_t>(i * 4 + 1)], poly[static_cast<std::size_t>(i * 4 + 2)],
                         poly[static_cast<std::size_t>(i * 4 + 3)]);  // 14051db20
    }
    std::fclose(f);
    return 0;
}

// @src 140142f10  SCRATCH.KEY: the first line containing RZ gives the node count, then the nodes;
// the next line containing ELMCON the element count and nodes per element (4 or 8), then the
// elements. The mesh is allocated whenever both counts are >= 1, even after a short read.
bool mesh3d_read_scratch_key(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    std::FILE* f = fopen_crt("SCRATCH.KEY", "rt");  // 14051db60, 14051d8cc
    if (f == nullptr) return false;
    bool ok = true;
    const int line_max = 500;
    char line[512];
    bool found = false;
    while (p806_ext::fun_1401b83d0(line, line_max, f) != 0) {
        if (line_has(line, "RZ")) {  // 14051d8dc
            found = true;
            break;
        }
    }
    if (!found) return false;  // the file stays open, as in the original
    // The counts are uninitialised locals in the original when sscanf cannot assign them; 0 here.
    std::int32_t nnode = 0;
    std::vector<double> xyz;
    std::sscanf(line, "%*s %*d %d\n", &nnode);  // 14051db70
    if (nnode < 1 || 1.0e20 <= static_cast<double>(nnode)) {  // 14051f1b0
        ok = false;
    } else {
        xyz.assign(static_cast<std::size_t>(nnode * 3), 0.0);  // 140257150
        // one set of locals for the whole loop (a short line keeps the previous values)
        double x = 0.0, y = 0.0, z = 0.0;
        for (std::int32_t i = 0; i < nnode; i = i + 1) {
            if (std::fscanf(f, "%*d %lf %lf %lf\n", &x, &y, &z) == -1) {  // 14051db80
                ok = false;
                break;
            }
            xyz[static_cast<std::size_t>(i * 3)] = x;
            xyz[static_cast<std::size_t>(i * 3 + 1)] = y;
            xyz[static_cast<std::size_t>(i * 3 + 2)] = z;
        }
    }
    found = false;
    while (p806_ext::fun_1401b83d0(line, line_max, f) != 0) {
        if (line_has(line, "ELMCON")) {  // 14051db94
            found = true;
            break;
        }
    }
    if (!found) return false;  // the file stays open, as in the original
    std::int32_t nelem = 0;
    std::int32_t npe = 0;
    std::vector<std::int32_t> conn;
    std::sscanf(line, "%*s %*d %d %d\n", &nelem, &npe);  // 14051dba0
    if (nelem < 1 || 1.0e20 <= static_cast<double>(nelem)) {
        ok = false;
    } else {
        conn.assign(static_cast<std::size_t>(nelem * npe), 0);  // 140256dc0
        std::int32_t v[8] = {};  // one set of locals for the whole loop, as x, y, z above
        for (std::int32_t e = 0; e < nelem; e = e + 1) {
            if (npe == 4) {
                if (std::fscanf(f, "%*d %d %d %d %d\n", &v[0], &v[1], &v[2], &v[3]) == -1) {  // 14051dbb0
                    ok = false;
                    break;
                }
                for (int j = 0; j < 4; j = j + 1) conn[static_cast<std::size_t>(e * 4 + j)] = v[j];
            } else {
                if (npe != 8) {
                    ok = false;
                    break;
                }
                if (std::fscanf(f, "%*d %d %d %d %d %d %d %d %d\n", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5],
                                &v[6], &v[7]) == -1) {  // 14051dbc8
                    ok = false;
                    break;
                }
                for (int j = 0; j < 8; j = j + 1) conn[static_cast<std::size_t>(e * 8 + j)] = v[j];
            }
        }
    }
    std::fclose(f);
    if (nnode < 1 || nelem < 1) return false;
    model::DB_STEP_DATA* m = mg.u3f48;
    geolib::fun_140202700(obj, nnode, nelem, npe == 4 ? 2 : 4, m->u15f8, 0);
    const std::int32_t w = m->u15f8[0];
    // the original copies nnode * dims[0] doubles from a buffer of nnode * 3 (dims[0] is 3 in 3D)
    if (xyz.size() < static_cast<std::size_t>(nnode * w)) xyz.resize(static_cast<std::size_t>(nnode * w), 0.0);
    std::memcpy(node_coords(obj), xyz.data(), static_cast<std::size_t>(nnode * w) * 8);
    const std::int32_t epn = at<std::int32_t>(obj, 0x894);
    // the original copies nelem * obj+0x894 ints from a buffer of nelem * npe (equal for 4 and 8)
    if (conn.size() < static_cast<std::size_t>(nelem * epn)) conn.resize(static_cast<std::size_t>(nelem * epn), 0);
    std::memcpy(connectivity(obj), conn.data(), static_cast<std::size_t>(nelem * epn) * 4);
    mesh3d_apply_group(mg);
    gk::build_boundary(*obj);  // 1401fc290
    return ok;
}

// @src 140143900
void mesh3d_set_volume_read(MeshGenBase&) { g_mesher_state = 2; }

// @src 1401445f0
void mesh3d_apply_group(MeshGenBase& mg) { geolib::fun_1402027e0(mg.u008, g_mesher_group_id, false, true); }

} // namespace def::cmd
