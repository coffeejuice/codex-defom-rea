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
                const std::string& body = "", const std::string& sampleAttribute = " numSamples=\"3\"", const std::string& linkAttribute = "")
{ return "<DoeVariable name=\"" + name + "\" varId=\"" + id + "\" dimension=\"3\"" + sampleAttribute + linkAttribute + ">" + body + "</DoeVariable>"; }
std::string linked(const std::string& name, const std::string& target, const std::string& id = "7", const std::string& body = "")
{ return row(name, id, body, " numSamples=\"3\"", " LinkTo=\"" + target + "\""); }
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-keyword-entry").u16string());
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
            qt::L("KeywordEntryStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table("<DoeVariables>" + linked("A", "B") + row("B", "8", "<Description>kept</Description><Keyword name=\"TLOC\"/><Description>unreached</Description>") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("KeywordEntryStudy"));
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

TEST_CASE(doe_variable_keyword_entry_keeps_fresh_header_and_earlier_description)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{99.0}};
    CHECK(refused([&] { (void)readRows(study, row("A", "7",
        "<Description>before</Description><Keyword name=\"TLOC\"/><Description>after</Description>")); })
        .find("Keyword entry 0x14090f5bf") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto& record = *study.headerVariableOwners[0];
    CHECK(study.vars[0] == &record && record.name == qt::L("A"));
    CHECK_EQ(record.number, 7);
    CHECK_EQ(record.numSamples, 3);
    CHECK_EQ(record.dimension, 3);
    CHECK_EQ(record.index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(record.description == qt::L("before"));
    CHECK(record.items.empty() && record.files.empty() && record.ops.empty());
    CHECK(!record.scalar && !record.geometry && record.link == nullptr);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{99.0}}));
}

TEST_CASE(doe_variable_keyword_entry_stops_at_first_direct_child_and_preserves_nested_text)
{
    doe::Study first;
    CHECK(!refused([&] { (void)readRows(first, row("A", "7", "<Keyword/><Description>after</Description>")); }).empty());
    REQUIRE(first.headerVariableOwners.size() == 1);
    CHECK(first.headerVariableOwners[0]->description.isNull());
    doe::Study nested;
    REQUIRE(readRows(nested, row("A", "7", "<Description>before<Keyword>nested</Keyword>after</Description>")));
    REQUIRE(nested.headerVariableOwners.size() == 1);
    CHECK(nested.headerVariableOwners[0]->description == qt::L("beforenestedafter"));
    CHECK_EQ(nested.vars[0]->index, 0);
}

TEST_CASE(doe_variable_keyword_entry_duplicate_keeps_creator_count_and_targets_last)
{
    doe::Study study;
    REQUIRE(readRows(study, seedRows()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    const auto* first = study.vars[0];
    const auto* last = study.vars[1];
    study.vars[1]->index = 73;
    study.samples.invalid.fill(false);
    CHECK(!refused([&] { (void)readRows(study, row("A", "9",
        "<Description>kept</Description><Keyword/><Description>unreached</Description>", " numSamples=\"99\"")); }).empty());
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK_EQ(last->index, 73);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 5);
    CHECK(study.headerVariableOwners[1]->description == qt::L("kept"));
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_keyword_entry_prevents_link_resolution_and_outer_id_repair)
{
    doe::Study study;
    CHECK(!refused([&] { (void)readRows(study, linked("A", "B") +
        "<DoeVariable name=\"B\" numSamples=\"3\" dimension=\"3\"><Keyword/></DoeVariable>"); }).empty());
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0]->link == nullptr && study.vars[1]->link == nullptr);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.vars[1]->index, -1);
    CHECK_EQ(study.vars[1]->number, 0); // No varId setter and no outer generated-ID pass.
    CHECK_EQ(study.selectedVariableIndex, 1);
}

TEST_CASE(doe_variable_keyword_entry_retains_previous_completed_container_links)
{
    doe::Study study;
    CHECK(!refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + linked("A", "B") + row("B", "8") +
        "</DoeVariables><DoeVariables>" + linked("B", "A", "9", "<Keyword/>") + "</DoeVariables>"), study); }).empty());
    REQUIRE(study.vars.size() == 2);
    CHECK(study.vars[0]->link == study.vars[1] && study.vars[1]->link == nullptr);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[1]->index, 1);
    CHECK_EQ(study.vars[1]->number, 9);
}

TEST_CASE(doe_variable_keyword_entry_keeps_other_preappend_admission_guards)
{
    const std::vector<std::string> guarded = {
        row("A", "7", "<Description>unreached</Description><Keyword/><Other/>"),
        row("A", "7", "<Keyword/>", " numSamples=\"01\""),
        row("A", "7", "<Keyword/>", " numSamples=\"0\"")
    };
    for (const auto& input : guarded) {
        doe::Study study;
        CHECK(refused([&] { (void)readRows(study, input); }).find("Keyword entry") == std::string::npos);
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    }
    doe::Study wrongMode;
    wrongMode.samples.mode = 1;
    CHECK(!refused([&] { (void)readRows(wrongMode, row("A", "7", "<Keyword/>")); }).empty());
    CHECK(wrongMode.vars.empty());
    doe::Study foreign;
    REQUIRE(readRows(foreign, seedRows()));
    REQUIRE(foreign.vars.size() == 2);
    doe::Var borrowed;
    foreign.vars[0] = &borrowed;
    CHECK(!refused([&] { (void)readRows(foreign, row("A", "9", "<Keyword/>")); }).empty());
    CHECK_EQ(foreign.headerVariableOwners[1]->number, 8);
}

TEST_CASE(doe_variable_keyword_entry_actual_lazy_owner_stops_before_sample_io)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    owner->samples.invalid.fill(false);
    owner->samples.tables[0] = {{99.0}};
    const auto countBefore = owner->samples.counts;
    CHECK(refused([&] { rig.read(); }).find("Keyword entry") != std::string::npos);
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* last = owner->vars[1];
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK(owner->name == qt::L("KeywordEntryStudy"));
    CHECK(first->link == nullptr && last->link == nullptr);
    CHECK_EQ(first->index, -1);
    CHECK_EQ(last->index, -1);
    CHECK_EQ(last->number, 8);
    CHECK(owner->headerVariableOwners[1]->description == qt::L("kept"));
    CHECK(owner->samples.invalid[0] && owner->samples.invalid[1] && owner->samples.invalid[2]);
    CHECK(owner->samples.tables[0] == doe::SampleTable({{99.0}}));
    CHECK(owner->samples.counts == countBefore);
    CHECK(p.doeIo->textWrites().size() == rig.seedWrites);
    REQUIRE(p.doeIo->reads().size() == 1);
    CHECK(p.doeIo->reads()[0].path == rig.control + qt::L("DoeInputTable.XML"));
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == first && owner->vars[1] == last);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == rig.seedWrites && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty() && rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
