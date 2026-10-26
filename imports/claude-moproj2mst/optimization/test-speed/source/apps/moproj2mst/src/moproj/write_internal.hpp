// The .moproj writer's DOM helpers and builder entry points, shared by the files of the writer:
// write.cpp (0x1400983c0, Sub_Project, DiagramItem, views; packet moproj), write_items.cpp (the item
// lists; owner L2) and write_ops.cpp (Operation_List, properties, blobs, ActionKeywords; owner L3).
#pragma once

#include <string>
#include <vector>

#include "m2m/moproj_write.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"

namespace m2m::moproj {

using qt::L;
using xml::Node;
using xml::NodePtr;

inline NodePtr element(const char* tag) { return xml::createElement(L(tag)); }
inline void append(Node& parent, const NodePtr& child)
{
    if (child) parent.children.push_back(child);   // appendChild of a null node does nothing
}
inline void attr(Node& e, const char* name, const String& v) { xml::setAttribute(e, L(name), v); }
// QDomElement::setAttribute(name, int): QString::number(value)
inline void attrInt(Node& e, const char* name, int v) { xml::setAttribute(e, L(name), qt::number(v)); }

// <tag>text</tag>: createElement + createTextNode(text) + appendChild (a null text gives an empty
// text node, saved as <tag></tag>)
inline NodePtr textElement(const char* tag, const String& text)
{
    NodePtr e = element(tag);
    e->children.push_back(xml::createTextNode(text));
    return e;
}

// @src 0x14008c7f0  the first child element of the root with this tag, created at the end of the
// root when missing and `create`
inline NodePtr rootChild(xml::Document& doc, const char* tag, bool create)
{
    for (const NodePtr& c : doc.root->children)
        if (c->kind == Node::Element && c->tag == L(tag)) return c;
    if (!create) return nullptr;
    NodePtr e = element(tag);
    doc.root->children.push_back(e);
    return e;
}

// @src 0x14008ca50  remove every child
inline void clearChildren(Node& n) { n.children.clear(); }

// @src 0x1400a42c0  QDir(0x140819590(P+0x18) = QFileInfo(path).absolutePath()).relativeFilePath(p)
inline String relativeToProject(const Project& p, const String& path)
{
    return qt::relativeFilePath(qt::absolutePath(p.path), path);
}

// @src 0x140042460  QStringList::join(" ") of numbers
inline String joinNumbers(const std::vector<int>& v)
{
    String r;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) r += u' ';
        r += qt::number(v[i]);
    }
    return r;
}

// ---- builders (one per binary function) ------------------------------------------------------------
// @src 0x1400a1890  <Data> blob of a project file, null when it cannot be read (write_ops.cpp)
NodePtr blobElement(const Project& p, const std::vector<const engine::FileEffects*>& earlier, const String& path);
// @src 0x1400a1110 / 0x1400a16b0 / 0x1400a12f0 / 0x1400a14d0 -> 0x1400a0f60 -> 0x1400a0210 (write_items.cpp)
void managerList(xml::Document& doc, const Project& p, const std::vector<const engine::FileEffects*>& earlier,
                 const char* tag, const ItemManager& m, bool withBlobs);
// @src 0x14009cfd0  Operation_List (write_ops.cpp)
void operationList(xml::Document& doc, Project& p, const std::vector<const engine::FileEffects*>& earlier,
                   bool withBlobs);

}  // namespace m2m::moproj
