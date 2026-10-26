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
        const String folder((m2m_test::scratchRoot() / "doe-variable-deferred-link").u16string());
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
            qt::L("DeferredLinkStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table("<DoeVariables>" + linked("A", "B") + row("B", "8", "<Description>last</Description>") + "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t5\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("DeferredLinkStudy"));
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

TEST_CASE(doe_variable_deferred_link_resolves_forward_backward_self_and_cycles)
{
    doe::Study study;
    REQUIRE(readRows(study, linked("A", "B") + linked("B", "A", "8") + linked("C", "C", "9")));
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    CHECK(study.vars[0]->link == study.vars[1]);
    CHECK(study.vars[1]->link == study.vars[0]);
    CHECK(study.vars[2]->link == study.vars[2]);
    for (std::size_t i = 0; i < 3; ++i) {
        CHECK(study.vars[i] == study.headerVariableOwners[i].get());
        CHECK_EQ(study.vars[i]->index, static_cast<int>(i));
        CHECK_EQ(study.headerVariableOwners[i]->numSamples, 3);
    }
    CHECK_EQ(study.selectedVariableIndex, 2);
    CHECK(study.unlinkedVars().empty());
}

TEST_CASE(doe_variable_deferred_link_duplicate_name_queues_reader_last_record)
{
    doe::Study study;
    REQUIRE(readRows(study, seedRows()));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    const auto* first = study.vars[0];
    const auto* last = study.vars[1];
    study.samples.invalid.fill(false);
    REQUIRE(readRows(study, linked("A", "A", "9", "<Description>last</Description>")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0] == first && study.vars[1] == last);
    CHECK(first->link == nullptr && last->link == first);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 9);
    CHECK_EQ(study.headerVariableOwners[0]->numSamples, 3);
    CHECK_EQ(study.headerVariableOwners[1]->numSamples, 5);
    CHECK(study.headerVariableOwners[1]->description == qt::L("last"));
    CHECK_EQ(study.selectedVariableIndex, 1);
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(doe_variable_deferred_link_applies_requests_in_order_and_missing_skips_store)
{
    doe::Study study;
    REQUIRE(readRows(study, seedRows() + row("C", "9")));
    REQUIRE(study.vars.size() == 3);
    REQUIRE(readRows(study, linked("C", "A", "10") + linked("C", "B", "11") + linked("C", "Missing", "12")));
    CHECK(study.vars[2]->link == study.vars[1]);
    CHECK(study.vars[0]->link == nullptr && study.vars[1]->link == nullptr);
    CHECK_EQ(study.vars[2]->number, 12);
    REQUIRE(readRows(study, row("C", "13", "", " numSamples=\"3\"", " LinkTo=\"\"")));
    CHECK(study.vars[2]->link == study.vars[1]);
    REQUIRE(readRows(study, linked("C", "a", "14"))); // Exact case-sensitive name lookup.
    CHECK(study.vars[2]->link == study.vars[1]);
    REQUIRE(readRows(study, row("C", "15"))); // Absent LinkTo also retains the old link.
    CHECK(study.vars[2]->link == study.vars[1]);
}

TEST_CASE(doe_variable_deferred_link_queue_lifetime_is_one_container)
{
    doe::Study study;
    REQUIRE(doe::readInputTable(table("<DoeVariables>" + linked("A", "B") +
        "</DoeVariables><DoeVariables>" + row("B", "8") + "</DoeVariables>"), study));
    REQUIRE(study.vars.size() == 2);
    CHECK(study.vars[0]->link == nullptr && study.vars[1]->link == nullptr);
    REQUIRE(readRows(study, linked("A", "B", "9")));
    CHECK(study.vars[0]->link == nullptr && study.vars[1]->link == study.vars[1]);
    // The later duplicate row's source is B; no stale first-container A request survived.
}

TEST_CASE(doe_variable_deferred_link_later_row_guard_discards_pending_links_only)
{
    doe::Study study;
    REQUIRE(readRows(study, linked("A", "B") + row("B", "8")));
    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    study.vars[1]->index = 73;
    const auto error = refused([&] { (void)readRows(study, linked("B", "A", "9", "<Description>kept</Description>") +
        row("B", "10", "<Geometry/>")); });
    CHECK(error.find("Geometry entry 0x140911532") != std::string::npos);
    CHECK(study.vars[0]->link == study.vars[1] && study.vars[1]->link == nullptr);
    CHECK_EQ(study.vars[1]->number, 10); // Later duplicate header mutation occurs before Geometry refusal.
    CHECK_EQ(study.vars[1]->index, 73);
    CHECK(study.headerVariableOwners[1]->description == qt::L("kept"));
}

TEST_CASE(doe_variable_keyword_entry_preserves_header_description_and_unresolved_link)
{
    doe::Study study;
    CHECK(!refused([&] { (void)readRows(study, linked("A", "B", "7",
        "<Description>kept</Description><Keyword/>")); }).empty());
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    REQUIRE(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.headerVariableOwners[0]->name == qt::L("A"));
    CHECK_EQ(study.headerVariableOwners[0]->number, 7);
    CHECK(study.headerVariableOwners[0]->description == qt::L("kept"));
    CHECK(study.vars[0]->link == nullptr); // The queued LinkTo is not finalized after Keyword stops the row.
}

TEST_CASE(doe_variable_deferred_link_later_sibling_guard_preserves_resolved_links)
{
    doe::Study study;
    CHECK(!refused([&] { (void)doe::readInputTable(table("<DoeVariables>" + linked("A", "B") + row("B", "8") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), study); }).empty());
    REQUIRE(study.vars.size() == 2);
    CHECK(study.vars[0]->link == study.vars[1] && study.vars[1]->link == nullptr);
    CHECK_EQ(study.vars[0]->index, 0);
    CHECK_EQ(study.vars[1]->index, 1);
}

TEST_CASE(doe_variable_deferred_link_preserves_payload_numeric_and_foreign_owner_guards)
{
    const std::vector<std::string> guarded = {
        row("A", "7", "", " numSamples=\"0\"", " LinkTo=\"B\""),
        row("A", "7", "", " numSamples=\"01\"", " LinkTo=\"B\"")
    };
    for (const auto& input : guarded) {
        doe::Study study;
        CHECK(!refused([&] { (void)readRows(study, input); }).empty());
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    }
    doe::Study foreign;
    REQUIRE(readRows(foreign, seedRows()));
    REQUIRE(foreign.vars.size() == 2);
    doe::Var borrowed;
    foreign.vars[0] = &borrowed;
    CHECK(!refused([&] { (void)readRows(foreign, linked("B", "A")); }).empty());
    CHECK(foreign.headerVariableOwners[1]->link == nullptr);
    doe::Study wrongMode;
    wrongMode.samples.mode = 1;
    CHECK(!refused([&] { (void)readRows(wrongMode, linked("A", "B")); }).empty());
    CHECK(wrongMode.vars.empty());
}

TEST_CASE(doe_variable_deferred_link_geometry_stops_after_header_before_link_resolution)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    const auto error = refused([&] {
        (void)readRows(study, linked("A", "B", "7", "<Geometry/>"));
    });
    CHECK(error.find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* const source = study.headerVariableOwners.front().get();
    CHECK(study.vars.front() == source);
    CHECK(source->name == qt::L("A"));
    CHECK_EQ(source->number, 7);
    CHECK_EQ(source->numSamples, 3);
    CHECK_EQ(source->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(source->link == nullptr); // queued LinkTo is abandoned when Geometry entry refuses.
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_variable_deferred_link_actual_lazy_owner_then_unchanged_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    REQUIRE(owner->vars.size() == 2 && owner->headerVariableOwners.size() == 2);
    const auto* first = owner->vars[0];
    const auto* last = owner->vars[1];
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK(first->link == last && last->link == nullptr);
    CHECK_EQ(first->number, 7);
    CHECK_EQ(last->number, 8);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0, 5.0}}));
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    const auto writesBefore = p.doeIo->textWrites().size();
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->vars[0] == first && owner->vars[1] == last && first->link == last);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty() && rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }

