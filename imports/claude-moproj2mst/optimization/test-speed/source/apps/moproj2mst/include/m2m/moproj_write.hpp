// m2m/moproj_write.hpp: the .moproj writer of File > Save (step 3, 0x1400983c0). Packet moproj.
// Spec: docs/re/moproj2mst/moproj/spec.md.
//
// The writer edits the DOM read at open (Project::dom, P+0x48) in place, rebuilds the lists and
// the active Sub_Project element and serialises it with QDomDocument::toString(1) in UTF-8
// (xml::toString). Parts of Sub_Project come from the GUI's live views (the diagram view's
// centre, the diagram items' scene rectangles, expansion and expanded state, and the 3D viewer's
// camera); they are asked from a DiagramViews object.
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/xml.hpp"

namespace m2m::engine {
struct FileEffects;
}

namespace m2m::moproj {

// One entry of a diagram item's expansion list (item vt+0x50): what 0x14009b710 writes.
using ExpansionItem = ExpansionRow;

struct RectF {
    double x = 0, y = 0, w = 0, h = 0;
};

// What 0x140099040 and 0x14009b710 read from the GUI's views. The GUI implementation is
// guiViews(); tests may substitute another.
class DiagramViews {
public:
    virtual ~DiagramViews() = default;
    // 0x14009a640: mapToScene(viewport centre) of the diagram view
    virtual void viewCentre(const SubProject& sub, double& x, double& y) = 0;
    // 0x140cd57b0: the SfDispView element of the 3D viewer (built in doc)
    virtual xml::NodePtr dispView(const SubProject& sub) = 0;
    // item vt+0xa8 / vt+0x98 / vt+0x50 of the diagram item at index i of the flow chart
    virtual bool expanded(const SubProject& sub, int i) = 0;
    virtual RectF rect(const SubProject& sub, int i) = 0;
    virtual std::vector<ExpansionItem> expansion(const SubProject& sub, int i) = 0;
};

// The GUI's views. rect() is the ported scene layout (sceneRect) and viewCentre() the ported view
// transform (viewCentre, both need expansion() for item heights); the other parts refuse
// (M2M_E_UNSUPPORTED) with the address that is missing until they are ported.
std::unique_ptr<DiagramViews> guiViews(Project& p);

// @src 0x140cd57b0  the SfDispView element of the main window's 3D display (engine::displayCamera)
xml::NodePtr dispViewElement(Project& p);

// @src 0x140157fe0 / 0x1401583e0  item i's model rectangle (vt+0x98) after the diagram cell editor
// laid the flow chart out: the rectangle read at open (0x140097d00) moved to the cell's scene
// position (0x14016d9f0), for a group item the rectangle read at open. Rows below row 0 take the
// graphics item heights of the rows above (0x1401637b0), which need views.expansion().
RectF sceneRect(const SubProject& sub, int i, DiagramViews& views);

// @src 0x14009a640  ViewTransform CenterX / CenterY: the diagram view's mapToScene of the viewport
// centre after File > Open in a fresh, maximised GUI (fixed view profile in write.cpp; item heights
// through views.expansion()). viewHeight: the view's height, 123 after open, 175 when the project's
// database opened (Project::diagramViewHeight). docs/re/moproj2mst/moproj/view.md
void viewCentre(const SubProject& sub, DiagramViews& views, double& x, double& y, int viewHeight = 123);
// @src 0x1401712a0  item i's model expanded flag (vt+0xa8) at save: "yes" for every item with a
// cell graphics item. Group items keep an uninitialised byte in the original; the port writes
// "yes" (the one known deviation, Aleksandr 2026-10-22).
bool savedExpanded(const SubProject& sub, int i);

// The project files as the GUI would see them when Save reaches step 3: files written or deleted
// earlier in this process (at open, in step 2), the blobs written at open (Project::overlay), then
// the disk. Returns false when the file does not exist.
bool readProjectFile(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const String& path,
                     std::string& bytes);

// @src 0x1400a1110 / 0x1400a16b0 / 0x1400a12f0 / 0x1400a14d0  Object_List, Material_List,
// Lubricant_List, Equipment_List of the DOM rebuilt from the four managers (in the order
// 0x1400983c0 calls them); packet moproj-lists
void itemLists(xml::Document& doc, const Project& p, const std::vector<const engine::FileEffects*>& earlier,
               bool withBlobs);

// @src 0x1400983c0  SaveProjectFile(P, withBlobs): the .moproj path gets its suffix (P+0x18), the
// DOM is edited and written. Returns false when the original returns 0 (empty path; the "Project
// file can't be saved" case cannot happen here: the bytes go to fx).
bool saveProjectFile(Project& p, bool withBlobs, DiagramViews& views,
                     const std::vector<const engine::FileEffects*>& earlier, engine::FileEffects& fx);

// Same DOM writer with original QFile mode2 opened before any DOM update. A
// failed open returns false with the old DOM intact. Success writes UTF-8 via
// QTextStream and closes before testing QFile.error (not Stream status).
// The Save coordinator must continue later stages even when this returns false.
bool saveProjectFileNative(Project& p, bool withBlobs, DiagramViews& views,
                           const std::vector<const engine::FileEffects*>& earlier);

}  // namespace m2m::moproj
