// The main window's 3D display and its camera, as far as File > Save writes them into the .moproj
// (SfDispView, 0x140cd57b0 -> 0x140d05a10 / 0x140d06610). Packet moproj; spec
// docs/re/moproj2mst/moproj/dispview.md. The scene boxes come from packet V (viewer.cpp,
// viewerSceneBox).
//
// Profile of a fresh GUI (inputs of the original that come from the machine, not the project):
//   * GL widget interior 3363 x 1709 px (Oracle B, coordinator 2026-10-22: maximised main window
//     on Sim2, measured from full-resolution screenshots after Open);
//   * QSettings "displayOption/dispUpwardAxis" unset (-1), which the iso view of every fresh 3D
//     save in Oracle B confirms.
#include <cmath>
#include <cstdint>
#include <memory>

#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/display_owner.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {

namespace {

constexpr int kGlWidth = 3363;
constexpr int kGlHeight = 1709;
constexpr int kDispUpwardAxis = -1;

// Qt 4.8 QVector3D: three floats
struct V3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;
};

V3 v3(double x, double y, double z)
{
    V3 v;
    v.x = static_cast<float>(x);
    v.y = static_cast<float>(y);
    v.z = static_cast<float>(z);
    return v;
}

// Qt 4.8 QQuaternion: qreal wp, xp, yp, zp
struct Quat {
    double w = 1.0, x = 0.0, y = 0.0, z = 0.0;
};

// Qt 4.8 QQuaternion::normalized()
Quat normalized(const Quat& q)
{
    const double len = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (std::fabs(len - 1.0f) <= 0.00001) return q;   // qFuzzyIsNull(len - 1.0f)
    if (std::fabs(len) <= 0.00001) return Quat{0.0, 0.0, 0.0, 0.0};
    const double l = std::sqrt(len);
    return Quat{q.w / l, q.x / l, q.y / l, q.z / l};
}

// Qt 4.8 QQuaternion::fromAxisAndAngle(qreal x, y, z, angle)
Quat fromAxisAndAngle(double x, double y, double z, double angle)
{
    const double length = std::sqrt(x * x + y * y + z * z);
    if (!(std::fabs(length - 1.0f) <= 0.00001) && !(std::fabs(length) <= 0.00001)) {
        x /= length;
        y /= length;
        z /= length;
    }
    const double a = (angle / 2.0f) * 3.14159265358979323846 / 180.0f;
    const double s = std::sin(a);
    const double c = std::cos(a);
    return normalized(Quat{c, x * s, y * s, z * s});
}

// @src 0x140d6a1e0  q1 * q2 (Qt 4.8 operator*)
Quat mul(const Quat& q1, const Quat& q2)
{
    const double ww = (q1.z + q1.x) * (q2.x + q2.y);
    const double yy = (q1.w - q1.y) * (q2.w + q2.z);
    const double zz = (q1.w + q1.y) * (q2.w - q2.z);
    const double xx = ww + yy + zz;
    const double qq = 0.5 * (xx + (q1.z - q1.x) * (q2.x - q2.y));
    return Quat{(qq - ww) + (q1.z - q1.y) * (q2.y - q2.z), (qq - xx) + (q1.x + q1.w) * (q2.x + q2.w),
                (qq - yy) + (q1.w - q1.x) * (q2.y + q2.z), (qq - zz) + (q1.z + q1.y) * (q2.w - q2.x)};
}

// Box3 with the derived fields of 0x141219af0 (spec V 1.1)
struct BoxD {
    bool valid = false;
    V3 mn, mx;
    V3 centre;                     // +0x60
    double radius = 0.0;           // +0x58
    double margin = 0.0;           // +0x88
    double vol[6] = {};            // +0x90 .. +0xb8
};

// @src 0x141219af0  centre, radius and view volume from min / max
void derive(BoxD& b)
{
    // 0x14121c260: double sums of the widened floats, stored as floats; 0x14121c340: / 2.0
    const V3 s = v3(static_cast<double>(b.mn.x) + b.mx.x, static_cast<double>(b.mn.y) + b.mx.y,
                    static_cast<double>(b.mn.z) + b.mx.z);
    b.centre = v3(static_cast<double>(s.x) / 2.0, static_cast<double>(s.y) / 2.0, static_cast<double>(s.z) / 2.0);
    // 0x14121c200: (max - centre) as floats (0x1411ed4d0), QVector3D::length(): float sum, qSqrt
    const V3 d = v3(static_cast<double>(b.mx.x) - b.centre.x, static_cast<double>(b.mx.y) - b.centre.y,
                    static_cast<double>(b.mx.z) - b.centre.z);
    const float sq = d.x * d.x + d.y * d.y + d.z * d.z;
    b.radius = std::sqrt(static_cast<double>(sq));
    b.margin = b.radius * 0.05;                                        // DAT_1421ec2e0
    const double cx = b.centre.x, cy = b.centre.y, cz = b.centre.z;
    b.vol[0] = (cx - b.radius) - b.margin;
    b.vol[1] = cx + b.radius + b.margin;
    b.vol[2] = (cy - b.radius) - b.margin;
    b.vol[3] = cy + b.radius + b.margin;
    b.vol[4] = (cz - b.radius * 3.0) - b.margin * 10.0;
    b.vol[5] = cz + b.radius * 3.0 + b.margin * 10.0;
}

BoxD boxOf(bool valid, const float mn[3], const float mx[3])
{
    BoxD b;
    if (!valid) return b;                                              // 0x1412196f0: all zero
    b.valid = true;
    b.mn = V3{mn[0], mn[1], mn[2]};
    b.mx = V3{mx[0], mx[1], mx[2]};
    derive(b);                                                         // 0x141219dd0
    return b;
}

// @src 0x14121a9f0  union (spec V 1.1): an invalid side gives the other
BoxD boxUnion(const BoxD& a, const BoxD& o)
{
    if (!a.valid) return o;
    if (!o.valid) return a;
    BoxD r;
    r.valid = true;
    // 0x14121bf20 minV / 0x14121c090 maxV (ties take the second)
    r.mn = V3{o.mn.x > a.mn.x ? a.mn.x : o.mn.x, o.mn.y > a.mn.y ? a.mn.y : o.mn.y, o.mn.z > a.mn.z ? a.mn.z : o.mn.z};
    r.mx = V3{a.mx.x > o.mx.x ? a.mx.x : o.mx.x, a.mx.y > o.mx.y ? a.mx.y : o.mx.y, a.mx.z > o.mx.z ? a.mx.z : o.mx.z};
    derive(r);
    return r;
}

}  // namespace

// SfGLDisplay (0xbc8, ctor 0x140cf8b00) and its camera (display+0x238, ctor 0x140d66d90)
struct Display {
    Display() = default;
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    // the operations' models outlive the display in the conversion; their +0x10 must not dangle
    ~Display();

    bool axis = true;              // +0x202 axis display (ctor 0x140cf8e64)
    GuiModelGuard main;            // +0x1a0 QPointer: the model shown (0x140cf9360)
    GuiModelGuard selectedOverride; // +0x1e0; constructor null, distinct from main/shown
    bool mainShown = true;         // +0x1a8 (0x140cf9730)
    DisplayModels models;          // +0x1b8, +0x1c8, +0x1d8, +0x204 (packet V, MDT L4)

    // ---- camera ----
    int width = 0;                // storage only until geometry source is bound
    int height = 0;
    DisplayGeometryProfile::Source geometrySource = DisplayGeometryProfile::Source::Unresolved;
    int projection = 0;            // +0x08 0 orthographic, 2 frustum
    double ortho[6] = {};          // +0x10 left, right, bottom, top, width, height
    Quat q;                        // +0x180 rotation (ctor: identity)
    V3 eye;                        // +0x1a0
    V3 view{0.0f, 0.0f, -1.0f};    // +0x1ac
    V3 up{0.0f, 1.0f, 0.0f};       // +0x1b8
    double zoom = 1.0;             // +0x1c8
    double rectMin[2] = {}, rectMax[2] = {}, rectCentre[2] = {};   // +600 Rect2 window
    bool rectFixed = false;        // +0x3a8
    BoxD box;                      // +0x298 scene box of the last fit
    double boxRadius = 0.0;        // +0x380
    double eyeBase[2] = {};        // +0x388
    double pan[2] = {};            // +0x398
    double lrtb[4] = {};           // +0x3b0 left, right, top, bottom
    double origin[3] = {};         // +0x3d0
};

namespace {

// @src 0x141239060 (part)  the window rect's centre after each setter (0x141239390 ...)
void rectDerive(Display& d)
{
    d.rectCentre[0] = (d.rectMin[0] + d.rectMax[0]) / 2.0;            // 0x1411ed5b0 / 0x1411ed6e0
    d.rectCentre[1] = (d.rectMin[1] + d.rectMax[1]) / 2.0;
}

// @src 0x140d67080  projection: the orthographic box from the window and the aspect
void projection(Display& d)
{
    const double a = static_cast<double>(d.width) / static_cast<double>(d.height);   // 0x140d67030
    if (d.projection == 1) return;
    if (d.projection == 2)
        throw Error(M2M_E_UNSUPPORTED, "3D display: perspective projection (glFrustum, 0x140d67080)");
    if (a < 1.0) {
        d.ortho[0] = d.rectMin[0] * d.zoom;
        d.ortho[1] = d.rectMax[0] * d.zoom;
        d.ortho[2] = (d.rectMin[1] * d.zoom) / a;
        d.ortho[3] = (d.rectMax[1] * d.zoom) / a;
    } else {
        d.ortho[0] = d.rectMin[0] * a * d.zoom;
        d.ortho[1] = d.rectMax[0] * a * d.zoom;
        d.ortho[2] = d.rectMin[1] * d.zoom;
        d.ortho[3] = d.rectMax[1] * d.zoom;
    }
    d.ortho[4] = d.ortho[1] - d.ortho[0];
    d.ortho[5] = d.ortho[3] - d.ortho[2];
    d.lrtb[0] = d.rectMin[0];                                          // 0x141239490
    d.lrtb[1] = d.rectMax[0];                                          // 0x1412394c0
    d.lrtb[2] = d.rectMax[1];                                          // 0x141239520
    d.lrtb[3] = d.rectMin[1];                                          // 0x1412394f0
    // glOrtho(..): display only
}

// @src 0x140d68ee0  window point -> scene; false when the widget or the projection is empty
bool unproject(const Display& d, const double* p, double* out)
{
    if (d.width < 1 || d.height < 1) return false;                     // 0x140d68190
    if (d.ortho[4] <= 0.0 || d.ortho[5] <= 0.0) return false;
    out[0] = d.ortho[0] + (d.ortho[4] * p[0]) / static_cast<double>(d.width);
    out[1] = d.ortho[3] - (d.ortho[5] * p[1]) / static_cast<double>(d.height);
    return true;
}

// @src 0x140d69820  eye: the window centre minus the scene point at the widget centre
void placeEye(Display& d)
{
    if (d.rectFixed) return;
    projection(d);                                                     // 0x140d67080(cam, 1)
    const double* c = d.rectCentre;                                    // 0x141239650
    const double pt[2] = {static_cast<double>(d.width) / 2.0, static_cast<double>(d.height) / 2.0};
    double s[2] = {0.0, 0.0};                                          // QPointF()
    unproject(d, pt, s);
    d.eyeBase[0] = c[0] - s[0];
    d.eyeBase[1] = c[1] - s[1];
    d.eye.x = static_cast<float>(d.eyeBase[0] + d.pan[0]);
    d.eye.y = static_cast<float>(d.eyeBase[1] + d.pan[1]);
}

// @src 0x140d69740  view reset: zoom 1, pan 0, eye, view (0, 0, -1), up (0, 1, 0)
void resetView(Display& d)
{
    d.zoom = 1.0;                                                      // +0x1c8 (+0x40 = 0, +0x218 = 1)
    d.pan[0] = 0.0;
    d.pan[1] = 0.0;
    d.rectFixed = false;
    placeEye(d);
    d.view = V3{0.0f, 0.0f, -1.0f};                                    // 0x140d69430
    d.up = V3{0.0f, 1.0f, 0.0f};                                       // 0x140d695b0
}

// @src 0x140cfd750(d, out, refresh = false)  union of the shown models' scene boxes
BoxD sceneBox(Display& d)
{
    BoxD b;                                                            // 0x1412195a0
    if (d.main.get() && d.mainShown) {
        bool valid = false;
        float mn[3], mx[3];
        viewerSceneBox(*d.main.get(), valid, mn, mx);                  // 0x140c8e0f0
        b = boxUnion(b, boxOf(valid, mn, mx));
    }
    // +0x1b8 / +0x1c8 (second and third model) and the +0x1d8 item list: not set on this path
    return b;
}

// @src 0x140cfe9c0  fit (display +0x708 == 0): refresh the shown models, then the box, window,
// view reset and origin
void fit(Display& d)
{
    if (d.main.get() && d.mainShown) viewerRefresh(*d.main.get());     // 0x140c8e640
    // 0x140cfe0f0: the box radius through 0x140cfdc60 (0x140c8f830 per model); taken here from the
    // same scene box (0x140c8f830 not traced; it only gates the store below at radius >= 0.001)
    const BoxD box = sceneBox(d);
    const double radius = box.radius;
    // @src 0x140d69e70  the box stored when valid and not degenerate
    if (box.valid && 0.001 <= radius && 0.001 <= box.vol[1] - box.vol[0] && 0.001 <= box.vol[3] - box.vol[2]) {
        d.box = box;
        d.boxRadius = radius;
    }
    // @src 0x140d69d60  the window from the box's view volume
    if (box.valid) {
        d.rectMin[0] = box.vol[0];
        rectDerive(d);
        d.rectMax[0] = box.vol[1];
        rectDerive(d);
        d.rectMin[1] = box.vol[2];
        rectDerive(d);
        d.rectMax[1] = box.vol[3];
        rectDerive(d);
    }
    resetView(d);                                                      // 0x140d69740
    // @src 0x140d67470  origin = -centre of the stored box (0x14121a3c0: centre * -1.0, floats)
    const V3 o = v3(static_cast<double>(d.box.centre.x) * -1.0, static_cast<double>(d.box.centre.y) * -1.0,
                    static_cast<double>(d.box.centre.z) * -1.0);
    d.origin[0] = o.x;
    d.origin[1] = o.y;
    d.origin[2] = o.z;
}

// dimension of the shown model (0x140cfaec0: 3 without one)
int shownDim(const Display& d)
{
    return d.main.get() ? guiModelDim(*d.main.get()) : 3;
}

}  // namespace

bool displayAxisOn(const Display& d)
{
    return d.axis;
}

Display::~Display()
{
    for (GuiModel* m : {main.get(), models.second.get(), models.third.get()})
        if (m && m->display == this) m->display = nullptr;
    for (DisplayModels::Named& n : models.named)
        if (n.model) n.model->display = nullptr;
}

GuiModel* displayMain(const Display& d)
{
    return d.main.get();
}

GuiModel* displaySelectedModel(const Display& d)
{
    if (auto* selected = d.selectedOverride.get()) return selected;
    return d.main.get();
}

GuiModel* selectDisplayModel(Display& d, GuiModel* requested)
{
    GuiModel* previous = displaySelectedModel(d);
    d.selectedOverride.reset();
    // The three selection helpers below are pure flag stores; no callbacks
    // mutate the named holder collection while traversing its key order.
    for (auto& named : d.models.named) {
        if (named.model.get() != requested) continue;
        named.flag = true; // d122c0, holder+8
        d.selectedOverride.reset(named.model.get());
        return previous;
    }
    if (d.models.third.get() == requested) {
        d.models.thirdShown = true; // cf9f50, Display+1d0
        d.selectedOverride = d.models.third;
    } else if (d.main.get() == requested) {
        d.mainShown = true; // cf96f0, before guarded pointer assignment
        d.selectedOverride = d.main;
    } else if (d.models.second.get() == requested) {
        d.models.secondShown = true; // cf9c70, Display+1c0
        d.selectedOverride = d.models.second;
    }
    return previous;
}

bool displayMainShown(const Display& d)
{
    return d.mainShown;
}

DisplayModels& displayModels(Display& d)
{
    return d.models;
}

void displayForget(Display& d, const GuiModel* m)
{
    if (d.main.get() == m) d.main.reset();
    if (d.models.second.get() == m) d.models.second.reset();
    if (d.models.third.get() == m) d.models.third.reset();
}

std::shared_ptr<Display> createSceneDisplayOwner()
{
    return std::make_shared<Display>();
}

DisplayGeometryProfile displayGeometryProfile(const Display& display) noexcept
{
    DisplayGeometryProfile profile;
    profile.source = display.geometrySource;
    if (profile.source != DisplayGeometryProfile::Source::Unresolved)
        profile.pixels = std::array{display.width, display.height};
    return profile;
}

Display& projectDisplay(Project& p)
{
    // Full lifecycle already aliases the session's actual primary scene. The
    // unbound legacy fixture adapter still creates a display on first use.
    if (!p.display) p.display = createSceneDisplayOwner();
    // the Operation Editor dock below the GL widget: when the Setup Type dialog's Yes leaves the
    // diagram view 175 px high instead of 123, the GL widget is 52 px lower (Oracle B 124: EyeVector
    // (-2^-41, -2^-42) is placeEye's residue at 3363 x 1657 and 0 at 3363 x 1709)
    p.display->width = kGlWidth;
    p.display->height = kGlHeight - (p.diagramViewHeight - 123);
    p.display->geometrySource = DisplayGeometryProfile::Source::HistoricalOracleB;
    return *p.display;
}

void displayShow(Project& p, GuiModel* m)
{
    Display& d = projectDisplay(p);
    // 0x140d12a30(+0x1d8): a named model, the second or the third model is shown as it is
    for (const DisplayModels::Named& n : d.models.named)
        if (n.model.get() == m) return;                                // 0x140d12200
    if (m == d.models.second.get() || m == d.models.third.get()) return; // 0x140cf9960, 0x140cf9d20
    if (d.main.get() == m) return;                                     // QPointer change check 0x140cf86f0
    d.main.reset(m);
    if (m) m->display = &d;                                            // 0x140c7ade0; display+0x204 = 0
    // 0x140cf9750: second / third model clean-up (none)
}

void displayRefreshScene(Project& p, GuiModel* m)
{
    displayShow(p, m);                                                 // 0x140ccdd10(m, 0)
    fit(projectDisplay(p));                                            // "viewFit": 0x140cfe9c0
}

void displayResetView(Project& p)
{
    Display& d = projectDisplay(p);
    if (shownDim(d) != 3) {
        // @src 0x140d011d0 (+0x708 == 0)  2D: rotation reset (0x140d691c0: QQuaternion())
        d.q = Quat{};
    } else {
        // @src 0x140cff570 (+0x708 == 0, model not 2D)  iso view: reset, then rotations by the
        // upward-axis preference (0x140d67ff0: q = fromAxisAndAngle(axis, angle) * q)
        d.q = Quat{};                                                  // 0x140d691c0
        auto rot = [&](double x, double y, double z, float deg) {
            d.q = mul(fromAxisAndAngle(x, y, z, static_cast<double>(deg)), d.q);
        };
        switch (kDispUpwardAxis) {
        case 0: rot(0, 1, 0, -90.0f); break;
        case 1: rot(0, 1, 0, 90.0f); break;
        case 2:
            rot(0, 1, 0, 90.0f);
            rot(1, 0, 0, 90.0f);
            break;
        case 3: rot(1, 0, 0, -90.0f); break;
        case 5: rot(1, 0, 0, 180.0f); break;
        default: break;
        }
        rot(1, 0, 0, -90.0f);                                          // DAT_1427e2950
        rot(0, 1, 0, -90.0f);
        rot(0, 1, 0, -45.0f);                                          // DAT_1429f916c
        rot(1, 0, 0, 30.0f);                                           // DAT_1429f9168
    }
    fit(d);                                                            // 0x140cfe9c0
}

CameraState displayCamera(Project& p)
{
    Display& d = projectDisplay(p);
    CameraState c;
    // @src 0x140d69060  the rotation; the spin step (angular speed +0x17c times the time since
    // +0x240) is zero: no spin is started on this path
    const Quat q = mul(fromAxisAndAngle(0.0, 1.0, 0.0, 0.0), d.q);
    // QQuaternion::toVector4D: floats (x, y, z, w)
    c.quat[0] = static_cast<float>(q.x);
    c.quat[1] = static_cast<float>(q.y);
    c.quat[2] = static_cast<float>(q.z);
    c.quat[3] = static_cast<float>(q.w);
    c.eye[0] = d.eye.x;
    c.eye[1] = d.eye.y;
    c.eye[2] = d.eye.z;
    c.view[0] = d.view.x;
    c.view[1] = d.view.y;
    c.view[2] = d.view.z;
    c.up[0] = d.up.x;
    c.up[1] = d.up.y;
    c.up[2] = d.up.z;
    c.zoom = d.zoom;
    c.centerX = d.pan[0];
    c.centerY = d.pan[1];
    for (int i = 0; i < 4; ++i) c.lrtb[i] = d.lrtb[i];
    for (int i = 0; i < 3; ++i) c.origin[i] = d.origin[i];
    // 0x140d0c920: the extra first child needs a second model (+0x1b8), never set here
    return c;
}

}  // namespace m2m::engine
