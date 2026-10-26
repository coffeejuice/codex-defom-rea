// The .moproj writer of File > Save, step 3 (packet moproj; docs/re/moproj2mst/moproj/spec.md).
// One function per binary function, in the original's call order. Decompiles:
// gui_mo/out_scope/save1.c (0x1400983c0), save2.c (0x14009cfd0 .. 0x1400a88a0) and
// gui_mo/s1/moproj/w1..w3.c.
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>

#include "m2m/blob.hpp"
#include "m2m/engine.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/objects.hpp"
#include "m2m/qt4path.hpp"
#include "write_internal.hpp"

namespace m2m::moproj {

namespace {

// @src 0x14009aaa0
NodePtr rectElement(const RectF& r)
{
    NodePtr e = element("RectF");
    append(*e, textElement("x", qt::number(r.x, 'g', 6)));
    append(*e, textElement("y", qt::number(r.y, 'g', 6)));
    append(*e, textElement("height", qt::number(r.h, 'g', 6)));
    append(*e, textElement("width", qt::number(r.w, 'g', 6)));
    return e;
}

// @src 0x14009b710  <DiagramItem> of item i of the sub-project's flow chart
NodePtr diagramItem(SubProject& sub, DiagramViews& views, int i)
{
    const FlowChart& flow = sub.flow;
    const FlowItem& it = *flow.items[static_cast<std::size_t>(i)];
    NodePtr e = element("DiagramItem");
    attr(*e, "Operation_Id", qt::number(it.opId));                  // vt+0x28
    attr(*e, "Expanded", views.expanded(sub, i) ? L("yes") : L("no"));   // vt+0xa8
    append(*e, textElement("ClassName", it.className));            // model data vt+0x38
    append(*e, textElement("ItemName", it.itemName));              // vt+0x18
    append(*e, rectElement(views.rect(sub, i)));                   // vt+0x98, 0x14009aaa0
    if (it.className != L("SfOprCycle")) {
        NodePtr c = element("CellPos");                            // 0x14009b180 (vt+0xc8)
        attr(*c, "Row", qt::number(it.row));
        attr(*c, "Col", qt::number(it.col));
        append(*e, c);
    } else {
        NodePtr g = element("GroupInfo");                          // 0x14009b320 (vt+0xd8, vt+0xe8)
        attr(*g, "BeginRow", qt::number(it.beginRow));
        attr(*g, "BeginCol", qt::number(it.beginCol));
        attr(*g, "EndRow", qt::number(it.endRow));
        attr(*g, "EndCol", qt::number(it.endCol));
        append(*e, g);
    }
    if (!it.itemData.empty()) {                                    // vt+0x100 QMap<QString, int>
        NodePtr d = element("ItemData");                           // 0x14009b5e0, key order
        std::vector<std::pair<String, int>> m = it.itemData;
        std::stable_sort(m.begin(), m.end(), [](const auto& a, const auto& b) { return a.first.u16() < b.first.u16(); });
        for (const auto& kv : m) xml::setAttribute(*d, kv.first, qt::number(kv.second));
        append(*e, d);
    }
    // one connector per item (vt+0x48) holding the connection to the next item (vt+0x38, vt+0x20)
    NodePtr cs = element("Connectors");
    attr(*cs, "Number", qt::number(1));
    NodePtr cn = element("Connections");
    int nc = it.next ? 1 : 0;
    attr(*cn, "Number", qt::number(nc));
    if (it.next) {
        int idx = -1;
        for (std::size_t k = 0; k < flow.items.size(); ++k)
            if (flow.items[k].get() == it.next) {
                idx = static_cast<int>(k);
                break;
            }
        append(*cn, textElement("ItemIndex", qt::number(idx)));
    }
    append(*cs, cn);
    append(*e, cs);
    const std::vector<ExpansionItem> ex = views.expansion(sub, i);   // vt+0x50
    NodePtr xs = element("Expansion_Items");
    attr(*xs, "Number", qt::number(static_cast<int>(ex.size())));
    for (const ExpansionItem& x : ex) {
        NodePtr xi = element("Expansion_Item");
        if (x.type == 0) {
            attr(*xi, "Type", L("Text"));
            attr(*xi, "Label", x.label);
        } else if (x.type == 1) {
            attr(*xi, "Type", L("Line"));
            const char* st = x.lineStyle == 0 ? "NoLine" : x.lineStyle == 2 ? "Dash" : x.lineStyle == 3 ? "Dot"
                             : x.lineStyle == 4 ? "DashDot" : x.lineStyle == 5 ? "DashDotDot" : "Solid";
            attr(*xi, "LineStyle", L(st));
            const String c = qt::arg(qt::arg(qt::arg(qt::arg(L("%1, %2, %3, %4"), x.r), x.g), x.b), x.a);
            attr(*xi, "LineColor", c);
        } else if (x.type == 2) {
            attr(*xi, "Type", L("Space"));
        } else {
            attr(*xi, "Type", L("Unknown"));
        }
        append(*xs, xi);
    }
    append(*e, xs);
    return e;
}

// @src 0x140099040  the active Sub_Project element (name "" from 0x1400983c0, so no Name here)
NodePtr subProjectElement(Project& p, SubProject& sub, DiagramViews& views)
{
    NodePtr e = element("Sub_Project");
    {
        // @src 0x14009a640
        double x = 0, y = 0;
        views.viewCentre(sub, x, y);
        NodePtr vt = element("ViewTransform");
        append(*vt, textElement("CenterX", qt::number(x, 'g', 6)));
        append(*vt, textElement("CenterY", qt::number(y, 'g', 6)));
        append(*e, vt);
    }
    append(*e, views.dispView(sub));                              // 0x140cd57b0
    const int n = static_cast<int>(sub.flow.items.size());
    attr(*e, "Number", qt::number(n));
    attr(*e, "Active", qt::number(sub.flow.activeItem));          // index of the view's current item, -1
    attr(*e, "Type", qt::number(sub.type));
    const int id = subProjectId(p, p.activeSub);                  // 0x1400a7760
    if (id > -1) attrInt(*e, "Id", id);
    for (int i = 0; i < n; ++i) append(*e, diagramItem(sub, views, i));
    if (sub.type == 2 || sub.type == 3)
        throw Error(M2M_E_UNSUPPORTED, "project file: DOE / optimisation study elements (0x1400d29d0, 0x140907a10)");
    // Subproject_DbName (0x1400b9e80: own DB name for type 4, else 0x1400b9df0)
    if (sub.type == 4) throw Error(M2M_E_UNSUPPORTED, "project file: Subproject_DbName of a die-stress sub-project (0x1400bbee0)");
    NodePtr db = element("Subproject_DbName");
    // 0x1400b9df0 -> 0x1400a4f60(P, 0): changeSuffix(completeBaseName(P+0x18) (0x14008a500), ".DB")
    attr(*db, "DbName", sub.project ? app::changeSuffix(projectBaseName(*sub.project), L(".DB")) : String());
    append(*e, db);
    return e;
}

std::string readAll(const std::filesystem::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

}  // namespace

namespace {

// SfDiagCellEditor geometry (ctor 0x14014dc10): cell width +0x00, row height +0x04, origin +0x08
// (0x14014e7b0 resets it to 0, 0), columns +0x28
constexpr int kCellW = 205, kRowH = 185, kColumns = 7;
constexpr double kOriginX = 0, kOriginY = 0;

// Fresh-GUI profile (inputs of the original that come from the machine, not the project):
// QFontMetrics::height() of QApplication::font()'s family at 8 pt, and the height of the item
// icon's QIcon::pixmap(30, 30). 13 = MS Shell Dlg 2 / Tahoma 8 pt at 96 DPI (GDI tmHeight measured
// on Sim2, coordinator 2026-10-22; DEF_GUI_MO_64 has no dpiAware entry, so 96 DPI is inferred);
// 30 = the requested size, an upper bound. Both reach the result only when larger than 24 / 48.
constexpr int kFontHeight = 13, kIconHeight = 30;

// @src 0x14016cab0  height of the graphics item's base rect (0, 0, 180, H) with a model item
double baseHeight()
{
    const double th = kFontHeight, oh = kFontHeight;      // "_<name>__" and "_Operation XXX__"
    const double icon = kIconHeight;
    // one input and one output port (vt+0x40, vt+0x48 return one entry each): n * 8 + (n - 1) * 4
    const double inH = 1 * 8.0 + 0 * 4.0, outH = 1 * 8.0 + 0 * 4.0;
    double v = th + oh;
    if (v <= icon) v = icon;
    if (v < inH) v = inH;
    if (v < outH) v = outH;
    v = v + 8.0 + 4.0;
    if (60.0 > v) v = 60.0;
    return std::ceil(v / 30.0) * 30.0;                    // 0x141470dd4 ceil
}

// @src 0x1401637b0  QGraphicsRectItem::rect().height() of item k after open: the expanded flag
// (+0x60) is 1 from the item's creation (0x140162340) and only the user toggles it (0x14017a3c0,
// paste 0x140173170); 0x14016d900 sets rect.adjusted(-15, -15, 15, 15)
double itemRectHeight(const SubProject& sub, int k, DiagramViews& views)
{
    const FlowItem& c = *sub.flow.items[static_cast<std::size_t>(k)];
    const double H = baseHeight();
    double h = H + 200.0;                                 // P+0x20 = base, height + 200
    if (c.opId < 0) {
        h = 160.0;                                        // data +0x110 (Operation_Id < 0)
    } else {
        // @src 0x140163e50  expansion rects from int(5 + bottom(base.adjusted(0, 0, -5, -5)))
        const std::vector<ExpansionItem> ex = views.expansion(sub, k);
        if (!ex.empty()) {
            int iy = static_cast<int>(5.0 + (H - 5.0));
            double bottom = 0;
            for (std::size_t j = 0; j < ex.size(); ++j) {
                const double eh = ex[j].type == 1 ? 3.0 : 15.0;   // Line 3, Text / Space / other 15
                const double b = static_cast<double>(iy) + eh;
                if (j == 0 || bottom < b) bottom = b;
                iy += ex[j].type == 1 ? 3 : 0xf;
            }
            h = (bottom - 0.0) + 30.0;                    // - top of the base rect, + 30
        }
    }
    return h + 30.0;
}

// 0x1401574f0: a group marks the cells from its begin to its end cell (row-major over the
// editor's columns) with its connector (cell +0x08)
bool rowHasConnector(const SubProject& sub, int row)
{
    for (const auto& g : sub.flow.items) {
        if (!g->group) continue;
        const int b = g->beginRow * kColumns + g->beginCol, e = g->endRow * kColumns + g->endCol;
        if (g->beginRow < 0 || g->beginCol < 0 || g->endRow < 0 || g->endCol < 0) continue;
        if (b <= e && b < (row + 1) * kColumns && e >= row * kColumns) return true;
    }
    return false;
}

// @src 0x140157fe0  y and height of row `last` of the row list (0x14014e1e0, 0x140159d20)
void rowGeometry(const SubProject& sub, DiagramViews& views, int last, double& rowY, double& rowH)
{
    double y = kOriginY;
    for (int row = 0;; ++row) {
        int iy = static_cast<int>(y);
        double h = static_cast<double>(kRowH);
        const bool cycle = rowHasConnector(sub, row);
        for (std::size_t k = 0; k < sub.flow.items.size(); ++k) {
            const FlowItem& c = *sub.flow.items[k];
            if (c.group || c.row != row) continue;
            const double v = itemRectHeight(sub, static_cast<int>(k), views) + 25.0;
            if (h < v) h = v;
        }
        if (cycle) iy += 0x14;
        if (row == last) {
            rowY = static_cast<double>(iy);
            rowH = h;
            return;
        }
        y = static_cast<double>(iy) + h;
    }
}

}  // namespace

bool savedExpanded(const SubProject& sub, int i)
{
    const FlowItem& it = *sub.flow.items[static_cast<std::size_t>(i)];
    // @src 0x1401712a0  the editor's item list copies each graphics item's expanded flag (+0x60,
    // 1 after open) into the model flag (vt+0xa0) for every item with a cell graphics item
    if (!it.group) return true;
    // A group item has no cell graphics item and keeps the flag the SfDiagItem ctor 0x14015ef80
    // leaves uninitialised (+0x80, heap memory); the original's value is not reproducible.
    // DEVIATION (Aleksandr 2026-10-22, option b): always "yes".
    return true;
}

RectF sceneRect(const SubProject& sub, int i, DiagramViews& views)
{
    const FlowItem& it = *sub.flow.items[static_cast<std::size_t>(i)];
    RectF r{it.rect[0], it.rect[1], it.rect[2], it.rect[3]};
    if (it.group) return r;   // a group has no cell; 0x1401583e0 moves only cell items
    // @src 0x140157fe0  row list (y, height) from the origin; @src 0x140159ab0 row y
    double y = kOriginY;
    double rowY = 0;
    for (int row = 0;; ++row) {
        int iy = static_cast<int>(y);
        rowY = static_cast<double>(iy);
        double h = static_cast<double>(kRowH);
        const bool cycle = rowHasConnector(sub, row);
        if (row == it.row) {
            if (cycle) rowY += 20.0;
            break;
        }
        for (std::size_t k = 0; k < sub.flow.items.size(); ++k) {
            const FlowItem& c = *sub.flow.items[k];
            if (c.group || c.row != row) continue;
            const double v = itemRectHeight(sub, static_cast<int>(k), views) + 25.0;
            if (h < v) h = v;
        }
        if (cycle) iy += 0x14;
        y = static_cast<double>(iy) + h;
    }
    // @src 0x14014e2e0 column x; 0x14016d9f0 QRectF::moveTo keeps the size
    r.x = static_cast<double>(it.col * kCellW) + kOriginX;
    r.y = rowY;
    return r;
}

namespace {

// Fresh-GUI view profile (inputs of the original that come from the machine, not the project),
// measured by Oracle B on Sim2 (coordinator 2026-10-22): the main window is maximised to client
// 3840x2036 at 168 DPI before any project opens, and the Operation Editor's QGraphicsView then has
// geometry 3371x123 with a 1 px frame (the height is viewCentre's argument). Its vertical scroll bar
// takes PM_ScrollBarExtent = 30 px (measured 29 px of bar; 30 is what the centre values need,
// docs/re/moproj2mst/moproj/view.md).
constexpr int kViewW = 3371, kFrame = 1, kScrollBarExtent = 30;

// Qt 4.8 qRound (q_round_bound for the scroll bar ranges)
int qRound(double d)
{
    return d >= 0.0 ? static_cast<int>(d + 0.5)
                    : static_cast<int>(d - static_cast<double>(static_cast<int>(d - 1)) + 0.5) + static_cast<int>(d - 1);
}

}  // namespace

// @src 0x14009a640  mapToScene(viewport()->geometry().center()) of the diagram view, with the view
// state a fresh GUI has after File > Open: the scene rect 0x14014f280 for the view size, FitAll
// 0x140174120 (fitInView(itemsBoundingRect.adjusted(-40, -40, 20, 20), KeepAspectRatio)) and Qt 4.8's
// QGraphicsView arithmetic (recalculateContentSize, centerOn, updateScroll, mapToScene).
void viewCentre(const SubProject& sub, DiagramViews& views, double& cx, double& cy, int kViewH)
{
    // @src 0x14014f280  scene rect (-25, -25, W + 25, H + 25)
    int lastRow = -1;
    for (const auto& c : sub.flow.items)
        if (!c->group && c->row > lastRow) lastRow = c->row;
    double W = static_cast<double>(kColumns * kCellW + 25), H = 0;   // 0x140154e90 columns
    if (lastRow >= 0) {
        double ry = 0, rh = 0;
        rowGeometry(sub, views, lastRow, ry, rh);
        H = ry + rh + 25.0;
    }
    H += static_cast<double>(kRowH);
    if (W < static_cast<double>(kViewW - 50)) W = static_cast<double>(kViewW - 50);
    if (H < static_cast<double>(kViewH - 50)) H = static_cast<double>(kViewH - 50);
    const double sx0 = -25.0, sy0 = -25.0, sw = W + 25.0, sh = H + 25.0;

    // @src 0x140174120  itemsBoundingRect of the cell items: the base rect (0, 0, 180, h) adjusted by
    // (-15, -15, 15, 15) at the cell position (0x1401637b0, 0x14016d900, 0x14016d9f0); a group
    // (SfOprCycle) adds no extent (checked against the Cycles2 saves); empty: (0, 0, 205, 185)
    double bl = 0, bt = 0, br = 0, bb = 0;
    bool any = false;
    for (std::size_t k = 0; k < sub.flow.items.size(); ++k) {
        if (sub.flow.items[k]->group) continue;
        const RectF pos = sceneRect(sub, static_cast<int>(k), views);
        const double l = pos.x - 15.0, t = pos.y - 15.0;
        const double r = pos.x + 195.0, b = t + itemRectHeight(sub, static_cast<int>(k), views);
        if (!any || l < bl) bl = l;
        if (!any || t < bt) bt = t;
        if (!any || r > br) br = r;
        if (!any || b > bb) bb = b;
        any = true;
    }
    if (!any) {
        bl = 0;
        bt = 0;
        br = static_cast<double>(kCellW);
        bb = static_cast<double>(kRowH);
    }
    const double fx = bl - 40.0, fy = bt - 40.0, fw = (br - bl) + 60.0, fh = (bb - bt) + 60.0;

    // QAbstractScrollArea::maximumViewportSize and the scroll bar policy (as needed) of
    // recalculateContentSize; the vertical bar shows for every fresh save (scene taller than 121 px)
    const int maxW = kViewW - 2 * kFrame, maxH = kViewH - 2 * kFrame;

    // QGraphicsView::fitInView(rect, Qt::KeepAspectRatio) with margin 2 on the viewport rect. The view
    // size is the one of the final layout: the window is maximised before the project opens.
    auto layout = [&](double s, int& w, int& h, bool& useH, bool& useV) {
        const double vw = sw * s, vh = sh * s;
        w = maxW;
        h = maxH;
        useH = vw > w;
        useV = vh > h;
        if (useH && !useV && vh > h - kScrollBarExtent) useV = true;
        if (useV && !useH && vw > w - kScrollBarExtent) useH = true;
        if (useH) h -= kScrollBarExtent;
        if (useV) w -= kScrollBarExtent;
    };
    // The viewport before the fit is the one of the previous transform: the empty-scene FitAll of
    // the first resize leaves the vertical bar shown and no horizontal bar.
    int w = maxW - kScrollBarExtent, h = maxH;
    bool useH = false, useV = true;
    const double xratio = static_cast<double>(w - 4) / fw, yratio = static_cast<double>(h - 4) / fh;
    const double s = xratio < yratio ? xratio : yratio;
    layout(s, w, h, useH, useV);

    // recalculateContentSize: ranges (q_round_bound) or indents (AlignCenter)
    const double vl = sx0 * s, vt = sy0 * s, vr = vl + sw * s, vb = vt + sh * s;
    double leftIndent = 0, topIndent = 0;
    int hmin = 0, hmax = 0, vmin = 0, vmax = 0;
    if (vr - vl > w) {
        hmin = qRound(vl);
        hmax = qRound(vr - w);
    } else {
        leftIndent = w / 2 - (vl + vr) / 2;
    }
    if (vb - vt > h) {
        vmin = qRound(vt);
        vmax = qRound(vb - h);
    } else {
        topIndent = h / 2 - (vt + vb) / 2;
    }
    // centerOn(rect.center()): int(viewPoint - viewport size / 2.0), clamped by the scroll bars
    auto clamp = [](int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; };
    int hval = 0, vval = 0;
    if (leftIndent == 0) hval = clamp(static_cast<int>((fx + fw / 2) * s - w / 2.0), hmin, hmax);
    if (topIndent == 0) vval = clamp(static_cast<int>((fy + fh / 2) * s - h / 2.0), vmin, vmax);
    // updateScroll and mapToScene(QPoint): (p + scroll) * matrix.inverted()
    const long long scrollX = static_cast<long long>(-leftIndent) + hval;
    const long long scrollY = static_cast<long long>(vval - topIndent);
    const int px = (kFrame + (kFrame + w - 1)) / 2;   // QRect::center of viewport()->geometry()
    const int py = (kFrame + (kFrame + h - 1)) / 2;
    const double inv = 1.0 / s;
    cx = static_cast<double>(px + scrollX) * inv;
    cy = static_cast<double>(py + scrollY) * inv;
}

namespace {

// The GUI's views: every part is ported.
class GuiViews : public DiagramViews {
public:
    explicit GuiViews(Project& p) : p_(p) {}
    void viewCentre(const SubProject& sub, double& x, double& y) override
    {
        moproj::viewCentre(sub, *this, x, y, p_.diagramViewHeight);
    }
    NodePtr dispView(const SubProject&) override { return dispViewElement(p_); }
    bool expanded(const SubProject& sub, int i) override { return savedExpanded(sub, i); }
    RectF rect(const SubProject& sub, int i) override { return sceneRect(sub, i, *this); }
    std::vector<ExpansionItem> expansion(const SubProject& sub, int i) override
    {
        return sub.flow.items[static_cast<std::size_t>(i)]->expansion;   // built at open (0x140073900)
    }

private:
    Project& p_;
};

// @src 0x140d07a00  <name> with n children x, y, z, s holding QString::number(v, 'g', 6)
void cameraField(Node& parent, const char* name, const double* v, int n)
{
    static const char* const kAxis[] = {"x", "y", "z", "s"};
    NodePtr e = element(name);
    for (int i = 0; i < n && i < 4; ++i) append(*e, textElement(kAxis[i], qt::number(v[i], 'g', 6)));
    append(parent, e);
}

}  // namespace

NodePtr dispViewElement(Project& p)
{
    NodePtr e = element("SfDispView");                                // 0x140cd57b0
    const engine::CameraState c = engine::displayCamera(p);           // 0x140d05a10
    // @src 0x140d06610
    cameraField(*e, "VersionNum", &c.version, 1);
    cameraField(*e, "Quaternion", c.quat, 4);
    cameraField(*e, "EyeVector", c.eye, 3);
    cameraField(*e, "ViewVector", c.view, 3);
    cameraField(*e, "UpVector", c.up, 3);
    cameraField(*e, "ZoomFactor", &c.zoom, 1);
    cameraField(*e, "CenterX", &c.centerX, 1);
    cameraField(*e, "CenterY", &c.centerY, 1);
    cameraField(*e, "Left", &c.lrtb[0], 1);
    cameraField(*e, "Right", &c.lrtb[1], 1);
    cameraField(*e, "Top", &c.lrtb[2], 1);
    cameraField(*e, "Bottom", &c.lrtb[3], 1);
    cameraField(*e, "Origin", c.origin, 3);
    return e;
}

std::unique_ptr<DiagramViews> guiViews(Project& p)
{
    return std::make_unique<GuiViews>(p);
}

bool readProjectFile(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const String& path,
                     std::string& bytes)
{
    const String key = qt::cleanPath(path);
    for (auto e = earlier.rbegin(); e != earlier.rend(); ++e) {
        const engine::FileEffects& fx = **e;
        for (auto d = fx.deletes.rbegin(); d != fx.deletes.rend(); ++d)
            if (qt::cleanPath(*d) == key) return false;
        for (auto w = fx.writes.rbegin(); w != fx.writes.rend(); ++w)
            if (qt::cleanPath(w->path) == key) {
                bytes = w->bytes;
                return true;
            }
    }
    auto it = p.overlay.find(key);
    if (it != p.overlay.end()) {
        bytes = it->second;
        return true;
    }
    if (!qt::fileExists(path)) return false;
    bytes = readAll(std::filesystem::path(path.u16()));
    return true;
}

static bool saveProjectFileBody(Project& p, bool withBlobs, DiagramViews& views,
                     const std::vector<const engine::FileEffects*>& earlier, engine::FileEffects& fx,
                     mst::OutputDevice* output)
{
    if (!p.dom || !p.dom->root) throw Error(M2M_E_UNSUPPORTED, "project file: no document read at open");
    xml::Document& doc = *p.dom;
    // Native caller has already passed the QFile open gate; recording caller
    // explicitly simulates it. Failed native open never reaches these mutations.
    NodePtr title = rootChild(doc, "Title", true);
    attr(*title, "Name", p.title);                                 // P+0x28
    NodePtr unit = rootChild(doc, "Unit", true);
    attr(*unit, "Name", p.unit == 1 ? L("SI") : L("English"));    // P+0x20
    NodePtr db = rootChild(doc, "Primary_DB_Name", true);
    attr(*db, "Name", relativeToProject(p, primaryDbPath(p)));    // 0x1400a4ee0, 0x1400a42c0
    operationList(doc, p, earlier, withBlobs);                     // 0x14009cfd0
    itemLists(doc, p, earlier, withBlobs);                         // 0x1400a1110 .. 0x1400a14d0
    SubProject* sub = activeSubProject(p);
    if (!sub) throw Error(M2M_E_UNSUPPORTED, "project file: no active sub-project");
    NodePtr ns = subProjectElement(p, *sub, views);                // 0x140099040(P, doc, "", type, 0)
    // @src 0x1400a88a0 / appendChild
    int k = 0;
    NodePtr old;
    for (const NodePtr& c : doc.root->children)
        if (c->kind == Node::Element && c->tag == L("Sub_Project")) {
            if (k == p.activeSub) {
                old = c;
                break;
            }
            ++k;
        }
    if (p.activeSub < static_cast<int>(p.subs.size())) {           // 0x1400a7360
        if (!old) throw Error(M2M_E_UNSUPPORTED, "project file: replacing a missing Sub_Project element (0x1400a88a0)");
        attr(*ns, "Name", old->attribute(L("Name"), String()));
        std::replace(doc.root->children.begin(), doc.root->children.end(), old, ns);
    } else {
        doc.root->children.push_back(ns);
    }
    const String text(xml::toString(doc, 1));                      // QTextStream UTF-8 << toString(1)
    if (output) {
        mst::Stream stream(65001, output);                        // setCodec("UTF-8")
        stream << text;
        output->close();                                         // aboutToClose flushes pending text
    } else {
        fx.writes.push_back({p.path, text.utf8()});
    }
    return true;
}

bool saveProjectFile(Project& p, bool withBlobs, DiagramViews& views,
                     const std::vector<const engine::FileEffects*>& earlier, engine::FileEffects& fx)
{
    if (p.path.isNull() || p.path.isEmpty()) return false;
    p.path = app::appendSuffix(p.path, L("moproj"));
    return saveProjectFileBody(p, withBlobs, views, earlier, fx, nullptr);
}

bool saveProjectFileNative(Project& p, bool withBlobs, DiagramViews& views,
                           const std::vector<const engine::FileEffects*>& earlier)
{
    if (p.path.isNull() || p.path.isEmpty()) return false;
    p.path = app::appendSuffix(p.path, L("moproj"));
    mst::FileDevice file(p.path);
    if (!file.opened()) return false;                             // information dialog's sole OK answer
    engine::FileEffects unused;
    (void)saveProjectFileBody(p, withBlobs, views, earlier, unused, &file);
    // 0x140098dd3 queries QFile.error AFTER close, which can clear an earlier
    // error even when QTextStream remains failed. Do not substitute stream status.
    return file.error() == 0;
}

}  // namespace m2m::moproj
