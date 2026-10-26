// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include <cstdint>
#include <limits>

#include "m2m/engine/report_qobject_ledger.hpp"

using namespace m2m::engine;

namespace {

constexpr const char* qtCore = kReportQObjectQtCoreSha256;

ReportQObjectSharedRefObservation shared(ReportQObjectSharedRefKind kind,
    std::uint32_t rva, std::uint32_t before, std::uint32_t after, std::uintptr_t frame)
{
    return {qtCore, rva, kind, before, after, frame};
}

ReportQObjectSharedRefLedger readyForBacklinks(std::uintptr_t allocation)
{
    ReportQObjectSharedRefLedger ledger(allocation);
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QListData,
        0x002f0a50u, 7, 8, allocation)));
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QString,
        0x002f0b30u, 12, 13, allocation)));
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QVectorData,
        0x002f0bb8u, 3, 4, allocation)));
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QListData,
        0x002f0a50u, 8, 9, allocation)));
    return ledger;
}

} // namespace

TEST_CASE(report_qobject_ledger_records_exact_shared_reference_order_and_wrap)
{
    int allocation = 0;
    ReportQObjectSharedRefLedger ledger(reinterpret_cast<std::uintptr_t>(&allocation));
    CHECK(ledger.valid());
    CHECK(ledger.nextStage() == ReportQObjectLedgerStage::QListFirst);

    const auto frame = reinterpret_cast<std::uintptr_t>(&allocation);
    auto wrongOrder = shared(ReportQObjectSharedRefKind::QString, 0x002f0b30u, 10, 11, frame);
    CHECK(!ledger.observeSharedRef(wrongOrder));
    CHECK(ledger.nextStage() == ReportQObjectLedgerStage::QListFirst);
    auto wrongOwner = shared(ReportQObjectSharedRefKind::QListData, 0x002f0a50u, 10, 11, frame + 1);
    CHECK(!ledger.observeSharedRef(wrongOwner));
    auto wrongModule = shared(ReportQObjectSharedRefKind::QListData, 0x002f0a50u, 10, 11, frame);
    wrongModule.moduleSha256 = "other-module";
    CHECK(!ledger.observeSharedRef(wrongModule));
    auto wrongRva = shared(ReportQObjectSharedRefKind::QListData, 0x002f0a54u, 10, 11, frame);
    CHECK(!ledger.observeSharedRef(wrongRva));
    auto wrongDelta = shared(ReportQObjectSharedRefKind::QListData, 0x002f0a50u, 10, 12, frame);
    CHECK(!ledger.observeSharedRef(wrongDelta));

    auto firstList = shared(ReportQObjectSharedRefKind::QListData, 0x002f0a50u,
        std::numeric_limits<std::uint32_t>::max(), 0u, frame);
    REQUIRE(ledger.observeSharedRef(firstList));
    CHECK(!ledger.observeSharedRef(firstList)); // duplicate cannot advance/commit twice
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QString,
        0x002f0b30u, 20, 21, frame)));
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QVectorData,
        0x002f0bb8u, 30, 31, frame)));
    REQUIRE(ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QListData,
        0x002f0a50u, 40, 41, frame)));

    CHECK(ledger.retainedDelta(ReportQObjectSharedRefKind::QListData) == 2u);
    CHECK(ledger.retainedDelta(ReportQObjectSharedRefKind::QString) == 1u);
    CHECK(ledger.retainedDelta(ReportQObjectSharedRefKind::QVectorData) == 1u);
    REQUIRE(ledger.sharedEvent(0));
    REQUIRE(ledger.sharedEvent(1));
    REQUIRE(ledger.sharedEvent(2));
    REQUIRE(ledger.sharedEvent(3));
    CHECK(ledger.sharedEvent(0)->after == 0u);
    CHECK(ledger.sharedEvent(3)->before == 40u); // no false assumption of adjacent global values
    CHECK(ledger.nextStage() == ReportQObjectLedgerStage::Backlinks);
    CHECK(!ledger.complete());
}

TEST_CASE(report_qobject_ledger_requires_backlinks_and_existing_nonnull_tls_fast_path)
{
    int allocation = 0, object = 0, privateState = 0, other = 0, threadData = 0;
    const auto objectId = reinterpret_cast<std::uintptr_t>(&object);
    const auto privateId = reinterpret_cast<std::uintptr_t>(&privateState);
    const auto otherId = reinterpret_cast<std::uintptr_t>(&other);
    const auto threadId = reinterpret_cast<std::uintptr_t>(&threadData);
    const auto frame = reinterpret_cast<std::uintptr_t>(&allocation);
    auto ledger = readyForBacklinks(frame);

    ReportQObjectBacklinkObservation links{objectId, privateId, otherId, objectId, frame};
    CHECK(!ledger.observeBacklinks(links));
    links.objectPrivatePointer = privateId;
    REQUIRE(ledger.observeBacklinks(links));
    CHECK(!ledger.observeBacklinks(links));

    ReportQObjectTlsObservation tls{qtCore, 0xffffffffu, 0xffffffffu,
        threadId, threadId, threadId, frame};
    CHECK(!ledger.observeTlsFastPath(tls)); // missing-index initialization is out of profile
    tls.tlsIndexBeforeInitializer = 41u;
    tls.tlsGetValueIndex = 42u;
    CHECK(!ledger.observeTlsFastPath(tls));
    tls.tlsGetValueIndex = 41u;
    tls.tlsValue = 0;
    CHECK(!ledger.observeTlsFastPath(tls)); // initialized index with null TLS result is not fast path
    tls.tlsValue = threadId;
    tls.privateThreadData = otherId;
    CHECK(!ledger.observeTlsFastPath(tls));
    tls.privateThreadData = threadId;
    REQUIRE(ledger.observeTlsFastPath(tls));
    CHECK(ledger.nextStage() == ReportQObjectLedgerStage::ThreadReference);
    CHECK(ledger.tlsFastPath()->tlsValue == threadId);
}

TEST_CASE(report_qobject_ledger_requires_thread_ref_then_counter_once)
{
    int allocation = 0, object = 0, privateState = 0, threadData = 0;
    const auto objectId = reinterpret_cast<std::uintptr_t>(&object);
    const auto privateId = reinterpret_cast<std::uintptr_t>(&privateState);
    const auto threadId = reinterpret_cast<std::uintptr_t>(&threadData);
    const auto frame = reinterpret_cast<std::uintptr_t>(&allocation);
    auto ledger = readyForBacklinks(frame);
    REQUIRE(ledger.observeBacklinks({objectId, privateId, privateId, objectId, frame}));
    REQUIRE(ledger.observeTlsFastPath({qtCore, 7, 7, threadId, threadId, threadId, frame}));

    ReportQObjectCounterObservation counter{qtCore, 0x002f4344u, 101, 102, frame};
    CHECK(!ledger.observeProcessCounter(counter)); // process counter is last
    ReportQObjectThreadRefObservation ref{qtCore, 0x00027630u, threadId,
        threadId, threadId, threadId, 5, 6, frame};
    auto wrongHelper = ref;
    wrongHelper.helperRva = 0x00027634u;
    CHECK(!ledger.observeThreadReference(wrongHelper));
    auto wrongIdentity = ref;
    wrongIdentity.refcountAddress = reinterpret_cast<std::uintptr_t>(&allocation);
    CHECK(!ledger.observeThreadReference(wrongIdentity));
    auto wrongDelta = ref;
    wrongDelta.after = 7;
    CHECK(!ledger.observeThreadReference(wrongDelta));
    ref.before = std::numeric_limits<std::uint32_t>::max();
    ref.after = 0;
    REQUIRE(ledger.observeThreadReference(ref));
    CHECK(!ledger.observeThreadReference(ref));

    auto wrongCounter = counter;
    wrongCounter.counterRva = 0x002f4348u;
    CHECK(!ledger.observeProcessCounter(wrongCounter));
    counter.before = 9;
    counter.after = 10;
    REQUIRE(ledger.observeProcessCounter(counter));
    CHECK(ledger.complete());
    CHECK(ledger.nextStage() == ReportQObjectLedgerStage::Complete);
    CHECK(!ledger.observeProcessCounter(counter));
    CHECK(ledger.processCounter()->before == 9u);
}

TEST_CASE(report_qobject_ledger_rejects_missing_allocation_identity)
{
    ReportQObjectSharedRefLedger ledger(0);
    CHECK(!ledger.valid());
    CHECK(!ledger.observeSharedRef(shared(ReportQObjectSharedRefKind::QListData,
        0x002f0a50u, 0, 1, 0)));
    CHECK(!ledger.observeBacklinks({1, 2, 2, 1, 0}));
}

int main() { return testkit::run_all(); }
