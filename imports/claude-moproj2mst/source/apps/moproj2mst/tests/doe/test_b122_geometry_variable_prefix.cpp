// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/xml.hpp"
#include "../../src/doe/geometry_variable_prefix.hpp"

using namespace m2m;
namespace fs = std::filesystem;

namespace {
constexpr const char* kB122Input = "122_B_DOE_GEO/DoeStudies/DoeStudy1/Control/DoeInputTable.XML";

std::string readFixture()
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") / fs::path(kB122Input);
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("unable to open retained B122 fixture: " + path.string());
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

xml::Document parseFixture()
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    const auto bytes = readFixture();
    if (!xml::setContent(doc, bytes, &error, &line, &column))
        throw std::runtime_error("retained B122 XML parse failed at " + std::to_string(line) + ":" +
            std::to_string(column) + ": " + error.utf8());
    return doc;
}

xml::Node& child(xml::Node& parent, const char* tag)
{
    for (auto& node : parent.children)
        if (node && node->kind == xml::Node::Element && node->tag == qt::L(tag)) return *node;
    throw std::runtime_error(std::string("missing XML child: ") + tag);
}

std::pair<xml::Node&, xml::Node&> sections(xml::Document& doc)
{
    if (!doc.root) throw std::runtime_error("missing document root");
    return {child(*doc.root, "DoeVariables"), child(*doc.root, "Operations")};
}

doe::B122GeometryVariableRecord readRecord(xml::Document& doc)
{
    auto [variables, operations] = sections(doc);
    return doe::parseB122GeometryVariablePrefix(variables, operations);
}

bool refuses(xml::Document& doc)
{
    try { (void)readRecord(doc); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
} // namespace

TEST_CASE(b122_geometry_variable_prefix_reads_one_record_and_three_referenced_geo_files)
{
    auto doc = parseFixture();
    const auto record = readRecord(doc);
    CHECK(record.name == qt::L("Workpiece-Geometry"));
    CHECK(record.variableId == 1);
    CHECK(record.dimension == 3);
    CHECK(record.sampleCount == 3);
    CHECK(record.geoObject == 1);
    CHECK(record.geoOperation == 1);
    CHECK(record.morphMethod == 100);
    CHECK(!record.upperDefined && !record.lowerDefined);
    CHECK(record.nominalGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Nominal.GEO"));
    CHECK(record.upperGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Upper.GEO"));
    CHECK(record.lowerGeoFile == qt::L("Workpiece-Geometry_Workpiece_1_Lower.GEO"));
    CHECK(record.upperCorrespondencePresent && record.lowerCorrespondencePresent);
    CHECK(record.operationBindingId == 1);
    CHECK(record.operationBindingName == record.name);

    const auto control = m2m_test::configuredPath("M2M_ORACLE_B") /
        "122_B_DOE_GEO/DoeStudies/DoeStudy1/Control";
    CHECK(fs::file_size(control / record.nominalGeoFile.u16()) == 41410);
    CHECK(fs::file_size(control / record.upperGeoFile.u16()) == 41410);
    CHECK(fs::file_size(control / record.lowerGeoFile.u16()) == 41410);
}

TEST_CASE(b122_geometry_variable_prefix_refuses_unretained_record_shapes)
{
    {
        auto doc = parseFixture();
        auto& variables = child(*doc.root, "DoeVariables");
        auto variable = variables.childElements().front();
        auto duplicate = std::make_shared<xml::Node>(*variable);
        variables.children.push_back(std::move(duplicate));
        CHECK(refuses(doc));
    }
    {
        auto doc = parseFixture();
        auto& variables = child(*doc.root, "DoeVariables");
        auto& variable = child(variables, "DoeVariable");
        auto& geometry = child(variable, "Geometry");
        geometry.attrs.erase(std::remove_if(geometry.attrs.begin(), geometry.attrs.end(),
            [](const xml::Attr& attr) { return attr.name == qt::L("NominalGeoFile"); }), geometry.attrs.end());
        CHECK(refuses(doc));
    }
    {
        auto doc = parseFixture();
        auto& operations = child(*doc.root, "Operations");
        auto& operation = child(operations, "Operation");
        auto& binding = child(operation, "DoeOprVariable");
        xml::setAttribute(binding, qt::L("name"), qt::L("Different-Variable"));
        CHECK(refuses(doc));
    }
    {
        auto doc = parseFixture();
        auto& variables = child(*doc.root, "DoeVariables");
        auto& variable = child(variables, "DoeVariable");
        auto& geometry = child(variable, "Geometry");
        geometry.children.push_back(xml::createElement(qt::L("Unexpected")));
        CHECK(refuses(doc));
    }
}

int main() { return testkit::run_all(); }
