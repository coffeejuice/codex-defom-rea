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
// Geometry-list meshes (0x14126d0c0 -> 0x14124bec0) admit the recovered populated 2D layout and
// a qualified ownerless connectivity-null readiness skip. Missing coordinates and unqualified
// ownership/registry or populated builder layouts retain the scene-box guard. The object's 2D
// curve and mesh item builders are recovered in viewer2d.cpp.
//
// Known limit: the binary unions the rep box patch by patch (0x141287090), the patches coming from a
// normal-angle flood fill (0x141282560); the port takes the faces in their own order as one patch.
// min / max are the same except for which of +0.0 / -0.0 wins a tie (minV / maxV take the later).
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>
#include <new>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include <defdbio/dbread/w2.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
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
#include <defmodel/layout/state_var_node.hpp>
#include <defmodel/layout/state_var_node_post.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <def_pre/menu56.hpp>

#include "engine_internal.hpp"
#include "gui_model_access.hpp"
#include "geometry_temp_prefix.hpp"
#include "mirror_plane_owner.hpp"
#include "viewer2d.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/doe_view_stepdata.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_property_table.hpp"

namespace m2m::engine {

namespace detail {
// Private forwarding observation for focused tests; normal execution has none.
// It cannot replace the real mutable lookup or its result.
using GeometrySizingLookupObserver = void (*)(const void*, std::int64_t, bool, const def::model::StateVarBase*);
GeometrySizingLookupObserver geometrySizingLookupObserverForTest = nullptr;
// Metadata-only resource admission observation; it never substitutes a native allocation.
using GeometryRecipientAppendObserver = void (*)(std::size_t, std::size_t, std::int32_t);
GeometryRecipientAppendObserver geometryRecipientAppendObserverForTest = nullptr;
}

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

// 141249140 owns 128 fixed 0x108-byte native records, not a requested-plane
// sidecar. Constructor-unknown matrices remain absent until 141247a70 writes them.
struct MirrorPlaneRecord {
    std::optional<std::array<double, 16>> matrixDouble; // +0x40..+0xb8
    std::optional<std::array<float, 16>> matrixFloat;   // +0x00..+0x3c
    std::uint8_t mode = 1;                  // 1412479e0: +0xf8
    bool active = false;                    // +0xf9
    std::array<double, 4> c0{};              // +0xc0..+0xd8
    std::optional<std::array<double, 3>> e0;  // native ctor leaves +0xe0..+0xf0 unknown
    std::optional<int> a;                    // +0x104, set by 14004e810
    std::array<int, 2> links{-1, -1};         // +0xfc/+0x100
};

struct MirrorPlaneOwner {
    std::array<MirrorPlaneRecord, 128> records;
    // 141249680 reuses an inactive slot below this extent before increasing it.
    int allocatedSlotCount = 0;              // +0x8468, not number of active records
    std::array<int, 2> links{-1, -1};         // +0x846c/+0x8470
};

// 141247ed0: disabling a record also removes both record links.
void setMirrorPlaneActive(MirrorPlaneRecord& record, bool active)
{
    record.active = active;
    if (!active) record.links = {-1, -1};
}

// CP447 static contracts: 140e23a00 -> 140945d20/140946ca0 -> 141249730.
// This is a guarded prefix, not the native builder's completed behavior.
void importEnabledMirrorPlanePrefix(MirrorPlaneOwner& owner, const mst::MirrorPlane& plane)
{
    // 141249420's extent result is unused. The local identity is overwritten
    // by the reflection helper, which uses the supplied, unnormalized normal.
    std::array<double, 16> matrix{1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
    const std::array<double, 3> n{static_cast<double>(plane.v[3]),
        static_cast<double>(plane.v[4]), static_cast<double>(plane.v[5])};
    const std::array<double, 3> p{static_cast<double>(plane.v[0]),
        static_cast<double>(plane.v[1]), static_cast<double>(plane.v[2])};
    const double d = (n[0] * p[0] + n[1] * p[1]) + n[2] * p[2]; //140945050
    // Preserve 140946ca0's multiplication, assignment and symmetry-copy order.
    matrix[0] = 1.0 - 2.0 * n[0] * n[0];
    matrix[5] = 1.0 - 2.0 * n[1] * n[1];
    matrix[10] = 1.0 - 2.0 * n[2] * n[2];
    matrix[15] = 1.0;
    matrix[1] = -2.0 * n[0] * n[1];
    matrix[2] = -2.0 * n[0] * n[2];
    matrix[6] = -2.0 * n[1] * n[2];
    matrix[4] = matrix[1];
    matrix[8] = matrix[2];
    matrix[9] = matrix[6];
    matrix[12] = 2.0 * d * n[0];
    matrix[13] = 2.0 * d * n[1];
    matrix[14] = 2.0 * d * n[2];
    matrix[3] = 0.0;
    matrix[7] = 0.0;
    matrix[11] = 0.0;

    if (plane.a != 0)
        throw Error(M2M_E_UNSUPPORTED, "CopyMirror positive View plane builder: nonzero parent matrix composition remains unsupported");
    // CP456: 141249730 returns id zero at exactly 128 before searching inactive
    // records or consuming bounds. 141249f80 then inserts zero at the root
    // (14124a010); lookup(0) skips all record work and the builder returns.
    if (owner.allocatedSlotCount == 128) {
        if (owner.links[0] == -1) owner.links[0] = 0;
        else if (owner.links[1] == -1) owner.links[1] = 0;
        return;
    }
    int slot = 0;
    while (slot < owner.allocatedSlotCount && owner.records[static_cast<std::size_t>(slot)].active)
        ++slot;
    if (slot == owner.allocatedSlotCount) ++owner.allocatedSlotCount; //141249680
    MirrorPlaneRecord& record = owner.records[static_cast<std::size_t>(slot)];
    record.matrixDouble = matrix; //141247a70: all +0x40 doubles precede float stores
    record.matrixFloat.emplace();
    for (std::size_t i = 0; i < matrix.size(); ++i)
        (*record.matrixFloat)[i] = static_cast<float>(matrix[i]);
    record.mode = 0;
    setMirrorPlaneActive(record, true); //141247ed0 enabling preserves links

    // The next native call, 141249e40, consumes two builder-local triples
    // without established stores. Preserve preceding mutations and propagate
    // the guard through importMirrorEntry, before its fixed-row caller writes.
    throw Error(M2M_E_UNSUPPORTED, "CopyMirror positive View plane builder: unknown builder-local bounds before 0x141249e40");
}

struct View {
    explicit View(GuiModel& model) : ownerModel(&model) {} //1f07d0, QPointer+10
    GuiModelGuard ownerModel; //actual nonowning model identity, independent of QObject parent
    std::unique_ptr<Settings> savedSettings; //+90, ctor1f4e60 null; lazy backup creator remains guarded
    LLOBJ_DATA* obj = nullptr;     // +0x18
    std::optional<Rep> geo;        // +0x38 (one entry)
    std::optional<Rep> mesh;       // +0x40 (one entry)
    std::vector<GeometryItem> items; // +0x48 geometry-list items
    // +0x28 2D curve items: the vertex rows each built at bind (0x141267410, viewer2d.cpp)
    std::vector<CurveItemState> curveItems;
    // +0x30 2D mesh items: entry 0's node positions (0x14124be10); made empty when missing
    std::vector<std::vector<float>> meshItems;
    Rect2 rect;                    // +0x98 2D rect
    Box3 box;                      // +0xd8
    MirrorPlaneOwner mirrorPlanes; // actual View+0x348, 141249140
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

struct GeometryAssignmentProgress {
    std::int32_t ordinal = -1, marIndex = -1, marStride = 0;
};

// Each record observes one actual recipient inside the original live borrow.
// Relocating these plain observations never changes native ownership or copied storage.
struct GeometryMainRecipientProgress {
    enum class Stage {
        Unstarted, Started, NodeResetCommitted, CoordinatesPublished,
        BcAttemptPublished, BcAttemptsCommitted, VariableLeafCommitted,
        VariablesSizedCommitted, ScalarDimensionsCommitted, CoordinateCopyCommitted,
        ScalarTempRawNullPending, ScalarTempRawNonNullPending,
        ScalarValueRawNullPending, ScalarValueBeforeBodyPublication
    };
    Stage stage = Stage::Unstarted;
    std::int32_t ordinal = -1;
    const void* producedMainObject = nullptr;
    const double* producedMainCoordinates = nullptr;
    std::int32_t producedMainCoordinateCount = 0;
    const std::int32_t* producedMainBc[6]{};
    std::int32_t producedMainBcCounts[6]{};
    std::size_t mainBcAttempts = 0;
    const void* mainVariableRegistryBody = nullptr;
    const void* scalarValueBody = nullptr; // Observation only; no nonempty storage is admitted.
    const def::model::StateVarBase* mainVariableReceivers[28]{};
    std::int32_t mainVariableCurBefore[28][3]{}, mainVariablePrevBefore[28][3]{};
    std::size_t mainVariablesCompleted = 0;
    const double* mainCopySource = nullptr;
    const double* mainCopyDestination = nullptr;
    std::size_t mainCopyBytes = 0;
    std::int32_t mainCopyOrdinal = -1;
};

static_assert(std::is_trivially_copyable_v<GeometryMainRecipientProgress>);
static_assert(std::is_nothrow_move_constructible_v<GeometryMainRecipientProgress>);
static_assert(std::is_nothrow_default_constructible_v<GeometryMainRecipientProgress>);

// Observational frame of one suspended native call. Borrowed identities are
// never dereferenced on a parked retry; no native ownership/refcount is added.
class GeometrySyncBarrier final {
public:
    enum class Stage {
        PreparedForQuery, QueryCommitted, AAllocationCommitted, EmptyRegionCacheCommitted,
        ArraysTimerCommitted, MainRecipientsInProgress, NodeSkipLoopCommitted, CoordinateReturned
    };
    Stage stage = Stage::PreparedForQuery;
    GuiModelGuard model;
    const DB_STEP_DATA* step = nullptr;
    std::weak_ptr<Viewer> viewer;
    const void* view = nullptr;
    std::size_t itemOrdinal = 0;
    std::int32_t geometryIndex = -1;
    const void* object = nullptr;
    const void* geometry = nullptr;
    const void* mesh = nullptr;
    const void* nodes = nullptr;
    const void* elements = nullptr;
    std::weak_ptr<DbHandle> dbOwner;
    def::model::directory_receipts::Receipt directory;
    std::int32_t queriedIndex = 0, oldVersion = 0, postVersion = 0;
    std::int32_t id = 0x0b, flag = 1, which = 0, dimension = 3;
    std::int32_t entryCache = 0;
    std::int32_t hsize = 0, marSize = 0, arraySize = 0, recordVersion = 0;
    GeometryAssignmentProgress assignment;
    std::uint64_t queryResult = 0;
    // Observed producer extent only. The actual step owns/frees this allocation.
    const double* producedA = nullptr;
    std::int32_t producedACount = 0;
    std::vector<GeometryMainRecipientProgress> mainRecipients;
    // Explicit bounded experiment only; the separate module record retains T
    // even after this observational frame is destroyed. Never a CowVector owner.
    std::shared_ptr<detail::GeometryTempPrefixBinding> scalarTemporary;
    Rect2 sampledEdges;
};

namespace {
bool geometryCompletedMainRecipientsMatch(DB_STEP_DATA& step, const GeometrySyncBarrier& frame,
    std::size_t completed);
}

struct GeometrySyncAccess {
    static void attemptScalarTemporary(DB_STEP_DATA&, LLOBJ_DATA&,
        const std::shared_ptr<GeometrySyncBarrier>&, GeometryMainRecipientProgress&) noexcept;
    static bool pending(const GuiModel& model) noexcept { return !!model.geometrySyncBarrier_; }
    static void requireAvailable(const GuiModel& model)
    {
        if (pending(model))
            throw Error(M2M_E_UNSUPPORTED, "geometry query is parked before unrecovered reader continuation");
    }
    static bool install(GuiModel& model, std::shared_ptr<GeometrySyncBarrier> frame) noexcept
    {
        if (!frame || model.geometrySyncBarrier_) return false;
        model.geometrySyncBarrier_ = std::move(frame);
        return true;
    }
    // Read-only proof for a prefix inside the original high callback, never
    //authority to reacquire/clear a parked frame or consume an arbitrary span.
    static bool matchesInitialHighProducer(const std::shared_ptr<GeometrySyncBarrier>& frame,
        DB_STEP_DATA& step, GeometrySyncBarrier::Stage stage) noexcept
    {
        auto* model = frame ? frame->model.get() : nullptr;
        if (!model) return false;
        const auto& installed = model->geometrySyncBarrier_;
        if (!installed || installed.get() != frame.get() || installed.owner_before(frame) ||
            frame.owner_before(installed) || model->step.get() != &step || frame->step != &step ||
            frame->stage != stage || frame->queryResult == 0 || frame->postVersion < 6000 ||
            frame->id != 11 || frame->flag != 1 || frame->which != 0 || frame->dimension != 3 ||
            step.dimension != 3 || step.u1b58 != 0 || step.step_version != frame->postVersion ||
            step.u26c4[11] != 1 || !frame->producedA || frame->producedACount < 3 ||
            step.u1b78 != frame->producedA || step.u2b24 < 0 ||
            step.u2b24 > std::numeric_limits<std::int32_t>::max() - 2 ||
            frame->producedACount != step.u2b24 + 2) return false;
        const auto owner = frame->dbOwner.lock();
        return owner && owner.get() == model->db && owner->reader.get() == step.db &&
            frame->directory.owner == owner.get() && frame->directory.reader == step.db &&
            def::model::directory_receipts::isCurrent(frame->directory);
    }
    // Single callsite inside the initial prepared borrow, never a parked retry.
    static bool completeInitialCoordinateRead(GuiModel& model,
        const std::shared_ptr<GeometrySyncBarrier>& frame, DB_STEP_DATA* borrowedStep) noexcept
    {
        const auto& installed = model.geometrySyncBarrier_;
        if (!frame || !installed || installed.get() != frame.get() ||
            installed.owner_before(frame) || frame.owner_before(installed) ||
            frame->model.get() != &model || !borrowedStep || model.step.get() != borrowedStep ||
            frame->stage != GeometrySyncBarrier::Stage::NodeSkipLoopCommitted ||
            frame->queryResult == 0 || frame->postVersion >= 6000 ||
            frame->id != 0x0b || frame->flag != 1 || frame->which != 0 || frame->dimension != 3)
            return false;
        //14082dc81..dc8a returns true; the which0 tail ignores AL, reads
        //the current version, then returns below11000. Getter returns N+8.
        if (borrowedStep->step_version != frame->postVersion) return false;
        frame->stage = GeometrySyncBarrier::Stage::CoordinateReturned;
        model.geometrySyncBarrier_.reset();
        return true;
    }
    // A distinct high producer proof; never relax the existing low completer.
    static bool completeInitialHighCoordinateRead(GuiModel& model,
        const std::shared_ptr<GeometrySyncBarrier>& frame, DB_STEP_DATA* borrowedStep) noexcept
    {
        const auto& installed = model.geometrySyncBarrier_;
        if (!frame || !installed || installed.get() != frame.get() ||
            installed.owner_before(frame) || frame.owner_before(installed) ||
            frame->model.get() != &model || !borrowedStep || model.step.get() != borrowedStep ||
            frame->step != borrowedStep || frame->stage != GeometrySyncBarrier::Stage::NodeSkipLoopCommitted ||
            frame->queryResult == 0 ||
            frame->postVersion < 6000 || frame->id != 0x0b || frame->flag != 1 || frame->which != 0 ||
            frame->dimension != 3 || borrowedStep->dimension != 3 || borrowedStep->u1b58 != 0 ||
            borrowedStep->step_version != frame->postVersion || borrowedStep->u26c4[frame->id] != 1 ||
            !frame->producedA || frame->producedACount < 2 || borrowedStep->u1b78 != frame->producedA ||
            borrowedStep->u2b24 < 0 || borrowedStep->u2b24 > std::numeric_limits<std::int32_t>::max() - 2 ||
            frame->producedACount != borrowedStep->u2b24 + 2)
            return false;
        const auto owner = frame->dbOwner.lock();
        if (!owner || owner.get() != model.db || owner->reader.get() != borrowedStep->db ||
            frame->directory.owner != owner.get() || frame->directory.reader != borrowedStep->db ||
            !def::model::directory_receipts::isCurrent(frame->directory)) return false;
        if (!geometryCompletedMainRecipientsMatch(*borrowedStep, *frame, frame->mainRecipients.size())) return false;
        //Reader AL1/outer which0/version tail/N+8 are closed for this exact
        //initial borrow. The allocation remains owned by the actual step.
        frame->stage = GeometrySyncBarrier::Stage::CoordinateReturned;
        model.geometrySyncBarrier_.reset();
        return true;
    }
};

void requireModelGeometrySyncAvailable(const GuiModel& model)
{
    GeometrySyncAccess::requireAvailable(model);
}

namespace {

Viewer& viewerOf(GuiModel& m)
{
    GeometrySyncAccess::requireAvailable(m);
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
    return m.dimension();
}

// @src 0x140c7b370  geometry type of the model's sim control (+0x100), 0 without one or for a
// model that is not 2D
int simGeometryType(const GuiModel& m)
{
    return m.simGeometryTypeIf2D();
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

// A null dimension table is safe in the ownerless preallocated-coordinate
// getter only for the recovered key0-skipping node-variable methods. Inspect
// without mutable lookup: the actual getter below owns any native COW detach.
bool canonicalGeometryNodeVariables(const def::model::Cls_14017ef20& nodes)
{
    using namespace def::model;
    auto* collection = const_cast<Cls_14017ef20*>(&nodes);
    constexpr std::size_t count = sizeof(Cls_14017ef20::kRegs) / sizeof(Cls_14017ef20::kRegs[0]);
    if (ext::fun_14032a140(collection) != static_cast<std::int32_t>(count)) return false;
    for (std::size_t i = 0; i < count; ++i) {
        const auto* variable = ext::fun_14032a160(collection, static_cast<std::int32_t>(i));
        const auto& expected = Cls_14017ef20::kRegs[i];
        if (!variable || variable->id != expected.id) return false;
        if (expected.ctor == 0x14032a990) {
            if (typeid(*variable) != typeid(StateVarNode)) return false;
        } else if (expected.ctor == 0x14032aa20) {
            if (typeid(*variable) != typeid(StateVarNodePost)) return false;
        } else {
            return false;
        }
    }
    return true;
}

enum class GeometryReadiness {
    Unqualified, HighVersionReaderPending, HighVersionDbLoaderPending,
    HighVersionPostLoaderTimerPending, HighVersionObjectAssignmentPending,
    RepeatedGetterRequired, HighCachedGetterRequired, Skip, Populated
};

enum class GeometryGetterRound { Readiness, BuilderNullCompanion, BuilderHighCached };
struct GeometryGetterValues {
    const double* coordinates = nullptr;
    const std::int32_t* connectivity = nullptr;
};

//1408281fb..8274: fresh calloc and empty-region cache commit. This profile
//does not admit file-array reads or certify any preexisting A allocation.
bool qualifiedGeometryFreshEmptyA(const DB_STEP_DATA& step)
{
    return step.dimension == 3 && step.db != nullptr && step.u1b58 == 0 &&
        step.step_version >= 6000 && step.u1b68 > 0 && step.u1b6c > 0 &&
        step.u26c4[0x0b] == 0 && step.u1b78 == nullptr && step.u2b28[0x0b] == 0 &&
        step.u2b24 >= 0 && step.u2b24 <= std::numeric_limits<std::int32_t>::max() - 2;
}

bool geometryNodeSelectorsMatch()
{
    constexpr std::int32_t selectors[][2] = {
        {11, 11}, {70, 151}, {12, 12}, {13, 13}, {14, 15}, {15, 16},
        {16, 17}, {17, 18}, {18, 20}, {19, 21}, {20, 22}, {21, 23},
        {22, 24}, {23, 25}, {24, 26}, {25, 27}, {27, 29}, {32, 34},
        {35, 38}, {42, 46}, {55, 58}, {57, 61}
    };
    for (const auto& selector : selectors)
        if (def::model::g_a_index[selector[0]] != selector[1]) return false;
    return true;
}

//140842239..3af3: an existing main allocation and selected LLCRZ width
//mismatch return without reading A/MAR. Bound every other region selector.
bool qualifiedGeometryNodeWidthMismatch(const DB_STEP_DATA& step, const LLOBJ_DATA& object,
                                        std::int32_t id)
{
    return step.dimension == 3 && step.db == nullptr && step.u1b58 == 0 &&
        step.step_version >= 6000 && step.u26c4[0x0b] == 0 && id == 11 &&
        object.u87c[0] > 0 && *at<double*>(&object, 0x8a8) != nullptr &&
        geometryNodeSelectorsMatch() && step.u15f8[0] != step.u1dc0[151];
}

bool geometryMainDescriptorMatches(const DB_STEP_DATA& step, const LLOBJ_DATA& object)
{
    return object.owner == &step && object.u87c[0] > 0 && object.u87c[1] == 1 &&
        *at<std::int32_t>(&object, 0x3984) == 1 && step.u15f8[0] == 3 &&
        step.u1dc0[151] == 3 && def::model::g_index_tables_dim == 3 && geometryNodeSelectorsMatch();
}

bool emptyGeometryMainVariables(const def::model::Cls_14017ef20& nodes)
{
    for (const auto* pointer : nodes.u010) if (pointer) return false;
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(&nodes);
    if (variables.use_count() != 1 || variables.size() != 28u) return false;
    std::size_t index = 0;
    for (const auto* variable : variables.items()) {
        const auto& expected = def::model::Cls_14017ef20::kRegs[index++];
        if (!variable || variable->id != expected.id || variable->values.size() != 0 ||
            variable->values.use_count() != 1 ||
            variable->u30 != static_cast<const void*>(at<void*>(&nodes, expected.slot))) return false;
        if (expected.ctor == 0x14032a990) {
            if (typeid(*variable) != typeid(def::model::StateVarNode)) return false;
        } else if (expected.ctor == 0x14032aa20) {
            if (typeid(*variable) != typeid(def::model::StateVarNodePost)) return false;
        } else return false;
    }
    return true;
}

bool freshGeometryMainNodes(const DB_STEP_DATA& step, const LLOBJ_DATA& object)
{
    if (!geometryMainDescriptorMatches(step, object)) return false;
    const auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    if (nodes->u008 != nullptr || nodes->n != 0) return false;
    for (const auto* pointer : nodes->u0f0) if (pointer) return false;
    return emptyGeometryMainVariables(*nodes);
}

bool geometryMainDimensionsMatch(const def::model::Cls_14017ef20& nodes,
    const GeometryMainRecipientProgress& recipient, std::size_t completed)
{
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(&nodes);
    if (completed > 28 || variables.size() != 28u || variables.body() != recipient.mainVariableRegistryBody) return false;
    std::size_t ordinal = 0;
    for (const auto* variable : variables.items()) {
        if (variable != recipient.mainVariableReceivers[ordinal]) return false;
        const bool changed = ordinal < completed && def::model::Cls_14017ef20::kRegs[ordinal].ctor == 0x14032a990;
        for (std::size_t dim = 0; dim < 3; ++dim) {
            const auto expectedCurrent = changed && dim < 2 ? (dim == 0 ? 1 : 0) : recipient.mainVariableCurBefore[ordinal][dim];
            const auto expectedPrevious = changed && dim < 2 ? recipient.mainVariableCurBefore[ordinal][dim] :
                recipient.mainVariablePrevBefore[ordinal][dim];
            if (variable->cur[dim] != expectedCurrent || variable->prev[dim] != expectedPrevious) return false;
        }
        ++ordinal;
    }
    return ordinal == 28;
}

bool geometryCompletedMainRecipientsMatch(DB_STEP_DATA& step, const GeometrySyncBarrier& frame,
    std::size_t completed)
{
    if (completed > frame.mainRecipients.size() || (!frame.mainRecipients.empty() &&
        (step.u1378 <= 0 || frame.mainRecipients.size() > static_cast<std::size_t>(step.u1378)))) return false;
    for (std::size_t i = 0; i < completed; ++i) {
        const auto& recipient = frame.mainRecipients[i];
        if (recipient.stage != GeometryMainRecipientProgress::Stage::CoordinateCopyCommitted ||
            recipient.ordinal < 0 || recipient.ordinal >= step.u1378 ||
            !recipient.producedMainObject || !recipient.producedMainCoordinates ||
            recipient.producedMainCoordinateCount != 3 || recipient.mainBcAttempts != 6 ||
            recipient.mainVariablesCompleted != 28 || recipient.mainCopyBytes != 24 ||
            recipient.mainCopyOrdinal != recipient.ordinal || frame.producedACount < 3 ||
            recipient.mainCopySource != frame.producedA ||
            recipient.mainCopyDestination != recipient.producedMainCoordinates ||
            recipient.producedMainCoordinates == frame.producedA) return false;
        auto* object = static_cast<LLOBJ_DATA*>(def::model::step_object_at(step, recipient.ordinal + 1));
        if (object != recipient.producedMainObject || !geometryMainDescriptorMatches(step, *object)) return false;
        const auto* nodes = at<def::model::Cls_14017ef20>(object, 0x8a0);
        if (nodes->u008 != recipient.producedMainCoordinates || nodes->n != 1 || nodes->u0f0[6] != nullptr ||
            !emptyGeometryMainVariables(*nodes) || !geometryMainDimensionsMatch(*nodes, recipient, 28)) return false;
        for (std::size_t k = 0; k < 6; ++k)
            if (nodes->u0f0[k] != recipient.producedMainBc[k] || recipient.producedMainBcCounts[k] < 0 ||
                step.u15f8[0x12 + k] != recipient.producedMainBcCounts[k]) return false;
        for (const auto& entry : def::model::Cls_14017ef20::kRegs)
            if (entry.ctor == 0x14032a990 && step.u15f8[entry.id] != 0) return false;
        for (std::size_t prior = 0; prior < i; ++prior)
            if (frame.mainRecipients[prior].producedMainObject == object ||
                frame.mainRecipients[prior].producedMainCoordinates == nodes->u008 ||
                frame.mainRecipients[prior].ordinal >= recipient.ordinal) return false;
    }
    return true;
}

bool geometryMainActiveMatches(DB_STEP_DATA& step, const LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, const GeometryMainRecipientProgress& recipient,
    GeometryMainRecipientProgress::Stage stage)
{
    return GeometrySyncAccess::matchesInitialHighProducer(frame, step,
            GeometrySyncBarrier::Stage::MainRecipientsInProgress) &&
        !frame->mainRecipients.empty() &&
        &recipient == &frame->mainRecipients.back() && recipient.stage == stage &&
        recipient.producedMainObject == &object && recipient.ordinal == frame->assignment.ordinal &&
        recipient.ordinal >= 0 && recipient.ordinal < step.u1378 &&
        def::model::step_object_at(step, recipient.ordinal + 1) == &object &&
        geometryCompletedMainRecipientsMatch(step, *frame, frame->mainRecipients.size() - 1);
}

bool geometryMainBcStateMatches(DB_STEP_DATA& step, const LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient, std::size_t attempts, const std::int32_t* counts)
{
    if (!geometryMainActiveMatches(step, object, frame, recipient,
            attempts == 0 ? GeometryMainRecipientProgress::Stage::CoordinatesPublished :
                GeometryMainRecipientProgress::Stage::BcAttemptPublished) ||
        !geometryMainDescriptorMatches(step, object) || recipient.mainBcAttempts != attempts ||
        recipient.producedMainObject != &object || !recipient.producedMainCoordinates ||
        recipient.producedMainCoordinateCount != 3) return false;
    const auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    if (nodes->u008 != recipient.producedMainCoordinates || nodes->n != 0 || nodes->u0f0[6] != nullptr ||
        !emptyGeometryMainVariables(*nodes)) return false;
    for (std::size_t i = 0; i < 6; ++i) {
        if (step.u15f8[0x12 + i] != counts[i]) return false;
        if (i < attempts) {
            if (nodes->u0f0[i] != recipient.producedMainBc[i] || recipient.producedMainBcCounts[i] != counts[i]) return false;
        } else if (nodes->u0f0[i] != nullptr) return false;
    }
    return true;
}

void publishGeometryMainBc(DB_STEP_DATA& step, LLOBJ_DATA& object,
                           const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient)
{
    std::int32_t counts[6];
    for (std::size_t i = 0; i < 6; ++i) {
        counts[i] = step.u15f8[0x12 + i]; // Native n1 product is exactly this count.
        if (counts[i] < 0) return;
    }
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    for (std::size_t i = 0; i < 6; ++i) {
        if (!geometryMainBcStateMatches(step, object, frame, recipient, i, counts)) return;
        auto* produced = def::model::bc::calloc_ints(counts[i]);
        nodes->u0f0[i] = produced;
        recipient.producedMainBc[i] = produced;
        recipient.producedMainBcCounts[i] = counts[i];
        recipient.mainBcAttempts = i + 1;
        recipient.stage = GeometryMainRecipientProgress::Stage::BcAttemptPublished;
        //Native stores null too and continues later attempts, including a
        //diagnosed failure. No rollback or pointer-success gate is added.
    }
    if (!geometryMainBcStateMatches(step, object, frame, recipient, 6, counts)) return;
    nodes->n = 1; //140854342, only after all six attempts; seventh slot stays null.
    recipient.stage = GeometryMainRecipientProgress::Stage::BcAttemptsCommitted;
    // The separate bounded sizing prefix precedes the qualified fixed-row copy.
}

bool geometryMainSizingOwnerMatches(DB_STEP_DATA& step, const LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient,
    GeometryMainRecipientProgress::Stage stage, bool scalarPrefix = false)
{
    if ((scalarPrefix && frame && frame->mainRecipients.size() != 1) || !geometryMainActiveMatches(step, object, frame, recipient, stage) ||
        !geometryMainDescriptorMatches(step, object) || recipient.producedMainObject != &object ||
        !recipient.producedMainCoordinates || recipient.producedMainCoordinateCount != 3 ||
        recipient.mainBcAttempts != 6) return false;
    const auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    if (nodes->u008 != recipient.producedMainCoordinates || nodes->n != 1 || nodes->u0f0[6] != nullptr ||
        !emptyGeometryMainVariables(*nodes)) return false;
    for (std::size_t i = 0; i < 6; ++i)
        if (nodes->u0f0[i] != recipient.producedMainBc[i] ||
            step.u15f8[0x12 + i] != recipient.producedMainBcCounts[i]) return false;
    for (const auto& entry : def::model::Cls_14017ef20::kRegs)
        if (entry.ctor == 0x14032a990 && step.u15f8[entry.id] != (scalarPrefix && entry.id == 1 ? 1 : 0)) return false;
    return true;
}

bool geometryMainSizingProgressMatches(DB_STEP_DATA& step, const LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient, std::size_t completed, bool sized = false)
{
    if ((sized && completed != 28) || !geometryMainSizingOwnerMatches(step, object, frame, recipient, sized ?
            GeometryMainRecipientProgress::Stage::VariablesSizedCommitted : completed == 0 ?
                GeometryMainRecipientProgress::Stage::BcAttemptsCommitted : GeometryMainRecipientProgress::Stage::VariableLeafCommitted) ||
        recipient.mainVariablesCompleted != completed) return false;
    const auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    return geometryMainDimensionsMatch(*nodes, recipient, completed);
}

bool geometryMainScalarCommittedMatches(DB_STEP_DATA& step, LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient)
{
    if (recipient.mainVariablesCompleted != 0 ||
        !geometryMainSizingOwnerMatches(step, object, frame, recipient,
            GeometryMainRecipientProgress::Stage::ScalarDimensionsCommitted, true)) return false;
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(nodes);
    if (variables.body() != recipient.mainVariableRegistryBody) return false;
    std::size_t ordinal = 0;
    for (const auto* variable : variables.items()) {
        if (variable != recipient.mainVariableReceivers[ordinal]) return false;
        for (std::size_t dim = 0; dim < 3; ++dim) {
            const bool changed = ordinal == 0 && dim < 2;
            const auto current = changed ? 1 : recipient.mainVariableCurBefore[ordinal][dim];
            const auto previous = changed ? recipient.mainVariableCurBefore[ordinal][dim] :
                recipient.mainVariablePrevBefore[ordinal][dim];
            if (variable->cur[dim] != current || variable->prev[dim] != previous) return false;
        }
        if (ordinal == 0) {
            const auto* body = variable->values.body();
            if (variable->id != 1 || variable->cur[2] != 1 || body != recipient.scalarValueBody ||
                body->refs != 1 || body->first || body->last || body->end) return false;
        }
        ++ordinal;
    }
    return ordinal == 28;
}

//1409e49fe..4a2b: native ordered setters. Explicitly opted-in models may then
//observe only a candidate raw HeapAlloc0->1 return; no wrapper/resize return.
void sizeGeometryMainFirstScalarPrefix(DB_STEP_DATA& step, LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient)
{
    if (!geometryMainSizingOwnerMatches(step, object, frame, recipient,
            GeometryMainRecipientProgress::Stage::BcAttemptsCommitted, true) || recipient.mainVariablesCompleted != 0) return;
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(nodes);
    auto* first = variables.items().front();
    const auto* body = first->values.body();
    if (first->id != 1 || first->cur[0] != 0 || first->cur[2] != 1 ||
        body->first != nullptr || body->last != nullptr || body->end != nullptr) return;
    recipient.mainVariableRegistryBody = variables.body(); recipient.scalarValueBody = body;
    std::size_t ordinal = 0;
    for (const auto* variable : variables.items()) {
        recipient.mainVariableReceivers[ordinal] = variable;
        for (std::size_t dim = 0; dim < 3; ++dim) {
            recipient.mainVariableCurBefore[ordinal][dim] = variable->cur[dim];
            recipient.mainVariablePrevBefore[ordinal][dim] = variable->prev[dim];
        }
        ++ordinal;
    }
    if (def::model::ext::fun_14032a140(nodes) != 28) return;
    const auto observer = detail::geometrySizingLookupObserverForTest;
    if (observer) observer(nodes, 0, true, nullptr);
    auto** receiver = def::model::ext::fun_14019a860(nodes, 0); // Actual mutable lookup, never replaced.
    if (observer) observer(nodes, 0, false, receiver ? *receiver : nullptr);
    if (!geometryMainSizingOwnerMatches(step, object, frame, recipient,
            GeometryMainRecipientProgress::Stage::BcAttemptsCommitted, true) ||
        !geometryMainDimensionsMatch(*nodes, recipient, 0) || !receiver ||
        *receiver != recipient.mainVariableReceivers[0]) return;
    // The live registry/receiver was re-established before following its values body.
    auto* variable = *receiver; body = variable->values.body();
    if (variable->id != 1 || variable->cur[0] != 0 || variable->cur[2] != 1 ||
        body != recipient.scalarValueBody || body->first != nullptr || body->last != nullptr || body->end != nullptr) return;
    variable->set_dim0(1); //1409e4a07: prev0 then current0.
    variable->set_dim1(step.u15f8[variable->get_id()]); //1409e4a26: reload actual id/D, prev1 then current1.
    recipient.stage = GeometryMainRecipientProgress::Stage::ScalarDimensionsCommitted;
    GeometrySyncAccess::attemptScalarTemporary(step, object, frame, recipient);
    // Whole-leaf count stays0; no value_data/resize, value8, copy, free or reader return.
}

void sizeGeometryMainZeroVariables(DB_STEP_DATA& step, LLOBJ_DATA& object,
                                   const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient)
{
    if (!geometryMainSizingOwnerMatches(step, object, frame, recipient, GeometryMainRecipientProgress::Stage::BcAttemptsCommitted) ||
        recipient.mainVariablesCompleted != 0) return;
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(nodes);
    recipient.mainVariableRegistryBody = variables.body();
    std::size_t ordinal = 0;
    for (const auto* variable : variables.items()) {
        recipient.mainVariableReceivers[ordinal] = variable;
        for (std::size_t dim = 0; dim < 3; ++dim) {
            recipient.mainVariableCurBefore[ordinal][dim] = variable->cur[dim];
            recipient.mainVariablePrevBefore[ordinal][dim] = variable->prev[dim];
        }
        ++ordinal;
    }
    const auto count = def::model::ext::fun_14032a140(nodes); // Native snapshots actual registry count once.
    if (count != 28) return;
    for (std::int32_t i = 0; i < count; ++i) {
        if (!geometryMainSizingProgressMatches(step, object, frame, recipient, static_cast<std::size_t>(i))) return;
        const auto observer = detail::geometrySizingLookupObserverForTest;
        if (observer) observer(nodes, i, true, nullptr);
        auto** receiver = def::model::ext::fun_14019a860(nodes, i); // Always the actual detaching lookup.
        if (observer) observer(nodes, i, false, receiver ? *receiver : nullptr);
        if (!geometryMainSizingProgressMatches(step, object, frame, recipient, static_cast<std::size_t>(i)) ||
            !receiver || *receiver != recipient.mainVariableReceivers[i]) return;
        (*receiver)->vslot2_140301690(1, step.u15f8, -1); // Actual normal/post virtual dispatch, never a field substitute.
        recipient.mainVariablesCompleted = static_cast<std::size_t>(i + 1);
        recipient.stage = GeometryMainRecipientProgress::Stage::VariableLeafCommitted;
    }
    if (!geometryMainSizingProgressMatches(step, object, frame, recipient, 28)) return;
    recipient.stage = GeometryMainRecipientProgress::Stage::VariablesSizedCommitted;
    // The separately qualified fixed-row copy follows; no wider span is admitted.
}

bool copyGeometryMainRow(DB_STEP_DATA& step, LLOBJ_DATA& object,
                         const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient)
{
    if (!geometryMainSizingProgressMatches(step, object, frame, recipient, 28, true) || recipient.mainCopyBytes != 0 ||
        frame->assignment.ordinal < 0 || frame->assignment.ordinal >= step.u1378 ||
        frame->assignment.marIndex < 0 || frame->assignment.marIndex >= 0x8c) return false;
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    //n1/d3/first1 gives source offset0 and exactly24 bytes. Both pointers
    //still match their separate actual allocation producers in this frame.
    constexpr std::size_t bytes = 3 * sizeof(double);
    static_assert(bytes == 24);
    std::memcpy(nodes->u008, frame->producedA, bytes);
    recipient.mainCopySource = frame->producedA;
    recipient.mainCopyDestination = nodes->u008;
    recipient.mainCopyBytes = bytes;
    recipient.mainCopyOrdinal = frame->assignment.ordinal;
    recipient.stage = GeometryMainRecipientProgress::Stage::CoordinateCopyCommitted;
    //All remaining regions exclude id11. Their only selected tail effect is
    //the current-version read; the helper then returns without more writes.
    return step.step_version == frame->postVersion && geometryNodeSelectorsMatch();
}

bool publishGeometryMainCoordinates(DB_STEP_DATA& step, LLOBJ_DATA& object,
                                    const std::shared_ptr<GeometrySyncBarrier>& frame)
{
    if (!frame || step.u1378 <= 0 || frame->mainRecipients.size() >= static_cast<std::size_t>(step.u1378) ||
        frame->mainRecipients.size() == frame->mainRecipients.max_size() ||
        !GeometrySyncAccess::matchesInitialHighProducer(frame, step, frame->mainRecipients.size() == 0 ?
            GeometrySyncBarrier::Stage::ArraysTimerCommitted : GeometrySyncBarrier::Stage::MainRecipientsInProgress) ||
        !geometryCompletedMainRecipientsMatch(step, *frame, frame->mainRecipients.size()) ||
        !freshGeometryMainNodes(step, object) || frame->assignment.ordinal < 0 ||
        frame->assignment.ordinal >= step.u1378 ||
        def::model::step_object_at(step, frame->assignment.ordinal + 1) != &object) return false;
    for (std::size_t i = 0; i < frame->mainRecipients.size(); ++i)
        if (frame->mainRecipients[i].producedMainObject == &object ||
            frame->mainRecipients[i].ordinal >= frame->assignment.ordinal) return false;
    // Reconstruction bookkeeping only: failure parks before this recipient's native reset.
    // Existing native buffer-allocation calls remain outside this narrow catch.
    try {
        if (const auto observer = detail::geometryRecipientAppendObserverForTest)
            observer(frame->mainRecipients.size(), frame->mainRecipients.capacity(), frame->assignment.ordinal);
        frame->mainRecipients.emplace_back();
    } catch (const std::bad_alloc&) { return false; }
      catch (const std::length_error&) { return false; }
    // No reference into this vector survives a later append: this helper returns
    // through the native caller backedge before the next recipient can start.
    auto& recipient = frame->mainRecipients.back();
    recipient.producedMainObject = &object;
    recipient.ordinal = frame->assignment.ordinal;
    recipient.stage = GeometryMainRecipientProgress::Stage::Started;
    frame->stage = GeometrySyncBarrier::Stage::MainRecipientsInProgress; // One-way reader transition, never arrays replay.
    if (!geometryMainActiveMatches(step, object, frame, recipient,
            GeometryMainRecipientProgress::Stage::Started) || !freshGeometryMainNodes(step, object)) return false;
    //140837093/140853350: only this actual fresh recipient is reset.
    def::model::bc::free_node_arrays(object);
    recipient.stage = GeometryMainRecipientProgress::Stage::NodeResetCommitted;
    if (!geometryMainActiveMatches(step, object, frame, recipient,
            GeometryMainRecipientProgress::Stage::NodeResetCommitted) || !freshGeometryMainNodes(step, object)) return false;
    auto* produced = def::model::bc::calloc_doubles(3);
    *at<double*>(&object, 0x8a8) = produced;
    recipient.producedMainCoordinates = produced;
    recipient.producedMainCoordinateCount = produced ? 3 : 0;
    recipient.stage = GeometryMainRecipientProgress::Stage::CoordinatesPublished;
    if (produced) {
        publishGeometryMainBc(step, object, frame, recipient);
        if (recipient.stage == GeometryMainRecipientProgress::Stage::BcAttemptsCommitted) {
            if (step.u15f8[1] == 1) sizeGeometryMainFirstScalarPrefix(step, object, frame, recipient);
            else sizeGeometryMainZeroVariables(step, object, frame, recipient);
        }
        if (recipient.stage == GeometryMainRecipientProgress::Stage::VariablesSizedCommitted)
            return copyGeometryMainRow(step, object, frame, recipient);
    }
    return false;
}

//14082d8db..dc81: qualified synchronous main-object profiles and actual node Stop.
bool completeGeometryNodeSkip(DB_STEP_DATA& step, const std::int32_t& id, std::int32_t flag,
                              GeometryAssignmentProgress& progress,
                              const std::shared_ptr<GeometrySyncBarrier>& frame = {})
{
    bool state = false;
    if (id == def::model::g_a_index[27]) state = step.u15f8[0x1a] > 0;
    else if (id == def::model::g_a_index[32]) state = step.u15f8[0x1b] > 0;
    else if (id == def::model::g_a_index[35]) state = step.u15f8[0x1c] > 0;
    // The qualified helpers never consume the native wrapping32
    //IMUL/LEA MAR record address. Load its inputs without forming a C++ pointer.
    for (std::int32_t i = 0; i < step.u1378; ++i) {
        auto* object = static_cast<def::model::LLOBJ_DATA*>(def::model::step_object_at(step, i + 1));
        progress.ordinal = i;
        progress.marIndex = def::model::g_mar_index[27];
        if (progress.marIndex < 0 || progress.marIndex >= 0x8c) return false;
        progress.marStride = step.u1b90[progress.marIndex]; // Before every caller skip.
        if (!object || !def::model::llobj_not_kind7(object) ||
            def::dbio::dbread::obj_byte_3610(object) != 0) continue;
        if (state || flag == 0 || object->owner != &step) return false;
        if (object->u87c[0] != 0 && !qualifiedGeometryNodeWidthMismatch(step, *object, id)) {
            if (frame && publishGeometryMainCoordinates(step, *object, frame)) continue;
            return false;
        }
        def::dbio::dbread::obj_read_node_arrays(object, static_cast<std::int32_t*>(step.u1b88),
            static_cast<double*>(step.u1b78), step.u1b90, step.u1dc0, step.u15f8, step.u13e8, &id, 0);
    }
    if (frame && frame->mainRecipients.size() != 0 &&
        (!GeometrySyncAccess::matchesInitialHighProducer(frame, step,
            GeometrySyncBarrier::Stage::MainRecipientsInProgress) ||
         !geometryCompletedMainRecipientsMatch(step, *frame, frame->mainRecipients.size()))) return false;
    auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
    if (nodeTimer.count != 2) return false;
    const char report = 1;
    (void)nodeTimer.stop(&report);
    return true;
}

// 1408ee940: after positive counts the coordinate getter precedes connectivity.
// The canonical preallocated profile preserves the getter effects on both the
// known-false skip and populated branches. A nonnull node owner must be the
// view's actual live step. Only recovered reader returns may continue;
//a populated null-companion result needs its explicit second GUI getter.
GeometryReadiness qualifiedGeometryReadiness(def::model::OBJ_MESH2D& mesh, const View& view,
                                             const GeometryItem& sourceItem, const void* geometry,
                                             GeometryGetterRound round = GeometryGetterRound::Readiness,
                                             GeometryGetterValues* returned = nullptr)
{
    if (returned) *returned = {};
    auto* nodes = reinterpret_cast<def::model::Cls_14017ef20*>(mesh.s008);
    const auto* elements = reinterpret_cast<const def::model::Cls_140182880*>(mesh.s148);
    if (nodes->u008 == nullptr || !canonicalGeometryNodeVariables(*nodes))
        return GeometryReadiness::Unqualified;
    auto* const model = (nodes->u130 != nullptr || elements->u0f0 != nullptr)
        ? view.ownerModel.get() : nullptr;
    if (elements->u0f0 != nullptr) {
        if (!model || !model->step ||
            static_cast<const void*>(elements->u0f0) != model->step.get())
            return GeometryReadiness::Unqualified;
        //14082bf60 reads actual owner+1b59 but returns true for both zero and
        //nonzero. 14082ebf0 then fast-returns without replacing connectivity.
        //Only this live actual identity qualifies the ported getter's fold.
    }
    auto nullCompanionProfile = [nodes](DB_STEP_DATA* step) {
        return step && nodes->u130 == step && step->db == nullptr && step->dimension == 3 &&
            step->u1b58 == 0 && step->step_version >= 6000 && step->u26c4[0x0b] == 0;
    };
    auto highCachedProfile = [nodes, elements](DB_STEP_DATA* step) {
        return step && nodes->u130 == step &&
            (elements->u0f0 == nullptr || static_cast<const void*>(elements->u0f0) == step) &&
            step->db != nullptr && step->dimension == 3 && step->u1b58 == 0 &&
            step->step_version >= 6000 && step->u26c4[0x0b] == 1;
    };
    if (round == GeometryGetterRound::BuilderNullCompanion &&
        (!returned || !model || !model->step || nodes->u130 != model->step.get() ||
            !GuiModelAccess::withStep(*model, nullCompanionProfile)))
        return GeometryReadiness::Unqualified;
    if (round == GeometryGetterRound::BuilderHighCached &&
        (!returned || !model || !model->step ||
            !GuiModelAccess::withStep(*model, highCachedProfile)))
        return GeometryReadiness::Unqualified;
    bool coordinatePrefixCompleted = false;
    bool repeatedGetterPending = false;
    bool highCachedGetterPending = false;
    if (nodes->u130 != nullptr) {
        if (!model || !model->step || nodes->u130 != model->step.get())
            return GeometryReadiness::Unqualified;
        if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
                return nativeStep->u1b58 == 0;
            })) {
            const std::int32_t dimension = modelDim(*model);
            if (dimension != 3) return GeometryReadiness::Unqualified;
            if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
                    return nativeStep->step_version >= 6000;
                })) {
                //140854af0: allocation/real key0 COW dispatch precedes sync.
                //14082d7d0 then selects actual module index tables before its
                //id0x0b/cache/DB work. Recovered branches below retain those effects.
                const auto* dimensions = def::model::fun_14017f020(nodes);
                def::model::fun_14017ffe0(nodes, nodes->n, dimensions, 0);
                // Refuse callback-sensitive cached changes immediately after
                //the COW, before choosing a table/reader/query continuation.
                if (round == GeometryGetterRound::BuilderHighCached &&
                    !GuiModelAccess::withStep(*model, highCachedProfile))
                    return GeometryReadiness::HighVersionReaderPending;
                def::model::select_index_tables(&dimension);
                const auto marIndex = def::model::g_mar_index[3];
                if (marIndex < 0 || marIndex >= 0x8c ||
                    def::model::g_a_index[128] <= 0x0b)
                    return GeometryReadiness::HighVersionReaderPending;
                // The explicit builder round may only repeat the same closed
                //null-companion profile, never start a different DB/frame path.
                if (round == GeometryGetterRound::BuilderNullCompanion &&
                    !GuiModelAccess::withStep(*model, nullCompanionProfile))
                    return GeometryReadiness::HighVersionReaderPending;
                if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
                        return nativeStep->u26c4[0x0b] == 0;
                    })) {
                    //14082d895/d8ab: the actual process-global node reader timer
                    //records both entry checkpoints before the DB-loader call.
                    auto& timer = def::model::g_timer_read_obj_node_state_variable;
                    timer.start("Start");
                    timer.checkpoint("ReadSingleAArrayFromDB", nullptr);
                    if (GuiModelAccess::withStep(*model, [](DB_STEP_DATA* nativeStep) {
                            return nativeStep->db != nullptr;
                        })) {
                        // Pin the actual Project-owned handle before touching
                        // the model's raw DB pointer or its companion reader.
                        const auto* project = currentProject();
                        const auto dbOwner = project ? project->dataDb : nullptr;
                        if (!dbOwner || dbOwner.get() != model->db)
                            return GeometryReadiness::HighVersionDbLoaderPending;
                        auto* reader = dbOwner->reader.get();
                        if (!reader || GuiModelAccess::withStep(*model, [reader](DB_STEP_DATA* nativeStep) {
                                return reader != nativeStep->db;
                            }))
                            return GeometryReadiness::HighVersionDbLoaderPending;
                        namespace receipts = def::model::directory_receipts;
                        const auto receipt = receipts::current(dbOwner.get(), *reader);
                        std::int32_t index = 0;
                        GuiModelAccess::withStep(*model, [&index](DB_STEP_DATA* nativeStep) {
                            index = nativeStep->u740;
                        });
                        if (!receipt || index < 0 ||
                            static_cast<std::size_t>(index) >= receipt->elementCount ||
                            !receipts::isCurrent(*receipt))
                            return GeometryReadiness::HighVersionDbLoaderPending;
                        const auto* records = static_cast<const def::model::DbDirRecord*>(receipt->allocation);
                        if (records[index].version < 6000) {
                            // Prepare the durable model barrier before the
                            // setter changes the next refresh's branch.
                            if (!model->viewer || viewPosition(*model->viewer, &view) == 0)
                                return GeometryReadiness::HighVersionDbLoaderPending;
                            std::size_t ordinal = 0;
                            while (ordinal < view.items.size() && &view.items[ordinal] != &sourceItem) ++ordinal;
                            if (ordinal == view.items.size())
                                return GeometryReadiness::HighVersionDbLoaderPending;
                            std::shared_ptr<GeometrySyncBarrier> frame;
                            try { frame = std::make_shared<GeometrySyncBarrier>(); }
                            catch (const std::bad_alloc&) { return GeometryReadiness::HighVersionDbLoaderPending; }
                            frame->model.reset(model); frame->step = model->step.get();
                            frame->viewer = model->viewer; frame->view = &view;
                            frame->itemOrdinal = ordinal; frame->geometryIndex = sourceItem.index;
                            frame->object = view.obj; frame->geometry = geometry; frame->mesh = &mesh;
                            frame->nodes = nodes; frame->elements = elements;
                            frame->dbOwner = dbOwner; frame->directory = *receipt;
                            frame->queriedIndex = index; frame->oldVersion = model->step->step_version;
                            frame->entryCache = model->step->u26c4[0x0b]; frame->sampledEdges = sourceItem.rect;
                            // Admit this initial invocation before installing its own barrier.
                            auto pending = GeometryReadiness::HighVersionDbLoaderPending;
                            bool coordinateReturned = false;
                            GuiModelAccess::withStep(*model,
                                [model, reader, index, &frame, &pending, &coordinateReturned](DB_STEP_DATA* nativeStep) {
                                    if (!GeometrySyncAccess::install(*model, frame)) return;
                                    frame->queryResult = def::dbio::dbread::step_dir_sizes(nativeStep, reader, index,
                                        &frame->hsize, &frame->marSize, &frame->arraySize, &frame->recordVersion);
                                    frame->postVersion = nativeStep->step_version;
                                    frame->stage = GeometrySyncBarrier::Stage::QueryCommitted;
                                    if (frame->queryResult == 0 || frame->postVersion >= 6000) return;
                                    //14082bf10 returns the loader for this admitted old
                                    //version. Keep this one prepared borrow through
                                    //the bounded assignment prefix, never a retry.
                                    auto& arraysTimer = def::model::g_timer_read_arrays;
                                    pending = GeometryReadiness::HighVersionPostLoaderTimerPending;
                                    if (arraysTimer.count < 0) return;
                                    arraysTimer.checkpoint("Assign the variable to the object data", nullptr);
                                    frame->stage = GeometrySyncBarrier::Stage::ArraysTimerCommitted;
                                    pending = GeometryReadiness::HighVersionObjectAssignmentPending;
                                    if (!completeGeometryNodeSkip(*nativeStep, frame->id, frame->flag,
                                        frame->assignment)) return;
                                    frame->stage = GeometrySyncBarrier::Stage::NodeSkipLoopCommitted;
                                    coordinateReturned = GeometrySyncAccess::completeInitialCoordinateRead(
                                        *model, frame, nativeStep);
                                });
                            if (!coordinateReturned) return pending;
                            // First getter COW already completed before the query.
                            // Continue from its return without invoking it again.
                            coordinatePrefixCompleted = true;
                        }
                        else {
                            bool freshEmptyA = false;
                            GuiModelAccess::withStep(*model, [&freshEmptyA](DB_STEP_DATA* nativeStep) {
                                freshEmptyA = nativeStep && qualifiedGeometryFreshEmptyA(*nativeStep);
                            });
                            if (freshEmptyA) {
                                if (!model->viewer || viewPosition(*model->viewer, &view) == 0)
                                    return GeometryReadiness::HighVersionDbLoaderPending;
                                std::size_t ordinal = 0;
                                while (ordinal < view.items.size() && &view.items[ordinal] != &sourceItem) ++ordinal;
                                if (ordinal == view.items.size()) return GeometryReadiness::HighVersionDbLoaderPending;
                                std::shared_ptr<GeometrySyncBarrier> frame;
                                try { frame = std::make_shared<GeometrySyncBarrier>(); }
                                catch (const std::bad_alloc&) { return GeometryReadiness::HighVersionDbLoaderPending; }
                                frame->model.reset(model); frame->step = model->step.get();
                                frame->viewer = model->viewer; frame->view = &view;
                                frame->itemOrdinal = ordinal; frame->geometryIndex = sourceItem.index;
                                frame->object = view.obj; frame->geometry = geometry; frame->mesh = &mesh;
                                frame->nodes = nodes; frame->elements = elements;
                                frame->dbOwner = dbOwner; frame->directory = *receipt;
                                frame->queriedIndex = index; frame->oldVersion = model->step->step_version;
                                frame->entryCache = model->step->u26c4[0x0b]; frame->sampledEdges = sourceItem.rect;
                                bool highCoordinateReturned = false;
                                // Keep this initial borrow through the actual query/producer.
                                // Cache1 must never let a retry bypass unfinished assignment.
                                GuiModelAccess::withStep(*model,
                                    [model, nodes, reader, index, &frame, &highCoordinateReturned](DB_STEP_DATA* nativeStep) {
                                        if (!nativeStep || model->step.get() != nativeStep || frame->step != nativeStep ||
                                            nodes->u130 != nativeStep || nativeStep->db != reader || nativeStep->u740 != index ||
                                            !qualifiedGeometryFreshEmptyA(*nativeStep) ||
                                            !def::model::directory_receipts::isCurrent(frame->directory)) return;
                                        if (!GeometrySyncAccess::install(*model, frame)) return;
                                        frame->queryResult = def::dbio::dbread::step_dir_sizes(nativeStep, reader, index,
                                            &frame->hsize, &frame->marSize, &frame->arraySize, &frame->recordVersion);
                                        frame->postVersion = nativeStep->step_version;
                                        frame->stage = GeometrySyncBarrier::Stage::QueryCommitted;
                                        // Actual GUI14082bf10 is false for this high3D profile.
                                        // The ported DEF_PRE names predicate cannot replace it.
                                        if (frame->queryResult == 0 || !qualifiedGeometryFreshEmptyA(*nativeStep)) return;
                                        const auto count = nativeStep->u2b24 + 2;
                                        auto* produced = def::model::bc::calloc_doubles(count);
                                        nativeStep->u1b78 = produced;
                                        frame->producedA = produced;
                                        frame->producedACount = produced ? count : 0;
                                        frame->stage = GeometrySyncBarrier::Stage::AAllocationCommitted;
                                        if (!produced || nativeStep->u2b28[frame->id] != 0 ||
                                            nativeStep->u26c4[frame->id] != 0) return;
                                        nativeStep->u26c4[frame->id] = 1;
                                        frame->stage = GeometrySyncBarrier::Stage::EmptyRegionCacheCommitted;
                                        auto& arraysTimer = def::model::g_timer_read_arrays;
                                        if (arraysTimer.count < 0) return;
                                        arraysTimer.checkpoint("Assign the variable to the object data", nullptr);
                                        frame->stage = GeometrySyncBarrier::Stage::ArraysTimerCommitted;
                                        if (!completeGeometryNodeSkip(*nativeStep, frame->id, frame->flag,
                                            frame->assignment, frame)) return;
                                        frame->stage = GeometrySyncBarrier::Stage::NodeSkipLoopCommitted;
                                        highCoordinateReturned = GeometrySyncAccess::completeInitialHighCoordinateRead(
                                            *model, frame, nativeStep);
                                    });
                                if (!highCoordinateReturned) return GeometryReadiness::HighVersionDbLoaderPending;
                                coordinatePrefixCompleted = true;
                                highCachedGetterPending = true;
                            }
                            else {
                                std::int32_t hsize = 0, marSize = 0, arraySize = 0, version = 0;
                                GuiModelAccess::withStep(*model,
                                    [reader, index, &hsize, &marSize, &arraySize, &version](DB_STEP_DATA* nativeStep) {
                                        (void)def::dbio::dbread::step_dir_sizes(nativeStep, reader, index,
                                            &hsize, &marSize, &arraySize, &version);
                                    });
                                // Other high-record loader continuations remain unclosed.
                                return GeometryReadiness::HighVersionDbLoaderPending;
                            }
                        }
                    }
                    else {
                        //140828060 returns immediately for the actual null companion.
                        auto pending = GeometryReadiness::HighVersionObjectAssignmentPending;
                        bool coordinateReturned = false;
                        GuiModelAccess::withStep(*model,
                            [nodes, &pending, &coordinateReturned](DB_STEP_DATA* nativeStep) {
                                if (!nativeStep || nodes->u130 != nativeStep || nativeStep->db != nullptr ||
                                    nativeStep->dimension != 3 || nativeStep->u1b58 != 0 ||
                                    nativeStep->step_version < 6000 || nativeStep->u26c4[0x0b] != 0) return;
                                auto& arraysTimer = def::model::g_timer_read_arrays;
                                pending = GeometryReadiness::HighVersionPostLoaderTimerPending;
                                if (arraysTimer.count < 0) return;
                                arraysTimer.checkpoint("Assign the variable to the object data", nullptr);
                                pending = GeometryReadiness::HighVersionObjectAssignmentPending;
                                GeometryAssignmentProgress progress;
                                const std::int32_t id = 0x0b;
                                coordinateReturned = completeGeometryNodeSkip(*nativeStep, id, 1, progress);
                            });
                        if (!coordinateReturned) return pending;
                        // No query/cache/version effect. A populated mesh needs
                        //another explicitly qualified GUI getter at builder time.
                        coordinatePrefixCompleted = true;
                        repeatedGetterPending = true;
                    }
                }
                else if (GuiModelAccess::withStep(*model, highCachedProfile)) {
                    // Fresh cached refreshes use the same explicit second
                    //COW/cache guard as the just-completed high producer.
                    highCachedGetterPending = true;
                }
                // Cached false return and the completed low-record reader
                //both return through which0. The first coordinate COW has
                //already completed; preserve its return without replay.
                coordinatePrefixCompleted = true;
            }
        }
        //14082bf10: nonzero owner+1b58 returns true; on the public3D zero-byte
        //route, signed owner+1b64 <6000 also returns true. These take fast
        //exits; a cached high-version prefix above is already complete.
    }
    // Preallocated coordinates avoid dims[0]; the canonical methods skip key0
    // resizing. Invoke the real ported getter to preserve collection COW lookup.
    const double* coordinates = coordinatePrefixCompleted ? nodes->u008 : def::model::fun_140220440(&mesh);
    if (!coordinates) return GeometryReadiness::Skip;
    const auto* connectivity = def::model::fun_140220460(&mesh);
    if (returned) *returned = {coordinates, connectivity};
    if (!connectivity) return GeometryReadiness::Skip;
    if (round == GeometryGetterRound::Readiness) {
        if (repeatedGetterPending) return GeometryReadiness::RepeatedGetterRequired;
        if (highCachedGetterPending) return GeometryReadiness::HighCachedGetterRequired;
    }
    return GeometryReadiness::Populated;
}

// 0x14124bec0's rectangle is rebuilt from every node's float-rounded (x, y), while its
// connectivity is one-based and converted to zero-based for the display mesh. The geometry-owned
// OBJ_MESH2D is independent of the object's main LLOBJ_DATA node set. Its packed builder profile
// uses recovered arrays directly and accepts only the 2D layout initialized by OBJ_MESH2D::reset.
// Missing coordinates must not call the owner-derived allocator with null dimensions.
bool geometryMeshRect(def::model::OBJ_MESH2D& mesh, GeometryReadiness readiness,
                     const View& view, const GeometryItem& sourceItem, const void* geometry, Rect2& out)
{
    if (readiness != GeometryReadiness::Populated && readiness != GeometryReadiness::RepeatedGetterRequired &&
        readiness != GeometryReadiness::HighCachedGetterRequired)
        return false;
    const auto* nodes = reinterpret_cast<const def::model::Cls_14017ef20*>(mesh.s008);
    const auto* elements = reinterpret_cast<const def::model::Cls_140182880*>(mesh.s148);
    const std::int32_t nodeCount = nodes->count();
    const std::int32_t elementCount = elements->count();
    const std::int32_t width = elements->width();
    //14126d296..d305: the true predicate is followed by count/count/width,
    //then a second coordinate/connectivity getter round before the builder.
    //Cached/low profiles retain their folded ported getter. The null-companion
    //high miss instead executes the explicit second round and consumes its values.
    const double* coordinates = nullptr;
    const std::int32_t* connectivity = nullptr;
    if (readiness == GeometryReadiness::RepeatedGetterRequired || readiness == GeometryReadiness::HighCachedGetterRequired) {
        GeometryGetterValues returned;
        if (qualifiedGeometryReadiness(mesh, view, sourceItem, geometry,
            readiness == GeometryReadiness::RepeatedGetterRequired ? GeometryGetterRound::BuilderNullCompanion :
                GeometryGetterRound::BuilderHighCached, &returned) != GeometryReadiness::Populated) return false;
        coordinates = returned.coordinates;
        connectivity = returned.connectivity;
    } else {
        coordinates = def::model::fun_140220440(&mesh);
        connectivity = def::model::fun_140220460(&mesh);
    }
    if (nodeCount <= 0 || elementCount <= 0) return true; // 0x14124bec0's empty contribution
    //14124bec0 consumes packed XY independently of E+f8 metadata. Keep the
    //actual node storage profile, admitting the separate element values 2/3.
    if (nodes->u138 != 2 || (elements->u0f8 != 2 && elements->u0f8 != 3) || width < 0) return false;

    // The original computes element*width and indexes with signed 32-bit loop variables.
    //Width0 skips the native connectivity loop but still contributes all XY nodes.
    if ((width > 0 && elementCount > std::numeric_limits<std::int32_t>::max() / width) ||
        //14124c270 allocates 3N floats; c41c later indexes the same triple stride.
        nodeCount > std::numeric_limits<std::int32_t>::max() / 3 ||
        coordinates == nullptr || connectivity == nullptr) return false;

    const std::int32_t connectivityCount = elementCount * width;
    for (std::int32_t i = 0; i < connectivityCount; ++i) {
        const std::int32_t oneBased = connectivity[i];
        if (oneBased < 1 || oneBased > nodeCount) return false;
    }

    Rect2 meshRect;
    for (std::int32_t i = 0; i < nodeCount; ++i) {
        const double x = coordinates[2 * i];
        const double y = coordinates[2 * i + 1];
        const float xy[2] = {static_cast<float>(x), static_cast<float>(y)};
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(xy[0]) || !std::isfinite(xy[1]))
            return false;
        rectExtend(meshRect, xy); // same double-to-float conversion as 0x14124bec0
    }
    out = meshRect;
    return true;
}

// @src 0x14126cae0 and 0x14126d0c0 (SfDisp2DCrossSection vt+0x48).
// A missing source leaves the previous cached rect alone. With a source the
// original resets it, samples each edge at 0.01, then unions the mesh item.
void refreshGeometryItem(GuiModel& model, View& view, GeometryItem& it)
{
    if (GeometrySyncAccess::pending(model)) return;
    if (view.ownerModel.get() != &model) { it.rectKnown = false; return; }
    if (!it.bound || !view.obj || it.index < 0 ||
        it.index >= def::model::geo_kw::geometry_count(*view.obj)) return;
    auto* geometry = def::model::geo_kw::geometry(*view.obj, it.index);
    if (!geometry) return;
    rectReset(it.rect);
    it.rectKnown = true;
    for (int i = 0; i < static_cast<int>(geometry->edges.size()); ++i)
        it.rect = rectUnion(it.rect, curveItemRect(curveItemPoints(geometry->edge_curve(i), 0.01)));
    // This is the geometry-list mesh, distinct from the object's main FE mesh.
    if (geometry->mesh) {
        const auto* nodes = reinterpret_cast<const def::model::Cls_14017ef20*>(geometry->mesh->s008);
        const auto* elements = reinterpret_cast<const def::model::Cls_140182880*>(geometry->mesh->s148);
        // 0x1400c3570 / 0x1400c3590: an empty side contributes no mesh rectangle.
        if (nodes->count() > 0 && elements->count() > 0) {
            // The source predicate can return false before width/coordinate
            // values or builder layout flags are read. That is a known skip,
            // whereas a missing coordinate/owner profile still stays unknown.
            const auto readiness = qualifiedGeometryReadiness(*geometry->mesh, view, it, geometry);
            if (readiness == GeometryReadiness::Skip) return;
            if (readiness != GeometryReadiness::Populated &&
                readiness != GeometryReadiness::RepeatedGetterRequired &&
                readiness != GeometryReadiness::HighCachedGetterRequired) {
                // Unknown getter effects cannot be replaced by raw rectangle
                // arithmetic. Keep the sampled edge prefix, but refuse it as
                // a complete known contribution when the scene needs it.
                //Reader/loader pending states retain their real prefix effects.
                it.rectKnown = false;
                return;
            }
            Rect2 meshRect;
            if (geometryMeshRect(*geometry->mesh, readiness, view, it, geometry, meshRect))
                it.rect = rectUnion(it.rect, meshRect);
            else it.rectKnown = false; // malformed or outside the recovered 2D builder contract
        }
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
        refreshGeometryItem(m, view, it);
        if (GeometrySyncAccess::pending(m)) return;
    }
}

// @src 0x1411f3560  curve item i of the view, made (empty) when missing
CurveItemState& curveItem(View& view, int i)
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
            CurveItemState& it = curveItem(view, i);
            it.bind(c, 0.01);                            // 0x141267410(item, c, DAT_142203f60)
            view.rect = rectUnion(view.rect, it.rectangle());      // 0x1412510a0, 0x141239830
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
    if (GeometrySyncAccess::pending(m)) return;
    // 1fdd30 consumes the existing Slice collection after actual object/reps
    // rebinding, before viewerSync refresh. A live canonical 2D owner exits
    // before the native adaptation body; canonical 3D with a null database
    // also exits before collection replacement.
    if (view.slice) {
        GuiModel* const owner = view.ownerModel.get();
        if (owner != &m || !owner->step)
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice object-rebind adaptation remains unsupported");
        const int ownerDimension = modelDim(*owner);
        if (ownerDimension != 2 && !(ownerDimension == 3 && owner->db == nullptr))
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice object-rebind adaptation remains unsupported");
    }
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
void recompute3D(GuiModel& m, Viewer& v, View& view)
{
    boxReset(view.box);
    Settings* s = settingsOf(v, &view);
    if (!s) return;
    if (flagOf(*s, 3) && view.obj && objHasGeometry(view.obj)) {      // 0x1411f1c00: 140834d50
        Rep& g = geoRep(view);                                        // 0x141235bd0: same buffers
        view.box = boxUnion(view.box, g.box);
    }
    if (flagOf(*s, 0x2b) && !view.items.empty()) {                    // 0x1411f1c40
        // 1411f5a90 refreshes the entire item list before 1411f58e0 unions
        // its Box3 values. Never treat an unknown partial item rectangle as
        // the complete native item contribution.
        for (auto& item : view.items)
            refreshGeometryItem(m, view, item);
        if (GeometrySyncAccess::pending(m)) return;
        // 1411f58e0 is a separate pass. Validate the complete refreshed list
        // before mutating the view box with any item contribution.
        for (const auto& item : view.items) {
            if (!item.rectKnown)
                throw Error(M2M_E_UNSUPPORTED,
                    "3D viewer: geometry-list item boxes (flag 0x2b, 0x1411f58e0)");
        }
        for (const auto& item : view.items)
            view.box = boxUnion(view.box, rectBox(item.rect));
    }
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
            view.rect = rectUnion(view.rect, curveItem(view, i).rectangle());   // 0x141267530
    }
    if (flagOf(*s, 2) && view.obj && objHasMesh(view.obj))           // 0x1411f1bc0
        view.rect = rectUnion(view.rect, meshItemRect(meshItem(view, 0)));        // 0x14124c3d0
}

// @src 0x140c8e640  refresh: per view box recompute and the model box
void refresh(GuiModel& m)
{
    if (GeometrySyncAccess::pending(m)) return;
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
                    recompute3D(m, v, *view);
                    v.modelBox = boxUnion(v.modelBox, view->box);     // 0x1411f51f0
                    GeometryItem& it = item(*view, 0);                 // creates an unbound empty item if absent
                    refreshGeometryItem(m, *view, it);                 // item vt+0x48
                    if (GeometrySyncAccess::pending(m)) {
                        v.modelRectKnown = false;
                        return; // the suspended item has not returned to its caller
                    }
                    v.modelRect = rectUnion(v.modelRect, it.rect);     // 0x1412510a0, 0x141239830
                    v.modelRectKnown = v.modelRectKnown && it.rectKnown;
                }
            }
        }
    }
}

}  // namespace

void GeometrySyncAccess::attemptScalarTemporary(DB_STEP_DATA& step, LLOBJ_DATA& object,
    const std::shared_ptr<GeometrySyncBarrier>& frame, GeometryMainRecipientProgress& recipient) noexcept
{
    auto* const model = frame ? frame->model.get() : nullptr;
    if (!model) return;
    const auto binding = detail::preparedGeometryTempPrefix(*model);
    if (!binding || !geometryMainScalarCommittedMatches(step, object, frame, recipient)) return;
    auto* nodes = at<def::model::Cls_14017ef20>(&object, 0x8a0);
    using Variables = def::model::CowList<def::model::StateVarBase*>;
    const auto& variables = *reinterpret_cast<const Variables*>(nodes);
    auto* variable = variables.items().front();
    detail::GeometryTempFrameEvidence evidence;
    evidence.model = model; evidence.step = &step; evidence.frame = frame.get(); evidence.object = &object;
    evidence.registry = variables.body(); evidence.receiver = variable; evidence.body = variable->values.body();
    evidence.slot = variable->u30; evidence.ordinal = recipient.ordinal;
    frame->scalarTemporary = binding;
    const bool scalar = binding->report().mode == def::rt::HeapPrefixMode::UnpublishedScalar;
    if (scalar) {
        // Native value_data precedes T, never retroactively after a parked T.
        // Actual refs1/null body makes this real typed accessor a no-detach null return.
        if (variable->value_data() != nullptr ||
            !geometryMainScalarCommittedMatches(step, object, frame, recipient)) return;
        evidence.preTempDataObservedNull = true;
    }
    struct CurrentProof {
        DB_STEP_DATA& step; LLOBJ_DATA& object;
        const std::shared_ptr<GeometrySyncBarrier>& frame; GeometryMainRecipientProgress& recipient;
    } current{step, object, frame, recipient};
    const auto proof = [](const void* context) noexcept {
        const auto& now = *static_cast<const CurrentProof*>(context);
        return geometryMainScalarCommittedMatches(now.step, now.object, now.frame, now.recipient);
    };
    // The callback/context never escape this initial borrow. B cannot be resumed later.
    const auto report = scalar ? binding->attempt(evidence, proof, &current) : binding->attempt(evidence);
    if (report.scalar.stage == def::rt::UnpublishedScalarStage::BeforeBodyPublication)
        recipient.stage = GeometryMainRecipientProgress::Stage::ScalarValueBeforeBodyPublication;
    else if (report.scalar.stage == def::rt::UnpublishedScalarStage::ValueRawNullPending)
        recipient.stage = GeometryMainRecipientProgress::Stage::ScalarValueRawNullPending;
    else if (report.temporary.stage == def::rt::Temp0Stage::RawNullPending)
        recipient.stage = GeometryMainRecipientProgress::Stage::ScalarTempRawNullPending;
    else if (report.temporary.stage == def::rt::Temp0Stage::RawNonNullPending)
        recipient.stage = GeometryMainRecipientProgress::Stage::ScalarTempRawNonNullPending;
}

void viewerSync(GuiModel& m, LLOBJ_DATA* obj, bool flag)
{
    GeometrySyncAccess::requireAvailable(m);
    if (!obj) return;
    Viewer& v = viewerOf(m);
    const int n = m.lastObjectNumber();
    resizeViews(m, v, &n);
    View* view = viewOf(m, v, at<std::int32_t>(obj, 8));
    if (!view) return;
    bind(m, *view, obj);                                              // 0x1411f17f0(v, obj, true)
    if (GeometrySyncAccess::pending(m)) { v.modelRectKnown = false; return; }
    Settings* s = settingsOf(v, view);
    if (s && flag) {
        const bool t = true;
        setDefaultFlags(*s, obj, &t);
    }
    refresh(m);                                                       // 0x140c8e640
}

void viewerRebuild(GuiModel& m, bool a, bool b)
{
    GeometrySyncAccess::requireAvailable(m);
    if (!GuiModelAccess::withStep(m, [](DB_STEP_DATA* nativeStep) {
            return nativeStep != nullptr;
        })) return;                                                  // 0x140c7acd0
    Viewer& v = viewerOf(m);
    if (a) viewerClear(m);                                            // 0x140c82c70
    const int n = m.lastObjectNumber();
    resizeViews(m, v, &n);
    int i = 0;
    for (auto& p : v.views) {
        View* view = p.get();
        ++i;
        LLOBJ_DATA* obj = m.objectAt(i);                              // 0x1408302a0 = 14017aa80
        bind(m, *view, obj);                                          // 0x1411f17f0(v, obj, a)
        if (GeometrySyncAccess::pending(m)) { v.modelRectKnown = false; return; }
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
    GeometrySyncAccess::requireAvailable(m);
    if (!m.viewer) return;
    if (m.viewer->pendingRemoval)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice view removal is pending unrecovered cleanup");
    while (!m.viewer->views.empty()) removeLastView(*m.viewer);
}

namespace detail {

bool importMirrorEntry(GuiModel& model, int objectNo, const std::vector<mst::MirrorPlane>& planes)
{
    Viewer& viewer = viewerOf(model); // includes the geometry-sync barrier
    View* view = viewOf(model, viewer, &objectNo); //140c82d50, including lazy View growth
    if (!view) return false;
    for (std::size_t i = 0; i < planes.size(); ++i) {
        // 140e23a00 first resolves actual View Settings. Missing Settings do not
        // authorize a synthetic false gate; the native flag reader dereferences them.
        Settings* settings = settingsOf(viewer, view);
        if (!settings)
            throw Error(M2M_E_UNSUPPORTED, "CopyMirror plane builder reached missing actual View Settings");
        const bool builderGate = flagOf(*settings, 1) &&
            (flagOf(*settings, 2) || flagOf(*settings, 3));
        if (builderGate)
            importEnabledMirrorPlanePrefix(view->mirrorPlanes, planes[i]);

        // 140e23490 ignores the builder's normal return, then addresses
        // fixed index i rather than the owner count or the parsed a/b/c links.
        // Preserve all preceding writes before refusing the first OOB store.
        if (i >= view->mirrorPlanes.records.size())
            throw Error(M2M_E_UNSUPPORTED, "CopyMirror post-load reached the first out-of-range fixed plane record");
        MirrorPlaneRecord& record = view->mirrorPlanes.records[i];
        const auto& plane = planes[i];
        record.a = plane.a; //14004e810: +0x104
        record.e0 = std::array<double, 3>{static_cast<double>(plane.v[0]),
            static_cast<double>(plane.v[1]), static_cast<double>(plane.v[2])};
        record.c0[0] = static_cast<double>(plane.v[3]);
        record.c0[1] = static_cast<double>(plane.v[4]);
        record.c0[2] = static_cast<double>(plane.v[5]);
        setMirrorPlaneActive(record, true);
        // A disabled builder or capacity return leaves the extent unchanged.
    }
    // The complete first pass precedes any parsed flag application. A refusal
    // above therefore retains the earlier records active, without a finalizer.
    for (std::size_t i = 0; i < planes.size(); ++i)
        setMirrorPlaneActive(view->mirrorPlanes.records[i], planes[i].flag != 0);
    return true;
}

int mirrorPlaneCount(GuiModel& model, int objectNo)
{
    Viewer& viewer = viewerOf(model);
    View* view = viewOf(model, viewer, &objectNo);
    return view ? view->mirrorPlanes.allocatedSlotCount : 0; //141249420
}

std::optional<MirrorPlaneOwnerSnapshot> mirrorPlaneOwnerSnapshot(const GuiModel& model, int objectNo)
{
    GeometrySyncAccess::requireAvailable(model);
    if (objectNo < 1 || !model.viewer ||
        static_cast<std::size_t>(objectNo) > model.viewer->views.size()) return std::nullopt;
    const auto& view = model.viewer->views[static_cast<std::size_t>(objectNo - 1)];
    if (!view) return std::nullopt;
    const auto& owner = view->mirrorPlanes;
    MirrorPlaneOwnerSnapshot snapshot;
    snapshot.viewIdentity = view.get();
    snapshot.allocatedSlotCount = owner.allocatedSlotCount;
    snapshot.links = owner.links;
    for (std::size_t i = 0; i < owner.records.size(); ++i) {
        const auto& record = owner.records[i];
        auto& target = snapshot.records[i];
        target.matrixDouble = record.matrixDouble;
        target.matrixFloat = record.matrixFloat;
        target.mode = record.mode;
        target.active = record.active;
        target.c0 = record.c0;
        target.e0 = record.e0;
        target.a = record.a;
        target.links = record.links;
    }
    return snapshot;
}

} // namespace detail

int viewerItemCount(const GuiModel& m)
{
    return m.viewer ? static_cast<int>(m.viewer->views.size()) : 0;
}

void finalizeStepDataPropertyTable(GuiModel& model, doe::PropertyTableState& table)
{
    GeometrySyncAccess::requireAvailable(model);
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
    GuiModelAccess::withStep(model, [&](DB_STEP_DATA* nativeStep) {
        if (nativeStep) (void)def::pre::step_current_object(nativeStep, true);
    });
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
    GeometrySyncAccess::requireAvailable(model);
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
    GeometrySyncAccess::requireAvailable(model);
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

// @src 0x140c8f830: independent fit-radius contribution, not the scene box.
bool viewerAllViewBox(GuiModel& m, bool& valid, float mn[3], float mx[3])
{
    Viewer& v = viewerOf(m);
    Box3 box;
    if (modelDim(m) == 2) {
        Rect2 rect;
        for (auto& p : v.views) {
            View* view = p.get();
            if (!view || !settingsOf(v, view)) continue;
            recompute2D(v, *view);
            rect = rectUnion(rect, view->rect);
            if (m.display && displayAxisOn(*m.display)) {
                const int geometry = simGeometryType(m);
                if (geometry == 1 || geometry == 3) {
                    // Native reads model+108, not this local all-view union.
                    Rect2 axis = v.modelRect;
                    axis.mn[0] = 0.0;
                    axis.valid = true;
                    rect = rectUnion(rect, axis);
                }
            }
        }
        box = rectBox(rect);
    } else if (modelDim(m) == 3) {
        for (auto& p : v.views) {
            if (p && settingsOf(v, p.get())) box = boxUnion(box, p->box);
        }
    }
    valid = box.valid;
    for (int c = 0; c < 3; ++c) { mn[c] = box.mn[c]; mx[c] = box.mx[c]; }
    return true;
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
