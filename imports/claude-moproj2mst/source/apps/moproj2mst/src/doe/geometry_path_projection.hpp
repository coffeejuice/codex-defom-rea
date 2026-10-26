#pragma once

#include "input_table_reader.hpp"

namespace m2m::xml { struct Node; }
namespace m2m::doe {

// Conditional normal-return path VALUES only. This is not a constructed
// QFileInfo, native ref/engine receipt, Geometry row or public import capability.
struct GeometryPathValues {
    qt::String sourceFileName;
    qt::String parentDirectory;
    qt::String upperGeoFile;
    qt::String lowerGeoFile;
    qt::String nominalGeoFile;
};

// Isolated projection for named absolute drive/UNC filenames shorter than
// 32768 UTF-16 units. Unknown source/search-prefix/default-construction branches
// refuse. Attribute names are joined literally; no I/O or Study/Var mutation.
GeometryPathValues projectGeometryPathValues(const InputTableSource&, const xml::Node& geometry);
} // namespace m2m::doe
