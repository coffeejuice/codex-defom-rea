// MDT packet V: the per-object 3D viewer items of a GUI model and their cached bounding boxes
// (docs/re/moproj2mst/mdt/V/spec_V.md). Decompiles: deform-rea-work/gui_mo/s1/mdt/V/v*.c,
// ../L0/B/b3.c, ../L1/B/b2.c, b3.c, b0.c.
//
// Kept: what decides the box 0x140766200 reads at Save through 0x140c82d50 + 0x1411f5040(.., false):
// the view list (m+0x18) and settings list (m+0x20) with their growth rules, each view's object,
// its 3D box (view+0xd8), the box of its geometry rep (view+0x38) and mesh rep (view+0x40), the
// geometry-list item bindings and cached curve rectangles (view+0x48), the display flags of each
// settings item, the model box (m+0x148) and rect (m+0x108), and the engine side effects of a bind
// (the "*Corrupt* " rename 0x1408340f0 and the
// in-place sign fix 0x141284970 of the mesh polygons).
//
// Dropped (display only, no box and no engine state): colours (0x1411f28f0), contact display
// (0x1411fdd30, 0x140c8c930), node flag vector (0x1411fc900, 0x141201e90, 0x14122d9e0), surface
// items s3328 / s3458 (0x1411f3990, 0x14126c010, 0x1411f3ee0), the "all parts" boxes (0x1411f5c90,
// 0x1411f5f10), normals and patch centres, and unused Box3 derived fields (the radius is derived
// when 0x140c8e0f0 tests the rectangle fallback).
//
// Refused: populated bound geometry-list mesh rectangles (0x14126d0c0 -> 0x14124bec0), and display
// flag 0x2b (geometry-list item boxes, 0x1411f58e0). DOE display-option import can
// now set this flag; populated contributions still guard when consumed. The object's 2D
// curve and mesh item builders are recovered in viewer2d.cpp.
//
// Known limit: the binary unions the rep box patch by patch (0x141287090), the patches coming from a
// normal-angle flood fill (0x141282560); the port takes the faces in their own order as one patch.
// min / max are the same except for which of +0.0 / -0.0 wins a tie (minV / maxV take the later).
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>

#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/gui_twins_view.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/node_access.hpp>
#include <def_pre/menu56.hpp>

#include "engine_internal.hpp"
#include "viewer2d.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/doe_view_stepdata.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_property_table.hpp"

namespace m2m::engine {

namespace {

// ---- Box3 (0xe8 bytes; spec 1.1): valid flag, min, max as Qt 4.8 QVector3D floats ---------------
struct Box3 {
    bool valid = false;            // +0x00
    float mn[3] = {0.0f, 0.0f, 0.0f};   // +0x08 QVector3D
    float mx[3] = {0.0f, 0.0f, 0.0f};   // +0x30 QVector3D
};

// @src 0x1412196f0  reset: valid = 0, min = max = (0,0,0) (derived fields zeroed, not kept)
void boxReset(Box3& b)
{
    b = Box3{};
}

// @src 0x14121bf20  per component, z then y then x: (b > a) ? a : b (COMISD / JBE: ties take b);
// QVector3D::x() etc. return the float widened to double, the result is stored as float again
void minV(float r[3], const float a[3], const float b[3])
{
    float t[3];
    for (int c = 2; c >= 0; --c) t[c] = (static_cast<double>(b[c]) > static_cast<double>(a[c])) ? a[c] : b[c];
    r[0] = t[0]; r[1] = t[1]; r[2] = t[2];
}

// @src 0x14121c090  per component: (a > b) ? a : b (ties take b)
void maxV(float r[3], const float a[3], const float b[3])
{
    float t[3];
    for (int c = 2; c >= 0; --c) t[c] = (static_cast<double>(a[c]) > static_cast<double>(b[c])) ? a[c] : b[c];
    r[0] = t[0]; r[1] = t[1]; r[2] = t[2];
}

// @src 0x141219980  extend by one point (const float* p: QVector3D setX((double)p[0]) ... = p exactly)
// @src 0x14121c630  setFrom(const float*)
void boxExtend(Box3& b, const float* p)
{
    if (!b.valid) {
        b.mn[0] = p[0]; b.mn[1] = p[1]; b.mn[2] = p[2];
        b.mx[0] = p[0]; b.mx[1] = p[1]; b.mx[2] = p[2];
        b.valid = true;
    } else {
        const float q[3] = {p[0], p[1], p[2]};
        minV(b.mn, b.mn, q);
        maxV(b.mx, b.mx, q);
    }
}

// @src 0x14121a9f0  union (other by value)
// @src 0x141219610  construct valid from min / max (0x141219dd0: valid = 1, copies, derived fields)
Box3 boxUnion(const Box3& self, const Box3& other)
{
    if (!self.valid) return other;
    if (!other.valid) return self;
    Box3 out;
    minV(out.mn, self.mn, other.mn);
    maxV(out.mx, self.mx, other.mx);
    out.valid = true;
    return out;
}

// @src 0x1412399d0  box of a Rect2: min = (min.first, min.second, 0), max likewise, as floats
// (0x14123a610 / 0x14121c510); valid even for an invalid rect (0x141219dd0)
Box3 rectBox(const Rect2& r)
{
    Box3 b;
    b.mn[0] = static_cast<float>(r.mn[0]);
    b.mn[1] = static_cast<float>(r.mn[1]);
    b.mn[2] = 0.0f;
    b.mx[0] = static_cast<float>(r.mx[0]);
    b.mx[1] = static_cast<float>(r.mx[1]);
    b.mx[2] = 0.0f;
    b.valid = true;
    return b;
}

// @src 0x141219af0 -> 0x14121c200 -> QVector3D::length. The centre
// sum, half-sum and difference each round through QVector3D floats; length
// squares/adds as floats, then widens the sum before the double square root.
double boxRadius(const Box3& b)
{
    float d[3];
    for (int c = 0; c < 3; ++c) {
        const float sum = static_cast<float>(static_cast<double>(b.mn[c]) + b.mx[c]);
        const float centre = static_cast<float>(static_cast<double>(sum) / 2.0);
        d[c] = static_cast<float>(static_cast<double>(b.mx[c]) - centre);
    }
    const float squared = (d[0] * d[0] + d[1] * d[1]) + d[2] * d[2];
    return std::sqrt(static_cast<double>(squared));
}

// ---- surface reps (spec 1.4, 4) ----------------------------------------------------------------
// One rep (GeoRep 0x530 / MeshRep 0x5d8, common base 0x141233520). The only state the box needs is
// the rep's cached box +0x2f8, which 0x141235bd0 recomputes from the patch vertex buffers the last
// build wrote; those buffers never change after the build, so the box is kept instead of them.
struct Rep {
    bool patchMode = true;         // +0xf9 (ctor 0x141233720: 1)
    Box3 box;                      // +0x2f8
};

// @src 0x141233950  patch-mode build of a rep; the parts of 0x141282560 that decide which faces
//   exist (the early returns), 0x141284970 (sign of every 4th face entry made positive in place),
//   0x141282be0 / 0x1412813a0 (positions of each face corner as floats) and the rep box
//   0x141235bd0 -> 0x141287090 -> 0x1412820d0 (min / max over those positions)
// part of @src 0x141233840  (0x141235d20 / 0x141236010 display state dropped)
void repBuildPatches(Rep& r, std::int32_t type, std::int32_t nFaces, std::int32_t nNodes, std::int32_t* faces,
                     double* xyz, bool copy)
{
    std::vector<std::int32_t> own;           // rep+0x28 (GeoRep: owned copy)
    std::int32_t* f = faces;
    std::int32_t t = type;
    if (copy) {
        own.assign(static_cast<std::size_t>(nFaces << 2), 0);   // 0x140452660
        f = own.empty() ? nullptr : own.data();                 // 0x1400c3680
        for (std::int32_t i = 0; i < nFaces; ++i) {
            f[i << 2] = faces[i << 2];
            f[i * 4 + 1] = faces[i * 4 + 1];
            f[i * 4 + 2] = faces[i * 4 + 2];
            if (faces[i * 4 + 3] < 0) f[i * 4 + 3] = -faces[i * 4 + 3];
            else f[i * 4 + 3] = faces[i * 4 + 3];
        }
        if (f == nullptr) t = 3;
        else if (*f == f[3]) t = 3;
        else t = 4;
    }
    // 0x141234730: rep+0x08.. = type, nFaces, nNodes, f, xyz (vt+0x20 resizes display flags)
    // 0x141233c90: both patch sets cleared (patch count 0 -> empty box)
    Box3 acc;                                                   // 0x141287090: empty
    // 0x141282560(set1 = {xyz, null floats, f}, nNodes, nFaces, t, kind 1, 0)
    if (xyz == nullptr) {                                       // qDebug UNK_142d4cdb8
    } else if (f == nullptr) {                                  // qDebug UNK_142d4cdd8
    } else if (nNodes < 3) {
    } else if (nFaces < 1) {
    } else {
        // 0x141284970: per face, a negative 4th entry becomes positive (the engine array itself
        // for a mesh rep: copy == false)
        for (std::int32_t i = 0; i < nFaces; ++i) {
            std::int32_t* p = f + static_cast<std::ptrdiff_t>(i << 2);   // set+0x10 + (i << 2) * 4
            if (p[3] < 0) p[3] = -p[3];
        }
        // 0x1412865a0 incidence, 0x141284bc0 / 0x141284c30 flood fill, 0x1412851d0 patch lists:
        // every face lands in exactly one patch; taken here as one patch in face order.
        // 0x1412813a0 per patch: npf = set+0xdc = t, stride set+0x38 = 4; QVector<float>(faces *
        // npf * 6, 0); npf other than 3 / 4 returns with the zeros in place.
        const std::int32_t npf = t;
        if (npf == 3 || npf == 4) {
            for (std::int32_t F = 0; F < nFaces; ++F) {
                for (std::int32_t j = 0; j < npf; ++j) {
                    const std::int32_t n = f[F * 4 + j] - 1;
                    const float pos[3] = {static_cast<float>(xyz[n * 3]), static_cast<float>(xyz[n * 3 + 1]),
                                          static_cast<float>(xyz[n * 3 + 2])};
                    boxExtend(acc, pos);                        // 0x1412820d0: buf + 6i + 3
                }
            }
        } else if (npf > 0) {
            const float zero[3] = {0.0f, 0.0f, 0.0f};
            const std::int64_t corners = static_cast<std::int64_t>(nFaces) * npf;
            for (std::int64_t k = 0; k < corners; ++k) boxExtend(acc, zero);
        }
        // set2 (kind 2, display only): 0x141282560 again on the same arrays; the sign fix is
        // already done, so it changes nothing
    }
    r.box = acc;
}

// @src 0x141233840  build a rep: sub-object path when rep+0xf9 == 0 (0x141233cd0), else patches
void repBuild(Rep& r, std::int32_t type, std::int32_t nFaces, std::int32_t nNodes, std::int32_t* faces, double* xyz,
              bool copy)
{
    if (!r.patchMode)
        throw Error(M2M_E_UNSUPPORTED, "3D viewer: the sub-object rep path (0x141233cd0, rep+0xf9 == 0)");
    repBuildPatches(r, type, nFaces, nNodes, faces, xyz, copy);
}

// ---- view item (0x87c0, ctor 0x1411f07d0; spec 1.3) and settings item (0x48; spec 1.5) ----------
struct GeometryItem {
    bool bound = false;            // +0x188 is null in a fresh 0x14126c540 item
    int index = -1;                // +0x190, stored by 0x14126c760
    Rect2 rect;                    // +0xb0, initially invalid with zero endpoints
    bool rectKnown = true;         // reconstruction qualification of the cached rect
};

struct Settings;

struct View {
    explicit View(GuiModel& model) : ownerModel(&model) {} //1f07d0, QPointer+10
    GuiModelGuard ownerModel; //actual nonowning model identity, independent of QObject parent
    std::unique_ptr<Settings> savedSettings; //+90, ctor1f4e60 null; lazy backup creator remains guarded
    LLOBJ_DATA* obj = nullptr;     // +0x18
    std::optional<Rep> geo;        // +0x38 (one entry)
    std::optional<Rep> mesh;       // +0x40 (one entry)
    std::vector<GeometryItem> items; // +0x48 geometry-list items
    // +0x28 2D curve items: the vertex rows each built at bind (0x141267410, viewer2d.cpp)
    std::vector<std::vector<double>> curveItems;
    // +0x30 2D mesh items: entry 0's node positions (0x14124be10); made empty when missing
    std::vector<std::vector<float>> meshItems;
    Rect2 rect;                    // +0x98 2D rect
    Box3 box;                      // +0xd8
    doe::ViewStepDataState doeStepData; //actual view+340 / embedded+348
    std::unique_ptr<doe::SliceCollectionState> slice; // lazy actual view+338
};

struct Settings {
    std::uint8_t b0 = 0;           // +0x00
    std::int32_t i4 = 0;           // +0x04
    std::uint8_t flag[0x2e] = {};  // +0x08 .. +0x35
    float f38 = 0.0f;              // +0x38
    std::uint8_t b3c = 0, b3d = 0; // +0x3c / +0x3d
};

// display preferences 0x143d781e0 (+4 / +5): 1 in a fresh GUI (CRT init 0x141ca0b50 -> 0x1402d5520
// -> 0x1402d5570); m2m has no user preferences
constexpr std::uint8_t kPref4 = 1;   // DAT_143d781e4
constexpr std::uint8_t kPref5 = 1;   // DAT_143d781e5

// @src 0x1411ee670  default flags (reset: +4 = 0)
void settingsDefaults(Settings& s, const bool* reset)
{
    if (*reset) s.i4 = 0;
    for (int i = 0; i < 0x2e; ++i) s.flag[i] = 0;
    s.flag[0] = 1;
    s.flag[1] = 1;
    s.flag[2] = 1;
    s.flag[3] = 1;
    s.flag[8] = kPref4;
    s.flag[9] = kPref5;
}

// @src 0x1411ee5b0  ctor (+0x40 = m) -> 0x1411ee610
std::unique_ptr<Settings> newSettings()
{
    auto s = std::make_unique<Settings>();
    // @src 0x1411ee610  b0 = 0; defaults(&true); 0x1411f0750; +0x38 = 0.5f (DAT_1421ebc38)
    s->b0 = 0;
    const bool t = true;
    settingsDefaults(*s, &t);
    // @src 0x1411f0750
    s->b3c = 0;
    s->b3d = 0;
    s->f38 = 0.5f;
    return s;
}

// @src 0x1411eea40  flag i (0 outside 0..0x2d)
bool flagOf(const Settings& s, int i)
{
    return (i < 0 || i > 0x2d) ? false : s.flag[i] != 0;
}

// @src 0x1411ee920  (== 0x1411ee990) store the object's flags
void settingsStore(Settings& s, const bool* mesh, const bool* geo, const bool* reset)
{
    settingsDefaults(s, reset);
    s.flag[1] = 1;
    s.flag[2] = *mesh ? 1 : 0;
    s.flag[3] = *geo ? 1 : 0;
    s.b0 = 1;
}

}  // namespace

// The viewer state of one GuiModel: views (m+0x18), settings (m+0x20), model box (m+0x148).
struct Viewer {
    std::vector<std::unique_ptr<View>> views;
    // c82b70 extracts the pointer before unbind/destruction. At an unresolved
    // cleanup boundary retain this in-flight allocation outside the visible list.
    // Adapter storage destruction is not native cleanup completion.
    std::unique_ptr<View> pendingRemoval;
    std::vector<std::unique_ptr<Settings>> settings;
    Box3 modelBox;                 // +0x148
    Rect2 modelRect;               // +0x108 (2D model rect)
    bool modelRectKnown = true;    // false only for an unrecovered item contribution
};

namespace {

Viewer& viewerOf(GuiModel& m)
{
    if (!m.viewer) m.viewer = std::make_shared<Viewer>();   // views and settings exist from the ctor
    if (m.viewer->pendingRemoval)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice view removal is pending unrecovered cleanup");
    return *m.viewer;
}

// 140065800 (= DEF_PRE 140158d00): obj+0x39c0 == 2 (2D object)
bool is2D(const LLOBJ_DATA* o)
{
    return def::model::p407::obj_kind_is_2(o);
}

// 140c7c0e0 / 140c7c0a0: engine present and engine+0x1c == 2 / 3
int modelDim(const GuiModel& m)
{
    return m.step ? *at<std::int32_t>(m.step.get(), 0x1c) : 0;
}

// @src 0x140c7b370  geometry type of the model's sim control (+0x100), 0 without one or for a
// model that is not 2D
int simGeometryType(const GuiModel& m)
{
    const def::model::Cls_1401e7b10* sc = m.simCtl();                // 0x140c7b2d0
    if (!sc || modelDim(m) != 2) return 0;                            // 0x140c7c0e0
    return *at<std::int32_t>(const_cast<def::model::Cls_1401e7b10*>(sc), 0x100);
}

void unbind(View& view, bool full);

// @src 0x140c82b70: extract last pointer, full-unbind, then virtual destruction.
void removeLastView(Viewer& v)
{
    if (v.pendingRemoval)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice view removal is pending unrecovered cleanup");
    v.pendingRemoval = std::move(v.views.back());
    v.views.pop_back();
    unbind(*v.pendingRemoval, true);
    v.pendingRemoval.reset();
}

// @src 0x140c82a20  settings follow the views (never shrink)
void growSettings(Viewer& v)
{
    while (v.settings.size() < v.views.size()) v.settings.push_back(newSettings());
}

// @src 0x140c82930  views list = n items
void resizeViews(GuiModel& model, Viewer& v, const int* n)
{
    if (static_cast<int>(v.views.size()) == *n) return;
    while (*n < static_cast<int>(v.views.size())) removeLastView(v);
    while (static_cast<int>(v.views.size()) < *n) v.views.push_back(std::make_unique<View>(model));   // 0x1411f07d0
    growSettings(v);
}

// @src 0x140c82d50  view of object number no (1-based), the list grown to it; null for no < 1
View* viewOf(GuiModel& model, Viewer& v, const int* no)
{
    if (*no < 1) return nullptr;
    while (static_cast<int>(v.views.size()) < *no) v.views.push_back(std::make_unique<View>(model));
    const int c = static_cast<int>(v.views.size());
    resizeViews(model, v, &c);                                     // same size: no-op
    return v.views[static_cast<std::size_t>(*no - 1)].get();
}

// @src 0x140c832d0  settings i (1-based, existing entries only)
Settings* settingsAt(Viewer& v, const int* i)
{
    if (0 < *i && *i <= static_cast<int>(v.settings.size())) return v.settings[static_cast<std::size_t>(*i - 1)].get();
    return nullptr;
}

// @src 0x140c83080  1-based position of view in the list, 0 when null or absent
int viewPosition(const Viewer& v, const View* view)
{
    if (!view) return 0;
    for (std::size_t k = 0; k < v.views.size(); ++k)
        if (v.views[k].get() == view) return static_cast<int>(k) + 1;
    return 0;
}

// @src 0x1411f1a10  settings of a view: by its object number (0x1411f1ab0), else by its position
Settings* settingsOf(Viewer& v, View* view)
{
    int i = view->obj ? *at<std::int32_t>(view->obj, 8) : 0;   // 0x1411f1ab0
    Settings* s = settingsAt(v, &i);
    if (!s) {
        i = viewPosition(v, view);
        s = settingsAt(v, &i);
    }
    return s;
}

// @src 0x1411f36d0  geometry rep, created on first use (ctor 0x14126dff0)
Rep& geoRep(View& view)
{
    if (!view.geo) view.geo.emplace();
    return *view.geo;
}

// @src 0x1411f3770  mesh rep, created on first use (ctor 0x14122b8d0)
Rep& meshRep(View& view)
{
    if (!view.mesh) view.mesh.emplace();
    return *view.mesh;
}

// @src 0x1411f3810  geometry-list item i, the list grown to it (ctor 0x14126c540)
GeometryItem& item(View& view, int i)
{
    while (static_cast<int>(view.items.size()) <= i) view.items.emplace_back();
    return view.items[static_cast<std::size_t>(i)];
}

// @src 0x14126cae0 and 0x14126d0c0 (SfDisp2DCrossSection vt+0x48).
// A missing source leaves the previous cached rect alone. With a source the
// original resets it, samples each edge at 0.01, then unions the mesh item.
void refreshGeometryItem(View& view, GeometryItem& it)
{
    if (!it.bound || !view.obj || it.index < 0 ||
        it.index >= def::model::geo_kw::geometry_count(*view.obj)) return;
    auto* geometry = def::model::geo_kw::geometry(*view.obj, it.index);
    if (!geometry) return;
    rectReset(it.rect);
    it.rectKnown = true;
    for (int i = 0; i < static_cast<int>(geometry->edges.size()); ++i)
        it.rect = rectUnion(it.rect, curveItemRect(curveItemPoints(geometry->edge_curve(i), 0.01)));
    // 0x1408ee940 / 0x14124bec0 on GEO2D_AND_MESH2D::mesh is a
    // different source from the object's 2D mesh. Do not substitute that mesh
    // or silently use just the curves while its item path is unrecovered.
    if (geometry->mesh) {
        const auto* nodes = reinterpret_cast<const def::model::Cls_14017ef20*>(geometry->mesh->s008);
        const auto* elements = reinterpret_cast<const def::model::Cls_140182880*>(geometry->mesh->s148);
        // 0x1400c3570 / 0x1400c3590: no mesh contribution when either
        // count is below one. Positive counts lead to the unrecovered mesh
        // coordinate/connectivity accessors and item builder.
        if (nodes->count() > 0 && elements->count() > 0) it.rectKnown = false;
    }
}

// @src 0x1411f6210  patch mode for new reps: !(m+0x258); m+0x258 is set only by 0x140c911e0 from
// the separate editor model 0x140d44320, never on an operation model
bool patchModeOf(const GuiModel&)
{
    return true;
}

// @src 0x14126e130  geometry rep from OBJ_GEO3D s
void buildGeoRep(Rep& g, def::model::OBJ_GEO3D* s)
{
    // +0x4a8 = s; 0x14126e260 empty
    if (s) {
        std::int32_t nNodes = s->node_count();                        // 0x1400c34d0 = 1400034d0
        std::int32_t nFaces = s->elem_count();                        // 0x1400c3500 = 140003500
        std::int32_t type = def::model::geo3d_face_node_count(s);     // 0x140929bd0 = 1402fabc0
        double* xyz = s->coords.mutable_begin();                      // 0x1401b2680 = 14026a3c0
        std::int32_t* faces = s->elems.mutable_begin();               // 0x1401b26a0 = 1402fe9d0
        // @src 0x14126e1e0  build (copy) + rep box; 0x14126e780 face centres (display)
        repBuild(g, type, nFaces, nNodes, faces, xyz, true);
    }
}

// @src 0x14122bb60  mesh rep from the object's boundary polygons
// @src 0x14122bc00  build (no copy) + rep box; +0x528.. source pointers kept (not needed)
void buildMeshRep(Rep& r, LLOBJ_DATA* obj)
{
    // +0x510 = obj; 0x141230490: +0x548 = 0
    if (obj) {
        double* xyz = def::model::fun_1401808d0(reinterpret_cast<def::model::Cls_14017ef20*>(obj->s8a0));   // 0x140854af0
        repBuild(r, *at<std::int32_t>(obj, 0x89c), *at<std::int32_t>(obj, 0xbf0), *at<std::int32_t>(obj, 0x880),
                 *at<std::int32_t*>(obj, 0xbf8), xyz, false);
    }
}

// @src 0x1411f2050  bind of a 3D object: bind-time box (geometry U mesh, no flag filter)
void bind3D(GuiModel& m, View& view)
{
    LLOBJ_DATA* obj = view.obj;
    boxReset(view.box);
    if (def::model::geo_kw::has_surface_flag(*obj)) {                // 0x140834e20 = 140160d90
        Rep& g = geoRep(view);
        g.patchMode = patchModeOf(m);                                 // 0x141233800
        buildGeoRep(g, def::model::geo_kw::surface(*obj, 0));         // 0x14086c7c0 = 140198620
        view.box = boxUnion(view.box, g.box);                         // 0x141235b60 copy
    }
    if (*at<std::int32_t>(obj, 0x87c) != 0 && def::model::llobj_mesh_coords_plausible(obj)) {   // 0x1408340f0
        Rep& r = meshRep(view);
        r.patchMode = patchModeOf(m);
        buildMeshRep(r, obj);
        view.box = boxUnion(view.box, r.box);
    }
    view.items.clear();                                               // 0x14020db90
    const int n = def::model::geo_kw::geometry_count(*obj);           // 0x14086c580 = 1401983e0
    for (int i = 0; i < n; ++i) {
        GeometryItem& it = item(view, i);
        it.bound = true;                                              // 0x14126c760: +0x188 = view
        it.index = i;                                                 // +0x190 = i
        refreshGeometryItem(view, it);
    }
}

// @src 0x1411f3560  curve item i of the view, made (empty) when missing
std::vector<double>& curveItem(View& view, int i)
{
    while (static_cast<int>(view.curveItems.size()) <= i) view.curveItems.emplace_back();   // new 0x1c0, 0x1412671d0
    return view.curveItems[static_cast<std::size_t>(i)];
}

// @src 0x1411f3620  mesh item i of the view, made (empty) when missing
std::vector<float>& meshItem(View& view, int i)
{
    while (static_cast<int>(view.meshItems.size()) <= i) view.meshItems.emplace_back();     // new 0x298, 0x14124bb30
    return view.meshItems[static_cast<std::size_t>(i)];
}

// @src 0x1411f1d80  bind of a 2D object: curve items and the mesh item with the rect of their union
void bind2D(View& view)
{
    LLOBJ_DATA* obj = view.obj;
    rectReset(view.rect);                                             // 0x141238d00
    if (def::model::geo_kw::geometry_has_edges(*obj)) {               // 0x140834db0 = 140160d20
        for (int i = 0; i < def::model::geo_kw::curve_count(*obj); ++i) {   // 0x14086c4b0 = 140198310
            def::model::OBJ_GEO2D* c = def::model::geo_kw::curve(*obj, i);  // 0x14086c460 = 1401982c0
            std::vector<double>& it = curveItem(view, i);
            it = curveItemPoints(c, 0.01);                            // 0x141267410(item, c, DAT_142203f60)
            view.rect = rectUnion(view.rect, curveItemRect(it));      // 0x1412510a0, 0x141239830
        }
    }
    if (*at<std::int32_t>(obj, 0x87c) != 0 && def::model::llobj_mesh_coords_plausible(obj)) {   // 0x1408340f0
        std::vector<float>& it = meshItem(view, 0);
        it = meshItemPoints(obj);                                     // 0x14124be10
        view.rect = rectUnion(view.rect, meshItemRect(it));
    }
    // 0x1411f5c90: the "all parts" rect (display)
}

// @src 0x1411f0b50  unbind: obj = 0, every display item deleted (fresh reps per bind)
void unbind(View& view, bool full)
{
    view.obj = nullptr;
    // 1f0b50 clears four ctor-null opaque auxiliaries before 1fdc40. That
    // helper saves the QPointer owner BEFORE collection cleanup/delete/null.
    if (full) {
        GuiModel* const owner = view.ownerModel.get();
        if (view.slice) {
            doe::prepareEmptySliceCollectionDestruction(*view.slice, [&] {
                if (!owner || !owner->viewer)
                    throw Error(M2M_E_UNSUPPORTED, "DOE Slice cleanup Settings owner is unavailable");
                if (view.slice->viewIdentity != &view)
                    throw Error(M2M_E_UNSUPPORTED, "DOE Slice cleanup has a different actual View owner");
                if (auto* settings = settingsOf(*owner->viewer, &view)) settings->flag[5] = 0;
            });
            view.slice.reset(); //55e260 then actual View+338=null
        }
        // c80560 visits CURRENT visible views, even if this View had no Slice.
        // 1f4df0's ctor-null backup pointer returns before Settings lookup.
        if (owner && owner->viewer) {
            for (const auto& current : owner->viewer->views) {
                if (!current)
                    throw Error(M2M_E_UNSUPPORTED, "View update reaches an original null View dereference");
                if (current->savedSettings)
                    throw Error(M2M_E_UNSUPPORTED, "View saved-Settings restore remains unsupported");
            }
        }
    }
    view.geo.reset();
    view.mesh.reset();
    view.items.clear();
    view.curveItems.clear();
    view.meshItems.clear();
}

// @src 0x1411f17f0  bind a view to obj (null: unbind)
void bind(GuiModel& m, View& view, LLOBJ_DATA* obj)
{
    if (!obj) {
        unbind(view, true);                                           // 0x1411f0b50(view, 1)
        return;                                                       // 0x1411f4e80 (display)
    }
    unbind(view, false);                                          // 0x1411f0b50(view, 0)
    view.obj = obj;
    if (!is2D(obj)) bind3D(m, view);
    else bind2D(view);
    // 1fdd30 consumes the existing Slice collection after actual object/reps
    // rebinding, before viewerSync refresh. Never silently retain stale binding.
    if (view.slice)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice object-rebind adaptation remains unsupported");
    // Remaining 0x1411f28f0 colours / 0x1411fc900 node display remain qualified.
}

// @src 0x1411ee800  display flags from the object (exactly one of mesh / geometry for 3D)
void setDefaultFlags(Settings& s, LLOBJ_DATA* obj, const bool* reset)
{
    bool mesh = false, geo = false;
    if (!is2D(obj)) {
        if (!def::model::llobj_is_rigid(obj)) {                      // 0x140835070 = 140160fe0
            if (objHasMesh(obj)) mesh = true;                         // 0x140834c80
            else if (def::model::geo_kw::has_surface_flag(*obj)) geo = true;   // 0x140834e20
        } else {
            if (def::model::geo_kw::has_surface_flag(*obj)) geo = true;
            else if (objHasMesh(obj)) mesh = true;
        }
        settingsStore(s, &mesh, &geo, reset);                         // 0x1411ee920
    } else {
        if (objHasMesh(obj)) mesh = true;
        if (def::model::geo_kw::geometry_has_edges(*obj)) geo = true; // 0x140834db0
        settingsStore(s, &mesh, &geo, reset);                         // 0x1411ee990
    }
}

// @src 0x1411f5540  recompute the 3D box of a view from its reps and the display flags
void recompute3D(Viewer& v, View& view)
{
    boxReset(view.box);
    Settings* s = settingsOf(v, &view);
    if (!s) return;
    if (flagOf(*s, 3) && view.obj && objHasGeometry(view.obj)) {      // 0x1411f1c00: 140834d50
        Rep& g = geoRep(view);                                        // 0x141235bd0: same buffers
        view.box = boxUnion(view.box, g.box);
    }
    if (flagOf(*s, 0x2b) && !view.items.empty())                      // 0x1411f1c40
        throw Error(M2M_E_UNSUPPORTED, "3D viewer: geometry-list item boxes (flag 0x2b, 0x1411f58e0)");
    if (flagOf(*s, 2) && view.obj && objHasMesh(view.obj)) {          // 0x1411f1bc0: 140834c80
        Rep& r = meshRep(view);                                       // empty rep when none was built
        view.box = boxUnion(view.box, r.box);
    }
}

// @src 0x1411f5250  recompute the 2D rect of a view from its items (vt+0x48 of each)
void recompute2D(Viewer& v, View& view)
{
    rectReset(view.rect);                                             // 0x141238d00
    Settings* s = settingsOf(v, &view);
    if (!s) return;
    if (flagOf(*s, 3) && view.obj && objHasGeometry(view.obj)) {      // 0x1411f1c00
        for (int i = 0; i < def::model::geo_kw::curve_count(*view.obj); ++i)   // 0x14086c4b0
            view.rect = rectUnion(view.rect, curveItemRect(curveItem(view, i)));   // 0x141267530
    }
    if (flagOf(*s, 2) && view.obj && objHasMesh(view.obj))           // 0x1411f1bc0
        view.rect = rectUnion(view.rect, meshItemRect(meshItem(view, 0)));        // 0x14124c3d0
}

// @src 0x140c8e640  refresh: per view box recompute and the model box
void refresh(GuiModel& m)
{
    Viewer& v = viewerOf(m);
    const int dim = modelDim(m);
    if (dim == 2) {
        rectReset(v.modelRect);                                       // 0x141238d00(m+0x108)
        v.modelRectKnown = true;
        for (auto& p : v.views) {
            View* view = p.get();
            if (view && view->obj && def::model::llobj_not_kind7(view->obj)) {   // 0x1411f1b80
                Settings* s = settingsOf(v, view);
                if (s && flagOf(*s, 1)) {
                    recompute2D(v, *view);
                    v.modelRect = rectUnion(v.modelRect, view->rect); // 0x1411f51d0
                    // shown in a display (m+0x10) whose axis display is on (+0x202, 1 from its ctor
                    // 0x140cf8e64), axisymmetric (1) or (3) model (0x140c7b370): the rect widened to
                    // x = 0 (0x1404ec860, 0x14123a280, 0x141238bf0)
                    if (m.display && displayAxisOn(*m.display)) {
                        const int geo = simGeometryType(m);           // 0x140c7b370
                        if (geo == 1 || geo == 3) {
                            Rect2 r = v.modelRect;
                            r.mn[0] = 0.0;
                            r.valid = true;
                            v.modelRect = rectUnion(v.modelRect, r);
                        }
                    }
                }
            }
        }
    } else if (dim == 3) {
        rectReset(v.modelRect);                                       // 0x141238d00(m+0x108)
        v.modelRectKnown = true;
        boxReset(v.modelBox);
        for (auto& p : v.views) {
            View* view = p.get();
            LLOBJ_DATA* obj = view->obj;                              // 0x1411f19f0
            if (obj && def::model::llobj_not_kind7(obj)) {            // 0x140834fa0
                Settings* s = settingsOf(v, view);
                if (!s) {
                    // qDebug UNK_1428fe258: box left as bound
                } else if (flagOf(*s, 1)) {
                    recompute3D(v, *view);
                    v.modelBox = boxUnion(v.modelBox, view->box);     // 0x1411f51f0
                    GeometryItem& it = item(*view, 0);                 // creates an unbound empty item if absent
                    refreshGeometryItem(*view, it);                    // item vt+0x48
                    v.modelRect = rectUnion(v.modelRect, it.rect);     // 0x1412510a0, 0x141239830
                    v.modelRectKnown = v.modelRectKnown && it.rectKnown;
                }
            }
        }
    }
}

}  // namespace

void viewerSync(GuiModel& m, LLOBJ_DATA* obj, bool flag)
{
    if (!obj) return;
    Viewer& v = viewerOf(m);
    int n = m.step ? def::model::last_object_number(*m.step) : 0;    // 0x140c7c220 -> 0x1408301c0 = 1401d6df0
    resizeViews(m, v, &n);
    View* view = viewOf(m, v, at<std::int32_t>(obj, 8));
    if (!view) return;
    bind(m, *view, obj);                                              // 0x1411f17f0(v, obj, true)
    Settings* s = settingsOf(v, view);
    if (s && flag) {
        const bool t = true;
        setDefaultFlags(*s, obj, &t);
    }
    refresh(m);                                                       // 0x140c8e640
}

void viewerRebuild(GuiModel& m, bool a, bool b)
{
    if (!m.step) return;                                              // 0x140c7acd0
    Viewer& v = viewerOf(m);
    if (a) viewerClear(m);                                            // 0x140c82c70
    int n = def::model::last_object_number(*m.step);                  // 0x140c7c220
    resizeViews(m, v, &n);
    int i = 0;
    for (auto& p : v.views) {
        View* view = p.get();
        ++i;
        LLOBJ_DATA* obj = m.objectAt(i);                              // 0x1408302a0 = 14017aa80
        bind(m, *view, obj);                                          // 0x1411f17f0(v, obj, a)
        Settings* s = settingsOf(v, view);
        if (a) {
            const bool t = true;
            if (!s || !obj)
                throw Error(M2M_E_UNSUPPORTED, "3D viewer: rebuild with a view lacking settings or object "
                                               "(0x1411ee800 on null)");
            setDefaultFlags(*s, obj, &t);                             // 0x1411ee800
        }
    }
    refresh(m);                                                       // 0x140c8e640
    (void)b;                                                          // b: 0x140c8c930 contact display
}

// @src 0x140c82c70  remove every view (0x140c82b70 until empty); the settings items stay
void viewerClear(GuiModel& m)
{
    if (!m.viewer) return;
    if (m.viewer->pendingRemoval)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice view removal is pending unrecovered cleanup");
    while (!m.viewer->views.empty()) removeLastView(*m.viewer);
}

int viewerItemCount(const GuiModel& m)
{
    return m.viewer ? static_cast<int>(m.viewer->views.size()) : 0;
}

void finalizeStepDataPropertyTable(GuiModel& model, doe::PropertyTableState& table)
{
    if (table.owner() != &model)
        throw Error(M2M_E_UNSUPPORTED, "DOE property table belongs to a different actual model");
    // d7c260 copies the QList of pointers before c7b1a0 refresh. This vector
    // preserves pointer identity/order; Qt allocation-failure equivalence is not
    // implied by the normal-return projection.
    std::vector<View*> views;
    if (model.viewer) {
        views.reserve(model.viewer->views.size());
        for (const auto& view : model.viewer->views) views.push_back(view.get());
    }
    if (model.step) def::pre::step_current_object(model.step.get(), true);
    std::int32_t row = 0;
    for (View* view : views) {
        if (!view || !view->obj || *at<std::int32_t>(view->obj, 0x0c) == 7) continue;
        GuiModel* owner = view->ownerModel.get();
        Settings* settings = owner && owner->viewer ? settingsOf(*owner->viewer, view) : nullptr;
        if (!settings) continue;
        for (std::int32_t column = 1; column <= 6; ++column) {
            // Native item(row,column) can return null; in that branch it never
            // reads a flag, changes a checkstate, or emits itemChanged.
            if (!table.itemIsAbsent(row, column))
                throw Error(M2M_E_UNSUPPORTED, "DOE populated property-table checkbox item remains unsupported");
        }
        ++row;
    }
}

void writeViewerStepDataObjects(GuiModel& model, xml::Node& stepData)
{
    std::vector<View*> views;
    if (model.viewer) {
        views.reserve(model.viewer->views.size());
        for (const auto& view : model.viewer->views) views.push_back(view.get());
    }
    int ordinal = 0;
    for (View* view : views) {
        auto object = xml::createElement(qt::L("Object"));
        xml::setAttribute(*object, qt::L("No"), qt::number(ordinal++));
        if (!view)
            throw Error(M2M_E_UNSUPPORTED, "DOE Object writer reached native null View access");
        GuiModel* owner = view->ownerModel.get();
        Settings* settings = owner && owner->viewer ? settingsOf(*owner->viewer, view) : nullptr;
        if (settings && view->obj) {
            auto options = xml::createElement(qt::L("SfDispOpt"));
            object->children.push_back(options); // before the child serializer
            doe::writeDisplayOptionFields(*options, settings->flag, settings->i4,
                *at<std::int32_t>(view->obj, 0x0c));
        }
        if (view->slice && view->slice->graphicCount > 0) {
            object->children.push_back(xml::createElement(qt::L("Slice")));
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice graphic payload writer remains unsupported");
        }
        doe::writeObjectSymmetry(view->doeStepData, *object);
        stepData.children.push_back(std::move(object)); // only on normal return
    }
}

doe::ViewStepDataState& viewerStepDataState(GuiModel& model, int index)
{
    if (index < 0 || !model.viewer ||
        static_cast<std::size_t>(index) >= model.viewer->views.size() ||
        !model.viewer->views[static_cast<std::size_t>(index)])
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData Object needs an actual existing viewer item");
    return model.viewer->views[static_cast<std::size_t>(index)]->doeStepData;
}

void readViewerDisplayOptions(GuiModel& model, int index, const xml::Node& node)
{
    // Validate without viewerOf/viewOf: neither native call grows the list here.
    (void)viewerStepDataState(model, index);
    View* view = model.viewer->views[static_cast<std::size_t>(index)].get();
    Settings* settings = settingsOf(*model.viewer, view);
    if (!settings) return; // native null-settings branch skips reset and import
    if (!view->obj)
        throw Error(M2M_E_UNSUPPORTED, "DOE display options with settings and no bound object reach native invalid access");
    const bool reset = true;
    setDefaultFlags(*settings, view->obj, &reset);
    // The discriminator is native OBJECT KIND at +0x0c, not model dimension.
    doe::readDisplayOptionFields(node, settings->flag, settings->i4,
        *at<std::int32_t>(view->obj, 0x0c));
    settings->b3c = settings->flag[2]; // 1411f06f0, only after import returns
    settings->b3d = settings->flag[3];
}

std::optional<ViewerDisplayOptionSnapshot> viewerDisplayOptionSnapshot(const GuiModel& model, int index)
{
    if (index < 0 || !model.viewer ||
        static_cast<std::size_t>(index) >= model.viewer->views.size()) return std::nullopt;
    View* view = model.viewer->views[static_cast<std::size_t>(index)].get();
    Settings* settings = view ? settingsOf(*model.viewer, view) : nullptr;
    if (!settings) return std::nullopt;
    ViewerDisplayOptionSnapshot snapshot;
    snapshot.mode = settings->i4;
    for (std::size_t i = 0; i < snapshot.flags.size(); ++i) snapshot.flags[i] = settings->flag[i];
    snapshot.cachedMesh = settings->b3c;
    snapshot.cachedGeometry = settings->b3d;
    return snapshot;
}

void readViewerSlice(GuiModel& model, int index, const doe::SliceCoefficients& coefficients,
                     doe::SliceConstructorRuntime* runtime)
{
    (void)viewerStepDataState(model, index); // existing View, no list growth
    View* view = model.viewer->views[static_cast<std::size_t>(index)].get();
    if (!view->slice) {
        if (!runtime)
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice constructor shared runtime is unavailable");
        view->slice = doe::createSliceCollection(view, *runtime); //1411fd600; commit after ctor
    }
    Settings* settings = settingsOf(*model.viewer, view);
    const bool savedSlice = settings && settings->flag[5] != 0;
    // 1411fe3f0 consumes plane coefficients before resolving model.db. Keep
    // the allocated collection even when unavailable coefficients stop here.
    const auto values = doe::makeSlicePlaneValues(coefficients);
    if (model.db) {
        if (!model.db->slicePlanes)
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice needs its actual database plane store");
        (void)doe::findOrCreateSlicePlane(*model.db->slicePlanes, values);
        // df8520 commits these real collection backlinks before lazy backend
        // construction. No graphic is appended and no flag is enabled yet.
        view->slice->model = &model;
        view->slice->viewIdentity = view;
        view->slice->boundObject = view->obj;
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice graphics backend construction remains unsupported");
    }
    // Native no-DB factory result is null; outer reader restores only after
    // normal return. An adapter refusal above does not run a synthetic finally.
    if (settings) settings->flag[5] = savedSlice;
}

const doe::SliceCollectionState* viewerPendingSliceCleanup(const GuiModel& model) noexcept
{
    return model.viewer && model.viewer->pendingRemoval
        ? model.viewer->pendingRemoval->slice.get() : nullptr;
}

const doe::SliceCollectionState* viewerSliceState(const GuiModel& model, int index) noexcept
{
    if (index < 0 || !model.viewer || static_cast<std::size_t>(index) >= model.viewer->views.size())
        return nullptr;
    const auto& view = model.viewer->views[static_cast<std::size_t>(index)];
    return view ? view->slice.get() : nullptr;
}

bool viewerBox(GuiModel& m, int objNo, float mn[3], float mx[3])
{
    Viewer& v = viewerOf(m);
    View* view = viewOf(m, v, &objNo);                                   // 0x140c82d50
    if (!view) return false;
    // @src 0x1411f5040  box of a view, recompute = false
    Box3 b;                                                           // 0x1412195a0: empty
    if (view->obj) {
        if (!is2D(view->obj)) {
            b = view->box;                                            // view+0xd8
        } else {
            b = rectBox(view->rect);                                  // 0x1412399d0
        }
    }
    for (int c = 0; c < 3; ++c) {
        mn[c] = b.mn[c];
        mx[c] = b.mx[c];
    }
    return true;
}

void viewerRefresh(GuiModel& m)
{
    refresh(m);
}

int guiModelDim(const GuiModel& m)
{
    return modelDim(m);
}

// @src 0x140c8e0f0  the model's scene box: a 2D model's rect (m+0x108) as a box; for other
// models the 3D box m+0x148, unless some view shows flag 1 and none of the views with flag 1 shows
// geometry (3), mesh (2) or flag 0x2d, when the rect box is taken if its radius exceeds 1e-6. That
// rect comes from item 0 of each shown 3D view. Bound geometry-mesh item
// contributions remain refused; empty/unbound and curve-only items are known.
bool viewerSceneBox(GuiModel& m, bool& valid, float mn[3], float mx[3])
{
    Viewer& v = viewerOf(m);
    Box3 b;                                                           // 0x1412195a0
    if (modelDim(m) == 2) {                                           // 0x140c7c0e0
        b = rectBox(v.modelRect);                                     // 0x1412399d0(m+0x108)
    } else {
        bool useRect = true, shown = false;
        for (auto& p : v.views) {
            Settings* s = p ? settingsOf(v, p.get()) : nullptr;       // 0x1411f1a10
            if (s && flagOf(*s, 1)) {                                 // 0x1411eea40(s, 1)
                shown = true;
                if (flagOf(*s, 3) || flagOf(*s, 2) || flagOf(*s, 0x2d)) useRect = false;
            }
        }
        if (!shown) useRect = false;
        b = v.modelBox;                                               // m+0x148
        if (useRect) {
            if (!v.modelRectKnown)
                throw Error(M2M_E_UNSUPPORTED, "3D viewer: a bound geometry-list mesh item rectangle "
                                               "(0x14126d0c0 -> 0x14124bec0) is not traced");
            const Box3 candidate = rectBox(v.modelRect);
            // Original COMISD/JBE after fabs(radius): equality and unordered
            // comparisons fall back to m+0x148. A radius is nonnegative (or
            // NaN), so the ordered positive comparison is equivalent here.
            if (boxRadius(candidate) > 1.0e-6) b = candidate;
        }
    }
    valid = b.valid;
    for (int c = 0; c < 3; ++c) {
        mn[c] = b.mn[c];
        mx[c] = b.mx[c];
    }
    return true;
}

// The scene exists after start-up: the main window's set-up 0x140c98b00 makes it (0x140ca5850)
bool glScenePresent() { return true; }

}  // namespace m2m::engine
