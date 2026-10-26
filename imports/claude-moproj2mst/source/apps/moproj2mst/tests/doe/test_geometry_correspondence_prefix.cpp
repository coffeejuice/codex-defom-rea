// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <cmath>
#include <limits>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "../../src/doe/geometry_row_prefix.hpp"

using namespace m2m;
namespace {
using Side = doe::GeometryCorrespondenceSide;
doe::GeometryRowPrefixInput input(int operation)
{
    return {qt::L("nominal.GEO"), qt::L("upper.GEO"), qt::L("lower.GEO"),
        operation, 2, 9, std::nullopt, std::nullopt};
}
void checkPair(const doe::GeometryPointPair& pair, double x0, double y0, double x1, double y1)
{
    CHECK_EQ(pair.first.x, x0);
    CHECK_EQ(pair.first.y, y0);
    CHECK_EQ(pair.second.x, x1);
    CHECK_EQ(pair.second.y, y1);
}
}

TEST_CASE(doe_geometry_correspondence_reader_row_zero_differs_from_tail_flag_target)
{
    doe::Study study;
    REQUIRE(doe::readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\"/></DoeVariables></sfDoeInputTable>", study));
    REQUIRE(study.headerVariableOwners.size() == 1);
    auto& variable = *study.headerVariableOwners[0];
    REQUIRE(study.vars[0] == &variable);
    auto a = input(1);
    a.upperDefined = qt::L("false");
    a.lowerDefined = qt::L("false");
    (void)doe::applyGeometryRowPrefix(variable, a);
    (void)doe::applyGeometryRowPrefix(variable, input(3));
    a.upperDefined = qt::L("");
    a.morphMethod = 17;
    const auto repeated = doe::applyGeometryRowPrefix(variable, a);
    REQUIRE(!repeated.appended && repeated.matchedRowIndex == 0 && repeated.tailRowIndex == 1);
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {1, 2}, {3, 4}));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Lower, {5, 6}, {7, 8}));
    REQUIRE(variable.files[0].upperCorrespondence.size() == 1);
    REQUIRE(variable.files[0].lowerCorrespondence.size() == 1);
    checkPair(variable.files[0].upperCorrespondence[0], 1, 2, 3, 4);
    checkPair(variable.files[0].lowerCorrespondence[0], 5, 6, 7, 8);
    CHECK(variable.files[1].upperCorrespondence.empty() && variable.files[1].lowerCorrespondence.empty());
    CHECK(!variable.files[0].useBefore && !variable.files[0].useAfter);
    CHECK(variable.files[1].useBefore && !variable.files[1].useAfter);
    CHECK_EQ(variable.geoType, 17);
    CHECK(variable.files[0].path == qt::L("nominal.GEO"));
    CHECK(variable.files[0].geoOperation == std::optional<int>(1));
    CHECK_EQ(variable.number, 7);
    CHECK_EQ(variable.numSamples, 3);
}

TEST_CASE(doe_geometry_correspondence_appends_repeated_pairs_and_keeps_sides_independent)
{
    doe::HeaderVariableRecord variable(3);
    (void)doe::applyGeometryRowPrefix(variable, input(1));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {1, 2}, {3, 4}));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Lower, {-1, -2}, {-3, -4}));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {1, 2}, {3, 4}));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {9, 10}, {11, 12}));
    REQUIRE(variable.files[0].upperCorrespondence.size() == 3);
    REQUIRE(variable.files[0].lowerCorrespondence.size() == 1);
    checkPair(variable.files[0].upperCorrespondence[0], 1, 2, 3, 4);
    checkPair(variable.files[0].upperCorrespondence[1], 1, 2, 3, 4);
    checkPair(variable.files[0].upperCorrespondence[2], 9, 10, 11, 12);
    checkPair(variable.files[0].lowerCorrespondence[0], -1, -2, -3, -4);
    // Native wrappers snapshot both point inputs before their append helper.
    const auto& existing = variable.files[0].upperCorrespondence[0];
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, existing.second, existing.first));
    REQUIRE(variable.files[0].upperCorrespondence.size() == 4);
    checkPair(variable.files[0].upperCorrespondence[3], 3, 4, 1, 2);
}

TEST_CASE(doe_geometry_correspondence_valid_index_does_not_require_dedup_keys)
{
    doe::HeaderVariableRecord variable(3);
    variable.files.resize(2); // Legacy rows have unknown keys, irrelevant to these setters.
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 1, Side::Lower, {1, 2}, {3, 4}));
    CHECK(variable.files[0].upperCorrespondence.empty() && variable.files[0].lowerCorrespondence.empty());
    REQUIRE(variable.files[1].lowerCorrespondence.size() == 1);
    checkPair(variable.files[1].lowerCorrespondence[0], 1, 2, 3, 4);
    CHECK(!variable.files[1].geoOperation && !variable.files[1].geoObject);
    CHECK(!variable.files[1].useBefore && !variable.files[1].useAfter);
    CHECK_EQ(variable.geoType, 100);
}

TEST_CASE(doe_geometry_correspondence_invalid_signed_index_is_silent_noop)
{
    doe::HeaderVariableRecord variable(3);
    CHECK(!doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {1, 2}, {3, 4}));
    CHECK(variable.files.empty());
    (void)doe::applyGeometryRowPrefix(variable, input(1));
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {1, 2}, {3, 4}));
    for (const int index : {-1, std::numeric_limits<int>::min(), 1, std::numeric_limits<int>::max()}) {
        CHECK(!doe::appendGeometryCorrespondencePair(variable, index, Side::Upper, {5, 6}, {7, 8}));
        CHECK(!doe::appendGeometryCorrespondencePair(variable, index, Side::Lower, {5, 6}, {7, 8}));
    }
    REQUIRE(variable.files.size() == 1);
    REQUIRE(variable.files[0].upperCorrespondence.size() == 1);
    CHECK(variable.files[0].lowerCorrespondence.empty());
    checkPair(variable.files[0].upperCorrespondence[0], 1, 2, 3, 4);
    CHECK_EQ(variable.geoType, 9);
}

TEST_CASE(doe_geometry_correspondence_copies_explicit_double_values_without_conversion)
{
    doe::HeaderVariableRecord variable(3);
    variable.files.resize(1);
    const double infinity = std::numeric_limits<double>::infinity();
    const double nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(doe::appendGeometryCorrespondencePair(variable, 0, Side::Upper, {-0.0, infinity}, {nan, -infinity}));
    const auto& pair = variable.files[0].upperCorrespondence[0];
    CHECK(pair.first.x == 0.0 && std::signbit(pair.first.x));
    CHECK(pair.first.y == infinity);
    CHECK(std::isnan(pair.second.x));
    CHECK(pair.second.y == -infinity);
}

TEST_CASE(doe_geometry_correspondence_does_not_admit_public_geometry)
{
    doe::Study study;
    bool stopped = false;
    try {
        (void)doe::readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\"><Geometry><UpperCorrespondence><PointPair>1, 2, 3, 4</PointPair></UpperCorrespondence></Geometry></DoeVariable></DoeVariables></sfDoeInputTable>", study);
    } catch (const Error& error) { CHECK_EQ(error.code, M2M_E_UNSUPPORTED); stopped = true; }
    CHECK(stopped);
    REQUIRE(study.headerVariableOwners.size() == 1);
    CHECK(study.headerVariableOwners[0]->files.empty());
    CHECK_EQ(study.headerVariableOwners[0]->geoType, 100);
    CHECK_EQ(study.headerVariableOwners[0]->index, -1);
}

int main() { return testkit::run_all(); }
