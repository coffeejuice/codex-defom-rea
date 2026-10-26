// ctest-labels: m2m-doe
// Candidate-only B122 geometry-row writer projection.
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "../../src/doe/geometry_variable_binding.hpp"
#include "../../src/doe/geometry_variable_prefix.hpp"
#include "../../src/doe/geometry_variable_writer.hpp"

using namespace m2m;
namespace {
std::string retainedInput()
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") /
        "122_B_DOE_GEO" / "DoeStudies" / "DoeStudy1" / "Control" /
        "DoeInputTable.XML";
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("retained B122 DoeInputTable.XML is unavailable");
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::unique_ptr<doe::Study> retainedStudy()
{
    xml::Document document;
    String error;
    int line = 0;
    int column = 0;
    const auto bytes = retainedInput();
    if (!xml::setContent(document, bytes, &error, &line, &column) || !document.root)
        throw std::runtime_error("retained B122 XML parse failed: " + error.utf8());
    const auto variables = document.root->childElements(qt::L("DoeVariables"));
    const auto operations = document.root->childElements(qt::L("Operations"));
    if (variables.size() != 1 || operations.size() != 1)
        throw std::runtime_error("retained B122 XML lacks unique DOE sections");
    auto record = doe::parseB122GeometryVariablePrefix(*variables.front(), *operations.front());
    auto study = std::make_unique<doe::Study>();
    (void)doe::bindB122GeometryVariablePrefix(*study, record);
    return study;
}

bool refuses(const doe::Study& study, engine::FileEffects& effects)
{
    try {
        (void)doe::writeB122GeometryVariablePrefix(qt::L("C:/candidate/DoeInputTable.XML"),
                                                   study, effects);
    } catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED;
    }
    return false;
}
} // namespace

TEST_CASE(b122_geometry_writer_prefix_matches_retained_817_byte_fixture)
{
    const auto study = retainedStudy();
    const auto expected = retainedInput();
    REQUIRE(expected.size() == 817u);

    engine::FileEffects effects;
    const auto path = qt::L("C:/candidate/DoeInputTable.XML");
    REQUIRE(doe::writeB122GeometryVariablePrefix(path, *study, effects));
    REQUIRE(effects.writes.size() == 1u);
    CHECK(effects.writes[0].path == path);
    CHECK(effects.writes[0].bytes == expected); // binary fixture bytes; no normalization
    CHECK(effects.deletes.empty());
    CHECK(effects.mkdirs.empty());
    CHECK(effects.renames.empty());
    CHECK(!effects.nativeDirectories);
    CHECK(!effects.nativeRemovals);
    CHECK(!effects.nativeTaskKeys);
    CHECK(!effects.nativeSimCtrl);
}

TEST_CASE(b122_geometry_writer_prefix_rejects_changed_record_before_effect)
{
    auto study = retainedStudy();
    study->vars[0]->geometry->nominalGeoFile = qt::L("unqualified.GEO");
    engine::FileEffects effects;
    CHECK(refuses(*study, effects));
    CHECK(effects.writes.empty());
    CHECK(effects.deletes.empty());
    CHECK(effects.mkdirs.empty());

    auto changedHeader = retainedStudy();
    changedHeader->roiType = 1;
    engine::FileEffects headerEffects;
    CHECK(refuses(*changedHeader, headerEffects));
    CHECK(headerEffects.writes.empty());
}

int main() { return testkit::run_all(); }
