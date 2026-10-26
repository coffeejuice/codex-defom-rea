#include "geometry_variable_writer.hpp"

#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("B122 geometry writer prefix: ") + reason);
}

bool isRetainedB122(const Study& study)
{
    if (!study.commonMeshPath.isEmpty() || !study.commonMeshOrigName.isEmpty() ||
        study.doPositioning || study.roiType != 0 || !study.regions.empty() ||
        study.constraints != 0 || study.vars.size() != 1 || !study.scalarPrefixVariableOwners.empty() ||
        study.geometryPrefixVariableOwners.size() != 1 || study.vars[0] == nullptr ||
        study.vars[0] != study.geometryPrefixVariableOwners[0].get())
        return false;
    const Var& variable = *study.vars[0];
    if (variable.name != qt::L("Workpiece-Geometry") || variable.number != 1 ||
        !variable.files.empty() || !variable.items.empty() || variable.link != nullptr ||
        variable.scalar.has_value() || !variable.geometry || variable.ops != std::vector<int>{1})
        return false;
    const GeometryVariable& g = *variable.geometry;
    return g.id == 1 && g.dimension == 3 && g.sampleCount == 3 && g.geoObject == 1 &&
        g.geoOperation == 1 && g.morphMethod == 100 && !g.upperDefined && !g.lowerDefined &&
        g.nominalGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Nominal.GEO") &&
        g.upperGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Upper.GEO") &&
        g.lowerGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Lower.GEO") &&
        g.upperCorrespondencePresent && g.lowerCorrespondencePresent;
}
} // namespace

bool writeB122GeometryVariablePrefix(const String& path,
                                     const Study& study,
                                     engine::FileEffects& effects)
{
    if (!isRetainedB122(study))
        refuse("study differs from the retained one-variable B122 geometry profile");
    const Var& variable = *study.vars[0];
    const GeometryVariable& geometry = *variable.geometry;

    // Attribute and child ordering below is pinned to the retained B122 byte
    // fixture. It is not a general QDom attribute-order or geometry serializer.
    const std::string text =
        "<!DOCTYPE DoeInputTableFile>\n"
        "<sfDoeInputTable Version=\"1\">\n"
        " <DoeCommonMeshPath path=\"\"/>\n"
        " <DoeCommonMeshOrigName originalName=\"\"/>\n"
        " <DoeDoPositioning bool=\"false\"/>\n"
        " <DoeRoiType type=\"0\"/>\n"
        " <DoeVariables>\n"
        "  <DoeVariable numSamples=\"" + std::to_string(geometry.sampleCount) +
        "\" varId=\"" + std::to_string(geometry.id) +
        "\" dimension=\"" + std::to_string(geometry.dimension) +
        "\" name=\"" + variable.name.utf8() + "\">\n"
        "   <Geometry GeoObject=\"" + std::to_string(geometry.geoObject) +
        "\" UpperDefined=\"false\" LowerDefined=\"false\" GeoOperation=\"" +
        std::to_string(geometry.geoOperation) + "\" MorphMethod=\"" +
        std::to_string(geometry.morphMethod) + "\" UpperGeoFile=\"" +
        geometry.upperGeoFile.utf8() + "\" LowerGeoFile=\"" + geometry.lowerGeoFile.utf8() +
        "\" NominalGeoFile=\"" + geometry.nominalGeoFile.utf8() + "\">\n"
        "    <UpperCorrespondence/>\n"
        "    <LowerCorrespondence/>\n"
        "   </Geometry>\n"
        "  </DoeVariable>\n"
        " </DoeVariables>\n"
        " <Operations>\n"
        "  <Operation id=\"" + std::to_string(variable.ops.front()) + "\">\n"
        "   <DoeOprVariable name=\"" + variable.name.utf8() + "\"/>\n"
        "  </Operation>\n"
        " </Operations>\n"
        "</sfDoeInputTable>\n";

    std::string bytes;
    bytes.reserve(text.size() + 64);
    for (const char c : text) {
        if (c == '\n') bytes += '\r';
        bytes += c;
    }
    effects.writes.push_back({path, std::move(bytes)});
    return true;
}

} // namespace m2m::doe
