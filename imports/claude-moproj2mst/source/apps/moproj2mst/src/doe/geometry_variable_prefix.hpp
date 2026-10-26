#pragma once

#include "m2m/qt4.hpp"
#include "m2m/xml.hpp"

namespace m2m::doe {

// Bounded value projection for the retained B122 Workpiece-Geometry record.
// These are XML metadata/path references only; no Study/Var or file ownership
// is implied by this type.
struct B122GeometryVariableRecord {
    qt::String name;
    int variableId = 0;
    int dimension = 0;
    int sampleCount = 0;
    int geoObject = 0;
    int geoOperation = 0;
    int morphMethod = 0;
    bool upperDefined = false;
    bool lowerDefined = false;
    qt::String nominalGeoFile;
    qt::String upperGeoFile;
    qt::String lowerGeoFile;
    bool upperCorrespondencePresent = false;
    bool lowerCorrespondencePresent = false;
    int operationBindingId = 0;
    qt::String operationBindingName;
};

// Parses only one retained B122 geometry variable and its one top-level
// operation binding. Unsupported shapes throw M2M_E_UNSUPPORTED.
B122GeometryVariableRecord parseB122GeometryVariablePrefix(
    const xml::Node& doeVariables, const xml::Node& operations);

} // namespace m2m::doe
