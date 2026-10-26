// ctest-labels: m2m-doe
#include <testkit/check.hpp>

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
                const std::string& body = "", const std::string& sampleAttribute = " numSamples=\"\"")
{ return "<DoeVariable name=\"" + name + "\" varId=\"" + id + "\" dimension=\"3\"" + sampleAttribute + ">" + body + "</DoeVariable>"; }
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-empty-samples").u16string());
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
            qt::L("EmptySamplesStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table("<DoeVariables>" + row("A", "7", "<Description>zero</Description>") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("EmptySamplesStudy"));
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

TEST_CASE(doe_variable_empty_samples_creates_actual_zero_count_owner)
{
    for (const auto& attribute : std::vector<std::string>{" numSamples=\"\"", " numSamples=''"}) {
        doe::Study study;
        study.samples.invalid.fill(false);
        REQUIRE(readRows(study, row("A", "7", "<Description>zero</Description>", attribute)));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        const auto& record = *study.headerVariableOwners[0];
        CHECK(study.vars[0] == &record && record.name == qt::L("A"));
        CHECK_EQ(record.numSamples, 0);
        CHECK_EQ(record.number, 7);
        CHECK_EQ(record.dimension, 3);
        CHECK_EQ(record.index, 0);
        CHECK_EQ(study.selectedVariableIndex, 0);
        CHECK(record.description == qt::L("zero"));
        CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    }
}

TEST_CASE(doe_variable_empty_samples_duplicate_ignores_zero_and_targets_last)
{
    doe::Study study;
    REQUIRE(readRows(study, "<DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\"/>"
        "<DoeVariable name=\"B\" numSamples=\"5\" varId=\"8\" dimension=\"3\"/>"));
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
    CHECK(study.headerVariableOwners[1]->description == qt::L("last"));
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_empty_samples_preserves_first_creator_count_across_profiles)
{
    for (const auto& creator : std::vector<std::string>{" numSamples=\"\"", "", " numSamples=\"3\""}) {
        doe::Study study;
        REQUIRE(readRows(study, row("A", "7", "", creator)));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        const auto* record = study.vars[0];
        const auto expectedSamples = study.headerVariableOwners[0]->numSamples;
        REQUIRE(readRows(study, row("A", "8") + row("A", "9", "", " numSamples=\"11\"") +
            row("A", "10", "", "")));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        CHECK(study.vars[0] == record);
        CHECK_EQ(study.headerVariableOwners[0]->numSamples, expectedSamples);
        CHECK_EQ(record->number, 10);
    }
}

TEST_CASE(doe_variable_empty_samples_preserves_reached_zero_before_later_guards)
{
    doe::Study sameList;
    CHECK(refused([&] { (void)readRows(sameList, row() + row("B", "8", "<Keyword/>")); })
        .find("Keyword entry") != std::string::npos);
    REQUIRE(sameList.vars.size() == 2 && sameList.headerVariableOwners.size() == 2);
    CHECK_EQ(sameList.headerVariableOwners[0]->numSamples, 0);
    CHECK_EQ(sameList.vars[0]->number, 7);
    CHECK_EQ(sameList.vars[0]->index, -1);
    CHECK_EQ(sameList.headerVariableOwners[1]->numSamples, 0);
    CHECK(sameList.vars[1]->name == qt::L("B"));
    CHECK_EQ(sameList.vars[1]->number, 8);
    CHECK_EQ(sameList.headerVariableOwners[1]->dimension, 3);
    CHECK_EQ(sameList.vars[1]->index, -1);
    CHECK_EQ(sameList.selectedVariableIndex, 1);
    CHECK(sameList.samples.invalid[0] && sameList.samples.invalid[1] && sameList.samples.invalid[2]);
    doe::Study sibling;
    CHECK(!refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row() +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), sibling); }).empty());
    REQUIRE(sibling.vars.size() == 1);
    CHECK_EQ(sibling.headerVariableOwners[0]->numSamples, 0);
    CHECK_EQ(sibling.vars[0]->index, 0);
}

TEST_CASE(doe_variable_empty_samples_keeps_unqualified_combinations_and_payload_guarded)
{
    const std::vector<std::string> guarded = {
        "<DoeVariable/>", "<DoeVariable name=\"A\" dimension=\"3\"/>",
        "<DoeVariable name=\"A\" varId=\"7\"/>", row(""), row("A", "0"),
        "<DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\" \"/>",
        "<DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\"-1\"/>",
        "<DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\"01\"/>",
        "<DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\"0\"/>",
        "<DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\"2147483648\"/>"
    };
    for (const auto& input : guarded) {
        doe::Study study;
        CHECK(!refused([&] { (void)readRows(study, input); }).empty());
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    }
    doe::Study wrongMode;
    wrongMode.samples.mode = 1;
    CHECK(!refused([&] { (void)readRows(wrongMode, row()); }).empty());
    CHECK(wrongMode.vars.empty());
    doe::Study foreign;
    REQUIRE(readRows(foreign, row()));
    doe::Var borrowed;
    foreign.vars[0] = &borrowed;
    CHECK(!refused([&] { (void)readRows(foreign, row("B", "8")); }).empty());
    CHECK_EQ(foreign.headerVariableOwners[0]->numSamples, 0);
}

TEST_CASE(doe_variable_empty_samples_geometry_refusal_keeps_reached_header_prefix)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRows(study, row("A", "7",
        "<Description>kept</Description><Geometry/><Description>unreached</Description>")); })
        .find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.headerVariableOwners[0]->description == qt::L("kept"));
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 0);
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_variable_empty_samples_actual_lazy_owner_continues_then_save_refuses)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    REQUIRE(owner->vars.size() == 1 && owner->headerVariableOwners.size() == 1);
    const auto* record = owner->vars[0];
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK_EQ(owner->headerVariableOwners[0]->numSamples, 0);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    const auto writesBefore = p.doeIo->textWrites().size();
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == record && p.dom->root->children[1] == rig.oldSub);
    CHECK(rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty() && rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
