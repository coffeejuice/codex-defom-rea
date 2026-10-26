#include "m2m/doe_qstring_path_owner.hpp"

#include <atomic>
#include <utility>
#include <vector>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("QString path owner: ") + reason); }
}

struct QStringPathOwner::Impl {
    struct Record {
        QStringPathDataKind kind;
        QStringPathDataOrigin origin = QStringPathDataOrigin::ObservationProfile;
        qt::String text;
        std::unique_ptr<std::atomic<std::uint32_t>> reference;
        std::string qualification;
        bool existingInputAdopted = false;
        bool deallocationPending = false;
    };
    std::shared_ptr<const void> scope = std::make_shared<int>(0);
    std::vector<std::unique_ptr<Record>> records;
    std::optional<std::size_t> sharedNull;
};

QStringPathOwner::QStringPathOwner() : impl_(std::make_unique<Impl>()) {}
QStringPathOwner::~QStringPathOwner() = default;

std::size_t QStringPathOwner::checked(const QStringPathData& handle) const
{
    const auto scope = handle.scope_.lock();
    if (!scope || scope != impl_->scope || scope.owner_before(impl_->scope) ||
        impl_->scope.owner_before(scope) || handle.ordinal_ >= impl_->records.size())
        refuse("data owner is unavailable or belongs to another scope");
    return handle.ordinal_;
}

std::size_t QStringPathOwner::checked(const QStringPathLease& lease) const
{
    if (lease.state_ != QStringPathLeaseState::Active)
        refuse("source lease is not active");
    QStringPathData handle;
    handle.scope_ = lease.scope_;
    handle.ordinal_ = lease.ordinal_;
    return checked(handle);
}

QStringPathData QStringPathOwner::observeInput(
    const qt::String& text, const QStringPathRefObservation& observation)
{
    if (observation.qualification.empty()) refuse("explicit input reference observation is unavailable");
    if (text.isNull() || text.isEmpty()) refuse("only an observed nonempty input path is admitted");
    if (observation.referenceCount && *observation.referenceCount == 0)
        refuse("a live observed input needs a nonzero reference");
    auto record = std::make_unique<Impl::Record>();
    record->kind = QStringPathDataKind::Input;
    record->text = text;
    record->qualification = observation.qualification;
    if (observation.referenceCount)
        record->reference = std::make_unique<std::atomic<std::uint32_t>>(*observation.referenceCount);
    QStringPathData handle;
    handle.scope_ = impl_->scope;
    handle.ordinal_ = impl_->records.size();
    impl_->records.push_back(std::move(record));
    return handle;
}

QStringPathData QStringPathOwner::bindSharedNull(const QStringPathSharedNullObservation& observation)
{
    if (observation.moduleSha256 != kQStringPathQtCoreSha256 ||
        observation.symbolRva != kQStringPathSharedNullRva || observation.reference.qualification.empty())
        refuse("actual QString shared-null module/symbol/reference observation is unavailable");
    if (observation.reference.referenceCount && *observation.reference.referenceCount == 0)
        refuse("a live observed shared-null owner needs a nonzero reference");
    if (impl_->sharedNull) refuse("shared-null owner is already bound; replacement is unsupported");
    auto record = std::make_unique<Impl::Record>();
    record->kind = QStringPathDataKind::SharedNull;
    record->qualification = observation.reference.qualification;
    if (observation.reference.referenceCount)
        record->reference = std::make_unique<std::atomic<std::uint32_t>>(*observation.reference.referenceCount);
    QStringPathData handle;
    handle.scope_ = impl_->scope;
    handle.ordinal_ = impl_->records.size();
    impl_->records.push_back(std::move(record));
    impl_->sharedNull = handle.ordinal_;
    return handle;
}

std::optional<QStringPathData> QStringPathOwner::sharedNull() const
{
    if (!impl_->sharedNull) return {};
    QStringPathData handle;
    handle.scope_ = impl_->scope;
    handle.ordinal_ = *impl_->sharedNull;
    return handle;
}

QStringPathDataSnapshot QStringPathOwner::snapshot(const QStringPathData& handle) const
{
    const auto& record = *impl_->records[checked(handle)];
    return {record.kind, record.reference ? std::optional<std::uint32_t>(record.reference->load()) : std::nullopt,
            record.deallocationPending, record.qualification, record.origin};
}

QStringPathData QStringPathOwner::constructFixedResourceInput(QStringPathLease& destination)
{
    if (destination.state() != QStringPathLeaseState::Empty)
        refuse("resource constructor destination is not an unconstructed source slot");
    auto record = std::make_unique<Impl::Record>();
    record->kind = QStringPathDataKind::Input;
    record->origin = QStringPathDataOrigin::ResourceNullCodecProjection;
    record->text = qt::L(":/SfTool/icon/tool/iconDoe_24_24.png");
    record->qualification = "isolated source-qualified null-codec resource constructor; no original runtime identity";
    // @src 0x6706bfda: this new source allocation initializes its ref word to1.
    // It is the canonical cell for this projection, not an observed count copy.
    record->reference = std::make_unique<std::atomic<std::uint32_t>>(1);
    QStringPathData handle;
    handle.scope_ = impl_->scope;
    handle.ordinal_ = impl_->records.size();
    impl_->records.push_back(std::move(record));
    // @src 0x67174be7: publish the completed source slot without incrementing.
    acquire(handle.ordinal_, destination, false);
    impl_->records[handle.ordinal_]->existingInputAdopted = true;
    return handle;
}

const qt::String& QStringPathOwner::text(const QStringPathData& handle) const
{ return impl_->records[checked(handle)]->text; }

QStringPathData QStringPathOwner::data(const QStringPathLease& lease) const
{
    (void)checked(lease);
    QStringPathData handle;
    handle.scope_ = lease.scope_;
    handle.ordinal_ = lease.ordinal_;
    return handle;
}

void QStringPathOwner::acquire(std::size_t ordinal, QStringPathLease& destination, bool increment)
{
    if (destination.state_ != QStringPathLeaseState::Empty)
        refuse("destination is not an unconstructed QString slot");
    auto& record = *impl_->records[ordinal];
    if (!record.reference) refuse("native reference count is unavailable");
    if (record.deallocationPending) refuse("QString data deallocation remains unsupported");
    // All scope checks precede destination mutation. These assignments do not
    // allocate; source data identity is published before the native ref change.
    destination.scope_ = impl_->scope;
    destination.ordinal_ = ordinal;
    destination.state_ = QStringPathLeaseState::Active;
    if (increment) record.reference->fetch_add(1);
}

void QStringPathOwner::adoptExistingInput(const QStringPathData& handle, QStringPathLease& destination)
{
    const auto ordinal = checked(handle);
    auto& record = *impl_->records[ordinal];
    if (record.kind != QStringPathDataKind::Input || record.existingInputAdopted)
        refuse("existing input slot is unavailable or already adopted");
    if (record.reference && record.reference->load() == 0)
        refuse("observed existing input slot needs a nonzero reference");
    acquire(ordinal, destination, false);
    record.existingInputAdopted = true;
}

void QStringPathOwner::acquirePathAlias(const QStringPathData& handle, QStringPathLease& destination)
{
    const auto ordinal = checked(handle);
    const auto& record = *impl_->records[ordinal];
    if (record.kind != QStringPathDataKind::Input || record.text.u16().find(u'\\') != std::u16string::npos)
        refuse("only the input no-backslash QString alias branch is recovered");
    acquire(ordinal, destination, true);
}

void QStringPathOwner::copyAlias(const QStringPathLease& source, QStringPathLease& destination)
{ acquire(checked(source), destination, true); }

void QStringPathOwner::acquireSharedNull(QStringPathLease& destination)
{
    if (!impl_->sharedNull) refuse("actual shared-null owner is unavailable");
    acquire(*impl_->sharedNull, destination, true);
}

void QStringPathOwner::release(QStringPathLease& lease)
{
    const auto ordinal = checked(lease);
    auto& record = *impl_->records[ordinal];
    if (!record.reference) refuse("native reference count is unavailable");
    if (record.deallocationPending) refuse("QString data deallocation remains unsupported");
    const auto before = record.reference->fetch_sub(1);
    lease.state_ = QStringPathLeaseState::Released;
    if (before == 1) {
        record.deallocationPending = true;
        refuse("QString data deallocation remains unsupported after reference reached zero");
    }
}

} // namespace m2m::doe
