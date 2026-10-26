// ctest-labels: m2m-doe
// CP125: actual notifier scalar effects only; key/receiver/public Save stay guarded.
#include <testkit/check.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_constraint_record.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_table_owner_state.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace {
const std::string rows =
    "<Constraint Id=\"1081\" Value=\"0\" ObjectNo=\"3\" RegionNo=\"7\"/>"
    "<Constraint Id=\"1010\" Value=\"0\" ObjectNo=\"5\" RegionNo=\"8\"/>"
    "<Constraint Id=\"1081\" Value=\"0\" ObjectNo=\"9\" RegionNo=\"2\"/>";
std::string table(const std::string& records)
{
    return "<sfDoeInputTable><DoeCommonMeshPath path=\"mesh\"/>"
        "<DoeVariables/><Constraints>" + records + "</Constraints><Operations/></sfDoeInputTable>";
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
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    qt::String path;
    std::string original;
    std::size_t writesBeforeSave = 0;
    explicit Rig(doe::RecordingXmlFault fault = {}, const std::string& records = rows)
    {
        auto& p = session.project();
        const auto folder = qt::L("C:/PROBLEM/constraint_xml");
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        const auto control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ConstraintXmlStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        path = control + qt::L("DoeInputTable.XML");
        fault.expectedPath = path;
        fault.expectedAttemptOrdinal = 0;
        p.doeIo = doe::makeRecordingStudyIo({fault});
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Actual C++ Session fixture; no C ABI selection claim.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        original = table(records);
        CHECK(p.doeIo->writeTextFile(path, original));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ConstraintXmlStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(p, *input, ctx); // Real lazy read creates the retained records.
        writesBeforeSave = p.doeIo->textWrites().size();
    }
    std::string save(engine::SaveEffects& saved)
    {
        auto& p = session.project();
        MinimalViews views;
        Ctx ctx;
        ctx.session = &session;
        engine::FileEffects opened;
        return refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); });
    }
    void checkStopped(const engine::SaveEffects& saved)
    {
        const auto& p = session.project();
        CHECK(p.dom->root->children[1] == oldSub && oldSub->children.empty());
        CHECK(p.doeIo->textWrites().size() == writesBeforeSave);
        CHECK(saved.project.writes.empty());
        CHECK(session.sessionNotificationStateOwner()->attempts.empty());
    }
};
void checkNoReceiverEffects(engine::ConversionSession& session)
{
    const auto* table = session.doeTableOwner();
    REQUIRE(table != nullptr);
    const auto& report = table->report();
    CHECK(!report.timerActive && !report.timerStopped && !report.deleteWidgetEnabled);
    CHECK(!report.nativeReceiverAlive && !report.timerId);
    CHECK(report.receipts.empty() && report.generations.empty());
}
}

TEST_CASE(refresh_notifier_actual_save_retains_unsuppressed_store_after_all_normal_xml_returns)
{
    for (int profile = 0; profile < 3; ++profile) {
        doe::RecordingXmlFault fault;
        if (profile == 1) fault.opened = false;
        if (profile == 2) fault.errorAfterClose = 7;
        Rig rig(fault);
        const auto notifier = rig.session.sessionNotificationStateOwner();
        REQUIRE(notifier != nullptr);
        CHECK(!notifier->suppressed);
        // Explicit diagnostic initial scalar, not a claimed original true producer.
        notifier->flag90 = true;
        const auto keys = notifier->emptyKeys;
        const auto study = rig.sub->ownedDoeStudy;
        REQUIRE(study && study->constraintRecords.size() == 3);
        const auto* record = study->constraintRecords.front().get();
        const auto tables = study->samples.tables;
        const auto invalid = study->samples.invalid;
        engine::SaveEffects saved;
        CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
        rig.checkStopped(saved);
        CHECK(!notifier->flag90 && !notifier->suppressed);
        CHECK(notifier->emptyKeys == keys && notifier->attempts.empty());
        CHECK(rig.session.sessionNotificationStateOwner() == notifier);
        CHECK(rig.sub->ownedDoeStudy == study && study->constraintRecords.front().get() == record);
        CHECK(!record->sourceField10Bits);
        CHECK(study->samples.tables == tables && study->samples.invalid == invalid);
        REQUIRE(saved.project.xmlJournal != nullptr);
        const auto& journal = *saved.project.xmlJournal;
        REQUIRE(journal.attempts().size() == 1);
        const auto& attempt = journal.attempts().front();
        REQUIRE(attempt.normalWriterReturn.has_value());
        CHECK(attempt.normalWriterReturn->writerSucceeded == (profile == 0));
        CHECK(attempt.refreshBoundaryReached && attempt.path == rig.path);
        REQUIRE(journal.events().size() >= 2);
        CHECK(journal.events()[journal.events().size() - 2].kind == doe::XmlJournalEventKind::NormalWriterReturn);
        CHECK(journal.events().back().kind == doe::XmlJournalEventKind::RefreshBoundaryReached);
        if (profile == 1) CHECK(journal.recordedFile(rig.path) == rig.original);
        else {
            REQUIRE(attempt.recordedBytesAfterClose.has_value());
            CHECK(attempt.recordedBytesAfterClose->find("<Constraints>") != std::string::npos);
            CHECK(journal.recordedFile(rig.path) == attempt.recordedBytesAfterClose);
        }
        checkNoReceiverEffects(rig.session);
    }
}

TEST_CASE(refresh_notifier_suppressed_callee_prefix_keeps_flag_and_public_continuation_guard)
{
    for (int initial = 0; initial < 2; ++initial) {
        Rig rig;
        const auto notifier = rig.session.sessionNotificationStateOwner();
        notifier->suppressed = true; // Explicit modeled branch; no startup suppression claim.
        notifier->flag90 = initial != 0;
        const auto keys = notifier->emptyKeys;
        engine::SaveEffects saved;
        CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
        rig.checkStopped(saved);
        CHECK(notifier->suppressed && notifier->flag90 == (initial != 0));
        CHECK(notifier->emptyKeys == keys && notifier->attempts.empty());
        REQUIRE(saved.project.xmlJournal != nullptr);
        REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
        const auto& attempt = saved.project.xmlJournal->attempts().front();
        REQUIRE(attempt.normalWriterReturn.has_value());
        CHECK(attempt.normalWriterReturn->writerSucceeded && attempt.refreshBoundaryReached);
        checkNoReceiverEffects(rig.session);
    }
}

TEST_CASE(refresh_notifier_isolated_prefix_returns_suppressed_without_table_or_store_effects)
{
    for (int initial = 0; initial < 2; ++initial) {
        Rig rig;
        auto* table = rig.session.doeTableOwner();
        REQUIRE(table != nullptr);
        const auto notifier = rig.session.sessionNotificationStateOwner();
        REQUIRE(notifier != nullptr);
        notifier->suppressed = true; // Explicit modeled branch, not a production producer claim.
        notifier->flag90 = initial != 0;
        notifier->emptyKeys.insert("fixture-existing-key");
        const auto keys = notifier->emptyKeys;
        const auto attempts = notifier->attempts;

        CHECK(table->report().tableField == doe::TableOwnerField::Unavailable);
        CHECK(table->enterRefreshNotifierPrefix(rig.session) ==
            doe::RefreshNotifierPrefixOutcome::SuppressedCalleeReturned);
        CHECK(notifier->suppressed && notifier->flag90 == (initial != 0));
        CHECK(notifier->emptyKeys == keys && notifier->attempts.size() == attempts.size());
        CHECK(table->report().generations.empty() && table->report().receipts.empty());
        CHECK(!table->report().timerActive && !table->report().timerStopped &&
              !table->report().nativeReceiverAlive && !table->report().timerId);

        // The helper outcome is not permission to continue the public Save.
        CHECK(refused([&] { table->requireRefreshDelivery(rig.session); })
            .find("main-window +458 is unavailable") != std::string::npos);
        CHECK(notifier->suppressed && notifier->flag90 == (initial != 0));
    }
}

TEST_CASE(refresh_notifier_isolated_prefix_unsuppressed_only_requests_unimplemented_dispatch)
{
    Rig rig;
    auto* table = rig.session.doeTableOwner();
    REQUIRE(table != nullptr);
    const auto notifier = rig.session.sessionNotificationStateOwner();
    REQUIRE(notifier != nullptr);
    notifier->suppressed = false;
    notifier->flag90 = true;
    const auto keys = notifier->emptyKeys;

    CHECK(table->enterRefreshNotifierPrefix(rig.session) ==
        doe::RefreshNotifierPrefixOutcome::UnsuppressedDispatchRequired);
    CHECK(!notifier->flag90 && !notifier->suppressed);
    CHECK(notifier->emptyKeys == keys && notifier->attempts.empty());
    CHECK(table->report().tableField == doe::TableOwnerField::Unavailable);
    CHECK(refused([&] { table->requireRefreshDelivery(rig.session); })
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(!notifier->flag90);
    checkNoReceiverEffects(rig.session);
}

TEST_CASE(refresh_notifier_isolated_prefix_rejects_foreign_or_released_scope_before_effects)
{
    Rig actual, foreign;
    auto* table = actual.session.doeTableOwner();
    REQUIRE(table != nullptr);
    const auto owner = actual.session.sessionNotificationStateOwner();
    const auto other = foreign.session.sessionNotificationStateOwner();
    REQUIRE(owner && other);
    owner->suppressed = other->suppressed = false;
    owner->flag90 = other->flag90 = true;

    CHECK(refused([&] { (void)table->enterRefreshNotifierPrefix(foreign.session); })
        .find("actual Session admission scope is unavailable") != std::string::npos);
    CHECK(owner->flag90 && other->flag90);
    table->releaseAdmissionScope();
    CHECK(refused([&] { (void)table->enterRefreshNotifierPrefix(actual.session); })
        .find("actual Session admission scope is unavailable") != std::string::npos);
    CHECK(owner->flag90 && other->flag90);
}

TEST_CASE(refresh_notifier_interrupted_xml_never_reaches_scalar_prefix)
{
    Rig rig;
    const auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    ++rig.sub->ownedDoeStudy->constraints; // Host owner/count mismatch stops after XML open.
    engine::SaveEffects saved;
    CHECK(rig.save(saved).find("constraint owner remains unsupported after XML open") != std::string::npos);
    rig.checkStopped(saved);
    CHECK(notifier->flag90 && !notifier->suppressed);
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.opened && attempt.closed && attempt.requestedStreamBytes.empty());
    CHECK(!attempt.normalWriterReturn && !attempt.refreshBoundaryReached);
    CHECK(saved.project.xmlJournal->recordedFile(rig.path) == "");
    checkNoReceiverEffects(rig.session);
}

TEST_CASE(refresh_notifier_foreign_or_released_scope_refuses_before_either_notifier_store)
{
    Rig actual, foreign;
    const auto owner = actual.session.sessionNotificationStateOwner();
    const auto other = foreign.session.sessionNotificationStateOwner();
    owner->flag90 = other->flag90 = true;
    auto* table = actual.session.doeTableOwner();
    REQUIRE(table != nullptr);
    CHECK(refused([&] { table->requireRefreshDelivery(foreign.session); })
        .find("actual Session admission scope is unavailable") != std::string::npos);
    CHECK(owner->flag90 && other->flag90);
    CHECK(owner->attempts.empty() && other->attempts.empty());
    table->releaseAdmissionScope();
    CHECK(refused([&] { table->requireRefreshDelivery(actual.session); })
        .find("actual Session admission scope is unavailable") != std::string::npos);
    CHECK(owner->flag90 && other->flag90);
    CHECK(table->report().adapterScopeReleased);
    checkNoReceiverEffects(actual.session);
}

TEST_CASE(refresh_notifier_constructor_field_receipt_does_not_execute_dispatcher_prefix)
{
    Rig rig;
    const auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    auto* table = rig.session.doeTableOwner();
    REQUIRE(table != nullptr);
    doe::TableOwnerObservation observation;
    observation.qualification = "diagnostic source-stage field receipt; no native Qt execution";
    table->record(rig.session, doe::TableOwnerEvent::InitializeFields, observation);
    CHECK(notifier->flag90 && notifier->attempts.empty() && notifier->emptyKeys.empty());
    CHECK(refused([&] { table->requireRefreshDelivery(rig.session); })
        .find("timer/UI receiver delivery remains unsupported") != std::string::npos);
    CHECK(!notifier->flag90 && notifier->attempts.empty() && notifier->emptyKeys.empty());
    CHECK(table->report().receipts.size() == 1);
    CHECK(!table->report().nativeReceiverAlive && !table->report().timerStopped);
}

int main() { return testkit::run_all(); }
