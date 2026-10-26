#include "geometry_variable_binding.hpp"

#include <string>
#include <utility>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("B122 geometry variable binding: ") + reason);
}

bool retainedB122Record(const B122GeometryVariableRecord& record)
{
    return record.name == qt::L("Workpiece-Geometry") && record.variableId == 1 &&
        record.dimension == 3 && record.sampleCount == 3 && record.geoObject == 1 &&
        record.geoOperation == 1 && record.morphMethod == 100 && !record.upperDefined &&
        !record.lowerDefined && record.nominalGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Nominal.GEO") &&
        record.upperGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Upper.GEO") &&
        record.lowerGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Lower.GEO") &&
        record.upperCorrespondencePresent && record.lowerCorrespondencePresent &&
        record.operationBindingId == 1 && record.operationBindingName == record.name;
}
} // namespace

Var& bindB122GeometryVariablePrefix(Study& study, const B122GeometryVariableRecord& record)
{
    if (!study.vars.empty() || !study.geometryPrefixVariableOwners.empty() ||
        !study.scalarPrefixVariableOwners.empty())
        refuse("requires a fresh empty Study and no prior DOE prefix owner");
    if (!retainedB122Record(record))
        refuse("record differs from the retained one-variable B122 shape");

    auto variable = std::make_unique<Var>();
    variable->name = record.name;
    variable->number = record.variableId;
    variable->index = -1; // manager queries reindex when reached
    variable->ops.push_back(record.operationBindingId); // separate top-level Operations mapping

    GeometryVariable geometry;
    geometry.id = record.variableId;
    geometry.dimension = record.dimension;
    geometry.sampleCount = record.sampleCount;
    geometry.geoObject = record.geoObject;
    geometry.geoOperation = record.geoOperation;
    geometry.morphMethod = record.morphMethod;
    geometry.upperDefined = record.upperDefined;
    geometry.lowerDefined = record.lowerDefined;
    geometry.nominalGeoFile = record.nominalGeoFile;
    geometry.upperGeoFile = record.upperGeoFile;
    geometry.lowerGeoFile = record.lowerGeoFile;
    geometry.upperCorrespondencePresent = record.upperCorrespondencePresent;
    geometry.lowerCorrespondencePresent = record.lowerCorrespondencePresent;
    variable->geometry = std::move(geometry);

    // Reserve both vectors before publishing either ownership or the nonowning
    // Model alias, so a capacity allocation failure leaves the logical Study empty.
    study.geometryPrefixVariableOwners.reserve(study.geometryPrefixVariableOwners.size() + 1);
    study.vars.reserve(study.vars.size() + 1);
    Var* const alias = variable.get();
    study.geometryPrefixVariableOwners.push_back(std::move(variable));
    study.vars.push_back(alias);
    return *alias;
}

} // namespace m2m::doe
