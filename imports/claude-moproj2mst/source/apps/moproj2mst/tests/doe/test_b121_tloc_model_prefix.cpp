// ctest-labels: m2m-doe
// Candidate-only model projection of the retained B121 TLOC input-table row.
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/doe_study.hpp"
#include "m2m/xml.hpp"

using namespace m2m;
namespace {
std::string retainedInput()
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") /
        "121_B_DOE_VAR" / "DoeStudies" / "DoeStudy1" / "Control" /
        "DoeInputTable.XML";
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("retained B121 DoeInputTable.XML is unavailable");
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

const std::string kInput =
    "<!DOCTYPE DoeInputTableFile>\n"
    "<sfDoeInputTable Version=\"1\">\n"
    " <DoeCommonMeshPath path=\"\"/>\n"
    " <DoeCommonMeshOrigName originalName=\"\"/>\n"
    " <DoeDoPositioning bool=\"false\"/>\n"
    " <DoeRoiType type=\"0\"/>\n"
    " <DoeVariables>\n"
    "  <DoeVariable numSamples=\"3\" varId=\"1\" dimension=\"3\" name=\"Transfer Time\">\n"
    "   <Keyword nominalValue=\"0.1\" unitType=\"0\" name=\"TLOC\" parmType=\"double\" parameter=\"-1\">\n"
    "    <NominalData>\n"
    "     <string>TLOC    \t0.1</string>\n"
    "    </NominalData>\n"
    "    <KwOperations>\n"
    "     <KwOperation id=\"1\"/>\n"
    "    </KwOperations>\n"
    "   </Keyword>\n"
    "  </DoeVariable>\n"
    " </DoeVariables>\n"
    " <Operations>\n"
    "  <Operation id=\"1\">\n"
    "   <DoeOprVariable name=\"Transfer Time\"/>\n"
    "  </Operation>\n"
    " </Operations>\n"
    "</sfDoeInputTable>\n";

xml::NodePtr parseInput()
{
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, retainedInput(), &error, &line, &column));
    REQUIRE(document.root != nullptr);
    return document.root;
}
}

TEST_CASE(b121_tloc_model_prefix_reads_scalar_fields_and_both_operation_maps)
{
    doe::Study study;
    const auto input = parseInput();
    REQUIRE(doe::readB121TlocModelPrefix(*input, study));
    REQUIRE(study.scalarPrefixVariableOwners.size() == 1u);
    REQUIRE(study.count() == 1);
    auto* variable = study.varAt(0);
    REQUIRE(variable != nullptr);
    CHECK(variable->name == qt::L("Transfer Time"));
    CHECK_EQ(variable->index, -1); // the native post-notification reindex is outside this prefix
    CHECK_EQ(variable->number, 1);
    CHECK_EQ(variable->geoType, 100);
    CHECK(variable->ops == std::vector<int>({1})); // top-level Operations mapping
    REQUIRE(variable->scalar.has_value());
    const auto& scalar = *variable->scalar;
    CHECK_EQ(scalar.id, 1);
    CHECK_EQ(scalar.dimension, 3);
    CHECK_EQ(scalar.sampleCount, 3);
    CHECK(scalar.keyword.name == qt::L("TLOC"));
    CHECK(scalar.keyword.parameterType == qt::L("double"));
    CHECK_EQ(scalar.keyword.unitType, 0);
    CHECK_EQ(scalar.keyword.parameter, -1);
    CHECK_BITS(scalar.keyword.nominalValue, 0.1);
    CHECK(scalar.keyword.nominalDataText == qt::L("TLOC    \t0.1"));
    CHECK(scalar.keyword.operationIds == std::vector<int>({1})); // nested KwOperations mapping
    CHECK(study.vars[0] == study.scalarPrefixVariableOwners[0].get());
}

TEST_CASE(b121_tloc_model_prefix_queries_operation_binding_without_public_reader)
{
    doe::Study study;
    const auto input = parseInput();
    REQUIRE(doe::readB121TlocModelPrefix(*input, study));
    const auto matching = study.varsForOperation(1);
    REQUIRE(matching.size() == 1u);
    CHECK(matching[0] == study.varAt(0));
    CHECK_EQ(matching[0]->index, 0); // the existing model query reindexes the visible list
    CHECK(study.varsForOperation(2).empty());
}

TEST_CASE(b121_tloc_model_prefix_writes_exact_retained_schema_bytes)
{
    doe::Study study;
    const auto input = parseInput();
    REQUIRE(doe::readB121TlocModelPrefix(*input, study));
    engine::FileEffects effects;
    REQUIRE(doe::writeB121TlocModelPrefix(qt::L("C:/candidate/DoeInputTable.XML"), study, effects));
    REQUIRE(effects.writes.size() == 1u);
    CHECK(effects.writes[0].path == qt::L("C:/candidate/DoeInputTable.XML"));
    const std::string expected = retainedInput();
    REQUIRE(expected.size() == 707u);
    CHECK(effects.writes[0].bytes == expected); // direct retained bytes; no normalization

    xml::Document roundtrip;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(roundtrip, effects.writes[0].bytes, &error, &line, &column));
    doe::Study reloaded;
    REQUIRE(doe::readB121TlocModelPrefix(*roundtrip.root, reloaded));
    REQUIRE(reloaded.count() == 1);
    REQUIRE(reloaded.varAt(0)->scalar.has_value());
    CHECK(reloaded.varAt(0)->scalar->keyword.operationIds == std::vector<int>({1}));
    CHECK(reloaded.varAt(0)->ops == std::vector<int>({1}));
}

TEST_CASE(b121_tloc_model_prefix_rejects_mapping_mismatch_before_model_mutation)
{
    const std::string mismatch = [] {
        std::string text = kInput;
        const auto at = text.find("<DoeOprVariable name=\"Transfer Time\"/>");
        REQUIRE(at != std::string::npos);
        text.replace(at, std::string("<DoeOprVariable name=\"Transfer Time\"/>").size(),
                     "<DoeOprVariable name=\"Other\"/>");
        return text;
    }();
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, mismatch, &error, &line, &column));
    REQUIRE(document.root != nullptr);
    doe::Study study;
    bool refused = false;
    try { (void)doe::readB121TlocModelPrefix(*document.root, study); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK(study.vars.empty());
    CHECK(study.scalarPrefixVariableOwners.empty());
}

TEST_CASE(b121_public_input_table_reader_retains_variable_prefix_before_keyword_guard)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    bool refused = false;
    try { (void)doe::readInputTable(kInput, study); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    REQUIRE(study.vars.size() == 1);
    REQUIRE(study.headerVariableOwners.size() == 1);
    CHECK(study.vars.front() == study.headerVariableOwners.front().get());
    CHECK(study.vars.front()->name == qt::L("Transfer Time"));
    CHECK_EQ(study.vars.front()->number, 1);
    CHECK_EQ(study.headerVariableOwners.front()->numSamples, 3);
    CHECK_EQ(study.headerVariableOwners.front()->dimension, 3);
    CHECK_EQ(study.vars.front()->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.scalarPrefixVariableOwners.empty());
}

TEST_CASE(b121_public_input_table_writer_keeps_nonempty_variable_guard)
{
    doe::Study study;
    const auto input = parseInput();
    REQUIRE(doe::readB121TlocModelPrefix(*input, study));
    engine::FileEffects effects;
    bool refused = false;
    try { (void)doe::writeInputTable(qt::L("C:/candidate/DoeInputTable.XML"), study, effects); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK(effects.writes.empty());
}

TEST_CASE(b121_fixture_writer_rejects_nonfixture_header_before_effect)
{
    doe::Study study;
    const auto input = parseInput();
    REQUIRE(doe::readB121TlocModelPrefix(*input, study));
    study.roiType = 1;
    engine::FileEffects effects;
    bool refused = false;
    try { (void)doe::writeB121TlocModelPrefix(qt::L("C:/candidate/DoeInputTable.XML"), study, effects); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK(effects.writes.empty());
}

int main() { return testkit::run_all(); }
