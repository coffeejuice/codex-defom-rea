// ctest-labels: m2m-doe
// Exact no-row dispatch; recognized variable payloads remain unsupported.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <utility>
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
    "<DoeVariables>text<!--comment--><![CDATA[raw]]>"
    "<Unknown bad=\"attributes\"><DoeVariable name=\"not-direct\"/></Unknown>"
    "<doevariable/><DOEVariable/><Metadata/></DoeVariables>";
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
    explicit ActiveRig(bool existingVariables = false)
    {
        auto& p = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-variable-dispatch").u16string());
        initProject(p, folder, qt::L("dispatch"));
        p.path = folder + qt::kSep + qt::L("dispatch.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit actual owner, not public C ABI selection.
        p.openedOk = true;
        if (existingVariables) {
            for (int number : {0, 7}) {
                auto variable = std::make_unique<doe::Var>();
                variable->number = number;
                variable->index = 54;
                sub->ownedDoeStudy->vars.push_back(variable.get());
                sub->ownedDoeStudy->scalarPrefixVariableOwners.push_back(std::move(variable));
            }
        }
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("DispatchStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            std::string("<RegionsOfInterest><Region name=\"R1\"/></RegionsOfInterest>") +
            ignoredRows + "<Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("DispatchStudy"));
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

TEST_CASE(doe_variable_dispatch_ignores_nonmatching_direct_children_without_records)
{
    doe::Study study;
    study.samples.mode = 2;
    study.samples.tables[2] = {{8.0}};
    study.samples.invalid[2] = false;
    CHECK(doe::readInputTable(table(ignoredRows), study));
    CHECK(study.vars.empty());
    CHECK(study.scalarPrefixVariableOwners.empty() && study.geometryPrefixVariableOwners.empty());
    CHECK(!study.retainedScalarPrefix());
    CHECK_EQ(study.samples.mode, 2);
    CHECK(study.samples.tables[2] == doe::SampleTable({{8.0}}));
    CHECK(!study.samples.invalid[2]);
}

TEST_CASE(doe_variable_dispatch_no_row_list_finalizes_existing_records)
{
    doe::Study study;
    doe::Var first, second;
    first.number = 0;
    second.number = 5;
    first.index = 90;
    second.index = 91;
    first.ops = {8};
    first.link = &second;
    study.vars = {&first, &second};
    CHECK(doe::readInputTable(table(ignoredRows), study));
    CHECK(study.vars == std::vector<doe::Var*>({&first, &second}));
    CHECK_EQ(first.index, 0);
    CHECK_EQ(second.index, 1);
    CHECK_EQ(first.number, 6);
    CHECK_EQ(second.number, 5);
    CHECK(first.ops == std::vector<int>({8}) && first.link == &second);
}

TEST_CASE(doe_variable_dispatch_exact_row_after_ignored_siblings_keeps_payload_guard)
{
    doe::Study study;
    doe::Var existing;
    existing.number = 0;
    existing.index = 90;
    study.vars.push_back(&existing);
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"7\"/><DoeVariables><Unknown/><doevariable/>"
        "<DoeVariable name=\"recognized\"/><Ignored/></DoeVariables>"), study); })
        .find("DoeVariables reader 0x14090ee20") != std::string::npos);
    CHECK_EQ(study.roiType, 7);
    CHECK_EQ(existing.index, 90);
    CHECK_EQ(existing.number, 0);
    CHECK(study.vars.size() == 1 && study.vars.front() == &existing);
    CHECK(study.scalarPrefixVariableOwners.empty() && study.geometryPrefixVariableOwners.empty());
}

TEST_CASE(doe_variable_dispatch_repeated_no_row_lists_have_one_outer_id_pass)
{
    doe::Study study;
    doe::Var first, second;
    first.number = -9;
    second.number = -2;
    study.vars = {&first, &second};
    CHECK(doe::readInputTable(table(std::string(ignoredRows) + ignoredRows), study));
    CHECK_EQ(first.index, 0);
    CHECK_EQ(second.index, 1);
    CHECK_EQ(first.number, 0);
    CHECK_EQ(second.number, 1);
}

TEST_CASE(doe_variable_dispatch_later_constraint_guard_retains_prior_prefix_only)
{
    doe::Study study;
    doe::Var existing;
    existing.number = 0;
    existing.index = 90;
    study.vars.push_back(&existing);
    study.constraints = 5;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        std::string("<RegionsOfInterest><Region name=\"kept\"/></RegionsOfInterest>") +
        ignoredRows + "<Constraints><Constraint/></Constraints>"), study); })
        .find("Constraints reader") != std::string::npos);
    CHECK(study.regions == std::vector<String>({qt::L("kept")}));
    CHECK_EQ(existing.index, 0);
    CHECK_EQ(existing.number, 0);
    CHECK_EQ(study.constraints, 0);
}

TEST_CASE(doe_variable_dispatch_no_row_reindex_null_fault_remains_guarded)
{
    doe::Study study;
    doe::Var existing;
    existing.index = 90;
    existing.number = 0;
    study.vars = {&existing, nullptr};
    CHECK(refused([&] { (void)doe::readInputTable(table(ignoredRows), study); })
        .find("0x14091c800") != std::string::npos);
    CHECK_EQ(existing.index, 0);
    CHECK_EQ(existing.number, 0);
}

TEST_CASE(doe_variable_dispatch_actual_owner_read_retains_sample_order_without_notifications)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK(owner->vars.empty() && owner->scalarPrefixVariableOwners.empty());
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

TEST_CASE(doe_variable_dispatch_actual_owner_save_drops_ignored_metadata_before_refresh_guard)
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
    CHECK(attempt.refreshBoundaryReached);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    CHECK(bytes->find("not-direct") == std::string::npos);
    CHECK(bytes->find("Metadata") == std::string::npos);
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, *bytes, &error, &line, &column));
    REQUIRE(document.root != nullptr);
    const auto variables = document.root->childElements(qt::L("DoeVariables"));
    REQUIRE(variables.size() == 1);
    CHECK(variables.front()->childElements().empty());
    CHECK(saved.project.writes.empty());
}

TEST_CASE(doe_variable_dispatch_existing_actual_records_keep_save_reconciliation_guard)
{
    ActiveRig rig(true);
    rig.read();
    auto& p = rig.session.project();
    REQUIRE(rig.sub->ownedDoeStudy->vars.size() == 2);
    CHECK_EQ(rig.sub->ownedDoeStudy->vars[0]->index, 0);
    CHECK_EQ(rig.sub->ownedDoeStudy->vars[0]->number, 8);
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
}

int main() { return testkit::run_all(); }
