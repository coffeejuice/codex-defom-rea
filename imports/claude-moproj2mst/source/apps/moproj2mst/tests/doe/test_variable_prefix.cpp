// ctest-labels: m2m-doe
// Verified native prefix only: analysis/resume50-doe-variable-body-pe.md.
// No fresh original execution, delivered notification or full parse is claimed.
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/mst/writer.hpp"
#include "../../src/doe/variable_prefix.hpp"

using namespace m2m;
namespace {
const char* const kInput = R"(<DoeVariables><DoeVariable name="Transfer Time" numSamples="3" varId="1" dimension="3"><Keyword name="TLOC" unitType="0" parmType="double" parameter="-1" nominalValue="0.1"><NominalData><string>TLOC    	0.1</string></NominalData><KwOperations><KwOperation id="1"/></KwOperations></Keyword></DoeVariable></DoeVariables>)";

xml::NodePtr parse(const std::string& text)
{
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, text, &error, &line, &column));
    REQUIRE(document.root != nullptr);
    return document.root;
}

std::string replace(std::string text, const std::string& from, const std::string& to)
{
    const auto at = text.find(from);
    REQUIRE(at != std::string::npos);
    text.replace(at, from.size(), to);
    return text;
}

struct Rig {
    Project project;
    std::shared_ptr<doe::Study> study;
    Rig()
    {
        project.path = qt::L("C:/diagnostic/variable-prefix.moproj");
        project.doeIo = doe::makeRecordingStudyIo();
        project.subs.push_back(makeSubProject(project, 2, 1));
        project.activeSub = 0;
        study = project.subs[0]->ownedDoeStudy;
    }
};

std::string refused(doe::B121ScalarVariablePrefix& prefix, const xml::Node& variables)
{
    try { prefix.read(variables); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    return {};
}
}

TEST_CASE(b121_scalar_prefix_retains_native_state_before_notification)
{
    Rig rig;
    auto variables = parse(kInput);
    rig.study->samples.invalid.fill(false); // observes the new-variable dirty stores
    const auto reads = rig.project.doeIo->reads().size();
    const auto writes = rig.project.doeIo->textWrites().size();
    const auto directories = rig.project.doeIo->directories().size();
    const auto* notifications = rig.project.doeIo->observedSessionNotificationState();
    doe::B121ScalarVariablePrefix prefix(rig.study);
    const auto error = refused(prefix, *variables);
    CHECK(error.find("2signalSetOprHasDoe()") != std::string::npos);
    REQUIRE(rig.study->vars.size() == 1);
    REQUIRE(prefix.variable() != nullptr);
    const auto& variable = *prefix.variable();
    CHECK(rig.study->vars[0] == &variable);
    CHECK(variable.name == qt::L("Transfer Time"));
    CHECK_EQ(variable.numSamples, 3);
    CHECK_EQ(variable.number, 1);
    CHECK_EQ(variable.dimension, 3);
    CHECK_EQ(variable.geoType, 100);
    CHECK_EQ(variable.index, -1); // final vt+1e0 has not run
    CHECK(variable.link == nullptr);
    REQUIRE(prefix.selectedIndex().has_value());
    CHECK_EQ(*prefix.selectedIndex(), 0);
    for (const bool invalid : rig.study->samples.invalid) CHECK(invalid);
    CHECK(variable.ops == std::vector<int>({1}));
    REQUIRE(variable.keywords.size() == 1);
    const auto& keyword = variable.keywords[0];
    CHECK_EQ(keyword.keywordId, 8);
    CHECK_EQ(keyword.unitType, 0);
    CHECK_BITS(keyword.nominalValue, 0.1);
    CHECK_BITS(keyword.lowerValue, 0.0);
    CHECK_BITS(keyword.upperValue, 0.0);
    CHECK(!keyword.function && !keyword.enumeration && !keyword.lowerDefined && !keyword.upperDefined);
    CHECK_EQ(keyword.parameter, -1); // constructor value, not post-message setter
    CHECK(keyword.nominalText.isEmpty());
    REQUIRE(prefix.pendingSignal().has_value());
    CHECK(prefix.pendingSignal()->key == qt::L("2signalSetOprHasDoe()"));
    CHECK(prefix.pendingSignal()->value);
    CHECK_EQ(prefix.pendingSignal()->operation, 1);
    CHECK(prefix.pendingSignal()->collectedNominalText == qt::L("TLOC    \t0.1"));
    CHECK_EQ(rig.project.doeIo->reads().size(), reads);
    CHECK_EQ(rig.project.doeIo->textWrites().size(), writes); // no scalar temp.KWD write
    CHECK_EQ(rig.project.doeIo->directories().size(), directories);
    CHECK(rig.project.doeIo->observedSessionNotificationState() == notifications);
}

TEST_CASE(b121_scalar_prefix_does_not_apply_post_message_parameter)
{
    Rig rig;
    const auto variables = parse(replace(kInput, "parameter=\"-1\"", "parameter=\"7\""));
    doe::B121ScalarVariablePrefix prefix(rig.study);
    CHECK(!refused(prefix, *variables).empty());
    REQUIRE(prefix.pendingSignal().has_value());
    CHECK_EQ(prefix.pendingSignal()->parsedParameter, 7);
    REQUIRE(prefix.variable()->keywords.size() == 1);
    CHECK_EQ(prefix.variable()->keywords[0].parameter, -1);
}

TEST_CASE(b121_scalar_prefix_preserves_first_child_nominal_rule)
{
    Rig rig;
    const auto variables = parse(replace(kInput, "<NominalData><string>", "<NominalData><!--first--><string>"));
    doe::B121ScalarVariablePrefix prefix(rig.study);
    CHECK(!refused(prefix, *variables).empty());
    REQUIRE(prefix.pendingSignal().has_value());
    CHECK(prefix.pendingSignal()->collectedNominalText.isEmpty());
}

TEST_CASE(b121_scalar_prefix_retains_headers_before_unrecovered_keyword_type)
{
    for (const auto* type : {"function", "enum"}) {
        Rig rig;
        const auto variables = parse(replace(kInput, "parmType=\"double\"", std::string("parmType=\"") + type + "\""));
        doe::B121ScalarVariablePrefix prefix(rig.study);
        CHECK(!refused(prefix, *variables).empty());
        REQUIRE(rig.study->vars.size() == 1);
        CHECK_EQ(prefix.variable()->number, 1);
        CHECK(prefix.variable()->keywords.empty());
        CHECK(!prefix.pendingSignal().has_value());
        CHECK(rig.project.doeIo->textWrites().empty());
    }
}

TEST_CASE(b121_scalar_prefix_requires_actual_active_ownership_and_io)
{
    Rig rig;
    const auto variables = parse(kInput);
    doe::B121ScalarVariablePrefix prefix(rig.study);
    rig.project.subs[0]->doe = nullptr;
    CHECK(!refused(prefix, *variables).empty());
    CHECK(rig.study->vars.empty());
    rig.project.subs[0]->doe = rig.study.get();
    rig.project.doeIo = doe::makeRecordingStudyIo();
    CHECK(!refused(prefix, *variables).empty());
    CHECK(rig.study->vars.empty());
}

TEST_CASE(b121_scalar_prefix_guards_design_modes_and_retries)
{
    for (const int mode : {1, 2}) {
        Rig rig;
        rig.study->samples.mode = mode;
        const auto variables = parse(kInput);
        doe::B121ScalarVariablePrefix prefix(rig.study);
        CHECK(!refused(prefix, *variables).empty());
        CHECK(rig.study->vars.empty());
    }
    Rig rig;
    const auto variables = parse(kInput);
    doe::B121ScalarVariablePrefix prefix(rig.study);
    CHECK(!refused(prefix, *variables).empty());
    const auto* retained = rig.study->vars[0];
    CHECK(!refused(prefix, *variables).empty());
    REQUIRE(rig.study->vars.size() == 1);
    CHECK(rig.study->vars[0] == retained);
    CHECK_EQ(prefix.variable()->index, -1);
}

TEST_CASE(b121_public_input_table_keyword_stops_after_header_mutation)
{
    Rig rig;
    bool unsupported = false;
    try { (void)doe::readInputTable(std::string("<sfDoeInputTable>") + kInput + "</sfDoeInputTable>", *rig.study); }
    catch (const Error& error) { unsupported = error.code == M2M_E_UNSUPPORTED; }
    CHECK(unsupported);
    REQUIRE(rig.study->vars.size() == 1 && rig.study->headerVariableOwners.size() == 1);
    const auto& variable = *rig.study->headerVariableOwners[0];
    CHECK(rig.study->vars[0] == &variable);
    CHECK(variable.name == qt::L("Transfer Time"));
    CHECK_EQ(variable.number, 1);
    CHECK_EQ(variable.numSamples, 3);
    CHECK_EQ(variable.dimension, 3);
    CHECK_EQ(variable.index, -1); // the outer reindex is after the Keyword stop
    CHECK_EQ(rig.study->selectedVariableIndex, 0);
    for (const bool invalid : rig.study->samples.invalid) CHECK(invalid);
    CHECK(variable.link == nullptr);
    CHECK(rig.project.doeIo->textWrites().empty()); // Keyword state/notification work is not reached.
}

int main() { return testkit::run_all(); }
