// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
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
std::string row(const std::string& samples = "3", const std::string& id = "7",
                const std::string& dimension = "3", const std::string& extra = "")
{
    return "<DoeVariable name=\"Header variable\" numSamples=\"" + samples +
        "\" varId=\"" + id + "\" dimension=\"" + dimension + "\"" + extra + "/>";
}
bool readRow(doe::Study& study, const std::string& input)
{ return doe::readInputTable(table("<DoeVariables>" + input + "</DoeVariables>"), study); }
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
        const String folder((m2m_test::scratchRoot() / "doe-variable-record").u16string());
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit internal Type2 selection, not C ABI behavior.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("HeaderStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + row() + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("HeaderStudy"));
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

TEST_CASE(doe_variable_record_constructor_has_source_defaults)
{
    doe::HeaderVariableRecord record(3);
    CHECK(record.name.isNull() && record.description.isNull());
    CHECK_EQ(record.numSamples, 3);
    CHECK_EQ(record.f14, 0);
    CHECK_EQ(record.index, -1);
    CHECK_EQ(record.number, 0);
    CHECK_EQ(record.geoType, 100);
    CHECK_EQ(record.dimension, 3);
    CHECK(record.files.empty() && record.items.empty() && record.ops.empty() && !record.link);
    CHECK(!record.scalar && !record.geometry);
    CHECK_EQ(doe::kNativeVariableRecordAllocationBytes, std::size_t(0x48));
    doe::Study study;
    CHECK_EQ(study.selectedVariableIndex, -1);
}

TEST_CASE(doe_variable_record_public_read_commits_actual_alias_and_reindexes)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{4.0}};
    REQUIRE(readRow(study, row()));
    REQUIRE(study.headerVariableOwners.size() == 1 && study.vars.size() == 1);
    const auto& record = *study.headerVariableOwners.front();
    CHECK(study.vars[0] == &record);
    CHECK(record.name == qt::L("Header variable") && !record.name.isNull());
    CHECK_EQ(record.numSamples, 3);
    CHECK_EQ(record.number, 7);
    CHECK_EQ(record.index, 0);
    CHECK_EQ(record.dimension, 3);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(!study.retainedScalarPrefix() && study.scalarPrefixVariableOwners.empty());
}

TEST_CASE(doe_variable_record_unqualified_profiles_preserve_preappend_guard)
{
    const std::vector<std::string> unqualified = {
        "<DoeVariable/>",
        row("0"), row("01"), row("-1"), row("2147483648"),
        row("3", "0"), row("3", "01"), row("3", "2147483648"), row("3", "7", "03"),
        row("3", "7", "2")
    };
    for (const auto& input : unqualified) {
        doe::Study study;
        study.samples.invalid.fill(false);
        CHECK(refused([&] { (void)readRow(study, input); }).find("DoeVariables reader 0x14090ee20") != std::string::npos);
        CHECK(study.headerVariableOwners.empty() && study.vars.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
        CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    }
    doe::Study max;
    REQUIRE(readRow(max, row("2147483647", "2147483647", "3", " LinkTo=\"\"")));
    CHECK_EQ(max.headerVariableOwners.front()->numSamples, 2147483647);
    CHECK_EQ(max.vars.front()->number, 2147483647);
}

TEST_CASE(doe_variable_record_direct_geometry_retains_header_prefix_before_filename_guard)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{4.0}};
    const auto error = refused([&] {
        (void)readRow(study, "<DoeVariable name=\"X\" numSamples=\"3\" varId=\"1\" dimension=\"3\"><Geometry/></DoeVariable>");
    });
    CHECK(error.find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* const record = study.headerVariableOwners.front().get();
    CHECK(study.vars.front() == record);
    CHECK(record->name == qt::L("X"));
    CHECK_EQ(record->numSamples, 3);
    CHECK_EQ(record->number, 1);
    CHECK_EQ(record->dimension, 3);
    CHECK_EQ(record->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(record->description.isNull());
    CHECK(record->items.empty() && record->files.empty() && record->ops.empty());
    CHECK(!record->geometry);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{4.0}}));
}

TEST_CASE(doe_variable_record_direct_keyword_retains_reached_header_prefix)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{4.0}};
    const auto error = refused([&] {
        (void)readRow(study, "<DoeVariable name=\"X\" numSamples=\"3\" varId=\"1\" dimension=\"3\"><Keyword/></DoeVariable>");
    });
    CHECK(error.find("Keyword entry 0x14090f5bf") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* const record = study.headerVariableOwners.front().get();
    CHECK(study.vars.front() == record);
    CHECK(record->name == qt::L("X"));
    CHECK_EQ(record->numSamples, 3);
    CHECK_EQ(record->number, 1);
    CHECK_EQ(record->dimension, 3);
    CHECK_EQ(record->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(record->description.isNull());
    CHECK(record->items.empty() && record->files.empty() && record->ops.empty());
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{4.0}}));
}

TEST_CASE(doe_variable_record_mode_and_preexisting_alias_guards_do_not_replace_owner)
{
    for (int mode = 1; mode <= 2; ++mode) {
        doe::Study study;
        study.samples.mode = mode;
        CHECK(!refused([&] { (void)readRow(study, row()); }).empty());
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
    }
    doe::Study study;
    doe::Var prior;
    prior.name = qt::L("prior");
    prior.index = 90;
    study.vars = {&prior};
    CHECK(!refused([&] { (void)readRow(study, row()); }).empty());
    REQUIRE(study.vars.size() == 1);
    CHECK(study.vars.front() == &prior && prior.index == 90);
    CHECK(study.headerVariableOwners.empty());
}

TEST_CASE(doe_variable_record_second_matched_row_retains_first_before_reindex)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRow(study, row() + row("3", "0")); }).find("DoeVariables reader") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* record = study.headerVariableOwners.front().get();
    CHECK(study.vars[0] == record);
    CHECK_EQ(record->index, -1);
    CHECK_EQ(record->number, 7);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(!refused([&] { (void)readRow(study, row("3", "0")); }).empty());
    CHECK(study.headerVariableOwners.front().get() == record && study.vars[0] == record);
}

TEST_CASE(doe_variable_record_later_guard_retains_prior_headers_and_finished_reindex)
{
    doe::Study study;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"8\"/><DoeVariables><Unknown><DoeVariable/></Unknown>" + row() +
        "</DoeVariables><Constraints><Constraint/></Constraints><DoeRoiType type=\"9\"/>"), study); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.roiType, 8);
    CHECK_EQ(study.constraints, 0);
    REQUIRE(doe::readInputTable(table("<DoeVariables/><Operations/>"), study));
    CHECK_EQ(study.vars[0]->number, 7);
}

TEST_CASE(doe_variable_record_actual_study_continues_sample_effects_without_notification)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->headerVariableOwners.size() == 1 && owner->vars.size() == 1);
    CHECK(owner->vars[0] == owner->headerVariableOwners[0].get());
    CHECK_EQ(owner->vars[0]->index, 0);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(doe_variable_record_actual_save_refuses_reconciliation_before_xml_open)
{
    ActiveRig rig;
    rig.read();
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    const auto* record = owner->headerVariableOwners.front().get();
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->headerVariableOwners.front().get() == record && owner->vars[0] == record);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
