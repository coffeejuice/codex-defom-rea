// _MeshGenBase helpers shared by the 2D and 3D mesher paths (packet p8-07): preparation, node
// temperature reset, porous density refresh, weight normalisation. Spec: DEFAMG.md sections 3, 4.0, 6.1.
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <cstring>

#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/p8_07_ext.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "mg_internal.hpp"
#include "mg_util.hpp"

namespace def::cmd {

using namespace mg;

// @src 140158ea0  byte obj+0x3948 (a 3D surface is not rebuilt from the mesh when set)
bool obj_keep_surface(const LLOBJ_DATA* obj) { return at<std::uint8_t>(obj, 0x3948) != 0; }

// @src 1401b06f0  byte obj+0x3f21 (BRDEXT: AMG_DXT succeeded)
bool obj_amg_dxt_ok(const LLOBJ_DATA* obj) { return obj->u3f21 != 0; }

// @src 1401438e0
void set_state_surface_read(MeshGenBase&) { g_mesher_state = 1; }

// @src 140145cc0
void set_state_coating_done(MeshGenBase&) { g_mesher_state = 3; }

// @src 140144810  uniform node temperature: u4761 / u4768
void detect_uniform_temperature(MeshGenBase& mg)
{
    mg.u4761 = false;
    mg.u4768 = 0.0;
    LLOBJ_DATA* obj = mg.u008;
    if (has_mesh(obj) == 0) {
        mg.u4761 = true;
        mg.u4768 = at<double>(obj, 0x1eb8);  // REFTMP
        return;
    }
    const std::int32_t n = node_count(obj);
    const std::int32_t w = mg.u3f48->u15f8[8];  // model+0x1618
    if (node_temperature(obj) == nullptr) {
        mg.u4761 = true;
        mg.u4768 = at<double>(obj, 0x1eb8);
        return;
    }
    const double first = *node_temperature(obj);
    for (std::int32_t i = 1; i < n * w; i = i + 1) {
        const double d = std::fabs(node_temperature(obj)[i] - first);  // 140484558
        if (0.001 < d) {  // DAT_14051f1e8
            mg.u4761 = false;
            return;
        }
    }
    mg.u4761 = true;
    mg.u4768 = first;
}

// @src 140144620  prepare: false when the object has neither geometry nor a mesh
bool meshgen_prepare(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    if (!rt::dimension_is_3(mg.u3f48)) {  // 1400015f0
        if (!model::geo_kw::geometry_has_edges(*obj) && has_mesh(obj) == 0) return false;  // 140160d20
        detect_uniform_temperature(mg);
        mg.u4760 = has_mesh(obj) != 0;
        std::int32_t count = 0;
        model::geo_kw::material_groups(*obj, &count, &g_mesher_group_id);  // 1401fb270
        if (!model::geo_kw::geometry_has_edges(*obj) && has_mesh(obj) != 0) {
            const char one = 1;
            ext::fun_1401fc730(obj, &one);
        }
        return true;
    }
    if (!dbio::gendb::object_has_geometry(obj) && has_mesh(obj) == 0) return false;  // 140160cc0
    detect_uniform_temperature(mg);
    mg.u4760 = has_mesh(obj) != 0;
    std::int32_t count = 0;
    model::geo_kw::material_groups(*obj, &count, &g_mesher_group_id);
    if (!dbio::gendb::object_has_geometry(obj) && has_mesh(obj) != 0 && !obj_keep_surface(obj)) {
        const char one = 1;
        ext::fun_1401fc730(obj, &one);
    }
    return true;
}

// @src 1401443c0  node temperatures after a new mesh: the uniform value, else 20.0 when the
// model's UNIT (model+0x750+0xfc = +0x84c) is 1, else 68.0 (68.0 also without a model)
void fun_1401443c0(MeshGenBase& mg)
{
    double t;
    if (!mg.u4761) {
        if (mg.u3f48 == nullptr) {
            t = 68.0;  // DAT_14051f1f8
        } else if (at<std::int32_t>(mg.u3f48, 0x84c) == 1) {
            t = 20.0;  // DAT_14051f1f0
        } else {
            t = 68.0;
        }
    } else {
        t = mg.u4768;
    }
    LLOBJ_DATA* obj = mg.u008;
    const std::int32_t n = node_count(obj) * mg.u3f48->u15f8[8];
    for (std::int32_t i = 0; i < n; i = i + 1) node_temperature(obj)[i] = t;
}

// @src 14020b260  obj+0x1ec0 (object density)
double obj_density(LLOBJ_DATA* obj) { return at<double>(obj, 0x1ec0); }

// @src 14020b280
void set_obj_density(LLOBJ_DATA* obj, double v) { at<double>(obj, 0x1ec0) = v; }

// @src 14020b1e0  every element density = v (meshed objects only)
void set_elem_density(LLOBJ_DATA* obj, double v)
{
    if (has_mesh(obj) == 0) return;
    for (std::int32_t i = 0; i < elem_count(obj); i = i + 1) {
        dbio::objcheck::elem_block_density(elems(obj))[i] = v;  // 140184050
    }
}

// @src 14020af50  min and max element density (0, 0 without a mesh or density array)
void elem_density_range(LLOBJ_DATA* obj, double* lo, double* hi)
{
    *hi = 0.0;
    *lo = 0.0;
    if (has_mesh(obj) == 0) return;
    const double scale = 1.0;  // DAT_140509c90
    if (dbio::objcheck::elem_block_density(elems(obj)) == nullptr) return;
    *hi = *dbio::objcheck::elem_block_density(elems(obj));
    *lo = *hi;
    for (std::int32_t i = 0; i < elem_count(obj); i = i + 1) {
        const double d = dbio::objcheck::elem_block_density(elems(obj))[i];
        if (1e-6 > d - *lo) *lo = d;  // DAT_14051f1b8
        if (d - *hi > 1e-6) *hi = d;
    }
    *lo = scale * *lo;
    *hi = scale * *hi;
}

// @src 14020ae70  density text: "%.12g", or "%.12g ~ %.12g" when the elements differ by >= 1e-6
void density_text(LLOBJ_DATA* obj, char* out)
{
    double hi = 0.0, lo = 0.0;
    if (has_mesh(obj) == 0) {
        rt::crt::sprintf(out, "%.12g", obj_density(obj));
        return;
    }
    elem_density_range(obj, &lo, &hi);
    if (1e-6 <= std::fabs(hi - lo)) {
        rt::crt::sprintf(out, "%.12g ~ %.12g", lo, hi);
    } else {
        rt::crt::sprintf(out, "%.12g", lo);
    }
}

// @src 14020b0f0  set the density from text (ignored when empty or a range "a ~ b")
void set_density_text(LLOBJ_DATA* obj, const char* text)
{
    double v = 0.0;
    if (std::strlen(text) == 0) return;
    for (std::uint32_t i = 0; i < std::strlen(text); i = i + 1) {
        if (text[i] == '~') return;
    }
    std::sscanf(text, "%lf", &v);
    const double scale = 1.0;
    v = v / scale;
    if (has_mesh(obj) != 0) set_elem_density(obj, v);
    set_obj_density(obj, v);
}

// @src 1401444f0  porous objects (type 2): density refreshed through its "%.12g" text
void fun_1401444f0(MeshGenBase& mg)
{
    LLOBJ_DATA* obj = mg.u008;
    if (obj_type(obj) != 2) return;
    density_text(obj, g_density_text);
    for (std::uint32_t i = 0; i < std::strlen(g_density_text); i = i + 1) {
        if (g_density_text[i] == '~') return;
    }
    double v = 1.0;
    if (obj_type(obj) == 2) v = obj_density(obj);
    rt::crt::sprintf(g_density_text, "%.12g", v);
    set_density_text(obj, g_density_text);
}

// @src 140144200  normalise the mesh weights of block w (obj+0x2d10) to sum 1
void normalise_weights(MeshGenBase& mg, std::byte* w)
{
    if (mg.u008 == nullptr) return;
    double& wtmp = at<double>(w, 0x18);
    double& wstn = at<double>(w, 0x20);
    double& wstr = at<double>(w, 0x28);
    double& wusr = at<double>(w, 0x30);
    double& wcuv = at<double>(w, 0x38);
    if (rt::dimension_is_2(mg.u3f48)) {  // 1400015e0
        if (dbio::gendb::windows_2d10_count(w) < 1) wusr = 0.0;  // 140189580
    } else if (0.001 < std::fabs(wusr) && dbio::gendb::windows_2d10_count(w) == 0) {
        wusr = 0.0;  // DAT_14051f1e8
    }
    double s = wcuv + wtmp;
    s = s + wstn;
    s = s + wstr;
    s = s + wusr;
    if (rt::dimension_is_2(mg.u3f48)) {
        if (std::fabs(s) <= 1e-10) s = 1.0;  // DAT_14051f1e0
    } else {
        if (std::fabs(s) <= 1e-8) s = 1.0;   // DAT_14051bed0
    }
    wcuv = wcuv / s;
    wtmp = wtmp / s;
    wstn = wstn / s;
    wstr = wstr / s;
    wusr = wusr / s;
}

} // namespace def::cmd
