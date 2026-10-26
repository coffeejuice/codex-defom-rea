#pragma once

#include <vector>

#include "m2m/xml.hpp"

namespace m2m::doe {

// Detached view of one direct element child of a SessionRoot-level DBLIST.
// This records structure only; it does not resolve paths or validate native
// DBLIST semantics.
struct DbListChildValue {
    qt::String tag;
    qt::String text;
};

// Return direct element children in document order, using each element's
// QDomElement::text()-compatible concatenated descendant text. No state or I/O.
std::vector<DbListChildValue> readDbListDirectChildren(const xml::Node& dbList);

} // namespace m2m::doe
