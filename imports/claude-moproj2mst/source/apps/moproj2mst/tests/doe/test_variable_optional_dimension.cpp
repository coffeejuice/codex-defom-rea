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
std::string row(const std::string& name, const std::string& attributes = "", const std::string& children = "")
{
    return "<DoeVariable name=\"" + name + "\" numSamples=\"3\"" + attributes + ">" + children + "</DoeVariable>";
}
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-optional-dimension").u16string());
        initProject(p, folder, qt::L("dimension"));
        p.path = folder + qt::kSep + qt::L("dimension.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit internal owner fixture, not C ABI selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("OptionalDimensionStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + row("A", "", "<Description>first</Description>") +
            row("B", " varId=\"5\"") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("OptionalDimensionStudy"));
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

TEST_CASE(doe_variable_optional_dimension_keeps_new_constructor_three)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", " varId=\"7\"", "<Description>kept</Description>")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK(study.headerVariableOwners[0]->description == qt::L("kept"));
}

TEST_CASE(doe_variable_optional_dimension_combines_with_absent_id_outer_finalization)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A") + row("B", " varId=\"5\"")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.headerVariableOwners[1]->dimension, 3);
    CHECK_EQ(study.vars[0]->number, 6);
    CHECK_EQ(study.vars[1]->number, 5);
    CHECK_EQ(study.selectedVariableIndex, 1);
}

TEST_CASE(doe_variable_optional_dimension_duplicate_skips_store_but_present_three_sets_last)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", " varId=\"7\" dimension=\"3\"") + row("B", " varId=\"8\" dimension=\"3\"")));
    auto* first = study.headerVariableOwners[0].get();
    auto* last = study.headerVariableOwners[1].get();
    last->dimension = 2; // Explicit field-state witness, not admitted dimension2 XML or GUI state.
    study.selectedVariableIndex = 0;
    study.samples.invalid.fill(false);
    REQUIRE(readRows(study, row("A", " varId=\"9\"", "<Description>last text</Description>")));
    CHECK(study.headerVariableOwners[0].get() == first && study.headerVariableOwners[1].get() == last);
    CHECK_EQ(first->dimension, 3);
    CHECK_EQ(last->dimension, 2);
    CHECK_EQ(last->number, 9);
    CHECK(last->description == qt::L("last text"));
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    REQUIRE(readRows(study, row("A", " dimension=\"3\"")));
    CHECK_EQ(first->dimension, 3);
    CHECK_EQ(last->dimension, 3);
    CHECK_EQ(last->number, 9); // Missing ID still skips its independent setter.
}

TEST_CASE(doe_variable_optional_dimension_present_unqualified_values_keep_guard)
{
    const std::vector<std::string> values = {"", "0", "2", "03", "-1", " 3", "2147483648"};
    for (const auto& value : values) {
        doe::Study study;
        study.samples.invalid.fill(false);
        CHECK(refused([&] { (void)readRows(study, row("A", " varId=\"7\" dimension=\"" + value + "\"")); })
            .find("DoeVariables reader 0x14090ee20") != std::string::npos);
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
        CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    }
}

TEST_CASE(doe_variable_optional_dimension_retains_constructor_and_header_state_before_later_guards)
{
    doe::Study sameList;
    CHECK(refused([&] { (void)readRows(sameList, row("A") + row("B", "", "<Keyword/>")); })
        .find("DoeVariables reader") != std::string::npos);
    REQUIRE(sameList.vars.size() == 2 && sameList.headerVariableOwners.size() == 2);
    CHECK_EQ(sameList.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(sameList.vars[0]->number, 0);
    CHECK_EQ(sameList.vars[0]->index, -1);
    CHECK_EQ(sameList.headerVariableOwners[1]->dimension, 3);
    CHECK_EQ(sameList.vars[1]->number, 0);
    CHECK_EQ(sameList.vars[1]->index, -1);
    CHECK(sameList.headerVariableOwners[1]->name == qt::L("B"));
    CHECK_EQ(sameList.selectedVariableIndex, 1);
    doe::Study sibling;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row("A") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), sibling); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(sibling.headerVariableOwners.size() == 1);
    CHECK_EQ(sibling.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(sibling.vars[0]->number, 0);
    CHECK_EQ(sibling.vars[0]->index, 0);
}

TEST_CASE(doe_variable_optional_dimension_actual_owner_continues_samples_and_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    CHECK_EQ(owner->headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(owner->headerVariableOwners[1]->dimension, 3);
    CHECK_EQ(owner->vars[0]->number, 6);
    CHECK_EQ(owner->vars[1]->number, 5);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0, 5.0}}));
    const auto* first = owner->vars[0];
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == first);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
