// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <optional>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/xml.hpp"
#include "../../src/doe/geometry_path_projection.hpp"
#include "../../src/doe/geometry_row_prefix.hpp"

using namespace m2m;
namespace {
doe::GeometryRowPrefixInput input(int operation = 1, int object = 2)
{
    return {qt::L("nominal.GEO"), qt::L("upper.GEO"), qt::L("lower.GEO"),
        operation, object, 9, std::nullopt, std::nullopt};
}
template<class Work> void refused(const Work& work)
{
    try { work(); CHECK(false); }
    catch (const Error& error) { CHECK_EQ(error.code, M2M_E_UNSUPPORTED); }
}
}

TEST_CASE(doe_geometry_row_prefix_projects_paths_into_actual_study_record_owner)
{
    doe::Study study;
    REQUIRE(doe::readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\"/></DoeVariables></sfDoeInputTable>", study));
    REQUIRE(study.headerVariableOwners.size() == 1);
    auto& variable = *study.headerVariableOwners[0];
    REQUIRE(study.vars[0] == &variable);
    auto node = xml::createElement(qt::L("Geometry"));
    xml::setAttribute(*node, qt::L("UpperGeoFile"), qt::L("../upper.GEO"));
    xml::setAttribute(*node, qt::L("NominalGeoFile"), qt::L("nominal.GEO"));
    const auto paths = doe::projectGeometryPathValues({qt::L("C:/Control/DoeInputTable.XML")}, *node);
    auto request = input();
    request.nominalGeoFile = paths.nominalGeoFile;
    request.upperGeoFile = paths.upperGeoFile;
    request.lowerGeoFile = paths.lowerGeoFile;
    const auto result = doe::applyGeometryRowPrefix(variable, request);
    CHECK(result.appended);
    CHECK_EQ(result.matchedRowIndex, std::size_t(0));
    CHECK_EQ(result.tailRowIndex, std::size_t(0));
    CHECK(result.nextBoundary == doe::GeometryRowPrefixBoundary::CorrespondencePayload);
    REQUIRE(variable.files.size() == 1);
    const auto& row = variable.files[0];
    CHECK(row.path == qt::L("C:/Control/nominal.GEO"));
    CHECK(row.before == qt::L("C:/Control/../upper.GEO"));
    CHECK(row.after == qt::L("C:/Control/"));
    CHECK(row.geoOperation == std::optional<int>(1));
    CHECK(row.geoObject == std::optional<int>(2));
    CHECK(row.useBefore && row.useAfter);
    CHECK_EQ(variable.geoType, 9);
    CHECK(variable.items.empty() && variable.ops.empty() && !variable.geometry);
    CHECK_EQ(variable.number, 7);
    CHECK_EQ(variable.numSamples, 3);
}

TEST_CASE(doe_geometry_row_prefix_dedupe_excludes_nominal_but_mutates_tail_flags)
{
    doe::HeaderVariableRecord variable(3);
    auto a = input();
    a.upperDefined = qt::L("false");
    a.lowerDefined = qt::L("false");
    (void)doe::applyGeometryRowPrefix(variable, a);
    auto b = input(3, 4);
    b.nominalGeoFile = qt::L("tail.GEO");
    (void)doe::applyGeometryRowPrefix(variable, b);
    a.nominalGeoFile = qt::L("ignored-replacement.GEO");
    a.upperDefined = qt::L("");
    a.lowerDefined = qt::L("false");
    a.morphMethod = -7;
    const auto result = doe::applyGeometryRowPrefix(variable, a);
    CHECK(!result.appended);
    CHECK_EQ(result.matchedRowIndex, std::size_t(0));
    CHECK_EQ(result.tailRowIndex, std::size_t(1));
    REQUIRE(variable.files.size() == 2);
    CHECK(variable.files[0].path == qt::L("nominal.GEO"));
    CHECK(!variable.files[0].useBefore && !variable.files[0].useAfter);
    CHECK(variable.files[1].path == qt::L("tail.GEO"));
    CHECK(variable.files[1].useBefore && !variable.files[1].useAfter);
    CHECK_EQ(variable.geoType, -7);
}

TEST_CASE(doe_geometry_row_prefix_uses_all_four_key_components_and_content_equality)
{
    doe::HeaderVariableRecord variable(3);
    auto request = input(0, 0);
    request.upperGeoFile = qt::String();
    request.lowerGeoFile = qt::L("");
    (void)doe::applyGeometryRowPrefix(variable, request);
    request.upperGeoFile = qt::L("");
    request.lowerGeoFile = qt::String();
    CHECK(!doe::applyGeometryRowPrefix(variable, request).appended);
    request.geoOperation = -1;
    CHECK(doe::applyGeometryRowPrefix(variable, request).appended);
    request.geoObject = -2;
    CHECK(doe::applyGeometryRowPrefix(variable, request).appended);
    request.upperGeoFile = qt::L("different-upper");
    CHECK(doe::applyGeometryRowPrefix(variable, request).appended);
    request.lowerGeoFile = qt::L("different-lower");
    CHECK(doe::applyGeometryRowPrefix(variable, request).appended);
    CHECK_EQ(variable.files.size(), std::size_t(5));
}

TEST_CASE(doe_geometry_row_prefix_preserves_asymmetric_absence_and_exact_false)
{
    doe::HeaderVariableRecord variable(3);
    auto request = input();
    request.upperGeoFile = qt::L("");
    request.lowerGeoFile = qt::L("");
    (void)doe::applyGeometryRowPrefix(variable, request);
    CHECK(!variable.files[0].useBefore && variable.files[0].useAfter);
    request.upperDefined = qt::L("false");
    request.lowerDefined = qt::L("false");
    (void)doe::applyGeometryRowPrefix(variable, request);
    CHECK(!variable.files[0].useBefore && !variable.files[0].useAfter);
    request.upperDefined = qt::L("False");
    request.lowerDefined = qt::L("");
    (void)doe::applyGeometryRowPrefix(variable, request);
    CHECK(variable.files[0].useBefore && variable.files[0].useAfter);
}

TEST_CASE(doe_geometry_row_prefix_unknown_legacy_key_refuses_before_any_mutation)
{
    for (int missing = 0; missing < 3; ++missing) {
        doe::HeaderVariableRecord variable(3);
        auto request = input();
        (void)doe::applyGeometryRowPrefix(variable, request);
        auto legacy = variable.files[0];
        if (missing != 1) legacy.geoOperation.reset();
        if (missing != 0) legacy.geoObject.reset();
        variable.files.push_back(legacy);
        request.morphMethod = 88;
        request.upperDefined = qt::L("false");
        request.lowerDefined = qt::L("false");
        refused([&] { (void)doe::applyGeometryRowPrefix(variable, request); });
        CHECK_EQ(variable.files.size(), std::size_t(2));
        CHECK(variable.files[0].useBefore && variable.files[0].useAfter);
        CHECK(variable.files[1].useBefore && variable.files[1].useAfter);
        CHECK_EQ(variable.geoType, 9);
    }
}

TEST_CASE(doe_geometry_row_prefix_does_not_admit_public_geometry_parser)
{
    doe::Study study;
    refused([&] { (void)doe::readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\"><Geometry/></DoeVariable></DoeVariables></sfDoeInputTable>", study); });
    REQUIRE(study.headerVariableOwners.size() == 1);
    CHECK(study.headerVariableOwners[0]->files.empty());
    CHECK_EQ(study.headerVariableOwners[0]->geoType, 100);
    CHECK_EQ(study.headerVariableOwners[0]->index, -1);
}

int main() { return testkit::run_all(); }
