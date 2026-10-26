// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"

using namespace m2m;
namespace {
std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }
std::string row(const std::string& name = "A", const std::string& id = "7",
                const std::string& body = "", const std::string& sampleAttribute = " numSamples=\"0\"")
{ return "<DoeVariable name=\"" + name + "\" varId=\"" + id + "\" dimension=\"3\"" + sampleAttribute + ">" + body + "</DoeVariable>"; }
std::string seedRows()
{ return row("A", "7", "", " numSamples=\"3\"") + row("B", "8", "", " numSamples=\"5\""); }
bool readRows(doe::Study& study, const std::string& rows)
{ return doe::readInputTable(table("<DoeVariables>" + rows + "</DoeVariables>"), study); }
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

struct ActiveRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    String control;
    std::size_t seedWrites = 0;
    ActiveRig()
    {
        auto& p = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-variable-zero-samples-duplicate").u16string());
        initProject(p, folder, qt::L("missing"));
        p.path = folder + qt::kSep + qt::L("missing.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1;
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ZeroSamplesDuplicateStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table("<DoeVariables>" + seedRows() + row("A", "9", "<Description>last</Description>") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ZeroSamplesDuplicateStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(session.project(), *input, ctx);
    }
};
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
}


TEST_CASE(doe_variable_textual_zero_duplicate_retains_sample_count_and_owner)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "7", "", " numSamples=\"3\"")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* record = study.vars[0];
    study.samples.invalid.fill(false);
    REQUIRE(readRows(study, row("A", "9", "<Description>later</Description>")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == record && record == study.headerVariableOwners[0].get());
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(record->number, 9);
    CHECK_EQ(record->index, 0);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.headerVariableOwners[0]->description == qt::L("later"));
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_textual_zero_nonlast_duplicate_updates_only_last_headers)
{
    doe::Study study;
    REQUIRE(readRows(study, seedRows()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    const auto* first = study.vars[0];
    const auto* last = study.vars[1];
    study.samples.invalid.fill(false);
    REQUIRE(readRows(study, row("A", "9", "<Description>last</Description>")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 5);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK(study.headerVariableOwners[0]->description.isEmpty());
    CHECK(study.headerVariableOwners[1]->description == qt::L("last"));
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_textual_zero_duplicate_preserves_empty_or_missing_creator_count)
{
    for (const auto& creator : std::vector<std::string>{"", " numSamples=\"\""}) {
        doe::Study study;
        REQUIRE(readRows(study, row("A", "7", "", creator)));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        const auto* record = study.vars[0];
        REQUIRE(readRows(study, row("A", "8")));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        CHECK(study.vars[0] == record);
        CHECK_EQ(study.headerVariableOwners[0]->numSamples, 0);
        CHECK_EQ(record->number, 8);
    }
}

TEST_CASE(doe_variable_textual_zero_duplicate_partial_headers_survive_later_guards)
{
    doe::Study sameList;
    REQUIRE(readRows(sameList, seedRows()));
    REQUIRE(sameList.vars.size() == 2);
    sameList.vars[1]->index = 73;
    CHECK(refused([&] { (void)readRows(sameList, row("A", "9", "<Description>kept</Description>") +
        row("B", "10", "<Keyword/>")); }).find("Keyword entry") != std::string::npos);
    CHECK_EQ(sameList.vars[1]->number, 10);
    CHECK_EQ(sameList.vars[1]->index, 73);
    CHECK_EQ(sameList.headerVariableOwners[1]->numSamples, 5);
    CHECK(sameList.headerVariableOwners[1]->description == qt::L("kept"));
    doe::Study sibling;
    REQUIRE(readRows(sibling, seedRows()));
    REQUIRE(sibling.vars.size() == 2);
    sibling.vars[1]->index = 73;
    CHECK(!refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row("A", "9") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), sibling); }).empty());
    CHECK_EQ(sibling.vars[1]->number, 9);
    CHECK_EQ(sibling.vars[1]->index, 1);
    CHECK_EQ(sibling.headerVariableOwners[1]->numSamples, 5);
}

TEST_CASE(doe_variable_textual_zero_duplicate_geometry_keeps_reached_state)
{
    doe::Study study;
    REQUIRE(readRows(study, seedRows()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    auto* first = study.vars[0];
    auto* last = study.vars[1];
    last->index = 73;
    study.selectedVariableIndex = 0;
    const std::array<bool, 3> invalid{{false, true, false}};
    study.samples.invalid = invalid;
    study.samples.tables[0] = {{1.0, 2.0}};

    CHECK(refused([&] { (void)readRows(study, row("A", "9",
        "<Description>kept</Description><Geometry/>")); }).find("Geometry entry") != std::string::npos);
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9); // zero-samples duplicate A updates reader-last B
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 5);
    CHECK_EQ(last->index, 73); // the outer reindex is after the Geometry stop
    CHECK(last->name == qt::L("B"));
    CHECK(study.headerVariableOwners[1]->description == qt::L("kept"));
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid == invalid);
    CHECK(study.samples.tables[0] == doe::SampleTable({{1.0, 2.0}}));
}

TEST_CASE(doe_variable_textual_zero_keeps_fresh_conversion_payload_and_owner_guards)
{
    doe::Study fresh;
    CHECK(!refused([&] { (void)readRows(fresh, row()); }).empty());
    CHECK(fresh.vars.empty() && fresh.headerVariableOwners.empty());
    const std::vector<std::string> guarded = {
        row("a"), row("C"),
        row("A", "7", "", " numSamples=\"00\""), row("A", "7", "", " numSamples=\"+0\""),
        row("A", "7", "", " numSamples=\"-0\""), row("A", "7", "", " numSamples=\" 0\""),
        row("A", "7", "", " numSamples=\"2147483648\""),
        "<DoeVariable name=\"A\" numSamples=\"0\" dimension=\"3\"/>",
        "<DoeVariable name=\"A\" numSamples=\"0\" varId=\"7\"/>"
    };
    for (const auto& input : guarded) {
        doe::Study study;
        REQUIRE(readRows(study, seedRows()));
        const auto* first = study.vars[0];
        const auto* last = study.vars[1];
        CHECK(!refused([&] { (void)readRows(study, input); }).empty());
        REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
        CHECK(study.vars[0] == first && study.vars[1] == last);
        CHECK_EQ(first->number, 7);
        CHECK_EQ(last->number, 8);
        CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
        CHECK_EQ(study.headerVariableOwners[1]->numSamples, 5);
    }
    doe::Study wrongMode;
    REQUIRE(readRows(wrongMode, seedRows()));
    wrongMode.samples.mode = 1;
    CHECK(!refused([&] { (void)readRows(wrongMode, row()); }).empty());
    doe::Study foreign;
    REQUIRE(readRows(foreign, seedRows()));
    doe::Var borrowed;
    foreign.vars[0] = &borrowed;
    CHECK(!refused([&] { (void)readRows(foreign, row()); }).empty());
    CHECK_EQ(foreign.headerVariableOwners[0]->numSamples, 3);
}

TEST_CASE(doe_variable_textual_zero_duplicate_actual_lazy_owner_then_same_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* last = owner->vars[1];
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK_EQ(owner->headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(owner->headerVariableOwners[1]->numSamples, 5);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK(owner->headerVariableOwners[1]->description == qt::L("last"));
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0, 5.0}}));
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    const auto writesBefore = p.doeIo->textWrites().size();
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == first && owner->vars[1] == last);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty() && rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
