#pragma once

#include <cstddef>
#include <memory>
#include <optional>

#include "m2m/doe_task_runtime.hpp"

namespace m2m::engine { class ConversionSession; class WidgetRuntimeState; struct WidgetFontValue; }

namespace m2m::doe {
class StudyIo;
namespace detail { struct TaskFactoryTestAccess; }

// @src 0x651f3560, used at TaskInfo private constructor 0x14140e989.
// Isolated value projection: retain supplied FontData ownership and reset only
// the destination mask. No current-font query, native refcount or TaskInfo commit.
engine::WidgetFontValue isolatedTaskInfoDefaultFontValue(
    const engine::WidgetFontValue& firstCurrentApplicationFont);

// @src 0x651f3060, used at TaskInfo private constructor 0x14140ea5c.
// Isolated second-font value assignment: copy the supplied owner AND mask, even
// when data is unchanged. This does not query QApplication or complete TaskInfo.
void assignIsolatedTaskInfoSecondFontValue(engine::WidgetFontValue& destination,
    const engine::WidgetFontValue& secondCurrentApplicationFont);

// Isolated accessor contract only, not a native QSettings backend or a completed
// Task constructor. Value methods supply the already-converted result; QVariant
// coercion, settings ownership and key encoding are not supplied by this seam.
class IsolatedTaskPreferenceSource {
public:
    virtual ~IsolatedTaskPreferenceSource() = default;
    virtual bool contains(const qt::String& key) const = 0;
    virtual qt::String stringValue(const qt::String& key) const = 0;
    virtual bool boolValue(const qt::String& key) const = 0;
    virtual int intValue(const qt::String& key) const = 0;
};

// 808860/808270/8085c0: absence returns false without reading a value or changing
// the existing destination. A present empty/false/zero value still assigns.
// @src 0x140808860 (isolated presence/assignment contract only)
bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, qt::String& destination);
// @src 0x140808270 (isolated presence/assignment contract only)
bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, bool& destination);
// @src 0x1408085c0 (isolated presence/assignment contract only)
bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, int& destination);

enum class TaskFactoryProfile { Native, BoundedParsedDoeProjection };

struct TaskFactoryConstructionSnapshot {
    bool factoryCommitted = false;
    bool dataCommitted = false;
    bool dataFactoryBacklinkMatches = false;
    std::size_t nextPrototypeOrdinal = 0;
    std::size_t metadataCacheCount = 0;
    std::size_t taskCacheCount = 0;
    std::optional<std::size_t> pendingReportAllocation;
    bool nativeInitializationComplete = false;
};

// The holder belongs to the actual main-window adapter, not to a Project or
// SessionRecord. Constructing it does not construct native mainwindow+4a0.
class TaskFactoryRuntime final {
public:
    explicit TaskFactoryRuntime(const std::shared_ptr<TaskAllocationStore>& allocations,
        const std::shared_ptr<engine::WidgetRuntimeState>& widgets = {});
    ~TaskFactoryRuntime();
    TaskFactoryRuntime(const TaskFactoryRuntime&) = delete;
    TaskFactoryRuntime& operator=(const TaskFactoryRuntime&) = delete;

    // Explicit isolated-fixture projection only; never the production default.
    static std::shared_ptr<TaskFactoryRuntime> boundedParsedDoeProjection(
        const std::shared_ptr<TaskAllocationStore>& allocations);
    TaskFactoryProfile profile() const noexcept { return profile_; }
    bool nativeReceiverAlive() const noexcept { return adapterScopeLive_; }
    void releaseAdapterScope() noexcept;
    TaskObserver createTaskBase(const qt::String& type);
    TaskFactoryConstructionSnapshot construction() const noexcept;
    std::shared_ptr<engine::WidgetRuntimeState> widgetRuntimeOwner() const noexcept
    { return widgets_.lock(); }

private:
    friend struct detail::TaskFactoryTestAccess;
    friend class engine::ConversionSession;
    void bindSessionReportNullCaller();
    void validateSessionReportNullCaller() const;
    // Eligibility survives removal of either nullable handle and scope release.
    bool sessionReportSourceEligible_ = false;
    bool sessionReportFrameIssued_ = false;
    std::shared_ptr<const SessionReportNullBinding> sessionReportCapability_;
    std::weak_ptr<const SessionReportNullBinding> sessionReportIssuedCapability_;
    std::shared_ptr<const SessionReportNullBinding> sessionReportFrameBinding_;
    // Independent issuance anchor; mutable call/slot records cannot appoint it.
    std::shared_ptr<const unsigned char> issuedNullParentCallOccurrence_;
    std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::CalleePrefixState> issuedCalleePrefix_;
    std::size_t calleePrefixStopOrdinal_ = 10;
    void validateCalleePrefix(const PendingTaskPrototype::QObjectPrefix&) const;
    void validateCalleePrefixHistory(const PendingTaskPrototype::QObjectPrefix&,std::size_t) const;
    void advanceTlsIndexCompare(PendingTaskPrototype::QObjectPrefix&);
    void advanceTlsIndexJneUnresolved(PendingTaskPrototype::QObjectPrefix&);
    void validateTlsIndexCompareHistory(const PendingTaskPrototype::QObjectPrefix&) const;
    void validateTlsIndexJneUnresolved(const PendingTaskPrototype::QObjectPrefix&) const;
    void validateTlsIndexCompare(const PendingTaskPrototype::QObjectPrefix&) const;
    engine::TlsIndexReadExpression::Context tlsReadContext(const PendingTaskPrototype::QObjectPrefix&) const;
    std::shared_ptr<const engine::TlsIndexSessionGrant> tlsIndexGrant_;
    std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::TlsIndexCompare> issuedTlsIndexCompare_;
    bool tlsIndexCompareEnabled_=true;
    bool failTlsComparePreparation_=false;
    std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::TlsIndexJneUnresolved> issuedTlsIndexJneUnresolved_;
    // CP200/CP201 diagnostic schema only: paired potential effects never execute.
    bool tlsIndexJneSchemaEnabled_=false;
    bool failTlsJnePreparation_=false;
    void advanceCalleePrefix(PendingTaskPrototype::QObjectPrefix&);
    // Internal diagnostic absolute batch stop, 1..6; unchanged targets refuse
    // reentry and raising the target resumes. This is not native behavior.
    std::size_t privateZeroBatchStopOrdinal_ = 6;
    // Absolute CP182/CP183/CP184/CP185/CP186/CP187/CP188/CP189/CP190/CP191/CP192/CP193/CP194/CP195/CP197 instruction stop, 0..18. Zero retains the CP181 entry;
    // each higher ordinal admits one more instruction; five stops before RBX restore; six before stack adjustment; seven before RDI restore; eight before RET; nine before caller XOR; ten before caller jump; eleven before the public task+8 store; twelve before the caller R12 copy; thirteen before the caller stack spill; fourteen before the reverse private+8 store; fifteen before TEST RDI; sixteen before JE; seventeen admits qualified JE; eighteen admits only source-qualified null-parent CALL.
    // This diagnostic control is never native behavior.
    std::size_t privateFlags20StopOrdinal_ = 18;
    struct Factory;
    void initializeNativePrefix();
    TaskFactoryProfile profile_ = TaskFactoryProfile::Native;
    bool adapterScopeLive_ = true;
    std::weak_ptr<TaskAllocationStore> allocations_;
    std::weak_ptr<engine::WidgetRuntimeState> widgets_;
    std::unique_ptr<Factory> factory_;
    std::optional<std::size_t> pendingReportAllocation_;
};

// Return-value notifier2cf740: suppression precedes lookup and does not reset
// flag90. Uses the actual shared notifier's qualified ordered receiver list.
TaskObserver dispatchCreateTaskBase(StudyIo& io, const qt::String& type);

// The caller retains the returned runtime lease. Explicit fixture setup only;
// production must bind its actual main-window receiver and native runtime.
std::shared_ptr<TaskFactoryRuntime> bindBoundedParsedDoeProjection(StudyIo& io,
    const std::shared_ptr<TaskAllocationStore>& allocations);

} // namespace m2m::doe
