// ctest-labels: m2m-doe
// Exact no-row constraint dispatch; recognized records remain guarded.
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
const char* ignoredRows =
    "<Constraints>text<!--comment--><![CDATA[raw]]>"
    "<Unknown type=\"invalid\"><Constraint name=\"not-direct\"/></Unknown>"
    "<constraint/><CONSTRAINT/><Metadata name=\"constraint-metadata\"/></Constraints>";
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
        const String folder((m2m_test::scratchRoot() / "doe-constraint-dispatch").u16string());
        initProject(p, folder, qt::L("constraints"));
        p.path = folder + qt::kSep + qt::L("constraints.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit fixture, not public C ABI selection.
        p.openedOk = true;
        sub->ownedDoeStudy->constraints = 7; // Count projection, not native record allocation.
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ConstraintStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            std::string("<DoeVariables/><RegionsOfInterest><Region name=\"R1\"/></RegionsOfInterest>") +
            ignoredRows + "<Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ConstraintStudy"));
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

TEST_CASE(doe_constraint_dispatch_ignored_direct_children_clear_only_constraint_projection)
{
    doe::Study study;
    study.constraints = 7;
    study.regions = {qt::L("preserved")};
    study.roiType = 3;
    study.samples.mode = 2;
    study.samples.tables[2] = {{8.0}};
    CHECK(doe::readInputTable(table(ignoredRows), study));
    CHECK_EQ(study.constraints, 0);
    CHECK(study.regions == std::vector<String>({qt::L("preserved")}));
    CHECK_EQ(study.roiType, 3);
    CHECK_EQ(study.samples.mode, 2);
    CHECK(study.samples.tables[2] == doe::SampleTable({{8.0}}));
    CHECK(study.vars.empty() && study.scalarPrefixVariableOwners.empty());
}

TEST_CASE(doe_constraint_dispatch_no_rows_allow_outer_ids_without_reindexing)
{
    doe::Study study;
    doe::Var first, second;
    first.number = 0;
    second.number = 5;
    first.index = 90;
    second.index = 91;
    study.vars = {&first, &second};
    study.constraints = 7;
    CHECK(doe::readInputTable(table(ignoredRows), study));
    CHECK_EQ(study.constraints, 0);
    CHECK_EQ(first.number, 6);
    CHECK_EQ(second.number, 5);
    CHECK_EQ(first.index, 90);
    CHECK_EQ(second.index, 91);
}

TEST_CASE(doe_constraint_dispatch_exact_direct_row_keeps_guard_after_clear)
{
    doe::Study study;
    doe::Var existing;
    existing.number = 0;
    existing.index = 90;
    study.vars.push_back(&existing);
    study.constraints = 7;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"8\"/><Constraints><Unknown/><constraint/>"
        "<Constraint name=\"recognized\"/></Constraints>"), study); })
        .find("Constraints reader 0x140912d10") != std::string::npos);
    CHECK_EQ(study.constraints, 0);
    CHECK_EQ(study.roiType, 8);
    CHECK_EQ(existing.number, 0);
    CHECK_EQ(existing.index, 90);
}

TEST_CASE(doe_constraint_dispatch_later_variable_guard_retains_clear_and_region_prefix)
{
    doe::Study study;
    study.constraints = 7;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        std::string("<RegionsOfInterest><Region name=\"kept\"/></RegionsOfInterest>") + ignoredRows +
        "<DoeVariables><DoeVariable/></DoeVariables>"), study); })
        .find("DoeVariables reader") != std::string::npos);
    CHECK_EQ(study.constraints, 0);
    CHECK(study.regions == std::vector<String>({qt::L("kept")}));
    CHECK(study.vars.empty());
}

TEST_CASE(doe_constraint_dispatch_earlier_guard_and_malformed_xml_prevent_clear)
{
    doe::Study study;
    study.constraints = 7;
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        std::string("<DoeVariables><DoeVariable/></DoeVariables>") + ignoredRows), study); }).empty());
    CHECK_EQ(study.constraints, 7);
    CHECK(!doe::readInputTable("<sfDoeInputTable><Constraints><Broken></sfDoeInputTable>", study));
    CHECK_EQ(study.constraints, 7);
}

TEST_CASE(doe_constraint_dispatch_repeated_no_row_lists_keep_one_outer_id_pass)
{
    doe::Study study;
    doe::Var first, second;
    first.number = -9;
    second.number = -2;
    study.vars = {&first, &second};
    study.constraints = 7;
    CHECK(doe::readInputTable(table(std::string(ignoredRows) + "<Constraints/>" + ignoredRows), study));
    CHECK_EQ(study.constraints, 0);
    CHECK_EQ(first.number, 0);
    CHECK_EQ(second.number, 1);
}

TEST_CASE(doe_constraint_dispatch_clear_precedes_outer_id_null_fault)
{
    doe::Study study;
    doe::Var existing;
    existing.number = 0;
    existing.index = 90;
    study.vars = {&existing, nullptr};
    study.constraints = 7;
    CHECK(refused([&] { (void)doe::readInputTable(table(ignoredRows), study); })
        .find("0x14091dadd") != std::string::npos);
    CHECK_EQ(study.constraints, 0);
    CHECK_EQ(existing.number, 0);
    CHECK_EQ(existing.index, 90);
}

TEST_CASE(doe_constraint_dispatch_actual_owned_study_continues_ordered_sample_effects)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK_EQ(owner->constraints, 0);
    CHECK(owner->vars.empty());
    CHECK(owner->regions == std::vector<String>({qt::L("R1")}));
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t mode = 0; mode < 3; ++mode)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + mode].path == rig.control + qt::L(names[mode]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(doe_constraint_dispatch_actual_owner_save_keeps_xml_refresh_boundary)
{
    ActiveRig rig;
    rig.read();
    auto& p = rig.session.project();
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore);
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.opened && attempt.closed && attempt.refreshBoundaryReached);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    CHECK(bytes->find("constraint-metadata") == std::string::npos);
    CHECK(bytes->find("not-direct") == std::string::npos);
    CHECK(saved.project.writes.empty());
}

int main() { return testkit::run_all(); }
