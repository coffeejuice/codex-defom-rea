// ctest-labels: m2m-mdt report-document-slot
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/report_document_slot.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
SubProject& addSub(Project& project, int id = 0)
{
    project.subs.push_back(makeSubProject(project, 0, id));
    return *project.subs.back();
}
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}
static_assert(!std::is_copy_constructible_v<ReportDocumentSlot>);
static_assert(!std::is_move_constructible_v<ReportDocumentSlot>);
static_assert(std::is_nothrow_default_constructible_v<ReportDocumentSlot>);
static_assert(!std::is_copy_constructible_v<ReportDocumentReleaseProgress>);
static_assert(!std::is_move_constructible_v<ReportDocumentReleaseProgress>);
static_assert(std::is_same_v<decltype(std::declval<const ReportDocumentSlot&>()
    .documentIdentityToken()), std::uintptr_t>);
static_assert(!ReportDocumentReleaseResult::documentDeleteExecuted);
static_assert(!ReportDocumentReleaseResult::guardAssignmentExecuted);
static_assert(!ReportDocumentReleaseResult::nativeSubProjectCleanupComplete);
static_assert(!ReportDocumentReleaseResult::completeForPublicClose);

TEST_CASE(report_document_actual_subproject_slot_starts_unknown_not_native_null)
{
    Project project;
    auto& sub = addSub(project);
    CHECK(sub.project == &project);
    CHECK(sub.reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::Unknown);
    CHECK(sub.reportDocumentSlot.documentIdentityToken() == 0);
    CHECK(sub.reportDocumentSlot.qualification().empty());
    CHECK(!sub.reportDocumentSlot.hasDiagnosticScope());
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence,
        "declared helper entry; no accepted-Close/root evidence");
    CHECK(sub.reportDocumentSlot.hasDiagnosticScope());
    const auto result = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(result.stop == ReportDocumentReleaseStop::GuardValueUnavailable);
    CHECK(!result.guardReadThisAttempt);
    CHECK(progress.stage() == ReportDocumentReleaseStage::AwaitGuardRead);
    CHECK(std::string(result.nextSourceSite).find("0x1400b5bc2") == 0);
}

TEST_CASE(report_document_normal_default_subprojects_do_not_allocate_diagnostic_scope)
{
    ReportDocumentSlot detached;
    SubProject ordinary;
    Project project;
    auto& first = addSub(project, 1);
    auto& second = addSub(project, 2);
    CHECK(!detached.hasDiagnosticScope());
    CHECK(!ordinary.reportDocumentSlot.hasDiagnosticScope());
    CHECK(!first.reportDocumentSlot.hasDiagnosticScope());
    CHECK(!second.reportDocumentSlot.hasDiagnosticScope());
    CHECK(first.reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::Unknown);
    first.reportDocumentSlot.observeInitialNull("first explicit diagnostic observation");
    CHECK(first.reportDocumentSlot.hasDiagnosticScope());
    CHECK(!second.reportDocumentSlot.hasDiagnosticScope());
}

TEST_CASE(report_document_known_null_first_read_returns_without_delete_or_guard_clear)
{
    Project project;
    auto& sub = addSub(project);
    REQUIRE(sub.reportDocumentSlot.observeInitialNull("declared actual+30 null at helper entry"));
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence,
        "declared initial-null helper invocation");
    const auto result = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(result.stop == ReportDocumentReleaseStop::NullGuardReturnReached);
    CHECK(result.guardReadThisAttempt && result.observedDocumentIdentity == 0);
    CHECK(progress.stage() == ReportDocumentReleaseStage::NullGuardReturn);
    CHECK(progress.consumedKnowledge() == ReportDocumentSlotKnowledge::KnownNull);
    CHECK(std::string(result.nextSourceSite).find("0x1400b5c26") == 0);
    CHECK(!result.documentDeleteExecuted && !result.guardAssignmentExecuted);
    CHECK(sub.reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::KnownNull);
    CHECK(sub.reportDocumentSlot.qualification() == "declared actual+30 null at helper entry");
    const auto retry = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(retry.stop == result.stop && !retry.guardReadThisAttempt);
}

TEST_CASE(report_document_unknown_can_later_receive_explicit_null_before_branch_consumption)
{
    Project project;
    auto& sub = addSub(project);
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence, "declared entry");
    attemptReportDocumentReleasePrefix(sub, progress);
    sub.reportDocumentSlot.observeInitialNull("later explicit initial null observation");
    const auto result = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(result.stop == ReportDocumentReleaseStop::NullGuardReturnReached);
    CHECK(result.guardReadThisAttempt);
}

TEST_CASE(report_document_nonnull_opaque_target_stops_before_vtable_and_preserves_identity)
{
    Project project;
    auto& sub = addSub(project);
    REQUIRE(sub.reportDocumentSlot.observeInitialNonNull(0x1234u,
        "declared nonnull identity; actual document/vtable/lifetime unresolved"));
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence, "declared entry");
    const auto result = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(result.stop == ReportDocumentReleaseStop::DocumentDeletingVirtualUnavailable);
    CHECK(result.guardReadThisAttempt && result.observedDocumentIdentity == 0x1234u);
    CHECK(progress.stage() == ReportDocumentReleaseStage::BeforeDocumentDeletingVirtual);
    CHECK(progress.observedDocumentIdentityToken() == 0x1234u);
    CHECK(!result.documentDeleteExecuted && !result.guardAssignmentExecuted);
    CHECK(std::string(result.nextSourceSite).find("0x1400b5bf6") == 0);
    const auto retry = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(retry.stop == result.stop && !retry.guardReadThisAttempt);
    CHECK(retry.observedDocumentIdentity == 0x1234u);
}

TEST_CASE(report_document_blocked_nonnull_cannot_be_bypassed_by_null_or_replacement_observation)
{
    Project project;
    auto& sub = addSub(project);
    sub.reportDocumentSlot.observeInitialNonNull(7, "first nonnull");
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence, "declared entry");
    attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(refuses([&] { sub.reportDocumentSlot.observeInitialNull("external null"); }, "cannot replace"));
    CHECK(refuses([&] { sub.reportDocumentSlot.observeInitialNonNull(8, "replacement"); }, "rebound"));
    const auto retry = attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(retry.stop == ReportDocumentReleaseStop::DocumentDeletingVirtualUnavailable);
    CHECK(retry.observedDocumentIdentity == 7 && !retry.guardReadThisAttempt);
    CHECK(sub.reportDocumentSlot.qualification() == "first nonnull");
}

TEST_CASE(report_document_idempotent_observations_keep_receipts_and_reject_conflicting_null_upgrade)
{
    Project project;
    auto& first = addSub(project, 1);
    auto& second = addSub(project, 2);
    first.reportDocumentSlot.observeInitialNull("first null");
    CHECK(!first.reportDocumentSlot.observeInitialNull("later null"));
    CHECK(refuses([&] { first.reportDocumentSlot.observeInitialNonNull(9, "upgrade"); }, "rebound"));
    second.reportDocumentSlot.observeInitialNonNull(11, "first document");
    CHECK(!second.reportDocumentSlot.observeInitialNonNull(11, "later document"));
    CHECK(first.reportDocumentSlot.qualification() == "first null");
    CHECK(second.reportDocumentSlot.qualification() == "first document");
}

TEST_CASE(report_document_release_uses_actual_member_not_detached_null_or_foreign_subproject)
{
    Project project;
    auto& first = addSub(project, 1);
    auto& second = addSub(project, 2);
    ReportDocumentSlot detached;
    detached.observeInitialNull("unrelated detached null state");
    ReportDocumentReleaseProgress progress(first, reportDocumentReleaseSourceOccurrence, "declared entry");
    CHECK(attemptReportDocumentReleasePrefix(first, progress).stop ==
        ReportDocumentReleaseStop::GuardValueUnavailable);
    second.reportDocumentSlot.observeInitialNull("other actual slot");
    CHECK(refuses([&] { attemptReportDocumentReleasePrefix(second, progress); }, "slot scope"));
    CHECK(progress.stage() == ReportDocumentReleaseStage::AwaitGuardRead);
    CHECK(first.reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::Unknown);
}

TEST_CASE(report_document_expired_subproject_scope_rejects_same_address_reuse)
{
    std::optional<SubProject> frame;
    frame.emplace();
    const auto address = reinterpret_cast<std::uintptr_t>(&*frame);
    frame->reportDocumentSlot.observeInitialNonNull(17, "old nonnull slot");
    std::optional<ReportDocumentReleaseProgress> progress;
    progress.emplace(*frame, reportDocumentReleaseSourceOccurrence, "old helper invocation");
    attemptReportDocumentReleasePrefix(*frame, *progress);
    frame.reset(); // adapter storage teardown, never a native document delete
    frame.emplace();
    REQUIRE(reinterpret_cast<std::uintptr_t>(&*frame) == address);
    frame->reportDocumentSlot.observeInitialNull("replacement slot");
    CHECK(refuses([&] { attemptReportDocumentReleasePrefix(*frame, *progress); }, "slot scope"));
    CHECK(progress->observedDocumentIdentityToken() == 17);
    CHECK(progress->stage() == ReportDocumentReleaseStage::BeforeDocumentDeletingVirtual);
}

TEST_CASE(report_document_progress_binding_captures_scope_before_first_attempt)
{
    std::optional<SubProject> frame;
    frame.emplace();
    CHECK(!frame->reportDocumentSlot.hasDiagnosticScope());
    const auto address = reinterpret_cast<std::uintptr_t>(&*frame);
    std::optional<ReportDocumentReleaseProgress> progress;
    progress.emplace(*frame, reportDocumentReleaseSourceOccurrence, "explicit diagnostic binding only");
    CHECK(frame->reportDocumentSlot.hasDiagnosticScope());
    CHECK(frame->reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::Unknown);
    frame.reset();
    frame.emplace();
    REQUIRE(reinterpret_cast<std::uintptr_t>(&*frame) == address);
    frame->reportDocumentSlot.observeInitialNull("replacement before old progress first attempt");
    CHECK(refuses([&] { attemptReportDocumentReleasePrefix(*frame, *progress); }, "slot scope"));
    CHECK(progress->stage() == ReportDocumentReleaseStage::AwaitGuardRead);
}

TEST_CASE(report_document_prefix_preserves_earlier_subproject_and_project_storage)
{
    unsigned dbDeletions = 0;
    Project project;
    auto& sub = addSub(project);
    sub.ownDb = std::shared_ptr<DbHandle>(new DbHandle(), [&](DbHandle* db) { ++dbDeletions; delete db; });
    const auto db = sub.ownDb;
    sub.loaded = true;
    sub.dbName = qt::L("retained.DB");
    const auto flowItems = sub.flow.items.size();
    const auto slots = project.modelSlotsOwner();
    sub.reportDocumentSlot.observeInitialNull("declared null helper entry with earlier+48 still live");
    ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence, "direct helper only");
    attemptReportDocumentReleasePrefix(sub, progress);
    CHECK(dbDeletions == 0 && sub.ownDb == db);
    CHECK(sub.loaded && sub.dbName == qt::L("retained.DB"));
    CHECK(sub.project == &project && project.subs.size() == 1);
    CHECK(sub.flow.items.size() == flowItems);
    CHECK(slots->projectStorageAlive() && slots->nextStorageBoundary() == MoModelSlots::Slot::Operations);
}

TEST_CASE(report_document_slot_and_entry_reject_missing_qualification_without_mutation)
{
    Project project;
    auto& sub = addSub(project);
    CHECK(refuses([&] { sub.reportDocumentSlot.observeInitialNull(" \t\r\n"); }, "qualification"));
    CHECK(refuses([&] { sub.reportDocumentSlot.observeInitialNonNull(0, "zero"); }, "nonzero"));
    CHECK(refuses([&] {
        ReportDocumentReleaseProgress progress(sub, "another source", "entry");
    }, "occurrence"));
    CHECK(refuses([&] {
        ReportDocumentReleaseProgress progress(sub, reportDocumentReleaseSourceOccurrence, " ");
    }, "qualification"));
    CHECK(sub.reportDocumentSlot.knowledge() == ReportDocumentSlotKnowledge::Unknown);
    CHECK(sub.reportDocumentSlot.documentIdentityToken() == 0 && sub.reportDocumentSlot.qualification().empty());
    CHECK(!sub.reportDocumentSlot.hasDiagnosticScope());
}

int main() { return testkit::run_all(); }
