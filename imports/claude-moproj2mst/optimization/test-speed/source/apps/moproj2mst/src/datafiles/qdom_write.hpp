// Qt 4.8 QDomDocument serialisation (toByteArray(1) of a document with a doctype and one root
// element, written through a QFile in Text mode), as the GUI's wizard writers use it
// (0x1402a16c0 DieStressBmWizard.Wzd, 0x1402b9050 DieStress.Wzd). Owner MDT L6.
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::datafiles {

struct DomElement {
    std::u16string name;
    std::vector<std::pair<std::u16string, std::u16string>> attrs;   // insertion order
    std::vector<DomElement> children;

    // QDomElement::setAttribute: an existing name keeps its place, its value changes
    void set(const std::u16string& n, const std::u16string& v);
    void setInt(const char16_t* n, int v);                       // setAttribute(QString, int)
    void setDouble(const char16_t* n, double v);                 // setAttribute(QString, double)
    void setString(const char16_t* n, const qt::String& v);
};

// "<!DOCTYPE name>\n" + the root element with indent 1, UTF-8, "\n" -> "\r\n" (Text mode)
std::string documentBytes(const std::u16string& doctype, const DomElement& root);

// QDomDocument::toByteArray(1), before QIODevice Text-mode expansion.
std::string documentUtf8(const std::u16string& doctype, const DomElement& root);

}  // namespace m2m::datafiles
