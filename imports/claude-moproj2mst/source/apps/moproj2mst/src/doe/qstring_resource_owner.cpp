#include "m2m/doe_qstring_resource_owner.hpp"

#include <limits>
#include <utility>
#include <vector>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("QString resource owner: ") + reason); }
}

struct QStringResourceProcessScope::Impl {
    std::shared_ptr<const void> scope = std::make_shared<int>(0);
    QStringResourceCodecSnapshot codec;
    QStringPathOwner owner;
    std::vector<std::unique_ptr<QStringResourceOccurrenceSnapshot>> occurrences;
    bool live = true;
};

QStringResourceProcessScope::QStringResourceProcessScope() : impl_(std::make_unique<Impl>()) {}
QStringResourceProcessScope::~QStringResourceProcessScope() = default;

void QStringResourceProcessScope::requireLive() const
{ if (!impl_->live) refuse("process admission is revoked"); }

QStringResourceCodecSnapshot QStringResourceProcessScope::codec() const { return impl_->codec; }

void QStringResourceProcessScope::setIsolatedCodecProfile(
    QStringCStringCodecKnowledge knowledge, const std::string& qualification)
{
    requireLive();
    if (qualification.empty()) refuse("isolated codec profile needs an explicit qualification");
    if (knowledge != QStringCStringCodecKnowledge::Unknown &&
        knowledge != QStringCStringCodecKnowledge::Null && knowledge != QStringCStringCodecKnowledge::NonNull)
        refuse("isolated codec knowledge is invalid");
    if (impl_->codec.epoch == std::numeric_limits<std::uint64_t>::max())
        refuse("codec epoch exhausted");
    auto prepared = qualification; // No partial knowledge/epoch update on host allocation failure.
    impl_->codec.qualification = std::move(prepared);
    impl_->codec.knowledge = knowledge;
    ++impl_->codec.epoch;
}

void QStringResourceProcessScope::revoke() { impl_->live = false; }

QStringResourceIsolatedOccurrence QStringResourceProcessScope::prepareIsolatedOccurrence()
{
    requireLive();
    auto record = std::make_unique<QStringResourceOccurrenceSnapshot>();
    record->generation = impl_->occurrences.size();
    impl_->occurrences.push_back(std::move(record));
    QStringResourceIsolatedOccurrence result;
    result.scope_ = impl_->scope;
    result.ordinal_ = impl_->occurrences.size() - 1;
    result.preparedEpoch_ = impl_->codec.epoch;
    return result;
}

std::size_t QStringResourceProcessScope::checked(const QStringResourceIsolatedOccurrence& token) const
{
    requireLive();
    const auto scope = token.scope_.lock();
    if (!scope || scope != impl_->scope || scope.owner_before(impl_->scope) ||
        impl_->scope.owner_before(scope) || token.ordinal_ >= impl_->occurrences.size())
        refuse("isolated occurrence is unavailable or belongs to another process scope");
    return token.ordinal_;
}

void QStringResourceProcessScope::readIsolatedCodec(QStringResourceIsolatedOccurrence& token)
{
    auto& occurrence = *impl_->occurrences[checked(token)];
    if (occurrence.stage != QStringResourceStage::AwaitingCodecRead)
        refuse("source codec read has already been reached");
    if (token.preparedEpoch_ != impl_->codec.epoch)
        refuse("pre-read codec admission is stale");
    // @src 0x67074126: capture the branch once, before allocation/decoder.
    occurrence.selectedCodec = impl_->codec.knowledge;
    occurrence.selectedCodecEpoch = impl_->codec.epoch;
    if (impl_->codec.knowledge == QStringCStringCodecKnowledge::Unknown) {
        occurrence.stage = QStringResourceStage::CodecStateGuard;
        refuse("invocation-time C-string codec state is unavailable");
    }
    if (impl_->codec.knowledge == QStringCStringCodecKnowledge::NonNull) {
        occurrence.stage = QStringResourceStage::CodecDecoderGuard;
        refuse("actual nonnull C-string codec virtual decoder/data owner is unavailable");
    }
    occurrence.stage = QStringResourceStage::NullBranchSelected;
}

QStringPathData QStringResourceProcessScope::constructIsolatedResource(
    QStringResourceIsolatedOccurrence& token, QStringPathLease& destination,
    QStringResourceAllocationProfile allocation)
{
    auto& occurrence = *impl_->occurrences[checked(token)];
    if (occurrence.stage != QStringResourceStage::NullBranchSelected)
        refuse("resource constructor requires the selected null-codec branch");
    if (destination.state() != QStringPathLeaseState::Empty)
        refuse("resource destination is not an unconstructed source slot");
    if (allocation != QStringResourceAllocationProfile::Success &&
        allocation != QStringResourceAllocationProfile::ReturnNull)
        refuse("isolated allocation profile is invalid");
    // The native branch does not reread the codec after its earlier decision.
    occurrence.nativeAllocationBytesRequested = 104; // @src 0x6706bfad/0x6706bfb2
    occurrence.stage = QStringResourceStage::AllocationRequested;
    if (allocation == QStringResourceAllocationProfile::ReturnNull) {
        occurrence.stage = QStringResourceStage::AllocationReturnedNull;
        // Isolated represented native throw, not a claim the host allocator failed.
        throw Error(M2M_E_UNSUPPORTED, "QString resource owner: isolated source malloc returned null; constructor did not return");
    }
    occurrence.stage = QStringResourceStage::AllocationAccepted;
    // Host metadata failure leaves AllocationAccepted retained, with no completed
    // source slot/return. No implicit native rollback or destructor is claimed.
    const auto data = impl_->owner.constructFixedResourceInput(destination);
    occurrence.data = data;
    occurrence.codeUnitCount = 36;
    occurrence.payloadOffset = 0x1a;
    occurrence.knownFlagMask = 0x1f;
    occurrence.knownFlagValue = 0;
    occurrence.trailingUtf16Nul = true;
    occurrence.stage = QStringResourceStage::ConstructorReturned;
    return data;
}

void QStringResourceProcessScope::constructSourceResource(QStringPathLease&)
{ refuse("actual Session/receiver resource QString source occurrence is unavailable"); }

std::size_t QStringResourceProcessScope::occurrenceCount() const noexcept { return impl_->occurrences.size(); }
QStringResourceOccurrenceSnapshot QStringResourceProcessScope::occurrence(std::size_t ordinal) const
{ return *impl_->occurrences.at(ordinal); }
const QStringPathOwner& QStringResourceProcessScope::pathOwner() const noexcept { return impl_->owner; }
void QStringResourceProcessScope::acquirePathAlias(const QStringPathData& data, QStringPathLease& destination)
{ requireLive(); impl_->owner.acquirePathAlias(data, destination); }
void QStringResourceProcessScope::copyAlias(const QStringPathLease& source, QStringPathLease& destination)
{ requireLive(); impl_->owner.copyAlias(source, destination); }
void QStringResourceProcessScope::release(QStringPathLease& lease)
{ requireLive(); impl_->owner.release(lease); }

} // namespace m2m::doe
