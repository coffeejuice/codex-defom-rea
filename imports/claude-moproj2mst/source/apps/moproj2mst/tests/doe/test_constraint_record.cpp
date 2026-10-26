// ctest-labels: m2m-doe
// Bounded committed constraint values; no native Qt allocation/refcount parity.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_constraint_record.hpp"
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
std::string row(const std::string& id = "1010", const std::string& value = "0",
                const std::string& object = "1", const std::string& region = "2")
{
    return "<Constraint Id=\"" + id + "\" Value=\"" + value + "\" ObjectNo=\"" +
        object + "\" RegionNo=\"" + region + "\"/>";
}
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
bool readRows(doe::Study& study, const std::string& rows)
{ return doe::readInputTable(table("<Constraints>" + rows + "</Constraints>"), study); }
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
        const String folder((m2m_test::scratchRoot() / "doe-constraint-record").u16string());
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit same-Session fixture, not C ABI selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ConstraintRecordStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables/><Constraints>" + row() + "</Constraints><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ConstraintRecordStudy"));
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

TEST_CASE(doe_constraint_record_commits_known_fields_and_explicit_unknown)
{
    doe::Study study;
    REQUIRE(readRows(study, row("1010", "0", "3", "4")));
    REQUIRE(study.constraintRecords.size() == 1);
    CHECK_EQ(study.constraints, 1);
    const auto& record = *study.constraintRecords.front();
    CHECK_EQ(record.category, 0);
    CHECK_EQ(record.id, 1010);
    CHECK_EQ(record.objectNo, 3);
    CHECK_EQ(record.regionNo, 4);
    CHECK(record.value == 0.0);
    CHECK(record.text.isEmpty() && !record.text.isNull());
    CHECK(!record.sourceField10Bits.has_value());
    const auto copied = record;
    CHECK(!copied.sourceField10Bits.has_value());
    CHECK_EQ(doe::kNativeConstraintRecordAllocationBytes, std::size_t(0x28));
}

TEST_CASE(doe_constraint_record_categories_and_duplicate_identity_follow_row_order)
{
    doe::Study study;
    REQUIRE(readRows(study, row("1010") + row("1012") + row("1080") + row("1081") + row("99") + row("99")));
    REQUIRE(study.constraintRecords.size() == 6);
    CHECK_EQ(study.constraints, 6);
    const int ids[] = {1010, 1012, 1080, 1081, 99, 99};
    const int categories[] = {0, 0, 0, 1, 2, 2};
    for (std::size_t i = 0; i < study.constraintRecords.size(); ++i) {
        CHECK_EQ(study.constraintRecords[i]->id, ids[i]);
        CHECK_EQ(study.constraintRecords[i]->category, categories[i]);
        CHECK(!study.constraintRecords[i]->sourceField10Bits);
    }
    CHECK(study.constraintRecords[4].get() != study.constraintRecords[5].get());
}

TEST_CASE(doe_constraint_record_unqualified_numeric_and_missing_fields_keep_guard)
{
    const std::vector<std::string> unqualified = {
        "<Constraint/>", "<Constraint Id=\"1010\" Value=\"0\" ObjectNo=\"1\"/>",
        row("0"), row("01"), row("-1"), row("2147483648"), row(" 1"),
        row("1", "1"), row("1", "0.0"), row("1", "-0"), row("1", "1e0"),
        row("1", "0", "0"), row("1", "0", "01"), row("1", "0", "1", "0")
    };
    for (const auto& input : unqualified) {
        doe::Study study;
        REQUIRE(readRows(study, row()));
        CHECK(refused([&] { (void)readRows(study, input); }).find("Constraints reader 0x140912d10") != std::string::npos);
        CHECK(study.constraintRecords.empty() && study.constraints == 0);
    }
    doe::Study max;
    REQUIRE(readRows(max, row("2147483647", "0", "2147483647", "2147483647")));
    CHECK_EQ(max.constraintRecords.front()->id, 2147483647);
}

TEST_CASE(doe_constraint_record_later_refusal_retains_prior_row_and_header_effects)
{
    doe::Study study;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"8\"/><Constraints>" + row("1081") +
        row("1010", "1") + "</Constraints><DoeRoiType type=\"9\"/>"), study); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(study.constraintRecords.size() == 1);
    CHECK_EQ(study.constraints, 1);
    CHECK_EQ(study.constraintRecords[0]->id, 1081);
    CHECK_EQ(study.roiType, 8);
}

TEST_CASE(doe_constraint_record_repeated_lists_replace_and_empty_list_clears)
{
    doe::Study study;
    REQUIRE(doe::readInputTable(table("<Constraints>" + row("99") + row("1081") +
        "</Constraints><Constraints><Unknown><Constraint/></Unknown>" + row("1012") +
        "</Constraints>"), study));
    REQUIRE(study.constraintRecords.size() == 1);
    CHECK_EQ(study.constraints, 1);
    CHECK_EQ(study.constraintRecords[0]->id, 1012);
    REQUIRE(readRows(study, ""));
    CHECK(study.constraintRecords.empty() && study.constraints == 0);
}

TEST_CASE(doe_constraint_record_variable_guards_preserve_encountered_ownership)
{
    doe::Study study;
    REQUIRE(readRows(study, row()));
    const auto* original = study.constraintRecords.front().get();
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables><DoeVariable/></DoeVariables><Constraints/>"), study); })
        .find("DoeVariables reader") != std::string::npos);
    CHECK(study.constraintRecords.front().get() == original && study.constraints == 1);
    doe::Var variable;
    variable.number = 0;
    variable.index = 90;
    study.vars = {&variable};
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables/><Constraints>" + row("1081") +
        "</Constraints><DoeVariables><DoeVariable/></DoeVariables>"), study); })
        .find("DoeVariables reader") != std::string::npos);
    REQUIRE(study.constraintRecords.size() == 1);
    CHECK_EQ(study.constraintRecords[0]->id, 1081);
    CHECK_EQ(variable.index, 0);
    CHECK_EQ(variable.number, 0); // Later fault prevents outer ID finalization.
}

TEST_CASE(doe_constraint_record_normal_completion_reaches_outer_id_pass)
{
    doe::Study study;
    doe::Var variable;
    variable.number = 0;
    variable.index = 90;
    study.vars = {&variable};
    REQUIRE(doe::readInputTable(table("<DoeVariables/><Constraints>" + row() + "</Constraints>"), study));
    CHECK_EQ(variable.index, 0);
    CHECK_EQ(variable.number, 1);
    CHECK_EQ(study.constraints, 1);
}

TEST_CASE(doe_constraint_record_actual_study_retains_record_and_ordered_sample_effects)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->constraintRecords.size() == 1);
    CHECK_EQ(owner->constraints, 1);
    CHECK_EQ(owner->constraintRecords.front()->id, 1010);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(doe_constraint_record_actual_save_writes_xml_before_refresh_refusal)
{
    ActiveRig rig;
    rig.read();
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    const auto* record = owner->constraintRecords.front().get();
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(owner->constraintRecords.front().get() == record && owner->constraints == 1);
    CHECK(p.doeIo->textWrites().size() == writesBefore);
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(attempt.opened && attempt.closed);
    CHECK(attempt.errorAfterClose == 0);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    CHECK(attempt.refreshBoundaryReached);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    REQUIRE(attempt.recordedBytesAfterClose.has_value());
    CHECK(*bytes == *attempt.recordedBytesAfterClose);
    xml::Document doc;
    REQUIRE(xml::setContent(doc, *bytes, nullptr, nullptr, nullptr));
    REQUIRE(doc.root != nullptr);
    const auto constraints = doc.root->childElements(qt::L("Constraints"));
    REQUIRE(constraints.size() == 1);
    const auto records = constraints.front()->childElements(qt::L("Constraint"));
    REQUIRE(records.size() == 1);
    CHECK(records.front()->attribute(qt::L("Id")) == qt::L("1010"));
    CHECK(records.front()->attribute(qt::L("Value")) == qt::L("0"));
    CHECK(records.front()->attribute(qt::L("ObjectNo")) == qt::L("1"));
    CHECK(records.front()->attribute(qt::L("RegionNo")) == qt::L("2"));
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
