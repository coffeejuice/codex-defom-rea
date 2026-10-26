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
std::string row(const std::string& attributes = "", const std::string& children = "", const std::string& samples = "3")
{ return "<DoeVariable numSamples=\"" + samples + "\"" + attributes + ">" + children + "</DoeVariable>"; }
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
std::string unnamedNamedUnnamed()
{
    return row(" varId=\"7\"", "<Description>first</Description>") +
        row(" name=\"Named\" varId=\"8\"", "<Description>second</Description>") +
        row(" varId=\"9\"", "<Description>replacement</Description>", "99");
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-absent-name").u16string());
        initProject(p, folder, qt::L("unnamed"));
        p.path = folder + qt::kSep + qt::L("unnamed.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit internal owner fixture, not C ABI Type2 selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("AbsentNameStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + unnamedNamedUnnamed() + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("AbsentNameStudy"));
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

TEST_CASE(doe_variable_absent_name_keeps_default_null_value_and_other_defaults)
{
    doe::Study study;
    REQUIRE(readRows(study, row()));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.vars[0]->name.isNull() && study.vars[0]->name.isEmpty());
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[0]->number, 1);
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.selectedVariableIndex, 0);
}

TEST_CASE(doe_variable_absent_name_repeated_rows_use_one_existing_owner)
{
    doe::Study study;
    REQUIRE(readRows(study, row(" varId=\"7\"", "<Description>first</Description>")));
    const auto* record = study.headerVariableOwners[0].get();
    REQUIRE(readRows(study, row(" varId=\"8\"", "<Description>second</Description>", "99")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == record && study.headerVariableOwners[0].get() == record);
    CHECK(record->name.isNull());
    CHECK_EQ(record->numSamples, 3);
    CHECK_EQ(record->number, 8);
    CHECK(record->description == qt::L("second"));
}

TEST_CASE(doe_variable_absent_name_nonlast_match_still_targets_reader_last)
{
    doe::Study study;
    REQUIRE(readRows(study, unnamedNamedUnnamed()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0]->name.isNull());
    CHECK(study.vars[1]->name == qt::L("Named") && !study.vars[1]->name.isNull());
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[1]->number, 9);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 3);
    CHECK(study.headerVariableOwners[0]->description == qt::L("first"));
    CHECK(study.headerVariableOwners[1]->description == qt::L("replacement"));
    CHECK_EQ(study.selectedVariableIndex, 1);
}

TEST_CASE(doe_variable_absent_name_retains_null_record_before_later_guards)
{
    doe::Study sameList;
    CHECK(refused([&] { (void)readRows(sameList, row("", "<Description>kept</Description>") +
        row(" name=\"other\" varId=\"8\"", "<Keyword/>")); }).find("Keyword entry") != std::string::npos);
    REQUIRE(sameList.vars.size() == 2 && sameList.headerVariableOwners.size() == 2);
    CHECK(sameList.vars[0]->name.isNull());
    CHECK_EQ(sameList.vars[0]->number, 0);
    CHECK_EQ(sameList.vars[0]->index, -1);
    CHECK(sameList.headerVariableOwners[0]->description == qt::L("kept"));
    CHECK(sameList.vars[1]->name == qt::L("other"));
    CHECK_EQ(sameList.vars[1]->number, 8);
    CHECK_EQ(sameList.vars[1]->index, -1);
    CHECK(sameList.headerVariableOwners[1]->description.isEmpty());
    CHECK_EQ(sameList.selectedVariableIndex, 1);
    CHECK(sameList.samples.invalid[0] && sameList.samples.invalid[1] && sameList.samples.invalid[2]);
    doe::Study sibling;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row() +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), sibling); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(sibling.vars.size() == 1);
    CHECK(sibling.vars[0]->name.isNull());
    CHECK_EQ(sibling.vars[0]->number, 0);
    CHECK_EQ(sibling.vars[0]->index, 0);
}

TEST_CASE(doe_variable_absent_name_keeps_missing_sample_and_foreign_guards)
{
    const std::vector<std::string> guarded = {
        "<DoeVariable/>", "<DoeVariable varId=\"1\"/>",
        row("", "", ""), row("", "", "0"), row("", "", "-1"), row("", "", "01"), row("", "", "2147483648")
    };
    for (const auto& input : guarded) {
        doe::Study study;
        CHECK(!refused([&] { (void)readRows(study, input); }).empty());
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
    }
    doe::Study study;
    REQUIRE(readRows(study, row()));
    const auto* record = study.headerVariableOwners[0].get();
    doe::Var foreign;
    study.vars[0] = &foreign;
    CHECK(!refused([&] { (void)readRows(study, row(" varId=\"9\"")); }).empty());
    CHECK(study.headerVariableOwners[0].get() == record && record->number == 1);
}

TEST_CASE(doe_variable_absent_name_geometry_refusal_retains_reached_header_prefix)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRows(study, row("", "<Geometry/>")); })
        .find("Geometry entry") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.vars[0]->name.isNull() && study.vars[0]->name.isEmpty());
    CHECK_EQ(study.vars[0]->number, 0);
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_variable_absent_name_actual_owner_reaches_samples_then_same_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* last = owner->vars[1];
    CHECK(first == owner->headerVariableOwners[0].get() && last == owner->headerVariableOwners[1].get());
    CHECK(first->name.isNull() && last->name == qt::L("Named"));
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0, 5.0}}));
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == first && owner->vars[1] == last);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
