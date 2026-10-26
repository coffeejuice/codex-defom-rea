#include "geometry_variable_prefix.hpp"

#include <string>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
using qt::String;

[[noreturn]] void refuse(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("B122 geometry variable prefix: ") + reason);
}

int requiredInt(const xml::Node& node, const char* name)
{
    const String key = qt::L(name);
    if (!node.hasAttribute(key)) refuse("required numeric attribute is missing");
    bool ok = false;
    const int value = qt::toInt(node.attribute(key), &ok);
    if (!ok) refuse("required numeric attribute is malformed");
    return value;
}

String requiredString(const xml::Node& node, const char* name)
{
    const String key = qt::L(name);
    if (!node.hasAttribute(key) || node.attribute(key).isEmpty())
        refuse("required string attribute is missing or empty");
    return node.attribute(key);
}

void requireFalse(const xml::Node& node, const char* name)
{
    if (!node.hasAttribute(qt::L(name)) || node.attribute(qt::L(name)) != qt::L("false"))
        refuse("defined flag differs from retained B122 false value");
}

bool element(const xml::Node* node, const char* tag)
{
    return node && node->kind == xml::Node::Element && node->tag == qt::L(tag);
}
} // namespace

B122GeometryVariableRecord parseB122GeometryVariablePrefix(
    const xml::Node& doeVariables, const xml::Node& operations)
{
    if (!element(&doeVariables, "DoeVariables") || !element(&operations, "Operations"))
        refuse("expected DoeVariables and Operations elements");
    const auto variables = doeVariables.childElements();
    if (variables.size() != 1 || !element(variables.front(), "DoeVariable"))
        refuse("expected exactly one DoeVariable");

    const xml::Node& variable = *variables.front();
    B122GeometryVariableRecord result;
    result.name = requiredString(variable, "name");
    result.variableId = requiredInt(variable, "varId");
    result.dimension = requiredInt(variable, "dimension");
    result.sampleCount = requiredInt(variable, "numSamples");
    if (result.name != qt::L("Workpiece-Geometry") || result.variableId != 1 ||
        result.dimension != 3 || result.sampleCount != 3 || variable.hasAttribute(qt::L("LinkTo")))
        refuse("variable header differs from retained Workpiece-Geometry record");

    const auto variableChildren = variable.childElements();
    if (variableChildren.size() != 1 || !element(variableChildren.front(), "Geometry"))
        refuse("expected exactly one Geometry child");
    const xml::Node& geometry = *variableChildren.front();
    result.geoObject = requiredInt(geometry, "GeoObject");
    result.geoOperation = requiredInt(geometry, "GeoOperation");
    result.morphMethod = requiredInt(geometry, "MorphMethod");
    requireFalse(geometry, "UpperDefined");
    requireFalse(geometry, "LowerDefined");
    result.upperDefined = false;
    result.lowerDefined = false;
    result.nominalGeoFile = requiredString(geometry, "NominalGeoFile");
    result.upperGeoFile = requiredString(geometry, "UpperGeoFile");
    result.lowerGeoFile = requiredString(geometry, "LowerGeoFile");
    if (result.geoObject != 1 || result.geoOperation != 1 || result.morphMethod != 100 ||
        result.nominalGeoFile != qt::L("Workpiece-Geometry_Workpiece_1_Nominal.GEO") ||
        result.upperGeoFile != qt::L("Workpiece-Geometry_Workpiece_1_Upper.GEO") ||
        result.lowerGeoFile != qt::L("Workpiece-Geometry_Workpiece_1_Lower.GEO"))
        refuse("geometry metadata or file names differ from retained B122 record");

    const auto correspondence = geometry.childElements();
    if (correspondence.size() != 2 || !element(correspondence[0], "UpperCorrespondence") ||
        !element(correspondence[1], "LowerCorrespondence") ||
        !correspondence[0]->children.empty() || !correspondence[1]->children.empty())
        refuse("expected ordered empty Upper/LowerCorrespondence elements");
    result.upperCorrespondencePresent = true;
    result.lowerCorrespondencePresent = true;

    const auto operationNodes = operations.childElements();
    if (operationNodes.size() != 1 || !element(operationNodes.front(), "Operation"))
        refuse("expected exactly one top-level Operation");
    const xml::Node& operation = *operationNodes.front();
    result.operationBindingId = requiredInt(operation, "id");
    const auto bindings = operation.childElements();
    if (result.operationBindingId != 1 || bindings.size() != 1 ||
        !element(bindings.front(), "DoeOprVariable"))
        refuse("expected one operation-1 DoeOprVariable binding");
    result.operationBindingName = requiredString(*bindings.front(), "name");
    if (result.operationBindingName != result.name)
        refuse("top-level operation binding names another variable");

    return result;
}

} // namespace m2m::doe
