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
std::string row(const std::string& name, const std::string& id = "7",
                const std::string& samples = "3", const std::string& children = "")
{
    return "<DoeVariable name=\"" + name + "\" numSamples=\"" + samples +
        "\" varId=\"" + id + "\" dimension=\"3\">" + children + "</DoeVariable>";
}
std::string initialRows()
{ return row("A", "7", "3", "<Description>first</Description>") + row("B", "8", "4", "<Description>second</Description>"); }
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-duplicate").u16string());
        initProject(p, folder, qt::L("duplicate"));
        p.path = folder + qt::kSep + qt::L("duplicate.moproj");
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
            qt::L("DuplicateStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + initialRows() + row("A", "9", "99", "<Description>replacement</Description>") +
            "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("DuplicateStudy"));
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

TEST_CASE(doe_variable_duplicate_single_owner_keeps_name_samples_and_identity)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "7", "3", "<Description>first</Description>")));
    const auto* record = study.headerVariableOwners.front().get();
    REQUIRE(readRows(study, row("A", "99", "100", "<Description>last</Description>")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0] == record && study.headerVariableOwners[0].get() == record);
    CHECK(record->name == qt::L("A"));
    CHECK_EQ(record->numSamples, 3);
    CHECK_EQ(record->number, 99);
    CHECK(record->description == qt::L("last"));
    CHECK_EQ(record->index, 0);
}

TEST_CASE(doe_variable_duplicate_nonlast_name_updates_only_reader_last_object)
{
    doe::Study study;
    REQUIRE(readRows(study, initialRows()));
    const auto* first = study.headerVariableOwners[0].get();
    const auto* last = study.headerVariableOwners[1].get();
    REQUIRE(readRows(study, row("A", "9", "99", "<Description>replacement</Description>")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK(first->name == qt::L("A") && last->name == qt::L("B"));
    CHECK_EQ(first->numSamples, 3);
    CHECK_EQ(last->numSamples, 4);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK(first->description == qt::L("first") && last->description == qt::L("replacement"));
}

TEST_CASE(doe_variable_duplicate_skips_selection_invalidation_and_sample_clear)
{
    doe::Study study;
    REQUIRE(readRows(study, initialRows()));
    study.selectedVariableIndex = 0; // Explicit value state to distinguish skipped helper stores.
    const std::array<bool, 3> invalid{{false, true, false}};
    study.samples.invalid = invalid;
    study.samples.tables[0] = {{1.0, 2.0}};
    study.samples.tables[1] = {{3.0, 4.0}};
    REQUIRE(readRows(study, row("A", "10", "99", "<Description/><Description><!--empty--></Description>")));
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid == invalid);
    CHECK(study.samples.tables[0] == doe::SampleTable({{1.0, 2.0}}));
    CHECK(study.samples.tables[1] == doe::SampleTable({{3.0, 4.0}}));
    CHECK(study.headerVariableOwners[1]->description == qt::L("second"));
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 4);
    CHECK_EQ(study.vars[1]->number, 10);
}

TEST_CASE(doe_variable_duplicate_reached_setters_survive_later_matched_guard)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRows(study, initialRows() +
        row("A", "9", "99", "<Description>committed</Description>") + row("C", "10", "3", "<Keyword/>")); })
        .find("DoeVariables reader") != std::string::npos);
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.vars[1]->index, -1);
    CHECK_EQ(study.vars[2]->index, -1);
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[1]->number, 9);
    CHECK(study.headerVariableOwners[1]->description == qt::L("committed"));
    CHECK_EQ(study.vars[2]->number, 10);
    CHECK(study.headerVariableOwners[2]->name == qt::L("C"));
    CHECK_EQ(study.selectedVariableIndex, 2);
}

TEST_CASE(doe_variable_duplicate_geometry_keeps_prior_setters_and_foreign_alias_stays_guarded)
{
    doe::Study study;
    REQUIRE(readRows(study, initialRows()));
    auto* first = study.vars[0];
    auto* last = study.vars[1];
    CHECK(refused([&] { (void)readRows(study, row("A", "99", "100", "<Description>reached</Description><Geometry/>")); })
        .find("Geometry entry") != std::string::npos);
    CHECK_EQ(last->number, 99); // duplicate A setters target the reader's last record B
    CHECK(study.headerVariableOwners[1]->description == qt::L("reached"));
    doe::Var foreign;
    foreign.name = qt::L("A");
    study.vars[0] = &foreign;
    CHECK(!refused([&] { (void)readRows(study, row("A", "99")); }).empty());
    CHECK_EQ(last->number, 99); // the foreign-alias guard leaves Geometry's prior setter intact
    CHECK(study.headerVariableOwners[0].get() == first && study.headerVariableOwners[1].get() == last);
}

TEST_CASE(doe_variable_duplicate_lookup_keeps_case_sensitive_content_decision)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A") + row("a", "8")));
    REQUIRE(study.vars.size() == 2);
    REQUIRE(readRows(study, row("A", "17")));
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[1]->number, 17);
    CHECK(study.vars[1]->name == qt::L("a"));
    CHECK(study.headerVariableOwners.size() == 2);
}

TEST_CASE(doe_variable_duplicate_actual_lazy_owner_continues_samples_and_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* last = owner->vars[1];
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK(owner->headerVariableOwners[1]->description == qt::L("replacement"));
    CHECK_EQ(owner->headerVariableOwners[1]->numSamples, 4);
    CHECK_EQ(owner->selectedVariableIndex, 1);
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
