// ctest-labels: m2m-doe
// Adapter retention of the source-qualified interrupted prefix, not native Close
// or completed SetOprHasDoe delivery/Type2 parsing.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/variable_prefix.hpp"

using namespace m2m;

namespace {
const char* const kInput = R"(<DoeVariables><DoeVariable name="Transfer Time" numSamples="3" varId="1" dimension="3"><Keyword name="TLOC" unitType="0" parmType="double" parameter="-1" nominalValue="0.1"><NominalData><string>TLOC    	0.1</string></NominalData><KwOperations><KwOperation id="1"/></KwOperations></Keyword></DoeVariable></DoeVariables>)";

xml::NodePtr parse(const std::string& text = kInput)
{
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, text, &error, &line, &column));
    REQUIRE(document.root != nullptr);
    return document.root;
}

struct Rig {
    engine::DiagramEffectDriver diagram;
    engine::ConversionSession session{diagram};
    Rig()
    {
        auto& project = session.project();
        initProject(project, qt::L("C:/PROBLEM"), qt::L("retained_scalar"));
        project.doeIo = doe::makeRecordingStudyIo();
        project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        project.doeIo->bindModelManager(session.modelManagerOwner());
        project.subs.push_back(makeSubProject(project, 2, 1));
        project.activeSub = 1;
    }
    Project& project() { return session.project(); }
    SubProject& sub() { return *project().subs.at(1); }
};

template<class Work> std::string refusal(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    return {};
}

std::string read(Rig& rig, const xml::Node& variables)
{
    return refusal([&] {
        doe::readOwnedB121ScalarPrefix(rig.session, rig.project(), rig.sub(), variables);
    });
}

std::string variableRow()
{
    const std::string input(kInput);
    const std::string begin = "<DoeVariables>", end = "</DoeVariables>";
    return input.substr(begin.size(), input.size() - begin.size() - end.size());
}
} // namespace

TEST_CASE(retained_scalar_prefix_survives_stack_driver_guard_with_exact_pending_state)
{
    Rig rig;
    const auto study = rig.sub().ownedDoeStudy;
    study->samples.invalid.fill(false);
    const auto io = rig.project().doeIo;
    const auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true; // This slice never enters the dispatcher.
    const auto directories = io->directories().size();
    const auto reads = io->reads().size();
    const auto writes = io->textWrites().size();
    const auto notifications = notifier->attempts.size();
    const auto variables = parse();
    CHECK(read(rig, *variables).find("2signalSetOprHasDoe()") != std::string::npos);

    const auto state = study->retainedScalarPrefix();
    REQUIRE(state != nullptr);
    REQUIRE(state->started());
    REQUIRE(state->variable() != nullptr);
    REQUIRE(study->vars.size() == 1);
    CHECK(study->vars[0] == state->variable());
    const auto& variable = *state->variable();
    CHECK(variable.name == qt::L("Transfer Time"));
    CHECK_EQ(variable.numSamples, 3);
    CHECK_EQ(variable.number, 1);
    CHECK_EQ(variable.dimension, 3);
    CHECK_EQ(variable.geoType, 100);
    CHECK_EQ(variable.index, -1); // No query/reindex was invoked.
    CHECK(variable.link == nullptr && !variable.scalar);
    REQUIRE(state->selectedIndex().has_value());
    CHECK_EQ(*state->selectedIndex(), 0);
    for (const bool invalid : study->samples.invalid) CHECK(invalid);
    for (const auto& table : study->samples.tables) CHECK(table.empty());
    CHECK(variable.ops == std::vector<int>({1}));
    REQUIRE(variable.keywords.size() == 1);
    const auto& keyword = variable.keywords[0];
    CHECK_EQ(keyword.keywordId, 8);
    CHECK_EQ(keyword.unitType, 0);
    CHECK_BITS(keyword.nominalValue, 0.1);
    CHECK_EQ(keyword.parameter, -1);
    CHECK(keyword.nominalText.isEmpty());
    REQUIRE(state->pendingSignal().has_value());
    CHECK_EQ(state->pendingSignal()->operation, 1);
    CHECK(state->pendingSignal()->value);
    CHECK(state->pendingSignal()->collectedNominalText == qt::L("TLOC    \t0.1"));
    CHECK_EQ(io->directories().size(), directories);
    CHECK_EQ(io->reads().size(), reads);
    CHECK_EQ(io->textWrites().size(), writes);
    CHECK(io->xmlWrites().empty());
    CHECK_EQ(notifier->attempts.size(), notifications);
    CHECK(notifier->flag90);
    CHECK(!rig.sub().loaded);
    CHECK(study->scalarPrefixVariableOwners.empty());
}

TEST_CASE(retained_scalar_prefix_retry_preserves_the_original_allocation_and_alias)
{
    Rig rig;
    const auto variables = parse();
    CHECK(!read(rig, *variables).empty());
    const auto study = rig.sub().ownedDoeStudy;
    const auto state = study->retainedScalarPrefix();
    REQUIRE(state != nullptr);
    const auto* variable = state->variable();
    CHECK(read(rig, *variables).find("retry") != std::string::npos);
    CHECK(study->retainedScalarPrefix() == state);
    REQUIRE(study->vars.size() == 1);
    CHECK(study->vars[0] == variable);
    REQUIRE(variable->keywords.size() == 1);
    CHECK(state->pendingSignal().has_value());
}

TEST_CASE(retained_scalar_prefix_owner_refusals_precede_state_and_model_mutation)
{
    Rig rig;
    const auto variables = parse();
    const auto study = rig.sub().ownedDoeStudy;
    Rig foreign;
    CHECK(!refusal([&] {
        doe::readOwnedB121ScalarPrefix(foreign.session, rig.project(), rig.sub(), *variables);
    }).empty());
    rig.project().activeSub = 0;
    CHECK(!read(rig, *variables).empty());
    rig.project().activeSub = 1;
    rig.sub().doe = nullptr;
    CHECK(!read(rig, *variables).empty());
    rig.sub().doe = study.get();
    const auto io = rig.project().doeIo;
    rig.project().doeIo = doe::makeRecordingStudyIo(); // Intentionally unbound.
    CHECK(!read(rig, *variables).empty());
    rig.project().doeIo = io;
    CHECK(!study->retainedScalarPrefix());
    CHECK(study->vars.empty());
    CHECK(study->scalarPrefixVariableOwners.empty());
}

TEST_CASE(retained_scalar_prefix_scope_refusal_does_not_publish_an_owner)
{
    Rig rig;
    const auto study = rig.sub().ownedDoeStudy;
    const auto wrongRoot = parse("<Operations/>");
    CHECK(!read(rig, *wrongRoot).empty());
    const auto variables = parse();
    study->samples.mode = 1;
    CHECK(!read(rig, *variables).empty());
    study->samples.mode = 0;
    study->samples.tables[0] = {{7}};
    CHECK(!read(rig, *variables).empty());
    CHECK(!study->retainedScalarPrefix());
    CHECK(study->vars.empty());
    CHECK(study->samples.tables[0] == doe::SampleTable({{7}}));
}

TEST_CASE(retained_scalar_prefix_preappend_admission_refuses_without_poisoning_fresh_read)
{
    const std::vector<std::string> rejected{
        "<DoeVariables/>",
        "<DoeVariables><!--comment-->text<Unknown/></DoeVariables>",
        "<DoeVariables><DoeVariable numSamples='3' varId='1' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='' numSamples='3' varId='1' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='Transfer Time' varId='1' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='Transfer Time' numSamples='0' varId='1' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='Transfer Time' numSamples='bad' varId='1' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='Transfer Time' numSamples='3' dimension='3'/></DoeVariables>",
        "<DoeVariables><DoeVariable name='Transfer Time' numSamples='3' varId='1'/></DoeVariables>"
    };
    for (const auto& input : rejected) {
        Rig rig;
        const auto study = rig.sub().ownedDoeStudy;
        study->samples.invalid.fill(false);
        const auto io = rig.project().doeIo;
        const auto notifier = rig.session.sessionNotificationStateOwner();
        notifier->flag90 = true;
        const auto directories = io->directories().size();
        const auto reads = io->reads().size();
        const auto writes = io->textWrites().size();
        const auto notifications = notifier->attempts.size();
        const auto variables = parse(input);
        CHECK(!read(rig, *variables).empty());
        CHECK(!study->retainedScalarPrefix());
        CHECK(study->vars.empty());
        CHECK(study->scalarPrefixVariableOwners.empty());
        CHECK_EQ(study->samples.mode, 0);
        for (const bool invalid : study->samples.invalid) CHECK(!invalid);
        for (const auto& table : study->samples.tables) CHECK(table.empty());
        CHECK_EQ(io->directories().size(), directories);
        CHECK_EQ(io->reads().size(), reads);
        CHECK_EQ(io->textWrites().size(), writes);
        CHECK(io->xmlWrites().empty());
        CHECK_EQ(notifier->attempts.size(), notifications);
        CHECK(notifier->flag90);

        const auto valid = parse();
        CHECK(read(rig, *valid).find("2signalSetOprHasDoe()") != std::string::npos);
        const auto state = study->retainedScalarPrefix();
        REQUIRE(state != nullptr && state->variable() != nullptr);
        REQUIRE(study->vars.size() == 1);
        CHECK(study->vars[0] == state->variable());
        CHECK(state->pendingSignal().has_value());
        for (const bool invalid : study->samples.invalid) CHECK(invalid);
    }
}

TEST_CASE(retained_scalar_prefix_preflight_selects_only_the_first_matching_row)
{
    {
        Rig rig;
        const auto variables = parse("<DoeVariables><!--before--><Unknown/>" +
            variableRow() + "<DoeVariable/></DoeVariables>");
        CHECK(read(rig, *variables).find("2signalSetOprHasDoe()") != std::string::npos);
        const auto state = rig.sub().ownedDoeStudy->retainedScalarPrefix();
        REQUIRE(state != nullptr && state->variable() != nullptr);
        CHECK(state->variable()->name == qt::L("Transfer Time"));
        CHECK(state->pendingSignal().has_value()); // Later malformed row was not preflighted.
    }
    {
        Rig rig;
        const auto variables = parse("<DoeVariables><Unknown/><DoeVariable name='' "
            "numSamples='3' varId='1' dimension='3'/>" + variableRow() + "</DoeVariables>");
        CHECK(read(rig, *variables).find("headers") != std::string::npos);
        CHECK(!rig.sub().ownedDoeStudy->retainedScalarPrefix());
        CHECK(rig.sub().ownedDoeStudy->vars.empty()); // Later valid row was not selected.
        const auto valid = parse();
        CHECK(read(rig, *valid).find("2signalSetOprHasDoe()") != std::string::npos);
    }
}

TEST_CASE(retained_scalar_prefix_preflight_does_not_hoist_postappend_guards)
{
    for (const std::string& attribute : {std::string("dimension=\"2\""),
                                       std::string("dimension=\"bad\"")}) {
        Rig rig;
        rig.sub().ownedDoeStudy->samples.invalid.fill(false);
        auto input = std::string(kInput);
        const auto at = input.find("dimension=\"3\"");
        REQUIRE(at != std::string::npos);
        input.replace(at, std::string("dimension=\"3\"").size(), attribute);
        const auto variables = parse(input);
        CHECK(read(rig, *variables).find("non-3D") != std::string::npos);
        const auto state = rig.sub().ownedDoeStudy->retainedScalarPrefix();
        REQUIRE(state != nullptr && state->variable() != nullptr);
        REQUIRE(rig.sub().ownedDoeStudy->vars.size() == 1);
        CHECK(rig.sub().ownedDoeStudy->vars[0] == state->variable());
        CHECK(state->variable()->name == qt::L("Transfer Time"));
        CHECK_EQ(state->variable()->index, -1);
        REQUIRE(state->selectedIndex().has_value());
        CHECK_EQ(*state->selectedIndex(), 0);
        for (const bool invalid : rig.sub().ownedDoeStudy->samples.invalid) CHECK(invalid);
        CHECK(!state->pendingSignal());
    }
    {
        Rig rig;
        auto input = std::string(kInput);
        const auto at = input.find("dimension=\"3\"");
        REQUIRE(at != std::string::npos);
        input.insert(at, "LinkTo=\"Other\" ");
        const auto variables = parse(input);
        CHECK(read(rig, *variables).find("LinkTo") != std::string::npos);
        const auto state = rig.sub().ownedDoeStudy->retainedScalarPrefix();
        REQUIRE(state != nullptr && state->variable() != nullptr);
        REQUIRE(rig.sub().ownedDoeStudy->vars.size() == 1);
        CHECK_EQ(state->variable()->number, 1);
        CHECK_EQ(state->variable()->dimension, 3);
        CHECK(state->variable()->keywords.empty());
        CHECK(!state->pendingSignal());
    }
    {
        Rig rig;
        auto input = std::string(kInput);
        const auto at = input.find("name=\"TLOC\"");
        REQUIRE(at != std::string::npos);
        input.replace(at, std::string("name=\"TLOC\"").size(), "name=\"Other\"");
        const auto variables = parse(input);
        CHECK(read(rig, *variables).find("keyword prefix") != std::string::npos);
        const auto state = rig.sub().ownedDoeStudy->retainedScalarPrefix();
        REQUIRE(state != nullptr && state->variable() != nullptr);
        REQUIRE(rig.sub().ownedDoeStudy->vars.size() == 1);
        CHECK_EQ(state->variable()->number, 1);
        CHECK(state->variable()->keywords.empty());
        CHECK(!state->pendingSignal());
    }
}

TEST_CASE(retained_scalar_prefix_earlier_guard_retains_appended_but_unfinished_record)
{
    Rig rig;
    auto input = std::string(kInput);
    const auto at = input.find("varId=\"1\"");
    REQUIRE(at != std::string::npos);
    input.replace(at, std::string("varId=\"1\"").size(), "varId=\"0\"");
    const auto variables = parse(input);
    CHECK(read(rig, *variables).find("nonpositive") != std::string::npos);
    const auto state = rig.sub().ownedDoeStudy->retainedScalarPrefix();
    REQUIRE(state != nullptr && state->variable() != nullptr);
    REQUIRE(rig.sub().ownedDoeStudy->vars.size() == 1);
    CHECK(rig.sub().ownedDoeStudy->vars[0] == state->variable());
    CHECK_EQ(state->variable()->number, 0);
    CHECK(state->variable()->keywords.empty());
    CHECK(!state->pendingSignal());
}

TEST_CASE(retained_scalar_prefix_has_no_study_cycle_and_diagnostics_can_outlive_storage)
{
    Rig rig;
    const auto variables = parse();
    CHECK(!read(rig, *variables).empty());
    std::weak_ptr<doe::Study> weakStudy = rig.sub().ownedDoeStudy;
    auto observation = rig.sub().ownedDoeStudy->retainedScalarPrefix();
    std::weak_ptr<const doe::B121ScalarPrefixState> weakState = observation;
    rig.session.releaseProjectAfterCloseEffects(); // Adapter storage release only.
    CHECK(weakStudy.expired());
    REQUIRE(observation != nullptr && observation->variable() != nullptr);
    CHECK(observation->variable()->name == qt::L("Transfer Time"));
    CHECK(observation->pendingSignal().has_value());
    observation.reset();
    CHECK(weakState.expired());
}

TEST_CASE(retained_scalar_prefix_is_reclaimed_with_sub_when_no_observer_escapes)
{
    Rig rig;
    const auto variables = parse();
    CHECK(!read(rig, *variables).empty());
    std::weak_ptr<doe::Study> weakStudy = rig.sub().ownedDoeStudy;
    std::weak_ptr<const doe::B121ScalarPrefixState> weakState =
        rig.sub().ownedDoeStudy->retainedScalarPrefix();
    rig.project().subs.clear();
    CHECK(weakStudy.expired());
    CHECK(weakState.expired());
}

TEST_CASE(retained_scalar_prefix_preserves_detached_driver_alias_cleanup_and_public_guards)
{
    Rig rig;
    const auto variables = parse();
    const auto study = rig.sub().ownedDoeStudy;
    {
        doe::B121ScalarVariablePrefix detached(study);
        CHECK(!refusal([&] { detached.read(*variables); }).empty());
        REQUIRE(study->vars.size() == 1);
        CHECK(!study->retainedScalarPrefix());
    }
    CHECK(study->vars.empty());
    engine::FileEffects effects;
    CHECK(!read(rig, *variables).empty());
    CHECK(!refusal([&] {
        (void)doe::saveInputTable(*study, qt::L("C:/PROBLEM/DoeStudies"), effects);
    }).empty());
    CHECK(effects.writes.empty());
    CHECK(!refusal([&] {
        (void)doe::readInputTable("<sfDoeInputTable><DoeVariables><DoeVariable/>"
                                "</DoeVariables></sfDoeInputTable>", *study);
    }).empty());
}

int main() { return testkit::run_all(); }
