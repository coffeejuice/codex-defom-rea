// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "m2m/common.hpp"
#include "../../src/doe/geometry_variable_binding.hpp"

using namespace m2m;
namespace fs = std::filesystem;

namespace {
std::string fixtureBytes()
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") /
        "122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML";
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("unable to open retained B122 fixture: " + path.string());
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

doe::B122GeometryVariableRecord fixtureRecord()
{
    xml::Document document;
    String error;
    int line = 0, column = 0;
    if (!xml::setContent(document, fixtureBytes(), &error, &line, &column))
        throw std::runtime_error("B122 fixture XML parse failed: " + error.utf8());
    const auto variables = document.root->childElements(qt::L("DoeVariables"));
    const auto operations = document.root->childElements(qt::L("Operations"));
    if (variables.size() != 1 || operations.size() != 1)
        throw std::runtime_error("B122 fixture lacks unique DOE sections");
    return doe::parseB122GeometryVariablePrefix(*variables.front(), *operations.front());
}

bool refuses(const auto& action)
{
    try { action(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
} // namespace

TEST_CASE(b122_geometry_prefix_binds_exactly_one_owned_model_variable_without_geo_effects)
{
    doe::Study study;
    const auto record = fixtureRecord();
    auto& variable = doe::bindB122GeometryVariablePrefix(study, record);

    REQUIRE(study.geometryPrefixVariableOwners.size() == 1u);
    REQUIRE(study.vars.size() == 1u);
    CHECK(study.vars[0] == &variable);
    CHECK(study.vars[0] == study.geometryPrefixVariableOwners[0].get());
    CHECK(variable.name == qt::L("Workpiece-Geometry"));
    CHECK(variable.number == 1);
    CHECK(variable.index == -1);
    CHECK(variable.ops == std::vector<int>{1});
    CHECK(variable.files.empty());
    CHECK(variable.items.empty());
    CHECK(variable.link == nullptr);
    CHECK(!variable.scalar.has_value());
    REQUIRE(variable.geometry.has_value());
    const auto& geometry = *variable.geometry;
    CHECK(geometry.id == 1 && geometry.dimension == 3 && geometry.sampleCount == 3);
    CHECK(geometry.geoObject == 1 && geometry.geoOperation == 1 && geometry.morphMethod == 100);
    CHECK(!geometry.upperDefined && !geometry.lowerDefined);
    CHECK(geometry.nominalGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Nominal.GEO"));
    CHECK(geometry.upperGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Upper.GEO"));
    CHECK(geometry.lowerGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Lower.GEO"));
    CHECK(geometry.upperCorrespondencePresent && geometry.lowerCorrespondencePresent);

    CHECK(study.count() == 1);
    CHECK(study.varAt(0) == &variable);
    CHECK(study.varAt(1) == nullptr);
    const auto all = study.allVars();
    REQUIRE(all.size() == 1u);
    CHECK(all[0] == &variable && variable.index == 0);
    const auto forOperation = study.varsForOperation(1);
    REQUIRE(forOperation.size() == 1u);
    CHECK(forOperation[0] == &variable);
    CHECK(study.varsForOperation(2).empty());
}

TEST_CASE(b122_geometry_prefix_duplicate_or_changed_record_refuses_without_mutation)
{
    doe::Study study;
    const auto record = fixtureRecord();
    auto& first = doe::bindB122GeometryVariablePrefix(study, record);
    CHECK(refuses([&] { (void)doe::bindB122GeometryVariablePrefix(study, record); }));
    REQUIRE(study.geometryPrefixVariableOwners.size() == 1u);
    REQUIRE(study.vars.size() == 1u);
    CHECK(study.geometryPrefixVariableOwners[0].get() == &first);
    CHECK(study.vars[0] == &first);

    doe::Study empty;
    auto invalid = record;
    invalid.upperGeoFile = qt::L("unqualified.geo");
    CHECK(refuses([&] { (void)doe::bindB122GeometryVariablePrefix(empty, invalid); }));
    CHECK(empty.vars.empty());
    CHECK(empty.geometryPrefixVariableOwners.empty());
}

int main() { return testkit::run_all(); }
