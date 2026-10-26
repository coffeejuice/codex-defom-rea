// 2D automatic mesh generation (DEF_AMG) of packet p8-07. Spec: docs/re/commands/DEFAMG.md 4.1, 6.1.
#define _CRT_SECURE_NO_WARNINGS
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>

#include <defcmd/file_util.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/key_file.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/p8_07_ext.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "mg_internal.hpp"
#include "mg_util.hpp"

namespace def::cmd {

using namespace mg;
namespace gk = model::geo_kw;

namespace {
inline bool file_remove(const char* name) { return dbio::gendb::remove_if_exists(name) != 0; }  // 1401b85d0
inline std::byte* mesh_block(LLOBJ_DATA* obj) { return obj->s2d10; }                             // obj+0x2d10
}  // namespace

// @src 14019d420  number of the i-th material (1-based) of the model's list, 0 when absent
std::int32_t material_id(DB_STEP_DATA* m, std::int32_t i)
{
    model::Material* mat = model::material_list_nth(&m->materials, i);  // 14029c260
    if (mat == nullptr) return 0;
    return at<std::int32_t>(mat, 0);
}

// @src 14029c3c0  the smallest material number >= 1 that is not in the list
std::int32_t first_free_material(void* materials)
{
    std::int32_t i = 1;
    while (model::material_list_find(static_cast<model::MaterialList*>(materials), i) != nullptr) i = i + 1;  // 14029c2c0
    return i;
}

// @src 140198520  build the topology of every geometry of the object (140281bf0)
void build_topology(LLOBJ_DATA* obj, const char* flag)
{
    const std::int32_t n = gk::geometry_count(*obj);  // 1401983e0
    for (std::int32_t i = 0; i < n; i = i + 1) {
        model::GEO2D_AND_MESH2D* g = gk::geometry(*obj, i);  // 140197f40
        if (g != nullptr) {
            const bool b = *flag != 0;
            gk::build_topology(g, &b);
        }
    }
}

// @src 1401574d0  every region of geometry 0 gets the number of the material at its position
bool region_materials(MeshGenBase& mg)
{
    model::GEO2D_AND_MESH2D* g = gk::geometry(*mg.u008, 0);
    if (g == nullptr) return false;
    const std::int32_t n = static_cast<std::int32_t>(model::llobj::list8_size_b(g));  // 140158f30
    if (n < 1) return false;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        model::DEF_FACE* f = model::geo2d_face_at(g, &i);  // 140283f80
        std::int32_t id = material_id(mg.u3f48, i + 1);
        f->set_u40(&id);  // 1402f7f90
    }
    return true;
}

// @src 140144a00  "<prob>.DST"
void dst_name(MeshGenBase& mg, char* out) { std::sprintf(out, "%s.DST", mg.u3f60); }

// @src 140145480  remove the exchange files of a DEF_AMG run
void mesh2d_cleanup(MeshGenBase& mg)
{
    char name[0x400];
    file_remove("DEF_MGN.ACT");
    file_remove("DEF_MGN.OUT");
    file_remove("NEWNOD.DAT");
    file_remove("NEWMSH.GEO");
    file_remove("NBCD.OUT");
    file_remove("NBCD.LST");
    file_remove("mesh.out");
    std::sprintf(name, "%s_%03d.KEY", mg.u3f60, obj_number(mg.u008));
    file_remove(name);
    std::sprintf(name, "%s.GEO", mg.u3f60);
    file_remove(name);
    dst_name(mg, name);
    file_remove(name);
}

// @src 140268390  inverse of a rigid 4x4 transform: transposed rotation, -R^T t
static void rigid_inverse(model::LLWINDEF_DATA*, const double* m, double* out)
{
    for (std::int32_t i = 0; i < 4; i = i + 1)
        for (std::int32_t j = 0; j < 4; j = j + 1) out[i * 4 + j] = 0.0;
    out[3] = -(m[3] * m[0] + m[7] * m[4] + m[11] * m[8]);    // XORPD 14051bec0
    out[7] = -(m[3] * m[1] + m[7] * m[5] + m[11] * m[9]);
    out[11] = -(m[3] * m[2] + m[7] * m[6] + m[11] * m[10]);
    out[15] = 1.0;
    for (std::int32_t i = 0; i < 3; i = i + 1)
        for (std::int32_t j = 0; j < 3; j = j + 1) out[i * 4 + j] = m[j * 4 + i];
}

// @src 14025c910  window +0x890 (the cross-section the window was defined on)
static model::GEO2D_AND_MESH2D* window_section(model::LLWINDEF_DATA* w)
{
    return reinterpret_cast<model::GEO2D_AND_MESH2D*>(w->u890);
}

// @src 140268590  move the window's points by its cross-section's matrix (invert: by the inverse)
void window_to_polygon(model::LLWINDEF_DATA* w, char invert)
{
    model::linked::Matrix4 mat;  // 140220540
    model::GEO2D_AND_MESH2D* g = window_section(w);
    if (g != nullptr) {
        gk::get_matrix(g, &mat);  // 14026a3e0
        double m[16];
        for (std::int32_t i = 0; i < 4; i = i + 1)
            for (std::int32_t j = 0; j < 4; j = j + 1) m[i * 4 + j] = *gk::matrix_element(&mat, &i, &j);  // 14026a4a0
        if (invert == 0) {
            ext::fun_140268290(w, m);
        } else {
            double inv[16];
            rigid_inverse(w, m, inv);
            ext::fun_140268290(w, inv);
        }
    }
}  // 140220560: Matrix4 dtor

// @src 140145ce0  USRWIN.DAT (user mesh windows); 0 also when nothing is written, 0xd when the
// file cannot be opened. pts2d: the windows store 2 doubles per point (obj+0x39c0 == 2);
// 0xfffffc19 (-999): take it from 140158d00.
std::int32_t write_usrwin(MeshGenBase& mg, std::byte* w, std::uint32_t pts2d)
{
    model::LLWINDEF_DATA win(nullptr);  // 14025c100(local, 0)
    file_remove("USRWIN.DAT");
    if (w == nullptr) w = mesh_block(mg.u008);
    if (pts2d == 0xfffffc19u) pts2d = model::p407::obj_kind_is_2(mg.u008) ? 1u : 0u;
    double& wusr = at<double>(w, 0x30);
    if (!(1e-6 < wusr)) return 0;  // DAT_14051f1b8
    if (dbio::gendb::windows_2d10_count(w) < 1) return 0;
    std::FILE* f = crt_fopen("USRWIN.DAT", "wt");
    if (f == nullptr) return 0xd;
    rt::crt::fprintf(f, "%1f\n", wusr);
    auto* list = reinterpret_cast<model::Cls_140269250*>(w + 0x40);
    for (std::int32_t i = 0; i < dbio::gendb::windows_2d10_count(w); i = i + 1) {
        model::LLWINDEF_DATA* src = model::winlist_nth(list, i + 1);  // 140269900
        if (src == nullptr) continue;
        model::window_copy(&win, src);  // 14025c9b0
        rt::crt::fprintf(f, "%6d %15.7e\n", win.npts, std::bit_cast<double>(win.u370));
        if (pts2d == 0) {
            if (win.shape == 0xb || win.shape == 0xc || win.shape == 0xd) {
                window_to_polygon(&win, 1);
                for (std::int32_t k = 0; k < win.npts; k = k + 1)
                    rt::crt::fprintf(f, "%15.7e %15.7e\n", win.pts[k * 3], win.pts[k * 3 + 1]);
            }
        } else {
            for (std::int32_t k = 0; k < win.npts; k = k + 1)
                rt::crt::fprintf(f, "%15.7e %15.7e\n", win.pts[k * 2], win.pts[k * 2 + 1]);
        }
    }
    std::fclose(f);
    return 0;
}  // 14025c6e0: window dtor

// @src 140154f20  nodes near the axis (|r| < limit, r = first coordinate) into out: with boundary
// chains (obj+0xbd8 chains of 0x30 bytes at obj+0xbe0, GEO23 k == 0) the chain stretch from the
// lowest to the highest such node, else every node with r < limit
static void near_axis_nodes(MeshGenBase&, double limit, model::CowList<std::int32_t>* out, LLOBJ_DATA* obj)
{
    out->clear();  // 14015a620
    const std::int32_t nchain = at<std::int32_t>(obj, 0xbd8);
    if (at<std::int32_t>(obj, 0x2c08) == 0 && 0 < nchain) {
        std::int32_t lo = 0, hi = 0, nfound = 0;
        std::int32_t* chain = nullptr;
        for (std::int32_t c = 0; c < nchain; c = c + 1) {
            chain = reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(obj) + 0xbe0 + c * 0x30);
            std::int32_t* ids = at<std::int32_t*>(chain, 8);
            if (ids == nullptr) continue;
            for (std::int32_t k = 0; k < chain[0] - 1; k = k + 1) {
                const std::int32_t v = ids[k];
                if (std::fabs(node_coords(obj)[v * 2 - 2]) < limit) {
                    nfound = nfound + 1;
                    if (nfound == 1) {
                        lo = k;
                        hi = k;
                    }
                    const std::int32_t vlo = ids[lo];
                    const std::int32_t vhi = ids[hi];
                    const double y = node_coords(obj)[v * 2 - 1];
                    if (y <= node_coords(obj)[vlo * 2 - 1]) {
                        if (y < node_coords(obj)[vhi * 2 - 1]) hi = k;
                    } else {
                        lo = k;
                    }
                }
            }
        }
        if (0 < nfound) {
            for (std::int32_t c = 0; c < nchain; c = c + 1) {
                std::int32_t* ch = reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(obj) + 0xbe0 + c * 0x30);
                std::int32_t* ids = at<std::int32_t*>(ch, 8);
                if (ids == nullptr) continue;
                const std::int32_t len = ch[0] - 1;
                if (hi < lo) hi = hi + len;
                for (std::int32_t k = lo; k <= hi; k = k + 1) out->push_back(ids[k % len]);  // 1401599e0
            }
        }
    } else {
        for (std::int32_t i = 1; i <= node_count(obj); i = i + 1) {
            if (node_coords(obj)[i * 2 - 2] < limit) out->push_back(i);
        }
    }
}

// @src 140155340  GEO23 mode 1: zero the radial coordinate of nodes and curve points with
// |r| < limit. A no-op when limit == 0 (the only value DEFAMG passes).
void geo23_clear(MeshGenBase& mg, LLOBJ_DATA* obj, double limit)
{
    if (limit == 0.0 || obj == nullptr || at<std::int32_t>(obj, 0x2be8) != 1) return;
    if (has_mesh(obj) != 0) {
        if (at<std::int32_t>(obj, 0x2c08) == 0 && 0 < at<std::int32_t>(obj, 0xbd8)) {
            model::CowList<std::int32_t> ids;  // 140159860
            near_axis_nodes(mg, limit, &ids, obj);
            const std::int32_t n = static_cast<std::int32_t>(ids.size());  // 140159950
            for (std::int32_t i = 0; i < n; i = i + 1) {
                const std::int32_t v = model::geo_kw::ints_at(ids, i);  // 140159a70
                node_coords(obj)[v * 2 - 2] = 0.0;
            }
        } else {
            for (std::int32_t i = 1; i <= node_count(obj); i = i + 1) {
                if (std::fabs(node_coords(obj)[i * 2 - 2]) < limit) node_coords(obj)[i * 2 - 2] = 0.0;
            }
        }
    }
    if (gk::geometry_has_edges(*obj)) {
        model::OBJ_GEO2D* c = gk::curve(*obj, 0);  // 1401982c0
        const std::int32_t n = c->n_points;
        double* p = c->points;
        for (std::int32_t i = 1; i <= n; i = i + 1) {
            if (std::fabs(p[(i - 1) * 3]) < limit) p[(i - 1) * 3] = 0.0;
        }
    }
}

// @src 140144de0  DEF_MGN.ACT, DEF_AMG.INI, USRWIN.DAT, <geo>.GEO, DSTPAR2.DAT, launch DEF_AMG
std::int32_t mesh2d_inputs(MeshGenBase& mg, const char* multi)
{
    static const char act_default[] = "6\n\n5\n6\n\ne\n";            // DAT_14051dc58
    static const char act_alt[] = "\n4\n3\nN\n\n\ne\n5\n6\n\ne\n";   // DAT_14051dc68
    LLOBJ_DATA* obj = mg.u008;
    if (obj == nullptr) return 0x14;
    if (!gk::geometry_has_edges(*obj) || gk::curve_count(*obj) < 1) return 0xe;
    if (gk::curve_count(*obj) == 1) {
        model::OBJ_GEO2D* c = gk::curve(*obj, 0);
        if (!c->is_ccw()) {  // 1402789f0
            if (!mg.confirm_reverse()) return 0x11;
            gk::curve(*obj, 0)->reverse();  // curve vtable +0x28
        }
    }
    if (model::llobj::first_item_has_items(gk::geometry(*obj, 0)) == 0) {  // 140158ec0
        if (gk::curve_count(*obj) == 1) {
            const char no = 0;
            build_topology(obj, &no);
            if (model::llobj::first_item_has_items(gk::geometry(*obj, 0)) == 0) return 0x12;
        } else {
            if (*multi == 0) return 0x12;
            build_topology(obj, multi);
            if (!region_materials(mg)) return 0x12;
        }
    }
    std::FILE* f = crt_fopen("DEF_MGN.ACT", "wt");
    if (f == nullptr) return 0xd;
    std::fprintf(f, "%s", at<std::int32_t>(obj, 0x2d18) == 1 ? act_alt : act_default);
    std::fclose(f);
    f = crt_fopen("DEF_AMG.INI", "wt");
    if (f == nullptr) return 0xd;
    std::fprintf(f, "%s\n", mg.u3f60);
    std::fprintf(f, "%6d\n", obj_number(obj));
    std::fclose(f);
    normalise_weights(mg, mesh_block(obj));
    const std::uint32_t pts2d = model::p407::obj_kind_is_2(obj) ? 1u : 0u;  // 140158d00
    std::int32_t rc = write_usrwin(mg, mesh_block(obj), pts2d);
    if (rc != 0) return rc;
    geo23_clear(mg, obj, mg.u4770);
    write_geo_file(mg, nullptr, nullptr, nullptr);  // result ignored in 2D
    f = crt_fopen("DSTPAR2.DAT", "wt");
    if (f == nullptr) return 0xd;
    rt::crt::fprintf(f, "%6d %6d %15.7e %15.7e\n", at<std::int32_t>(obj, 0x2dc0), at<std::int32_t>(obj, 0x2dc4),
                     at<double>(obj, 0x2db0), at<double>(obj, 0x2db8));
    rt::crt::fprintf(f, "%6d %15.7e %6d %15.7e %15.7e %15.7e %15.7e\n", at<std::int32_t>(obj, 0x2d80),
                     at<double>(obj, 0x2d20), at<std::int32_t>(obj, 0x2d84), at<double>(obj, 0x2d48),
                     at<double>(obj, 0x2d38), at<double>(obj, 0x2d28), at<double>(obj, 0x2d30));
    std::fclose(f);
    write_text_file("DEFORM_UIRUNREMESH.DAT", "2D");  // str 14051dab4
    char cmd[0xd0];
    if (!rt::dimension_is_2(obj->get_owner())) {  // 140158f50, 1400015e0
        std::sprintf(cmd, "DEF_AMG3_GUI < %s > %s", "DEF_MGN.ACT", "DEF_MGN.OUT");
        launch_program_3d(1, cmd);
    } else {
        std::sprintf(cmd, "DEF_AMG < %s > %s", "DEF_MGN.ACT", "DEF_MGN.OUT");
        launch_program_2d(1, cmd);
    }
    std::remove("DEFORM_UIRUNREMESH.DAT");  // 14047e29c
    std::remove("DEF_MGN.ACT");
    std::remove("DEF_AMG.INI");
    std::remove("DEF_MGN.OUT");
    std::remove("USRWIN.DAT");
    std::remove("DSTPAR2.DAT");
    return 0;
}

// @src 1401455b0  read <prob>_<obj:%03d>.KEY back into the model; true when the object has a mesh
bool mesh2d_read_back(MeshGenBase& mg, const char* set_group)
{
    char name[0x400];
    LLOBJ_DATA* obj = mg.u008;
    std::sprintf(name, "%s_%03d.KEY", mg.u3f60, obj_number(obj));
    if (!dbio::file_exists(name)) return false;  // 1401b8560
    const char reset = 0;
    const std::int32_t flags = 0;
    if (read_keyword_file(mg.u3f48, name, &reset, &flags) != 0) {  // 1401b5f00
        file_remove(name);
        return false;
    }
    file_remove(name);
    obj = mg.u008;
    if (elem_count(obj) < 1 || node_count(obj) < 1) {
        geolib::fun_1401ffd80(obj);
        return false;
    }
    if (*set_group == 0) {
        if (g_mesher_group_id == 0) {
            const std::int32_t k = first_free_material(&mg.u3f48->materials);
            const std::int32_t n = elem_count(obj) * mg.u3f48->u15f8[0x3d];  // model+0x16ec
            for (std::int32_t i = 0; i < n; i = i + 1) {
                if (elem_groups(obj)[i] == k) elem_groups(obj)[i] = 0;
            }
        }
    } else {
        geolib::fun_1402027e0(obj, g_mesher_group_id, false, false);
    }
    fun_1401443c0(mg);
    fun_1401444f0(mg);
    return true;
}

// @src 1401458d0  AddCoating.INI and the object's KEY file for COATINGMESH
bool write_add_coating(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    const std::int32_t n = at<std::int32_t>(obj, 0x2de8);
    if (n < 1) return true;
    char name[0x400];
    std::sprintf(name, "%s_%03d.KEY", mg.u3f60, obj_number(obj));
    std::FILE* f = crt_fopen(name, "wt");
    if (f == nullptr) return false;
    std::int32_t group = g_mesher_group_id;
    if (g_mesher_group_id == 0) {
        bool same = true;
        for (std::int32_t i = 1; i <= elem_count(obj); i = i + 1) {
            if (elem_groups(obj)[i - 1] != *elem_groups(obj)) {
                same = false;
                break;
            }
        }
        if (same) {
            group = first_free_material(&mg.u3f48->materials);
            geolib::fun_1402027e0(obj, group, false, true);
        }
    }
    key::g2obj::write_object(*mg.u3f48, f, obj, static_cast<std::uint32_t>(obj_number(obj)), false);  // 14029e250
    std::fclose(f);
    f = crt_fopen("AddCoating.INI", "wt");
    if (f == nullptr) return false;
    std::fprintf(f, "%s\n", name);
    std::fprintf(f, "%s\n", name);
    rt::crt::fprintf(f, "%f %d %d %d\n", 0.0, n, at<std::int32_t>(obj, 0x2e00), 1);
    for (std::int32_t i = 0; i < n; i = i + 1) rt::crt::fprintf(f, "%g  ", at<double*>(obj, 0x2df0)[i]);
    std::int32_t a = 0, b = 0;
    const std::int32_t geotyp = at<std::int32_t>(mg.u3f48, 0x850);
    if (geotyp == 1 || geotyp == 3) {
        b = -1;
        a = b;
    }
    std::fprintf(f, "\n%d  %d\n%d  %d\n%d\n", a, b, obj_number(obj), group, n);
    for (std::int32_t i = 0; i < n; i = i + 1) std::fprintf(f, "%d\n", at<std::int32_t*>(obj, 0x2df8)[i]);
    std::fclose(f);
    return true;
}

// @src 140145810  COATINGMESH (only with coating layers, obj+0x2de8 > 0)
void mesh2d_coating(MeshGenBase& mg)
{
    if (0 < at<std::int32_t>(mg.u008, 0x2de8) && write_add_coating(mg)) {
        char cmd[0xd0];
        std::sprintf(cmd, "COATINGMESH.EXE");
        launch_program_2d(1, cmd);
        file_remove("AddCoating.INI");
        const char no = 0;
        mesh2d_read_back(mg, &no);
    }
}

// @src 140144a70  2D driver: inputs + DEF_AMG, read-back, coating, cleanup
bool mesh2d_run(MeshGenBase& mg, const char* multi)
{
    g_mesher_state = 4;
    mg.set_busy(1);
    g_mesher_progress = 1;
    mg.progress();
    bool clear_groups = false;
    std::int32_t count = 0, group = 0;
    gk::material_groups(*mg.u008, &count, &group);  // 1401fb270
    if (group < 1) clear_groups = true;
    const std::int32_t rc = mesh2d_inputs(mg, multi);
    mg.set_busy(0);
    if (rc != 0) {
        mg.report_error(rc);
        mesh2d_cleanup(mg);
        g_mesher_state = 0;
        return false;
    }
    mg.set_busy(1);
    const char yes = 1;
    const bool ok = mesh2d_read_back(mg, &yes);
    if (clear_groups) {
        LLOBJ_DATA* obj = mg.u008;
        const std::int32_t n = elem_count(obj);
        if (elem_groups(obj) != nullptr) {
            for (std::int32_t i = 0; i < n; i = i + 1) elem_groups(obj)[i] = 0;
        }
    }
    mg.set_busy(0);
    if (!ok) {
        mg.report_error(5);
        g_mesher_state = 0;
        return false;
    }
    mg.set_busy(1);
    mesh2d_coating(mg);
    mg.set_busy(0);
    set_state_coating_done(mg);
    g_mesher_progress = 2;
    mg.progress();
    mesh2d_cleanup(mg);
    return true;
}

} // namespace def::cmd
