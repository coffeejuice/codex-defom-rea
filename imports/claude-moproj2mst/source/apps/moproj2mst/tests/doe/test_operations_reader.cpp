// ctest-labels: m2m-doe
// Source-qualified Operations no-op traversal; recording effects are simulated.
#include <testkit/check.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_activation.hpp"
#include "m2m/doe_io.hpp"
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
const char* populatedOperations =
    "<Operations>ignored text<!-- ignored comment -->"
    "<Operation id=\"9\" name=\"must-ignore\"><DoeOprVariable name=\"not-a-binding\"/></Operation>"
    "<Unknown><Nested/></Unknown></Operations>";

std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }

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

struct ActiveStudyRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    String studiesDir;
    String control;
    std::size_t seedWriteCount = 0;

    ActiveStudyRig()
    {
        auto& p = session.project();
        // All possible recording-overlay miss probes remain in configured scratch.
        const String folder((m2m_test::scratchRoot() / "doe-operations").u16string());
        initProject(p, folder, qt::L("operations"));
        p.path = folder + qt::kSep + qt::L("operations.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit actual-owner fixture, not C ABI SUB selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        xml::setAttribute(*oldSub, qt::L("Name"), qt::L("retained previous SUB"));
        p.dom->root->children.push_back(oldSub);
        studiesDir = projectDir(p) + qt::kSep + qt::L("DoeStudies");
        control = studiesDir + qt::kSep + qt::L("OperationsStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table(std::string("<DoeVariables/>") + populatedOperations + "<RegionsOfInterest/><Constraints/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t12.5\t\r\n"));
        seedWriteCount = p.doeIo->textWrites().size();
    }

    void readThroughActiveSub()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("OperationsStudy"));
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

TEST_CASE(doe_operations_populated_reader_returns_true_without_applying_mappings)
{
    doe::Study study;
    doe::Var variable;
    variable.name = qt::L("not-a-binding");
    variable.index = 23;
    variable.ops = {4, 7};
    study.vars.push_back(&variable);
    study.name = qt::L("unchanged");
    study.regions = {qt::L("existing region")};
    study.constraints = 3;
    study.samples.mode = 2;
    study.samples.tables[2] = {{5.0}};
    study.samples.invalid[2] = false;
    CHECK(doe::readInputTable(table(populatedOperations), study));
    CHECK(study.vars.size() == 1 && study.vars.front() == &variable);
    CHECK_EQ(variable.index, 23);
    CHECK(variable.ops == std::vector<int>({4, 7}));
    CHECK(study.name == qt::L("unchanged"));
    CHECK(study.regions == std::vector<String>({qt::L("existing region")}));
    CHECK_EQ(study.constraints, 3);
    CHECK_EQ(study.samples.mode, 2);
    CHECK(study.samples.tables[2] == doe::SampleTable({{5.0}}));
    CHECK(!study.samples.invalid[2]);
    CHECK(!doe::readInputTable("<sfDoeInputTable><Operations><Broken></Operations>", study));
    CHECK_EQ(variable.index, 23);
}

TEST_CASE(doe_operations_actual_active_study_read_continues_ordered_sample_effects)
{
    ActiveStudyRig rig;
    auto& p = rig.session.project();
    const auto originalOwner = rig.sub->ownedDoeStudy;
    rig.readThroughActiveSub();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == originalOwner && rig.sub->doe == originalOwner.get());
    CHECK(originalOwner->name == qt::L("OperationsStudy"));
    CHECK(originalOwner->vars.empty());
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads()[0].path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(p.doeIo->reads()[1].path == rig.control + qt::L("DEF_DOE.PAR"));
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWriteCount + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t mode = 0; mode < 3; ++mode) {
        const auto& write = p.doeIo->textWrites()[rig.seedWriteCount + mode];
        CHECK(write.path == rig.control + qt::L(names[mode]));
        CHECK(write.requestedBytes == std::to_string(mode) + "\r\n");
    }
    CHECK_EQ(originalOwner->samples.mode, 0);
    CHECK(originalOwner->samples.tables[0] == doe::SampleTable({{12.5}}));
    CHECK(!originalOwner->samples.invalid[0]);
    CHECK(p.doeIo->xmlWrites().empty());
}

TEST_CASE(doe_operations_actual_study_save_keeps_refresh_guard_and_xml_partial_effect)
{
    ActiveStudyRig rig;
    rig.readThroughActiveSub();
    auto& p = rig.session.project();
    const auto writesBeforeSave = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub);
    CHECK(rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBeforeSave);
    REQUIRE(saved.project.xmlJournal != nullptr);
    CHECK(saved.project.xmlJournal.get() == p.doeIo->xmlJournalOwner().get());
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(attempt.opened && attempt.closed && attempt.refreshBoundaryReached);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    CHECK(bytes->find("must-ignore") == std::string::npos);
    CHECK(bytes->find("not-a-binding") == std::string::npos);
    CHECK(saved.project.writes.empty()); // Immediate file is not replayable.
}

TEST_CASE(doe_operations_does_not_bypass_other_payload_guards_or_clear_order)
{
    doe::Study variableStudy;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        std::string("<DoeVariables><DoeVariable/></DoeVariables>") + populatedOperations), variableStudy); })
        .find("DoeVariables reader") != std::string::npos);
    CHECK(variableStudy.vars.empty());

    doe::Study regionStudy;
    regionStudy.regions = {qt::L("prior")};
    regionStudy.constraints = 9;
    CHECK(refused([&] { (void)doe::readInputTable(table(std::string(populatedOperations) +
        "<RegionsOfInterest><Region name=\"new\"/></RegionsOfInterest>"
        "<Constraints><Constraint/></Constraints>"), regionStudy); })
        .find("Constraints reader") != std::string::npos);
    CHECK(regionStudy.regions == std::vector<String>({qt::L("new")}));
    CHECK_EQ(regionStudy.constraints, 0);

    doe::Study constraintStudy;
    constraintStudy.constraints = 6;
    constraintStudy.regions = {qt::L("preserved")};
    CHECK(refused([&] { (void)doe::readInputTable(table(std::string(populatedOperations) +
        "<Constraints><Constraint/></Constraints>"), constraintStudy); })
        .find("Constraints reader") != std::string::npos);
    CHECK_EQ(constraintStudy.constraints, 0);
    CHECK(constraintStudy.regions == std::vector<String>({qt::L("preserved")}));
}

int main() { return testkit::run_all(); }
