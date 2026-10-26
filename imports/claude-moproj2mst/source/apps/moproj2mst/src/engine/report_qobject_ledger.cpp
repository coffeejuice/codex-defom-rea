#include "m2m/engine/report_qobject_ledger.hpp"

namespace m2m::engine {
namespace {

constexpr std::uint32_t kQListDataRva = 0x002f0a50u;
constexpr std::uint32_t kQStringRva = 0x002f0b30u;
constexpr std::uint32_t kQVectorDataRva = 0x002f0bb8u;
constexpr std::uint32_t kThreadRefHelperRva = 0x00027630u;
constexpr std::uint32_t kQObjectCounterRva = 0x002f4344u;

bool qualifiedQtCore(const std::string& sha256) noexcept
{
    return sha256 == kReportQObjectQtCoreSha256;
}

bool incrementedByOne(std::uint32_t before, std::uint32_t after) noexcept
{
    return after == before + std::uint32_t{1};
}

std::size_t deltaIndex(ReportQObjectSharedRefKind kind) noexcept
{
    switch (kind) {
    case ReportQObjectSharedRefKind::QListData: return 0;
    case ReportQObjectSharedRefKind::QString: return 1;
    case ReportQObjectSharedRefKind::QVectorData: return 2;
    }
    return 3;
}

bool expectedSharedEvent(ReportQObjectLedgerStage stage,
    ReportQObjectSharedRefKind kind, std::uint32_t symbolRva) noexcept
{
    switch (stage) {
    case ReportQObjectLedgerStage::QListFirst:
    case ReportQObjectLedgerStage::QListSecond:
        return kind == ReportQObjectSharedRefKind::QListData && symbolRva == kQListDataRva;
    case ReportQObjectLedgerStage::QString:
        return kind == ReportQObjectSharedRefKind::QString && symbolRva == kQStringRva;
    case ReportQObjectLedgerStage::QVector:
        return kind == ReportQObjectSharedRefKind::QVectorData && symbolRva == kQVectorDataRva;
    default:
        return false;
    }
}

ReportQObjectLedgerStage nextSharedStage(ReportQObjectLedgerStage stage) noexcept
{
    switch (stage) {
    case ReportQObjectLedgerStage::QListFirst: return ReportQObjectLedgerStage::QString;
    case ReportQObjectLedgerStage::QString: return ReportQObjectLedgerStage::QVector;
    case ReportQObjectLedgerStage::QVector: return ReportQObjectLedgerStage::QListSecond;
    case ReportQObjectLedgerStage::QListSecond: return ReportQObjectLedgerStage::Backlinks;
    default: return stage;
    }
}

} // namespace

ReportQObjectSharedRefLedger::ReportQObjectSharedRefLedger(
    std::uintptr_t stableAllocationIdentity) noexcept
    : allocationIdentity_(stableAllocationIdentity)
{}

bool ReportQObjectSharedRefLedger::observeSharedRef(
    const ReportQObjectSharedRefObservation& observation)
{
    if (!valid() || !qualifiedQtCore(observation.moduleSha256) ||
        observation.constructorIdentity != allocationIdentity_ ||
        !expectedSharedEvent(stage_, observation.kind, observation.symbolRva) ||
        !incrementedByOne(observation.before, observation.after) || sharedEventCount_ >= sharedEvents_.size())
        return false;

    const std::size_t index = deltaIndex(observation.kind);
    if (index >= retainedDeltas_.size()) return false;
    sharedEvents_[sharedEventCount_++] = observation;
    ++retainedDeltas_[index];
    stage_ = nextSharedStage(stage_);
    return true;
}

bool ReportQObjectSharedRefLedger::observeBacklinks(
    const ReportQObjectBacklinkObservation& observation) noexcept
{
    if (!valid() || stage_ != ReportQObjectLedgerStage::Backlinks || hasBacklinks_ ||
        observation.constructorIdentity != allocationIdentity_ ||
        !observation.objectIdentity || !observation.privateIdentity ||
        observation.objectPrivatePointer != observation.privateIdentity ||
        observation.privateObjectPointer != observation.objectIdentity)
        return false;
    backlinks_ = observation;
    hasBacklinks_ = true;
    stage_ = ReportQObjectLedgerStage::ThreadSelection;
    return true;
}

bool ReportQObjectSharedRefLedger::observeTlsFastPath(
    const ReportQObjectTlsObservation& observation)
{
    if (!valid() || stage_ != ReportQObjectLedgerStage::ThreadSelection || hasTls_ ||
        observation.constructorIdentity != allocationIdentity_ ||
        !qualifiedQtCore(observation.moduleSha256) ||
        observation.tlsIndexBeforeInitializer == 0xffffffffu ||
        observation.tlsGetValueIndex != observation.tlsIndexBeforeInitializer ||
        !observation.tlsValue || observation.currentThreadDataReturn != observation.tlsValue ||
        observation.privateThreadData != observation.tlsValue)
        return false;
    tls_ = observation;
    hasTls_ = true;
    stage_ = ReportQObjectLedgerStage::ThreadReference;
    return true;
}

bool ReportQObjectSharedRefLedger::observeThreadReference(
    const ReportQObjectThreadRefObservation& observation)
{
    if (!valid() || stage_ != ReportQObjectLedgerStage::ThreadReference || hasThreadRef_ || !hasTls_ ||
        observation.constructorIdentity != allocationIdentity_ ||
        !qualifiedQtCore(observation.moduleSha256) || observation.helperRva != kThreadRefHelperRva ||
        !observation.selectedThreadData || observation.selectedThreadData != tls_.tlsValue ||
        observation.privateThreadData != observation.selectedThreadData ||
        observation.helperArgument != observation.selectedThreadData ||
        observation.refcountAddress != observation.helperArgument ||
        !incrementedByOne(observation.before, observation.after))
        return false;
    threadRef_ = observation;
    hasThreadRef_ = true;
    stage_ = ReportQObjectLedgerStage::ProcessCounter;
    return true;
}

bool ReportQObjectSharedRefLedger::observeProcessCounter(
    const ReportQObjectCounterObservation& observation)
{
    if (!valid() || stage_ != ReportQObjectLedgerStage::ProcessCounter || hasCounter_ || !hasThreadRef_ ||
        observation.constructorIdentity != allocationIdentity_ ||
        !qualifiedQtCore(observation.moduleSha256) || observation.counterRva != kQObjectCounterRva ||
        !incrementedByOne(observation.before, observation.after))
        return false;
    counter_ = observation;
    hasCounter_ = true;
    stage_ = ReportQObjectLedgerStage::Complete;
    return true;
}

std::uint32_t ReportQObjectSharedRefLedger::retainedDelta(
    ReportQObjectSharedRefKind kind) const noexcept
{
    const std::size_t index = deltaIndex(kind);
    return index < retainedDeltas_.size() ? retainedDeltas_[index] : 0;
}

const ReportQObjectSharedRefObservation* ReportQObjectSharedRefLedger::sharedEvent(
    std::size_t index) const noexcept
{
    return index < sharedEventCount_ ? &sharedEvents_[index] : nullptr;
}

const ReportQObjectBacklinkObservation* ReportQObjectSharedRefLedger::backlinks() const noexcept
{ return hasBacklinks_ ? &backlinks_ : nullptr; }

const ReportQObjectTlsObservation* ReportQObjectSharedRefLedger::tlsFastPath() const noexcept
{ return hasTls_ ? &tls_ : nullptr; }

const ReportQObjectThreadRefObservation* ReportQObjectSharedRefLedger::threadReference() const noexcept
{ return hasThreadRef_ ? &threadRef_ : nullptr; }

const ReportQObjectCounterObservation* ReportQObjectSharedRefLedger::processCounter() const noexcept
{ return hasCounter_ ? &counter_ : nullptr; }

} // namespace m2m::engine
