// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <limits>
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
std::string row(const std::string& name, const std::string& idAttribute = "", const std::string& children = "")
{
    return "<DoeVariable name=\"" + name + "\" numSamples=\"3\" dimension=\"3\"" +
        idAttribute + ">" + children + "</DoeVariable>";
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-optional-id").u16string());
        initProject(p, folder, qt::L("optional"));
        p.path = folder + qt::kSep + qt::L("optional.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit internal fixture, not C ABI Type2 selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("OptionalIdStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + row("A") + row("B", "", "<Description>second</Description>") +
            row("C", " varId=\"5\"") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t6\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("OptionalIdStudy"));
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

TEST_CASE(doe_variable_optional_id_normal_completion_reaches_existing_outer_finalizer)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "", "<Description>kept</Description>")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK_EQ(study.vars[0]->number, 1);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK(study.headerVariableOwners[0]->description == qt::L("kept"));
}

TEST_CASE(doe_variable_optional_id_outer_scan_uses_all_rows_and_current_updates)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A") + row("B") + row("C", " varId=\"5\"")));
    REQUIRE(study.vars.size() == 3);
    CHECK_EQ(study.vars[0]->number, 6);
    CHECK_EQ(study.vars[1]->number, 7);
    CHECK_EQ(study.vars[2]->number, 5);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[1]->index, 1);
    CHECK_EQ(study.vars[2]->index, 2);
    CHECK_EQ(study.selectedVariableIndex, 2);
}

TEST_CASE(doe_variable_optional_id_list_order_preserves_native_wrapping_results)
{
    doe::Study explicitFirst;
    REQUIRE(readRows(explicitFirst, row("A", " varId=\"2147483647\"") + row("B")));
    REQUIRE(explicitFirst.vars.size() == 2);
    CHECK_EQ(explicitFirst.vars[0]->number, std::numeric_limits<int>::max());
    CHECK_EQ(explicitFirst.vars[1]->number, 1);
    doe::Study missingFirst;
    REQUIRE(readRows(missingFirst, row("A") + row("B", " varId=\"2147483647\"")));
    REQUIRE(missingFirst.vars.size() == 2);
    CHECK_EQ(missingFirst.vars[0]->number, std::numeric_limits<int>::min());
    CHECK_EQ(missingFirst.vars[1]->number, std::numeric_limits<int>::max());
}

TEST_CASE(doe_variable_optional_id_constructor_zero_survives_before_outer_pass)
{
    doe::Study sameList;
    CHECK(refused([&] { (void)readRows(sameList, row("A", "", "<Description>kept</Description>") +
        row("B", "", "<Keyword/>")); }).find("DoeVariables reader") != std::string::npos);
    REQUIRE(sameList.vars.size() == 2 && sameList.headerVariableOwners.size() == 2);
    CHECK_EQ(sameList.vars[0]->number, 0);
    CHECK_EQ(sameList.vars[0]->index, -1);
    CHECK(sameList.headerVariableOwners[0]->description == qt::L("kept"));
    CHECK_EQ(sameList.vars[1]->number, 0); // Missing ID keeps the fresh constructor value; outer repair is unreached.
    CHECK_EQ(sameList.vars[1]->index, -1);
    CHECK(sameList.headerVariableOwners[1]->name == qt::L("B"));
    CHECK_EQ(sameList.selectedVariableIndex, 1);
    doe::Study sibling;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row("A") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), sibling); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(sibling.vars.size() == 1);
    CHECK_EQ(sibling.vars[0]->number, 0);
    CHECK_EQ(sibling.vars[0]->index, 0);
}

TEST_CASE(doe_variable_optional_id_duplicate_preserves_last_existing_number)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", " varId=\"7\"") + row("B", " varId=\"8\"")));
    const auto* first = study.vars[0];
    const auto* last = study.vars[1];
    study.samples.invalid.fill(false);
    study.selectedVariableIndex = 0;
    REQUIRE(readRows(study, row("A", "", "<Description>last gets text</Description>")));
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 8);
    CHECK(study.headerVariableOwners[1]->description == qt::L("last gets text"));
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_optional_id_present_invalid_values_keep_preappend_guard)
{
    const std::vector<std::string> invalid = {"", "0", "-1", "01", " 1", "2147483648"};
    for (const auto& value : invalid) {
        doe::Study study;
        study.samples.invalid.fill(false);
        CHECK(refused([&] { (void)readRows(study, row("A", " varId=\"" + value + "\"")); })
            .find("DoeVariables reader 0x14090ee20") != std::string::npos);
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
        CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    }
}

TEST_CASE(doe_variable_optional_id_actual_owner_reaches_samples_then_same_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->vars.size() == 3 && owner->headerVariableOwners.size() == 3);
    for (std::size_t i = 0; i < 3; ++i) CHECK(owner->vars[i] == owner->headerVariableOwners[i].get());
    CHECK_EQ(owner->vars[0]->number, 6);
    CHECK_EQ(owner->vars[1]->number, 7);
    CHECK_EQ(owner->vars[2]->number, 5);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0, 5.0, 6.0}}));
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
