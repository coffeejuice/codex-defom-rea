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
std::string row(const std::string& name, const std::string& id = "7",
                const std::string& samples = "3", const std::string& children = "")
{
    return "<DoeVariable name=\"" + name + "\" numSamples=\"" + samples +
        "\" varId=\"" + id + "\" dimension=\"3\">" + children + "</DoeVariable>";
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-unique").u16string());
        initProject(p, folder, qt::L("unique"));
        p.path = folder + qt::kSep + qt::L("unique.moproj");
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
            qt::L("UniqueStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + row("A", "7", "3", "<Description>first</Description>") +
            row("B", "8", "4", "<Description>second</Description>") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("UniqueStudy"));
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

TEST_CASE(doe_variable_unique_append_preserves_order_fields_and_concrete_identity)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "7", "3", "<Description>first</Description>") +
        row("B", "8", "4", "<Description>second</Description>") + row("C", "9", "5")));
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    for (std::size_t i = 0; i < 3; ++i) {
        CHECK(study.vars[i] == study.headerVariableOwners[i].get());
        CHECK_EQ(study.vars[i]->index, static_cast<int>(i));
        CHECK_EQ(study.vars[i]->number, static_cast<int>(7 + i));
        CHECK_EQ(study.headerVariableOwners[i]->numSamples, static_cast<int>(3 + i));
    }
    CHECK(study.vars[0] != study.vars[1] && study.vars[1] != study.vars[2]);
    CHECK(study.vars[0]->name == qt::L("A") && study.vars[2]->name == qt::L("C"));
    CHECK(study.headerVariableOwners[0]->description == qt::L("first"));
    CHECK(study.headerVariableOwners[1]->description == qt::L("second"));
    CHECK(study.headerVariableOwners[2]->description.isNull());
    CHECK_EQ(study.selectedVariableIndex, 2);
}

TEST_CASE(doe_variable_unique_append_extends_existing_concrete_owner_without_clearing_samples)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A")));
    const auto* first = study.vars[0];
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{4.0, 5.0}};
    REQUIRE(readRows(study, row("B")));
    REQUIRE(study.vars.size() == 2);
    CHECK(study.vars[0] == first && study.vars[1] != first);
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[1]->index, 1);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{4.0, 5.0}}));
}

TEST_CASE(doe_variable_unique_append_case_sensitive_names_do_not_generate_or_deduplicate_ids)
{
    doe::Study study;
    REQUIRE(readRows(study, row("Case", "2147483647") + row("case", "2147483647") + row("other", "1")));
    REQUIRE(study.vars.size() == 3);
    CHECK_EQ(study.vars[0]->number, 2147483647);
    CHECK_EQ(study.vars[1]->number, 2147483647);
    CHECK_EQ(study.vars[2]->number, 1);
    CHECK_EQ(study.selectedVariableIndex, 2);
}

TEST_CASE(doe_variable_unique_append_duplicate_name_applies_reader_setters_to_last_record)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "7", "3", "<Description>A</Description>") +
        row("B", "8", "4", "<Description>B</Description>") +
        row("A", "99", "9", "<Description>applied to last</Description>")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[1]->index, 1);
    CHECK_EQ(study.vars[0]->number, 7);
    CHECK_EQ(study.vars[1]->number, 99);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 4);
    CHECK(study.headerVariableOwners[1]->description == qt::L("applied to last"));
}

TEST_CASE(doe_variable_unique_append_unqualified_later_row_retains_prior_unfinalized_state)
{
    doe::Study study;
    CHECK(!refused([&] { (void)readRows(study, row("A") + row("B", "8", "4", "<Description>kept</Description>") +
        row("C", "9", "5", "<Keyword/>")); }).empty());
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    CHECK_EQ(study.vars[0]->index, -1);
    CHECK_EQ(study.vars[1]->index, -1);
    CHECK_EQ(study.vars[2]->index, -1);
    CHECK_EQ(study.vars[2]->number, 9);
    CHECK_EQ(study.headerVariableOwners[2]->numSamples, 5);
    CHECK(study.headerVariableOwners[2]->name == qt::L("C"));
    CHECK_EQ(study.selectedVariableIndex, 2);
    CHECK(study.headerVariableOwners[1]->description == qt::L("kept"));
    doe::Study later;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + row("A") + row("B") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), later); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(later.vars.size() == 2);
    CHECK_EQ(later.vars[0]->index, 0);
    CHECK_EQ(later.vars[1]->index, 1);
}

TEST_CASE(doe_variable_unique_append_rejects_foreign_null_reordered_and_mismatched_aliases)
{
    for (int profile = 0; profile < 4; ++profile) {
        doe::Study study;
        REQUIRE(readRows(study, row("A") + row("B")));
        auto* first = study.headerVariableOwners[0].get();
        auto* second = study.headerVariableOwners[1].get();
        doe::Var foreign;
        foreign.name = qt::L("same values do not prove ownership");
        if (profile == 0) study.vars[0] = &foreign;
        if (profile == 1) study.vars[0] = nullptr;
        if (profile == 2) { study.vars[0] = second; study.vars[1] = first; }
        if (profile == 3) study.vars.pop_back();
        CHECK(!refused([&] { (void)readRows(study, row("C")); }).empty());
        REQUIRE(study.headerVariableOwners.size() == 2);
        CHECK(study.headerVariableOwners[0].get() == first && study.headerVariableOwners[1].get() == second);
        CHECK_EQ(study.selectedVariableIndex, 1);
    }
    doe::Study nullOwner;
    nullOwner.headerVariableOwners.push_back(nullptr);
    nullOwner.vars.push_back(nullptr);
    CHECK(!refused([&] { (void)readRows(nullOwner, row("A")); }).empty());
    CHECK(nullOwner.headerVariableOwners.size() == 1 && nullOwner.vars.size() == 1);
}

TEST_CASE(doe_variable_unique_append_actual_lazy_owner_keeps_ordered_samples_and_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* second = owner->vars[1];
    CHECK(first == owner->headerVariableOwners[0].get() && second == owner->headerVariableOwners[1].get());
    CHECK_EQ(first->index, 0);
    CHECK_EQ(second->index, 1);
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
    CHECK(owner->vars[0] == first && owner->vars[1] == second);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
