// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/xml.hpp"
#include "../../src/doe/geometry_path_projection.hpp"

using namespace m2m;
namespace {
xml::NodePtr geometry()
{
    auto node = xml::createElement(qt::L("Geometry"));
    xml::setAttribute(*node, qt::L("UpperGeoFile"), qt::L("upper.GEO"));
    xml::setAttribute(*node, qt::L("LowerGeoFile"), qt::L("lower.GEO"));
    xml::setAttribute(*node, qt::L("NominalGeoFile"), qt::L("nominal.GEO"));
    return node;
}
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
}

TEST_CASE(doe_geometry_path_values_preserve_drive_unc_and_native_parent_rules)
{
    const std::vector<std::pair<qt::String, qt::String>> cases = {
        {qt::L("C:\\Problem\\DoeStudies\\S\\Control\\DoeInputTable.XML"), qt::L("C:/Problem/DoeStudies/S/Control")},
        {qt::L("c:/DoeInputTable.XML"), qt::L("c:/")},
        {qt::L("\\\\server\\share\\Control\\DoeInputTable.XML"), qt::L("//server/share/Control")},
        {qt::L("C:/a//b/../Control/DoeInputTable.XML"), qt::L("C:/a//b/../Control")},
        {qt::L("C:/a//DoeInputTable.XML"), qt::L("C:/a/")}
    };
    const auto node = geometry();
    for (const auto& test : cases) {
        const auto values = doe::projectGeometryPathValues({test.first}, *node);
        CHECK(values.sourceFileName == test.first);
        CHECK(values.parentDirectory == test.second);
        CHECK(values.upperGeoFile == test.second + qt::L("/upper.GEO"));
        CHECK(values.lowerGeoFile == test.second + qt::L("/lower.GEO"));
        CHECK(values.nominalGeoFile == test.second + qt::L("/nominal.GEO"));
    }
}

TEST_CASE(doe_geometry_path_values_join_raw_attributes_without_absolute_override_or_cleaning)
{
    auto node = geometry();
    xml::setAttribute(*node, qt::L("UpperGeoFile"), qt::L("../up//upper.GEO"));
    xml::setAttribute(*node, qt::L("LowerGeoFile"), qt::L("D:\\other\\lower.GEO"));
    xml::setAttribute(*node, qt::L("NominalGeoFile"), qt::L("/root/nominal.GEO"));
    const auto values = doe::projectGeometryPathValues({qt::L("C:/Control/DoeInputTable.XML")}, *node);
    CHECK(values.upperGeoFile == qt::L("C:/Control/../up//upper.GEO"));
    CHECK(values.lowerGeoFile == qt::L("C:/Control/D:\\other\\lower.GEO"));
    CHECK(values.nominalGeoFile == qt::L("C:/Control//root/nominal.GEO"));
}

TEST_CASE(doe_geometry_path_values_missing_and_empty_attributes_keep_literal_slash)
{
    auto node = xml::createElement(qt::L("Geometry"));
    xml::setAttribute(*node, qt::L("UpperGeoFile"), qt::L(""));
    const auto values = doe::projectGeometryPathValues({qt::L("C:/DoeInputTable.XML")}, *node);
    CHECK(values.parentDirectory == qt::L("C:/"));
    CHECK(values.upperGeoFile == qt::L("C://"));
    CHECK(values.lowerGeoFile == qt::L("C://"));
    CHECK(values.nominalGeoFile == qt::L("C://"));
    CHECK(!values.upperGeoFile.isNull() && !values.lowerGeoFile.isNull() && !values.nominalGeoFile.isNull());
}

TEST_CASE(doe_geometry_path_values_refuse_unqualified_filename_and_element_profiles)
{
    const auto node = geometry();
    const std::vector<qt::String> paths = {
        qt::String(), qt::L(""), qt::L("DoeInputTable.XML"), qt::L("C:DoeInputTable.XML"),
        qt::L("prefix:DoeInputTable.XML"), qt::L(":/resource/input.XML"), qt::L("/root/input.XML"),
        qt::L("C:/Control/"), qt::L("//server/share"), qt::L("//server//input.XML"),
        qt::L("//?/C:/Control/input.XML"), qt::String(std::u16string(u"C:/in\0put.XML", 13))
    };
    for (const auto& path : paths)
        CHECK(!refused([&] { (void)doe::projectGeometryPathValues({path}, *node); }).empty());
    auto wrongCase = xml::createElement(qt::L("geometry"));
    CHECK(!refused([&] { (void)doe::projectGeometryPathValues({qt::L("C:/input.XML")}, *wrongCase); }).empty());
}

TEST_CASE(doe_geometry_path_values_bound_signed16_separator_length)
{
    const auto node = geometry();
    std::u16string valid = u"C:/" + std::u16string(32758, u'a') + u"/x.XML";
    REQUIRE(valid.size() == 32767);
    const auto values = doe::projectGeometryPathValues({qt::String(valid)}, *node);
    CHECK(values.parentDirectory == qt::String(valid.substr(0, valid.size() - 6)));
    valid.insert(3, 1, u'a');
    REQUIRE(valid.size() == 32768);
    CHECK(!refused([&] { (void)doe::projectGeometryPathValues({qt::String(valid)}, *node); }).empty());
}

TEST_CASE(doe_geometry_path_values_do_not_admit_public_geometry_or_mutate_study)
{
    const auto node = geometry();
    const doe::InputTableSource source{qt::L("C:/Control/DoeInputTable.XML")};
    const auto values = doe::projectGeometryPathValues(source, *node);
    CHECK(values.nominalGeoFile == qt::L("C:/Control/nominal.GEO"));
    const std::string input = "<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" numSamples=\"3\" "
        "varId=\"7\" dimension=\"3\"><Geometry/></DoeVariable></DoeVariables></sfDoeInputTable>";
    doe::Study named, bytesOnly;
    const auto expected = refused([&] { (void)doe::readInputTable(input, bytesOnly); });
    CHECK(refused([&] { (void)doe::readInputTableFromSource(input, named, source); }) == expected);
    CHECK(expected.find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(named.vars.size() == 1 && named.headerVariableOwners.size() == 1);
    CHECK(named.vars[0]->files.empty() && !named.vars[0]->geometry);
    CHECK_EQ(named.vars[0]->index, -1);
}

int main() { return testkit::run_all(); }
