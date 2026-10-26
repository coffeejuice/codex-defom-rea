// 3D surface step of the mesh generators (packet p8-07): DEF_AMG3.INI, <geo>.GEO, NBCD.DAT,
// DSTPAR.DAT, BLOCK3.DAT, DEF_AMG3_GUI, MESH3D.INP read-back; NBCD.OUT read-back; size scaling.
// Spec: docs/re/commands/DEFAMG.md 4.2, 6.2, 6.3; CNVT3D.md 6.3. The volume step 140140110 is p8-06's.
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defcmd/file_util.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/vec3.hpp>
#include <defcmd/p8_07_ext.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/port.hpp>
#include <bit>

#include "mg_internal.hpp"
#include "mg_util.hpp"

namespace def::cmd {

using namespace mg;
namespace gk = model::geo_kw;
using model::OBJ_GEO3D;

namespace {
inline bool file_remove(const char* name) { return dbio::gendb::remove_if_exists(name) != 0; }  // 1401b85d0
inline std::byte* mesh_block(LLOBJ_DATA* obj) { return obj->s2d10; }
inline model::Cls_140269250* window_list(LLOBJ_DATA* obj)
{
    return reinterpret_cast<model::Cls_140269250*>(obj->s2d10 + 0x40);  // obj+0x2d50
}
// symmetry planes of a surface (140158e60: surface + 0x28)
inline model::linked::Cls_140257340List* sym_planes(OBJ_GEO3D* s)
{
    return reinterpret_cast<model::linked::Cls_140257340List*>(s->u28);
}
inline std::int32_t* bc_code(LLOBJ_DATA* o) { return at<std::int32_t*>(o, 0x990); }   // node BC per DOF
inline std::int32_t* bc_plane(LLOBJ_DATA* o) { return at<std::int32_t*>(o, 0x998); }  // BC function / plane
}  // namespace

// 14023b850  squared distance of two 3-vectors: p67-1's v3::dist2 (defcmd/vec3.hpp)
static double dist2(const double* a, const double* b) { return v3::dist2(a, b); }

// @src 14013efb0  files of a DEF_AMG3_GUI run
void surface_cleanup(MeshGenBase&)
{
    file_remove("DEF_MGN.ACT");
    file_remove("DEF_MGN.OUT");
    file_remove("NEWNOD.DAT");
    file_remove("NEWMSH.GEO");
    file_remove("NBCD.LST");
    file_remove("mesh.out");
}

// @src 1402062b0  some polygon of surface 0 has a BC code (surface + 0x20, 140158e80)
bool surface_has_bc_codes(LLOBJ_DATA* obj)
{
    for (std::int32_t i = 0;; i = i + 1) {
        if (gk::surface(*obj, 0)->elem_count() <= i) return false;
        if (gk::surface(*obj, 0)->elem_flags[i] != 0) return true;
    }
}

// @src 140142240  NBCD.DAT: per node up to three BC codes of the polygons using it, symmetry planes
bool write_nbcd_dat(MeshGenBase& mg)
{
    OBJ_GEO3D* s = gk::surface(*mg.u008, 0);
    const std::int32_t nn = s->node_count();
    const std::int32_t np = s->elem_count();
    const std::int32_t* poly = gk::geo3d_elems(s);
    std::int32_t* code = model::bc::calloc_ints(nn * 3);  // 140256dc0
    const std::int32_t* pc = s->elem_flags;               // 140158e80
    std::int32_t ncoded = 0;
    for (std::int32_t i = 0; i < np; i = i + 1) {
        if (pc[i] == 0) continue;
        for (std::int32_t k = 0; k < 4; k = k + 1) {
            const std::int32_t v = std::abs(poly[k + i * 4]) - 1;  // 140484ab8
            if (v < 0 || nn <= v) continue;
            std::int32_t slot = 2;
            bool store = true;
            while (true) {
                if (slot < 0 || code[v * 3 + slot] == pc[i]) {
                    store = false;
                    break;
                }
                if (code[v * 3 + slot] == 0) break;
                slot = slot - 1;
            }
            if (store) code[v * 3 + slot] = pc[i];
        }
        ncoded = ncoded + 1;
    }
    if (0 < ncoded) {
        std::FILE* f = crt_fopen("NBCD.DAT", "wt");
        if (f != nullptr) {  // the original does not test the handle
            std::fprintf(f, "%7d %7d %7d\n", 0, 0, nn);
            for (std::int32_t i = 0; i < nn; i = i + 1)
                std::fprintf(f, "%7d %7d %7d %7d\n", i + 1, code[i * 3], code[i * 3 + 1], code[i * 3 + 2]);
            const std::int32_t m = sym_planes(s)->size();  // 140158e70
            std::fprintf(f, "%7d\n", m);
            for (std::int32_t j = 0; j < m; j = j + 1) {
                model::linked::Cls_140257340* p = sym_planes(s)->nth(j + 1);  // 140258090
                if (p == nullptr) continue;
                std::fprintf(f, "%7d\n", j + 0x259);
                rt::crt::fprintf(f, "%15.7g %15.7g %15.7g\n", p->point[0], p->point[1], p->point[2]);
                rt::crt::fprintf(f, "%15.7g %15.7g %15.7g\n", p->direction[0], p->direction[1], p->direction[2]);
            }
            std::fprintf(f, "%7d\n", 0);
            std::fprintf(f, "%7d\n", 0);
            std::fclose(f);
        }
    }
    if (code != nullptr) std::free(code);
    return true;
}

// @src 14013f8f0  BLOCK3.DAT (3D user mesh windows); 4 when it cannot be opened
std::int32_t write_block3(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    file_remove("BLOCK3.DAT");
    if (!(0 < dbio::gendb::windows_2d10_count(mesh_block(obj)) && 0.0 < at<double>(obj, 0x2d40))) return 0;
    std::FILE* f = crt_fopen("BLOCK3.DAT", "wt");
    if (f == nullptr) return 4;
    rt::crt::fprintf(f, "%lf\n", at<double>(obj, 0x2d40));
    rt::crt::fprintf(f, "%lf\n", at<double>(obj, 0x2d98));
    std::fprintf(f, "%d\n", dbio::gendb::windows_2d10_count(mesh_block(obj)));
    for (std::int32_t i = 0; i < dbio::gendb::windows_2d10_count(mesh_block(obj)); i = i + 1) {
        model::LLWINDEF_DATA* w = model::winlist_nth(window_list(obj), i + 1);  // 140269900
        if (w == nullptr) continue;
        rt::crt::fprintf(f, "%d %lf\n", w->shape, std::bit_cast<double>(w->u370));
        if (w->shape == 1 || w->shape == 2) {
            for (std::int32_t k = 0; k < w->npts; k = k + 1)
                rt::crt::fprintf(f, "%5d %15.7E %15.7E %15.7E\n", k + 1, w->pts[k * 3], w->pts[k * 3 + 1], w->pts[k * 3 + 2]);
            if (w->shape == 2) rt::crt::fprintf(f, "%15.7E %15.7E\n", w->u1b8, w->u1c0);
        } else if (w->shape == 3) {
            OBJ_GEO3D* g = &w->geo3d;  // window +0x220
            const std::int32_t nn = g->node_count();
            const std::int32_t np = g->elem_count();
            const double* x = gk::geo3d_coords(g);
            const std::int32_t* p = gk::geo3d_elems(g);
            std::fprintf(f, "%d\n", nn);
            for (std::int32_t k = 0; k < nn; k = k + 1)
                rt::crt::fprintf(f, "%10d %15.7f %15.7f %15.7f\n", k + 1, x[k * 3], x[k * 3 + 1], x[k * 3 + 2]);
            std::fprintf(f, "%d\n", np);
            for (std::int32_t k = 0; k < np; k = k + 1)
                std::fprintf(f, "%10d %10d %10d %10d %10d\n", k + 1, p[k * 4], p[k * 4 + 1], p[k * 4 + 2], p[k * 4 + 3]);
        } else if (w->shape == 0xb || w->shape == 0xc || w->shape == 0xd) {
            auto& axes = *reinterpret_cast<model::linked::Cls_140258580List*>(reinterpret_cast<char*>(obj) + 0x20a0);
            const void* ax = gk::wpaxis_get(axes, 1);  // 140259bc0(obj+0x20a0, 1)
            const std::int32_t type = at<std::int32_t>(ax, 4);
            std::fprintf(f, "%d\t", type);
            rt::crt::fprintf(f, "%lf\t%lf\t%lf\n", at<double>(ax, 8), at<double>(ax, 0x10), at<double>(ax, 0x18));
            if (type == 1) {
                const double dx = std::fabs(at<double>(ax, 0x20) - at<double>(ax, 8));     // 140484558
                const double dy = std::fabs(at<double>(ax, 0x28) - at<double>(ax, 0x10));
                const double dz = std::fabs(at<double>(ax, 0x30) - at<double>(ax, 0x18));
                const double len = std::sqrt(dx * dx + dy * dy + dz * dz);  // 140484644
                if (len == 0.0) {
                    std::fprintf(f, "0\t0\t0\t\n");
                } else {
                    rt::crt::fprintf(f, "%lf\t%lf\t%lf\n", dx / len, dy / len, dz / len);
                }
            } else {
                rt::crt::fprintf(f, "%lf\t%lf\t%lf\n", at<double>(ax, 0x20), at<double>(ax, 0x28), at<double>(ax, 0x30));
            }
            std::fprintf(f, "%d\n", w->npts);
            for (std::int32_t k = 0; k < w->npts; k = k + 1)
                rt::crt::fprintf(f, "%15.7f %15.7f %15.7f\n", w->pts[k * 3], w->pts[k * 3 + 1], w->pts[k * 3 + 2]);
        }
    }
    std::fclose(f);
    return 0;
}

// @src 14013f410  surface-mesh inputs and DEF_AMG3_GUI. keep_act: DEF_MGN.ACT is not rewritten;
// keep_fixgeo: FIXGEO.DAT is not deleted. 0x14 nothing to mesh, 3 INI/GEO, 4 DSTPAR, 1 ACT.
int fun_14013f410(MeshGenBase& mg, char keep_act, char keep_fixgeo)
{
    static const char act[] = "5\nN\n7\ne\nY\n";  // DAT_14051d520 (11 bytes copied)
    LLOBJ_DATA* obj = mg.u008;
    if (node_count(obj) < 1 && gk::surface_count(*obj) < 1) return 0x14;  // 1401985a0
    std::FILE* f = crt_fopen("DEF_AMG3.INI", "wt");
    if (f == nullptr) return 3;
    std::fprintf(f, "%s\n", mg.u3f60);
    std::fprintf(f, "%6d\n", obj_number(obj));
    std::fclose(f);
    if (!write_geo_file(mg, nullptr, nullptr, nullptr)) return 3;
    if (g_amg_dxt_ok == 0 && surface_has_bc_codes(obj)) write_nbcd_dat(mg);
    f = crt_fopen("DSTPAR.DAT", "wt");
    if (f == nullptr) return 4;
    normalise_weights(mg, mesh_block(obj));
    rt::crt::fprintf(f, "%lf %lf %lf\n", at<double>(obj, 0x2d30), at<double>(obj, 0x2d38), at<double>(obj, 0x2d28));
    const std::int32_t nelem = at<std::int32_t>(obj, 0x2d88);
    const bool subst = nelem == 0;
    if (subst) at<std::int32_t>(obj, 0x2d88) = 8000;
    rt::crt::fprintf(f, "%lf %lf %d\n", at<double>(obj, 0x2d48), at<double>(obj, 0x2d20), at<std::int32_t>(obj, 0x2d88));
    if (subst) at<std::int32_t>(obj, 0x2d88) = nelem;
    std::fprintf(f, "%d %d\n", 0, 0);
    rt::crt::fprintf(f, "%d %lf\n", at<std::int32_t>(obj, 0x2da0), at<double>(obj, 0x2d98));
    std::fclose(f);
    write_block3(mg);
    if (keep_act == 0) {
        f = crt_fopen("DEF_MGN.ACT", "wt");
        if (f == nullptr) return 1;
        std::fprintf(f, "%s", act);
        std::fclose(f);
    }
    file_remove("DEF_MGN.OUT");
    file_remove("MESH3D.INP");
    if (keep_fixgeo == 0) file_remove("FIXGEO.DAT");
    write_text_file("DEFORM_UIRUNREMESH.DAT", "3D");  // str 14051d24c
    char cmd[0x70];
    std::sprintf(cmd, "DEF_AMG3_GUI < %s > %s", "DEF_MGN.ACT", "DEF_MGN.OUT");
    launch_program_3d(1, cmd);
    file_remove("DEFORM_UIRUNREMESH.DAT");
    char name[0x400];
    std::sprintf(name, "%s.GEO", mg.u3f60);
    file_remove(name);
    std::sprintf(name, "%s.DST", mg.u3f60);
    file_remove(name);
    file_remove("DEF_AMG3.INI");
    file_remove("DEF_MGN.ACT");
    file_remove("DSTPAR.DAT");
    file_remove("BLOCK3.DAT");
    file_remove("DEF_AMG.DST");
    return 0;
}

// @src 14013f010  slot +0x18: MESH3D.INP (surface mesh of DEF_AMG3_GUI) into the object's nodes
// and surface polygons. False when it cannot be opened or a count is out of range.
bool MeshGenBase::read_mesh3d_inp()
{
    std::int32_t* polys = nullptr;
    double* xyz = nullptr;
    std::int32_t nread = 0, pread = 0, n = 0, m = 0;
    std::FILE* f = crt_fopen("MESH3D.INP", "rt");
    if (f == nullptr) return false;
    bool ok = true;
    std::fscanf(f, "%d\n", &n);
    if (n < 1 || 1e20 <= static_cast<double>(n)) {  // DAT_14051f1b0
        ok = false;
    } else {
        xyz = model::bc::calloc_doubles(n * 3);  // 140257150
        nread = 0;
        double x = 0.0, y = 0.0, z = 0.0;
        while (nread < n) {
            // a read error (-1) retries forever, as in the original
            if (std::fscanf(f, "%*d %lf %lf %lf\n", &x, &y, &z) != -1) {
                xyz[nread * 3] = x;
                xyz[nread * 3 + 1] = y;
                xyz[nread * 3 + 2] = z;
                nread = nread + 1;
            }
        }
    }
    std::fscanf(f, "%d\n", &m);
    if (m < 1 || 1e7 <= static_cast<double>(m)) {  // DAT_14051f1a8
        ok = false;
    } else {
        polys = model::bc::calloc_ints(m << 2);  // 140256dc0
        pread = 0;
        std::int32_t a = 0, b = 0, c = 0, d = 0;
        while (pread < m) {
            if (std::fscanf(f, "%*d %d %d %d %d\n", &a, &b, &c, &d) != -1) {
                polys[pread * 4] = a;
                polys[pread * 4 + 1] = b;
                polys[pread * 4 + 2] = c;
                polys[pread * 4 + 3] = d;
                pread = pread + 1;
            }
        }
        m = pread;
    }
    std::fclose(f);
    if (0 < nread && 0 < pread) {
        LLOBJ_DATA* obj = u008;
        geolib::fun_140202700(obj, nread, 0, 2, u3f48->u15f8, 0);
        std::memcpy(node_coords(obj), xyz, static_cast<std::size_t>(nread * 3) << 3);
        has_mesh(obj) = 1;
        model::bc::set_boundary_count(*obj, 1);           // 140163300
        model::bc::set_boundary_face_count(*obj, pread, 0);  // 140200a70
        std::memcpy(at<std::int32_t*>(obj, 0xbf8), polys, static_cast<std::size_t>(pread << 2) << 2);
        model::bc::recount_boundary_segments(*obj);       // 140165d30
    }
    if (polys != nullptr) std::free(polys);
    polys = nullptr;
    if (xyz != nullptr) std::free(xyz);
    return ok;
}

// @src 14013ee00  slot +0x08: surface step: inputs + DEF_AMG3_GUI (14013f410), MESH3D.INP read-back
int MeshGenBase::surface_step(char keep_fixgeo)
{
    g_mesher_state = 0;
    g_mesher_progress = 7;
    progress();
    const int rc = fun_14013f410(*this, 0, keep_fixgeo);
    if (rc != 0) {
        surface_cleanup(*this);
        return rc;
    }
    if (!read_mesh3d_inp()) {
        surface_cleanup(*this);
        return 0x14;
    }
    set_state_surface_read(*this);
    g_mesher_progress = 3;
    progress();
    surface_cleanup(*this);
    return 0;
}

// @src 140141580  NBCD.OUT: BC codes found by DEF_AMG3 at surface points, put on the nearest node
// within the shortest element edge; boundary faces without a common 6/7/8 code get flag 2;
// rigid objects (type 0) have all node BCs cleared. -1 when the file is missing.
int fun_140141580(MeshGenBase& mg)
{
    char line[0x400];
    LLOBJ_DATA* obj = mg.u008;
    std::FILE* f = crt_fopen("NBCD.OUT", "rt");
    if (f == nullptr) return -1;
    std::int32_t best = -1;
    double tol = ext::fun_140202a20(obj);  // p67-5
    tol = tol * tol;
    std::int32_t n = 0;
    std::fgets(line, 0x400, f);
    std::sscanf(line, "%d\n", &n);
    double p[3] = {0.0, 0.0, 0.0};  // stack locals: kept between lines when sscanf matches less
    std::int32_t c[3] = {0, 0, 0}, u1 = 0, u2 = 0;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        std::fgets(line, 0x400, f);
        std::sscanf(line, "%lf %lf %lf %d %d %d %d %d\n", &p[0], &p[1], &p[2], &c[0], &c[1], &c[2], &u1, &u2);
        double dmin = 1e30;  // DAT_140509b50
        best = -1;
        for (std::int32_t k = 0; k < node_count(obj); k = k + 1) {
            const double* q = node_coords(obj) + k * 3;
            const double d = dist2(p, q);
            if (d < dmin) {
                best = k;
                dmin = d;
            }
        }
        if (dmin <= tol && -1 < best) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                if (c[j] / 100 == 6) {
                    bc_code(obj)[best * 3 + j] = 6;
                    bc_plane(obj)[best * 3 + j] = c[j] - 600;
                } else {
                    bc_code(obj)[best * 3 + j] = c[j];
                }
            }
        }
    }
    std::fclose(f);
    if (obj_type(obj) != 0) {
        auto* planes = reinterpret_cast<model::linked::Cls_140257340List*>(reinterpret_cast<char*>(obj) + 0x2e80);
        planes->copy_from(*sym_planes(gk::surface(*obj, 0)));  // 140257c80
    }
    for (std::int32_t i = 0; i < model::bc::edge_face_slot_count(*obj); i = i + 1) {  // 140165170
        bool shared = false;
        std::int32_t a = 0, b = 0, cc = 0, d = 0;
        model::bc::face_nodes(*obj, i, &a, &b, &cc, &d);  // 140201210
        for (std::int32_t j = 0; j < 3; j = j + 1) {
            const std::int32_t code = bc_code(obj)[(a - 1) * 3 + j];
            if (code != 6 && code != 7 && code != 8) continue;
            const std::int32_t plane = bc_plane(obj)[(a - 1) * 3 + j];
            bool hit = false;
            for (std::int32_t k = 0; k < 3; k = k + 1) {
                if (bc_code(obj)[(b - 1) * 3 + k] == code && bc_plane(obj)[(b - 1) * 3 + k] == plane) {
                    hit = true;
                    break;
                }
            }
            if (!hit) continue;
            hit = false;
            for (std::int32_t k = 0; k < 3; k = k + 1) {
                if (bc_code(obj)[(cc - 1) * 3 + k] == code && bc_plane(obj)[(cc - 1) * 3 + k] == plane) {
                    hit = true;
                    break;
                }
            }
            if (hit) {
                shared = true;
                break;
            }
        }
        if (!shared) {
            at<std::int32_t*>(obj, 0xb00)[i] = 2;
            at<std::int32_t*>(obj, 0xb18)[i] = 2;
        }
    }
    if (obj_type(obj) == 0) {
        for (std::int32_t k = 0; k < node_count(obj); k = k + 1) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                bc_code(obj)[k * 3 + j] = 0;
                bc_plane(obj)[k * 3 + j] = 0;
            }
        }
    }
    return 0;
}

// @src 1402395d0  calloc(n, 0x18) (3-vectors), message on failure
static model::p407::Vec3* alloc_vec3(std::int32_t n)
{
    if (n == 0) return nullptr;
    void* p = std::calloc(static_cast<std::size_t>(n), 0x18);
    if (p == nullptr) {
        // 14039d3a0("%s Error when allocating %s memory", "ERROR", "DEFdvector3", n)
        std::printf("%s Error when allocating %s memory\n", "ERROR", "DEFdvector3");
    }
    return static_cast<model::p407::Vec3*>(p);
}

// @src 140155600  area of surface 0 (or of the object's surface polygons without one)
double surface_area(LLOBJ_DATA* obj)
{
    double area = 0.0;
    std::int32_t np = 0;
    const std::int32_t* poly = nullptr;
    const double* xyz = nullptr;
    OBJ_GEO3D* s = gk::surface(*obj, 0);
    if (s == nullptr || s->node_count() < 1) {
        if (obj == nullptr || at<std::int32_t>(obj, 0xbf0) < 1) return 0.0;
        np = at<std::int32_t>(obj, 0xbf0);
        poly = at<std::int32_t*>(obj, 0xbf8);
        xyz = node_coords(obj);
    } else {
        np = s->elem_count();
        poly = gk::geo3d_elems(s);
        xyz = gk::geo3d_coords(s);
    }
    if (xyz == nullptr) return 0.0;
    model::p407::Vec3* pts = alloc_vec3(4);
    for (std::int32_t i = 0; i < np; i = i + 1) {
        for (std::int32_t k = 0; k < 4; k = k + 1) {
            const std::int32_t v = std::abs(poly[i * 4 + k]);
            const double* q = xyz + (v - 1) * 3;
            pts[k] = model::p407::Vec3{{q[0], q[1], q[2]}};  // 140239700
        }
        area = area + model::p407::polygon_area(4, pts);  // 14023e670
    }
    if (pts != nullptr) std::free(pts);
    return area;
}

// @src 1401559d0  3D size scaling: element size factor r from the area and MGNELM (obj+0x2d88 is
// forced to >= 1); with *apply, the MGUSER window densities are rescaled and obj+0x2d98 = r
double size_scaling(LLOBJ_DATA* obj, double area, std::int32_t n, const char* apply)
{
    if (area <= 1e-8) area = surface_area(obj);  // DAT_14051bed0
    std::int32_t k = n;
    if (n == 0) k = at<std::int32_t>(obj, 0x2d88) < 1 ? 1 : at<std::int32_t>(obj, 0x2d88);
    at<std::int32_t>(obj, 0x2d88) = k < 1 ? 1 : k;
    double h = 4.0 * (area / static_cast<double>(at<std::int32_t>(obj, 0x2d88)));  // DAT_14051f248
    const double s3 = rt::libm::crt_pow(3.0, 0.5);                                  // 140483f60
    h = std::sqrt(h / s3);                                                          // 140484644
    double r = (2.0 * h) / (1.0 + 1.0 / at<double>(obj, 0x2d20));
    if (1e-8 < std::fabs(r)) {
        r = 1.0 / r;
    } else {
        r = 1.0;
    }
    if (*apply != 0) {
        double& w2 = at<double>(obj, 0x2d98);
        if (w2 < 1e-8) w2 = 1.0;
        for (std::int32_t i = 1; i <= dbio::gendb::windows_2d10_count(mesh_block(obj)); i = i + 1) {
            model::LLWINDEF_DATA* w = model::winlist_nth(window_list(obj), i);
            double d = std::bit_cast<double>(w->u370);
            if (1e-12 < std::fabs(d)) {  // DAT_14051f260
                d = (d * r) / w2;
                w->u370 = std::bit_cast<std::uint64_t>(d);
            }
        }
        w2 = r;
    }
    return r;
}

} // namespace def::cmd
