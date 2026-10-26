// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/type2_save_node_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
bool unsupported(const auto& work, const char* message = "")
{
    try { work(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(message) != std::string::npos;
    }
    return false;
}
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    Rig()
    {
        auto& p = session.project();
        initProject(p, qt::L("C:/PROBLEM"), qt::L("type2_save_prefix"));
        p.doeIo = makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit writer fixture, not public C ABI selection.
        p.path = qt::L("C:/PROBLEM/type2_save_prefix.moproj");
        p.title = qt::L("reached root update");
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        xml::setAttribute(*oldSub, qt::L("Name"), qt::L("old owned SUB"));
        p.dom->root->children.push_back(oldSub);
    }
};
class MinimalViews final : public moproj::DiagramViews {
public:
    int queried = 0;
    void viewCentre(const SubProject&, double& x, double& y) override { ++queried; x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override
    { ++queried; return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
}

TEST_CASE(type2_save_node_prefix_appends_actual_name_then_stages_empty_xml_device_boundary)
{
    Rig rig;
    auto& p = rig.session.project();
    rig.sub->ownedDoeStudy->name = qt::L("Saved & study");
    const auto io = p.doeIo;
    const auto directoryCount = io->directories().size();
    const auto manager = rig.session.modelManagerOwner();
    const auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    auto pending = xml::createElement(qt::L("Sub_Project"));
    auto preceding = xml::createElement(qt::L("DiagramItem"));
    pending->children.push_back(preceding);
    const auto result = appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending);
    CHECK(result.nextBoundary == Type2SaveNextBoundary::XmlOutputDevice);
    CHECK(std::string(result.guardMessage()) == "Type2 study Save: InputTable XML output device remains unsupported");
    REQUIRE(pending->children.size() == 2);
    CHECK(pending->children[0] == preceding && pending->children[1] == result.appendedStudyNode);
    CHECK(result.appendedStudyNode->tag == qt::L("SfDoeStudy"));
    CHECK(result.appendedStudyNode->attribute(qt::L("name")) == qt::L("Saved & study"));
    CHECK(result.studyOwner.lock().get() == rig.sub->ownedDoeStudy.get());
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(io->directories().size() == directoryCount && io->textWrites().empty() && io->reads().empty());
    CHECK(manager->size() == 0 && !manager->currentModel());
    CHECK(notifier->flag90 && notifier->attempts.empty());
}

TEST_CASE(type2_save_node_prefix_nonempty_count_stops_before_variable_or_current_model_access)
{
    Rig rig;
    auto& p = rig.session.project();
    rig.sub->ownedDoeStudy->vars.push_back(nullptr); // Count-only unavailable-variable fixture.
    auto pending = xml::createElement(qt::L("Sub_Project"));
    const auto result = appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending);
    CHECK(result.nextBoundary == Type2SaveNextBoundary::VariableDimensionReconciliation);
    CHECK(std::string(result.guardMessage()) == "Type2 study Save: InputTable variable dimension reconciliation remains unsupported");
    REQUIRE(pending->children.size() == 1);
    CHECK(pending->children[0] == result.appendedStudyNode);
    CHECK(rig.sub->ownedDoeStudy->vars.size() == 1 && !rig.sub->ownedDoeStudy->vars[0]);
    CHECK(!rig.session.modelManagerOwner()->currentModel() && p.doeIo->textWrites().empty());
    rig.sub->ownedDoeStudy->vars.clear();
}

TEST_CASE(type2_save_node_prefix_preserves_cached_sub_identity_without_reselecting_active)
{
    Rig rig;
    auto& p = rig.session.project();
    p.activeSub = 0; // A supplied, still-owned Save-frame identity remains distinct.
    auto pending = xml::createElement(qt::L("Sub_Project"));
    const auto result = appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending);
    CHECK(result.studyOwner.lock().get() == rig.sub->ownedDoeStudy.get());
    CHECK(p.activeSub == 0 && result.nextBoundary == Type2SaveNextBoundary::XmlOutputDevice);
}

TEST_CASE(type2_save_node_prefix_rejects_foreign_project_sub_and_study_alias_before_append)
{
    Rig rig;
    auto& p = rig.session.project();
    auto pending = xml::createElement(qt::L("Sub_Project"));
    Project foreignProject;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, foreignProject, *rig.sub, *pending); }));
    SubProject foreignSub;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, foreignSub, *pending); }));
    auto* alias = rig.sub->doe;
    rig.sub->doe = nullptr;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    rig.sub->doe = alias;
    rig.sub->ownedDoeStudy->sub = &foreignSub;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    rig.sub->ownedDoeStudy->sub = rig.sub;
    rig.sub->type = 3;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    rig.sub->type = 2;
    CHECK(pending->children.empty());
}

TEST_CASE(type2_save_node_prefix_rejects_missing_or_foreign_io_manager_and_installed_dom_node)
{
    Rig rig, other;
    auto& p = rig.session.project();
    auto pending = xml::createElement(qt::L("Sub_Project"));
    const auto originalIo = p.doeIo;
    p.doeIo.reset();
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    p.doeIo = makeRecordingStudyIo();
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    p.doeIo->bindModelManager(other.session.modelManagerOwner());
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *pending); }));
    p.doeIo = originalIo;
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *rig.oldSub); }, "pending new"));
    auto wrongTag = xml::createElement(qt::L("Project"));
    CHECK(unsupported([&] { appendType2SaveNodePrefix(rig.session, p, *rig.sub, *wrongTag); }, "pending new"));
    CHECK(pending->children.empty() && rig.oldSub->children.empty());
}

TEST_CASE(type2_save_node_prefix_result_does_not_keep_study_alive_after_project_release)
{
    Rig rig;
    auto pending = xml::createElement(qt::L("Sub_Project"));
    const auto result = appendType2SaveNodePrefix(rig.session, rig.session.project(), *rig.sub, *pending);
    rig.session.releaseProjectAfterCloseEffects();
    CHECK(result.studyOwner.expired());
    CHECK(result.appendedStudyNode->attribute(qt::L("name")) == qt::L("DoeStudy7"));
    CHECK(!rig.session.liveProject());
}

TEST_CASE(type2_save_shared_recording_writer_reaches_precise_guard_and_retains_old_sub_dom)
{
    Rig rig;
    auto& p = rig.session.project();
    MinimalViews views;
    engine::FileEffects effects;
    CHECK(unsupported([&] {
        (void)moproj::saveProjectFile(p, false, views, {}, effects, &rig.session);
    }, "main-window +458 is unavailable"));
    CHECK(views.queried == 2);
    const auto installed = p.dom->root->childElements(qt::L("Sub_Project"));
    REQUIRE(installed.size() == 2);
    CHECK(installed[1] == rig.oldSub.get());
    CHECK(rig.oldSub->attribute(qt::L("Name")) == qt::L("old owned SUB"));
    CHECK(rig.oldSub->children.empty());
    CHECK(!p.dom->root->childElements(qt::L("Title")).empty()); // Earlier root mutation survives.
    CHECK(effects.writes.empty() && p.doeIo->textWrites().empty());
    REQUIRE(p.doeIo->xmlWrites().size() == 1);
    CHECK(p.doeIo->xmlWrites().front().opened);
    CHECK(p.doeIo->xmlWrites().front().closed);
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(type2_engine_save_retains_actual_xml_observation_after_refresh_guard)
{
    Rig rig;
    auto& p = rig.session.project();
    p.openedOk = true;
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(unsupported([&] {
        (void)engine::saveProject(p, ctx, views, opened, saved);
    }, "main-window +458 is unavailable"));
    REQUIRE(saved.project.xmlJournal != nullptr);
    CHECK(saved.project.xmlJournal.get() == p.doeIo->xmlJournalOwner().get());
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.opened && attempt.closed);
    CHECK(attempt.path.utf8().find("DoeInputTable.XML") != std::string::npos);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    CHECK(attempt.refreshBoundaryReached);
    REQUIRE(!saved.project.xmlJournal->events().empty());
    CHECK(saved.project.xmlJournal->events().back().kind == XmlJournalEventKind::RefreshBoundaryReached);
    CHECK_EQ(saved.project.xmlJournal->events().back().attempt, 0u);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    CHECK(!bytes->empty());
    CHECK(saved.project.writes.empty()); // Immediate XML cannot be replayed.
}

TEST_CASE(type2_engine_save_keeps_failed_xml_open_before_unconditional_refresh_guard)
{
    Rig rig;
    auto& p = rig.session.project();
    p.openedOk = true;
    RecordingXmlFault fault;
    fault.opened = false;
    fault.label = "injected recording open failure";
    p.doeIo = makeRecordingStudyIo({fault});
    p.doeIo->bindModelManager(rig.session.modelManagerOwner());
    p.doeIo->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(unsupported([&] {
        (void)engine::saveProject(p, ctx, views, opened, saved);
    }, "main-window +458 is unavailable"));
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.openResultObserved && !attempt.opened && !attempt.closed);
    CHECK(attempt.path.utf8().find("DoeInputTable.XML") != std::string::npos);
    CHECK(!attempt.errorAfterClose.has_value());
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(!attempt.normalWriterReturn->writerSucceeded && attempt.refreshBoundaryReached);
    CHECK(attempt.normalWriterReturn->stickyLastError.utf8().find("Failed to open file") != std::string::npos);
    CHECK(saved.project.xmlJournal->events().back().kind == XmlJournalEventKind::RefreshBoundaryReached);
    CHECK_EQ(saved.project.xmlJournal->events().back().attempt, 0u);
    CHECK(!saved.project.xmlJournal->recordedFile(attempt.path).has_value());
    CHECK(p.doeIo->lastError().utf8().find("Failed to open file") != std::string::npos);
    CHECK(saved.project.writes.empty());
}

TEST_CASE(type2_engine_save_keeps_postclose_xml_error_before_unconditional_refresh_guard)
{
    Rig rig;
    auto& p = rig.session.project();
    p.openedOk = true;
    RecordingXmlFault fault;
    fault.errorAfterClose = 1;
    fault.label = "injected recording post-close error";
    p.doeIo = makeRecordingStudyIo({fault});
    p.doeIo->bindModelManager(rig.session.modelManagerOwner());
    p.doeIo->bindSessionNotificationState(rig.session.sessionNotificationStateOwner());
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(unsupported([&] {
        (void)engine::saveProject(p, ctx, views, opened, saved);
    }, "main-window +458 is unavailable"));
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.openResultObserved && attempt.opened && attempt.closed);
    CHECK(attempt.path.utf8().find("DoeInputTable.XML") != std::string::npos);
    CHECK(attempt.errorAfterClose.has_value() && *attempt.errorAfterClose == 1);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(!attempt.normalWriterReturn->writerSucceeded && attempt.refreshBoundaryReached);
    CHECK(attempt.normalWriterReturn->stickyLastError.utf8().find("Error writing file") != std::string::npos);
    CHECK(saved.project.xmlJournal->events().back().kind == XmlJournalEventKind::RefreshBoundaryReached);
    CHECK_EQ(saved.project.xmlJournal->events().back().attempt, 0u);
    CHECK(saved.project.xmlJournal->recordedFile(attempt.path).has_value());
    CHECK(p.doeIo->lastError().utf8().find("Error writing file") != std::string::npos);
    CHECK(saved.project.writes.empty());
}

int main() { return testkit::run_all(); }
