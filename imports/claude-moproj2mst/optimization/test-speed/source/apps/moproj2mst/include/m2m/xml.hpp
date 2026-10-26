// m2m/xml.hpp: the subset of QDomDocument the .moproj reader needs (seam; owner P01).
// s1-01 2: namespace processing off, exact tag names, text() = concatenated descendant text,
// attribute order irrelevant, file read in text mode (CRLF -> LF).
#pragma once

#include <memory>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::xml {
using qt::String;

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Attr {
    String name, value;
};

struct Node {
    enum Kind { Element, Text, CData, Comment, ProcessingInstruction } kind = Element;
    String tag;                    // Element: tag name
    String data;                   // Text/CData/Comment: content after entity expansion
    std::vector<Attr> attrs;       // Element
    std::vector<NodePtr> children;

    // QDomElement::attribute(name, defValue)
    String attribute(const String& name, const String& defValue = String()) const;
    bool hasAttribute(const String& name) const;
    // QDomElement::text()
    String text() const;
    // Child elements in document order (QDomNode::firstChild/nextSibling + toElement).
    std::vector<const Node*> childElements() const;
    std::vector<const Node*> childElements(const String& tag) const;
};

struct Document {
    NodePtr root;                  // documentElement()
    // ---- added by the .moproj writer: the document's other children, for toString ----------
    String doctypeName;            // <!DOCTYPE name> (null when the file has none)
    String doctypePublic, doctypeSystem;   // external ids (null when absent)
    // a top-level node the writer does not reproduce: <?xml ...?>, a PI or comment outside the
    // root, an internal DTD subset (toString refuses such a document)
    bool otherTopLevel = false;
};

// QDomDocument::setContent(QIODevice* opened ReadOnly|Text, namespaceProcessing=false, ...)
// on the file bytes. Returns false with Qt's error message, line and column on a parse error.
bool setContent(Document& doc, const std::string& fileBytes, String* err, int* line, int* col);

// ---- added by the .moproj writer (src/xml/save.cpp) ---------------------------------------------
// QDomDocument::toString(indent) of Qt 4.8 (QDomDocumentPrivate::saveDocument and the node save()
// functions): attributes in the order of QDomNamedNodeMapPrivate's QHash, which is replayed from
// the order the attributes were set (Node::attrs). Throws Error(M2M_E_UNSUPPORTED) for a document
// with otherTopLevel.
std::u16string toString(const Document& doc, int indent);
// QDomElement::setAttribute(name, value): an existing attribute keeps its place
void setAttribute(Node& e, const String& name, const String& value);
// QDomDocument::createElement(tag), not yet in a tree
NodePtr createElement(const String& tag);
// QDomDocument::createTextNode(text)
NodePtr createTextNode(const String& text);

}  // namespace m2m::xml
