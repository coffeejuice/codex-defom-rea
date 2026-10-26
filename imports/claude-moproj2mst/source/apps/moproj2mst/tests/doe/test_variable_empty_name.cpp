// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <initializer_list>
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
std::string row(const std::string& attributes = " name=\"\"", const std::string& children = "", const std::string& samples = "3")
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
std::string emptyNamedEmpty()
{
    return row(" name=\"\" varId=\"7\"", "<Description>first</Description>") +
        row(" name=\"Named\" varId=\"8\"", "<Description>second</Description>") +
        row(" name=\"\" varId=\"9\"", "<Description>replacement</Description>", "99");
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-empty-name").u16string());
        initProject(p, folder, qt::L("empty"));
        p.path = folder + qt::kSep + qt::L("empty.moproj");
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
            qt::L("EmptyNameStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + emptyNamedEmpty() + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("EmptyNameStudy"));
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

TEST_CASE(doe_variable_empty_name_both_quote_forms_preserve_nonnull_empty)
{
    for (const auto& attribute : std::vector<std::string>{" name=\"\"", " name=''"}) {
        doe::Study study;
        REQUIRE(readRows(study, row(attribute)));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        CHECK(study.vars[0] == study.headerVariableOwners[0].get());
        CHECK(study.vars[0]->name.isEmpty() && !study.vars[0]->name.isNull());
        CHECK_EQ(study.vars[0]->number, 1);
        CHECK_EQ(study.vars[0]->index, 0);
        CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
        CHECK_EQ(study.selectedVariableIndex, 0);
    }
}

TEST_CASE(doe_variable_empty_name_content_match_preserves_first_creator_nullness)
{
    for (bool nullFirst : {false, true}) {
        doe::Study study;
        REQUIRE(readRows(study, row(nullFirst ? " varId=\"7\"" : " name=\"\" varId=\"7\"")));
        const auto* record = study.headerVariableOwners[0].get();
        study.samples.invalid.fill(false);
        REQUIRE(readRows(study, row(nullFirst ? " name=\"\" varId=\"8\"" : " varId=\"8\"",
                                  "<Description>later</Description>", "99")));
        REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
        CHECK(study.vars[0] == record && study.headerVariableOwners[0].get() == record);
        CHECK(record->name.isNull() == nullFirst && record->name.isEmpty());
        CHECK_EQ(record->numSamples, 3);
        CHECK_EQ(record->number, 8);
        CHECK(record->description == qt::L("later"));
        CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    }
}

TEST_CASE(doe_variable_empty_name_nonlast_match_updates_only_reader_last_record)
{
    doe::Study study;
    REQUIRE(readRows(study, emptyNamedEmpty()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0]->name.isEmpty() && !study.vars[0]->name.isNull());
    CHECK(study.vars[1]->name == qt::L("Named"));
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[1]->number, 9);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 3);
    CHECK(study.headerVariableOwners[0]->description == qt::L("first"));
    CHECK(study.headerVariableOwners[1]->description == qt::L("replacement"));
    CHECK_EQ(study.selectedVariableIndex, 1);
}

TEST_CASE(doe_variable_empty_name_partial_value_survives_later_reader_guards)
{
    doe::Study sameList;
    CHECK(refused([&] { (void)readRows(sameList, row() +
        row(" name=\"other\" varId=\"8\"", "<Keyword/>")); }).find("Keyword entry") != std::string::npos);
    REQUIRE(sameList.vars.size() == 2 && sameList.headerVariableOwners.size() == 2);
    CHECK(sameList.vars[0]->name.isEmpty() && !sameList.vars[0]->name.isNull());
    CHECK_EQ(sameList.vars[0]->number, 0);
    CHECK_EQ(sameList.vars[0]->index, -1);
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
    CHECK(sibling.vars[0]->name.isEmpty() && !sibling.vars[0]->name.isNull());
    CHECK_EQ(sibling.vars[0]->number, 0);
    CHECK_EQ(sibling.vars[0]->index, 0);
}

TEST_CASE(doe_variable_empty_name_keeps_sample_payload_and_foreign_owner_guards)
{
    const std::vector<std::string> guarded = {
        "<DoeVariable name=\"\"/>", row(" name=\"\"", "", ""), row(" name=\"\"", "", "0"),
        row(" name=\"\"", "", "01"), row(" name=\"\"", "", "2147483648")
    };
    for (const auto& input : guarded) {
        doe::Study study;
        CHECK(!refused([&] { (void)readRows(study, input); }).empty());
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    }
    doe::Study study;
    REQUIRE(readRows(study, row()));
    const auto* record = study.headerVariableOwners[0].get();
    doe::Var foreign;
    foreign.name = qt::L("");
    study.vars[0] = &foreign;
    CHECK(!refused([&] { (void)readRows(study, row(" name=\"\" varId=\"9\"")); }).empty());
    CHECK(study.headerVariableOwners[0].get() == record && record->number == 1);
}

TEST_CASE(doe_variable_empty_name_geometry_refusal_retains_reached_header_prefix)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRows(study, row(" name=\"\"", "<Geometry/>")); })
        .find("Geometry entry") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.vars[0]->name.isEmpty() && !study.vars[0]->name.isNull());
    CHECK_EQ(study.vars[0]->number, 0);
    CHECK_EQ(study.headerVariableOwners[0]->dimension, 3);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_variable_empty_name_actual_owner_reaches_samples_then_same_save_guard)
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
    CHECK(first->name.isEmpty() && !first->name.isNull() && last->name == qt::L("Named"));
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
