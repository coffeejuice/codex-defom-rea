// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <map>
#include <memory>
#include <variant>
#include <type_traits>
#include <tuple>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_factory.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/view_manager.hpp"

using namespace m2m;
using qt::L;

namespace m2m::doe::detail {
struct TaskFactoryTestAccess {
    static void stopAt(TaskFactoryRuntime& factory, std::size_t ordinal)
    { factory.privateZeroBatchStopOrdinal_ = ordinal; }
    static void flagsStopAt(TaskFactoryRuntime& factory, std::size_t ordinal)
    { factory.privateFlags20StopOrdinal_ = ordinal; }
    static void replacePendingOrdinal(TaskFactoryRuntime& factory, std::size_t ordinal)
    { factory.pendingReportAllocation_ = ordinal; }
    static auto sourceState(const TaskFactoryRuntime& factory)
    { return std::make_tuple(factory.sessionReportSourceEligible_, factory.sessionReportFrameIssued_,
        factory.sessionReportCapability_, factory.sessionReportFrameBinding_, factory.sessionReportIssuedCapability_.lock(), factory.issuedNullParentCallOccurrence_, factory.issuedCalleePrefix_, factory.tlsIndexGrant_, factory.issuedTlsIndexCompare_, factory.issuedTlsIndexJneUnresolved_); }
    static void tlsCmpAt(TaskFactoryRuntime& factory,bool enabled){factory.tlsIndexCompareEnabled_=enabled;}
    static void jneSchema(TaskFactoryRuntime& factory,bool enabled=true){factory.tlsIndexJneSchemaEnabled_=enabled;}
    static void restoreJnePredecessor(TaskFactoryRuntime& factory,PendingTaskPrototype::QObjectPrefix& frame)
    {factory.issuedTlsIndexJneUnresolved_.reset();frame.tlsIndexJneUnresolved.reset();frame.phase=PendingTaskPrototype::QObjectPrefix::Phase::BeforeTlsIndexJne;}
    static void restoreJneCheckpoint(TaskFactoryRuntime& factory,PendingTaskPrototype::QObjectPrefix& frame,
        std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::TlsIndexJneUnresolved> record)
    {factory.issuedTlsIndexJneUnresolved_=record;frame.tlsIndexJneUnresolved=std::move(record);
        frame.phase=PendingTaskPrototype::QObjectPrefix::Phase::TlsIndexJneUnresolved;}
    static void revokeLiveGrant(TaskFactoryRuntime& factory,engine::WidgetRuntimeState& root)
    {root.revokeTlsGrant(factory.tlsIndexGrant_);}
    static void replaceRootThread(engine::WidgetRuntimeState& root,std::shared_ptr<engine::WidgetThreadState> thread)
    {root.thread_=std::move(thread);}
    static void replaceGrants(TaskFactoryRuntime& factory,engine::WidgetRuntimeState& root,
        std::shared_ptr<const engine::TlsIndexSessionGrant> grant)
    {factory.tlsIndexGrant_=grant;root.tlsGrant_=std::move(grant);}
    static auto cloneGrant(const engine::TlsIndexSessionGrant& grant,std::weak_ptr<engine::WidgetThreadState> thread,
        std::weak_ptr<const SessionReportNullBinding> capability)
    {return std::shared_ptr<const engine::TlsIndexSessionGrant>(new engine::TlsIndexSessionGrant(
        grant.receiver,grant.root,std::move(thread),grant.allocations,std::move(capability),grant.symbol));}
    static auto authorityState(const TaskFactoryRuntime& factory,const engine::WidgetRuntimeState& root)
    {return std::make_tuple(factory.widgets_.lock(),factory.allocations_.lock(),root.thread_);}
    static void failJnePreparation(TaskFactoryRuntime& factory,bool enabled){factory.failTlsJnePreparation_=enabled;}
    static auto jneControls(const TaskFactoryRuntime& factory){return std::make_pair(factory.tlsIndexJneSchemaEnabled_,factory.failTlsJnePreparation_);}
    static void failTlsPreparation(TaskFactoryRuntime& factory,bool enabled){factory.failTlsComparePreparation_=enabled;}
    static auto tlsRootState(const engine::WidgetRuntimeState& root)
    {return std::make_tuple(root.tlsSymbol_,root.tlsGrant_,root.tlsRead_);}
    static void restoreCmpPredecessor(TaskFactoryRuntime& factory,PendingTaskPrototype::QObjectPrefix& frame)
    {
        if(const auto root=factory.widgets_.lock())root->tlsRead_.reset();
        factory.issuedTlsIndexJneUnresolved_.reset();frame.tlsIndexJneUnresolved.reset();
        factory.issuedTlsIndexCompare_.reset();frame.tlsIndexCompare.reset();
        frame.phase=PendingTaskPrototype::QObjectPrefix::Phase::BeforeTlsIndexCompare;
    }
    static void clearTlsGrant(TaskFactoryRuntime& factory){factory.tlsIndexGrant_.reset();}
    static void clearRootRead(engine::WidgetRuntimeState& root){root.tlsRead_.reset();}
    static void clearRootGrant(engine::WidgetRuntimeState& root){root.tlsGrant_.reset();}
    static void replaceRootRead(engine::WidgetRuntimeState& root,std::shared_ptr<const engine::TlsIndexReadExpression> read)
    {root.tlsRead_=std::move(read);}
    static void replaceRootSymbol(engine::WidgetRuntimeState& root,std::shared_ptr<const engine::QtCoreTlsIndexSymbol> symbol)
    {root.tlsSymbol_=std::move(symbol);}
    static std::shared_ptr<const engine::TlsIndexReadExpression> cloneRead(
        const engine::TlsIndexReadExpression& read,engine::QtCoreTlsIndexIdentity identity,
        engine::TlsIndexReadExpression::Context context,
        std::weak_ptr<engine::WidgetRuntimeState> rootOverride={},bool overrideRoot=false)
    {
        return std::shared_ptr<const engine::TlsIndexReadExpression>(new engine::TlsIndexReadExpression(
            std::move(context),overrideRoot?std::move(rootOverride):read.root,read.thread,read.symbol,read.grant,identity));
    }
    static std::shared_ptr<const engine::TlsIndexReadExpression> cloneReadThread(
        const engine::TlsIndexReadExpression& read,std::weak_ptr<engine::WidgetThreadState> thread)
    {return std::shared_ptr<const engine::TlsIndexReadExpression>(new engine::TlsIndexReadExpression(
        read.context,read.root,std::move(thread),read.symbol,read.grant,read.identity));}
    static auto unqualifiedTlsRead(engine::WidgetRuntimeState& root)
    {return root.prepareTlsRead({}, {}, {});}
    static std::shared_ptr<const engine::QtCoreTlsIndexSymbol> cloneSymbol(
        const engine::QtCoreTlsIndexSymbol& symbol,engine::QtCoreTlsIndexIdentity identity)
    {return std::shared_ptr<const engine::QtCoreTlsIndexSymbol>(new engine::QtCoreTlsIndexSymbol(symbol.root,identity));}
    static void calleeStopAt(TaskFactoryRuntime& factory, std::size_t ordinal)
    { factory.calleePrefixStopOrdinal_ = ordinal; }
    static void restoreCalleeCheckpoint(TaskFactoryRuntime& factory, PendingTaskPrototype::QObjectPrefix& frame,
        std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::CalleePrefixState> checkpoint)
    {
        factory.issuedCalleePrefix_ = checkpoint; frame.calleePrefix = std::move(checkpoint);
        const auto k = frame.calleePrefix ? frame.calleePrefix->completed : 0;
        frame.phase = static_cast<PendingTaskPrototype::QObjectPrefix::Phase>(
            static_cast<std::size_t>(PendingTaskPrototype::QObjectPrefix::Phase::BeforeNullParentHelperEntry) + k);
        frame.rspFromEntryS = frame.calleeRspFromEntryS();
    }
    static void clearIssuedCallForRewind(TaskFactoryRuntime& factory)
    { factory.issuedNullParentCallOccurrence_.reset(); }
    static void clearCapability(TaskFactoryRuntime& factory) { factory.sessionReportCapability_.reset(); }
    static void clearIssuedBinding(TaskFactoryRuntime& factory) { factory.sessionReportFrameBinding_.reset(); }
    static void copyCapability(TaskFactoryRuntime& target, const TaskFactoryRuntime& source)
    { target.sessionReportCapability_ = source.sessionReportCapability_; }
    static void replaceCapability(TaskFactoryRuntime& target, std::shared_ptr<const SessionReportNullBinding> source)
    { target.sessionReportCapability_ = std::move(source); }
    static void copyFrameBinding(TaskFactoryRuntime& target, const TaskFactoryRuntime& source)
    { target.sessionReportFrameBinding_ = source.sessionReportFrameBinding_; }
    static void lateBind(TaskFactoryRuntime& factory) { factory.bindSessionReportNullCaller(); }
    static void replaceWidgetOwner(TaskFactoryRuntime& factory, const std::shared_ptr<engine::WidgetRuntimeState>& widgets)
    { factory.widgets_ = widgets; }
    static void replaceAllocationOwner(TaskFactoryRuntime& factory, const std::shared_ptr<TaskAllocationStore>& allocations)
    { factory.allocations_ = allocations; }
};
} // namespace m2m::doe::detail

// Historical fixtures pin CP189 budget 11 to retain their predecessor contract.
// CP190 tests below pin budget 12; CP191 tests pin budget 13; CP192 tests pin budget 14; CP193 tests pin budget 15; CP194 tests pin budget 16; CP195 exercises the production default.
namespace {
using Cp180Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
constexpr Cp180Phase cp180Phases[] = {Cp180Phase::BeforePrivateZero10,
    Cp180Phase::BeforePrivateZero24, Cp180Phase::BeforePrivateZero38,
    Cp180Phase::BeforePrivateZero64, Cp180Phase::BeforePrivateZero60,
    Cp180Phase::BeforePrivateZero28};
constexpr Cp180Phase cp182Phases[] = {Cp180Phase::BeforePrivateFlags20Read,
    Cp180Phase::BeforeFlags20And, Cp180Phase::BeforeFlags20Or,
    Cp180Phase::BeforeFlags20Write, Cp180Phase::BeforePrivateReturnSetup};
constexpr const char* cp182Stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
    "RVA 0x13122f", "RVA 0x131232"};
constexpr const char* cp180Stops[] = {"RVA 0x13120f", "RVA 0x131213", "RVA 0x131216",
    "RVA 0x13121a", "RVA 0x13121d", "RVA 0x131220"};
void cp180SetPrefix(doe::PendingTaskPrototype::QObjectPrefix& frame, std::size_t ordinal)
{
    auto& state = *frame.privateState;
    state.private10 = ordinal > 0 ? std::optional<std::uint64_t>{0} : std::nullopt;
    state.private24 = ordinal > 1 ? std::optional<std::uint32_t>{0} : std::nullopt;
    state.private38 = ordinal > 2 ? std::optional<std::uint64_t>{0} : std::nullopt;
    state.private64 = ordinal > 3 ? std::optional<std::uint32_t>{0} : std::nullopt;
    state.private60 = ordinal > 4 ? std::optional<std::uint32_t>{0} : std::nullopt;
    frame.phase = cp180Phases[ordinal]; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
}
void cp180CheckPrefix(const doe::PendingTaskPrototype::QObjectPrefix& frame, std::size_t ordinal)
{
    const auto& state = *frame.privateState;
    CHECK(frame.phase == cp180Phases[ordinal]);
    CHECK(state.private10 == (ordinal > 0 ? std::optional<std::uint64_t>{0} : std::nullopt));
    CHECK(state.private24 == (ordinal > 1 ? std::optional<std::uint32_t>{0} : std::nullopt));
    CHECK(state.private38 == (ordinal > 2 ? std::optional<std::uint64_t>{0} : std::nullopt));
    CHECK(state.private64 == (ordinal > 3 ? std::optional<std::uint32_t>{0} : std::nullopt));
    CHECK(state.private60 == (ordinal > 4 ? std::optional<std::uint32_t>{0} : std::nullopt));
}
using Cp180Pending = doe::PendingTaskPrototype;
static auto savedRbxValues(const Cp180Pending::QObjectPrefix::SavedTaskRbx& saved)
{
    return std::make_tuple(saved.reportOrdinal, saved.taskIdentity, saved.privateIdentity,
        saved.moduleSha256, saved.initializerEntryRva, saved.callRva, saved.returnRva,
        saved.rspFromEntryS, saved.saveSlotFromEntryS, saved.saveSlotFromRsp);
}
static auto sourceBindingValues(const std::shared_ptr<const doe::SessionReportNullBinding>& binding)
{
    using Values = decltype(std::make_tuple(binding->source, binding->receiver, binding->task,
        binding->privateIdentity, binding->ordinal, binding->capability,
        binding->allocations.lock(), binding->widgets.lock(),
        binding->frameOccurrence.lock(), binding->tokenOccurrence.lock(), binding->privateOccurrence.lock(), binding->initializerCallOccurrence.lock()));
    return binding ? std::optional<Values>{std::make_tuple(binding->source, binding->receiver,
        binding->task, binding->privateIdentity, binding->ordinal, binding->capability,
        binding->allocations.lock(), binding->widgets.lock(),
        binding->frameOccurrence.lock(), binding->tokenOccurrence.lock(), binding->privateOccurrence.lock(), binding->initializerCallOccurrence.lock())} : std::optional<Values>{};
}
static auto savedRdiValues(const Cp180Pending::QObjectPrefix::SavedEntryRdi& saved)
{
    return std::make_tuple(saved.reportOrdinal, saved.taskIdentity, saved.privateIdentity,
        saved.moduleSha256, saved.initializerEntryRva, saved.saveRva, saved.origin,
        saved.incomingMoveRva, saved.callRva, saved.returnRva, saved.parentIsNull,
        saved.sourceOccurrence.identity(), saved.sessionReportSource, sourceBindingValues(saved.sessionReportSource));
}
static auto returnSlotValues(const Cp180Pending::QObjectPrefix::ReturnToCaller& slot)
{
    return std::make_tuple(slot.reportOrdinal, slot.taskIdentity, slot.privateIdentity,
        slot.moduleSha256, slot.initializerEntryRva, slot.callRva, slot.targetRva, slot.slotFromEntryS,
        slot.callGeneration, slot.occurrence.identity());
}
static auto spillValues(const Cp180Pending::QObjectPrefix::CallerPrivateSpill& slot)
{
    return std::make_tuple(slot.reportOrdinal, slot.taskIdentity, slot.privateIdentity,
        slot.moduleSha256, slot.initializerEntryRva, slot.callRva, slot.returnRva,
        slot.instructionRva, slot.slotFromEntryS, slot.slotFromRsp);
}
static auto reverseValues(const Cp180Pending::QObjectPrefix::PrivateTaskPublication& value)
{
    return std::make_tuple(value.reportOrdinal, value.taskIdentity, value.privateIdentity,
        value.moduleSha256, value.instructionRva, value.destinationOffset);
}
static auto helperCallValues(const Cp180Pending::QObjectPrefix::NullParentCall& call)
{
    return std::make_tuple(call.sourceBinding, call.callerOccurrence.lock(), call.initializerOccurrence.lock(),
        call.callGeneration, call.precedingGeneration, call.callRva, call.targetRva, call.returnRva,
        call.callerRspFromEntryS, call.calleeRspFromEntryS, call.slotFromEntryS, call.occurrence.identity());
}
static auto liveSlotValues(const std::optional<Cp180Pending::QObjectPrefix::LiveReturnSlot>& slot)
{
    using Values = decltype(std::make_tuple(slot->callGeneration, slot->valueRva, slot->slotFromEntryS,
        slot->callerOccurrence.lock(), slot->callOccurrence.lock()));
    return slot ? std::optional<Values>{std::make_tuple(slot->callGeneration, slot->valueRva,
        slot->slotFromEntryS, slot->callerOccurrence.lock(), slot->callOccurrence.lock())} : std::optional<Values>{};
}
static auto calleeSlotValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::CalleeStackSlot>& slot)
{
    using Values = decltype(std::make_tuple(slot->callerOccurrence.lock(),slot->callOccurrence.lock(),
        slot->generation,slot->width,slot->instructionRva,slot->fromEntryS,slot->value,slot->task,slot->parent,
        slot->numeric,slot->occurrence.identity()));
    return slot ? std::optional<Values>{std::make_tuple(slot->callerOccurrence.lock(),slot->callOccurrence.lock(),
        slot->generation,slot->width,slot->instructionRva,slot->fromEntryS,slot->value,slot->task,slot->parent,
        slot->numeric,slot->occurrence.identity())} : std::optional<Values>{};
}
static auto subFlagsValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::StackSubFlags>& flags)
{
    using Values = decltype(std::make_tuple(flags->stackOrigin.lock(),flags->width,flags->producerRva,
        flags->operandFromEntryS,flags->resultFromEntryS,flags->immediate,flags->arithmetic,flags->occurrence.identity()));
    return flags ? std::optional<Values>{std::make_tuple(flags->stackOrigin.lock(),flags->width,flags->producerRva,
        flags->operandFromEntryS,flags->resultFromEntryS,flags->immediate,flags->arithmetic,flags->occurrence.identity())} : std::optional<Values>{};
}
static auto nestedCallValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::NestedInitializerCall>& call)
{
    using Values = decltype(std::make_tuple(call->sourceBinding,call->callerOccurrence.lock(),call->enclosingOccurrence.lock(),
        call->generation,call->precedingGeneration,call->callRva,call->targetRva,call->returnRva,call->callerRsp,call->calleeRsp,
        call->slotFromEntryS,call->occurrence.identity()));
    return call ? std::optional<Values>{std::make_tuple(call->sourceBinding,call->callerOccurrence.lock(),call->enclosingOccurrence.lock(),
        call->generation,call->precedingGeneration,call->callRva,call->targetRva,call->returnRva,call->callerRsp,call->calleeRsp,
        call->slotFromEntryS,call->occurrence.identity())} : std::optional<Values>{};
}
static auto calleePrefixValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::CalleePrefixState>& state)
{
    using Slots = std::array<decltype(calleeSlotValues({})),8>;
    Slots slots{};
    if (state) for (std::size_t i=0;i!=8;++i) slots[i]=calleeSlotValues(state->slots[i]);
    return std::make_tuple(state, state ? state->occurrence.identity() : std::shared_ptr<const unsigned char>{},
        state ? state->completed : 0, slots, nestedCallValues(state ? state->nestedCall : nullptr),
        liveSlotValues(state ? state->innerReturnSlot : std::nullopt),subFlagsValues(state ? state->firstSub : nullptr),
        subFlagsValues(state ? state->secondSub : nullptr),subFlagsValues(state ? state->currentSub : nullptr));
}
static auto tlsReadValues(const std::shared_ptr<const engine::TlsIndexReadExpression>& read)
{
    using Values=decltype(std::make_tuple(read->identity,read->domain,read->occurrence,read->root.lock(),read->thread.lock(),
        read->symbol.lock(),read->grant.lock(),read->context.sourceBinding.lock(),read->context.frame.lock(),
        read->context.outerCall.lock(),read->context.innerCall.lock(),read->context.task,read->context.privateIdentity,
        read->context.ordinal,read->context.instructionRva));
    return read?std::optional<Values>{std::make_tuple(read->identity,read->domain,read->occurrence,read->root.lock(),read->thread.lock(),
        read->symbol.lock(),read->grant.lock(),read->context.sourceBinding.lock(),read->context.frame.lock(),
        read->context.outerCall.lock(),read->context.innerCall.lock(),read->context.task,read->context.privateIdentity,
        read->context.ordinal,read->context.instructionRva)}:std::optional<Values>{};
}
static auto tlsCmpValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::TlsIndexCompare>& cmp)
{
    using Values=decltype(std::make_tuple(cmp->operand,tlsReadValues(cmp->operand),cmp->predecessor.lock(),cmp->width,cmp->producerRva,
        cmp->immediate,cmp->encodedImmediateWidth,cmp->signExtended,cmp->result,cmp->arithmetic,cmp->occurrence.identity()));
    return cmp?std::optional<Values>{std::make_tuple(cmp->operand,tlsReadValues(cmp->operand),cmp->predecessor.lock(),cmp->width,cmp->producerRva,
        cmp->immediate,cmp->encodedImmediateWidth,cmp->signExtended,cmp->result,cmp->arithmetic,cmp->occurrence.identity())}:std::optional<Values>{};
}
static auto tlsJneValues(const std::shared_ptr<const Cp180Pending::QObjectPrefix::TlsIndexJneUnresolved>& jne)
{
    using Values=decltype(std::make_tuple(jne->predecessor,jne->moduleSha256,jne->producerRva,jne->bytes,
        jne->instructionLength,jne->flagsIndex,jne->status,jne->edges,jne->occurrence.identity()));
    return jne?std::optional<Values>{std::make_tuple(jne->predecessor,jne->moduleSha256,jne->producerRva,jne->bytes,
        jne->instructionLength,jne->flagsIndex,jne->status,jne->edges,jne->occurrence.identity())}:std::optional<Values>{};
}
static auto tlsRootValues(const engine::WidgetRuntimeState& root)
{
    const auto state=doe::detail::TaskFactoryTestAccess::tlsRootState(root);
    const auto& symbol=std::get<0>(state);const auto& grant=std::get<1>(state);
    using Symbol=decltype(std::make_tuple(symbol->identity,symbol->root.lock(),symbol->occurrence));
    using Grant=decltype(std::make_tuple(grant->receiver,grant->root.lock(),grant->thread.lock(),grant->allocations.lock(),
        grant->capability.lock(),grant->symbol.lock(),grant->occurrence));
    return std::make_tuple(state,symbol?std::optional<Symbol>{std::make_tuple(symbol->identity,symbol->root.lock(),symbol->occurrence)}:std::optional<Symbol>{},
        grant?std::optional<Grant>{std::make_tuple(grant->receiver,grant->root.lock(),grant->thread.lock(),grant->allocations.lock(),
            grant->capability.lock(),grant->symbol.lock(),grant->occurrence)}:std::optional<Grant>{},tlsReadValues(std::get<2>(state)));
}
static auto frameValues(const Cp180Pending& pending)
{
    const auto& frame = *pending.qobjectPrefix;
    return std::make_tuple(pending.kind, pending.nativeAllocationBytes, pending.qobjectBaseCompleted,
        frame.phase, frame.sourceOccurrence.identity(), frame.sessionReportSource, sourceBindingValues(frame.sessionReportSource), frame.privateIdentity(), frame.allocatedPrivateIdentity,
        frame.privateState ? frame.privateState->allocationOccurrence.identity() : std::shared_ptr<const unsigned char>{},
        frame.taskAddressPointRva,
        frame.taskAddressPointWritten, frame.taskPrivateBacklinkAvailable, frame.taskPrivateIdentity,
        frame.privateTaskBacklinkAvailable, frame.complete, frame.flags20Eax, frame.returnPrivateIdentity, frame.restoredTaskRbxIdentity, frame.rspFromEntryS, frame.savedRdiSlotFromEntryS, frame.returnSlotFromEntryS,
        frame.savedTaskRbx ? std::optional{savedRbxValues(*frame.savedTaskRbx)} : std::nullopt,
        frame.savedEntryRdi ? std::optional{savedRdiValues(*frame.savedEntryRdi)} : std::nullopt,
        frame.restoredRdiToken, frame.restoredRdiParentIsNull, frame.parentTestOutcome, frame.parentBranchPath, frame.returnBoundaryRva(), frame.callerRsi, frame.callerArithmeticFlags, frame.r12PrivateIdentity,
        frame.returnToCaller ? std::optional{returnSlotValues(*frame.returnToCaller)} : std::nullopt,
        frame.callerPrivateSpill ? std::optional{spillValues(*frame.callerPrivateSpill)} : std::nullopt,
        frame.privateTaskPublication ? std::optional{reverseValues(*frame.privateTaskPublication)} : std::nullopt,
        frame.nullParentCall ? std::optional{helperCallValues(*frame.nullParentCall)} : std::nullopt,
        liveSlotValues(frame.helperReturnSlot), liveSlotValues(frame.liveReturnSlot()),
        calleePrefixValues(frame.calleePrefix), frame.currentArithmeticFlags(), frame.entrySavedRdiSlotLive(),
        frame.tlsIndexCompare,tlsCmpValues(frame.tlsIndexCompare),frame.tlsIndexJneUnresolved,tlsJneValues(frame.tlsIndexJneUnresolved));
}
static auto constructionValues(const doe::TaskFactoryRuntime& factory)
{
    const auto value = factory.construction();
    return std::make_tuple(value.factoryCommitted, value.dataCommitted, value.dataFactoryBacklinkMatches,
        value.nextPrototypeOrdinal, value.metadataCacheCount, value.taskCacheCount,
        value.pendingReportAllocation, value.nativeInitializationComplete,
        doe::detail::TaskFactoryTestAccess::sourceState(factory),
        sourceBindingValues(std::get<2>(doe::detail::TaskFactoryTestAccess::sourceState(factory))),
        sourceBindingValues(std::get<3>(doe::detail::TaskFactoryTestAccess::sourceState(factory))));
}
static auto privateValues(const Cp180Pending::SparseQObjectPrivate& state)
{
    return std::make_tuple(state.nativeAllocationBytes, state.addressPointRva, state.addressPointWritten,
        state.sharedNullReferenceCommitted, state.qstringSharedNullReferenceCommitted,
        state.secondQListReferenceCommitted, state.qvectorSharedNullReferenceCommitted,
        state.private40, state.private48, state.private50, state.private58, state.private78,
        state.private80, state.private08, state.private10, state.private24, state.private38,
        state.private64, state.private60, state.private28, state.private20);
}
struct Cp180Snapshot {
    using Pending = doe::PendingTaskPrototype;
    static bool sameWeak(const auto& a, const auto& b)
    { return a.lock() == b.lock() && !a.owner_before(b) && !b.owner_before(a); }
    const Pending* pending;
    const doe::TaskAllocationStore* store;
    const doe::TaskFactoryRuntime* factory;
    const engine::WidgetRuntimeState* widgets;
    std::optional<Pending::SparseQObjectPrivate> state;
    std::optional<Pending::QObjectPrefix::PrivateInitializerVersion> version;
    std::optional<Pending::QObjectPrefix::SavedTaskRbx> savedRbx;
    std::optional<Pending::QObjectPrefix::SavedEntryRdi> savedRdi;
    std::optional<Pending::QObjectPrefix::ReturnToCaller> returnSlot;
    std::optional<Pending::QObjectPrefix::CallerPrivateSpill> spill;
    std::optional<Pending::QObjectPrefix::PrivateTaskPublication> reverse;
    std::shared_ptr<const doe::SessionReportNullBinding> source;
    std::optional<Pending::QObjectPrefix::NullParentCall> helperCall;
    std::optional<Pending::QObjectPrefix::LiveReturnSlot> liveSlot;
    std::shared_ptr<const unsigned char> issuedCallOccurrence;
    std::shared_ptr<const Pending::QObjectPrefix::CalleePrefixState> issuedCalleePrefix;
    std::shared_ptr<const Pending::QObjectPrefix::CalleePrefixState> observedCalleePrefix;
    std::shared_ptr<const Pending::QObjectPrefix::TlsIndexJneUnresolved> issuedJne, observedJne;
    decltype(tlsRootValues(std::declval<const engine::WidgetRuntimeState&>())) tlsRoot;
    decltype(frameValues(std::declval<const Pending&>())) frame;
    decltype(constructionValues(std::declval<const doe::TaskFactoryRuntime&>())) construction;
    std::size_t tasks, prototypes, sessions;
    engine::WidgetRuntimeState::QListSharedNullState list;
    engine::WidgetRuntimeState::QStringSharedNullState string;
    engine::WidgetRuntimeState::QVectorSharedNullState vector;
    Cp180Snapshot(const Pending& p, const doe::TaskAllocationStore& s,
        const doe::TaskFactoryRuntime& f, const engine::WidgetRuntimeState& w)
        : pending(&p), store(&s), factory(&f), widgets(&w),
          state(p.qobjectPrefix->privateState ? std::optional<Pending::SparseQObjectPrivate>{*p.qobjectPrefix->privateState} : std::nullopt),
          version(p.qobjectPrefix->privateInitializerVersion), savedRbx(p.qobjectPrefix->savedTaskRbx), savedRdi(p.qobjectPrefix->savedEntryRdi), returnSlot(p.qobjectPrefix->returnToCaller), spill(p.qobjectPrefix->callerPrivateSpill), reverse(p.qobjectPrefix->privateTaskPublication), source(p.qobjectPrefix->sessionReportSource), helperCall(p.qobjectPrefix->nullParentCall),
          liveSlot(p.qobjectPrefix->liveReturnSlot()),
          issuedCallOccurrence(std::get<5>(doe::detail::TaskFactoryTestAccess::sourceState(f))),
          issuedCalleePrefix(std::get<6>(doe::detail::TaskFactoryTestAccess::sourceState(f))),
          observedCalleePrefix(p.qobjectPrefix->calleePrefix),
          issuedJne(std::get<9>(doe::detail::TaskFactoryTestAccess::sourceState(f))),observedJne(p.qobjectPrefix->tlsIndexJneUnresolved), tlsRoot(tlsRootValues(w)), frame(frameValues(p)), construction(constructionValues(f)),
          tasks(s.retainedTaskCount()), prototypes(s.retainedPrototypeCount()), sessions(s.retainedSessionCount()),
          list(*w.qlistSharedNull()), string(*w.qstringSharedNull()), vector(*w.qvectorSharedNull()) {}
    void check() const
    {
        CHECK(frameValues(*pending) == frame && constructionValues(*factory) == construction);
        CHECK(tlsRootValues(*widgets)==tlsRoot);
        const auto currentTls=doe::detail::TaskFactoryTestAccess::tlsRootState(*widgets);
        const auto& oldTls=std::get<0>(tlsRoot);
        CHECK(sameWeak(std::weak_ptr{std::get<0>(oldTls)},std::weak_ptr{std::get<0>(currentTls)}));
        CHECK(sameWeak(std::weak_ptr{std::get<1>(oldTls)},std::weak_ptr{std::get<1>(currentTls)}));
        CHECK(sameWeak(std::weak_ptr{std::get<2>(oldTls)},std::weak_ptr{std::get<2>(currentTls)}));
        CHECK(store->retainedTaskCount() == tasks && store->retainedPrototypeCount() == prototypes);
        CHECK(store->retainedSessionCount() == sessions);
        const auto& current = *pending->qobjectPrefix;
        CHECK(sameWeak(std::weak_ptr{issuedJne},std::weak_ptr{std::get<9>(doe::detail::TaskFactoryTestAccess::sourceState(*factory))}));
        CHECK(sameWeak(std::weak_ptr{observedJne},std::weak_ptr{current.tlsIndexJneUnresolved}));
        if(observedJne) {
            REQUIRE(current.tlsIndexJneUnresolved);
            CHECK(sameWeak(std::weak_ptr{observedJne->predecessor},std::weak_ptr{current.tlsIndexJneUnresolved->predecessor}));
        }
        const auto currentIssuedCall = std::get<5>(doe::detail::TaskFactoryTestAccess::sourceState(*factory));
        CHECK(sameWeak(std::weak_ptr{issuedCallOccurrence}, std::weak_ptr{currentIssuedCall}));
        const auto currentIssuedPrefix = std::get<6>(doe::detail::TaskFactoryTestAccess::sourceState(*factory));
        CHECK(sameWeak(std::weak_ptr{issuedCalleePrefix}, std::weak_ptr{currentIssuedPrefix}));
        CHECK(sameWeak(std::weak_ptr{observedCalleePrefix}, std::weak_ptr{current.calleePrefix}));
        if (observedCalleePrefix) {
            REQUIRE(current.calleePrefix);
            for (std::size_t i=0;i!=8;++i) {
                const auto& before=observedCalleePrefix->slots[i]; const auto& after=current.calleePrefix->slots[i];
                CHECK(sameWeak(std::weak_ptr{before},std::weak_ptr{after}));
                if (before && after) {
                    CHECK(sameWeak(before->callerOccurrence,after->callerOccurrence));
                    CHECK(sameWeak(before->callOccurrence,after->callOccurrence));
                }
            }
            const auto& nested=observedCalleePrefix->nestedCall;
            if (nested) {
                REQUIRE(current.calleePrefix->nestedCall);
                CHECK(sameWeak(nested->callerOccurrence,current.calleePrefix->nestedCall->callerOccurrence));
                CHECK(sameWeak(nested->enclosingOccurrence,current.calleePrefix->nestedCall->enclosingOccurrence));
            }
            const std::array beforeFlags{observedCalleePrefix->firstSub,observedCalleePrefix->secondSub,observedCalleePrefix->currentSub};
            const std::array afterFlags{current.calleePrefix->firstSub,current.calleePrefix->secondSub,current.calleePrefix->currentSub};
            for (std::size_t i=0;i!=3;++i) {
                CHECK(sameWeak(std::weak_ptr{beforeFlags[i]},std::weak_ptr{afterFlags[i]}));
                if (beforeFlags[i] && afterFlags[i]) CHECK(sameWeak(beforeFlags[i]->stackOrigin,afterFlags[i]->stackOrigin));
            }
        }
        if (helperCall) {
            REQUIRE(current.nullParentCall);
            CHECK(sameWeak(helperCall->callerOccurrence, current.nullParentCall->callerOccurrence));
            CHECK(sameWeak(helperCall->initializerOccurrence, current.nullParentCall->initializerOccurrence));
            CHECK(sameWeak(std::weak_ptr{helperCall->sourceBinding}, std::weak_ptr{current.nullParentCall->sourceBinding}));
        }
        if (liveSlot) {
            const auto live = current.liveReturnSlot();
            REQUIRE(live);
            CHECK(sameWeak(liveSlot->callerOccurrence, live->callerOccurrence));
            CHECK(sameWeak(liveSlot->callOccurrence, live->callOccurrence));
        }
        if (source) {
            REQUIRE(current.sessionReportSource);
            CHECK(sameWeak(source->allocations, current.sessionReportSource->allocations));
            CHECK(sameWeak(source->widgets, current.sessionReportSource->widgets));
            CHECK(sameWeak(source->frameOccurrence, current.sessionReportSource->frameOccurrence));
            CHECK(sameWeak(source->tokenOccurrence, current.sessionReportSource->tokenOccurrence));
            CHECK(sameWeak(source->privateOccurrence, current.sessionReportSource->privateOccurrence));
            CHECK(sameWeak(source->initializerCallOccurrence, current.sessionReportSource->initializerCallOccurrence));
        }
        CHECK(current.savedTaskRbx.has_value() == savedRbx.has_value());
        if (savedRbx) {
            REQUIRE(current.savedTaskRbx);
            CHECK(sameWeak(current.savedTaskRbx->allocationOwner, savedRbx->allocationOwner));
        }
        CHECK(current.savedEntryRdi.has_value() == savedRdi.has_value());
        if (savedRdi) {
            REQUIRE(current.savedEntryRdi);
            CHECK(sameWeak(current.savedEntryRdi->allocationOwner, savedRdi->allocationOwner));
        }
        CHECK(current.returnToCaller.has_value() == returnSlot.has_value());
        if (returnSlot) {
            REQUIRE(current.returnToCaller);
            CHECK(sameWeak(current.returnToCaller->allocationOwner, returnSlot->allocationOwner));
        }
        CHECK(current.callerPrivateSpill.has_value() == spill.has_value());
        if (spill) {
            REQUIRE(current.callerPrivateSpill);
            CHECK(sameWeak(current.callerPrivateSpill->allocationOwner, spill->allocationOwner));
        }
        CHECK(current.privateTaskPublication.has_value() == reverse.has_value());
        if (reverse) {
            REQUIRE(current.privateTaskPublication);
            CHECK(sameWeak(current.privateTaskPublication->allocationOwner, reverse->allocationOwner));
        }
        CHECK(current.privateInitializerVersion.has_value() == version.has_value());
        if (version) {
            REQUIRE(current.privateInitializerVersion);
            CHECK(current.privateInitializerVersion->moduleSha256 == version->moduleSha256);
            CHECK(current.privateInitializerVersion->argument == version->argument);
            CHECK(current.privateInitializerVersion->argumentLoadRva == version->argumentLoadRva);
            CHECK(current.privateInitializerVersion->callRva == version->callRva);
        }
        CHECK(static_cast<bool>(current.privateState) == state.has_value());
        if (state) {
            REQUIRE(current.privateState);
            CHECK(privateValues(*current.privateState) == privateValues(*state));
            CHECK(sameWeak(current.privateState->sharedNullBinding, state->sharedNullBinding));
            CHECK(sameWeak(current.privateState->secondQListSharedNullBinding, state->secondQListSharedNullBinding));
            CHECK(sameWeak(current.privateState->qstringSharedNullBinding, state->qstringSharedNullBinding));
            CHECK(sameWeak(current.privateState->qvectorSharedNullBinding, state->qvectorSharedNullBinding));
        }
        const auto currentList = widgets->qlistSharedNull();
        const auto currentVector = widgets->qvectorSharedNull();
        CHECK(currentList->referenceDelta == list.referenceDelta);
        CHECK(currentList->observedAbsoluteCount == list.observedAbsoluteCount);
        CHECK(currentList->observedAtDelta == list.observedAtDelta);
        CHECK(currentList->observationQualification == list.observationQualification);
        CHECK(widgets->qstringSharedNull()->referenceDelta == string.referenceDelta);
        CHECK(currentVector->referenceDelta == vector.referenceDelta);
        CHECK(currentVector->observedAbsoluteCount == vector.observedAbsoluteCount);
        CHECK(currentVector->observedAtDelta == vector.observedAtDelta);
        CHECK(currentVector->observationQualification == vector.observationQualification);
        CHECK(currentVector->qualifiedSymbolRva == vector.qualifiedSymbolRva);
    }
};
} // namespace

namespace {
struct IsolatedFontFixture {
    doe::SessionNotificationState notifications;
    std::shared_ptr<engine::MainViewState> main = std::make_shared<engine::MainViewState>();
    std::shared_ptr<engine::ViewManagerState> manager = main->constructManagerChild(notifications);
    std::shared_ptr<engine::WidgetRuntimeState> runtime =
        std::make_shared<engine::WidgetRuntimeState>(true, "isolated DOE font fixture");

    explicit IsolatedFontFixture(std::uint32_t mask)
    {
        main->bindWidgetRuntime(runtime);
        engine::WidgetRuntimeValueObservation values;
        values.mainView = main;
        values.manager = manager;
        values.qualification = "isolated DOE font value fixture, not native TaskInfo execution";
        values.fontData = {{1, std::nullopt, std::nullopt}};
        values.applicationFont.defaultValue = engine::WidgetFontValueObservation{1, mask};
        runtime->observeStartupValues(values);
    }
};

class IsolatedPreferenceFixture final : public doe::IsolatedTaskPreferenceSource {
public:
    std::map<qt::String, std::variant<qt::String, bool, int>> values;
    mutable std::vector<qt::String> calls;

    bool contains(const qt::String& key) const override
    {
        calls.push_back(L("contains:") + key);
        return values.find(key) != values.end();
    }
    qt::String stringValue(const qt::String& key) const override
    {
        calls.push_back(L("string:") + key);
        return std::get<qt::String>(values.at(key));
    }
    bool boolValue(const qt::String& key) const override
    {
        calls.push_back(L("bool:") + key);
        return std::get<bool>(values.at(key));
    }
    int intValue(const qt::String& key) const override
    {
        calls.push_back(L("int:") + key);
        return std::get<int>(values.at(key));
    }
};

bool refuses(const auto& action, const char* message)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(message) != std::string::npos;
    }
    return false;
}

void qualifiedReceiver(doe::StudyIo& io, const std::shared_ptr<doe::TaskFactoryRuntime>& factory)
{
    io.bindTaskFactoryRuntime(factory);
    io.sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{factory, "slotCreateTaskBase"}}, "Explicit fixed-source receiver test profile", true};
}
} // namespace

TEST_CASE(doe_taskinfo_isolated_second_font_copies_mask_when_data_is_unchanged)
{
    IsolatedFontFixture fixture(0x1234u);
    const auto supplied = fixture.runtime->applicationFont();
    REQUIRE(supplied && supplied->data);
    auto destination = doe::isolatedTaskInfoDefaultFontValue(*supplied);
    CHECK(destination.data == supplied->data && destination.resolveMask == 0);

    doe::assignIsolatedTaskInfoSecondFontValue(destination, *supplied);
    CHECK(destination.data == supplied->data && destination.resolveMask == 0x1234u);
    CHECK(!destination.data.owner_before(supplied->data) && !supplied->data.owner_before(destination.data));

    auto sameData = *supplied;
    sameData.resolveMask = 0x5678u;
    doe::assignIsolatedTaskInfoSecondFontValue(destination, sameData);
    CHECK(destination.data == supplied->data && destination.resolveMask == 0x5678u);
    CHECK(supplied->resolveMask == 0x1234u && sameData.resolveMask == 0x5678u);
    CHECK(fixture.runtime->applicationFont()->data == supplied->data);
    CHECK(fixture.runtime->applicationFont()->resolveMask == 0x1234u);

    doe::assignIsolatedTaskInfoSecondFontValue(destination, destination);
    CHECK(destination.data == supplied->data && destination.resolveMask == 0x5678u);
}

TEST_CASE(doe_taskinfo_isolated_second_font_replaces_data_owner_and_preserves_null_mask)
{
    engine::WidgetFontValue destination;
    std::weak_ptr<const engine::WidgetFontDataState> previousOwner, incomingOwner;
    {
        IsolatedFontFixture initial(0x10u);
        const auto supplied = initial.runtime->applicationFont();
        REQUIRE(supplied && supplied->data);
        destination = doe::isolatedTaskInfoDefaultFontValue(*supplied);
        previousOwner = destination.data;
    }
    CHECK(!previousOwner.expired());
    {
        IsolatedFontFixture incoming(0x80u); // Alias 1 is a new runtime's owner.
        const auto supplied = incoming.runtime->applicationFont();
        REQUIRE(supplied && supplied->data);
        CHECK(supplied->data != destination.data && supplied->data->observationAlias() == 1);
        incomingOwner = supplied->data;
        doe::assignIsolatedTaskInfoSecondFontValue(destination, *supplied);
        CHECK(previousOwner.expired());
        CHECK(destination.data == supplied->data && destination.resolveMask == 0x80u);
        CHECK(!destination.data.owner_before(supplied->data) && !supplied->data.owner_before(destination.data));
        CHECK(supplied->resolveMask == 0x80u);
    }
    CHECK(!incomingOwner.expired()); // Destination retains the newly supplied owner.
    const engine::WidgetFontValue suppliedNull{nullptr, 0x42u};
    doe::assignIsolatedTaskInfoSecondFontValue(destination, suppliedNull);
    CHECK(incomingOwner.expired());
    CHECK(!destination.data && destination.resolveMask == 0x42u);
    CHECK(!suppliedNull.data && suppliedNull.resolveMask == 0x42u);
    // Explicit value projection only; native Report/TaskInfo remains guarded.
}

TEST_CASE(doe_taskinfo_isolated_default_font_preserves_supplied_owner_and_localizes_mask)
{
    IsolatedFontFixture firstFixture(0x1234u), secondFixture(0x5678u);
    const auto first = firstFixture.runtime->applicationFont();
    const auto second = secondFixture.runtime->applicationFont();
    REQUIRE(first && second && first->data && second->data);
    CHECK(first->data->observationAlias() == second->data->observationAlias());
    CHECK(first->data != second->data); // Alias 1 belongs to each separate runtime.

    const auto projected = doe::isolatedTaskInfoDefaultFontValue(*first);
    CHECK(projected.data == first->data && projected.resolveMask == 0);
    CHECK(!projected.data.owner_before(first->data) && !first->data.owner_before(projected.data));
    CHECK(first->resolveMask == 0x1234u);
    CHECK(firstFixture.runtime->applicationFont()->data == first->data);
    CHECK(firstFixture.runtime->applicationFont()->resolveMask == 0x1234u);

    const auto other = doe::isolatedTaskInfoDefaultFontValue(*second);
    CHECK(other.data == second->data && other.data != projected.data && other.resolveMask == 0);
    CHECK(!other.data.owner_before(second->data) && !second->data.owner_before(other.data));
    CHECK(second->resolveMask == 0x5678u);
    CHECK(projected.data == first->data); // A later call did not replace prior ownership.

    engine::WidgetFontValue retained;
    std::weak_ptr<const engine::WidgetFontDataState> dataLifetime;
    {
        IsolatedFontFixture scopedFixture(0x90u);
        const auto supplied = scopedFixture.runtime->applicationFont();
        REQUIRE(supplied && supplied->data);
        dataLifetime = supplied->data;
        retained = doe::isolatedTaskInfoDefaultFontValue(*supplied);
    }
    CHECK(!dataLifetime.expired()); // Returned value retains that exact data owner.
    retained.data.reset();
    CHECK(dataLifetime.expired());
}

TEST_CASE(doe_taskinfo_isolated_default_font_distinguishes_null_data_from_unavailable_value)
{
    auto unobserved = std::make_shared<engine::WidgetRuntimeState>(true, "unobserved DOE font fixture");
    CHECK(!unobserved->applicationFont()); // No value exists to pass to the helper.

    const engine::WidgetFontValue suppliedNull{nullptr, 0x42u};
    const auto projected = doe::isolatedTaskInfoDefaultFontValue(suppliedNull);
    CHECK(!projected.data && projected.resolveMask == 0);
    CHECK(!suppliedNull.data && suppliedNull.resolveMask == 0x42u);
    CHECK(!unobserved->applicationFont());
    // Neither value test advances native Report or TaskInfo construction.
}

TEST_CASE(doe_task_preference_isolated_accessors_preserve_missing_and_assign_present_values)
{
    // This exercises accessor semantics only; no native factory/Task/QObject runs.
    IsolatedPreferenceFixture source;
    auto executable = L("existing executable");
    auto arguments = L("existing arguments");
    bool enabled = true;
    int count = 7;
    const auto executableKey = L("SfTaskReport/exeFileName");
    const auto argumentsKey = L("SfTaskReport/arguments");
    const auto enabledKey = L("SfTaskReport/exeFileEnabled");
    const auto countKey = L("SfTaskReport/inputFileNo");

    CHECK(!doe::readIsolatedTaskPreference(source, executableKey, executable));
    CHECK(!doe::readIsolatedTaskPreference(source, argumentsKey, arguments));
    CHECK(!doe::readIsolatedTaskPreference(source, enabledKey, enabled));
    CHECK(!doe::readIsolatedTaskPreference(source, countKey, count));
    CHECK(executable == L("existing executable") && arguments == L("existing arguments"));
    CHECK(enabled && count == 7);
    const std::vector<qt::String> absentCalls{
        L("contains:SfTaskReport/exeFileName"), L("contains:SfTaskReport/arguments"),
        L("contains:SfTaskReport/exeFileEnabled"), L("contains:SfTaskReport/inputFileNo")};
    CHECK(source.calls == absentCalls); // No value method ran for any absent key.

    source.values[executableKey] = L("new executable");
    source.values[argumentsKey] = L("new arguments");
    source.values[enabledKey] = false;
    source.values[countKey] = 3;
    source.calls.clear();
    CHECK(doe::readIsolatedTaskPreference(source, executableKey, executable));
    CHECK(doe::readIsolatedTaskPreference(source, argumentsKey, arguments));
    CHECK(doe::readIsolatedTaskPreference(source, enabledKey, enabled));
    CHECK(doe::readIsolatedTaskPreference(source, countKey, count));
    CHECK(executable == L("new executable") && arguments == L("new arguments"));
    CHECK(!enabled && count == 3);
    const std::vector<qt::String> presentCalls{
        L("contains:SfTaskReport/exeFileName"), L("string:SfTaskReport/exeFileName"),
        L("contains:SfTaskReport/arguments"), L("string:SfTaskReport/arguments"),
        L("contains:SfTaskReport/exeFileEnabled"), L("bool:SfTaskReport/exeFileEnabled"),
        L("contains:SfTaskReport/inputFileNo"), L("int:SfTaskReport/inputFileNo")};
    CHECK(source.calls == presentCalls);

    source.values[executableKey] = L("");
    source.values[argumentsKey] = L("");
    source.values[countKey] = 0;
    enabled = true;
    CHECK(doe::readIsolatedTaskPreference(source, executableKey, executable));
    CHECK(doe::readIsolatedTaskPreference(source, argumentsKey, arguments));
    CHECK(doe::readIsolatedTaskPreference(source, enabledKey, enabled));
    CHECK(doe::readIsolatedTaskPreference(source, countKey, count));
    CHECK(executable.isEmpty() && arguments.isEmpty() && !enabled && count == 0);
    CHECK(!executable.isNull() && !arguments.isNull());

    source.values.erase(executableKey);
    executable = qt::String();
    CHECK(!doe::readIsolatedTaskPreference(source, executableKey, executable));
    CHECK(executable.isNull()); // Absence also preserves QString's null state.
}

TEST_CASE(doe_task_factory_suppression_precedes_unavailable_registration_and_runtime)
{
    auto io = doe::makeRecordingStudyIo();
    auto& notifier = io->sessionNotificationState();
    notifier.suppressed = true;
    notifier.flag90 = true;
    CHECK(doe::dispatchCreateTaskBase(*io, L("Custom")) == nullptr);
    CHECK(!io->taskFactoryRuntimeOwner() && !notifier.createTaskBaseRegistrations);
    CHECK(notifier.flag90 && notifier.emptyKeys.empty());
}

TEST_CASE(doe_task_factory_unknown_receiver_population_is_not_empty)
{
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().flag90 = true;
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "registration population"));
    CHECK(io->sessionNotificationState().flag90 && io->sessionNotificationState().emptyKeys.empty());
}

TEST_CASE(doe_task_factory_native_prefix_commits_factory_data_and_raw_report_before_qobject_guard)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    auto io = doe::makeRecordingStudyIo();
    qualifiedReceiver(*io, factory);
    io->sessionNotificationState().flag90 = true;
    CHECK(factory->profile() == doe::TaskFactoryProfile::Native);
    CHECK(!factory->construction().factoryCommitted);
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "RVA 0x131185"));
    const auto state = factory->construction();
    CHECK(state.factoryCommitted && state.dataCommitted && state.dataFactoryBacklinkMatches);
    REQUIRE(state.pendingReportAllocation.has_value());
    CHECK(*state.pendingReportAllocation == 0 && state.nextPrototypeOrdinal == 0);
    CHECK(!state.nativeInitializationComplete && state.metadataCacheCount == 0 && state.taskCacheCount == 0);
    REQUIRE(store->retainedPrototypeCount() == 1);
    const auto* pending = store->retainedPrototypeAt(0);
    CHECK(pending->kind == doe::PendingTaskPrototype::Kind::Report);
    CHECK(pending->nativeAllocationBytes == 0xc0 && !pending->qobjectBaseCompleted);
    REQUIRE(pending->qobjectPrefix.has_value());
    const auto& frame = *pending->qobjectPrefix;
    REQUIRE(frame.privateState && frame.privateIdentity());
    CHECK(frame.privateState->nativeAllocationBytes == 0x88);
    CHECK(frame.taskAddressPointRva == 0x23cac8 && frame.privateState->addressPointRva == 0x1a9a30);
    CHECK(frame.taskAddressPointWritten && frame.privateState->addressPointWritten);
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable && !frame.complete);
    CHECK(store->retainedTaskCount() == 0);
    CHECK(io->sessionNotificationState().flag90 && io->sessionNotificationState().emptyKeys.empty());
}

TEST_CASE(doe_task_factory_binds_root_owned_qstring_and_zeros_private40_before_private48)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP164 QList root owner");
    store->associateWidgetRuntime(widgets);
    const auto rootIdentity = widgets->qlistSharedNull();
    const auto stringIdentity = widgets->qstringSharedNull();
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    const auto* privateIdentity = pending->qobjectPrefix->privateIdentity();
    const auto& frame = *pending->qobjectPrefix;
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(frame.privateState->qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
    CHECK(frame.privateState->private40 == std::optional<std::uint64_t>{0});
    CHECK(frame.privateState->private48 == std::optional<std::uint64_t>{0});
    CHECK(frame.privateState->addressPointRva == 0x1a9d10);
    CHECK(frame.privateState->sharedNullBinding.lock() == rootIdentity);
    CHECK(frame.privateState->sharedNullReferenceCommitted);
    CHECK(frame.privateState->qstringSharedNullBinding.lock() == stringIdentity);
    CHECK(frame.privateState->qstringSharedNullReferenceCommitted);
    CHECK(rootIdentity->symbolRva == 0x2f0a50);
    CHECK(rootIdentity->referenceDelta == 2 && !rootIdentity->observedAbsoluteCount);
    CHECK(stringIdentity->symbolRva == 0x2f0b30 && stringIdentity->referenceDelta == 1);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(factory->construction().metadataCacheCount == 0 && factory->construction().taskCacheCount == 0);
    CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    CHECK(store->retainedPrototypeAt(0) == pending && pending->qobjectPrefix->privateIdentity() == privateIdentity);
    CHECK(pending->qobjectPrefix->privateState->sharedNullBinding.lock() == rootIdentity);
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(pending->qobjectPrefix->privateState->private48 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->privateState->addressPointRva == 0x1a9d10);
    CHECK(rootIdentity->referenceDelta == 2);
    CHECK(stringIdentity->referenceDelta == 1 && pending->qobjectPrefix->privateState->qstringSharedNullBinding.lock() == stringIdentity);
    CHECK(refuses([&] { factory->createTaskBase(L("Custom")); }, "RVA 0x131425"));
    CHECK(store->retainedPrototypeAt(0) == pending && pending->qobjectPrefix->privateIdentity() == privateIdentity);
    CHECK(pending->qobjectPrefix->privateState->sharedNullBinding.lock() == rootIdentity);
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(pending->qobjectPrefix->privateState->addressPointRva == 0x1a9d10);
    CHECK(rootIdentity->referenceDelta == 2 && stringIdentity->referenceDelta == 1);
}

TEST_CASE(doe_task_factory_actual_session_pairs_allocation_store_with_its_widget_root)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto store = session.taskAllocationStoreOwner();
    const auto widgets = session.widgetRuntimeOwner();
    const auto factory = session.taskFactoryRuntimeOwner();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    REQUIRE(store && widgets && factory);
    CHECK(store->ownsWidgetRuntime(widgets) && factory->widgetRuntimeOwner() == widgets);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    CHECK(pending->qobjectPrefix->privateState->sharedNullBinding.lock() == widgets->qlistSharedNull());
    CHECK(widgets->qlistSharedNull()->referenceDelta == 2);
    CHECK(pending->qobjectPrefix->privateState->qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
    CHECK(widgets->qstringSharedNull()->referenceDelta == 1);
    CHECK(pending->qobjectPrefix->privateState->private40 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->privateState->private48 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->privateState->private50 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    CHECK(pending->qobjectPrefix->privateState->private40 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->privateState->private48 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->privateState->private50 == std::optional<std::uint64_t>{0});
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(widgets->qlistSharedNull()->referenceDelta == 2 && widgets->qstringSharedNull()->referenceDelta == 1);
    CHECK(widgets->qvectorSharedNull()->referenceDelta == 1);
}

TEST_CASE(doe_task_factory_cp168_zeros_private40_and_private48_once_and_stops_before_private50)
{
    doe::PendingTaskPrototype::SparseQObjectPrivate freshPrivate;
    CHECK(!freshPrivate.private40.has_value());
    CHECK(!freshPrivate.private48.has_value());
    CHECK(!freshPrivate.private50.has_value());
    CHECK(!freshPrivate.private58.has_value());
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP167 root");
    store->associateWidgetRuntime(widgets);
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    state.private50.reset();
    state.private58.reset();
    state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
    frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
    const auto* privateIdentity = frame.privateIdentity();
    const auto list = widgets->qlistSharedNull();
    const auto string = widgets->qstringSharedNull();
    REQUIRE(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50);
    REQUIRE(state.private40 == std::optional<std::uint64_t>{0});
    REQUIRE(state.private48 == std::optional<std::uint64_t>{0});
    CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
    CHECK(state.private40 == std::optional<std::uint64_t>{0});
    CHECK(state.private48 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50);
    CHECK(state.sharedNullBinding.lock() == list && state.qstringSharedNullBinding.lock() == string);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(factory->construction().metadataCacheCount == 0 && factory->construction().taskCacheCount == 0);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    for (const auto& key : {qt::L("SfTaskReport"), qt::L("SfTaskDoe")}) {
        CHECK(refuses([&] { factory->createTaskBase(key); }, "RVA 0x131425"));
        CHECK(state.private40 == std::optional<std::uint64_t>{0});
        CHECK(state.private48 == std::optional<std::uint64_t>{0});
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
        CHECK(list->referenceDelta == 3 && string->referenceDelta == 1);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    }
}

TEST_CASE(doe_task_factory_cp168_refuses_prewrite_corruption_without_mutation)
{
    for (int damage = 0; damage != 18; ++damage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP168 guard root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP168 foreign root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        state.private50.reset();
        state.private58.reset();
        state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        const auto* identity = frame.privateIdentity();
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.private48.reset();
        state.private58.reset();
        switch (damage) {
        case 0: state.private40.reset(); break;
        case 1: state.private40 = std::uint64_t{0x40}; break;
        case 2: state.private48 = std::uint64_t{0x48}; break;
        case 3: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
                    widgets, foreign->qlistSharedNull().get()); break;
        case 4: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case 5: state.sharedNullReferenceCommitted = false; break;
        case 6: state.addressPointRva = 0x1a9a30; break;
        case 7: frame.complete = true; break;
        case 8: frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::QObjectAddressPoint; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case 9: widgets.reset(); break;
        case 10: state.qstringSharedNullBinding =
                std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(
                    widgets, foreign->qstringSharedNull().get()); break;
        case 11: state.qstringSharedNullReferenceCommitted = false; break;
        case 12: frame.taskAddressPointRva = 0x23cac0; break;
        case 13: frame.taskAddressPointWritten = false; break;
        case 14: frame.taskPrivateBacklinkAvailable = true; break;
        case 15: frame.privateTaskBacklinkAvailable = true; break;
        case 16:
            frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
            frame.privateState->private08.reset();
            frame.privateState->private10.reset(); frame.privateState->private24.reset();
            frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
            frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
            state.private50 = std::uint64_t{0};
            break;
        case 17:
            frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
            frame.privateState->private08.reset();
            frame.privateState->private10.reset(); frame.privateState->private24.reset();
            frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
            frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
            state.private40 = std::uint64_t{0};
            state.private50 = std::uint64_t{0};
            break;
        }
        const auto field40 = state.private40;
        const auto field48 = state.private48;
        const auto field50 = state.private50;
        const auto phaseBefore = frame.phase;
        const auto listBindingBefore = state.sharedNullBinding.lock();
        const auto stringBindingBefore = state.qstringSharedNullBinding.lock();
        const auto listCommitBefore = state.sharedNullReferenceCommitted;
        const auto stringCommitBefore = state.qstringSharedNullReferenceCommitted;
        const auto taskAddressPointBefore = frame.taskAddressPointRva;
        const auto taskAddressPointWrittenBefore = frame.taskAddressPointWritten;
        const auto taskBacklinkBefore = frame.taskPrivateBacklinkAvailable;
        const auto privateBacklinkBefore = frame.privateTaskBacklinkAvailable;
        const auto completeBefore = frame.complete;
        const auto baseCompleteBefore = pending->qobjectBaseCompleted;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == 14 ? "CP190 requires phase-qualified task+8 publication" : damage == 8 ? "private zero fields" : damage == 16 ? "CP166 roots" : "CP167 roots"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
        CHECK(frame.phase == phaseBefore);
        CHECK(state.private40 == field40 && state.private48 == field48 && state.private50 == field50);
        CHECK(state.sharedNullBinding.lock() == listBindingBefore);
        CHECK(state.qstringSharedNullBinding.lock() == stringBindingBefore);
        CHECK(state.sharedNullReferenceCommitted == listCommitBefore);
        CHECK(state.qstringSharedNullReferenceCommitted == stringCommitBefore);
        CHECK(frame.taskAddressPointRva == taskAddressPointBefore);
        CHECK(frame.taskAddressPointWritten == taskAddressPointWrittenBefore);
        CHECK(frame.taskPrivateBacklinkAvailable == taskBacklinkBefore);
        CHECK(frame.privateTaskBacklinkAvailable == privateBacklinkBefore);
        CHECK(frame.complete == completeBefore && pending->qobjectBaseCompleted == baseCompleteBefore);
        CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    }
}

TEST_CASE(doe_task_factory_cp169_zeros_private50_once_and_refuses_premature_or_reentry)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP169 root");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto* identity = frame.privateIdentity();
    const auto list = widgets->qlistSharedNull();
    const auto string = widgets->qstringSharedNull();
    CHECK(state.private40 == std::optional<std::uint64_t>{0});
    CHECK(state.private48 == std::optional<std::uint64_t>{0});
    CHECK(state.private50 == std::optional<std::uint64_t>{0});
    CHECK(state.private58 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
    CHECK(state.private50 == std::optional<std::uint64_t>{0});
    CHECK(state.private58 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);

    for (int damage = 0; damage != 3; ++damage) {
        state.private40 = std::uint64_t{0};
        state.private48 = std::uint64_t{0};
        state.private50.reset();
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        if (damage == 0) state.private40.reset();
        if (damage == 1) state.private48 = std::uint64_t{0x48};
        if (damage == 2) state.private50 = std::uint64_t{0x50};
        state.private58.reset();
        REQUIRE(!state.private58.has_value());
        const auto private40 = state.private40;
        const auto private48 = state.private48;
        const auto private50 = state.private50;
        const auto private58 = state.private58;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "CP168 state"));
        CHECK(state.private40 == private40 && state.private48 == private48 && state.private50 == private50 &&
            state.private58 == private58);
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50);
        CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp169_refuses_corrupt_prewrite_state_without_mutation)
{
    enum class Damage {
        Nonzero40, Missing48, PrematureZero50, ExpiredRoot, ForeignListAlias,
        ForeignQString, UncommittedList, UncommittedQString, WrongTaskAddress,
        WrongPrivateAddress, ReplacedPrivateIdentity, WrongPhase, Complete,
        TaskBacklink, PrivateBacklink
    };
    constexpr Damage cases[] = {
        Damage::Nonzero40, Damage::Missing48, Damage::PrematureZero50, Damage::ExpiredRoot,
        Damage::ForeignListAlias, Damage::ForeignQString, Damage::UncommittedList,
        Damage::UncommittedQString, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::ReplacedPrivateIdentity, Damage::WrongPhase, Damage::Complete,
        Damage::TaskBacklink, Damage::PrivateBacklink};
    for (const auto damage : cases) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP169 owner");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP169 foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto ownerList = widgets->qlistSharedNull();
        const auto ownerString = widgets->qstringSharedNull();
        const auto foreignList = foreign->qlistSharedNull();
        const auto foreignString = foreign->qstringSharedNull();
        // The first call has already performed CP169. Rebuild the accepted
        // CP168 cut before introducing each independent prewrite fault.
        REQUIRE(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
        REQUIRE(state.private50 == std::optional<std::uint64_t>{0});
        REQUIRE(state.private58 == std::optional<std::uint64_t>{0});
        REQUIRE(frame.privateIdentity() == frame.allocatedPrivateIdentity);
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.private50.reset();
        state.private58.reset();
    state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        REQUIRE(state.private40 == std::optional<std::uint64_t>{0});
        REQUIRE(state.private48 == std::optional<std::uint64_t>{0});
        REQUIRE(!state.private50.has_value());
        std::unique_ptr<doe::PendingTaskPrototype::SparseQObjectPrivate> retiredPrivate;
        switch (damage) {
        case Damage::Nonzero40: state.private40 = std::uint64_t{0x40}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::PrematureZero50: state.private50 = std::uint64_t{0}; break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::ForeignListAlias:
            state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
                widgets, foreignList.get());
            break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreignString; break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::ReplacedPrivateIdentity:
            // Copy every qualified field before replacing the pointee. Only
            // the allocation-time identity is now invalid.
            {
                auto clone = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state);
                retiredPrivate = std::move(frame.privateState);
                frame.privateState = std::move(clone);
            }
            break;
        case Damage::WrongPhase:
            frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
            frame.privateState->private08.reset();
            frame.privateState->private10.reset(); frame.privateState->private24.reset();
            frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
            frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
            break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        }
        const auto* privateIdentity = frame.privateIdentity();
        auto& damagedState = *frame.privateState;
        const auto* allocatedIdentity = frame.allocatedPrivateIdentity;
        if (damage == Damage::ReplacedPrivateIdentity) {
            CHECK(privateIdentity != allocatedIdentity);
            CHECK(damagedState.nativeAllocationBytes == 0x88 && damagedState.addressPointWritten);
            CHECK(damagedState.addressPointRva == 0x1a9d10);
            CHECK(damagedState.sharedNullBinding.lock() == ownerList);
            CHECK(damagedState.qstringSharedNullBinding.lock() == ownerString);
            CHECK(damagedState.sharedNullReferenceCommitted && damagedState.qstringSharedNullReferenceCommitted);
            CHECK(damagedState.private40 == std::optional<std::uint64_t>{0});
            CHECK(damagedState.private48 == std::optional<std::uint64_t>{0});
            CHECK(!damagedState.private50.has_value());
        }
        const auto phase = frame.phase;
        const auto private40 = damagedState.private40;
        const auto private48 = damagedState.private48;
        const auto private50 = damagedState.private50;
        const auto private58 = damagedState.private58;
        const auto listBinding = damagedState.sharedNullBinding.lock();
        const auto stringBinding = damagedState.qstringSharedNullBinding.lock();
        const auto taskAddress = frame.taskAddressPointRva;
        const auto privateAddress = damagedState.addressPointRva;
        const auto listCommitted = damagedState.sharedNullReferenceCommitted;
        const auto stringCommitted = damagedState.qstringSharedNullReferenceCommitted;
        const auto complete = frame.complete;
        const auto taskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto privateBacklink = frame.privateTaskBacklinkAvailable;
        const auto baseComplete = pending->qobjectBaseCompleted;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto listDelta = ownerList->referenceDelta;
        const auto stringDelta = ownerString->referenceDelta;
        const auto foreignListDelta = foreignList->referenceDelta;
        const auto foreignStringDelta = foreignString->referenceDelta;

        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "private+0x48 zero" : "CP168 state"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
        CHECK(frame.allocatedPrivateIdentity == allocatedIdentity);
        CHECK(frame.phase == phase && damagedState.private40 == private40 &&
            damagedState.private48 == private48 && damagedState.private50 == private50 &&
            damagedState.private58 == private58);
        CHECK(damagedState.sharedNullBinding.lock() == listBinding);
        CHECK(damagedState.qstringSharedNullBinding.lock() == stringBinding);
        CHECK(damagedState.sharedNullReferenceCommitted == listCommitted);
        CHECK(damagedState.qstringSharedNullReferenceCommitted == stringCommitted);
        CHECK(frame.taskAddressPointRva == taskAddress && damagedState.addressPointRva == privateAddress);
        CHECK(frame.complete == complete && frame.taskPrivateBacklinkAvailable == taskBacklink);
        CHECK(frame.privateTaskBacklinkAvailable == privateBacklink && pending->qobjectBaseCompleted == baseComplete);
        CHECK(ownerList->referenceDelta == listDelta && ownerString->referenceDelta == stringDelta);
        CHECK(foreignList->referenceDelta == foreignListDelta && foreignString->referenceDelta == foreignStringDelta);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    }
}


TEST_CASE(doe_task_factory_cp170_zeros_private58_once_and_refuses_corrupt_prewrite_state)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP170 root");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto privateIdentity = frame.privateIdentity();
    const auto list = widgets->qlistSharedNull();
    const auto string = widgets->qstringSharedNull();
    const auto listBinding = state.sharedNullBinding.lock();
    const auto stringBinding = state.qstringSharedNullBinding.lock();
    REQUIRE(state.private40 == std::optional<std::uint64_t>{0});
    REQUIRE(state.private48 == std::optional<std::uint64_t>{0});
    REQUIRE(state.private50 == std::optional<std::uint64_t>{0});
    CHECK(state.private58 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    CHECK(frame.privateIdentity() == privateIdentity && state.private58 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
    CHECK(state.sharedNullBinding.lock() == listBinding && state.qstringSharedNullBinding.lock() == stringBinding);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);

    // Each malformed CP170 case gets a fresh, retained CP169 frame.
    auto ownedWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP170 matrix root");
    auto matrixStore = std::make_shared<doe::TaskAllocationStore>();
    matrixStore->associateWidgetRuntime(ownedWidgets);
    doe::TaskFactoryRuntime matrixFactory(matrixStore, ownedWidgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(matrixFactory, 11);
    CHECK(refuses([&] { matrixFactory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* matrixPending = matrixStore->retainedPrototypeAt(0);
    REQUIRE(matrixPending && matrixPending->qobjectPrefix && matrixPending->qobjectPrefix->privateState);
    auto& matrixFrame = *matrixPending->qobjectPrefix;
    auto& matrixOriginalState = *matrixFrame.privateState;
    const auto matrixList = ownedWidgets->qlistSharedNull();
    const auto matrixString = ownedWidgets->qstringSharedNull();
    enum class Damage { Missing40, Bad40, Missing48, Bad48, Missing50, Bad50, Premature58,
        ForeignList, AliasForeignList, ForeignString, UncommittedList, UncommittedString,
        WrongTaskAddress, WrongPrivateAddress, ReplacedPrivate, WrongSize, WrongTaskSize, BaseComplete,
        WrongPhase, Complete, TaskBacklink, PrivateBacklink };
    constexpr Damage cases[] = {Damage::Missing40, Damage::Bad40, Damage::Missing48, Damage::Bad48,
        Damage::Missing50, Damage::Bad50, Damage::Premature58, Damage::ForeignList,
        Damage::AliasForeignList, Damage::ForeignString, Damage::UncommittedList, Damage::UncommittedString, Damage::WrongTaskAddress,
        Damage::WrongPrivateAddress, Damage::ReplacedPrivate, Damage::WrongSize, Damage::WrongTaskSize, Damage::BaseComplete,
        Damage::WrongPhase, Damage::Complete, Damage::TaskBacklink, Damage::PrivateBacklink};
    std::unique_ptr<doe::PendingTaskPrototype::SparseQObjectPrivate> retiredAcrossCases;
    for (const auto damage : cases) {
        if (retiredAcrossCases) matrixFrame.privateState = std::move(retiredAcrossCases);
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP170 foreign");
        matrixFrame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58;
        // Rewind the CP182/CP183 observations with this diagnostic cursor.
        matrixFrame.returnPrivateIdentity = nullptr; matrixFrame.restoredTaskRbxIdentity = nullptr; matrixFrame.restoredRdiToken = nullptr; matrixFrame.rspFromEntryS = -0x38; matrixFrame.callerRsi.reset(); matrixFrame.callerArithmeticFlags.reset();
        matrixFrame.flags20Eax.reset(); matrixFrame.privateState->private20.reset();
        matrixFrame.privateState->private28.reset();
        matrixFrame.privateState->private08.reset();
        matrixFrame.privateState->private10.reset(); matrixFrame.privateState->private24.reset();
        matrixFrame.privateState->private38.reset(); matrixFrame.privateState->private64.reset(); matrixFrame.privateState->private60.reset();
        matrixFrame.privateState->private40 = std::uint64_t{0}; matrixFrame.privateState->private48 = std::uint64_t{0}; matrixFrame.privateState->private50 = std::uint64_t{0};
        matrixFrame.privateState->private58.reset();
        matrixFrame.privateState->qvectorSharedNullBinding.reset(); matrixFrame.privateState->qvectorSharedNullReferenceCommitted = false;
        matrixFrame.privateState->secondQListSharedNullBinding.reset(); matrixFrame.privateState->secondQListReferenceCommitted = false; matrixFrame.privateState->private78.reset(); matrixFrame.privateState->private80.reset();
        matrixFrame.complete = false; matrixFrame.taskPrivateBacklinkAvailable = false;
        matrixFrame.privateTaskBacklinkAvailable = false; matrixPending->qobjectBaseCompleted = false;
        matrixFrame.taskAddressPointRva = 0x23cac8; matrixFrame.privateState->addressPointRva = 0x1a9d10;
        matrixFrame.privateState->nativeAllocationBytes = 0x88; matrixPending->nativeAllocationBytes = 0xc0;
        matrixFrame.privateState->sharedNullBinding = matrixList; matrixFrame.privateState->qstringSharedNullBinding = matrixString;
        matrixFrame.privateState->sharedNullReferenceCommitted = true; matrixFrame.privateState->qstringSharedNullReferenceCommitted = true;
        if (damage == Damage::Missing40) matrixFrame.privateState->private40.reset();
        if (damage == Damage::Bad40) matrixFrame.privateState->private40 = std::uint64_t{0x40};
        if (damage == Damage::Missing48) matrixFrame.privateState->private48.reset();
        if (damage == Damage::Bad48) matrixFrame.privateState->private48 = std::uint64_t{0x48};
        if (damage == Damage::Missing50) matrixFrame.privateState->private50.reset();
        if (damage == Damage::Bad50) matrixFrame.privateState->private50 = std::uint64_t{0x50};
        if (damage == Damage::Premature58) matrixFrame.privateState->private58 = std::uint64_t{0};
        if (damage == Damage::ForeignList) matrixFrame.privateState->sharedNullBinding = foreign->qlistSharedNull();
        if (damage == Damage::AliasForeignList) matrixFrame.privateState->sharedNullBinding =
            std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(ownedWidgets, foreign->qlistSharedNull().get());
        if (damage == Damage::ForeignString) matrixFrame.privateState->qstringSharedNullBinding = foreign->qstringSharedNull();
        if (damage == Damage::UncommittedList) matrixFrame.privateState->sharedNullReferenceCommitted = false;
        if (damage == Damage::UncommittedString) matrixFrame.privateState->qstringSharedNullReferenceCommitted = false;
        if (damage == Damage::WrongTaskAddress) matrixFrame.taskAddressPointRva = 0x23cac0;
        if (damage == Damage::WrongPrivateAddress) matrixFrame.privateState->addressPointRva = 0x1a9a30;

        if (damage == Damage::ReplacedPrivate) { auto copy = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(matrixOriginalState); retiredAcrossCases = std::move(matrixFrame.privateState); matrixFrame.privateState = std::move(copy); }
        if (damage == Damage::WrongSize) matrixFrame.privateState->nativeAllocationBytes = 0x80;
        if (damage == Damage::WrongTaskSize) matrixPending->nativeAllocationBytes = 0xb8;
        if (damage == Damage::BaseComplete) matrixPending->qobjectBaseCompleted = true;
        if (damage == Damage::WrongPhase) matrixFrame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50;
        if (damage == Damage::Complete) matrixFrame.complete = true;
        if (damage == Damage::TaskBacklink) matrixFrame.taskPrivateBacklinkAvailable = true;
        if (damage == Damage::PrivateBacklink) matrixFrame.privateTaskBacklinkAvailable = true;
        auto& observed = *matrixFrame.privateState;
        const auto* currentIdentity = matrixFrame.privateIdentity();
        const auto* allocationIdentity = matrixFrame.allocatedPrivateIdentity;
        const auto phase = matrixFrame.phase; const auto field40 = observed.private40; const auto field48 = observed.private48;
        const auto field50 = observed.private50; const auto field58 = observed.private58;
        const auto listBound = observed.sharedNullBinding.lock(); const auto stringBound = observed.qstringSharedNullBinding.lock();
        const auto listCommitted = observed.sharedNullReferenceCommitted; const auto stringCommitted = observed.qstringSharedNullReferenceCommitted;
        const auto taskAddress = matrixFrame.taskAddressPointRva; const auto privateAddress = observed.addressPointRva;
        const auto taskSize = matrixPending->nativeAllocationBytes; const auto privateSize = observed.nativeAllocationBytes;
        const auto complete = matrixFrame.complete; const auto taskBack = matrixFrame.taskPrivateBacklinkAvailable;
        const auto privateBack = matrixFrame.privateTaskBacklinkAvailable; const auto baseComplete = matrixPending->qobjectBaseCompleted;
        const auto listDelta = matrixList->referenceDelta; const auto stringDelta = matrixString->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto prototypeCount = matrixStore->retainedPrototypeCount(); const auto taskCount = matrixStore->retainedTaskCount();
        const auto allocationOwnerMatches = ownedWidgets && matrixStore->ownsWidgetRuntime(ownedWidgets);
        const auto rootListIdentity = ownedWidgets ? ownedWidgets->qlistSharedNull() : std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>{};
        const auto rootStringIdentity = ownedWidgets ? ownedWidgets->qstringSharedNull() : std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>{};
        const auto constructionBefore = matrixFactory.construction();
        CHECK(refuses([&] { matrixFactory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP168 state" : "CP169 state"));
        CHECK(matrixStore->retainedPrototypeAt(0) == matrixPending && matrixFrame.privateIdentity() == currentIdentity);
        CHECK(matrixFrame.allocatedPrivateIdentity == allocationIdentity);
        CHECK(matrixFrame.phase == phase && observed.private40 == field40 && observed.private48 == field48);
        CHECK(observed.private50 == field50 && observed.private58 == field58);
        CHECK(matrixFrame.complete == complete && matrixFrame.taskPrivateBacklinkAvailable == taskBack);
        CHECK(matrixFrame.privateTaskBacklinkAvailable == privateBack && matrixPending->qobjectBaseCompleted == baseComplete);
        CHECK(observed.sharedNullBinding.lock() == listBound && observed.qstringSharedNullBinding.lock() == stringBound);
        CHECK(observed.sharedNullReferenceCommitted == listCommitted && observed.qstringSharedNullReferenceCommitted == stringCommitted);
        CHECK(matrixFrame.taskAddressPointRva == taskAddress && observed.addressPointRva == privateAddress);
        CHECK(matrixPending->nativeAllocationBytes == taskSize && observed.nativeAllocationBytes == privateSize);
        CHECK(matrixList->referenceDelta == listDelta && matrixString->referenceDelta == stringDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta && foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(matrixStore->retainedPrototypeCount() == prototypeCount && matrixStore->retainedTaskCount() == taskCount);
        CHECK((ownedWidgets && ownedWidgets->qlistSharedNull() == rootListIdentity) || (!ownedWidgets && !rootListIdentity));
        CHECK((ownedWidgets && ownedWidgets->qstringSharedNull() == rootStringIdentity) || (!ownedWidgets && !rootStringIdentity));
        CHECK((ownedWidgets && matrixStore->ownsWidgetRuntime(ownedWidgets)) == allocationOwnerMatches);
        const auto constructionAfter = matrixFactory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount &&
            constructionAfter.taskCacheCount == constructionBefore.taskCacheCount &&
            constructionAfter.pendingReportAllocation == constructionBefore.pendingReportAllocation);
    }

    // Expiring the root in this same factory exercises the retained CP170 guard.
    frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
    state.private58.reset();
    widgets.reset();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "CP169 state"));
    CHECK(store->retainedPrototypeAt(0) == pending && frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58);
    CHECK(frame.privateIdentity() == privateIdentity && !state.private58.has_value());
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
}

TEST_CASE(doe_task_factory_cp167_refuses_premature_private48_before_private40_write)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP168 malformed predecessor root");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto list = widgets->qlistSharedNull();
    const auto string = widgets->qstringSharedNull();
    frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
    state.private40.reset();
    state.private48 = std::uint64_t{0x48};
    const auto phaseBefore = frame.phase;
    const auto listBindingBefore = state.sharedNullBinding.lock();
    const auto stringBindingBefore = state.qstringSharedNullBinding.lock();
    const auto prototypeCountBefore = store->retainedPrototypeCount();
    const auto taskCountBefore = store->retainedTaskCount();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "CP166 roots"));
    CHECK(state.private40 == std::nullopt && state.private48 == std::optional<std::uint64_t>{0x48});
    CHECK(frame.phase == phaseBefore);
    CHECK(state.sharedNullBinding.lock() == listBindingBefore && state.qstringSharedNullBinding.lock() == stringBindingBefore);
    CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted);
    CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
    CHECK(store->retainedPrototypeCount() == prototypeCountBefore && store->retainedTaskCount() == taskCountBefore);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
}

TEST_CASE(doe_task_factory_cp168_refuses_unassociated_or_foreign_factory_root_without_mutation)
{
    for (const bool foreignFactoryRoot : {false, true}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto expected = std::make_shared<engine::WidgetRuntimeState>(true, "CP168 associated root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP168 foreign factory root");
        store->associateWidgetRuntime(expected);
        std::shared_ptr<engine::WidgetRuntimeState> factoryWidgets;
        if (foreignFactoryRoot) factoryWidgets = foreign;
        doe::TaskFactoryRuntime factory(store, factoryWidgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131185"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto* identity = frame.privateIdentity();
        // Reconstitute a qualified CP167 predecessor while keeping the
        // factory's missing or foreign root as the admission fault.
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = expected->qlistSharedNull();
        state.qstringSharedNullBinding = expected->qstringSharedNull();
        expected->commitQListSharedNullReference();
        expected->commitQStringSharedNullReference();
        state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullReferenceCommitted = true;
        state.private40 = std::uint64_t{0};
        state.private48.reset();
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        const auto phaseBefore = frame.phase;
        const auto listBindingBefore = state.sharedNullBinding.lock();
        const auto stringBindingBefore = state.qstringSharedNullBinding.lock();
        const auto listDeltaBefore = expected->qlistSharedNull()->referenceDelta;
        const auto stringDeltaBefore = expected->qstringSharedNull()->referenceDelta;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "CP167 roots"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
        CHECK(frame.phase == phaseBefore);
        CHECK(state.private40 == std::optional<std::uint64_t>{0} && !state.private48.has_value());
        CHECK(state.sharedNullBinding.lock() == listBindingBefore);
        CHECK(state.qstringSharedNullBinding.lock() == stringBindingBefore);
        CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted);
        CHECK(expected->qlistSharedNull()->referenceDelta == listDeltaBefore);
        CHECK(expected->qstringSharedNull()->referenceDelta == stringDeltaBefore);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qstringSharedNull()->referenceDelta == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    }
}

TEST_CASE(doe_task_factory_cp167_refuses_aliasing_foreign_root_without_writing_private40)
{
    for (int damage = 0; damage != 8; ++damage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto expected = std::make_shared<engine::WidgetRuntimeState>(true, "CP167 expected root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP167 foreign root");
        store->associateWidgetRuntime(expected);
        auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, expected);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        // The first call really executes CP166 and CP167, then stops at +0x48.
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto* privateIdentity = frame.privateIdentity();
        const auto list = expected->qlistSharedNull();
        const auto string = expected->qstringSharedNull();
        // Reconstitute the CP166 predecessor to exercise CP167's pre-write guard.
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.private40.reset();
        state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        if (damage == 0) {
            state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
                expected, foreign->qlistSharedNull().get());
        } else if (damage == 1) {
            state.qstringSharedNullBinding.reset();
        } else if (damage == 2) {
            state.qstringSharedNullReferenceCommitted = false;
        } else if (damage == 3) {
            // The store and factory retain only weak root observations.
            expected.reset();
        } else if (damage == 4) {
            state.qstringSharedNullBinding = foreign->qstringSharedNull();
        } else if (damage == 5) {
            state.sharedNullReferenceCommitted = false;
        } else if (damage == 6) {
            state.addressPointRva = 0x1a9a30;
        } else {
            state.private40 = std::uint64_t{0x40};
        }
        const auto fieldBefore = state.private40;
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "CP166 roots"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields);
        CHECK(state.private40 == fieldBefore);
        CHECK(state.sharedNullBinding.lock() == (damage == 0 ? foreign->qlistSharedNull() : list));
        if (damage == 1) CHECK(state.qstringSharedNullBinding.expired());
        else if (damage == 4) CHECK(state.qstringSharedNullBinding.lock() == foreign->qstringSharedNull());
        else CHECK(state.qstringSharedNullBinding.lock() == string);
        CHECK(list->referenceDelta == 2 && string->referenceDelta == 1);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qstringSharedNull()->referenceDelta == 0);
        CHECK(!pending->qobjectBaseCompleted && !frame.complete);
        CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
        CHECK(factory->construction().metadataCacheCount == 0 && factory->construction().taskCacheCount == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    }
}

TEST_CASE(doe_task_factory_final_private_address_point_refuses_uncommitted_or_foreign_root)
{
    for (const bool foreignBinding : {false, true}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 expected root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 foreign root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        const auto* privateIdentity = frame.privateIdentity();
        const auto root = widgets->qlistSharedNull();
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateFinalAddressPoint; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        frame.privateState->private40.reset();
        frame.privateState->qvectorSharedNullBinding.reset(); frame.privateState->qvectorSharedNullReferenceCommitted = false;
        frame.privateState->addressPointRva = 0x1a9a30;
        if (foreignBinding) frame.privateState->sharedNullBinding = foreign->qlistSharedNull();
        else frame.privateState->sharedNullReferenceCommitted = false;
        const auto previousPhase = frame.phase;
        const auto previousAddressPoint = frame.privateState->addressPointRva;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "committed matching QList root"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
        CHECK(frame.phase == previousPhase && frame.privateState->addressPointRva == previousAddressPoint);
        CHECK(frame.privateState->addressPointRva == 0x1a9a30);
        CHECK(root->referenceDelta == 2 && foreign->qlistSharedNull()->referenceDelta == 0);
        CHECK(widgets->qstringSharedNull()->referenceDelta == 1 &&
            frame.privateState->qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(!pending->qobjectBaseCompleted && !frame.complete);
        CHECK(factory.construction().taskCacheCount == 0 && store->retainedTaskCount() == 0);
    }
}

TEST_CASE(doe_task_factory_final_private_address_point_guard_prevents_write_on_bad_cp164_frame)
{
    for (const bool foreignBinding : {false, true}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 expected root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 foreign root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        const auto* privateIdentity = frame.privateIdentity();
        auto root = widgets->qlistSharedNull();
        // Reconstitute the exact pre-write seam to inject a corrupted CP164
        // binding/marker without adding a production-only test hook.
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateFinalAddressPoint; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        frame.privateState->private40.reset();
        frame.privateState->qvectorSharedNullBinding.reset(); frame.privateState->qvectorSharedNullReferenceCommitted = false;
        frame.privateState->addressPointRva = 0x1a9a30;
        if (foreignBinding) frame.privateState->sharedNullBinding = foreign->qlistSharedNull();
        else frame.privateState->sharedNullReferenceCommitted = false;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "committed matching QList root"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateFinalAddressPoint);
        CHECK(frame.privateState->addressPointRva == 0x1a9a30);
        CHECK(root->referenceDelta == 2 && foreign->qlistSharedNull()->referenceDelta == 0);
        CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    }
}

TEST_CASE(doe_task_factory_qstring_guard_refuses_tampered_cp165_without_new_delta)
{
    for (int damage = 0; damage != 5; ++damage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP166 expected root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP166 foreign root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto* privateIdentity = frame.privateIdentity();
        const auto stringIdentity = widgets->qstringSharedNull();
        const auto listIdentity = widgets->qlistSharedNull();
        // Reconstitute the CP165 cut to exercise the production admission
        // guard. The already observed symbolic delta remains as a baseline.
        frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.private40.reset();
        state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        state.qstringSharedNullBinding.reset();
        state.qstringSharedNullReferenceCommitted = false;
        switch (damage) {
        case 0: state.sharedNullReferenceCommitted = false; break;
        case 1: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case 2: state.addressPointRva = 0x1a9a30; break;
        case 3: frame.taskPrivateBacklinkAvailable = true; break;
        case 4: frame.complete = true; break;
        }
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, damage == 3 ? "CP190 requires phase-qualified task+8 publication" : "committed matching CP165 root"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull);
        CHECK(state.qstringSharedNullBinding.expired() && !state.qstringSharedNullReferenceCommitted);
        CHECK(stringIdentity->referenceDelta == 1 && listIdentity->referenceDelta == 2);
        CHECK(foreign->qstringSharedNull()->referenceDelta == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
    }
}

TEST_CASE(doe_task_factory_one_shot_root_association_keeps_cp166_owner)
{
    for (const bool foreignAttempt : {false, true}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP166 expected root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP166 foreign root");
        store->associateWidgetRuntime(widgets);
        if (foreignAttempt) store->associateWidgetRuntime(foreign);
        else store->associateWidgetRuntime({});
        CHECK(store->ownsWidgetRuntime(widgets) && !store->ownsWidgetRuntime(foreign));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        const auto& frame = *pending->qobjectPrefix;
        CHECK(store->ownsWidgetRuntime(widgets) && !store->ownsWidgetRuntime(foreign));
        CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink);
        CHECK(frame.privateState->private40 == std::optional<std::uint64_t>{0});
        CHECK(frame.privateState->sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(frame.privateState->qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(frame.privateState->sharedNullReferenceCommitted && frame.privateState->qstringSharedNullReferenceCommitted);
        CHECK(widgets->qlistSharedNull()->referenceDelta == 2 && widgets->qstringSharedNull()->referenceDelta == 1);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qstringSharedNull()->referenceDelta == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
    }
}

TEST_CASE(doe_task_factory_qstring_guard_refuses_expired_root_without_new_delta)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP166 expected root");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto* privateIdentity = frame.privateIdentity();
    const auto listIdentity = widgets->qlistSharedNull();
    const auto stringIdentity = widgets->qstringSharedNull();
    // Reconstitute the CP165 cut; the first symbolic deltas are the baseline.
    frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
    state.private40.reset();
    state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
    state.qstringSharedNullBinding.reset();
    state.qstringSharedNullReferenceCommitted = false;
    widgets.reset();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "committed matching CP165 root"));
    CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull);
    CHECK(state.qstringSharedNullBinding.expired() && !state.qstringSharedNullReferenceCommitted);
    CHECK(listIdentity->referenceDelta == 2 && stringIdentity->referenceDelta == 1);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
}

TEST_CASE(doe_task_factory_first_qlist_guard_refuses_qualified_private_replacement_without_effects)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP169 first QList owner");
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    // The unpaired root leaves the published frame before the first binding.
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root WidgetRuntimeState"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    REQUIRE(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    REQUIRE(frame.privateIdentity() == frame.allocatedPrivateIdentity);
    auto clone = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(*frame.privateState);
    auto originalPrivate = std::move(frame.privateState);
    frame.privateState = std::move(clone);
    auto& state = *frame.privateState;
    REQUIRE(originalPrivate.get() == frame.allocatedPrivateIdentity);
    REQUIRE(frame.privateIdentity() != frame.allocatedPrivateIdentity);
    REQUIRE(state.nativeAllocationBytes == 0x88 && state.addressPointWritten);
    REQUIRE(state.addressPointRva == 0x1a9a30);
    REQUIRE(state.sharedNullBinding.expired() && !state.sharedNullReferenceCommitted);
    REQUIRE(state.qstringSharedNullBinding.expired() && !state.qstringSharedNullReferenceCommitted);
    REQUIRE(!state.private40 && !state.private48 && !state.private50);
    store->associateWidgetRuntime(widgets);
    REQUIRE(store->ownsWidgetRuntime(widgets));

    const auto* allocatedIdentity = frame.allocatedPrivateIdentity;
    const auto* currentIdentity = frame.privateIdentity();
    const auto list = widgets->qlistSharedNull();
    const auto string = widgets->qstringSharedNull();
    const auto listDelta = list->referenceDelta;
    const auto stringDelta = string->referenceDelta;
    const auto prototypeCount = store->retainedPrototypeCount();
    const auto taskCount = store->retainedTaskCount();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "allocation-time private identity"));
    CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == currentIdentity);
    CHECK(frame.allocatedPrivateIdentity == allocatedIdentity);
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(state.sharedNullBinding.expired() && !state.sharedNullReferenceCommitted);
    CHECK(state.qstringSharedNullBinding.expired() && !state.qstringSharedNullReferenceCommitted);
    CHECK(state.addressPointRva == 0x1a9a30 && state.addressPointWritten);
    CHECK(!state.private40 && !state.private48 && !state.private50);
    CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
    CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    CHECK(!pending->qobjectBaseCompleted && !frame.complete);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
}

TEST_CASE(doe_task_factory_rejects_foreign_root_before_shared_null_binding)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto owner = std::make_shared<engine::WidgetRuntimeState>(true, "CP164 actual root");
    auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP164 foreign root");
    doe::TaskFactoryRuntime wrong(store, foreign);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(wrong, 11);
    // A factory cannot appoint itself as the root, even when it is first.
    CHECK(!store->ownsWidgetRuntime(owner) && !store->ownsWidgetRuntime(foreign));
    CHECK(refuses([&] { wrong.createTaskBase(L("SfTaskDoe")); }, "matching root WidgetRuntimeState"));
    REQUIRE(store->retainedPrototypeCount() == 1);
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    const auto* privateIdentity = pending->qobjectPrefix->privateIdentity();
    const auto& frame = *pending->qobjectPrefix;
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(frame.privateState->sharedNullBinding.expired() && !frame.privateState->sharedNullReferenceCommitted);
    CHECK(owner->qlistSharedNull()->referenceDelta == 0 && foreign->qlistSharedNull()->referenceDelta == 0);
    store->associateWidgetRuntime(owner); // The actual Session supplies this pairing.
    CHECK(store->ownsWidgetRuntime(owner) && !store->ownsWidgetRuntime(foreign));
    CHECK(refuses([&] { wrong.createTaskBase(L("SfTaskDoe")); }, "matching root WidgetRuntimeState"));
    CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == privateIdentity);
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(frame.privateState->sharedNullBinding.expired() && !frame.privateState->sharedNullReferenceCommitted);
    CHECK(owner->qlistSharedNull()->referenceDelta == 0 && foreign->qlistSharedNull()->referenceDelta == 0);
    CHECK(wrong.construction().metadataCacheCount == 0 && wrong.construction().taskCacheCount == 0);
    CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
}

TEST_CASE(doe_task_factory_rejects_first_call_from_another_actual_session_scope)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession ownerSession(driver);
    engine::ConversionSession foreignSession(driver);
    const auto store = ownerSession.taskAllocationStoreOwner();
    const auto owner = ownerSession.widgetRuntimeOwner();
    const auto foreign = foreignSession.widgetRuntimeOwner();
    REQUIRE(store && owner && foreign && owner != foreign);
    CHECK(store->ownsWidgetRuntime(owner) && !store->ownsWidgetRuntime(foreign));
    doe::TaskFactoryRuntime wrong(store, foreign);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(wrong, 11);
    CHECK(refuses([&] { wrong.createTaskBase(L("SfTaskDoe")); }, "matching root WidgetRuntimeState"));
    REQUIRE(store->retainedPrototypeCount() == 1);
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(pending->qobjectPrefix->privateState->sharedNullBinding.expired());
    CHECK(!pending->qobjectPrefix->privateState->sharedNullReferenceCommitted);
    CHECK(owner->qlistSharedNull()->referenceDelta == 0 && foreign->qlistSharedNull()->referenceDelta == 0);
}

TEST_CASE(doe_task_factory_rejects_aliasing_shared_ptr_with_foreign_widget_pointee)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto expected = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 expected owner");
    auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP165 foreign pointee");
    store->associateWidgetRuntime(expected);
    std::shared_ptr<engine::WidgetRuntimeState> aliased(expected, foreign.get());
    CHECK(!store->ownsWidgetRuntime(aliased));
    doe::TaskFactoryRuntime factory(store, aliased);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root WidgetRuntimeState"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    const auto& frame = *pending->qobjectPrefix;
    CHECK(frame.phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(frame.privateState->sharedNullBinding.expired());
    CHECK(!frame.privateState->sharedNullReferenceCommitted);
    CHECK(frame.privateState->addressPointRva == 0x1a9a30);
    CHECK(expected->qlistSharedNull()->referenceDelta == 0);
    CHECK(foreign->qlistSharedNull()->referenceDelta == 0);
}

TEST_CASE(doe_task_factory_independent_frames_share_root_symbols_and_accumulate_symbolic_deltas)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP164 shared root owner");
    store->associateWidgetRuntime(widgets);
    const auto rootIdentity = widgets->qlistSharedNull();
    const auto stringIdentity = widgets->qstringSharedNull();
    doe::TaskFactoryRuntime first(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(first, 11);
    doe::TaskFactoryRuntime second(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(second, 11);
    CHECK(refuses([&] { first.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    CHECK(refuses([&] { second.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    REQUIRE(store->retainedPrototypeCount() == 2);
    const auto* firstBinding = store->retainedPrototypeAt(0)->qobjectPrefix->privateState->sharedNullBinding.lock().get();
    const auto* secondBinding = store->retainedPrototypeAt(1)->qobjectPrefix->privateState->sharedNullBinding.lock().get();
    const auto* firstStringBinding = store->retainedPrototypeAt(0)->qobjectPrefix->privateState->qstringSharedNullBinding.lock().get();
    const auto* secondStringBinding = store->retainedPrototypeAt(1)->qobjectPrefix->privateState->qstringSharedNullBinding.lock().get();
    CHECK(firstBinding == rootIdentity.get() && secondBinding == rootIdentity.get());
    CHECK(firstStringBinding == stringIdentity.get() && secondStringBinding == stringIdentity.get());
    CHECK(rootIdentity->referenceDelta == 4 && !rootIdentity->observedAbsoluteCount);
    CHECK(stringIdentity->referenceDelta == 2);
    // Two genuine retained private allocations share roots, but their unknown
    // flags reads must retain distinct B origins even with identical masks.
    const auto& firstFrame = *store->retainedPrototypeAt(0)->qobjectPrefix;
    const auto& secondFrame = *store->retainedPrototypeAt(1)->qobjectPrefix;
    REQUIRE(firstFrame.flags20Eax && secondFrame.flags20Eax);
    REQUIRE(firstFrame.privateState->private20 && secondFrame.privateState->private20);
    CHECK(firstFrame.allocatedPrivateIdentity != secondFrame.allocatedPrivateIdentity);
    CHECK(firstFrame.flags20Eax->privateIdentity == firstFrame.allocatedPrivateIdentity);
    CHECK(secondFrame.flags20Eax->privateIdentity == secondFrame.allocatedPrivateIdentity);
    CHECK(firstFrame.flags20Eax->privateIdentity != secondFrame.flags20Eax->privateIdentity);
    CHECK(firstFrame.flags20Eax->readRva == secondFrame.flags20Eax->readRva);
    CHECK(firstFrame.flags20Eax->knownOne == secondFrame.flags20Eax->knownOne);
    CHECK(firstFrame.flags20Eax->knownZero == secondFrame.flags20Eax->knownZero);
    CHECK(*firstFrame.flags20Eax != *secondFrame.flags20Eax);
    CHECK(firstFrame.privateState->private20 == firstFrame.flags20Eax);
    CHECK(secondFrame.privateState->private20 == secondFrame.flags20Eax);
    CHECK(firstFrame.privateState->private20 != secondFrame.privateState->private20);
}

TEST_CASE(doe_task_factory_native_unknown_key_still_initializes_before_key_lookup)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("Unregistered")); }, "RVA 0x131185"));
    CHECK(factory.construction().dataCommitted && store->retainedPrototypeCount() == 1);
}

TEST_CASE(doe_task_factory_repeat_guard_does_not_replay_allocations_or_complete_cache)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "RVA 0x131185"));
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    CHECK(factory.construction().taskCacheCount == 0 && factory.construction().nextPrototypeOrdinal == 0);
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending->qobjectPrefix && pending->qobjectPrefix->privateIdentity());
    const auto* privateIdentity = pending->qobjectPrefix->privateIdentity();
    CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
    CHECK(!pending->qobjectPrefix->complete && !pending->qobjectBaseCompleted);
    for (int index = 0; index < 2; ++index) {
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "RVA 0x131185"));
        CHECK(store->retainedPrototypeAt(0) == pending);
        CHECK(pending->qobjectPrefix->privateIdentity() == privateIdentity);
        CHECK(pending->qobjectPrefix->phase == doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList);
        CHECK(pending->qobjectPrefix->taskAddressPointWritten && pending->qobjectPrefix->privateState->addressPointWritten);
    }
    //This is retained adapter suspension, not a native C++ failure/retry claim.
}

TEST_CASE(doe_task_factory_expired_allocation_owner_guards_after_factory_data_commits)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    doe::TaskFactoryRuntime factory(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    store.reset();
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "before first Report allocation"));
    const auto state = factory.construction();
    CHECK(state.factoryCommitted && state.dataCommitted && state.dataFactoryBacklinkMatches);
    CHECK(!state.pendingReportAllocation && state.taskCacheCount == 0);
}

TEST_CASE(doe_task_factory_explicit_projection_is_not_native_initialization)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    const auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    const auto first = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    const auto second = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    CHECK(first && second && first.get() != second.get());
    CHECK(factory->profile() == doe::TaskFactoryProfile::BoundedParsedDoeProjection);
    CHECK(!factory->construction().factoryCommitted && store->retainedPrototypeCount() == 0);
    CHECK(!factory->construction().nativeInitializationComplete && store->retainedTaskCount() == 2);
    CHECK(doe::dispatchCreateTaskBase(*io, qt::String()) == nullptr);
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("Custom")); }, "bounded projection"));
}

TEST_CASE(doe_task_factory_observed_empty_list_inserts_key_without_resetting_flag90)
{
    auto io = doe::makeRecordingStudyIo();
    auto& notifier = io->sessionNotificationState();
    notifier.flag90 = true;
    notifier.createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {}, "Explicit observed empty return-receiver list", false};
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(notifier.createTaskBaseRegistrations->mapKeyPresent && notifier.flag90);
    CHECK(notifier.emptyKeys.count("2signalCreateTaskBase()") == 1);
}

TEST_CASE(doe_task_factory_expired_receiver_is_skipped_without_pruning_persistent_list)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    factory.reset();
    CHECK(!io->taskFactoryRuntimeOwner());
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(io->sessionNotificationState().createTaskBaseRegistrations->entries.size() == 1);
    CHECK(io->sessionNotificationState().emptyKeys.empty() && store->retainedTaskCount() == 0);
}

TEST_CASE(doe_task_factory_receiver_order_retains_first_effect_before_second_receiver_guard)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto first = doe::TaskFactoryRuntime::boundedParsedDoeProjection(store);
    auto second = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*second, 11);
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{first, "slotCreateTaskBase"}, {second, "slotCreateTaskBase"}},
        "Explicit ordered two-receiver fixture, not fixed startup singleton", true};
    CHECK(refuses([&] { doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")); }, "RVA 0x131185"));
    CHECK(store->retainedTaskCount() == 1 && store->retainedPrototypeCount() == 1);
    CHECK(!first->construction().factoryCommitted && second->construction().dataCommitted);
}

TEST_CASE(doe_task_factory_binding_rejects_replacement_and_released_receiver_is_skipped)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    const auto factory = doe::bindBoundedParsedDoeProjection(*io, store);
    io->bindTaskFactoryRuntime(factory);
    auto other = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*other, 11);
    CHECK(refuses([&] { io->bindTaskFactoryRuntime(other); }, "without replacement"));
    factory->releaseAdapterScope();
    factory->releaseAdapterScope();
    CHECK(!factory->nativeReceiverAlive() && io->taskFactoryRuntimeOwner() == factory);
    CHECK(doe::dispatchCreateTaskBase(*io, L("SfTaskDoe")) == nullptr);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "scope has been released"));
    CHECK(store->retainedTaskCount() == 0);
}

TEST_CASE(doe_task_factory_released_holder_does_not_delete_store_owned_pending_prototype)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto factory = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131185"));
    factory->releaseAdapterScope();
    CHECK(!factory->construction().factoryCommitted && store->retainedPrototypeCount() == 1);
    CHECK(!store->retainedPrototypeAt(0)->qobjectBaseCompleted);
    const auto* privateIdentity = store->retainedPrototypeAt(0)->qobjectPrefix->privateIdentity();
    REQUIRE(privateIdentity);
    store->releaseAdapterScope();
    CHECK(store->retainedPrototypeCount() == 0);
    //Both releases are adapter scope cleanup, not native normal-Close proof.
}

TEST_CASE(doe_task_factory_binding_cannot_replace_expired_actual_owner)
{
    auto io = doe::makeRecordingStudyIo();
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto original = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*original, 11);
    io->bindTaskFactoryRuntime(original);
    original.reset();
    CHECK(!io->taskFactoryRuntimeOwner());
    auto replacement = std::make_shared<doe::TaskFactoryRuntime>(store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*replacement, 11);
    CHECK(refuses([&] { io->bindTaskFactoryRuntime(replacement); }, "without replacement"));
    CHECK(refuses([&] { io->bindTaskFactoryRuntime({}); }, "without replacement"));
    CHECK(!io->taskFactoryRuntimeOwner());
}

TEST_CASE(doe_task_factory_return_slot_is_overwritten_in_receiver_order)
{
    auto firstStore = std::make_shared<doe::TaskAllocationStore>();
    auto secondStore = std::make_shared<doe::TaskAllocationStore>();
    auto first = doe::TaskFactoryRuntime::boundedParsedDoeProjection(firstStore);
    auto second = doe::TaskFactoryRuntime::boundedParsedDoeProjection(secondStore);
    auto io = doe::makeRecordingStudyIo();
    io->sessionNotificationState().createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{first, "slotCreateTaskBase"}, {second, "slotCreateTaskBase"}},
        "Explicit ordered multi-receiver fixture; not native startup population", true};
    const auto result = doe::dispatchCreateTaskBase(*io, L("SfTaskDoe"));
    REQUIRE(result);
    CHECK(firstStore->retainedTaskCount() == 1 && secondStore->retainedTaskCount() == 1);
    CHECK(result.get() == secondStore->retainedTaskAt(0).get());
    CHECK(result.get() != firstStore->retainedTaskAt(0).get());
}


TEST_CASE(doe_task_factory_zero_prefix_seams_keep_phase_diagnostics_and_prior_sparse_effects)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    struct Seam { Phase phase; const char* diagnostic; int damagedField; };
    const Seam seams[] = {
        {Phase::BeforePrivateZeroFields, "CP166 roots at RVA 0x1311b5", 1},
        {Phase::BeforePrivateZero48, "CP167 roots and private+0x40", 2},
        {Phase::BeforePrivateZero50, "CP168 state at RVA 0x1311bf", 3},
        {Phase::BeforePrivateZero58, "CP169 state at RVA 0x1311c3", 4},
    };
    for (const auto& seam : seams) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "zero seam regression root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        state.private40.reset(); state.private48.reset(); state.private50.reset(); state.private58.reset();
        state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        if (seam.phase != Phase::BeforePrivateZeroFields) state.private40 = std::uint64_t{0};
        if (seam.phase == Phase::BeforePrivateZero50 || seam.phase == Phase::BeforePrivateZero58)
            state.private48 = std::uint64_t{0};
        if (seam.phase == Phase::BeforePrivateZero58) state.private50 = std::uint64_t{0};
        if (seam.damagedField == 1) state.private48 = std::uint64_t{0};
        if (seam.damagedField == 2) state.private40 = std::uint64_t{0x40};
        if (seam.damagedField == 3) state.private48 = std::uint64_t{0x48};
        if (seam.damagedField == 4) state.private50 = std::uint64_t{0x50};
        frame.phase = seam.phase; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        const auto before40 = state.private40; const auto before48 = state.private48;
        const auto before50 = state.private50; const auto before58 = state.private58;
        const auto beforePhase = frame.phase;
        const auto* privateIdentity = frame.privateIdentity();
        const auto* allocatedIdentity = frame.allocatedPrivateIdentity;
        const auto listBinding = state.sharedNullBinding.lock();
        const auto stringBinding = state.qstringSharedNullBinding.lock();
        const auto listCommitted = state.sharedNullReferenceCommitted;
        const auto stringCommitted = state.qstringSharedNullReferenceCommitted;
        const auto taskAddress = frame.taskAddressPointRva;
        const auto taskAddressWritten = frame.taskAddressPointWritten;
        const auto privateAddress = state.addressPointRva;
        const auto privateAddressWritten = state.addressPointWritten;
        const auto privateSize = state.nativeAllocationBytes;
        const auto taskSize = pending->nativeAllocationBytes;
        const auto rootList = widgets->qlistSharedNull();
        const auto rootString = widgets->qstringSharedNull();
        const auto listDelta = rootList->referenceDelta;
        const auto stringDelta = rootString->referenceDelta;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, seam.diagnostic));
        CHECK(state.private40 == before40 && state.private48 == before48);
        CHECK(state.private50 == before50 && state.private58 == before58);
        CHECK(frame.phase == beforePhase && !pending->qobjectBaseCompleted && !frame.complete);
        CHECK(frame.privateIdentity() == privateIdentity && frame.allocatedPrivateIdentity == allocatedIdentity);
        CHECK(state.sharedNullBinding.lock() == listBinding && state.qstringSharedNullBinding.lock() == stringBinding);
        CHECK(state.sharedNullReferenceCommitted == listCommitted && state.qstringSharedNullReferenceCommitted == stringCommitted);
        CHECK(frame.taskAddressPointRva == taskAddress && frame.taskAddressPointWritten == taskAddressWritten);
        CHECK(state.addressPointRva == privateAddress && state.addressPointWritten == privateAddressWritten);
        CHECK(state.nativeAllocationBytes == privateSize && pending->nativeAllocationBytes == taskSize);
        CHECK(widgets->qlistSharedNull() == rootList && widgets->qstringSharedNull() == rootString);
        CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
        CHECK(rootList->referenceDelta == listDelta && rootString->referenceDelta == stringDelta);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    }
}

int main() { return testkit::run_all(); }

TEST_CASE(doe_task_factory_qvector_shared_null_increment_uses_session_root_once)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto store = session.taskAllocationStoreOwner();
    const auto widgets = session.widgetRuntimeOwner();
    REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
    const auto vector = widgets->qvectorSharedNull();
    REQUIRE(vector);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    REQUIRE(store->retainedPrototypeCount() == 1);
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    const auto& frame = *pending->qobjectPrefix;
    const auto& state = *frame.privateState;
    CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
    CHECK(state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
    CHECK(state.secondQListReferenceCommitted);
    CHECK(state.qvectorSharedNullBinding.lock() == vector);
    const auto bound = state.qvectorSharedNullBinding.lock();
    CHECK(!bound.owner_before(vector) && !vector.owner_before(bound));
    CHECK(state.qvectorSharedNullReferenceCommitted && vector->referenceDelta == 1);
    CHECK(!vector->observedAbsoluteCount && !vector->observedAtDelta);
    CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
    CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
    CHECK(!frame.complete && !pending->qobjectBaseCompleted);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    CHECK(store->retainedTaskCount() == 0 && store->retainedPrototypeCount() == 1);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskDoe")); }, "RVA 0x131425"));
    CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink && state.qvectorSharedNullBinding.lock() == vector);
    CHECK(state.qvectorSharedNullReferenceCommitted && vector->referenceDelta == 1);
}

TEST_CASE(doe_task_factory_malformed_qvector_root_refuses_without_mutation)
{
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP172 malformed QVector root");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
    vector->qualifiedSymbolRva = 0x2f0bb9;
    frame.phase = doe::PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNull; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
    state.qvectorSharedNullBinding.reset();
    state.qvectorSharedNullReferenceCommitted = false;
    const auto identity = frame.privateIdentity();
    const auto phase = frame.phase;
    const auto vectorDelta = vector->referenceDelta;
    const auto listDelta = widgets->qlistSharedNull()->referenceDelta;
    const auto stringDelta = widgets->qstringSharedNull()->referenceDelta;
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "QVector shared-null binding"));
    CHECK(frame.privateIdentity() == identity && frame.phase == phase);
    CHECK(state.qvectorSharedNullBinding.expired() && !state.qvectorSharedNullReferenceCommitted);
    CHECK(vector->referenceDelta == vectorDelta && widgets->qlistSharedNull()->referenceDelta == listDelta);
    CHECK(widgets->qstringSharedNull()->referenceDelta == stringDelta);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
}

TEST_CASE(doe_task_factory_cp172_refuses_corrupt_prebinding_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { PrematureBinding, PrematureCommit, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva };
    constexpr Damage damages[] = {Damage::PrematureBinding, Damage::PrematureCommit, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP172 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP172 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforeQVectorSharedNull; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullBinding.reset(); state.qvectorSharedNullReferenceCommitted = false;
        switch (damage) {
        case Damage::PrematureBinding: state.qvectorSharedNullBinding = vector; break;
        case Damage::PrematureCommit: state.qvectorSharedNullReferenceCommitted = true; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto listCount = list->observedAbsoluteCount;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "QVector shared-null binding"));
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}



TEST_CASE(doe_task_factory_cp173_refuses_corrupt_preincrement_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, PrematureCommit, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::PrematureCommit, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP173 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP173 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforeQVectorSharedNullIncrement; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = false;
        switch (damage) {
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::PrematureCommit: state.qvectorSharedNullReferenceCommitted = true; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto listCount = list->observedAbsoluteCount;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "QVector shared-null increment"));
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}



TEST_CASE(doe_task_factory_cp173_symbolic_increment_preserves_observation_and_wraps)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (const bool observed : {false, true}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP173 symbolic increment");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        // Reconstitute accepted CP172 with an independent root history. This is
        // a symbolic fixture, never a claim about a live native reference count.
        frame.phase = Phase::BeforeQVectorSharedNullIncrement; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullReferenceCommitted = false;
        vector->referenceDelta = observed ? 0xffffffffu : 7u;
        if (observed) {
            vector->observedAbsoluteCount = 42u;
            vector->observedAtDelta = 0xfffffffeu;
            vector->observationQualification = "isolated supplied baseline fixture";
        }
        const auto absolute = vector->observedAbsoluteCount, atDelta = vector->observedAtDelta;
        const auto qualification = vector->observationQualification;
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto* identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        CHECK(vector->referenceDelta == (observed ? 0u : 8u));
        CHECK(state.qvectorSharedNullReferenceCommitted && frame.phase == Phase::BeforeTaskPrivateBacklink);
        CHECK(frame.privateIdentity() == identity && frame.allocatedPrivateIdentity == identity);
        CHECK(state.qvectorSharedNullBinding.lock() == vector);
        CHECK(state.sharedNullBinding.lock() == list && state.qstringSharedNullBinding.lock() == string);
        CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
        CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(vector->referenceDelta == (observed ? 0u : 8u));
            CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink && state.qvectorSharedNullReferenceCommitted);
        }
        CHECK(vector->observedAbsoluteCount == absolute && vector->observedAtDelta == atDelta);
        CHECK(vector->observationQualification == qualification);
        CHECK(list->referenceDelta == listDelta + 1u && string->referenceDelta == stringDelta);
        CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted);
        CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
    }
}

TEST_CASE(doe_task_factory_cp174_refuses_corrupt_prebinding_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, PrematureSecondBinding, PrematureSecondCommit, ExpiredSecondBinding, ExpiredList, ExpiredQString };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::PrematureSecondBinding, Damage::PrematureSecondCommit, Damage::ExpiredSecondBinding, Damage::ExpiredList, Damage::ExpiredQString};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforeSecondQListBinding; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding.reset(); frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::PrematureSecondBinding: state.secondQListSharedNullBinding = list; break;
        case Damage::PrematureSecondCommit: state.secondQListReferenceCommitted = true; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto listCount = list->observedAbsoluteCount;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "second QList binding"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}




TEST_CASE(doe_task_factory_cp174_session_store_preserves_root_history_and_reentry)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto store = session.taskAllocationStoreOwner();
    const auto widgets = session.widgetRuntimeOwner();
    REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
    const auto string = widgets->qstringSharedNull();
    const auto vector = widgets->qvectorSharedNull();
    REQUIRE(state.secondQListSharedNullBinding.lock() == list);
    // Reconstitute the accepted CP173 predecessor with independent symbolic
    // history and a supplied observation. Continuation adds only CP175's event.
    frame.phase = Phase::BeforeSecondQListBinding; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    state.secondQListSharedNullBinding.reset();
    state.secondQListReferenceCommitted = false; state.private78.reset(); state.private80.reset();
    list->referenceDelta = 17u;
    list->observedAbsoluteCount = 42u; list->observedAtDelta = 11u;
    list->observationQualification = "isolated supplied CP174 fixture";
    const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
    const auto vectorDelta = vector->referenceDelta;
    const auto absolute = list->observedAbsoluteCount, observedAt = list->observedAtDelta;
    const auto qualification = list->observationQualification;
    const auto identity = frame.privateIdentity();
    const auto construction = factory.construction();
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
        CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
        CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && frame.phase == Phase::BeforeTaskPrivateBacklink);
        const auto second = state.secondQListSharedNullBinding.lock();
        REQUIRE(second);
        CHECK(second == list && second == state.sharedNullBinding.lock());
        CHECK(!second.owner_before(list) && !list.owner_before(second));
        CHECK(state.secondQListReferenceCommitted);
        CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
        CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
        CHECK(list->referenceDelta == listDelta + 1u && string->referenceDelta == stringDelta && vector->referenceDelta == vectorDelta);
        CHECK(list->observedAbsoluteCount == absolute && list->observedAtDelta == observedAt);
        CHECK(list->observationQualification == qualification);
        CHECK(!vector->observedAbsoluteCount && !vector->observedAtDelta);
        CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
        CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
        CHECK(state.nativeAllocationBytes == 0x88 && pending->nativeAllocationBytes == 0xc0);
        CHECK(state.addressPointWritten && state.addressPointRva == 0x1a9d10);
        CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted);
        CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
        CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
    }
}

TEST_CASE(doe_task_factory_cp174_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforeSecondQListBinding; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "second QList binding"));
        CHECK(frame.phase == Phase::BeforeSecondQListBinding && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.expired());
        CHECK(!state.secondQListReferenceCommitted);
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}

TEST_CASE(doe_task_factory_cp175_refuses_corrupt_preincrement_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, PrematureSecondCommit, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::PrematureSecondCommit, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP175 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP175 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforeSecondQListIncrement; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = false; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::PrematureSecondCommit: state.secondQListReferenceCommitted = true; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "second QList increment"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}





TEST_CASE(doe_task_factory_cp175_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforeSecondQListIncrement; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "second QList increment"));
        CHECK(frame.phase == Phase::BeforeSecondQListIncrement && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(!state.secondQListReferenceCommitted);
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}


TEST_CASE(doe_task_factory_cp175_session_increment_preserves_observation_wrap_and_reentry)
{
    for (const bool observed : {false, true}) {
        using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        REQUIRE(state.secondQListSharedNullBinding.lock() == list);
        // Reconstitute accepted CP174 with independent symbolic history; the
        // supplied absolute baseline is a fixture, never a live native count.
        frame.phase = Phase::BeforeSecondQListIncrement; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.secondQListReferenceCommitted = false; state.private78.reset(); state.private80.reset();
        list->referenceDelta = observed ? 0xffffffffu : 17u;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 0xfffffffeu;
            list->observationQualification = "isolated supplied CP175 fixture";
        }
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto vectorDelta = vector->referenceDelta;
        const auto absolute = list->observedAbsoluteCount, observedAt = list->observedAtDelta;
        const auto qualification = list->observationQualification;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
            CHECK(frame.allocatedPrivateIdentity == identity && frame.phase == Phase::BeforeTaskPrivateBacklink);
            const auto second = state.secondQListSharedNullBinding.lock();
            REQUIRE(second);
            CHECK(second == list && second == state.sharedNullBinding.lock());
            CHECK(!second.owner_before(list) && !list.owner_before(second));
            CHECK(state.secondQListReferenceCommitted);
            CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
            CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
            CHECK(list->referenceDelta == static_cast<std::uint32_t>(listDelta + 1u) && string->referenceDelta == stringDelta && vector->referenceDelta == vectorDelta);
            CHECK(list->observedAbsoluteCount == absolute && list->observedAtDelta == observedAt);
            CHECK(list->observationQualification == qualification);
            CHECK(list->projectedCount() == (observed ? std::optional<std::uint32_t>{44u} : std::nullopt));
            CHECK(!vector->observedAbsoluteCount && !vector->observedAtDelta);
            CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
            CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
            CHECK(state.nativeAllocationBytes == 0x88 && pending->nativeAllocationBytes == 0xc0);
            CHECK(state.addressPointWritten && state.addressPointRva == 0x1a9d10);
            CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
            CHECK(!frame.complete && !pending->qobjectBaseCompleted);
            CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
            CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
        }
    }
}

TEST_CASE(doe_task_factory_cp176_refuses_corrupt_prezero78_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, UncommittedSecond, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString, Premature78, WrongAllocationIdentity, Bad40, Missing48, Bad50, Missing58 };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::UncommittedSecond, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString, Damage::Premature78, Damage::WrongAllocationIdentity, Damage::Bad40, Damage::Missing48, Damage::Bad50, Damage::Missing58};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP176 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP176 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforePrivateZero78; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = true; frame.privateState->private78.reset(); frame.privateState->private80.reset();
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::Premature78: state.private78 = std::uint64_t{0}; break;
        case Damage::WrongAllocationIdentity: frame.allocatedPrivateIdentity = nullptr; break;
        case Damage::Bad40: state.private40 = std::uint64_t{1}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::Bad50: state.private50 = std::uint64_t{1}; break;
        case Damage::Missing58: state.private58.reset(); break;
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::UncommittedSecond: state.secondQListReferenceCommitted = false; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto before78 = frame.privateState->private78;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "private+0x78 zero"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateState->private78 == before78);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}






TEST_CASE(doe_task_factory_cp176_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP174 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforePrivateZero78; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        state.secondQListReferenceCommitted = true;
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "private+0x78 zero"));
        CHECK(frame.phase == Phase::BeforePrivateZero78 && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.secondQListReferenceCommitted && !state.private78);
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}



TEST_CASE(doe_task_factory_cp176_session_zero78_preserves_observation_and_reentry)
{
    for (const bool observed : {false, true}) {
        using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        REQUIRE(state.secondQListSharedNullBinding.lock() == list);
        // Reconstitute accepted CP175 with independent symbolic history; the
        // supplied absolute baseline is a fixture, never a live native count.
        frame.phase = Phase::BeforePrivateZero78; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.secondQListReferenceCommitted = true; state.private78.reset(); state.private80.reset();
        list->referenceDelta = observed ? 0xffffffffu : 17u;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 0xfffffffeu;
            list->observationQualification = "isolated supplied CP176 fixture";
        }
        CHECK(!state.private78);
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto vectorDelta = vector->referenceDelta;
        const auto absolute = list->observedAbsoluteCount, observedAt = list->observedAtDelta;
        const auto qualification = list->observationQualification;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
            CHECK(frame.allocatedPrivateIdentity == identity && frame.phase == Phase::BeforeTaskPrivateBacklink);
            const auto second = state.secondQListSharedNullBinding.lock();
            REQUIRE(second);
            CHECK(second == list && second == state.sharedNullBinding.lock());
            CHECK(!second.owner_before(list) && !list.owner_before(second));
            CHECK(state.secondQListReferenceCommitted);
            CHECK(state.private78 == std::optional<std::uint64_t>{0});
            CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
            CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
            CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta && vector->referenceDelta == vectorDelta);
            CHECK(list->observedAbsoluteCount == absolute && list->observedAtDelta == observedAt);
            CHECK(list->observationQualification == qualification);
            CHECK(list->projectedCount() == (observed ? std::optional<std::uint32_t>{43u} : std::nullopt));
            CHECK(!vector->observedAbsoluteCount && !vector->observedAtDelta);
            CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
            CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
            CHECK(state.nativeAllocationBytes == 0x88 && pending->nativeAllocationBytes == 0xc0);
            CHECK(state.addressPointWritten && state.addressPointRva == 0x1a9d10);
            CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
            CHECK(!frame.complete && !pending->qobjectBaseCompleted);
            CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
            CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
        }
    }
}

static_assert(std::is_same_v<decltype(doe::PendingTaskPrototype::SparseQObjectPrivate::private78),
    std::optional<std::uint64_t>>);
TEST_CASE(doe_task_factory_cp176_fresh_private78_is_unknown)
{
    doe::PendingTaskPrototype::SparseQObjectPrivate state;
    CHECK(!state.private78.has_value());
}

TEST_CASE(doe_task_factory_cp177_refuses_corrupt_prezero80_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, UncommittedSecond, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString, Premature80, WrongAllocationIdentity, Bad40, Missing48, Bad50, Missing58, PrematureNonzero80, Missing78, Bad78 };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::UncommittedSecond, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString, Damage::Premature80, Damage::WrongAllocationIdentity, Damage::Bad40, Damage::Missing48, Damage::Bad50, Damage::Missing58, Damage::PrematureNonzero80, Damage::Missing78, Damage::Bad78};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP177 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP177 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforePrivateZero80; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = true; frame.privateState->private78 = std::uint64_t{0}; frame.privateState->private80.reset();
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::Premature80: state.private80 = std::uint64_t{0}; break;
        case Damage::PrematureNonzero80: state.private80 = std::uint64_t{1}; break;
        case Damage::Missing78: state.private78.reset(); break;
        case Damage::Bad78: state.private78 = std::uint64_t{1}; break;
        case Damage::WrongAllocationIdentity: frame.allocatedPrivateIdentity = nullptr; break;
        case Damage::Bad40: state.private40 = std::uint64_t{1}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::Bad50: state.private50 = std::uint64_t{1}; break;
        case Damage::Missing58: state.private58.reset(); break;
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::UncommittedSecond: state.secondQListReferenceCommitted = false; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto before78 = frame.privateState->private78;
        const auto before80 = frame.privateState->private80;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto weakList = frame.privateState->sharedNullBinding;
        const auto weakString = frame.privateState->qstringSharedNullBinding;
        const auto weakVector = frame.privateState->qvectorSharedNullBinding;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto vectorAt = vector->observedAtDelta;
        const auto vectorObservation = vector->observationQualification;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "private+0x80 zero"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateState->private78 == before78);
        CHECK(frame.privateState->private80 == before80);
        CHECK(!weakList.owner_before(frame.privateState->sharedNullBinding) && !frame.privateState->sharedNullBinding.owner_before(weakList));
        CHECK(!weakString.owner_before(frame.privateState->qstringSharedNullBinding) && !frame.privateState->qstringSharedNullBinding.owner_before(weakString));
        CHECK(!weakVector.owner_before(frame.privateState->qvectorSharedNullBinding) && !frame.privateState->qvectorSharedNullBinding.owner_before(weakVector));
        CHECK(vector->observedAtDelta == vectorAt && vector->observationQualification == vectorObservation);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}







TEST_CASE(doe_task_factory_cp177_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP177 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP177 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforePrivateZero80; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        state.secondQListReferenceCommitted = true;
        state.private78 = std::uint64_t{0};
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "private+0x80 zero"));
        CHECK(frame.phase == Phase::BeforePrivateZero80 && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.secondQListReferenceCommitted && !state.private80);
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}




TEST_CASE(doe_task_factory_cp177_session_zero80_preserves_observation_and_reentry)
{
    for (const bool observed : {false, true}) {
        using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        REQUIRE(state.secondQListSharedNullBinding.lock() == list);
        // Reconstitute accepted CP176 with independent symbolic history; the
        // supplied absolute baseline is a fixture, never a live native count.
        frame.phase = Phase::BeforePrivateZero80; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.secondQListReferenceCommitted = true; state.private78 = std::uint64_t{0}; state.private80.reset();
        list->referenceDelta = observed ? 0xffffffffu : 17u;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 0xfffffffeu;
            list->observationQualification = "isolated supplied CP177 fixture";
        }
        CHECK(state.private78 == std::optional<std::uint64_t>{0});
        CHECK(!state.private80);
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto vectorDelta = vector->referenceDelta;
        const auto absolute = list->observedAbsoluteCount, observedAt = list->observedAtDelta;
        const auto qualification = list->observationQualification;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
            CHECK(frame.allocatedPrivateIdentity == identity && frame.phase == Phase::BeforeTaskPrivateBacklink);
            const auto second = state.secondQListSharedNullBinding.lock();
            REQUIRE(second);
            CHECK(second == list && second == state.sharedNullBinding.lock());
            CHECK(!second.owner_before(list) && !list.owner_before(second));
            CHECK(state.secondQListReferenceCommitted);
            CHECK(state.private78 == std::optional<std::uint64_t>{0});
            CHECK(state.private80 == std::optional<std::uint64_t>{0});
            CHECK(state.sharedNullReferenceCommitted && state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
            CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
            CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta && vector->referenceDelta == vectorDelta);
            CHECK(list->observedAbsoluteCount == absolute && list->observedAtDelta == observedAt);
            CHECK(list->observationQualification == qualification);
            CHECK(list->projectedCount() == (observed ? std::optional<std::uint32_t>{43u} : std::nullopt));
            CHECK(!vector->observedAbsoluteCount && !vector->observedAtDelta);
            CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
            CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
            CHECK(state.nativeAllocationBytes == 0x88 && pending->nativeAllocationBytes == 0xc0);
            CHECK(state.addressPointWritten && state.addressPointRva == 0x1a9d10);
            CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
            CHECK(!frame.complete && !pending->qobjectBaseCompleted);
            CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
            CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
        }
    }
}


static_assert(std::is_same_v<decltype(doe::PendingTaskPrototype::SparseQObjectPrivate::private80),
    std::optional<std::uint64_t>>);
TEST_CASE(doe_task_factory_cp177_fresh_private80_is_unknown)
{
    doe::PendingTaskPrototype::SparseQObjectPrivate state;
    CHECK(!state.private80.has_value());
}

TEST_CASE(doe_task_factory_cp177_sparse_predecessor_needs_no_absolute_counts_or_prior_deltas)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP177 sparse supplied predecessor");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
    const auto string = std::const_pointer_cast<engine::WidgetRuntimeState::QStringSharedNullState>(widgets->qstringSharedNull());
    const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
    // Supply a retained CP176 frame independently of symbolic reference history.
    frame.phase = Phase::BeforePrivateZero80; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    frame.privateState->private08.reset();
    frame.privateState->private10.reset(); frame.privateState->private24.reset();
    frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
    state.private80.reset();
    list->referenceDelta = string->referenceDelta = vector->referenceDelta = 0;
    const auto identity = frame.privateIdentity();
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
        CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
        CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink && frame.privateIdentity() == identity);
        CHECK(state.private80 == std::optional<std::uint64_t>{0});
        CHECK(state.private78 == std::optional<std::uint64_t>{0});
        CHECK(list->referenceDelta == 0 && string->referenceDelta == 0 && vector->referenceDelta == 0);
        CHECK(!list->observedAbsoluteCount && !vector->observedAbsoluteCount);
        CHECK(state.sharedNullBinding.lock() == list && state.secondQListSharedNullBinding.lock() == list);
        CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
        CHECK(state.sharedNullReferenceCommitted && state.secondQListReferenceCommitted);
        CHECK(state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
        CHECK(store->retainedPrototypeAt(0) == pending && store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}


TEST_CASE(doe_task_factory_cp178_refuses_corrupt_preversion_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, UncommittedSecond, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString, Missing80, WrongAllocationIdentity, Bad40, Missing48, Bad50, Missing58, Bad80, Missing78, Bad78, MissingVersion, WrongVersion, WrongModule, WrongLoad, WrongCaller };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::UncommittedSecond, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString, Damage::Missing80, Damage::WrongAllocationIdentity, Damage::Bad40, Damage::Missing48, Damage::Bad50, Damage::Missing58, Damage::Bad80, Damage::Missing78, Damage::Bad78, Damage::MissingVersion, Damage::WrongVersion, Damage::WrongModule, Damage::WrongLoad, Damage::WrongCaller};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP178 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP178 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforeQtVersionCheck; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = true; frame.privateState->private78 = std::uint64_t{0}; frame.privateState->private80 = std::uint64_t{0};
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::MissingVersion: frame.privateInitializerVersion.reset(); break;
        case Damage::WrongVersion: frame.privateInitializerVersion->argument = 0x40803; break;
        case Damage::WrongModule: frame.privateInitializerVersion->moduleSha256 = "foreign QtCore"; break;
        case Damage::WrongLoad: frame.privateInitializerVersion->argumentLoadRva = 0x131410; break;
        case Damage::WrongCaller: frame.privateInitializerVersion->callRva = 0x131418; break;
        case Damage::Missing80: state.private80.reset(); break;
        case Damage::Bad80: state.private80 = std::uint64_t{1}; break;
        case Damage::Missing78: state.private78.reset(); break;
        case Damage::Bad78: state.private78 = std::uint64_t{1}; break;
        case Damage::WrongAllocationIdentity: frame.allocatedPrivateIdentity = nullptr; break;
        case Damage::Bad40: state.private40 = std::uint64_t{1}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::Bad50: state.private50 = std::uint64_t{1}; break;
        case Damage::Missing58: state.private58.reset(); break;
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::UncommittedSecond: state.secondQListReferenceCommitted = false; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto before78 = frame.privateState->private78;
        const auto before80 = frame.privateState->private80;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto beforeVersion = frame.privateInitializerVersion;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto weakList = frame.privateState->sharedNullBinding;
        const auto weakString = frame.privateState->qstringSharedNullBinding;
        const auto weakVector = frame.privateState->qvectorSharedNullBinding;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto vectorAt = vector->observedAtDelta;
        const auto vectorObservation = vector->observationQualification;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "Qt version comparison"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateState->private78 == before78);
        CHECK(frame.privateState->private80 == before80);
        CHECK(frame.privateInitializerVersion.has_value() == beforeVersion.has_value());
        if (beforeVersion) {
            CHECK(frame.privateInitializerVersion->moduleSha256 == beforeVersion->moduleSha256);
            CHECK(frame.privateInitializerVersion->argument == beforeVersion->argument);
            CHECK(frame.privateInitializerVersion->argumentLoadRva == beforeVersion->argumentLoadRva);
            CHECK(frame.privateInitializerVersion->callRva == beforeVersion->callRva);
        }
        CHECK(!weakList.owner_before(frame.privateState->sharedNullBinding) && !frame.privateState->sharedNullBinding.owner_before(weakList));
        CHECK(!weakString.owner_before(frame.privateState->qstringSharedNullBinding) && !frame.privateState->qstringSharedNullBinding.owner_before(weakString));
        CHECK(!weakVector.owner_before(frame.privateState->qvectorSharedNullBinding) && !frame.privateState->qvectorSharedNullBinding.owner_before(weakVector));
        CHECK(vector->observedAtDelta == vectorAt && vector->observationQualification == vectorObservation);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}


TEST_CASE(doe_task_factory_cp178_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP178 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP178 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforeQtVersionCheck; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        state.secondQListReferenceCommitted = true;
        state.private78 = state.private80 = std::uint64_t{0};
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "Qt version comparison"));
        CHECK(frame.phase == Phase::BeforeQtVersionCheck && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.secondQListReferenceCommitted && state.private80 == std::optional<std::uint64_t>{0});
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}

static_assert(0x1311f7 + 2 + 0x12 == 0x13120b); // JE rel8 target, after the eight-byte cut
static_assert(std::is_same_v<decltype(doe::PendingTaskPrototype::QObjectPrefix::privateInitializerVersion),
    std::optional<doe::PendingTaskPrototype::QObjectPrefix::PrivateInitializerVersion>>);
TEST_CASE(doe_task_factory_cp178_fresh_frame_has_no_qualified_caller)
{
    doe::PendingTaskPrototype::QObjectPrefix frame;
    CHECK(!frame.privateInitializerVersion);
}

TEST_CASE(doe_task_factory_cp178_session_equal_branch_preserves_state_and_reentry)
{
    using Prefix = doe::PendingTaskPrototype::QObjectPrefix;
    using Phase = Prefix::Phase;
    for (const bool observed : {false, true}) {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        REQUIRE(frame.privateInitializerVersion);
        CHECK(frame.privateInitializerVersion->moduleSha256 == Prefix::moduleSha256);
        CHECK(frame.privateInitializerVersion->argument == 0x40804);
        CHECK(frame.privateInitializerVersion->argumentLoadRva == 0x13140f);
        CHECK(frame.privateInitializerVersion->callRva == 0x131417);
        CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
        // Supply the accepted CP177 predecessor, with independent symbolic
        // history and optional fixture observations, to isolate the decision.
        frame.phase = Phase::BeforeQtVersionCheck; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = std::const_pointer_cast<engine::WidgetRuntimeState::QStringSharedNullState>(widgets->qstringSharedNull());
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        list->referenceDelta = string->referenceDelta = vector->referenceDelta = observed ? 0xffffffffu : 0u;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 17u;
            list->observationQualification = "isolated CP178 list fixture";
            vector->observedAbsoluteCount = 31u; vector->observedAtDelta = 9u;
            vector->observationQualification = "isolated CP178 vector fixture";
        }
        const auto before = state;
        const auto version = *frame.privateInitializerVersion;
        const auto listBefore = *list;
        const auto vectorBefore = *vector;
        const auto stringDelta = string->referenceDelta;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        auto sameWeak = [](const auto& a, const auto& b) {
            return a.lock() == b.lock() && !a.owner_before(b) && !b.owner_before(a);
        };
        // First call executes CMP/JE, private+0x08, CP180/181 stores and
        // CP182 symbolic flags. Later same/different keys preserve the stop.
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
            CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
            CHECK(frame.allocatedPrivateIdentity == identity);
            REQUIRE(frame.privateInitializerVersion);
            CHECK(frame.privateInitializerVersion->moduleSha256 == version.moduleSha256);
            CHECK(frame.privateInitializerVersion->argument == version.argument);
            CHECK(frame.privateInitializerVersion->argumentLoadRva == version.argumentLoadRva);
            CHECK(frame.privateInitializerVersion->callRva == version.callRva);
            CHECK(sameWeak(state.sharedNullBinding, before.sharedNullBinding));
            CHECK(sameWeak(state.secondQListSharedNullBinding, before.secondQListSharedNullBinding));
            CHECK(sameWeak(state.qstringSharedNullBinding, before.qstringSharedNullBinding));
            CHECK(sameWeak(state.qvectorSharedNullBinding, before.qvectorSharedNullBinding));
            CHECK(state.sharedNullReferenceCommitted && state.secondQListReferenceCommitted);
            CHECK(state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
            CHECK(state.private40 == before.private40 && state.private48 == before.private48);
            CHECK(state.private50 == before.private50 && state.private58 == before.private58);
            CHECK(state.private78 == before.private78 && state.private80 == before.private80);
            CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
            CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
            CHECK(state.private78 == std::optional<std::uint64_t>{0} && state.private80 == std::optional<std::uint64_t>{0});
            CHECK(state.nativeAllocationBytes == before.nativeAllocationBytes && state.addressPointRva == before.addressPointRva);
            CHECK(state.addressPointWritten == before.addressPointWritten);
            CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
            CHECK(pending->nativeAllocationBytes == 0xc0 && pending->kind == doe::PendingTaskPrototype::Kind::Report);
            CHECK(list->referenceDelta == listBefore.referenceDelta && string->referenceDelta == stringDelta);
            CHECK(vector->referenceDelta == vectorBefore.referenceDelta && vector->qualifiedSymbolRva == vectorBefore.qualifiedSymbolRva);
            CHECK(list->observedAbsoluteCount == listBefore.observedAbsoluteCount && list->observedAtDelta == listBefore.observedAtDelta);
            CHECK(list->observationQualification == listBefore.observationQualification);
            CHECK(vector->observedAbsoluteCount == vectorBefore.observedAbsoluteCount && vector->observedAtDelta == vectorBefore.observedAtDelta);
            CHECK(vector->observationQualification == vectorBefore.observationQualification);
            CHECK(!frame.complete && !pending->qobjectBaseCompleted);
            CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(factory.construction().pendingReportAllocation == construction.pendingReportAllocation);
            CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
            CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
        }
        CHECK(state.private08 == std::optional<std::uint64_t>{0});
    }
}

static_assert(std::is_same_v<decltype(doe::PendingTaskPrototype::SparseQObjectPrivate::private08),
    std::optional<std::uint64_t>>);
TEST_CASE(doe_task_factory_cp179_fresh_private08_is_unknown)
{
    doe::PendingTaskPrototype::SparseQObjectPrivate state;
    CHECK(!state.private08.has_value());
}

TEST_CASE(doe_task_factory_cp179_sparse_predecessor_needs_no_absolute_counts_or_prior_deltas)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    auto store = std::make_shared<doe::TaskAllocationStore>();
    auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP179 sparse supplied predecessor");
    store->associateWidgetRuntime(widgets);
    doe::TaskFactoryRuntime factory(store, widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
    CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
    const auto string = std::const_pointer_cast<engine::WidgetRuntimeState::QStringSharedNullState>(widgets->qstringSharedNull());
    const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
    // Supply a retained CP178 frame independently of symbolic reference history.
    frame.phase = Phase::BeforePrivateZero08; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    state.private08.reset();
    state.private10.reset(); state.private24.reset(); state.private38.reset();
    state.private64.reset(); state.private60.reset();
    list->referenceDelta = string->referenceDelta = vector->referenceDelta = 0;
    const auto identity = frame.privateIdentity();
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe")}) {
        CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
        CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink && frame.privateIdentity() == identity);
        CHECK(state.private08 == std::optional<std::uint64_t>{0});
        CHECK(state.private80 == std::optional<std::uint64_t>{0});
        CHECK(state.private78 == std::optional<std::uint64_t>{0});
        CHECK(list->referenceDelta == 0 && string->referenceDelta == 0 && vector->referenceDelta == 0);
        CHECK(!list->observedAbsoluteCount && !vector->observedAbsoluteCount);
        CHECK(state.sharedNullBinding.lock() == list && state.secondQListSharedNullBinding.lock() == list);
        CHECK(state.qstringSharedNullBinding.lock() == string && state.qvectorSharedNullBinding.lock() == vector);
        CHECK(state.sharedNullReferenceCommitted && state.secondQListReferenceCommitted);
        CHECK(state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
        CHECK(store->retainedPrototypeAt(0) == pending && store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}


TEST_CASE(doe_task_factory_cp179_refuses_corrupt_equal_branch_frame_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, UncommittedSecond, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString, Missing80, WrongAllocationIdentity, Bad40, Missing48, Bad50, Missing58, Bad80, Missing78, Bad78, MissingVersion, WrongVersion, WrongModule, WrongLoad, WrongCaller, Observed08Zero, Observed08Nonzero };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::UncommittedSecond, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString, Damage::Missing80, Damage::WrongAllocationIdentity, Damage::Bad40, Damage::Missing48, Damage::Bad50, Damage::Missing58, Damage::Bad80, Damage::Missing78, Damage::Bad78, Damage::MissingVersion, Damage::WrongVersion, Damage::WrongModule, Damage::WrongLoad, Damage::WrongCaller, Damage::Observed08Zero, Damage::Observed08Nonzero};
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP179 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP179 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        frame.phase = Phase::BeforePrivateZero08; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = true; frame.privateState->private78 = std::uint64_t{0}; frame.privateState->private80 = std::uint64_t{0};
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::Observed08Zero: state.private08 = std::uint64_t{0}; break;
        case Damage::Observed08Nonzero: state.private08 = std::uint64_t{1}; break;
        case Damage::MissingVersion: frame.privateInitializerVersion.reset(); break;
        case Damage::WrongVersion: frame.privateInitializerVersion->argument = 0x40803; break;
        case Damage::WrongModule: frame.privateInitializerVersion->moduleSha256 = "foreign QtCore"; break;
        case Damage::WrongLoad: frame.privateInitializerVersion->argumentLoadRva = 0x131410; break;
        case Damage::WrongCaller: frame.privateInitializerVersion->callRva = 0x131418; break;
        case Damage::Missing80: state.private80.reset(); break;
        case Damage::Bad80: state.private80 = std::uint64_t{1}; break;
        case Damage::Missing78: state.private78.reset(); break;
        case Damage::Bad78: state.private78 = std::uint64_t{1}; break;
        case Damage::WrongAllocationIdentity: frame.allocatedPrivateIdentity = nullptr; break;
        case Damage::Bad40: state.private40 = std::uint64_t{1}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::Bad50: state.private50 = std::uint64_t{1}; break;
        case Damage::Missing58: state.private58.reset(); break;
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::UncommittedSecond: state.secondQListReferenceCommitted = false; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto before08 = frame.privateState->private08;
        const auto before78 = frame.privateState->private78;
        const auto before80 = frame.privateState->private80;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto beforeVersion = frame.privateInitializerVersion;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto weakList = frame.privateState->sharedNullBinding;
        const auto weakString = frame.privateState->qstringSharedNullBinding;
        const auto weakVector = frame.privateState->qvectorSharedNullBinding;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto vectorAt = vector->observedAtDelta;
        const auto vectorObservation = vector->observationQualification;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : "private+0x08 zero requires qualified CP178"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateState->private08 == before08);
        CHECK(frame.privateState->private78 == before78);
        CHECK(frame.privateState->private80 == before80);
        CHECK(frame.privateInitializerVersion.has_value() == beforeVersion.has_value());
        if (beforeVersion) {
            CHECK(frame.privateInitializerVersion->moduleSha256 == beforeVersion->moduleSha256);
            CHECK(frame.privateInitializerVersion->argument == beforeVersion->argument);
            CHECK(frame.privateInitializerVersion->argumentLoadRva == beforeVersion->argumentLoadRva);
            CHECK(frame.privateInitializerVersion->callRva == beforeVersion->callRva);
        }
        CHECK(!weakList.owner_before(frame.privateState->sharedNullBinding) && !frame.privateState->sharedNullBinding.owner_before(weakList));
        CHECK(!weakString.owner_before(frame.privateState->qstringSharedNullBinding) && !frame.privateState->qstringSharedNullBinding.owner_before(weakString));
        CHECK(!weakVector.owner_before(frame.privateState->qvectorSharedNullBinding) && !frame.privateState->qvectorSharedNullBinding.owner_before(weakVector));
        CHECK(vector->observedAtDelta == vectorAt && vector->observationQualification == vectorObservation);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        (void)privateIdentity; (void)allocationIdentity;
    }
}


TEST_CASE(doe_task_factory_cp179_rejects_unpaired_root_at_retained_predecessor)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (int ownerDamage = 0; ownerDamage != 4; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP179 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP179 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        frame.phase = Phase::BeforePrivateZero08; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        state.secondQListReferenceCommitted = true;
        state.private78 = state.private80 = std::uint64_t{0};
        const auto identity = frame.privateIdentity();
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "private+0x08 zero requires qualified CP178"));
        CHECK(frame.phase == Phase::BeforePrivateZero08 && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.secondQListReferenceCommitted && state.private80 == std::optional<std::uint64_t>{0});
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}

TEST_CASE(doe_task_factory_cp179_session_private08_preserves_state_and_reentry)
{
    using Prefix = doe::PendingTaskPrototype::QObjectPrefix;
    using Phase = Prefix::Phase;
    for (const bool observed : {false, true}) {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        REQUIRE(frame.privateInitializerVersion);
        CHECK(frame.privateInitializerVersion->moduleSha256 == Prefix::moduleSha256);
        CHECK(frame.privateInitializerVersion->argument == 0x40804);
        CHECK(frame.privateInitializerVersion->argumentLoadRva == 0x13140f);
        CHECK(frame.privateInitializerVersion->callRva == 0x131417);
        CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
        // Supply the accepted CP178 predecessor, with independent symbolic
        // history and optional fixture observations, to isolate the decision.
        frame.phase = Phase::BeforePrivateZero08; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        frame.privateState->private08.reset();
        frame.privateState->private10.reset(); frame.privateState->private24.reset();
        frame.privateState->private38.reset(); frame.privateState->private64.reset(); frame.privateState->private60.reset();
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = std::const_pointer_cast<engine::WidgetRuntimeState::QStringSharedNullState>(widgets->qstringSharedNull());
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        list->referenceDelta = string->referenceDelta = vector->referenceDelta = observed ? 0xffffffffu : 0u;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 17u;
            list->observationQualification = "isolated CP179 list fixture";
            vector->observedAbsoluteCount = 31u; vector->observedAtDelta = 9u;
            vector->observationQualification = "isolated CP179 vector fixture";
        }
        CHECK(!state.private08);
        const auto before = state;
        const auto version = *frame.privateInitializerVersion;
        const auto listBefore = *list;
        const auto vectorBefore = *vector;
        const auto stringDelta = string->referenceDelta;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        auto sameWeak = [](const auto& a, const auto& b) {
            return a.lock() == b.lock() && !a.owner_before(b) && !b.owner_before(a);
        };
        // First call executes private+0x08, CP180/181 stores and CP182
        // symbolic flags. Later same/different keys preserve the stop.
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "RVA 0x131425"));
            CHECK(state.private08 == std::optional<std::uint64_t>{0});
            CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
            CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
            CHECK(frame.allocatedPrivateIdentity == identity);
            REQUIRE(frame.privateInitializerVersion);
            CHECK(frame.privateInitializerVersion->moduleSha256 == version.moduleSha256);
            CHECK(frame.privateInitializerVersion->argument == version.argument);
            CHECK(frame.privateInitializerVersion->argumentLoadRva == version.argumentLoadRva);
            CHECK(frame.privateInitializerVersion->callRva == version.callRva);
            CHECK(sameWeak(state.sharedNullBinding, before.sharedNullBinding));
            CHECK(sameWeak(state.secondQListSharedNullBinding, before.secondQListSharedNullBinding));
            CHECK(sameWeak(state.qstringSharedNullBinding, before.qstringSharedNullBinding));
            CHECK(sameWeak(state.qvectorSharedNullBinding, before.qvectorSharedNullBinding));
            CHECK(state.sharedNullReferenceCommitted && state.secondQListReferenceCommitted);
            CHECK(state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
            CHECK(state.private40 == before.private40 && state.private48 == before.private48);
            CHECK(state.private50 == before.private50 && state.private58 == before.private58);
            CHECK(state.private78 == before.private78 && state.private80 == before.private80);
            CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
            CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
            CHECK(state.private78 == std::optional<std::uint64_t>{0} && state.private80 == std::optional<std::uint64_t>{0});
            CHECK(state.nativeAllocationBytes == before.nativeAllocationBytes && state.addressPointRva == before.addressPointRva);
            CHECK(state.addressPointWritten == before.addressPointWritten);
            CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
            CHECK(pending->nativeAllocationBytes == 0xc0 && pending->kind == doe::PendingTaskPrototype::Kind::Report);
            CHECK(list->referenceDelta == listBefore.referenceDelta && string->referenceDelta == stringDelta);
            CHECK(vector->referenceDelta == vectorBefore.referenceDelta && vector->qualifiedSymbolRva == vectorBefore.qualifiedSymbolRva);
            CHECK(list->observedAbsoluteCount == listBefore.observedAbsoluteCount && list->observedAtDelta == listBefore.observedAtDelta);
            CHECK(list->observationQualification == listBefore.observationQualification);
            CHECK(vector->observedAbsoluteCount == vectorBefore.observedAbsoluteCount && vector->observedAtDelta == vectorBefore.observedAtDelta);
            CHECK(vector->observationQualification == vectorBefore.observationQualification);
            CHECK(!frame.complete && !pending->qobjectBaseCompleted);
            CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(factory.construction().pendingReportAllocation == construction.pendingReportAllocation);
            CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
            CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
        }
        CHECK(state.private08 == std::optional<std::uint64_t>{0});
    }
}

TEST_CASE(doe_task_factory_cp180_refuses_corrupt_owner_and_predecessor_at_every_prefix_without_mutation)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    enum class Damage { MissingVector, UncommittedVector, Missing40, Bad48, Missing50, Bad58,
        WrongTaskAddress, WrongPrivateAddress, WrongPrivateSize, WrongTaskSize, ReplacedPrivate,
        MissingList, ForeignList, AliasForeignList, MissingQString, ForeignQString, AliasForeignQString,
        UncommittedList, UncommittedQString, WrongTaskAddressWritten, WrongPrivateAddressWritten, WrongKind, ExpiredRoot,
        Complete, BaseComplete, TaskBacklink, PrivateBacklink, WrongPhase, MalformedRva, ForeignVector, AliasForeignVector, WrongVectorOwner, ExpiredVector, WrongListOwner, WrongQStringOwner, MissingSecondBinding, UncommittedSecond, ExpiredSecondBinding, ForeignSecondBinding, AliasForeignSecondBinding, WrongSecondOwner, ExpiredList, ExpiredQString, Missing80, WrongAllocationIdentity, Bad40, Missing48, Bad50, Missing58, Bad80, Missing78, Bad78, MissingVersion, WrongVersion, WrongModule, WrongLoad, WrongCaller, Missing08, Bad08 };
    constexpr Damage damages[] = {Damage::MissingVector, Damage::UncommittedVector, Damage::Missing40,
        Damage::Bad48, Damage::Missing50, Damage::Bad58, Damage::WrongTaskAddress, Damage::WrongPrivateAddress,
        Damage::WrongPrivateSize, Damage::WrongTaskSize, Damage::ReplacedPrivate, Damage::MissingList,
        Damage::ForeignList, Damage::AliasForeignList, Damage::MissingQString, Damage::ForeignQString,
        Damage::AliasForeignQString, Damage::UncommittedList, Damage::UncommittedQString,
        Damage::WrongTaskAddressWritten, Damage::WrongPrivateAddressWritten, Damage::WrongKind, Damage::ExpiredRoot,
        Damage::Complete, Damage::BaseComplete, Damage::TaskBacklink, Damage::PrivateBacklink,
        Damage::WrongPhase, Damage::MalformedRva, Damage::ForeignVector, Damage::AliasForeignVector,
        Damage::WrongVectorOwner, Damage::ExpiredVector, Damage::WrongListOwner, Damage::WrongQStringOwner, Damage::MissingSecondBinding, Damage::UncommittedSecond, Damage::ExpiredSecondBinding, Damage::ForeignSecondBinding, Damage::AliasForeignSecondBinding, Damage::WrongSecondOwner, Damage::ExpiredList, Damage::ExpiredQString, Damage::Missing80, Damage::WrongAllocationIdentity, Damage::Bad40, Damage::Missing48, Damage::Bad50, Damage::Missing58, Damage::Bad80, Damage::Missing78, Damage::Bad78, Damage::MissingVersion, Damage::WrongVersion, Damage::WrongModule, Damage::WrongLoad, Damage::WrongCaller, Damage::Missing08, Damage::Bad08};
    for (std::size_t ordinal = 0; ordinal != 11; ++ordinal)
    for (const auto damage : damages) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP180 refusal matrix root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP180 refusal matrix foreign");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        if (ordinal >= 6) doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, ordinal - 6);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            ordinal < 6 ? "RVA 0x131425" : cp182Stops[ordinal - 6]));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto privateIdentity = frame.privateIdentity();
        const auto allocationIdentity = frame.allocatedPrivateIdentity;
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        if (ordinal < 6) cp180SetPrefix(frame, ordinal);
        state.private08 = std::uint64_t{0};
        frame.privateState->secondQListSharedNullBinding = list; frame.privateState->secondQListReferenceCommitted = true; frame.privateState->private78 = std::uint64_t{0}; frame.privateState->private80 = std::uint64_t{0};
        state.qvectorSharedNullBinding = vector; state.qvectorSharedNullReferenceCommitted = true;
        switch (damage) {
        case Damage::Missing08: state.private08.reset(); break;
        case Damage::Bad08: state.private08 = std::uint64_t{1}; break;
        case Damage::MissingVersion: frame.privateInitializerVersion.reset(); break;
        case Damage::WrongVersion: frame.privateInitializerVersion->argument = 0x40803; break;
        case Damage::WrongModule: frame.privateInitializerVersion->moduleSha256 = "foreign QtCore"; break;
        case Damage::WrongLoad: frame.privateInitializerVersion->argumentLoadRva = 0x131410; break;
        case Damage::WrongCaller: frame.privateInitializerVersion->callRva = 0x131418; break;
        case Damage::Missing80: state.private80.reset(); break;
        case Damage::Bad80: state.private80 = std::uint64_t{1}; break;
        case Damage::Missing78: state.private78.reset(); break;
        case Damage::Bad78: state.private78 = std::uint64_t{1}; break;
        case Damage::WrongAllocationIdentity: frame.allocatedPrivateIdentity = nullptr; break;
        case Damage::Bad40: state.private40 = std::uint64_t{1}; break;
        case Damage::Missing48: state.private48.reset(); break;
        case Damage::Bad50: state.private50 = std::uint64_t{1}; break;
        case Damage::Missing58: state.private58.reset(); break;
        case Damage::MissingSecondBinding: state.secondQListSharedNullBinding.reset(); break;
        case Damage::UncommittedSecond: state.secondQListReferenceCommitted = false; break;
        case Damage::ExpiredSecondBinding: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.secondQListSharedNullBinding = expired;
            break;
        }
        case Damage::ForeignSecondBinding: state.secondQListSharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignSecondBinding: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::WrongSecondOwner: state.secondQListSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::ExpiredList: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QListSharedNullState>();
            state.sharedNullBinding = expired;
            break;
        }
        case Damage::ExpiredQString: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QStringSharedNullState>();
            state.qstringSharedNullBinding = expired;
            break;
        }
        case Damage::MissingVector: state.qvectorSharedNullBinding.reset(); break;
        case Damage::UncommittedVector: state.qvectorSharedNullReferenceCommitted = false; break;
        case Damage::Missing40: state.private40.reset(); break;
        case Damage::Bad48: state.private48 = std::uint64_t{0x48}; break;
        case Damage::Missing50: state.private50.reset(); break;
        case Damage::Bad58: state.private58 = std::uint64_t{0x58}; break;
        case Damage::WrongTaskAddress: frame.taskAddressPointRva = 0x23cac0; break;
        case Damage::WrongPrivateAddress: state.addressPointRva = 0x1a9a30; break;
        case Damage::WrongPrivateSize: state.nativeAllocationBytes = 0x80; break;
        case Damage::WrongTaskSize: pending->nativeAllocationBytes = 0xb8; break;
        case Damage::ReplacedPrivate: frame.privateState = std::make_unique<doe::PendingTaskPrototype::SparseQObjectPrivate>(state); break;
        case Damage::MissingList: state.sharedNullBinding.reset(); break;
        case Damage::ForeignList: state.sharedNullBinding = foreign->qlistSharedNull(); break;
        case Damage::AliasForeignList: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(widgets, foreign->qlistSharedNull().get()); break;
        case Damage::MissingQString: state.qstringSharedNullBinding.reset(); break;
        case Damage::ForeignQString: state.qstringSharedNullBinding = foreign->qstringSharedNull(); break;
        case Damage::AliasForeignQString: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(widgets, foreign->qstringSharedNull().get()); break;
        case Damage::UncommittedList: state.sharedNullReferenceCommitted = false; break;
        case Damage::UncommittedQString: state.qstringSharedNullReferenceCommitted = false; break;
        case Damage::WrongTaskAddressWritten: frame.taskAddressPointWritten = false; break;
        case Damage::WrongPrivateAddressWritten: state.addressPointWritten = false; break;
        case Damage::WrongKind: pending->kind = static_cast<doe::PendingTaskPrototype::Kind>(99); break;
        case Damage::ExpiredRoot: widgets.reset(); break;
        case Damage::Complete: frame.complete = true; break;
        case Damage::BaseComplete: pending->qobjectBaseCompleted = true; break;
        case Damage::TaskBacklink: frame.taskPrivateBacklinkAvailable = true; break;
        case Damage::PrivateBacklink: frame.privateTaskBacklinkAvailable = true; break;
        case Damage::WrongPhase: frame.phase = Phase::BeforePrivateZero58; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); break;
        case Damage::MalformedRva: vector->qualifiedSymbolRva = 0x2f0bb9; break;
        case Damage::ForeignVector: state.qvectorSharedNullBinding = foreign->qvectorSharedNull(); break;
        case Damage::AliasForeignVector: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(widgets, foreign->qvectorSharedNull().get()); break;
        case Damage::WrongVectorOwner: state.qvectorSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(foreign, vector.get()); break;
        case Damage::ExpiredVector: {
            auto expired = std::make_shared<engine::WidgetRuntimeState::QVectorSharedNullState>();
            state.qvectorSharedNullBinding = expired;
            break;
        }
        case Damage::WrongListOwner: state.sharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(foreign, list.get()); break;
        case Damage::WrongQStringOwner: state.qstringSharedNullBinding = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(foreign, string.get()); break;
        }
        const auto* testedPrivateIdentity = frame.privateIdentity();
        const auto* testedAllocationIdentity = frame.allocatedPrivateIdentity;
        const auto before10 = frame.privateState->private10;
        const auto before24 = frame.privateState->private24;
        const auto before38 = frame.privateState->private38;
        const auto before64 = frame.privateState->private64;
        const auto before60 = frame.privateState->private60;
        const auto before08 = frame.privateState->private08;
        const auto before78 = frame.privateState->private78;
        const auto before80 = frame.privateState->private80;
        const auto beforeSecond = frame.privateState->secondQListSharedNullBinding;
        const auto beforeSecondCommit = frame.privateState->secondQListReferenceCommitted;
        const auto beforePhase = frame.phase;
        const auto beforeVersion = frame.privateInitializerVersion;
        const auto before40 = frame.privateState->private40, before48 = frame.privateState->private48;
        const auto before50 = frame.privateState->private50, before58 = frame.privateState->private58;
        const auto weakList = frame.privateState->sharedNullBinding;
        const auto weakString = frame.privateState->qstringSharedNullBinding;
        const auto weakVector = frame.privateState->qvectorSharedNullBinding;
        const auto beforeList = frame.privateState->sharedNullBinding.lock();
        const auto beforeString = frame.privateState->qstringSharedNullBinding.lock();
        const auto beforeVector = frame.privateState->qvectorSharedNullBinding.lock();
        const auto beforeListCommit = frame.privateState->sharedNullReferenceCommitted;
        const auto beforeStringCommit = frame.privateState->qstringSharedNullReferenceCommitted;
        const auto beforeVectorCommit = frame.privateState->qvectorSharedNullReferenceCommitted;
        const auto beforeComplete = frame.complete, beforeBaseComplete = pending->qobjectBaseCompleted;
        const auto beforeTaskBacklink = frame.taskPrivateBacklinkAvailable;
        const auto beforePrivateBacklink = frame.privateTaskBacklinkAvailable;
        const auto beforeTaskAddress = frame.taskAddressPointRva;
        const auto beforeTaskSize = pending->nativeAllocationBytes;
        const auto beforePrivateAddress = frame.privateState->addressPointRva;
        const auto beforePrivateSize = frame.privateState->nativeAllocationBytes;
        const auto beforeTaskAddressWritten = frame.taskAddressPointWritten;
        const auto beforePrivateAddressWritten = frame.privateState->addressPointWritten;
        const auto beforeKind = pending->kind;
        const auto vectorDelta = vector->referenceDelta;
        const auto listDelta = list->referenceDelta, stringDelta = string->referenceDelta;
        const auto foreignVectorDelta = foreign->qvectorSharedNull()->referenceDelta;
        const auto foreignListDelta = foreign->qlistSharedNull()->referenceDelta;
        const auto foreignStringDelta = foreign->qstringSharedNull()->referenceDelta;
        const auto vectorCount = vector->observedAbsoluteCount;
        const auto vectorAt = vector->observedAtDelta;
        const auto vectorObservation = vector->observationQualification;
        const auto listCount = list->observedAbsoluteCount;
        const auto listAt = list->observedAtDelta;
        const auto listQualification = list->observationQualification;
        const auto vectorQualification = vector->qualifiedSymbolRva;
        const auto prototypeCount = store->retainedPrototypeCount();
        const auto taskCount = store->retainedTaskCount();
        const auto constructionBefore = factory.construction();
        const Cp180Snapshot fullBefore(*pending, *store, factory, widgets ? *widgets : *foreign);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); },
            damage == Damage::TaskBacklink ? "CP190 requires phase-qualified task+8 publication" : damage == Damage::WrongPhase ? "CP169 state" : ordinal >= 6 ? "CP182 requires" : ordinal == 5 ? "CP181 requires qualified CP180 retained prefix" : "CP180 requires qualified CP179 retained prefix"));
        CHECK(!frame.privateState->secondQListSharedNullBinding.owner_before(beforeSecond) &&
            !beforeSecond.owner_before(frame.privateState->secondQListSharedNullBinding));
        CHECK(frame.privateState->secondQListSharedNullBinding.lock() == beforeSecond.lock());
        CHECK(frame.privateState->secondQListReferenceCommitted == beforeSecondCommit);
        CHECK(frame.privateState->private10 == before10 && frame.privateState->private24 == before24);
        CHECK(frame.privateState->private38 == before38 && frame.privateState->private64 == before64);
        CHECK(frame.privateState->private60 == before60);
        CHECK(frame.privateState->private08 == before08);
        CHECK(frame.privateState->private78 == before78);
        CHECK(frame.privateState->private80 == before80);
        CHECK(frame.privateInitializerVersion.has_value() == beforeVersion.has_value());
        if (beforeVersion) {
            CHECK(frame.privateInitializerVersion->moduleSha256 == beforeVersion->moduleSha256);
            CHECK(frame.privateInitializerVersion->argument == beforeVersion->argument);
            CHECK(frame.privateInitializerVersion->argumentLoadRva == beforeVersion->argumentLoadRva);
            CHECK(frame.privateInitializerVersion->callRva == beforeVersion->callRva);
        }
        CHECK(!weakList.owner_before(frame.privateState->sharedNullBinding) && !frame.privateState->sharedNullBinding.owner_before(weakList));
        CHECK(!weakString.owner_before(frame.privateState->qstringSharedNullBinding) && !frame.privateState->qstringSharedNullBinding.owner_before(weakString));
        CHECK(!weakVector.owner_before(frame.privateState->qvectorSharedNullBinding) && !frame.privateState->qvectorSharedNullBinding.owner_before(weakVector));
        CHECK(vector->observedAtDelta == vectorAt && vector->observationQualification == vectorObservation);
        CHECK(frame.privateIdentity() == testedPrivateIdentity && frame.allocatedPrivateIdentity == testedAllocationIdentity);
        CHECK(frame.phase == beforePhase && frame.privateState->private40 == before40 && frame.privateState->private48 == before48);
        CHECK(frame.privateState->private50 == before50 && frame.privateState->private58 == before58);
        CHECK(frame.privateState->sharedNullBinding.lock() == beforeList && frame.privateState->qstringSharedNullBinding.lock() == beforeString);
        CHECK(frame.privateState->qvectorSharedNullBinding.lock() == beforeVector);
        const auto afterList = frame.privateState->sharedNullBinding.lock();
        const auto afterString = frame.privateState->qstringSharedNullBinding.lock();
        const auto afterVector = frame.privateState->qvectorSharedNullBinding.lock();
        CHECK(!afterList.owner_before(beforeList) && !beforeList.owner_before(afterList));
        CHECK(!afterString.owner_before(beforeString) && !beforeString.owner_before(afterString));
        CHECK(!afterVector.owner_before(beforeVector) && !beforeVector.owner_before(afterVector));
        CHECK(frame.privateState->sharedNullReferenceCommitted == beforeListCommit && frame.privateState->qstringSharedNullReferenceCommitted == beforeStringCommit);
        CHECK(frame.privateState->qvectorSharedNullReferenceCommitted == beforeVectorCommit);
        CHECK(vector->referenceDelta == vectorDelta && vector->observedAbsoluteCount == vectorCount);
        CHECK(list->referenceDelta == listDelta && string->referenceDelta == stringDelta);
        CHECK(foreign->qvectorSharedNull()->referenceDelta == foreignVectorDelta);
        CHECK(foreign->qlistSharedNull()->referenceDelta == foreignListDelta);
        CHECK(foreign->qstringSharedNull()->referenceDelta == foreignStringDelta);
        CHECK(vector->observedAtDelta == std::nullopt && list->observedAbsoluteCount == listCount);
        CHECK(vector->qualifiedSymbolRva == vectorQualification);
        CHECK(list->observedAtDelta == listAt);
        CHECK(list->observationQualification == listQualification);
        CHECK(frame.complete == beforeComplete && pending->qobjectBaseCompleted == beforeBaseComplete);
        CHECK(frame.taskPrivateBacklinkAvailable == beforeTaskBacklink && frame.privateTaskBacklinkAvailable == beforePrivateBacklink);
        CHECK(frame.taskAddressPointRva == beforeTaskAddress && pending->nativeAllocationBytes == beforeTaskSize);
        CHECK(frame.taskAddressPointWritten == beforeTaskAddressWritten && frame.privateState->addressPointWritten == beforePrivateAddressWritten);
        CHECK(pending->kind == beforeKind);
        CHECK(frame.privateState->addressPointRva == beforePrivateAddress && frame.privateState->nativeAllocationBytes == beforePrivateSize);
        CHECK(store->retainedPrototypeCount() == prototypeCount && store->retainedTaskCount() == taskCount);
        const auto constructionAfter = factory.construction();
        CHECK(constructionAfter.metadataCacheCount == constructionBefore.metadataCacheCount && constructionAfter.taskCacheCount == constructionBefore.taskCacheCount);
        fullBefore.check();
        (void)privateIdentity; (void)allocationIdentity;
    }
}



TEST_CASE(doe_task_factory_cp180_session_all_prefixes_resume_and_reentry_preserve_state)
{
    using Prefix = doe::PendingTaskPrototype::QObjectPrefix;
    using Phase = Prefix::Phase;
    for (std::size_t stop = 1; stop != 6; ++stop)
    for (int history = 0; history != 3; ++history) {
        const bool observed = history != 0;
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        REQUIRE(store && widgets && store->ownsWidgetRuntime(widgets));
        const auto factoryOwner = session.taskFactoryRuntimeOwner();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factoryOwner, 11);
        REQUIRE(factoryOwner);
        auto& factory = *factoryOwner;
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        REQUIRE(frame.privateInitializerVersion);
        CHECK(frame.privateInitializerVersion->moduleSha256 == Prefix::moduleSha256);
        CHECK(frame.privateInitializerVersion->argument == 0x40804);
        CHECK(frame.privateInitializerVersion->argumentLoadRva == 0x13140f);
        CHECK(frame.privateInitializerVersion->callRva == 0x131417);
        CHECK(frame.phase == Phase::BeforeTaskPrivateBacklink);
        const auto list = std::const_pointer_cast<engine::WidgetRuntimeState::QListSharedNullState>(widgets->qlistSharedNull());
        const auto string = std::const_pointer_cast<engine::WidgetRuntimeState::QStringSharedNullState>(widgets->qstringSharedNull());
        const auto vector = std::const_pointer_cast<engine::WidgetRuntimeState::QVectorSharedNullState>(widgets->qvectorSharedNull());
        list->referenceDelta = history == 0 ? 0u : history == 1 ? 17u : 0xffffffffu;
        string->referenceDelta = history == 0 ? 0u : history == 1 ? 23u : 0xfffffffeu;
        vector->referenceDelta = history == 0 ? 0u : history == 1 ? 31u : 0xfffffffdu;
        if (observed) {
            list->observedAbsoluteCount = 42u; list->observedAtDelta = 17u;
            list->observationQualification = "isolated CP180 list fixture";
            vector->observedAbsoluteCount = 31u; vector->observedAtDelta = 9u;
            vector->observationQualification = "isolated CP180 vector fixture";
        }
        const Cp180Snapshot uninterrupted(*pending, *store, factory, *widgets);
        // Retain the accepted CP179 entry with the same private allocation.
        cp180SetPrefix(frame, 0);
        CHECK(state.private08 == std::optional<std::uint64_t>{0});
        const auto before = state;
        const auto version = *frame.privateInitializerVersion;
        const auto listBefore = *list;
        const auto vectorBefore = *vector;
        const auto stringDelta = string->referenceDelta;
        const auto identity = frame.privateIdentity();
        const auto construction = factory.construction();
        auto sameWeak = [](const auto& a, const auto& b) {
            return a.lock() == b.lock() && !a.owner_before(b) && !b.owner_before(a);
        };
        // The stop ordinal is absolute: unchanged targets refuse reentry;
        // explicitly raising it permits the remaining prefix to execute.
        for (const auto destination : {stop, std::size_t{5}}) {
            doe::detail::TaskFactoryTestAccess::stopAt(factory, destination);
            std::optional<Cp180Snapshot> stopped;
            for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
                CHECK(refuses([&] { factory.createTaskBase(key); }, cp180Stops[destination]));
                cp180CheckPrefix(frame, destination);
                if (stopped) stopped->check();
                else stopped.emplace(*pending, *store, factory, *widgets);
                CHECK(state.private08 == std::optional<std::uint64_t>{0});
                CHECK(store->retainedPrototypeAt(0) == pending && frame.privateIdentity() == identity);
                CHECK(frame.allocatedPrivateIdentity == identity);
                REQUIRE(frame.privateInitializerVersion);
                CHECK(frame.privateInitializerVersion->moduleSha256 == version.moduleSha256);
                CHECK(frame.privateInitializerVersion->argument == version.argument);
                CHECK(frame.privateInitializerVersion->argumentLoadRva == version.argumentLoadRva);
                CHECK(frame.privateInitializerVersion->callRva == version.callRva);
                CHECK(sameWeak(state.sharedNullBinding, before.sharedNullBinding));
                CHECK(sameWeak(state.secondQListSharedNullBinding, before.secondQListSharedNullBinding));
                CHECK(sameWeak(state.qstringSharedNullBinding, before.qstringSharedNullBinding));
                CHECK(sameWeak(state.qvectorSharedNullBinding, before.qvectorSharedNullBinding));
                CHECK(state.sharedNullReferenceCommitted && state.secondQListReferenceCommitted);
                CHECK(state.qstringSharedNullReferenceCommitted && state.qvectorSharedNullReferenceCommitted);
                CHECK(state.private40 == before.private40 && state.private48 == before.private48);
                CHECK(state.private50 == before.private50 && state.private58 == before.private58);
                CHECK(state.private78 == before.private78 && state.private80 == before.private80);
                CHECK(state.private40 == std::optional<std::uint64_t>{0} && state.private48 == std::optional<std::uint64_t>{0});
                CHECK(state.private50 == std::optional<std::uint64_t>{0} && state.private58 == std::optional<std::uint64_t>{0});
                CHECK(state.private78 == std::optional<std::uint64_t>{0} && state.private80 == std::optional<std::uint64_t>{0});
                CHECK(state.nativeAllocationBytes == before.nativeAllocationBytes && state.addressPointRva == before.addressPointRva);
                CHECK(state.addressPointWritten == before.addressPointWritten);
                CHECK(frame.taskAddressPointWritten && frame.taskAddressPointRva == 0x23cac8);
                CHECK(pending->nativeAllocationBytes == 0xc0 && pending->kind == doe::PendingTaskPrototype::Kind::Report);
                CHECK(list->referenceDelta == listBefore.referenceDelta && string->referenceDelta == stringDelta);
                CHECK(vector->referenceDelta == vectorBefore.referenceDelta && vector->qualifiedSymbolRva == vectorBefore.qualifiedSymbolRva);
                CHECK(list->observedAbsoluteCount == listBefore.observedAbsoluteCount && list->observedAtDelta == listBefore.observedAtDelta);
                CHECK(list->observationQualification == listBefore.observationQualification);
                CHECK(vector->observedAbsoluteCount == vectorBefore.observedAbsoluteCount && vector->observedAtDelta == vectorBefore.observedAtDelta);
                CHECK(vector->observationQualification == vectorBefore.observationQualification);
                CHECK(!frame.complete && !pending->qobjectBaseCompleted);
                CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
                CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
                CHECK(factory.construction().pendingReportAllocation == construction.pendingReportAllocation);
                CHECK(factory.construction().metadataCacheCount == construction.metadataCacheCount);
                CHECK(factory.construction().taskCacheCount == construction.taskCacheCount);
            }
        }
        cp180CheckPrefix(frame, 5);
        doe::detail::TaskFactoryTestAccess::stopAt(factory, 6);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        uninterrupted.check();
        CHECK(state.private08 == std::optional<std::uint64_t>{0});
    }
}


TEST_CASE(doe_task_factory_cp180_rejects_unpaired_roots_at_every_retained_prefix)
{
    using Phase = doe::PendingTaskPrototype::QObjectPrefix::Phase;
    for (std::size_t ordinal = 0; ordinal != 11; ++ordinal)
    for (int ownerDamage = 0; ownerDamage != 5; ++ownerDamage) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP180 unpaired root");
        auto foreign = std::make_shared<engine::WidgetRuntimeState>(true, "CP180 foreign owner");
        if (ownerDamage == 1) store->associateWidgetRuntime(foreign);
        if (ownerDamage == 2) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(widgets, foreign.get()));
        if (ownerDamage == 3) store->associateWidgetRuntime(std::shared_ptr<engine::WidgetRuntimeState>(foreign, widgets.get()));
        if (ownerDamage == 4) {
            auto expired = std::make_shared<engine::WidgetRuntimeState>(true, "CP180 expired association");
            store->associateWidgetRuntime(expired);
        }
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 11);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, "matching root"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        // Supply all predecessor observations independently of the rejected owner.
        cp180SetPrefix(frame, ordinal < 6 ? ordinal : 5);
        if (ordinal >= 6) {
            using Flags = doe::PendingTaskPrototype::PrivateFlags20Expression;
            const auto cut = ordinal - 6;
            state.private28 = std::uint64_t{0};
            frame.phase = cp182Phases[cut];
            const auto read = Flags::read(frame.allocatedPrivateIdentity);
            if (cut != 0) frame.flags20Eax = cut == 1 ? read : cut == 2 ? read.afterAnd() : read.afterAnd().afterOr();
            if (cut == 4) state.private20 = *frame.flags20Eax;
        }
        state.private08 = std::uint64_t{0};
        state.addressPointRva = 0x1a9d10;
        state.sharedNullBinding = widgets->qlistSharedNull(); state.sharedNullReferenceCommitted = true;
        state.qstringSharedNullBinding = widgets->qstringSharedNull(); state.qstringSharedNullReferenceCommitted = true;
        state.qvectorSharedNullBinding = widgets->qvectorSharedNull(); state.qvectorSharedNullReferenceCommitted = true;
        state.private40 = state.private48 = state.private50 = state.private58 = std::uint64_t{0};
        state.secondQListSharedNullBinding = widgets->qlistSharedNull();
        state.secondQListReferenceCommitted = true;
        state.private78 = state.private80 = std::uint64_t{0};
        const auto identity = frame.privateIdentity();
        const Cp180Snapshot before(*pending, *store, factory, *widgets);
        CHECK(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, ordinal >= 6
            ? "CP182 requires qualified CP181 retained prefix" : ordinal == 5
            ? "CP181 requires qualified CP180 retained prefix" : "CP180 requires qualified CP179 retained prefix"));
        before.check();
        CHECK(frame.phase == (ordinal < 6 ? cp180Phases[ordinal] : cp182Phases[ordinal - 6]) && frame.privateIdentity() == identity);
        CHECK(frame.allocatedPrivateIdentity == identity && state.secondQListSharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.secondQListReferenceCommitted && state.private80 == std::optional<std::uint64_t>{0});
        CHECK(state.sharedNullBinding.lock() == widgets->qlistSharedNull());
        CHECK(state.qstringSharedNullBinding.lock() == widgets->qstringSharedNull());
        CHECK(state.qvectorSharedNullBinding.lock() == widgets->qvectorSharedNull());
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        CHECK(foreign->qlistSharedNull()->referenceDelta == 0 && foreign->qvectorSharedNull()->referenceDelta == 0);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory.construction().metadataCacheCount == 0 && factory.construction().taskCacheCount == 0);
        CHECK(!frame.complete && !pending->qobjectBaseCompleted && !frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    }
}

using Cp180Private = doe::PendingTaskPrototype::SparseQObjectPrivate;
static_assert(std::is_same_v<decltype(Cp180Private::private10), std::optional<std::uint64_t>>);
static_assert(std::is_same_v<decltype(Cp180Private::private24), std::optional<std::uint32_t>>);
static_assert(std::is_same_v<decltype(Cp180Private::private38), std::optional<std::uint64_t>>);
static_assert(std::is_same_v<decltype(Cp180Private::private64), std::optional<std::uint32_t>>);
static_assert(std::is_same_v<decltype(Cp180Private::private60), std::optional<std::uint32_t>>);
static_assert(noexcept(std::declval<std::optional<std::uint64_t>&>() = std::uint64_t{0}));
static_assert(noexcept(std::declval<std::optional<std::uint32_t>&>() = std::uint32_t{0}));
template<class T> concept Cp180HasPrivate28 = requires(T value) { value.private28; };
template<class T> concept Cp180HasPrivate20 = requires(T value) { value.private20; };
static_assert(Cp180HasPrivate28<Cp180Private> && Cp180HasPrivate20<Cp180Private>);
static_assert(std::is_same_v<decltype(Cp180Private::private28), std::optional<std::uint64_t>>);

TEST_CASE(doe_task_factory_cp180_fields_are_fresh_unknown_and_exact_native_widths)
{
    Cp180Private state;
    CHECK(!state.private10 && !state.private24 && !state.private38 && !state.private64 && !state.private60 && !state.private28);
    state.private10 = std::uint64_t{0x100000001ull};
    state.private38 = std::uint64_t{0x200000002ull};
    state.private24 = state.private64 = state.private60 = std::uint32_t{0xffffffffu};
    CHECK(*state.private10 == 0x100000001ull && *state.private38 == 0x200000002ull);
    CHECK(*state.private24 == 0xffffffffu && *state.private64 == 0xffffffffu && *state.private60 == 0xffffffffu);
}

TEST_CASE(doe_task_factory_cp180_sparse_refusals_preserve_every_current_prefix)
{
    for (std::size_t ordinal = 0; ordinal != 5; ++ordinal)
    for (std::size_t field = 0; field != 5; ++field)
    for (const bool nonzero : {false, true}) {
        const auto store = std::make_shared<doe::TaskAllocationStore>();
        const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "standalone retained-prefix corruption");
        store->associateWidgetRuntime(widgets);
        const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        REQUIRE(store && widgets && factory);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        cp180SetPrefix(frame, ordinal);
        auto& state = *frame.privateState;
        auto corrupt = [=](auto& value) {
            if (field < ordinal && !nonzero) value.reset();
            else value = nonzero ? 1 : 0;
        };
        switch (field) {
        case 0: corrupt(state.private10); break;
        case 1: corrupt(state.private24); break;
        case 2: corrupt(state.private38); break;
        case 3: corrupt(state.private64); break;
        case 4: corrupt(state.private60); break;
        }
        const Cp180Snapshot before(*pending, *store, *factory, *widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory->createTaskBase(key); }, "CP180 requires exact sparse store prefix"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp180_cursor_null_private_and_report_ordinal_refuse_without_mutation)
{
    for (std::size_t ordinal = 0; ordinal != 5; ++ordinal)
    for (int damage = 0; damage != 4; ++damage) {
        const auto store = std::make_shared<doe::TaskAllocationStore>();
        const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "standalone retained-prefix corruption");
        store->associateWidgetRuntime(widgets);
        const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        REQUIRE(store && widgets && factory);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        cp180SetPrefix(frame, ordinal);
        if (damage == 0) { frame.phase = cp180Phases[(ordinal + 1) % 5]; frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); }
        if (damage == 1) frame.privateState.reset();
        if (damage == 2) doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*factory, store->retainedPrototypeCount());
        if (damage == 3) {
            frame.phase = static_cast<Cp180Phase>(0xff);
            frame.privateState->private28.reset(); frame.privateState->private20.reset(); frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        }
        const Cp180Snapshot before(*pending, *store, *factory, *widgets);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); },
            damage == 0 ? "CP180 requires exact sparse store prefix" :
            damage == 1 ? "CP180 requires qualified CP179 retained prefix" :
            damage == 2 ? "pending Task diagnostic index is unavailable" : "prefix stops before"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp180_invalid_internal_stop_refuses_before_any_mutation)
{
    for (const auto invalid : {std::size_t{0}, std::size_t{7}, static_cast<std::size_t>(-1)}) {
        const auto store = std::make_shared<doe::TaskAllocationStore>();
        const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "standalone retained-prefix corruption");
        store->associateWidgetRuntime(widgets);
        const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        REQUIRE(store && widgets && factory);
        const auto initial = constructionValues(*factory);
        doe::detail::TaskFactoryTestAccess::stopAt(*factory, invalid);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "CP180 invalid internal stop ordinal"));
        CHECK(constructionValues(*factory) == initial);
        CHECK(store->retainedPrototypeCount() == 0 && store->retainedTaskCount() == 0);
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0);
        CHECK(widgets->qstringSharedNull()->referenceDelta == 0 && widgets->qvectorSharedNull()->referenceDelta == 0);
        doe::detail::TaskFactoryTestAccess::stopAt(*factory, 2);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, cp180Stops[2]));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        cp180CheckPrefix(*pending->qobjectPrefix, 2);
        doe::detail::TaskFactoryTestAccess::stopAt(*factory, invalid);
        const Cp180Snapshot before(*pending, *store, *factory, *widgets);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "CP180 invalid internal stop ordinal"));
        before.check();
    }
}


TEST_CASE(doe_task_factory_cp181_sparse_predecessor_and_premature_observation_refuse_without_mutation)
{
    for (int damage = 0; damage != 14; ++damage) {
        const auto store = std::make_shared<doe::TaskAllocationStore>();
        const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "standalone CP181 prefix");
        store->associateWidgetRuntime(widgets);
        const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        REQUIRE(store && widgets && factory);
        doe::detail::TaskFactoryTestAccess::stopAt(*factory, 5);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, cp180Stops[5]));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        auto& frame = *pending->qobjectPrefix;
        auto& state = *frame.privateState;
        const auto corrupt = [damage](auto& field) {
            if (damage % 2 == 0) field.reset(); else field = 1;
        };
        switch (damage / 2) {
        case 0: corrupt(state.private10); break;
        case 1: corrupt(state.private24); break;
        case 2: corrupt(state.private38); break;
        case 3: corrupt(state.private64); break;
        case 4: corrupt(state.private60); break;
        case 5: state.private28 = std::uint64_t{damage % 2 == 0 ? 0u : 1u}; break;
        case 6: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*factory, store->retainedPrototypeCount()); break;
        }
        doe::detail::TaskFactoryTestAccess::stopAt(*factory, 6);
        const Cp180Snapshot before(*pending, *store, *factory, *widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { factory->createTaskBase(key); }, damage < 10
                ? "CP180 requires exact sparse store prefix" : damage < 12
                ? "CP181 requires unobserved private+0x28" : "pending Task diagnostic index is unavailable"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp181_session_single_qword_store_and_reentry)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto store = session.taskAllocationStoreOwner();
    const auto widgets = session.widgetRuntimeOwner();
    const auto factory = session.taskFactoryRuntimeOwner();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    REQUIRE(store && widgets && factory && store->ownsWidgetRuntime(widgets));
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 0);
    doe::detail::TaskFactoryTestAccess::stopAt(*factory, 5);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, cp180Stops[5]));
    auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    auto& frame = *pending->qobjectPrefix;
    auto& state = *frame.privateState;
    cp180CheckPrefix(frame, 5);
    CHECK(!state.private28);
    const Cp180Snapshot predecessor(*pending, *store, *factory, *widgets);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, cp180Stops[5]));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::stopAt(*factory, 6);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131224"));
    CHECK(state.private28 == std::optional<std::uint64_t>{0});
    CHECK(frame.phase == Cp180Phase::BeforePrivateFlags20Read);
    // Undo only the two admitted observations to compare every prior field.
    state.private28.reset(); frame.phase = Cp180Phase::BeforePrivateZero28;
    predecessor.check();
    state.private28 = std::uint64_t{0}; frame.phase = Cp180Phase::BeforePrivateFlags20Read;
    const Cp180Snapshot stopped(*pending, *store, *factory, *widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { factory->createTaskBase(key); }, "RVA 0x131224"));
        stopped.check();
    }
}



namespace {
using Cp182Flags = doe::PendingTaskPrototype::PrivateFlags20Expression;
static_assert(std::is_trivially_copyable_v<Cp182Flags>);
static_assert(std::is_same_v<decltype(Cp180Private::private20), std::optional<Cp182Flags>>);
static_assert(std::is_same_v<decltype(doe::PendingTaskPrototype::QObjectPrefix::flags20Eax), std::optional<Cp182Flags>>);
static_assert(std::is_same_v<decltype(Cp182Flags::knownOne), std::uint32_t>);
static_assert(std::is_same_v<decltype(Cp182Flags::knownZero), std::uint32_t>);
static_assert(noexcept(std::declval<std::optional<Cp182Flags>&>() = std::declval<const Cp182Flags&>()));
static_assert(noexcept(std::declval<Cp182Flags>().afterAnd()));
static_assert(noexcept(std::declval<Cp182Flags>().afterOr()));

struct Cp182Fixture {
    std::shared_ptr<doe::TaskAllocationStore> store = std::make_shared<doe::TaskAllocationStore>();
    std::shared_ptr<engine::WidgetRuntimeState> widgets =
        std::make_shared<engine::WidgetRuntimeState>(true, "standalone symbolic parent fixture");
    std::shared_ptr<doe::TaskFactoryRuntime> factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
    doe::PendingTaskPrototype* pending = nullptr;
    explicit Cp182Fixture(std::size_t stop)
    {
        store->associateWidgetRuntime(widgets);
        REQUIRE(store && widgets && factory && store->ownsWidgetRuntime(widgets));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, stop);
        REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, cp182Stops[stop]));
        pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    }
    auto& frame() { return *pending->qobjectPrefix; }
    void checkCut(std::size_t ordinal)
    {
        auto& f = frame();
        auto& state = *f.privateState;
        CHECK(f.phase == cp182Phases[ordinal]);
        CHECK(f.privateIdentity() == f.allocatedPrivateIdentity);
        const auto read = Cp182Flags::read(f.allocatedPrivateIdentity);
        const auto expected = ordinal < 2 ? read : ordinal == 2 ? read.afterAnd() : read.afterAnd().afterOr();
        CHECK(f.flags20Eax == (ordinal == 0 ? std::nullopt : std::optional<Cp182Flags>{expected}));
        CHECK(state.private20 == (ordinal == 4 ? std::optional<Cp182Flags>{expected} : std::nullopt));
        if (f.flags20Eax) {
            CHECK(f.flags20Eax->knownOne == (ordinal >= 3 ? 0x60u : 0u));
            CHECK(f.flags20Eax->knownZero == (ordinal >= 2 ? 0x30fu : 0u));
            CHECK(f.flags20Eax->unknownMask() == (ordinal >= 3 ? 0xfffffc90u : ordinal == 2 ? 0xfffffcf0u : 0xffffffffu));
            CHECK(f.flags20Eax->sourceKnownOne == 0 && f.flags20Eax->sourceKnownZero == 0);
            CHECK(f.flags20Eax->offset == 0x20 && f.flags20Eax->width == 32 && f.flags20Eax->readRva == 0x131224);
        }
        CHECK(state.private24 == std::optional<std::uint32_t>{0});
        CHECK(state.private28 == std::optional<std::uint64_t>{0});
        CHECK(!f.complete && !pending->qobjectBaseCompleted);
        CHECK(!f.taskPrivateBacklinkAvailable && !f.privateTaskBacklinkAvailable);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(factory->construction().metadataCacheCount == 0 && factory->construction().taskCacheCount == 0);
        CHECK(widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(widgets->qstringSharedNull()->referenceDelta == 1 && widgets->qvectorSharedNull()->referenceDelta == 1);
    }
};
} // namespace

TEST_CASE(doe_task_factory_cp182_default_session_retains_symbolic_expression_and_only_admitted_effects)
{
    Cp182Fixture fixture(0);
    fixture.checkCut(0);
    auto& frame = fixture.frame();
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 4);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, cp182Stops[4]));
    fixture.checkCut(4);
    const auto expression = *frame.flags20Eax;
    frame.phase = cp182Phases[0];
    frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); frame.privateState->private20.reset();
    before.check();
    frame.phase = cp182Phases[4]; frame.flags20Eax = expression; frame.privateState->private20 = expression;
    const Cp180Snapshot after(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, cp182Stops[4]));
        after.check();
    }
    // A default factory also takes CP189 caller jump to the public store boundary.
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto factory = session.taskFactoryRuntimeOwner();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
    REQUIRE(factory);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    const auto* pending = session.taskAllocationStoreOwner()->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
    CHECK(pending->qobjectPrefix->phase == Cp180Phase::BeforeTaskPrivateBacklink);
    REQUIRE(pending->qobjectPrefix->flags20Eax && pending->qobjectPrefix->privateState->private20);
    CHECK(*pending->qobjectPrefix->flags20Eax == *pending->qobjectPrefix->privateState->private20);
    CHECK(pending->qobjectPrefix->flags20Eax->privateIdentity == pending->qobjectPrefix->allocatedPrivateIdentity);
}

TEST_CASE(doe_task_factory_cp182_each_instruction_stop_reentry_and_resume_matches_uninterrupted)
{
    for (std::size_t stop = 0; stop != 5; ++stop) {
        Cp182Fixture fixture(4);
        const Cp180Snapshot uninterrupted(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        auto& frame = fixture.frame();
        const auto identity = frame.allocatedPrivateIdentity;
        frame.phase = cp182Phases[0]; frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset(); frame.privateState->private20.reset();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, cp182Stops[stop]));
        fixture.checkCut(stop);
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        const auto readIdentity = frame.flags20Eax;
        for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, cp182Stops[stop]));
            stopped.check();
            CHECK(frame.flags20Eax == readIdentity);
        }
        for (std::size_t next = stop + 1; next < 5; ++next) {
            doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, next);
            CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, cp182Stops[next]));
            fixture.checkCut(next);
            REQUIRE(frame.flags20Eax);
            CHECK(frame.flags20Eax->privateIdentity == identity && frame.flags20Eax->readRva == 0x131224);
        }
        uninterrupted.check();
    }
}

TEST_CASE(doe_task_factory_cp182_known_bit_transforms_preserve_unknown_input_dependency)
{
    Cp180Private state;
    CHECK(!state.private20 && !state.private24);
    const auto read = Cp182Flags::read(&state);
    const auto masked = read.afterAnd();
    const auto final = masked.afterOr();
    CHECK(read.unknownMask() == 0xffffffffu && read.knownOne == 0 && read.knownZero == 0);
    CHECK(masked.knownOne == 0 && masked.knownZero == 0x30fu && masked.unknownMask() == 0xfffffcf0u);
    CHECK(final.knownOne == 0x60u && final.knownZero == 0x30fu && final.unknownMask() == 0xfffffc90u);
    CHECK(final.privateIdentity == read.privateIdentity && final.readRva == read.readRva);
    CHECK(final.stage == Cp182Flags::Stage::Or && read.stage == Cp182Flags::Stage::Read);
    // Pure test-injected knowledge is never supplied to production allocator state.
    for (const auto input : {0u, 0xffffffffu, 0xa5a5a5a5u, 0x12345678u}) {
        const auto inputRead = Cp182Flags::read(&state, input, ~input);
        const auto inputAnd = inputRead.afterAnd();
        const auto output = inputAnd.afterOr();
        const auto numeric = (input & 0xfffffcf0u) | 0x60u;
        CHECK(inputAnd.knownOne == (input & 0xfffffcf0u));
        CHECK(inputAnd.knownZero == (~input | 0x30fu));
        CHECK(output.knownOne == numeric && output.knownZero == ~numeric && output.unknownMask() == 0);
        CHECK(output.sourceKnownOne == input && output.sourceKnownZero == ~input);
        CHECK(output.privateIdentity == &state && output.readRva == 0x131224);
    }
    const auto partlyKnown = Cp182Flags::read(&state, 0x80000001u, 0x40000002u);
    const auto result = partlyKnown.afterAnd().afterOr();
    CHECK(result.knownOne == ((0x80000001u & 0xfffffcf0u) | 0x60u));
    CHECK(result.knownZero == ((0x40000002u | 0x30fu) & ~0x60u));
    CHECK((result.knownOne & result.knownZero) == 0);
    CHECK(result.unknownMask() == ~(result.knownOne | result.knownZero));
    CHECK(!state.private20);
    state.private24 = 0;
    CHECK(!state.private20 && Cp182Flags::read(&state).unknownMask() == 0xffffffffu);
}

TEST_CASE(doe_task_factory_cp182_symbolic_state_and_cursor_corruption_refuse_without_mutation)
{
    for (std::size_t stop = 0; stop != 5; ++stop)
    for (int damage = 0; damage != 18; ++damage) {
        Cp182Fixture fixture(stop);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        Cp180Private foreign;
        // Forge one component of the current symbolic prefix, including absent
        // slots at entry and premature memory writes before the fourth step.
        auto expression = frame.flags20Eax.value_or(Cp182Flags::read(frame.allocatedPrivateIdentity));
        switch (damage) {
        case 0: expression.privateIdentity = &foreign; frame.flags20Eax = expression; break;
        case 1: expression.offset = 0x24; frame.flags20Eax = expression; break;
        case 2: expression.width = 64; frame.flags20Eax = expression; break;
        case 3: expression.readRva = 0x131227; frame.flags20Eax = expression; break;
        case 4: expression.stage = static_cast<Cp182Flags::Stage>(0xff); frame.flags20Eax = expression; break;
        case 5: expression.knownOne ^= 1u; frame.flags20Eax = expression; break;
        case 6: expression.knownZero ^= 1u; frame.flags20Eax = expression; break;
        case 7: expression.sourceKnownOne = 0x80000000u; frame.flags20Eax = expression; break;
        case 8: expression.sourceKnownZero = 0x80000000u; frame.flags20Eax = expression; break;
        case 9:
            if (stop == 0) frame.flags20Eax = expression; else frame.flags20Eax.reset(); frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
            break;
        case 10:
            if (stop == 4) state.private20.reset(); else state.private20 = expression;
            break;
        case 11:
            expression.privateIdentity = &foreign; state.private20 = expression;
            break;
        case 12: frame.phase = static_cast<Cp180Phase>(0xff); break;
        case 13: frame.phase = cp182Phases[(stop + 2) % 5]; break;
        case 14: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, fixture.store->retainedPrototypeCount()); break;
        case 15: expression.knownOne = expression.knownZero = 1u; frame.flags20Eax = expression; break;
        case 16: expression.sourceKnownOne = expression.sourceKnownZero = 1u; frame.flags20Eax = expression; break;
        case 17:
            expression.stage = stop > 1 ? Cp182Flags::Stage::Read : Cp182Flags::Stage::Or;
            frame.flags20Eax = expression; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, damage == 14
                ? "pending Task diagnostic index is unavailable" : damage == 12
                ? "prefix stops before" : "CP182 requires exact symbolic flags prefix"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp182_every_sparse_predecessor_missing_or_nonzero_refuses_at_each_cursor)
{
    for (std::size_t stop = 0; stop != 5; ++stop)
    for (std::size_t field = 0; field != 13; ++field)
    for (const bool nonzero : {false, true}) {
        Cp182Fixture fixture(stop);
        auto& state = *fixture.frame().privateState;
        const auto corrupt = [nonzero](auto& value) { if (nonzero) value = 1; else value.reset(); };
        switch (field) {
        case 0: corrupt(state.private40); break;
        case 1: corrupt(state.private48); break;
        case 2: corrupt(state.private50); break;
        case 3: corrupt(state.private58); break;
        case 4: corrupt(state.private78); break;
        case 5: corrupt(state.private80); break;
        case 6: corrupt(state.private08); break;
        case 7: corrupt(state.private10); break;
        case 8: corrupt(state.private24); break;
        case 9: corrupt(state.private38); break;
        case 10: corrupt(state.private64); break;
        case 11: corrupt(state.private60); break;
        case 12: corrupt(state.private28); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP182 requires exact sparse zero prefix"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp182_failed_later_prefix_keeps_earlier_steps_and_can_resume)
{
    for (std::size_t stop = 1; stop != 4; ++stop) {
        Cp182Fixture fixture(stop);
        auto& frame = fixture.frame();
        const auto symbol = *frame.flags20Eax;
        const auto prior24 = frame.privateState->private24;
        frame.privateState->private24.reset();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 4);
        const Cp180Snapshot damaged(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 requires exact sparse zero prefix"));
        damaged.check();
        CHECK(frame.phase == cp182Phases[stop] && frame.flags20Eax == std::optional<Cp182Flags>{symbol});
        CHECK(!frame.privateState->private20);
        frame.privateState->private24 = prior24;
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, cp182Stops[4]));
        fixture.checkCut(4);
        CHECK(frame.flags20Eax->privateIdentity == symbol.privateIdentity && frame.flags20Eax->readRva == symbol.readRva);
    }
}

TEST_CASE(doe_task_factory_cp182_invalid_stop_refuses_before_allocation_and_at_retained_prefix)
{
    // CP193 admits ordinal 15; 16 is the first invalid absolute stop.
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        const auto store = std::make_shared<doe::TaskAllocationStore>();
        const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "standalone retained-prefix corruption");
        store->associateWidgetRuntime(widgets);
        const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 11);
        REQUIRE(store && widgets && factory);
        const auto construction = constructionValues(*factory);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, invalid);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
        CHECK(constructionValues(*factory) == construction && store->retainedPrototypeCount() == 0);
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
        CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 2);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, cp182Stops[2]));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, invalid);
        const Cp180Snapshot before(*pending, *store, *factory, *widgets);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "CP182 invalid internal stop ordinal"));
        before.check();
    }
}


TEST_CASE(doe_task_factory_cp182_null_private_refuses_without_mutation_at_every_cursor)
{
    for (std::size_t stop = 0; stop != 5; ++stop) {
        Cp182Fixture fixture(stop);
        fixture.frame().privateState.reset();
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP182 requires qualified CP181 retained prefix"));
            before.check();
        }
    }
}


TEST_CASE(doe_task_factory_cp183_copies_only_qualified_private_identity_and_stops_before_epilogue)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(frame.returnPrivateIdentity == nullptr);
    const auto identity = frame.allocatedPrivateIdentity;
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131235"));
    CHECK(frame.phase == Cp180Phase::BeforePrivateEpilogue);
    CHECK(frame.returnPrivateIdentity == identity && frame.returnPrivateIdentity == frame.privateIdentity());
    // Remove exactly the two admitted effects to compare the complete CP182
    // frame, sparse fields, roots, references, cache/count and owner state.
    frame.phase = Cp180Phase::BeforePrivateReturnSetup;
    frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    predecessor.check();
    fixture.checkCut(4);
    frame.phase = Cp180Phase::BeforePrivateEpilogue;
    frame.returnPrivateIdentity = identity;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131235"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp183_refuses_corrupted_predecessor_without_any_publication)
{
    for (int damage = 0; damage != 19; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        Cp180Private foreign;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
        switch (damage) {
        case 0: frame.returnPrivateIdentity = &foreign; break;
        case 1: frame.returnPrivateIdentity = frame.allocatedPrivateIdentity; break;
        case 2: frame.allocatedPrivateIdentity = &foreign; break;
        case 3: frame.flags20Eax.reset(); break;
        case 4: state.private20.reset(); break;
        case 5: frame.flags20Eax->privateIdentity = &foreign; break;
        case 6: state.private20->privateIdentity = &foreign; break;
        case 7: frame.flags20Eax->stage = Cp182Flags::Stage::Read; break;
        case 8: state.private20->sourceKnownOne = 1; break;
        case 9: state.private28 = 1; break;
        case 10: frame.privateInitializerVersion->argument ^= 1; break;
        case 11: state.secondQListSharedNullBinding.reset(); break;
        case 12: state.qvectorSharedNullReferenceCommitted = false; break;
        case 13: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 14: frame.taskPrivateBacklinkAvailable = true; break;
        case 15: frame.phase = static_cast<Cp180Phase>(0xff); break;
        case 16: frame.privateState.reset(); break;
        case 17: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, 1); break;
        case 18: frame.privateInitializerVersion->callRva ^= 1; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp183_missing_or_foreign_terminal_identity_refuses_without_replay)
{
    for (int damage = 0; damage != 2; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131235"));
        Cp180Private foreign;
        frame.returnPrivateIdentity = damage == 0 ? nullptr : &foreign;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP183 requires allocation-bound return identity"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp183_earlier_diagnostic_rewinds_clear_return_slot_and_resume)
{
    for (std::size_t stop = 0; stop != 5; ++stop) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131235"));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        frame.phase = cp182Phases[0];
        frame.flags20Eax.reset(); frame.privateState->private20.reset();
        frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, cp182Stops[stop]));
        fixture.checkCut(stop);
        CHECK(frame.returnPrivateIdentity == nullptr);
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, cp182Stops[stop]));
        stopped.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131235"));
        terminal.check();
    }
}


TEST_CASE(doe_task_factory_cp184_restores_only_saved_task_rbx_before_stack_adjustment)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 5);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131235"));
    REQUIRE(frame.savedTaskRbx);
    CHECK(frame.savedTaskRbx->taskIdentity == fixture.pending);
    CHECK(frame.savedTaskRbx->privateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.savedTaskRbx->rspFromEntryS == -0x38);
    CHECK(frame.savedTaskRbx->saveSlotFromEntryS == 0x10 && frame.savedTaskRbx->saveSlotFromRsp == 0x48);
    CHECK(!frame.restoredTaskRbxIdentity);
    const auto privateIdentity = frame.returnPrivateIdentity;
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123a"));
    CHECK(frame.phase == Cp180Phase::BeforePrivateStackAdjustment);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(frame.returnPrivateIdentity == privateIdentity && privateIdentity == frame.allocatedPrivateIdentity);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    // Exactly the result and cursor differ from the full predecessor snapshot.
    frame.phase = Cp180Phase::BeforePrivateEpilogue;
    frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    predecessor.check();
    frame.phase = Cp180Phase::BeforePrivateStackAdjustment;
    frame.restoredTaskRbxIdentity = fixture.pending;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13123a"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp184_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 34; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 6 : 5);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13123a" : "RVA 0x131235"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP184 foreign");
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = terminal ? nullptr : fixture.pending; break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp184_rewinds_clear_restored_result_and_preserve_symbolic_stack)
{
    for (std::size_t stop = 0; stop != 6; ++stop) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123a"));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        frame.phase = cp182Phases[0];
        frame.flags20Eax.reset(); frame.privateState->private20.reset();
        frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); },
            stop == 5 ? "RVA 0x131235" : cp182Stops[stop]));
        CHECK(!frame.restoredTaskRbxIdentity);
        if (stop < 5) fixture.checkCut(stop);
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            stop == 5 ? "RVA 0x131235" : cp182Stops[stop]));
        stopped.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123a"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp184_earlier_rewind_never_repairs_corrupted_saved_stack_origin)
{
    Cp182Fixture fixture(0);
    auto& frame = fixture.frame();
    frame.savedTaskRbx->saveSlotFromEntryS = 0x18;
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP184 requires qualified saved RBX"));
    before.check();
}

TEST_CASE(doe_task_factory_cp185_adjusts_only_symbolic_rsp_and_stops_before_rdi_restore)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123a"));
    REQUIRE(frame.phase == Cp180Phase::BeforePrivateStackAdjustment);
    REQUIRE(frame.rspFromEntryS == -0x38);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123e"));
    CHECK(frame.phase == Cp180Phase::BeforePrivateRdiRestore && frame.rspFromEntryS == -8);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(frame.returnPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.savedRdiSlotFromEntryS == -8 && frame.returnSlotFromEntryS == 0);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    // Restoring only these two fields compares every represented retained
    // identity, owner/control block, private expression, allocation and map.
    frame.phase = Cp180Phase::BeforePrivateStackAdjustment;
    frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    predecessor.check();
    frame.phase = Cp180Phase::BeforePrivateRdiRestore;
    frame.rspFromEntryS = -8;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13123e"));
        terminal.check();
    }
    // Lowering a diagnostic budget never replays ADD or rolls back a terminal.
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 6);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123e"));
    terminal.check();
    // Explicit rewind to CP184 retains its RBX result; the budget stops before ADD.
    frame.phase = Cp180Phase::BeforePrivateStackAdjustment;
    frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123a"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123e"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp185_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 40; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 7 : 6);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13123e" : "RVA 0x13123a"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP185 foreign");
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = terminal ? -0x38 : -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}


TEST_CASE(doe_task_factory_cp185_earlier_rewinds_preserve_budgets_and_resume_once)
{
    for (std::size_t stop = 0; stop != 7; ++stop) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123e"));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        frame.phase = cp182Phases[0];
        frame.flags20Eax.reset(); frame.privateState->private20.reset();
        frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset();
        frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        const auto boundary = stop < 5 ? cp182Stops[stop] : stop == 5 ? "RVA 0x131235" : "RVA 0x13123a";
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, boundary));
        CHECK(frame.rspFromEntryS == -0x38);
        CHECK(frame.restoredTaskRbxIdentity == (stop == 6 ? fixture.pending : nullptr));
        if (stop < 5) fixture.checkCut(stop);
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, boundary));
        stopped.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123e"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp186_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 57; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 8 : 7);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13123f" : "RVA 0x13123e"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP186 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = terminal ? -8 : 0; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = terminal ? nullptr : &*frame.savedEntryRdi; break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}



TEST_CASE(doe_task_factory_cp186_pop_changes_only_rdi_rsp_and_cursor_before_ret)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123e"));
    REQUIRE(frame.savedEntryRdi && !frame.restoredRdiToken);
    REQUIRE(frame.rspFromEntryS == -8);
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123f"));
    CHECK(frame.phase == Cp180Phase::BeforePrivateReturn && frame.rspFromEntryS == 0);
    CHECK(frame.restoredRdiToken == &*frame.savedEntryRdi);
    CHECK(frame.returnPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 9; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13123f"));
            terminal.check();
        }
    }
    // Undo exactly the POP effects: every other represented field must agree.
    frame.phase = Cp180Phase::BeforePrivateRdiRestore;
    frame.rspFromEntryS = -8;
    frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset();
    before.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 7);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123e"));
    before.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13123f"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp186_all_earlier_flags_rewinds_resume_and_forbid_premature_rdi)
{
    for (std::size_t stop = 0; stop != 8; ++stop) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123f"));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        frame.phase = cp182Phases[0];
        frame.flags20Eax.reset(); frame.privateState->private20.reset();
        frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr;
        frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        const auto boundary = stop < 5 ? cp182Stops[stop] : stop == 5 ? "RVA 0x131235" : stop == 6 ? "RVA 0x13123a" : "RVA 0x13123e";
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, boundary));
        CHECK(!frame.restoredRdiToken);
        frame.restoredRdiToken = &*frame.savedEntryRdi;
        const Cp180Snapshot damaged(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "premature restored RDI"));
        damaged.check();
        frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123f"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp186_premature_token_is_forbidden_at_every_earlier_phase)
{
    for (int phase = 0; phase < static_cast<int>(Cp180Phase::BeforePrivateReturn); ++phase) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        frame.phase = static_cast<Cp180Phase>(phase);
        frame.returnPrivateIdentity = nullptr;
        frame.restoredTaskRbxIdentity = nullptr;
        frame.restoredRdiToken = &*frame.savedEntryRdi;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "premature restored RDI"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp186_rewind_never_repairs_saved_rdi_provenance)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123f"));
    frame.savedEntryRdi->incomingMoveRva ^= 1;
    frame.phase = Cp180Phase::BeforePrivateRdiRestore;
    frame.rspFromEntryS = -8;
    frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset();
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "qualified saved entry RDI"));
    before.check();
    cp180SetPrefix(frame, 0);
    const Cp180Snapshot earlier(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "qualified saved entry RDI"));
    earlier.check();
}

TEST_CASE(doe_task_factory_cp186_admitted_cuts_reject_wrong_saved_rdi_allocation_origin)
{
    for (const std::size_t stop : {std::size_t{7}, std::size_t{8}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            stop == 7 ? "RVA 0x13123e" : "RVA 0x13123f"));
        frame.savedEntryRdi->privateIdentity = nullptr;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "qualified saved entry RDI token"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp187_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 70; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 9 : 8);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13141c" : "RVA 0x13123f"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP187 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = terminal ? 0 : 8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = terminal ? Cp180Phase::BeforePrivateReturn : Cp180Phase::BeforeCallerXor; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}




TEST_CASE(doe_task_factory_cp187_ret_changes_only_rsp_and_cursor_before_caller_xor)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13123f"));
    REQUIRE(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x13123f});
    REQUIRE(frame.returnToCaller && frame.returnToCaller->targetRva == 0x13141c);
    REQUIRE(frame.rspFromEntryS == 0 && frame.restoredRdiToken == &*frame.savedEntryRdi);
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13141c"));
    CHECK(frame.phase == Cp180Phase::BeforeCallerXor && frame.rspFromEntryS == 8);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x13141c});
    CHECK(frame.returnPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(frame.restoredRdiToken == &*frame.savedEntryRdi);
    CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 10; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13141c"));
            terminal.check();
        }
    }
    // Rewind precisely RET's live stack and phase; even consumed [S] persists.
    frame.phase = Cp180Phase::BeforePrivateReturn;
    frame.rspFromEntryS = 0;
    before.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 8);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x13123f"));
    before.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141c"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp187_all_earlier_flags_rewinds_resume_without_repairing_slot)
{
    for (std::size_t stop = 0; stop != 9; ++stop) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141c"));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        frame.phase = cp182Phases[0];
        frame.flags20Eax.reset(); frame.privateState->private20.reset();
        frame.returnPrivateIdentity = nullptr; frame.restoredTaskRbxIdentity = nullptr;
        frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); frame.rspFromEntryS = -0x38; frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
        const auto boundary = stop < 5 ? cp182Stops[stop] : stop == 5 ? "RVA 0x131235" :
            stop == 6 ? "RVA 0x13123a" : stop == 7 ? "RVA 0x13123e" : "RVA 0x13123f";
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, boundary));
        frame.returnToCaller->targetRva ^= 1;
        const Cp180Snapshot damaged(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "qualified symbolic return slot"));
        damaged.check();
        frame.returnToCaller->targetRva ^= 1;
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141c"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp188_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 80; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 10 : 9);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13141e" : "RVA 0x13141c"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 10);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP188 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = terminal ? Cp180Phase::BeforeCallerXor : Cp180Phase::BeforeCallerJump; break;
        case 70: frame.callerRsi = terminal ? 1 : 0; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: if (terminal) frame.callerRsi.reset(); else frame.callerRsi = 0; break;
        case 78: if (terminal) frame.callerArithmeticFlags.reset();
            else frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{}; break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}




TEST_CASE(doe_task_factory_cp188_xor_changes_only_rsi_flags_and_cursor_before_jump)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141c"));
    REQUIRE(frame.phase == Cp180Phase::BeforeCallerXor);
    REQUIRE(!frame.callerRsi && !frame.callerArithmeticFlags);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 10);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x13141e"));
    CHECK(frame.phase == Cp180Phase::BeforeCallerJump);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x13141e});
    CHECK(frame.rspFromEntryS == 8);
    CHECK(frame.callerRsi == std::optional<std::uint64_t>{0});
    REQUIRE(frame.callerArithmeticFlags);
    CHECK(!frame.callerArithmeticFlags->carry && !frame.callerArithmeticFlags->overflow);
    CHECK(frame.callerArithmeticFlags->zero && frame.callerArithmeticFlags->parity);
    CHECK(!frame.callerArithmeticFlags->sign && !frame.callerArithmeticFlags->auxiliary.has_value());
    CHECK(frame.returnPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(frame.restoredRdiToken == &*frame.savedEntryRdi);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Revert only XOR's effects. The full snapshot proves no memory, stack,
    // identity, reference, allocation, cache or return-slot change occurred.
    frame.phase = Cp180Phase::BeforeCallerXor;
    frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    predecessor.check();
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x13141e"));
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 11; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13141e"));
            terminal.check();
        }
    }
    // An explicit validated predecessor rewind restores unavailable RSI/flags.
    frame.phase = Cp180Phase::BeforeCallerXor;
    frame.callerRsi.reset(); frame.callerArithmeticFlags.reset();
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 9);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141c"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 10);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141e"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp189_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 81; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 11 : 10);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x131425" : "RVA 0x13141e"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 11);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP189 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = true; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = Cp180Phase::BeforeCallerXor; break;
        case 70: frame.callerRsi = 1; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: frame.callerRsi.reset(); break;
        case 78: frame.callerArithmeticFlags.reset(); break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        case 80: frame.privateTaskBacklinkAvailable = true; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}





TEST_CASE(doe_task_factory_cp189_jump_changes_only_cursor_and_preserves_all_predecessor_observations)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 10);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13141e"));
    REQUIRE(frame.phase == Cp180Phase::BeforeCallerJump);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 11);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131425"));
    CHECK(frame.phase == Cp180Phase::BeforeTaskPrivateBacklink);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131425});
    CHECK(frame.callerRsi == std::optional<std::uint64_t>{0});
    CHECK(frame.callerArithmeticFlags == std::optional<Cp180Pending::QObjectPrefix::CallerArithmeticFlags>{
        Cp180Pending::QObjectPrefix::CallerArithmeticFlags{}});
    CHECK(frame.rspFromEntryS == 8);
    CHECK(frame.returnPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    // RIP is phase-derived: undo only the jump's phase to compare the full
    // register, memory, ownership, return lineage and reference snapshot.
    frame.phase = Cp180Phase::BeforeCallerJump;
    predecessor.check();
    frame.phase = Cp180Phase::BeforeTaskPrivateBacklink;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 12; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131425"));
            terminal.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp189_all_intermediate_budgets_reenter_then_resume_to_public_store_boundary)
{
    const Cp180Phase phases[] = {Cp180Phase::BeforePrivateFlags20Read,
        Cp180Phase::BeforeFlags20And, Cp180Phase::BeforeFlags20Or,
        Cp180Phase::BeforeFlags20Write, Cp180Phase::BeforePrivateReturnSetup,
        Cp180Phase::BeforePrivateEpilogue, Cp180Phase::BeforePrivateStackAdjustment,
        Cp180Phase::BeforePrivateRdiRestore, Cp180Phase::BeforePrivateReturn,
        Cp180Phase::BeforeCallerXor, Cp180Phase::BeforeCallerJump,
        Cp180Phase::BeforeTaskPrivateBacklink};
    const char* stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
        "RVA 0x13122f", "RVA 0x131232", "RVA 0x131235", "RVA 0x13123a",
        "RVA 0x13123e", "RVA 0x13123f", "RVA 0x13141c", "RVA 0x13141e", "RVA 0x131425"};
    for (std::size_t stop = 0; stop != 12; ++stop) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stops[stop]));
        CHECK(frame.phase == phases[stop]);
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stops[stop]));
            stopped.check();
        }
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 11);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
        CHECK(frame.phase == Cp180Phase::BeforeTaskPrivateBacklink);
        CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131425});
        CHECK(frame.callerRsi == std::optional<std::uint64_t>{0} && frame.rspFromEntryS == 8);
        CHECK(!frame.taskPrivateBacklinkAvailable && !frame.privateTaskBacklinkAvailable);
        CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
        CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
        CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == 1);
        CHECK(fixture.widgets->qvectorSharedNull()->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp190_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 101; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 12 : 11);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x131429" : "RVA 0x131425"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP189 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = !frame.taskPrivateBacklinkAvailable; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = Cp180Phase::BeforeCallerXor; break;
        case 70: frame.callerRsi = 1; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: frame.callerRsi.reset(); break;
        case 78: frame.callerArithmeticFlags.reset(); break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        case 80: frame.privateTaskBacklinkAvailable = true; break;
        case 81: frame.taskPrivateIdentity = terminal ? nullptr : frame.allocatedPrivateIdentity; break;
        case 82: frame.taskPrivateIdentity = &foreignPrivate; break;
        case 83: frame.taskPrivateBacklinkAvailable = false; frame.taskPrivateIdentity = &foreignPrivate; break;
        case 84: frame.taskPrivateBacklinkAvailable = true; frame.taskPrivateIdentity = nullptr; break;
        case 85: frame.phase = terminal ? Cp180Phase::BeforeTaskPrivateBacklink : Cp180Phase::BeforeCallerPrivateCopy; break;
        case 86: frame.complete = true; break;
        case 87: fixture.pending->qobjectBaseCompleted = true; break;
        case 88: state.private08 = 1; break;
        case 89: state.private40.reset(); break;
        case 90: state.nativeAllocationBytes = 0xc0; break;
        case 91: state.sharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 92: state.qstringSharedNullBinding = foreignWidgets->qstringSharedNull(); break;
        case 93: state.qvectorSharedNullBinding = foreignWidgets->qvectorSharedNull(); break;
        case 94: frame.privateInitializerVersion.reset(); break;
        case 95: frame.privateState.reset(); break;
        case 96: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, 99); break;
        case 97: state.private80 = 1; break;
        case 98: frame.savedEntryRdi->taskIdentity = nullptr; break;
        case 99: frame.returnToCaller->privateIdentity = nullptr; break;
        case 100: state.qstringSharedNullReferenceCommitted = false; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}






TEST_CASE(doe_task_factory_cp190_publication_changes_only_task08_and_cursor_and_reentry_never_replays)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 11);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    REQUIRE(frame.phase == Cp180Phase::BeforeTaskPrivateBacklink);
    CHECK(!frame.taskPrivateIdentity && !frame.taskPrivateBacklinkAvailable);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131429"));
    CHECK(frame.phase == Cp180Phase::BeforeCallerPrivateCopy);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131429});
    CHECK(frame.taskPrivateBacklinkAvailable && frame.taskPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.taskPrivateIdentity == frame.returnPrivateIdentity && frame.restoredTaskRbxIdentity == fixture.pending);
    CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
    CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Consistently undo the sole store and derived cursor for full preservation
    // comparison. No R12 or caller-spill observation exists at this cut.
    frame.phase = Cp180Phase::BeforeTaskPrivateBacklink;
    frame.taskPrivateIdentity = nullptr;
    frame.taskPrivateBacklinkAvailable = false;
    predecessor.check();
    frame.taskPrivateIdentity = frame.allocatedPrivateIdentity;
    frame.taskPrivateBacklinkAvailable = true;
    frame.phase = Cp180Phase::BeforeCallerPrivateCopy;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 13; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131429"));
            terminal.check();
        }
    }
    // Explicit consistent rewind preserves predecessor reentry then resumes.
    frame.phase = Cp180Phase::BeforeTaskPrivateBacklink;
    frame.taskPrivateIdentity = nullptr;
    frame.taskPrivateBacklinkAvailable = false;
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 11);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131425"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131429"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp190_pinned_and_all_intermediate_budgets_reach_only_public_store)
{
    // Preserve CP190's historical budget; CP191 retains budget 13 below.
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto factory = session.taskFactoryRuntimeOwner();
    const auto store = session.taskAllocationStoreOwner();
    REQUIRE(factory && store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 12);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131429"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix);
    CHECK(pending->qobjectPrefix->taskPrivateIdentity == pending->qobjectPrefix->allocatedPrivateIdentity);
    const char* stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
        "RVA 0x13122f", "RVA 0x131232", "RVA 0x131235", "RVA 0x13123a",
        "RVA 0x13123e", "RVA 0x13123f", "RVA 0x13141c", "RVA 0x13141e", "RVA 0x131425", "RVA 0x131429"};
    for (std::size_t stop = 0; stop != 13; ++stop) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stops[stop]));
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stops[stop]));
            stopped.check();
        }
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131429"));
        CHECK(frame.phase == Cp180Phase::BeforeCallerPrivateCopy);
        CHECK(frame.taskPrivateIdentity == frame.allocatedPrivateIdentity && frame.taskPrivateBacklinkAvailable);
        CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
        CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
        CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
        CHECK(fixture.factory->construction().metadataCacheCount == 0 && fixture.factory->construction().taskCacheCount == 0);
        CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == 1 && fixture.widgets->qvectorSharedNull()->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp190_premature_publication_refuses_before_any_prefix_mutation)
{
    for (std::size_t stop = 0; stop != 12; ++stop) for (int damage = 0; damage != 3; ++damage) {
        Cp182Fixture fixture(0);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA"));
        auto& frame = fixture.frame();
        frame.taskPrivateBacklinkAvailable = damage != 0;
        frame.taskPrivateIdentity = damage != 1 ? frame.allocatedPrivateIdentity : nullptr;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "CP190 requires phase-qualified task+8 publication"));
        before.check();
    }
    // A prefix earlier than the instruction loop must refuse before shared-null
    // binding or reference mutation as well.
    Cp182Fixture fixture(0);
    auto& frame = fixture.frame();
    frame.phase = Cp180Phase::BeforeFirstSharedQList;
    frame.taskPrivateIdentity = frame.allocatedPrivateIdentity;
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP190 requires phase-qualified task+8 publication"));
    before.check();
}

TEST_CASE(doe_task_factory_cp190_expired_widget_root_refuses_at_predecessor_and_terminal_without_mutation)
{
    for (const auto stop : {std::size_t{11}, std::size_t{12}}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP190 expiring root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, stop);
        REQUIRE(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, stop == 11 ? "RVA 0x131425" : "RVA 0x131429"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        const auto frameBefore = frameValues(*pending);
        const auto stateBefore = privateValues(*pending->qobjectPrefix->privateState);
        const auto constructionBefore = constructionValues(factory);
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        widgets.reset();
        CHECK(!factory.widgetRuntimeOwner());
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 12);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "CP182 requires qualified CP181 retained prefix"));
            CHECK(frameValues(*pending) == frameBefore && privateValues(*pending->qobjectPrefix->privateState) == stateBefore);
            CHECK(constructionValues(factory) == constructionBefore);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(list->referenceDelta == 2 && string->referenceDelta == 1 && vector->referenceDelta == 1);
        }
    }
}


static_assert(std::is_same_v<decltype(Cp180Pending::QObjectPrefix::r12PrivateIdentity),
    const Cp180Private*>);
static_assert(noexcept(std::declval<Cp180Pending::QObjectPrefix&>().r12PrivateIdentity =
    std::declval<const Cp180Private*>()));

TEST_CASE(doe_task_factory_cp191_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 118; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 13 : 12);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x13142c" : "RVA 0x131429"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP191 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        auto aliasList = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
            fixture.widgets->qlistSharedNull().get(), [](auto*) {});
        auto aliasString = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(
            fixture.widgets->qstringSharedNull().get(), [](auto*) {});
        auto aliasVector = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(
            fixture.widgets->qvectorSharedNull().get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expiredOwner;
        { auto temporary = std::make_shared<doe::TaskAllocationStore>(); expiredOwner = temporary; }
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = !frame.taskPrivateBacklinkAvailable; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = Cp180Phase::BeforeCallerXor; break;
        case 70: frame.callerRsi = 1; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: frame.callerRsi.reset(); break;
        case 78: frame.callerArithmeticFlags.reset(); break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        case 80: frame.privateTaskBacklinkAvailable = true; break;
        case 81: frame.taskPrivateIdentity = nullptr; break;
        case 82: frame.taskPrivateIdentity = &foreignPrivate; break;
        case 83: frame.taskPrivateBacklinkAvailable = false; frame.taskPrivateIdentity = &foreignPrivate; break;
        case 84: frame.taskPrivateBacklinkAvailable = true; frame.taskPrivateIdentity = nullptr; break;
        case 85: frame.phase = terminal ? Cp180Phase::BeforeCallerPrivateCopy : Cp180Phase::BeforeCallerPrivateSpill; break;
        case 86: frame.complete = true; break;
        case 87: fixture.pending->qobjectBaseCompleted = true; break;
        case 88: state.private08 = 1; break;
        case 89: state.private40.reset(); break;
        case 90: state.nativeAllocationBytes = 0xc0; break;
        case 91: state.sharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 92: state.qstringSharedNullBinding = foreignWidgets->qstringSharedNull(); break;
        case 93: state.qvectorSharedNullBinding = foreignWidgets->qvectorSharedNull(); break;
        case 94: frame.privateInitializerVersion.reset(); break;
        case 95: frame.privateState.reset(); break;
        case 96: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, 99); break;
        case 97: state.private80 = 1; break;
        case 98: frame.savedEntryRdi->taskIdentity = nullptr; break;
        case 99: frame.returnToCaller->privateIdentity = nullptr; break;
        case 100: state.qstringSharedNullReferenceCommitted = false; break;
        case 101: frame.r12PrivateIdentity = terminal ? nullptr : frame.allocatedPrivateIdentity; break;
        case 102: frame.r12PrivateIdentity = &foreignPrivate; break;
        case 103: state.sharedNullBinding.reset(); break;
        case 104: state.secondQListSharedNullBinding.reset(); break;
        case 105: state.qstringSharedNullBinding.reset(); break;
        case 106: state.qvectorSharedNullBinding.reset(); break;
        case 107: state.sharedNullReferenceCommitted = false; break;
        case 108: state.sharedNullBinding = aliasList; break;
        case 109: state.secondQListSharedNullBinding = aliasList; break;
        case 110: state.qstringSharedNullBinding = aliasString; break;
        case 111: state.qvectorSharedNullBinding = aliasVector; break;
        case 112: frame.savedTaskRbx->allocationOwner = expiredOwner; break;
        case 113: frame.savedEntryRdi->allocationOwner = expiredOwner; break;
        case 114: frame.returnToCaller->allocationOwner = expiredOwner; break;
        case 115: frame.taskAddressPointWritten = false; break;
        case 116: state.addressPointWritten = false; break;
        case 117: fixture.pending->kind = static_cast<Cp180Pending::Kind>(99); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp191_copy_changes_only_r12_and_cursor_and_reentry_never_replays)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131429"));
    REQUIRE(frame.phase == Cp180Phase::BeforeCallerPrivateCopy && !frame.r12PrivateIdentity);
    REQUIRE(frame.taskPrivateIdentity == frame.allocatedPrivateIdentity && frame.taskPrivateBacklinkAvailable);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x13142c"));
    CHECK(frame.phase == Cp180Phase::BeforeCallerPrivateSpill);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x13142c});
    CHECK(frame.r12PrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.r12PrivateIdentity == frame.returnPrivateIdentity && frame.taskPrivateIdentity == frame.allocatedPrivateIdentity);
    CHECK(frame.taskPrivateBacklinkAvailable && frame.rspFromEntryS == 8);
    CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
    CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Undo only MOV's observation and its derived cursor. The full snapshot
    // proves that memory, flags, saved slots, register identities, shared-null
    // records and allocation/reference counts are unchanged. No spill is modeled.
    frame.r12PrivateIdentity = nullptr;
    frame.phase = Cp180Phase::BeforeCallerPrivateCopy;
    predecessor.check();
    frame.r12PrivateIdentity = frame.returnPrivateIdentity;
    frame.phase = Cp180Phase::BeforeCallerPrivateSpill;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 14; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x13142c"));
            terminal.check();
        }
    }
    // Explicit consistent rewind restores both R12 observation and cursor.
    frame.r12PrivateIdentity = nullptr;
    frame.phase = Cp180Phase::BeforeCallerPrivateCopy;
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 12);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131429"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp191_default_and_all_intermediate_budgets_reach_only_register_copy)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto factory = session.taskFactoryRuntimeOwner();
    const auto store = session.taskAllocationStoreOwner();
    REQUIRE(factory && store);
    // Preserve the historical CP191 terminal.
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 13);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix);
    CHECK(pending->qobjectPrefix->r12PrivateIdentity == pending->qobjectPrefix->allocatedPrivateIdentity);
    const char* stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
        "RVA 0x13122f", "RVA 0x131232", "RVA 0x131235", "RVA 0x13123a",
        "RVA 0x13123e", "RVA 0x13123f", "RVA 0x13141c", "RVA 0x13141e",
        "RVA 0x131425", "RVA 0x131429", "RVA 0x13142c"};
    for (std::size_t stop = 0; stop != 14; ++stop) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stops[stop]));
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stops[stop]));
            stopped.check();
        }
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
        CHECK(frame.phase == Cp180Phase::BeforeCallerPrivateSpill && frame.r12PrivateIdentity == frame.allocatedPrivateIdentity);
        CHECK(frame.taskPrivateIdentity == frame.allocatedPrivateIdentity && frame.taskPrivateBacklinkAvailable);
        CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
        CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
        CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
        CHECK(fixture.factory->construction().metadataCacheCount == 0 && fixture.factory->construction().taskCacheCount == 0);
        CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == 1 && fixture.widgets->qvectorSharedNull()->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp191_premature_r12_refuses_before_any_prefix_mutation)
{
    for (std::size_t stop = 0; stop != 13; ++stop) {
        Cp182Fixture fixture(0);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA"));
        auto& frame = fixture.frame();
        frame.r12PrivateIdentity = frame.allocatedPrivateIdentity;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "CP191 requires phase-qualified R12 identity"));
        before.check();
    }
    Cp182Fixture fixture(0);
    auto& frame = fixture.frame();
    frame.phase = Cp180Phase::BeforeFirstSharedQList;
    frame.r12PrivateIdentity = frame.allocatedPrivateIdentity;
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP191 requires phase-qualified R12 identity"));
    before.check();
}

TEST_CASE(doe_task_factory_cp191_expired_widget_root_refuses_at_predecessor_and_terminal_without_mutation)
{
    for (const auto stop : {std::size_t{12}, std::size_t{13}}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP191 expiring root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, stop);
        REQUIRE(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, stop == 12 ? "RVA 0x131429" : "RVA 0x13142c"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        const auto frameBefore = frameValues(*pending);
        const auto stateBefore = privateValues(*pending->qobjectPrefix->privateState);
        const auto constructionBefore = constructionValues(factory);
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        widgets.reset();
        CHECK(!factory.widgetRuntimeOwner());
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 13);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "CP182 requires qualified CP181 retained prefix"));
            CHECK(frameValues(*pending) == frameBefore && privateValues(*pending->qobjectPrefix->privateState) == stateBefore);
            CHECK(constructionValues(factory) == constructionBefore);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(list->referenceDelta == 2 && string->referenceDelta == 1 && vector->referenceDelta == 1);
        }
    }
}

TEST_CASE(doe_task_factory_cp191_invalid_stop_refuses_at_terminal_without_mutation)
{
    Cp182Fixture fixture(4);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp192_rejects_corrupted_predecessor_and_terminal_without_mutation)
{
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 118; ++damage) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 14 : 13);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            terminal ? "RVA 0x131431" : "RVA 0x13142c"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP192 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        auto aliasList = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
            fixture.widgets->qlistSharedNull().get(), [](auto*) {});
        auto aliasString = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(
            fixture.widgets->qstringSharedNull().get(), [](auto*) {});
        auto aliasVector = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(
            fixture.widgets->qvectorSharedNull().get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expiredOwner;
        { auto temporary = std::make_shared<doe::TaskAllocationStore>(); expiredOwner = temporary; }
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = !frame.taskPrivateBacklinkAvailable; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = Cp180Phase::BeforeCallerXor; break;
        case 70: frame.callerRsi = 1; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: frame.callerRsi.reset(); break;
        case 78: frame.callerArithmeticFlags.reset(); break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        case 80: frame.privateTaskBacklinkAvailable = true; break;
        case 81: frame.taskPrivateIdentity = nullptr; break;
        case 82: frame.taskPrivateIdentity = &foreignPrivate; break;
        case 83: frame.taskPrivateBacklinkAvailable = false; frame.taskPrivateIdentity = &foreignPrivate; break;
        case 84: frame.taskPrivateBacklinkAvailable = true; frame.taskPrivateIdentity = nullptr; break;
        case 85: frame.phase = terminal ? Cp180Phase::BeforeCallerPrivateSpill : Cp180Phase::BeforePrivateTaskBacklink; break;
        case 86: frame.complete = true; break;
        case 87: fixture.pending->qobjectBaseCompleted = true; break;
        case 88: state.private08 = 1; break;
        case 89: state.private40.reset(); break;
        case 90: state.nativeAllocationBytes = 0xc0; break;
        case 91: state.sharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 92: state.qstringSharedNullBinding = foreignWidgets->qstringSharedNull(); break;
        case 93: state.qvectorSharedNullBinding = foreignWidgets->qvectorSharedNull(); break;
        case 94: frame.privateInitializerVersion.reset(); break;
        case 95: frame.privateState.reset(); break;
        case 96: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, 99); break;
        case 97: state.private80 = 1; break;
        case 98: frame.savedEntryRdi->taskIdentity = nullptr; break;
        case 99: frame.returnToCaller->privateIdentity = nullptr; break;
        case 100: state.qstringSharedNullReferenceCommitted = false; break;
        case 101: frame.r12PrivateIdentity = nullptr; break;
        case 102: frame.r12PrivateIdentity = &foreignPrivate; break;
        case 103: state.sharedNullBinding.reset(); break;
        case 104: state.secondQListSharedNullBinding.reset(); break;
        case 105: state.qstringSharedNullBinding.reset(); break;
        case 106: state.qvectorSharedNullBinding.reset(); break;
        case 107: state.sharedNullReferenceCommitted = false; break;
        case 108: state.sharedNullBinding = aliasList; break;
        case 109: state.secondQListSharedNullBinding = aliasList; break;
        case 110: state.qstringSharedNullBinding = aliasString; break;
        case 111: state.qvectorSharedNullBinding = aliasVector; break;
        case 112: frame.savedTaskRbx->allocationOwner = expiredOwner; break;
        case 113: frame.savedEntryRdi->allocationOwner = expiredOwner; break;
        case 114: frame.returnToCaller->allocationOwner = expiredOwner; break;
        case 115: frame.taskAddressPointWritten = false; break;
        case 116: state.addressPointWritten = false; break;
        case 117: fixture.pending->kind = static_cast<Cp180Pending::Kind>(99); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}



using Cp192Spill = Cp180Pending::QObjectPrefix::CallerPrivateSpill;
static_assert(std::is_nothrow_move_constructible_v<Cp192Spill>);
static_assert(std::is_nothrow_move_assignable_v<std::optional<Cp192Spill>>);
static_assert(noexcept(std::declval<std::optional<Cp192Spill>&>() = std::declval<Cp192Spill>()));

TEST_CASE(doe_task_factory_cp192_spill_changes_only_slot_and_cursor_and_reentry_never_replays)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
    REQUIRE(!frame.callerPrivateSpill && frame.phase == Cp180Phase::BeforeCallerPrivateSpill);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131431"));
    REQUIRE(frame.callerPrivateSpill);
    const auto spill = *frame.callerPrivateSpill;
    CHECK(frame.phase == Cp180Phase::BeforePrivateTaskBacklink);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131431});
    CHECK(Cp180Snapshot::sameWeak(spill.allocationOwner, std::weak_ptr{fixture.store}));
    CHECK(spill.reportOrdinal == *fixture.factory->construction().pendingReportAllocation);
    CHECK(spill.taskIdentity == fixture.pending && spill.privateIdentity == frame.allocatedPrivateIdentity);
    CHECK(spill.privateIdentity == frame.returnPrivateIdentity && spill.privateIdentity == frame.r12PrivateIdentity);
    CHECK(spill.moduleSha256 == Cp180Pending::QObjectPrefix::moduleSha256);
    CHECK(spill.initializerEntryRva == 0x131160 && spill.callRva == 0x131417 && spill.returnRva == 0x13141c);
    CHECK(spill.instructionRva == 0x13142c && spill.slotFromEntryS == 0x60 && spill.slotFromRsp == 0x58);
    CHECK(frame.rspFromEntryS == 8 && spill.slotFromEntryS == frame.rspFromEntryS + spill.slotFromRsp);
    CHECK(frame.taskPrivateIdentity == frame.allocatedPrivateIdentity && frame.taskPrivateBacklinkAvailable);
    CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
    CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Remove exactly the new slot and cursor: every other snapshot field must match.
    frame.callerPrivateSpill.reset();
    frame.phase = Cp180Phase::BeforeCallerPrivateSpill;
    predecessor.check();
    frame.callerPrivateSpill = spill;
    frame.phase = Cp180Phase::BeforePrivateTaskBacklink;
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 15; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131431"));
            terminal.check();
        }
    }
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
        terminal.check();
    }
    frame.callerPrivateSpill.reset();
    frame.phase = Cp180Phase::BeforeCallerPrivateSpill;
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 13);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x13142c"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
    terminal.check();
}

TEST_CASE(doe_task_factory_cp192_spill_corruption_refuses_even_with_smaller_budget)
{
    for (int damage = 0; damage != 18; ++damage) for (const auto budget : {std::size_t{0}, std::size_t{14}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expired;
        { auto owner = std::make_shared<doe::TaskAllocationStore>(); expired = owner; }
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        switch (damage) {
        case 0: frame.callerPrivateSpill.reset(); break;
        case 1: frame.callerPrivateSpill->allocationOwner.reset(); break;
        case 2: frame.callerPrivateSpill->allocationOwner = foreignOwner; break;
        case 3: frame.callerPrivateSpill->allocationOwner = aliasOwner; break;
        case 4: frame.callerPrivateSpill->allocationOwner = expired; break;
        case 5: ++frame.callerPrivateSpill->reportOrdinal; break;
        case 6: frame.callerPrivateSpill->taskIdentity = &foreignTask; break;
        case 7: frame.callerPrivateSpill->privateIdentity = &foreignPrivate; break;
        case 8: frame.callerPrivateSpill->privateIdentity = nullptr; break;
        case 9: frame.callerPrivateSpill->moduleSha256 = "foreign"; break;
        case 10: ++frame.callerPrivateSpill->initializerEntryRva; break;
        case 11: ++frame.callerPrivateSpill->callRva; break;
        case 12: ++frame.callerPrivateSpill->returnRva; break;
        case 13: ++frame.callerPrivateSpill->instructionRva; break;
        case 14: ++frame.callerPrivateSpill->slotFromEntryS; break;
        case 15: ++frame.callerPrivateSpill->slotFromRsp; break;
        case 16: frame.phase = Cp180Phase::BeforeCallerPrivateSpill; break; // phase-only rewind
        case 17: frame.callerPrivateSpill->taskIdentity = nullptr; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP192 requires phase-qualified caller spill"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp192_premature_spill_refuses_at_every_earlier_phase)
{
    for (int phase = 0; phase < static_cast<int>(Cp180Phase::BeforePrivateTaskBacklink); ++phase) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        frame.phase = static_cast<Cp180Phase>(phase);
        frame.callerPrivateSpill = Cp192Spill{fixture.store, 0, fixture.pending,
            frame.allocatedPrivateIdentity, Cp180Pending::QObjectPrefix::moduleSha256};
        if (frame.phase == Cp180Phase::BeforeCallerPrivateCopy || frame.phase == Cp180Phase::BeforeCallerPrivateSpill) {
            frame.taskPrivateIdentity = frame.allocatedPrivateIdentity;
            frame.taskPrivateBacklinkAvailable = true;
        }
        if (frame.phase == Cp180Phase::BeforeCallerPrivateSpill) frame.r12PrivateIdentity = frame.allocatedPrivateIdentity;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "CP192 requires phase-qualified caller spill"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp192_default_and_every_intermediate_budget_stop_before_reverse_store)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto factory = session.taskFactoryRuntimeOwner();
    const auto store = session.taskAllocationStoreOwner();
    REQUIRE(factory && store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 14);
    CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->callerPrivateSpill);
    CHECK(pending->qobjectPrefix->callerPrivateSpill->privateIdentity == pending->qobjectPrefix->allocatedPrivateIdentity);
    const char* stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
        "RVA 0x13122f", "RVA 0x131232", "RVA 0x131235", "RVA 0x13123a",
        "RVA 0x13123e", "RVA 0x13123f", "RVA 0x13141c", "RVA 0x13141e",
        "RVA 0x131425", "RVA 0x131429", "RVA 0x13142c", "RVA 0x131431"};
    for (std::size_t stop = 0; stop != 15; ++stop) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stops[stop]));
        CHECK(frame.callerPrivateSpill.has_value() == (stop == 14));
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stops[stop]));
            stopped.check();
        }
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
        CHECK(frame.phase == Cp180Phase::BeforePrivateTaskBacklink);
        REQUIRE(frame.callerPrivateSpill);
        CHECK(frame.callerPrivateSpill->privateIdentity == frame.allocatedPrivateIdentity);
        CHECK(frame.privateState->private08 == std::optional<std::uint64_t>{0});
        CHECK(!frame.privateTaskBacklinkAvailable && !frame.complete && !fixture.pending->qobjectBaseCompleted);
        CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
        CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == 1 && fixture.widgets->qvectorSharedNull()->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp192_expired_widget_root_refuses_at_predecessor_and_terminal_without_mutation)
{
    for (const auto stop : {std::size_t{13}, std::size_t{14}}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP192 expiring root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, stop);
        REQUIRE(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, stop == 13 ? "RVA 0x13142c" : "RVA 0x131431"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        const auto frameBefore = frameValues(*pending);
        const auto stateBefore = privateValues(*pending->qobjectPrefix->privateState);
        const auto constructionBefore = constructionValues(factory);
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        widgets.reset();
        CHECK(!factory.widgetRuntimeOwner());
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 14);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "CP182 requires qualified CP181 retained prefix"));
            CHECK(frameValues(*pending) == frameBefore && privateValues(*pending->qobjectPrefix->privateState) == stateBefore);
            CHECK(constructionValues(factory) == constructionBefore);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(list->referenceDelta == 2 && string->referenceDelta == 1 && vector->referenceDelta == 1);
        }
    }
}


TEST_CASE(doe_task_factory_cp193_rejects_inherited_cp192_corruption_without_mutation)
{
    for (const auto stop : {std::size_t{14}, std::size_t{15}, std::size_t{16}, std::size_t{17}, std::size_t{18}})
    for (const auto budget : {std::size_t{0}, std::size_t{17}})
    for (int damage = 0; damage != 118; ++damage) {
        const bool terminal = stop >= 15;
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        auto& state = *frame.privateState;
        if (stop >= 17) frame.savedEntryRdi->parentIsNull = (stop == 17);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop >= 17 ? 17 : stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            stop == 18 ? "RVA 0x13143a" : stop == 17 ? "RVA 0x131449" : stop == 16 ? "RVA 0x131438" : terminal ? "RVA 0x131435" : "RVA 0x131431"));
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP192 foreign");
        Cp180Pending::QObjectPrefix::SavedEntryRdi foreignRdi;
        auto aliasList = std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState>(
            fixture.widgets->qlistSharedNull().get(), [](auto*) {});
        auto aliasString = std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState>(
            fixture.widgets->qstringSharedNull().get(), [](auto*) {});
        auto aliasVector = std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState>(
            fixture.widgets->qvectorSharedNull().get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expiredOwner;
        { auto temporary = std::make_shared<doe::TaskAllocationStore>(); expiredOwner = temporary; }
        switch (damage) {
        case 0: frame.savedTaskRbx.reset(); break;
        case 1: frame.savedTaskRbx->taskIdentity = nullptr; break;
        case 2: frame.savedTaskRbx->taskIdentity = &foreignTask; break;
        case 3: frame.savedTaskRbx->allocationOwner = foreignOwner; break;
        case 4: frame.savedTaskRbx->allocationOwner = aliasOwner; break;
        case 5: ++frame.savedTaskRbx->reportOrdinal; break;
        case 6: frame.savedTaskRbx->privateIdentity = &foreignPrivate; break;
        case 7: frame.savedTaskRbx->moduleSha256 = "foreign"; break;
        case 8: ++frame.savedTaskRbx->initializerEntryRva; break;
        case 9: ++frame.savedTaskRbx->callRva; break;
        case 10: ++frame.savedTaskRbx->returnRva; break;
        case 11: ++frame.savedTaskRbx->rspFromEntryS; break;
        case 12: ++frame.savedTaskRbx->saveSlotFromEntryS; break;
        case 13: ++frame.savedTaskRbx->saveSlotFromRsp; break;
        case 14: frame.returnPrivateIdentity = nullptr; break;
        case 15: frame.returnPrivateIdentity = &foreignPrivate; break;
        case 16: frame.restoredTaskRbxIdentity = nullptr; frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 17: frame.restoredTaskRbxIdentity = &foreignTask; break;
        case 18: frame.phase = Cp180Phase::BeforePrivateReturnSetup; break;
        case 19: frame.flags20Eax.reset(); break;
        case 20: state.private20.reset(); break;
        case 21: frame.flags20Eax->privateIdentity = &foreignPrivate; break;
        case 22: state.private20->stage = Cp182Flags::Stage::Read; break;
        case 23: state.private28 = 1; break;
        case 24: frame.privateInitializerVersion->argument ^= 1; break;
        case 25: frame.privateInitializerVersion->callRva ^= 1; break;
        case 26: state.secondQListSharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 27: state.qvectorSharedNullReferenceCommitted = false; break;
        case 28: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 29: frame.taskPrivateBacklinkAvailable = !frame.taskPrivateBacklinkAvailable; break;
        case 30: frame.allocatedPrivateIdentity = &foreignPrivate; break;
        case 31: frame.taskAddressPointRva ^= 1; break;
        case 32: state.addressPointRva ^= 1; break;
        case 33: frame.savedTaskRbx->allocationOwner.reset(); break;
        case 34: ++frame.rspFromEntryS; break;
        case 35: frame.rspFromEntryS = -8; break;
        case 36: ++frame.savedRdiSlotFromEntryS; break;
        case 37: ++frame.returnSlotFromEntryS; break;
        case 38: frame.privateInitializerVersion->moduleSha256 = "foreign"; break;
        case 39: frame.privateInitializerVersion->argumentLoadRva ^= 1; break;
        case 40: frame.savedEntryRdi.reset(); break;
        case 41: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 42: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 43: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 44: ++frame.savedEntryRdi->reportOrdinal; break;
        case 45: frame.savedEntryRdi->taskIdentity = &foreignTask; break;
        case 46: frame.savedEntryRdi->privateIdentity = &foreignPrivate; break;
        case 47: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 48: ++frame.savedEntryRdi->initializerEntryRva; break;
        case 49: ++frame.savedEntryRdi->saveRva; break;
        case 50: frame.savedEntryRdi->origin = static_cast<Cp180Pending::QObjectPrefix::SavedEntryRdi::Origin>(99); break;
        case 51: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 52: ++frame.savedEntryRdi->callRva; break;
        case 53: ++frame.savedEntryRdi->returnRva; break;
        case 54: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 55: frame.restoredRdiToken = &foreignRdi; break;
        case 56: frame.phase = Cp180Phase::BeforePrivateStackAdjustment; break;
        case 57: frame.returnToCaller.reset(); break;
        case 58: frame.returnToCaller->allocationOwner.reset(); break;
        case 59: frame.returnToCaller->allocationOwner = foreignOwner; break;
        case 60: frame.returnToCaller->allocationOwner = aliasOwner; break;
        case 61: ++frame.returnToCaller->reportOrdinal; break;
        case 62: frame.returnToCaller->taskIdentity = &foreignTask; break;
        case 63: frame.returnToCaller->privateIdentity = &foreignPrivate; break;
        case 64: frame.returnToCaller->moduleSha256 = "foreign"; break;
        case 65: ++frame.returnToCaller->initializerEntryRva; break;
        case 66: ++frame.returnToCaller->callRva; break;
        case 67: frame.returnToCaller->targetRva = 0x131240; break;
        case 68: ++frame.returnToCaller->slotFromEntryS; break;
        case 69: frame.phase = Cp180Phase::BeforeCallerXor; break;
        case 70: frame.callerRsi = 1; break;
        case 71: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->carry = true; break;
        case 72: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->parity = false; break;
        case 73: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->zero = false; break;
        case 74: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->sign = true; break;
        case 75: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->overflow = true; break;
        case 76: frame.callerArithmeticFlags = Cp180Pending::QObjectPrefix::CallerArithmeticFlags{};
            frame.callerArithmeticFlags->auxiliary = false; break;
        case 77: frame.callerRsi.reset(); break;
        case 78:
            if (stop >= 16) frame.parentTestOutcome.reset();
            else frame.callerArithmeticFlags.reset();
            break;
        case 79: frame.phase = static_cast<Cp180Phase>(99); break;
        case 80: frame.privateTaskBacklinkAvailable = !frame.privateTaskBacklinkAvailable; break;
        case 81: frame.taskPrivateIdentity = nullptr; break;
        case 82: frame.taskPrivateIdentity = &foreignPrivate; break;
        case 83: frame.taskPrivateBacklinkAvailable = false; frame.taskPrivateIdentity = &foreignPrivate; break;
        case 84: frame.taskPrivateBacklinkAvailable = true; frame.taskPrivateIdentity = nullptr; break;
        case 85: frame.phase = terminal ? Cp180Phase::BeforePrivateTaskBacklink : Cp180Phase::BeforeCallerParentTest; break;
        case 86: frame.complete = true; break;
        case 87: fixture.pending->qobjectBaseCompleted = true; break;
        case 88: state.private08 = 1; break;
        case 89: state.private40.reset(); break;
        case 90: state.nativeAllocationBytes = 0xc0; break;
        case 91: state.sharedNullBinding = foreignWidgets->qlistSharedNull(); break;
        case 92: state.qstringSharedNullBinding = foreignWidgets->qstringSharedNull(); break;
        case 93: state.qvectorSharedNullBinding = foreignWidgets->qvectorSharedNull(); break;
        case 94: frame.privateInitializerVersion.reset(); break;
        case 95: frame.privateState.reset(); break;
        case 96: doe::detail::TaskFactoryTestAccess::replacePendingOrdinal(*fixture.factory, 99); break;
        case 97: state.private80 = 1; break;
        case 98: frame.savedEntryRdi->taskIdentity = nullptr; break;
        case 99: frame.returnToCaller->privateIdentity = nullptr; break;
        case 100: state.qstringSharedNullReferenceCommitted = false; break;
        case 101: frame.r12PrivateIdentity = nullptr; break;
        case 102: frame.r12PrivateIdentity = &foreignPrivate; break;
        case 103: state.sharedNullBinding.reset(); break;
        case 104: state.secondQListSharedNullBinding.reset(); break;
        case 105: state.qstringSharedNullBinding.reset(); break;
        case 106: state.qvectorSharedNullBinding.reset(); break;
        case 107: state.sharedNullReferenceCommitted = false; break;
        case 108: state.sharedNullBinding = aliasList; break;
        case 109: state.secondQListSharedNullBinding = aliasList; break;
        case 110: state.qstringSharedNullBinding = aliasString; break;
        case 111: state.qvectorSharedNullBinding = aliasVector; break;
        case 112: frame.savedTaskRbx->allocationOwner = expiredOwner; break;
        case 113: frame.savedEntryRdi->allocationOwner = expiredOwner; break;
        case 114: frame.returnToCaller->allocationOwner = expiredOwner; break;
        case 115: frame.taskAddressPointWritten = false; break;
        case 116: state.addressPointWritten = false; break;
        case 117: fixture.pending->kind = static_cast<Cp180Pending::Kind>(99); break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}




TEST_CASE(doe_task_factory_cp193_inherited_spill_corruption_refuses_even_with_smaller_budget)
{
    for (int damage = 0; damage != 18; ++damage) for (const auto budget : {std::size_t{0}, std::size_t{15}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expired;
        { auto owner = std::make_shared<doe::TaskAllocationStore>(); expired = owner; }
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        switch (damage) {
        case 0: frame.callerPrivateSpill.reset(); break;
        case 1: frame.callerPrivateSpill->allocationOwner.reset(); break;
        case 2: frame.callerPrivateSpill->allocationOwner = foreignOwner; break;
        case 3: frame.callerPrivateSpill->allocationOwner = aliasOwner; break;
        case 4: frame.callerPrivateSpill->allocationOwner = expired; break;
        case 5: ++frame.callerPrivateSpill->reportOrdinal; break;
        case 6: frame.callerPrivateSpill->taskIdentity = &foreignTask; break;
        case 7: frame.callerPrivateSpill->privateIdentity = &foreignPrivate; break;
        case 8: frame.callerPrivateSpill->privateIdentity = nullptr; break;
        case 9: frame.callerPrivateSpill->moduleSha256 = "foreign"; break;
        case 10: ++frame.callerPrivateSpill->initializerEntryRva; break;
        case 11: ++frame.callerPrivateSpill->callRva; break;
        case 12: ++frame.callerPrivateSpill->returnRva; break;
        case 13: ++frame.callerPrivateSpill->instructionRva; break;
        case 14: ++frame.callerPrivateSpill->slotFromEntryS; break;
        case 15: ++frame.callerPrivateSpill->slotFromRsp; break;
        case 16: frame.phase = Cp180Phase::BeforeCallerPrivateSpill; break; // phase-only rewind
        case 17: frame.callerPrivateSpill->taskIdentity = nullptr; break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}


using Cp193Publication = Cp180Pending::QObjectPrefix::PrivateTaskPublication;
static_assert(std::is_nothrow_move_constructible_v<Cp193Publication>);
static_assert(std::is_nothrow_move_assignable_v<std::optional<Cp193Publication>>);
static_assert(noexcept(std::declval<std::optional<Cp193Publication>&>() = std::declval<Cp193Publication>()));

TEST_CASE(doe_task_factory_cp193_reverse_store_changes_only_current_slot_and_cursor)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131435"));
    REQUIRE(frame.privateTaskPublication);
    const auto publication = *frame.privateTaskPublication;
    CHECK(frame.phase == Cp180Phase::BeforeCallerParentTest);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131435});
    CHECK(frame.privateTaskBacklinkAvailable && !frame.privateState->private08);
    CHECK(publication.taskIdentity == fixture.pending && publication.taskIdentity == frame.restoredTaskRbxIdentity);
    CHECK(publication.privateIdentity == frame.allocatedPrivateIdentity && publication.privateIdentity == frame.returnPrivateIdentity);
    CHECK(publication.reportOrdinal == *fixture.factory->construction().pendingReportAllocation);
    CHECK(Cp180Snapshot::sameWeak(publication.allocationOwner, std::weak_ptr{fixture.store}));
    CHECK(publication.moduleSha256 == Cp180Pending::QObjectPrefix::moduleSha256);
    CHECK(publication.instructionRva == 0x131431 && publication.destinationOffset == 8);
    CHECK(frame.taskPrivateBacklinkAvailable && frame.taskPrivateIdentity == frame.allocatedPrivateIdentity);
    REQUIRE(frame.callerPrivateSpill);
    CHECK(frame.callerPrivateSpill->privateIdentity == frame.allocatedPrivateIdentity);
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Undo exactly CP193: all inherited fields, ownership and counts match.
    frame.privateTaskPublication.reset();
    frame.privateTaskBacklinkAvailable = false;
    frame.privateState->private08 = std::uint64_t{0};
    frame.phase = Cp180Phase::BeforePrivateTaskBacklink;
    predecessor.check();
    // A deliberately consistent rewind reenters CP192 with its spill retained.
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131431"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
    CHECK(reverseValues(*frame.privateTaskPublication) == reverseValues(publication));
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 16; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131435"));
            terminal.check();
        }
    }
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp193_reverse_publication_corruption_refuses_without_mutation)
{
    for (int damage = 0; damage != 18; ++damage) for (const auto budget : {std::size_t{0}, std::size_t{15}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expired;
        { auto owner = std::make_shared<doe::TaskAllocationStore>(); expired = owner; }
        Cp180Pending foreignTask;
        Cp180Private foreignPrivate;
        switch (damage) {
        case 0: frame.privateTaskPublication.reset(); break;
        case 1: frame.privateTaskPublication->taskIdentity = nullptr; break;
        case 2: frame.privateTaskPublication->taskIdentity = &foreignTask; break;
        case 3: frame.privateTaskPublication->taskIdentity = reinterpret_cast<const Cp180Pending*>(frame.allocatedPrivateIdentity); break;
        case 4: frame.privateTaskPublication->allocationOwner.reset(); break;
        case 5: frame.privateTaskPublication->allocationOwner = expired; break;
        case 6: frame.privateTaskPublication->allocationOwner = foreignOwner; break;
        case 7: frame.privateTaskPublication->allocationOwner = aliasOwner; break;
        case 8: ++frame.privateTaskPublication->reportOrdinal; break;
        case 9: frame.privateTaskPublication->privateIdentity = &foreignPrivate; break;
        case 10: frame.privateTaskPublication->privateIdentity = nullptr; break;
        case 11: frame.privateTaskPublication->moduleSha256 = "foreign"; break;
        case 12: ++frame.privateTaskPublication->instructionRva; break;
        case 13: ++frame.privateTaskPublication->destinationOffset; break;
        case 14: frame.privateState->private08 = std::uint64_t{0}; break;
        case 15: frame.privateTaskBacklinkAvailable = false; break;
        case 16: frame.privateTaskBacklinkAvailable = false; frame.privateTaskPublication.reset(); break;
        case 17: frame.phase = Cp180Phase::BeforePrivateTaskBacklink; break; // phase-only rewind
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP193 requires phase-qualified private+8 publication"));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp193_premature_reverse_value_and_marker_refuse_at_every_earlier_phase)
{
    for (int phase = 0; phase < static_cast<int>(Cp180Phase::BeforeCallerParentTest); ++phase)
        for (int damage = 0; damage != 3; ++damage) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        frame.phase = static_cast<Cp180Phase>(phase);
        if (damage != 0) frame.privateTaskPublication = Cp193Publication{fixture.store, 0, fixture.pending,
            frame.allocatedPrivateIdentity, Cp180Pending::QObjectPrefix::moduleSha256};
        if (damage != 1) frame.privateTaskBacklinkAvailable = true;
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        // A bare marker retains earlier phase-specific prewrite diagnostics.
        // A value-bearing reverse observation always uses the CP193 guard.
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); },
            damage == 0 ? "" : "CP193 requires phase-qualified private+8 publication"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp193_pinned_session_and_every_intermediate_budget_stop_before_parent_test)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    const auto factory = session.taskFactoryRuntimeOwner();
    const auto store = session.taskAllocationStoreOwner();
    REQUIRE(factory && store);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, 15);
    REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateTaskPublication);
    CHECK(pending->qobjectPrefix->privateTaskPublication->taskIdentity == pending);
    CHECK(!pending->qobjectPrefix->privateState->private08 && pending->qobjectPrefix->privateTaskBacklinkAvailable);
    const char* stops[] = {"RVA 0x131224", "RVA 0x131227", "RVA 0x13122c",
        "RVA 0x13122f", "RVA 0x131232", "RVA 0x131235", "RVA 0x13123a",
        "RVA 0x13123e", "RVA 0x13123f", "RVA 0x13141c", "RVA 0x13141e",
        "RVA 0x131425", "RVA 0x131429", "RVA 0x13142c", "RVA 0x131431", "RVA 0x131435"};
    for (std::size_t stop = 0; stop != 16; ++stop) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, stop);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stops[stop]));
        CHECK(frame.privateTaskPublication.has_value() == (stop == 15));
        const Cp180Snapshot stopped(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stops[stop]));
            stopped.check();
        }
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
        REQUIRE(frame.privateTaskPublication && frame.callerPrivateSpill);
        CHECK(frame.privateTaskPublication->taskIdentity == fixture.pending);
        CHECK(frame.privateTaskPublication->privateIdentity == frame.allocatedPrivateIdentity);
        CHECK(!frame.privateState->private08 && frame.privateTaskBacklinkAvailable);
        CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
        CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
        CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == 2);
        CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == 1 && fixture.widgets->qvectorSharedNull()->referenceDelta == 1);
    }
}

TEST_CASE(doe_task_factory_cp193_expired_widget_root_refuses_at_predecessor_and_terminal_without_mutation)
{
    for (const auto stop : {std::size_t{14}, std::size_t{15}, std::size_t{16}}) {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP193 expiring root");
        store->associateWidgetRuntime(widgets);
        doe::TaskFactoryRuntime factory(store, widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, stop);
        REQUIRE(refuses([&] { factory.createTaskBase(L("SfTaskReport")); }, stop == 14 ? "RVA 0x131431" : stop == 15 ? "RVA 0x131435" : "RVA 0x131438"));
        auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->privateState);
        const auto frameBefore = frameValues(*pending);
        const auto stateBefore = privateValues(*pending->qobjectPrefix->privateState);
        const auto constructionBefore = constructionValues(factory);
        const auto list = widgets->qlistSharedNull();
        const auto string = widgets->qstringSharedNull();
        const auto vector = widgets->qvectorSharedNull();
        widgets.reset();
        CHECK(!factory.widgetRuntimeOwner());
        doe::detail::TaskFactoryTestAccess::flagsStopAt(factory, 15);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory.createTaskBase(key); }, "CP182 requires qualified CP181 retained prefix"));
            CHECK(frameValues(*pending) == frameBefore && privateValues(*pending->qobjectPrefix->privateState) == stateBefore);
            CHECK(constructionValues(factory) == constructionBefore);
            CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
            CHECK(list->referenceDelta == 2 && string->referenceDelta == 1 && vector->referenceDelta == 1);
        }
    }
}



TEST_CASE(doe_task_factory_cp193_requires_current_zero_before_reverse_store)
{
    for (const auto budget : {std::size_t{0}, std::size_t{15}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 14);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131431"));
        frame.privateState->private08.reset();
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 requires exact sparse zero prefix"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp193_refuses_other_live_report_task_token)
{
    Cp182Fixture fixture(4);
    auto& frame = fixture.frame();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
    const auto& otherReport = fixture.store->allocatePendingReportPrototype();
    frame.privateTaskPublication->taskIdentity = &otherReport;
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
        CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP193 requires phase-qualified private+8 publication"));
        before.check();
    }
}

TEST_CASE(doe_task_factory_cp193_bare_reverse_marker_preserves_historical_prewrite_diagnostics)
{
    for (const auto budget : {std::size_t{0}, std::size_t{15}}) {
        Cp182Fixture fixture(0);
        auto& frame = fixture.frame();
        cp180SetPrefix(frame, 0);
        frame.privateTaskBacklinkAvailable = true;
        REQUIRE(!frame.privateTaskPublication);
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); },
            "CP180 requires qualified CP179 retained prefix"));
        before.check();
        // Adding the current task value is independently premature, even when
        // the historical marker was already sufficient to refuse this phase.
        frame.privateTaskPublication = Cp193Publication{fixture.store, 0, fixture.pending,
            frame.allocatedPrivateIdentity, Cp180Pending::QObjectPrefix::moduleSha256};
        const Cp180Snapshot withValue(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("Custom")); },
            "CP193 requires phase-qualified private+8 publication"));
        withValue.check();
    }
}


TEST_CASE(doe_task_factory_cp194_test_flags_preserve_all_nonflag_state_and_predict_only)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Flag = Prefix::ParentTestOutcome::Flag;
    for (const auto nullness : {std::optional<bool>{}, std::optional<bool>{true}, std::optional<bool>{false}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        // Refine the symbolic saved input before POP consumes its qualification.
        frame.savedEntryRdi->parentIsNull = nullness;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131435"));
        REQUIRE(frame.restoredRdiParentIsNull == nullness);
        const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 16);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131438"));
        REQUIRE(frame.parentTestOutcome);
        const auto result = *frame.parentTestOutcome;
        CHECK(frame.phase == Cp180Phase::BeforeCallerParentJump);
        CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131438});
        CHECK(result.operand == &*frame.savedEntryRdi && result.operand == frame.restoredRdiToken);
        CHECK(result.parentIsNull == nullness && !frame.callerArithmeticFlags);
        CHECK(result.carry == Flag::Clear && result.overflow == Flag::Clear);
        CHECK(result.auxiliary == Flag::Undefined);
        CHECK(result.zero == (!nullness ? Flag::ParentIsZero : *nullness ? Flag::Set : Flag::Clear));
        CHECK(result.sign == (nullness == std::optional<bool>{true} ? Flag::Clear : Flag::ParentSign));
        CHECK(result.parity == (nullness == std::optional<bool>{true} ? Flag::Set : Flag::ParentLowByteEvenParity));
        CHECK(result.predictedJeSuccessorRva() == (!nullness ? std::optional<std::uint32_t>{} :
            std::optional<std::uint32_t>{*nullness ? 0x131449u : 0x13143au}));
        CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
        // Restore just current flags/phase. The complete predecessor snapshot
        // proves TEST changed no register, saved slot, memory, link or count.
        frame.parentTestOutcome.reset();
        frame.callerArithmeticFlags = Prefix::CallerArithmeticFlags{};
        frame.phase = Cp180Phase::BeforeCallerParentTest;
        predecessor.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 15);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131435"));
        predecessor.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 16);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131438"));
        CHECK(frame.parentTestOutcome == std::optional<Prefix::ParentTestOutcome>{result});
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (std::size_t budget = 0; budget != 17; ++budget) {
            doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
            for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
                CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131438"));
                terminal.check();
            }
        }
        for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
            doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
            CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
            terminal.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp194_parent_and_flag_corruption_refuses_without_mutation)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Flag = Prefix::ParentTestOutcome::Flag;
    for (const bool terminal : {false, true}) for (int damage = 0; damage != 34; ++damage)
    for (const auto budget : {std::size_t{0}, std::size_t{16}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        frame.savedEntryRdi->parentIsNull = false;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 16 : 15);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, terminal ? "RVA 0x131438" : "RVA 0x131435"));
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expired;
        { auto owner = std::make_shared<doe::TaskAllocationStore>(); expired = owner; }
        Prefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: frame.savedEntryRdi.reset(); break;
        case 1: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 2: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 3: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 4: frame.savedEntryRdi->allocationOwner = expired; break;
        case 5: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 6: frame.restoredRdiToken = &foreignRdi; break;
        case 7: frame.savedEntryRdi->parentIsNull = true; break;
        case 8: frame.restoredRdiParentIsNull.reset(); break;
        case 9: ++frame.savedRdiSlotFromEntryS; break;
        case 10: ++frame.savedEntryRdi->saveRva; break;
        case 11: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 12: ++frame.savedEntryRdi->callRva; break;
        case 13: ++frame.savedEntryRdi->returnRva; break;
        case 14: frame.privateTaskPublication->taskIdentity = nullptr; break;
        case 15: frame.privateTaskPublication->allocationOwner = aliasOwner; break;
        case 16: frame.taskPrivateIdentity = nullptr; break;
        case 17: frame.r12PrivateIdentity = nullptr; break;
        case 18: frame.callerPrivateSpill.reset(); break;
        case 19: ++frame.rspFromEntryS; break;
        case 20: frame.complete = true; break;
        case 21: fixture.pending->qobjectBaseCompleted = true; break;
        case 22: frame.phase = terminal ? Cp180Phase::BeforeCallerParentTest : Cp180Phase::BeforeCallerParentJump; break;
        case 23:
            if (terminal) frame.parentTestOutcome.reset();
            else frame.callerArithmeticFlags.reset();
            break;
        case 24:
            if (terminal) frame.parentTestOutcome->zero = Flag::Set;
            else frame.callerArithmeticFlags->zero = false;
            break;
        case 25:
            if (terminal) frame.parentTestOutcome->auxiliary = Flag::Clear;
            else frame.callerArithmeticFlags->auxiliary = false;
            break;
        case 26:
            if (terminal) frame.parentTestOutcome->operand = &foreignRdi;
            else frame.parentTestOutcome = Prefix::ParentTestOutcome::test(&*frame.savedEntryRdi);
            break;
        case 27:
            if (terminal) frame.parentTestOutcome->parentIsNull = true;
            else frame.callerArithmeticFlags->carry = true;
            break;
        case 28:
            if (terminal) frame.parentTestOutcome->sign = Flag::Clear;
            else frame.callerArithmeticFlags->sign = true;
            break;
        case 29:
            if (terminal) frame.parentTestOutcome->parity = Flag::Set;
            else frame.callerArithmeticFlags->parity = false;
            break;
        case 30:
            if (terminal) frame.parentTestOutcome->overflow = Flag::Set;
            else frame.callerArithmeticFlags->overflow = true;
            break;
        case 31:
            if (terminal) frame.callerArithmeticFlags = Prefix::CallerArithmeticFlags{};
            else frame.phase = static_cast<Cp180Phase>(999);
            break;
        case 32:
            if (terminal) {
                frame.parentTestOutcome.reset();
                frame.callerArithmeticFlags = Prefix::CallerArithmeticFlags{};
            } else frame.parentTestOutcome = Prefix::ParentTestOutcome::test(&*frame.savedEntryRdi);
            break;
        case 33:
            if (terminal) frame.parentTestOutcome->carry = Flag::Set;
            else frame.savedEntryRdi->moduleSha256 = "foreign";
            break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp194_standalone_retains_unknown_parent_expression)
{
    const auto store = std::make_shared<doe::TaskAllocationStore>();
    const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "unqualified standalone parent");
    store->associateWidgetRuntime(widgets);
    const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
    REQUIRE(factory && store);
    REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131438"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->parentTestOutcome);
    const auto& frame = *pending->qobjectPrefix;
    CHECK(!frame.savedEntryRdi->parentIsNull && !frame.parentTestOutcome->predictedJeSuccessorRva());
    CHECK(frame.parentTestOutcome->operand == frame.restoredRdiToken);
    CHECK(!frame.complete && !pending->qobjectBaseCompleted);
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
}


TEST_CASE(doe_task_factory_cp195_je_changes_only_path_and_cursor_for_qualified_refinements)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Path = Prefix::ParentBranchPath;
    for (const bool nullParent : {true, false}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        frame.savedEntryRdi->parentIsNull = nullParent;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 16);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131438"));
        REQUIRE(frame.parentTestOutcome && !frame.parentBranchPath);
        const auto testFlags = *frame.parentTestOutcome;
        const auto predicted = testFlags.predictedJeSuccessorRva();
        const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        const auto stop = nullParent ? "RVA 0x131449" : "RVA 0x13143a";
        const auto phase = nullParent ? Cp180Phase::BeforeCallerNullParentCall : Cp180Phase::BeforeCallerParentLoad;
        const auto path = nullParent ? Path::TakenNullParent : Path::FallthroughNonnullParent;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 17);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, stop));
        CHECK(frame.phase == phase && frame.parentBranchPath == std::optional<Path>{path});
        CHECK(frame.returnBoundaryRva() == predicted);
        CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{nullParent ? 0x131449u : 0x13143au});
        CHECK(frame.parentTestOutcome == std::optional<Prefix::ParentTestOutcome>{testFlags});
        CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
        // Undo exactly JE: flags and every register/memory/ownership/count
        // observation must already equal the complete CP194 predecessor.
        frame.phase = Cp180Phase::BeforeCallerParentJump;
        frame.parentBranchPath.reset();
        predecessor.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 16);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131438"));
        predecessor.check();
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 17);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stop));
        const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (std::size_t budget = 0; budget != 18; ++budget) {
            doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
            for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
                CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stop));
                terminal.check();
            }
        }
        for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
            doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
            CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
            terminal.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp195_standalone_unknown_branch_retains_full_cp194_state)
{
    const auto store = std::make_shared<doe::TaskAllocationStore>();
    const auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "unqualified standalone parent");
    store->associateWidgetRuntime(widgets);
    const auto factory = std::make_shared<doe::TaskFactoryRuntime>(store, widgets);
    REQUIRE(factory && store && widgets);
    // Default diagnostic budget admits TEST and the qualified JE seam, but unknown P
    // keeps the existing pre-JE guard without guessing either successor.
    REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131438"));
    const auto* pending = store->retainedPrototypeAt(0);
    REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->parentTestOutcome);
    const auto& frame = *pending->qobjectPrefix;
    CHECK(!frame.savedEntryRdi->parentIsNull && !frame.restoredRdiParentIsNull);
    CHECK(!frame.parentTestOutcome->predictedJeSuccessorRva() && !frame.parentBranchPath);
    CHECK(frame.phase == Cp180Phase::BeforeCallerParentJump);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131438});
    const Cp180Snapshot before(*pending, *store, *factory, *widgets);
    for (std::size_t budget = 0; budget != 18; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory->createTaskBase(key); }, "RVA 0x131438"));
            before.check();
        }
    }
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, invalid);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "CP182 invalid internal stop ordinal"));
        before.check();
    }
    CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
    CHECK(!frame.complete && !pending->qobjectBaseCompleted);
}

TEST_CASE(doe_task_factory_cp195_wrong_branch_state_and_partial_rewind_refuse_without_mutation)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Path = Prefix::ParentBranchPath;
    using Flag = Prefix::ParentTestOutcome::Flag;
    // Unknown predecessor, both qualified predecessors and both JE terminals.
    for (int mode = 0; mode != 5; ++mode) for (int damage = 0; damage != 27; ++damage)
    for (const auto budget : {std::size_t{0}, std::size_t{17}}) {
        Cp182Fixture fixture(4);
        auto& frame = fixture.frame();
        const bool terminal = mode >= 3;
        if (mode != 0) frame.savedEntryRdi->parentIsNull = (mode == 1 || mode == 3);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, terminal ? 17 : 16);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); },
            !terminal ? "RVA 0x131438" : mode == 3 ? "RVA 0x131449" : "RVA 0x13143a"));
        Prefix::SavedEntryRdi foreignRdi;
        auto foreignOwner = std::make_shared<doe::TaskAllocationStore>();
        auto aliasOwner = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        std::weak_ptr<doe::TaskAllocationStore> expired;
        { auto owner = std::make_shared<doe::TaskAllocationStore>(); expired = owner; }
        switch (damage) {
        case 0: frame.parentBranchPath = terminal ? std::optional<Path>{} : std::optional<Path>{Path::TakenNullParent}; break;
        case 1: frame.parentBranchPath = terminal ? (mode == 3 ? Path::FallthroughNonnullParent : Path::TakenNullParent) : static_cast<Path>(99); break;
        case 2: frame.phase = terminal ? Cp180Phase::BeforeCallerParentJump : Cp180Phase::BeforeCallerNullParentCall; break;
        case 3: frame.phase = terminal ? (mode == 3 ? Cp180Phase::BeforeCallerParentLoad : Cp180Phase::BeforeCallerNullParentCall) : Cp180Phase::BeforeCallerParentLoad; break;
        case 4: frame.savedEntryRdi.reset(); break;
        case 5: frame.savedEntryRdi->allocationOwner.reset(); break;
        case 6: frame.savedEntryRdi->allocationOwner = foreignOwner; break;
        case 7: frame.savedEntryRdi->allocationOwner = aliasOwner; break;
        case 8: frame.savedEntryRdi->allocationOwner = expired; break;
        case 9: frame.restoredRdiToken = nullptr; frame.restoredRdiParentIsNull.reset(); break;
        case 10: frame.restoredRdiToken = &foreignRdi; break;
        case 11: ++frame.savedRdiSlotFromEntryS; break;
        case 12: frame.parentTestOutcome.reset(); break;
        case 13: frame.parentTestOutcome->operand = &foreignRdi; break;
        case 14: frame.parentTestOutcome->zero = frame.parentTestOutcome->zero == Flag::Set ? Flag::Clear : Flag::Set; break;
        case 15: frame.parentTestOutcome->auxiliary = Flag::Clear; break;
        case 16: frame.callerArithmeticFlags = Prefix::CallerArithmeticFlags{}; break;
        case 17: frame.privateTaskPublication->taskIdentity = nullptr; break;
        case 18: frame.callerPrivateSpill.reset(); break;
        case 19: frame.r12PrivateIdentity = nullptr; break;
        case 20: frame.complete = true; break;
        case 21: fixture.pending->qobjectBaseCompleted = true; break;
        case 22:
            frame.savedEntryRdi->parentIsNull = terminal ? std::optional<bool>{} : std::optional<bool>{true};
            if (!terminal) frame.restoredRdiParentIsNull = false;
            break;
        case 23: frame.parentTestOutcome->parentIsNull = frame.parentTestOutcome->parentIsNull == std::optional<bool>{true} ? false : true; break;
        case 24: ++frame.rspFromEntryS; break;
        case 25: frame.phase = static_cast<Cp180Phase>(999); break;
        case 26:
            if (terminal) frame.parentBranchPath.reset();
            else {
                frame.phase = Cp180Phase::BeforeCallerNullParentCall;
                frame.parentBranchPath = Path::TakenNullParent;
                if (mode != 0) frame.savedEntryRdi->parentIsNull.reset();
            }
            break;
        }
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}


namespace {
static_assert(!std::is_default_constructible_v<doe::SessionReportNullBinding>);
static_assert(!std::is_copy_constructible_v<doe::SessionReportNullBinding>);
static_assert(std::is_nothrow_move_constructible_v<Cp180Pending::QObjectPrefix>);
static_assert(std::is_nothrow_move_constructible_v<Cp180Private>);
static_assert(std::is_nothrow_move_assignable_v<std::optional<Cp180Pending::QObjectPrefix>>);
struct Cp196Fixture {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    std::shared_ptr<doe::TaskAllocationStore> store = session.taskAllocationStoreOwner();
    std::shared_ptr<engine::WidgetRuntimeState> widgets = session.widgetRuntimeOwner();
    std::shared_ptr<doe::TaskFactoryRuntime> factory = session.taskFactoryRuntimeOwner();
    doe::PendingTaskPrototype* pending = nullptr;
    explicit Cp196Fixture(std::size_t stop = 17) {
        doe::detail::TaskFactoryTestAccess::calleeStopAt(*factory, 0);
        doe::detail::TaskFactoryTestAccess::tlsCmpAt(*factory,false);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*factory, stop);
        REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskDoe")); },
            stop == 17 ? "RVA 0x131449" : stop == 16 ? "RVA 0x131438" : ""));
        pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->savedEntryRdi);
    }
    auto& frame() { return *pending->qobjectPrefix; }
};
}

TEST_CASE(doe_task_factory_cp196_actual_session_issues_exact_null_source_and_stops_before_call)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Flag = Prefix::ParentTestOutcome::Flag;
    Cp196Fixture fixture;
    auto& frame = fixture.frame();
    const auto state = doe::detail::TaskFactoryTestAccess::sourceState(*fixture.factory);
    REQUIRE(std::get<0>(state) && std::get<1>(state) && std::get<2>(state) && std::get<3>(state));
    const auto capability = std::get<2>(state);
    const auto binding = std::get<3>(state);
    REQUIRE(frame.sessionReportSource == binding && frame.savedEntryRdi->sessionReportSource == binding);
    CHECK(binding->capability == capability && binding->receiver == fixture.factory.get());
    CHECK(binding->task == fixture.pending && binding->privateIdentity == frame.allocatedPrivateIdentity);
    CHECK(binding->ordinal == 0 && capability->ordinal == 0);
    CHECK(binding->source == doe::SessionReportNullBinding::SourceIdentity{});
    CHECK(binding->source.addresses[6] == 0x141403222 && binding->source.addresses[7] == 0x141403229);
    CHECK(binding->source.addresses[8] == 0x141e041c0 && binding->source.addresses[9] == 0x671313d0);
    CHECK(Cp180Snapshot::sameWeak(binding->allocations, std::weak_ptr{fixture.store}));
    CHECK(Cp180Snapshot::sameWeak(binding->widgets, std::weak_ptr{fixture.widgets}));
    CHECK(Cp180Snapshot::sameWeak(binding->frameOccurrence, std::weak_ptr{frame.sourceOccurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(binding->tokenOccurrence, std::weak_ptr{frame.savedEntryRdi->sourceOccurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(binding->privateOccurrence, std::weak_ptr{frame.privateState->allocationOccurrence.identity()}));
    CHECK(frame.savedEntryRdi->parentIsNull == std::optional<bool>{true});
    CHECK(frame.restoredRdiParentIsNull == std::optional<bool>{true});
    CHECK(frame.restoredRdiToken == &*frame.savedEntryRdi);
    REQUIRE(frame.parentTestOutcome);
    CHECK(frame.parentTestOutcome->zero == Flag::Set && frame.parentTestOutcome->sign == Flag::Clear);
    CHECK(frame.parentTestOutcome->parity == Flag::Set && frame.parentTestOutcome->auxiliary == Flag::Undefined);
    CHECK(frame.parentBranchPath == std::optional<Prefix::ParentBranchPath>{Prefix::ParentBranchPath::TakenNullParent});
    CHECK(frame.phase == Cp180Phase::BeforeCallerNullParentCall);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x131449});
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
    CHECK(!fixture.factory->construction().nativeInitializationComplete);
    CHECK(fixture.factory->construction().metadataCacheCount == 0 && fixture.factory->construction().taskCacheCount == 0);
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 18; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x131449"));
            terminal.check();
        }
    }
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "invalid internal stop ordinal"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp196_null_source_exists_before_pop_and_preserves_test_je_cuts)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    for (std::size_t stop = 0; stop != 18; ++stop) {
        Cp196Fixture fixture(stop);
        const auto& frame = fixture.frame();
        REQUIRE(frame.sessionReportSource && frame.savedEntryRdi->sessionReportSource);
        CHECK(frame.savedEntryRdi->parentIsNull == std::optional<bool>{true});
        CHECK(frame.restoredRdiParentIsNull == (stop >= 8 ? std::optional<bool>{true} : std::optional<bool>{}));
        CHECK(frame.parentTestOutcome.has_value() == (stop >= 16));
        CHECK(frame.parentBranchPath.has_value() == (stop >= 17));
    }
    Cp196Fixture fixture(15);
    auto& frame = fixture.frame();
    const Cp180Snapshot beforeTest(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 16);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131438"));
    frame.phase = Cp180Phase::BeforeCallerParentTest;
    frame.parentTestOutcome.reset();
    frame.callerArithmeticFlags = Prefix::CallerArithmeticFlags{};
    beforeTest.check();
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x131438"));
    const Cp180Snapshot beforeJe(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 17);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x131449"));
    frame.parentBranchPath.reset();
    frame.phase = Cp180Phase::BeforeCallerParentJump;
    beforeJe.check();
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131449"));
}

TEST_CASE(doe_task_factory_cp196_source_corruption_and_partial_rewind_refuse_without_repair)
{
    using Prefix = Cp180Pending::QObjectPrefix;
    using Access = doe::detail::TaskFactoryTestAccess;
    for (const bool helperTerminal : {false, true})
    for (int damage = 0; damage != 40; ++damage)
    for (const auto budget : {std::size_t{0}, std::size_t{18}}) {
        Cp196Fixture fixture;
        Cp196Fixture foreign;
        auto& frame = fixture.frame();
        if (helperTerminal) {
            Access::flagsStopAt(*fixture.factory, 18);
            REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x2aca0"));
        }
        auto foreignWidgets = std::make_shared<engine::WidgetRuntimeState>(true, "CP196 foreign root");
        auto aliasWidgets = std::shared_ptr<engine::WidgetRuntimeState>(fixture.widgets.get(), [](auto*) {});
        auto aliasStore = std::shared_ptr<doe::TaskAllocationStore>(fixture.store.get(), [](auto*) {});
        Prefix::SavedEntryRdi foreignRdi;
        switch (damage) {
        case 0: Access::clearCapability(*fixture.factory); break;
        case 1: frame.sessionReportSource.reset(); break;
        case 2: frame.savedEntryRdi->sessionReportSource.reset(); break;
        case 3: Access::clearCapability(*fixture.factory); frame.sessionReportSource.reset();
            frame.savedEntryRdi->sessionReportSource.reset(); Access::clearIssuedBinding(*fixture.factory); break;
        case 4: Access::copyCapability(*fixture.factory, *foreign.factory); break;
        case 5: Access::copyFrameBinding(*fixture.factory, *foreign.factory); break;
        case 6: frame.sessionReportSource = foreign.frame().sessionReportSource; break;
        case 7: frame.savedEntryRdi->sessionReportSource = foreign.frame().savedEntryRdi->sessionReportSource; break;
        case 8: Access::replaceWidgetOwner(*fixture.factory, foreignWidgets); break;
        case 9: Access::replaceWidgetOwner(*fixture.factory, aliasWidgets); break;
        case 10: Access::replaceAllocationOwner(*fixture.factory, aliasStore); break;
        case 11: Access::replaceAllocationOwner(*fixture.factory, foreign.store); break;
        case 12: fixture.pending->kind = static_cast<Cp180Pending::Kind>(99); break;
        case 13: fixture.pending->nativeAllocationBytes = 0x88; break;
        case 14: frame.allocatedPrivateIdentity = nullptr; break;
        case 15: frame.savedEntryRdi->parentIsNull.reset(); break;
        case 16: frame.savedEntryRdi->parentIsNull = false; break;
        case 17: frame.restoredRdiParentIsNull.reset(); break;
        case 18: frame.restoredRdiToken = &foreignRdi; break;
        case 19: ++frame.savedEntryRdi->incomingMoveRva; break;
        case 20: frame.savedEntryRdi->moduleSha256 = "foreign"; break;
        case 21: ++frame.savedEntryRdi->callRva; break;
        case 22: frame.parentTestOutcome->zero = Prefix::ParentTestOutcome::Flag::Clear; break;
        case 23: frame.parentBranchPath = Prefix::ParentBranchPath::FallthroughNonnullParent; break;
        case 24: frame.phase = Cp180Phase::BeforeCallerParentLoad; break;
        case 25: frame.complete = true; break;
        case 26: fixture.pending->qobjectBaseCompleted = true; break;
        case 27: frame.phase = Cp180Phase::BeforeCallerParentJump; break;
        case 28: frame.parentBranchPath.reset(); break;
        case 29: ++frame.savedRdiSlotFromEntryS; break;
        case 30: Access::replacePendingOrdinal(*fixture.factory, 1); break;
        case 31: frame.savedEntryRdi->sourceOccurrence = Prefix::SourceOccurrence{}; break;
        case 32: frame.sourceOccurrence = Prefix::SourceOccurrence{}; break;
        case 33: {
            auto saved = *frame.savedEntryRdi;
            frame.savedEntryRdi.reset();
            frame.savedEntryRdi = std::move(saved);
            break;
        }
        case 34: frame.privateState = std::make_unique<Cp180Private>(); break;
        case 35: {
            auto temporary = std::make_shared<engine::WidgetRuntimeState>(true, "expired root");
            Access::replaceWidgetOwner(*fixture.factory, temporary); break;
        }
        case 36: {
            auto temporary = std::make_shared<doe::TaskAllocationStore>();
            Access::replaceAllocationOwner(*fixture.factory, temporary); break;
        }
        case 37: frame.sessionReportSource = std::shared_ptr<const doe::SessionReportNullBinding>(
            frame.sessionReportSource.get(), [](auto*) {}); break;
        case 38: frame.savedEntryRdi->sessionReportSource = std::shared_ptr<const doe::SessionReportNullBinding>(
            frame.savedEntryRdi->sessionReportSource.get(), [](auto*) {}); break;
        case 39: {
            const auto capability = std::get<2>(Access::sourceState(*fixture.factory));
            Access::replaceCapability(*fixture.factory, std::shared_ptr<const doe::SessionReportNullBinding>(
                capability.get(), [](auto*) {})); break;
        }
        }
        Access::flagsStopAt(*fixture.factory, budget);
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp196_prefix_or_token_removal_never_reissues_source_occurrence)
{
    using Access = doe::detail::TaskFactoryTestAccess;
    for (int damage = 0; damage != 3; ++damage) {
        Cp196Fixture fixture;
        if (damage == 0) fixture.pending->qobjectPrefix.reset();
        if (damage == 1) fixture.frame().savedEntryRdi.reset();
        if (damage == 2) fixture.pending->qobjectPrefix.emplace();
        const auto factoryBefore = constructionValues(*fixture.factory);
        const auto listBefore = fixture.widgets->qlistSharedNull()->referenceDelta;
        const auto stringBefore = fixture.widgets->qstringSharedNull()->referenceDelta;
        const auto vectorBefore = fixture.widgets->qvectorSharedNull()->referenceDelta;
        const bool framePresent = fixture.pending->qobjectPrefix.has_value();
        std::optional<Cp180Snapshot> damaged;
        if (framePresent) damaged.emplace(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        const auto pendingBefore = std::make_tuple(fixture.pending->kind, fixture.pending->nativeAllocationBytes,
            fixture.pending->qobjectBaseCompleted);
        for (const auto budget : {std::size_t{0}, std::size_t{17}}) {
            Access::flagsStopAt(*fixture.factory, budget);
            CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP196"));
            CHECK(constructionValues(*fixture.factory) == factoryBefore);
            if (damaged) damaged->check();
            CHECK(std::make_tuple(fixture.pending->kind, fixture.pending->nativeAllocationBytes,
                fixture.pending->qobjectBaseCompleted) == pendingBefore);
            CHECK(fixture.pending->qobjectPrefix.has_value() == framePresent);
            CHECK(fixture.store->retainedPrototypeCount() == 1 && fixture.store->retainedTaskCount() == 0);
            CHECK(fixture.widgets->qlistSharedNull()->referenceDelta == listBefore);
            CHECK(fixture.widgets->qstringSharedNull()->referenceDelta == stringBefore);
            CHECK(fixture.widgets->qvectorSharedNull()->referenceDelta == vectorBefore);
        }
    }
}

TEST_CASE(doe_task_factory_cp196_pristine_ordinal_late_binding_and_post_scope_guards)
{
    using Access = doe::detail::TaskFactoryTestAccess;
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto factory = session.taskFactoryRuntimeOwner();
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        const auto before = constructionValues(*factory);
        CHECK(refuses([&] { Access::lateBind(*factory); }, "pristine Session Report issuer"));
        CHECK(constructionValues(*factory) == before);
        store->allocatePendingReportPrototype();
        const auto preallocated = constructionValues(*factory);
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "CP196"));
        CHECK(constructionValues(*factory) == preallocated);
        CHECK(store->retainedPrototypeCount() == 1 && !store->retainedPrototypeAt(0)->qobjectPrefix);
        CHECK(widgets->qlistSharedNull()->referenceDelta == 0);
    }
    {
        Cp196Fixture source;
        auto other = std::make_shared<doe::TaskFactoryRuntime>(source.store, source.widgets);
        Access::copyCapability(*other, *source.factory);
        const auto before = constructionValues(*other);
        const auto count = source.store->retainedPrototypeCount();
        CHECK(refuses([&] { other->createTaskBase(L("SfTaskReport")); }, "CP196"));
        CHECK(constructionValues(*other) == before && source.store->retainedPrototypeCount() == count);
        auto projection = doe::TaskFactoryRuntime::boundedParsedDoeProjection(source.store);
        Access::copyCapability(*projection, *source.factory);
        const auto projectionBefore = constructionValues(*projection);
        CHECK(refuses([&] { projection->createTaskBase(L("SfTaskDoe")); }, "CP196"));
        CHECK(constructionValues(*projection) == projectionBefore && source.store->retainedTaskCount() == 0);
    }
    {
        auto store = std::make_shared<doe::TaskAllocationStore>();
        auto widgets = std::make_shared<engine::WidgetRuntimeState>(true, "unpaired issuer refusal");
        doe::TaskFactoryRuntime unpaired(store, widgets);
        const auto before = constructionValues(unpaired);
        CHECK(refuses([&] { Access::lateBind(unpaired); }, "pristine Session Report issuer"));
        CHECK(constructionValues(unpaired) == before && store->retainedPrototypeCount() == 0);
        store->associateWidgetRuntime(widgets);
        store->allocatePendingReportPrototype();
        CHECK(refuses([&] { Access::lateBind(unpaired); }, "pristine Session Report issuer"));
        CHECK(constructionValues(unpaired) == before && store->retainedPrototypeCount() == 1);
    }
    {
        Cp182Fixture standalone(4);
        const Cp180Snapshot before(*standalone.pending, *standalone.store, *standalone.factory, *standalone.widgets);
        CHECK(refuses([&] { Access::lateBind(*standalone.factory); }, "pristine Session Report issuer"));
        before.check();
        const auto projection = doe::TaskFactoryRuntime::boundedParsedDoeProjection(standalone.store);
        const auto source = Access::sourceState(*projection);
        CHECK(!std::get<0>(source) && !std::get<1>(source) && !std::get<2>(source) && !std::get<3>(source));
    }
    {
        Cp196Fixture fixture;
        fixture.factory->releaseAdapterScope();
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "released"));
        before.check();
    }
    {
        std::shared_ptr<doe::TaskFactoryRuntime> factory;
        std::shared_ptr<doe::TaskAllocationStore> store;
        engine::DiagramEffectDriver driver;
        {
            engine::ConversionSession session(driver);
            factory = session.taskFactoryRuntimeOwner(); store = session.taskAllocationStoreOwner();
            Access::flagsStopAt(*factory, 17);
            REQUIRE(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131449"));
        }
        const auto before = constructionValues(*factory);
        CHECK(!store->adapterScopeLive() && !factory->nativeReceiverAlive());
        CHECK(refuses([&] { factory->createTaskBase(L("SfTaskReport")); }, "released"));
        CHECK(constructionValues(*factory) == before);
    }
}


TEST_CASE(doe_task_factory_cp196_missing_capability_and_invalid_budget_refuse_before_first_allocation)
{
    using Access = doe::detail::TaskFactoryTestAccess;
    for (int damage = 0; damage != 3; ++damage) {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto factory = session.taskFactoryRuntimeOwner();
        const auto store = session.taskAllocationStoreOwner();
        const auto widgets = session.widgetRuntimeOwner();
        if (damage == 0) Access::clearCapability(*factory);
        if (damage == 1) Access::flagsStopAt(*factory, 19);
        if (damage == 2) Access::stopAt(*factory, 0);
        const auto before = constructionValues(*factory);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { factory->createTaskBase(key); }, damage == 0 ? "CP196" : "invalid internal stop ordinal"));
            CHECK(constructionValues(*factory) == before);
            CHECK(store->retainedPrototypeCount() == 0 && store->retainedTaskCount() == 0);
            CHECK(widgets->qlistSharedNull()->referenceDelta == 0 && widgets->qstringSharedNull()->referenceDelta == 0);
            CHECK(widgets->qvectorSharedNull()->referenceDelta == 0);
        }
    }
}


TEST_CASE(doe_task_factory_cp196_same_storage_private_replacement_refuses_without_mutation)
{
    Cp196Fixture fixture;
    auto& frame = fixture.frame();
    auto* const storage = frame.privateState.get();
    const auto originalOccurrence = storage->allocationOccurrence.identity();
    const auto originalModel = privateValues(*storage);
    const auto list = storage->sharedNullBinding;
    const auto string = storage->qstringSharedNullBinding;
    const auto second = storage->secondQListSharedNullBinding;
    const auto vector = storage->qvectorSharedNullBinding;
    // Prepare the value copy while the original is alive. Copying creates an
    // independent allocation occurrence. Move construction into the same owned
    // storage is nonthrowing and preserves every copied model field.
    auto replacement = *storage;
    const auto replacementOccurrence = replacement.allocationOccurrence.identity();
    REQUIRE(replacementOccurrence != originalOccurrence);
    std::destroy_at(storage);
    std::construct_at(storage, std::move(replacement));
    CHECK(frame.privateState.get() == storage && frame.allocatedPrivateIdentity == storage);
    CHECK(frame.sessionReportSource->privateIdentity == storage);
    CHECK(privateValues(*storage) == originalModel);
    CHECK(Cp180Snapshot::sameWeak(storage->sharedNullBinding, list));
    CHECK(Cp180Snapshot::sameWeak(storage->qstringSharedNullBinding, string));
    CHECK(Cp180Snapshot::sameWeak(storage->secondQListSharedNullBinding, second));
    CHECK(Cp180Snapshot::sameWeak(storage->qvectorSharedNullBinding, vector));
    CHECK(storage->allocationOccurrence.identity() == replacementOccurrence);
    CHECK(Cp180Snapshot::sameWeak(frame.sessionReportSource->privateOccurrence, std::weak_ptr{originalOccurrence}));
    CHECK(!Cp180Snapshot::sameWeak(frame.sessionReportSource->privateOccurrence, std::weak_ptr{replacementOccurrence}));
    // Keep the original witness alive to demonstrate that refusal depends on
    // lifetime/control-block identity, independently of expiry or raw address.
    const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (const auto budget : {std::size_t{0}, std::size_t{17}}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "CP196 requires exact Session Report source binding"));
            before.check();
        }
    }
}


namespace {
using Cp197Prefix = Cp180Pending::QObjectPrefix;
static_assert(std::is_nothrow_move_assignable_v<std::optional<Cp197Prefix::NullParentCall>>);
static_assert(std::is_nothrow_move_assignable_v<std::optional<Cp197Prefix::LiveReturnSlot>>);
}

TEST_CASE(doe_task_factory_cp197_session_call_changes_only_generation_live_slot_rsp_and_cursor)
{
    Cp196Fixture fixture(17);
    auto& frame = fixture.frame();
    REQUIRE(frame.returnToCaller && frame.returnToCaller->callGeneration == 1);
    CHECK(frame.rspFromEntryS == 8 && !frame.liveReturnSlot());
    const auto initializerHistory = returnSlotValues(*frame.returnToCaller);
    const Cp180Snapshot predecessor(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 18);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskDoe")); }, "RVA 0x2aca0"));
    REQUIRE(frame.nullParentCall && frame.helperReturnSlot && frame.liveReturnSlot());
    const auto& call = *frame.nullParentCall;
    const auto live = *frame.liveReturnSlot();
    CHECK(frame.phase == Cp180Phase::BeforeNullParentHelperEntry);
    CHECK(frame.returnBoundaryRva() == std::optional<std::uint32_t>{0x2aca0});
    CHECK(frame.rspFromEntryS == 0);
    CHECK(call.callRva == 0x131449 && call.targetRva == 0x2aca0 && call.returnRva == 0x13144e);
    CHECK(call.callGeneration == 2 && call.precedingGeneration == 1);
    CHECK(call.callerRspFromEntryS == 8 && call.calleeRspFromEntryS == 0 && call.slotFromEntryS == 0);
    CHECK(call.sourceBinding == frame.sessionReportSource);
    CHECK(Cp180Snapshot::sameWeak(call.callerOccurrence, std::weak_ptr{frame.sourceOccurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(call.initializerOccurrence, std::weak_ptr{frame.returnToCaller->occurrence.identity()}));
    CHECK(call.occurrence.identity() != frame.returnToCaller->occurrence.identity());
    CHECK(live.callGeneration == 2 && live.valueRva == 0x13144e && live.slotFromEntryS == 0);
    CHECK(Cp180Snapshot::sameWeak(live.callOccurrence, std::weak_ptr{call.occurrence.identity()}));
    CHECK(returnSlotValues(*frame.returnToCaller) == initializerHistory);
    CHECK(frame.privateState->private40 == std::optional<std::uint64_t>{0});
    CHECK(!frame.complete && !fixture.pending->qobjectBaseCompleted);
    // Restore exactly the CALL's state. Full comparison proves flags, GPRs,
    // memory, links, owner/reference records and counts were all preserved.
    frame.nullParentCall.reset(); frame.helperReturnSlot.reset();
    doe::detail::TaskFactoryTestAccess::clearIssuedCallForRewind(*fixture.factory);
    frame.rspFromEntryS = 8; frame.phase = Cp180Phase::BeforeCallerNullParentCall;
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 17);
    CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x131449"));
    predecessor.check();
    doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 18);
    REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("Custom")); }, "RVA 0x2aca0"));
    const Cp180Snapshot terminal(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
    for (std::size_t budget = 0; budget != 19; ++budget) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, "RVA 0x2aca0"));
            terminal.check();
        }
    }
    for (const auto invalid : {std::size_t{19}, static_cast<std::size_t>(-1)}) {
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, invalid);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "invalid internal stop ordinal"));
        terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp197_initializer_live_slot_is_consumed_history_before_new_call)
{
    for (const auto stop : {std::size_t{0}, std::size_t{8}, std::size_t{9}, std::size_t{17}}) {
        Cp196Fixture fixture(stop);
        const auto& frame = fixture.frame();
        REQUIRE(frame.returnToCaller);
        CHECK(frame.returnToCaller->callGeneration == 1 && frame.returnToCaller->callRva == 0x131417);
        CHECK(frame.returnToCaller->targetRva == 0x13141c);
        const auto live = frame.liveReturnSlot();
        CHECK(live.has_value() == (stop <= 8));
        if (live) {
            CHECK(live->callGeneration == 1 && live->valueRva == 0x13141c && live->slotFromEntryS == 0);
            CHECK(Cp180Snapshot::sameWeak(live->callOccurrence, std::weak_ptr{frame.returnToCaller->occurrence.identity()}));
        }
        CHECK(!frame.nullParentCall && !frame.helperReturnSlot);
    }
}

TEST_CASE(doe_task_factory_cp197_pinned_session_enters_helper_while_standalone_parents_remain_guarded)
{
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        const auto factory = session.taskFactoryRuntimeOwner();
        doe::detail::TaskFactoryTestAccess::calleeStopAt(*factory, 0);
        const auto store = session.taskAllocationStoreOwner();
        REQUIRE(refuses([&] { factory->createTaskBase(L("Custom")); }, "RVA 0x2aca0"));
        const auto* pending = store->retainedPrototypeAt(0);
        REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->nullParentCall);
        CHECK(store->retainedPrototypeCount() == 1 && store->retainedTaskCount() == 0);
        CHECK(!pending->qobjectBaseCompleted && !pending->qobjectPrefix->complete);
    }
    for (const auto nullness : {std::optional<bool>{}, std::optional<bool>{true}, std::optional<bool>{false}}) {
        Cp182Fixture fixture(4);
        fixture.frame().savedEntryRdi->parentIsNull = nullness;
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 17);
        const auto stop = !nullness ? "RVA 0x131438" : *nullness ? "RVA 0x131449" : "RVA 0x13143a";
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, stop));
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, 18);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, stop));
            before.check();
        }
        CHECK(!fixture.frame().nullParentCall && !fixture.frame().helperReturnSlot);
    }
}

TEST_CASE(doe_task_factory_cp197_call_history_live_slot_and_partial_rewind_corruption_refuse)
{
    using Access = doe::detail::TaskFactoryTestAccess;
    using Flag = Cp197Prefix::ParentTestOutcome::Flag;
    for (const auto budget : {std::size_t{0}, std::size_t{18}})
    for (int damage = 0; damage != 47; ++damage) {
        Cp196Fixture fixture(17);
        Cp196Fixture foreign(17);
        auto& frame = fixture.frame();
        Access::flagsStopAt(*fixture.factory, 18);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x2aca0"));
        auto aliasTag = std::shared_ptr<const unsigned char>(frame.nullParentCall->occurrence.identity().get(), [](auto*) {});
        switch (damage) {
        case 0: frame.nullParentCall.reset(); break;
        case 1: frame.helperReturnSlot.reset(); break;
        case 2: ++frame.nullParentCall->callRva; break;
        case 3: frame.nullParentCall->targetRva = 0x13144e; break;
        case 4: frame.nullParentCall->returnRva = 0x13141c; break;
        case 5: frame.nullParentCall->callGeneration = 1; break;
        case 6: frame.nullParentCall->precedingGeneration = 2; break;
        case 7: ++frame.nullParentCall->slotFromEntryS; break;
        case 8: ++frame.nullParentCall->callerRspFromEntryS; break;
        case 9: --frame.nullParentCall->calleeRspFromEntryS; break;
        case 10: frame.nullParentCall->sourceBinding = foreign.frame().sessionReportSource; break;
        case 11: frame.nullParentCall->callerOccurrence = foreign.frame().sourceOccurrence.identity(); break;
        case 12: frame.nullParentCall->initializerOccurrence = foreign.frame().returnToCaller->occurrence.identity(); break;
        case 13: frame.helperReturnSlot->valueRva = 0x13141c; break;
        case 14: frame.helperReturnSlot->callGeneration = 1; break;
        case 15: ++frame.helperReturnSlot->slotFromEntryS; break;
        case 16: frame.helperReturnSlot->callOccurrence = frame.returnToCaller->occurrence.identity(); break;
        case 17: frame.helperReturnSlot->callOccurrence = aliasTag; break;
        case 18: frame.helperReturnSlot->callerOccurrence = foreign.frame().sourceOccurrence.identity(); break;
        case 19: frame.returnToCaller->callGeneration = 2; break;
        case 20: frame.returnToCaller->targetRva = 0x13144e; break;
        case 21: frame.returnToCaller->occurrence = Cp197Prefix::SourceOccurrence{}; break;
        case 22: frame.phase = Cp180Phase::BeforeCallerNullParentCall; break;
        case 23: frame.rspFromEntryS = 8; break;
        case 24: frame.nullParentCall.reset(); frame.helperReturnSlot.reset(); break;
        case 25: frame.phase = Cp180Phase::BeforeCallerNullParentCall; frame.rspFromEntryS = 8; break;
        case 26: frame.savedEntryRdi->parentIsNull.reset(); break;
        case 27: frame.savedEntryRdi->parentIsNull = false; break;
        case 28: frame.restoredRdiParentIsNull = false; break;
        case 29: frame.parentTestOutcome->zero = Flag::Clear; break;
        case 30: frame.parentBranchPath = Cp197Prefix::ParentBranchPath::FallthroughNonnullParent; break;
        case 31: frame.privateState->private40 = 42; break;
        case 32: frame.complete = true; break;
        case 33: fixture.pending->qobjectBaseCompleted = true; break;
        case 34: Access::clearCapability(*fixture.factory); break;
        case 35: Access::copyCapability(*fixture.factory, *foreign.factory); break;
        case 36: frame.sessionReportSource = foreign.frame().sessionReportSource; break;
        case 37: frame.r12PrivateIdentity = nullptr; break;
        case 38: ++frame.savedRdiSlotFromEntryS; break;
        case 39: frame.phase = static_cast<Cp180Phase>(999); break;
        case 40: frame.privateState->qvectorSharedNullReferenceCommitted = false; break;
        case 41: frame.nullParentCall->sourceBinding.reset(); break;
        case 42: frame.nullParentCall->occurrence = Cp197Prefix::SourceOccurrence{}; break;
        case 43: frame.helperReturnSlot->callerOccurrence.reset(); break;
        case 44: frame.rspFromEntryS = -8; break; // first callee PUSH is outside this cut
        case 45: frame.nullParentCall->sourceBinding = std::shared_ptr<const doe::SessionReportNullBinding>(
            frame.sessionReportSource.get(), [](auto*) {}); break;
        case 46: frame.helperReturnSlot->callOccurrence.reset(); break;
        }
        Access::flagsStopAt(*fixture.factory, budget);
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
            CHECK(refuses([&] { fixture.factory->createTaskBase(key); }, ""));
            before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp197_premature_call_or_slot_state_refuses_at_predecessor)
{
    for (const auto budget : {std::size_t{0}, std::size_t{18}}) for (int damage = 0; damage != 3; ++damage) {
        Cp196Fixture fixture(17);
        auto& frame = fixture.frame();
        if (damage != 1) frame.nullParentCall.emplace();
        if (damage != 0) frame.helperReturnSlot.emplace();
        const Cp180Snapshot before(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        doe::detail::TaskFactoryTestAccess::flagsStopAt(*fixture.factory, budget);
        CHECK(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "CP197 forbids premature"));
        before.check();
    }
}


TEST_CASE(doe_task_factory_cp197_coherent_replaced_call_occurrence_refuses_exact_issuance_error)
{
    using Access = doe::detail::TaskFactoryTestAccess;
    for (int replacement = 0; replacement != 4; ++replacement) {
        Cp196Fixture fixture(17);
        Cp196Fixture foreign(17);
        Access::flagsStopAt(*fixture.factory, 18);
        Access::flagsStopAt(*foreign.factory, 18);
        REQUIRE(refuses([&] { fixture.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x2aca0"));
        REQUIRE(refuses([&] { foreign.factory->createTaskBase(L("SfTaskReport")); }, "RVA 0x2aca0"));
        auto& frame = fixture.frame();
        const auto originalOccurrence = frame.nullParentCall->occurrence.identity();
        const auto issuedOccurrence = std::get<5>(Access::sourceState(*fixture.factory));
        REQUIRE(originalOccurrence == issuedOccurrence);
        if (replacement == 0) frame.nullParentCall->occurrence = Cp197Prefix::SourceOccurrence{};
        if (replacement == 1) {
            auto copied = *frame.nullParentCall;
            frame.nullParentCall = std::move(copied);
        }
        if (replacement == 2)
            frame.nullParentCall->occurrence = std::move(foreign.frame().nullParentCall->occurrence);
        if (replacement == 3) Access::clearIssuedCallForRewind(*fixture.factory);
        frame.helperReturnSlot->callOccurrence = frame.nullParentCall->occurrence.identity();
        CHECK((frame.nullParentCall->occurrence.identity() != originalOccurrence) == (replacement != 3));
        CHECK(Cp180Snapshot::sameWeak(frame.helperReturnSlot->callOccurrence,
            std::weak_ptr{frame.nullParentCall->occurrence.identity()}));
        CHECK(std::get<5>(Access::sourceState(*fixture.factory)) ==
            (replacement == 3 ? std::shared_ptr<const unsigned char>{} : issuedOccurrence));
        const Cp180Snapshot damaged(*fixture.pending, *fixture.store, *fixture.factory, *fixture.widgets);
        // The old issued witness remains alive. All mutable call/slot tuples
        // agree, so only the independent issuance anchor can reject this state.
        for (const auto budget : {std::size_t{0}, std::size_t{18}}) {
            Access::flagsStopAt(*fixture.factory, budget);
            for (const auto& key : {L("SfTaskReport"), L("SfTaskDoe"), L("Custom")}) {
                bool caught = false;
                try { fixture.factory->createTaskBase(key); }
                catch (const Error& error) {
                    caught = true;
                    CHECK(error.code == M2M_E_UNSUPPORTED);
                    CHECK(std::string(error.what()) ==
                        "DOE Task factory CP197 requires issued null-parent CALL occurrence");
                }
                CHECK(caught);
                damaged.check();
            }
        }
    }
}


namespace {
using Cp198Prefix = Cp180Pending::QObjectPrefix;
using Cp198State = Cp198Prefix::CalleePrefixState;
constexpr const char* cp198Stops[]{"RVA 0x2aca0","RVA 0x2aca2","RVA 0x2aca3","RVA 0x2aca4","RVA 0x2aca8",
    "RVA 0x2acb1","RVA 0x29600","RVA 0x29602","RVA 0x29606","RVA 0x2960f","RVA 0x29614"};
constexpr std::int32_t cp198Rsp[]{0,-8,-0x10,-0x18,-0x68,-0x68,-0x70,-0x78,-0xa8,-0xa8,-0xa8};
static_assert(std::is_nothrow_move_assignable_v<std::shared_ptr<const Cp198State>>);
static_assert(std::is_nothrow_move_constructible_v<Cp198State>);
template<class T> concept Cp198HasTlsIndex = requires(T value) { value.tlsIndex; };
static_assert(!Cp198HasTlsIndex<Cp198State> && !Cp198HasTlsIndex<Cp198Prefix>);
void cp198CheckBoundary(Cp196Fixture& fixture, std::size_t k)
{
    auto& f=fixture.frame();
    CHECK(f.rspFromEntryS==cp198Rsp[k]);
    CHECK(f.phase==static_cast<Cp180Phase>(static_cast<std::size_t>(Cp180Phase::BeforeNullParentHelperEntry)+k));
    REQUIRE(f.helperReturnSlot && f.nullParentCall && f.returnToCaller);
    CHECK(f.helperReturnSlot->callGeneration==2 && f.helperReturnSlot->valueRva==0x13144e);
    CHECK(f.returnToCaller->callGeneration==1 && f.returnToCaller->targetRva==0x13141c);
    CHECK(!f.entrySavedRdiSlotLive());
    CHECK(f.restoredTaskRbxIdentity==fixture.pending && f.callerRsi==std::optional<std::uint64_t>{0});
    CHECK(f.restoredRdiToken==&*f.savedEntryRdi && f.savedEntryRdi->parentIsNull==std::optional<bool>{true});
    CHECK(f.returnPrivateIdentity==f.allocatedPrivateIdentity && f.r12PrivateIdentity==f.allocatedPrivateIdentity);
    CHECK(f.currentArithmeticFlags()==(k>=4 ? Cp198Prefix::CurrentFlags::StackSubtract : Cp198Prefix::CurrentFlags::ParentTest));
    REQUIRE(f.parentTestOutcome);
    CHECK(f.parentTestOutcome->zero==Cp198Prefix::ParentTestOutcome::Flag::Set);
    if (k==0) { CHECK(!f.calleePrefix); return; }
    REQUIRE(f.calleePrefix);
    const auto& state=*f.calleePrefix;
    CHECK(state.completed==k);
    constexpr std::size_t at[]{1,2,3,5,6,7,9,10};
    constexpr std::int32_t offsets[]{-8,-0x10,-0x18,-0x28,-0x70,-0x78,-0x88,-0x68};
    constexpr std::uint32_t producers[]{0x2aca0,0x2aca2,0x2aca3,0x2aca8,0x2acb1,0x29600,0x29606,0x2960f};
    using Value=Cp198Prefix::CalleeStackSlot::Value;
    constexpr Value values[]{Value::TaskRbx,Value::CallerRsi,Value::ParentRdi,Value::MinusTwo,
        Value::NestedReturn,Value::ParentRdi,Value::MinusTwo,Value::TaskRbx};
    for (std::size_t i=0;i!=8;++i) {
        CHECK(static_cast<bool>(state.slots[i])==(k>=at[i]));
        if (k<at[i]) continue;
        const auto& slot=*state.slots[i];
        CHECK(slot.width==64 && slot.fromEntryS==offsets[i] && slot.instructionRva==producers[i]);
        CHECK(slot.generation==(i<4 ? 2u : 3u) && slot.value==values[i]);
        CHECK(slot.task==(values[i]==Value::TaskRbx ? fixture.pending : nullptr));
        CHECK(slot.parent==(values[i]==Value::ParentRdi ? f.restoredRdiToken : nullptr));
        CHECK(slot.numeric==(values[i]==Value::MinusTwo ? UINT64_C(0xfffffffffffffffe) :
            values[i]==Value::NestedReturn ? UINT64_C(0x2acb6) : 0));
        CHECK(Cp180Snapshot::sameWeak(slot.callerOccurrence,std::weak_ptr{f.sourceOccurrence.identity()}));
        CHECK(Cp180Snapshot::sameWeak(slot.callOccurrence,std::weak_ptr{i<4 ? f.nullParentCall->occurrence.identity() : state.nestedCall->occurrence.identity()}));
    }
    CHECK(static_cast<bool>(state.firstSub)==(k>=4)); CHECK(static_cast<bool>(state.secondSub)==(k>=8));
    if (k>=4) {
        CHECK(state.firstSub->producerRva==0x2aca4 && state.firstSub->operandFromEntryS==-0x18 &&
            state.firstSub->resultFromEntryS==-0x68 && state.firstSub->immediate==0x50);
        CHECK(state.currentSub==(k>=8 ? state.secondSub : state.firstSub));
        CHECK(state.currentSub->width==64);
        CHECK(Cp180Snapshot::sameWeak(state.currentSub->stackOrigin,std::weak_ptr{f.sourceOccurrence.identity()}));
        using Function=Cp198Prefix::StackSubFlags::Function;
        constexpr std::array functions{Function::CarryOfSub64,Function::EvenParityOfSubLowByte,
            Function::AuxiliaryBorrowOfSub64,Function::ZeroOfSub64,Function::SignOfSub64,Function::OverflowOfSub64};
        CHECK(state.currentSub->arithmetic==functions);
    }
    if (k>=8) CHECK(state.secondSub->producerRva==0x29602 && state.secondSub->operandFromEntryS==-0x78 &&
        state.secondSub->resultFromEntryS==-0xa8 && state.secondSub->immediate==0x30);
    CHECK(static_cast<bool>(state.nestedCall)==(k>=6));
    CHECK(state.innerReturnSlot.has_value()==(k>=6));
    const auto active=f.activeReturnSlots();
    CHECK(active[0].has_value() && active[1].has_value()==(k>=6));
    if (k>=6) {
        CHECK(state.nestedCall->generation==3 && state.nestedCall->precedingGeneration==2);
        CHECK(state.nestedCall->callRva==0x2acb1 && state.nestedCall->targetRva==0x29600 && state.nestedCall->returnRva==0x2acb6);
        CHECK(state.nestedCall->callerRsp==-0x68 && state.nestedCall->calleeRsp==-0x70);
        CHECK(f.liveReturnSlot()->callGeneration==3 && f.liveReturnSlot()->slotFromEntryS==-0x70);
        CHECK(active[0]->valueRva==0x13144e && active[1]->valueRva==0x2acb6);
    }
    CHECK(f.privateState->private40==std::optional<std::uint64_t>{0});
    CHECK(!f.complete && !fixture.pending->qobjectBaseCompleted);
}
void cp198Advance(Cp196Fixture& fixture,std::size_t target)
{
    doe::detail::TaskFactoryTestAccess::calleeStopAt(*fixture.factory,target);
    REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("SfTaskReport"));},cp198Stops[target]));
    cp198CheckBoundary(fixture,target);
}
}

TEST_CASE(doe_task_factory_cp198_ten_instruction_batch_has_exact_slots_flags_and_nested_returns)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    Cp196Fixture fixture(18);
    const Cp180Snapshot predecessor(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for (std::size_t k=0;k!=11;++k) cp198Advance(fixture,k);
    const auto terminal=fixture.frame().calleePrefix;
    CHECK(fixture.frame().returnBoundaryRva()==std::optional<std::uint32_t>{0x29614});
    Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),{});
    predecessor.check();
    Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),terminal);
    const Cp180Snapshot done(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for (std::size_t budget=0;budget!=11;++budget) {
        Access::calleeStopAt(*fixture.factory,budget);
        for (const auto baseBudget:{std::size_t{0},std::size_t{18}}) {
            Access::flagsStopAt(*fixture.factory,baseBudget);
            for (const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
                CHECK(refuses([&]{fixture.factory->createTaskBase(key);},"RVA 0x29614")); done.check();
            }
        }
    }
    Access::calleeStopAt(*fixture.factory,11);
    CHECK(refuses([&]{fixture.factory->createTaskBase(L("SfTaskReport"));},"CP198 invalid callee-prefix stop ordinal"));
    done.check();
}

TEST_CASE(doe_task_factory_cp198_all_split_resumes_and_coherent_boundary_rewinds_match_full_batch)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    Cp196Fixture fixture(18);
    const Cp180Snapshot entry(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    std::array<std::shared_ptr<const Cp198State>,11> checkpoints{};
    for(std::size_t k=0;k!=11;++k) {cp198Advance(fixture,k);checkpoints[k]=fixture.frame().calleePrefix;}
    CHECK(checkpoints[4]->firstSub==checkpoints[10]->firstSub);
    CHECK(checkpoints[6]->nestedCall==checkpoints[10]->nestedCall);
    CHECK(checkpoints[8]->secondSub==checkpoints[10]->secondSub);
    CHECK(checkpoints[1]->slots[0]==checkpoints[10]->slots[0]);
    for(std::size_t a=0;a!=11;++a) for(std::size_t b=a;b!=11;++b) {
        Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),{}); entry.check();
        cp198Advance(fixture,a); cp198Advance(fixture,b); cp198Advance(fixture,10);
        Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),{}); entry.check();
    }
    for(std::size_t k=0;k!=11;++k) {
        Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),checkpoints[k]);
        const Cp180Snapshot rewound(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        Access::calleeStopAt(*fixture.factory,k);
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},cp198Stops[k])); rewound.check();
        cp198Advance(fixture,10);
    }
}

TEST_CASE(doe_task_factory_cp198_default_session_stops_before_tls_cmp_and_invalid_budget_allocates_nothing)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
    const auto factory=session.taskFactoryRuntimeOwner(); const auto store=session.taskAllocationStoreOwner();
    Access::tlsCmpAt(*factory,false);
    Access::calleeStopAt(*factory,11);const auto before=constructionValues(*factory);
    CHECK(refuses([&]{factory->createTaskBase(L("Custom"));},"CP198 invalid callee-prefix stop ordinal"));
    CHECK(constructionValues(*factory)==before && store->retainedPrototypeCount()==0);
    Access::calleeStopAt(*factory,10);
    REQUIRE(refuses([&]{factory->createTaskBase(L("Custom"));},"RVA 0x29614"));
    const auto* pending=store->retainedPrototypeAt(0);REQUIRE(pending && pending->qobjectPrefix);
    CHECK(pending->qobjectPrefix->rspFromEntryS==-0xa8);
    CHECK(store->retainedPrototypeCount()==1 && store->retainedTaskCount()==0);
    CHECK(!pending->qobjectPrefix->complete && !pending->qobjectBaseCompleted);
}

TEST_CASE(doe_task_factory_cp198_copied_state_slot_nested_call_and_flags_refuse_exact_provenance)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    for(int kind=0;kind!=4;++kind) {
        Cp196Fixture fixture(18);cp198Advance(fixture,10);
        const auto issued=fixture.frame().calleePrefix;
        auto copied=std::make_shared<Cp198State>(*issued);
        if(kind==1) copied->slots[0]=std::make_shared<const Cp198Prefix::CalleeStackSlot>(*issued->slots[0]);
        if(kind==2) {
            auto nested=std::make_shared<const Cp198Prefix::NestedInitializerCall>(*issued->nestedCall);
            copied->nestedCall=nested;copied->innerReturnSlot->callOccurrence=nested->occurrence.identity();
            for(std::size_t i=4;i!=8;++i) {
                auto slot=std::make_shared<Cp198Prefix::CalleeStackSlot>(*issued->slots[i]);
                slot->callOccurrence=nested->occurrence.identity();copied->slots[i]=slot;
            }
        }
        if(kind==3) {
            copied->secondSub=std::make_shared<const Cp198Prefix::StackSubFlags>(*issued->secondSub);
            copied->currentSub=copied->secondSub;
        }
        fixture.frame().calleePrefix=copied;
        const Cp180Snapshot damaged(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto budget:{std::size_t{0},std::size_t{10}}) {
            Access::calleeStopAt(*fixture.factory,budget);
            for(const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
                bool caught=false;try{fixture.factory->createTaskBase(key);}catch(const Error& error){caught=true;
                    CHECK(error.code==M2M_E_UNSUPPORTED);
                    CHECK(std::string(error.what())=="DOE Task factory CP198 requires issued callee-prefix occurrence");}
                CHECK(caught);damaged.check();
            }
        }
    }
}

TEST_CASE(doe_task_factory_cp198_slot_call_flags_phase_and_source_corruption_refuse_without_mutation)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    for(int damage=0;damage!=32;++damage) for(const auto budget:{std::size_t{0},std::size_t{10}}) {
        Cp196Fixture fixture(18);cp198Advance(fixture,10);auto& f=fixture.frame();
        auto state=std::make_shared<Cp198State>(*f.calleePrefix);
        auto slot=std::make_shared<Cp198Prefix::CalleeStackSlot>(*state->slots[7]);
        auto call=std::make_shared<Cp198Prefix::NestedInitializerCall>(*state->nestedCall);
        auto flags=std::make_shared<Cp198Prefix::StackSubFlags>(*state->secondSub);
        // Preserve original immutable children unless the selected case changes one.
        switch(damage) {
        case 0: slot->width=32;state->slots[7]=slot;break;
        case 1: slot->fromEntryS=-8;state->slots[7]=slot;break;
        case 2: ++slot->instructionRva;state->slots[7]=slot;break;
        case 3: slot->value=Cp198Prefix::CalleeStackSlot::Value::MinusTwo;state->slots[7]=slot;break;
        case 4: slot->task=nullptr;state->slots[7]=slot;break;
        case 5: slot->numeric=42;state->slots[7]=slot;break;
        case 6: slot->generation=2;state->slots[7]=slot;break;
        case 7: slot->callOccurrence=f.nullParentCall->occurrence.identity();state->slots[7]=slot;break;
        case 8: slot->callerOccurrence.reset();state->slots[7]=slot;break;
        case 9: state->slots[0].reset();break;
        case 10: ++call->callRva;state->nestedCall=call;break;
        case 11: ++call->targetRva;state->nestedCall=call;break;
        case 12: call->returnRva=0x13144e;state->nestedCall=call;break;
        case 13: call->generation=2;state->nestedCall=call;break;
        case 14: call->enclosingOccurrence.reset();state->nestedCall=call;break;
        case 15: state->innerReturnSlot->valueRva=0x13144e;break;
        case 16: state->innerReturnSlot->slotFromEntryS=0;break;
        case 17: ++flags->producerRva;state->secondSub=flags;state->currentSub=flags;break;
        case 18: flags->operandFromEntryS=-0x18;state->secondSub=flags;state->currentSub=flags;break;
        case 19: flags->immediate=0x50;state->secondSub=flags;state->currentSub=flags;break;
        case 20: flags->arithmetic[2]=Cp198Prefix::StackSubFlags::Function::ZeroOfSub64;
            state->secondSub=flags;state->currentSub=flags;break;
        case 21: state->currentSub=state->firstSub;break;
        case 22: state->completed=9;break;
        case 23: f.phase=Cp180Phase::BeforeInitializerRbxSpill;break;
        case 24: f.rspFromEntryS=-0x70;break;
        case 25: f.helperReturnSlot->valueRva=0x2acb6;break;
        case 26: f.privateState->private40=42;break;
        case 27: f.complete=true;break;
        case 28: fixture.pending->qobjectBaseCompleted=true;break;
        case 29: Access::clearCapability(*fixture.factory);break;
        case 30: f.phase=static_cast<Cp180Phase>(999);break;
        case 31: f.privateState->qvectorSharedNullReferenceCommitted=false;break;
        }
        if(damage<23)f.calleePrefix=state;
        Access::calleeStopAt(*fixture.factory,budget);
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
            bool caught=false;try{fixture.factory->createTaskBase(key);}catch(const Error& error){caught=true;
                CHECK(error.code==M2M_E_UNSUPPORTED);
                CHECK(std::string(error.what()).find("stops at QtCore RVA")==std::string::npos);}
            CHECK(caught);before.check();
        }
    }
}


TEST_CASE(doe_task_factory_cp198_partial_public_rewind_and_premature_prefix_refuse)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    {
        Cp196Fixture fixture(18);cp198Advance(fixture,9);
        const auto earlier=fixture.frame().calleePrefix;
        cp198Advance(fixture,10);
        // Public state, phase, RSP, slots and flags are coherent, but private
        // issuance still identifies the later committed boundary.
        fixture.frame().calleePrefix=earlier;
        fixture.frame().phase=Cp180Phase::BeforeInitializerRbxSpill;
        fixture.frame().rspFromEntryS=-0xa8;
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto budget:{std::size_t{0},std::size_t{10}}) {
            Access::calleeStopAt(*fixture.factory,budget);
            bool caught=false;try{fixture.factory->createTaskBase(L("SfTaskReport"));}catch(const Error& error){caught=true;
                CHECK(std::string(error.what())=="DOE Task factory CP198 requires issued callee-prefix occurrence");}
            CHECK(caught);before.check();
        }
    }
    {
        Cp196Fixture fixture(17);
        fixture.frame().calleePrefix=std::make_shared<const Cp198State>();
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("SfTaskReport"));},"CP198 forbids premature"));before.check();
    }
}


TEST_CASE(doe_task_factory_cp198_every_stack_slot_width_producer_address_and_value_refuse_exactly)
{
    using Access=doe::detail::TaskFactoryTestAccess;
    using Value=Cp198Prefix::CalleeStackSlot::Value;
    for(std::size_t i=0;i!=8;++i) for(int damage=0;damage!=4;++damage) {
        Cp196Fixture fixture(18);cp198Advance(fixture,10);
        auto state=std::make_shared<Cp198State>(*fixture.frame().calleePrefix);
        auto slot=std::make_shared<Cp198Prefix::CalleeStackSlot>(*state->slots[i]);
        if(damage==0)slot->width=32;
        if(damage==1)++slot->instructionRva;
        if(damage==2)slot->fromEntryS+=8;
        if(damage==3) {
            if(slot->value==Value::TaskRbx)slot->task=nullptr;
            else if(slot->value==Value::ParentRdi)slot->parent=nullptr;
            else slot->numeric=42;
        }
        state->slots[i]=slot;fixture.frame().calleePrefix=state;
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto budget:{std::size_t{0},std::size_t{10}}) {
            Access::calleeStopAt(*fixture.factory,budget);
            for(const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
                bool caught=false;try{fixture.factory->createTaskBase(key);}catch(const Error& error){caught=true;
                    CHECK(error.code==M2M_E_UNSUPPORTED);
                    CHECK(std::string(error.what())=="DOE Task factory CP198 requires exact callee-prefix state");}
                CHECK(caught);before.check();
            }
        }
    }
}


namespace {
using Cp199Prefix=Cp180Pending::QObjectPrefix;
using Cp199Cmp=Cp199Prefix::TlsIndexCompare;
using Cp199Access=doe::detail::TaskFactoryTestAccess;
static_assert(!std::is_default_constructible_v<engine::QtCoreTlsIndexSymbol>);
static_assert(!std::is_copy_constructible_v<engine::QtCoreTlsIndexSymbol>);
static_assert(!std::is_copy_constructible_v<engine::TlsIndexReadExpression>);
static_assert(!std::is_copy_constructible_v<engine::TlsIndexSessionGrant>);
static_assert(std::is_nothrow_move_constructible_v<Cp199Cmp>);
template<class T>concept Cp199ConcreteValue=requires(T value){value.concreteValue;};
static_assert(!Cp199ConcreteValue<engine::TlsIndexReadExpression>);
struct Cp199Fixture:Cp196Fixture {
    Cp199Fixture():Cp196Fixture(18){cp198Advance(*this,10);}
    void compare(){Cp199Access::tlsCmpAt(*factory,true);Cp199Access::calleeStopAt(*factory,10);
        REQUIRE(refuses([&]{factory->createTaskBase(L("SfTaskReport"));},"RVA 0x2961b"));}
};
}

TEST_CASE(doe_task_factory_cp199_private_root_unknown_read_and_sub32_flags_preserve_predecessor)
{
    Cp199Fixture fixture;auto& f=fixture.frame();
    const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    const auto prefix=f.calleePrefix;fixture.compare();
    REQUIRE(f.tlsIndexCompare);const auto cmp=f.tlsIndexCompare;const auto read=cmp->operand;
    REQUIRE(read);const auto rootState=Cp199Access::tlsRootState(*fixture.widgets);
    const auto symbol=std::get<0>(rootState);const auto grant=std::get<1>(rootState);
    REQUIRE(symbol && grant && std::get<2>(rootState)==read);
    CHECK(symbol->identity==engine::QtCoreTlsIndexIdentity{});
    CHECK(symbol->identity.rva==0x2f08e8 && symbol->identity.width==32);
    CHECK(read->identity==symbol->identity && read->domain==engine::TlsIndexReadExpression::Domain::FullUnknownDword);
    CHECK(Cp180Snapshot::sameWeak(read->root,std::weak_ptr{fixture.widgets}));
    CHECK(Cp180Snapshot::sameWeak(read->thread,std::weak_ptr{fixture.widgets->threadState()}));
    CHECK(Cp180Snapshot::sameWeak(read->symbol,std::weak_ptr{symbol}));
    CHECK(Cp180Snapshot::sameWeak(read->grant,std::weak_ptr{grant}));
    CHECK(Cp180Snapshot::sameWeak(read->context.sourceBinding,std::weak_ptr{f.sessionReportSource}));
    CHECK(Cp180Snapshot::sameWeak(read->context.frame,std::weak_ptr{f.sourceOccurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(read->context.outerCall,std::weak_ptr{f.nullParentCall->occurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(read->context.innerCall,std::weak_ptr{prefix->nestedCall->occurrence.identity()}));
    CHECK(read->context.task==fixture.pending && read->context.privateIdentity==f.allocatedPrivateIdentity);
    CHECK(read->context.ordinal==0 && read->context.instructionRva==0x29614);
    CHECK(cmp->width==32 && cmp->producerRva==0x29614 && cmp->immediate==0xffffffffu);
    CHECK(cmp->encodedImmediateWidth==8 && cmp->signExtended && cmp->result==Cp199Cmp::Result::SubtractModulo32);
    using Function=Cp199Cmp::Function;
    constexpr std::array functions{Function::UnsignedBorrow32,Function::EvenParityOfResultLowByte,Function::AuxiliaryBorrow32,
        Function::ZeroOfResult32,Function::SignOfResult32,Function::SignedOverflow32};
    CHECK(cmp->arithmetic==functions);
    CHECK(f.currentArithmeticFlags()==Cp199Prefix::CurrentFlags::TlsIndexCompare);
    CHECK(f.phase==Cp180Phase::BeforeTlsIndexJne && f.returnBoundaryRva()==std::optional<std::uint32_t>{0x2961b});
    CHECK(!f.inCalleePrefix() && f.rspFromEntryS==-0xa8 && f.calleePrefix==prefix);
    CHECK(prefix->secondSub->producerRva==0x29602 && prefix->firstSub->producerRva==0x2aca4);
    CHECK(f.activeReturnSlots()[0]->callGeneration==2 && f.liveReturnSlot()->callGeneration==3);
    CHECK(!f.complete && !fixture.pending->qobjectBaseCompleted);
    Cp199Access::restoreCmpPredecessor(*fixture.factory,f);before.check();
    fixture.compare();
    CHECK(f.tlsIndexCompare!=cmp && f.tlsIndexCompare->operand!=read);
    CHECK(f.tlsIndexCompare->operand->domain==engine::TlsIndexReadExpression::Domain::FullUnknownDword);
}

TEST_CASE(doe_task_factory_cp199_terminal_reentry_has_no_second_read_under_any_valid_diagnostic_budget)
{
    Cp199Fixture fixture;fixture.compare();
    const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for(std::size_t base=0;base!=19;++base)for(const auto prefix:{std::size_t{0},std::size_t{10}})for(const bool enabled:{false,true}) {
        Cp199Access::flagsStopAt(*fixture.factory,base);Cp199Access::calleeStopAt(*fixture.factory,prefix);
        Cp199Access::tlsCmpAt(*fixture.factory,enabled);
        for(const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
            CHECK(refuses([&]{fixture.factory->createTaskBase(key);},"RVA 0x2961b"));before.check();
        }
    }
    Cp199Access::calleeStopAt(*fixture.factory,10);Cp199Access::flagsStopAt(*fixture.factory,19);
    CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"invalid internal stop ordinal"));before.check();
    Cp199Access::flagsStopAt(*fixture.factory,18);Cp199Access::calleeStopAt(*fixture.factory,11);
    CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"invalid callee-prefix stop ordinal"));before.check();
}

TEST_CASE(doe_task_factory_cp199_interrupted_prefixes_and_preparation_failure_preserve_trusted_root_state)
{
    Cp196Fixture fixture(18);
    const Cp180Snapshot entry(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for(std::size_t split=0;split!=11;++split) {
        Cp199Access::tlsCmpAt(*fixture.factory,false);cp198Advance(fixture,split);
        Cp199Access::calleeStopAt(*fixture.factory,10);Cp199Access::tlsCmpAt(*fixture.factory,true);
        REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"RVA 0x2961b"));
        CHECK(fixture.frame().rspFromEntryS==-0xa8 && fixture.frame().tlsIndexCompare);
        Cp199Access::restoreCmpPredecessor(*fixture.factory,fixture.frame());
        Cp199Access::restoreCalleeCheckpoint(*fixture.factory,fixture.frame(),{});entry.check();
    }
    Cp199Access::tlsCmpAt(*fixture.factory,false);cp198Advance(fixture,10);
    const Cp180Snapshot predecessor(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    Cp199Access::tlsCmpAt(*fixture.factory,true);Cp199Access::failTlsPreparation(*fixture.factory,true);
    for(const auto& key:{L("SfTaskReport"),L("SfTaskDoe"),L("Custom")}) {
        bool failed=false;try{fixture.factory->createTaskBase(key);}catch(const std::bad_alloc&){failed=true;}
        CHECK(failed);predecessor.check();
    }
    Cp199Access::failTlsPreparation(*fixture.factory,false);
    REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"RVA 0x2961b"));
}

TEST_CASE(doe_task_factory_cp199_default_session_and_distinct_roots_do_not_infer_values_or_branches)
{
    engine::DiagramEffectDriver driver;engine::ConversionSession session(driver);
    const auto factory=session.taskFactoryRuntimeOwner();const auto store=session.taskAllocationStoreOwner();
    REQUIRE(refuses([&]{factory->createTaskBase(L("Custom"));},"RVA 0x2961b"));
    const auto* pending=store->retainedPrototypeAt(0);REQUIRE(pending && pending->qobjectPrefix && pending->qobjectPrefix->tlsIndexCompare);
    const auto read=pending->qobjectPrefix->tlsIndexCompare->operand;
    Cp199Fixture other;other.compare();const auto otherRead=other.frame().tlsIndexCompare->operand;
    CHECK(read!=otherRead && read->occurrence!=otherRead->occurrence);
    CHECK(read->symbol.lock()!=otherRead->symbol.lock());
    CHECK(read->domain==otherRead->domain && read->identity==otherRead->identity);
    CHECK(store->retainedPrototypeCount()==1 && store->retainedTaskCount()==0 && !pending->qobjectBaseCompleted);
    CHECK(!factory->construction().nativeInitializationComplete);
}

TEST_CASE(doe_task_factory_cp199_copied_cmp_and_read_handle_substitutions_refuse_exact_errors)
{
    for(int mode=0;mode!=9;++mode) {
        Cp199Fixture fixture;fixture.compare();Cp199Fixture other;other.compare();
        const auto original=fixture.frame().tlsIndexCompare;
        const auto originalSymbol=std::get<0>(Cp199Access::tlsRootState(*fixture.widgets));
        auto clone=std::make_shared<Cp199Cmp>(*original);
        if(mode==1)clone->operand=other.frame().tlsIndexCompare->operand;
        if(mode==2)clone->operand=std::shared_ptr<const engine::TlsIndexReadExpression>(original->operand.get(),[](auto*){});
        if(mode==3) {
            clone->operand=Cp199Access::cloneRead(*original->operand,original->operand->identity,original->operand->context);
            Cp199Access::replaceRootRead(*fixture.widgets,clone->operand);
        }
        if(mode==4) {
            std::weak_ptr<engine::WidgetRuntimeState> expired;
            {auto temporary=std::make_shared<engine::WidgetRuntimeState>(true,"expired read root");expired=temporary;}
            clone->operand=Cp199Access::cloneRead(*original->operand,original->operand->identity,original->operand->context,expired,true);
            Cp199Access::replaceRootRead(*fixture.widgets,clone->operand);
        }
        if(mode==5) {
            clone->operand=Cp199Access::cloneReadThread(*original->operand,other.widgets->threadState());
            Cp199Access::replaceRootRead(*fixture.widgets,clone->operand);
        }
        if(mode==6) {
            Cp199Access::replaceRootSymbol(*fixture.widgets,std::shared_ptr<const engine::QtCoreTlsIndexSymbol>(originalSymbol.get(),[](auto*){}));
        }
        if(mode==7) {
            const auto symbol=std::get<0>(Cp199Access::tlsRootState(*fixture.widgets));auto identity=symbol->identity;identity.rva=0;
            Cp199Access::replaceRootSymbol(*fixture.widgets,Cp199Access::cloneSymbol(*symbol,identity));
        }
        if(mode==8)Cp199Access::clearRootGrant(*fixture.widgets);
        fixture.frame().tlsIndexCompare=clone;
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto budget:{std::size_t{0},std::size_t{10}}) {
            Cp199Access::calleeStopAt(*fixture.factory,budget);
            bool caught=false;try{fixture.factory->createTaskBase(L("Custom"));}catch(const Error& error){caught=true;
                CHECK(std::string(error.what())==(mode>=6 ? "DOE Task factory CP199 requires exact Session TLS grant" :
                    mode==0 || mode==3 ? "DOE Task factory CP199 requires issued symbolic CMP occurrence" :
                    "DOE Task factory CP199 requires exact symbolic TLS CMP state"));}
            CHECK(caught);before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp199_cmp_read_symbol_phase_and_history_corruption_refuse_without_mutation)
{
    for(int damage=0;damage!=25;++damage) {
        Cp199Fixture fixture;fixture.compare();auto& f=fixture.frame();
        const auto original=f.tlsIndexCompare;auto cmp=std::make_shared<Cp199Cmp>(*original);
        auto identity=original->operand->identity;auto context=original->operand->context;
        switch(damage) {
        case 0:cmp->width=64;break;case 1:cmp->immediate=0;break;case 2:cmp->encodedImmediateWidth=32;break;
        case 3:cmp->signExtended=false;break;case 4:++cmp->producerRva;break;
        case 5:cmp->arithmetic[0]=Cp199Cmp::Function::SignOfResult32;break;
        case 6:cmp->predecessor.reset();break;
        case 7:identity.width=64;break;case 8:++identity.rva;break;case 9:identity.moduleSha256="foreign";break;
        case 10:++context.instructionRva;break;case 11:context.innerCall.reset();break;case 12:context.outerCall.reset();break;
        case 13:context.frame.reset();break;case 14:context.task=nullptr;break;case 15:context.privateIdentity=nullptr;break;
        case 16:++context.ordinal;break;case 17:context.sourceBinding.reset();break;
        case 18:f.phase=Cp180Phase::BeforeTlsIndexCompare;break;case 19:f.rspFromEntryS=-0x70;break;
        case 20:f.privateState->private40=42;break;case 21:f.complete=true;break;
        case 22:fixture.pending->qobjectBaseCompleted=true;break;case 23:Cp199Access::clearRootRead(*fixture.widgets);break;
        case 24:Cp199Access::clearTlsGrant(*fixture.factory);break;
        }
        if(damage>=7 && damage<=17) {
            cmp->operand=Cp199Access::cloneRead(*original->operand,identity,context);
            Cp199Access::replaceRootRead(*fixture.widgets,cmp->operand);
        }
        if(damage<18)f.tlsIndexCompare=cmp;
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const auto budget:{std::size_t{0},std::size_t{10}}) {
            Cp199Access::calleeStopAt(*fixture.factory,budget);
            bool caught=false;try{fixture.factory->createTaskBase(L("Custom"));}catch(const Error& error){caught=true;
                CHECK(std::string(error.what()).find("stops at QtCore RVA")==std::string::npos);}
            CHECK(caught);before.check();
        }
    }
}

TEST_CASE(doe_task_factory_cp199_scope_release_and_retained_metadata_do_not_keep_roots_or_authorize_reads)
{
    {
        Cp199Fixture fixture;fixture.compare();const auto read=fixture.frame().tlsIndexCompare->operand;
        fixture.factory->releaseAdapterScope();
        CHECK(!std::get<2>(Cp199Access::tlsRootState(*fixture.widgets)) && !std::get<1>(Cp199Access::tlsRootState(*fixture.widgets)));
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"released"));
        CHECK(!read->root.expired()); // root still exists, grant has been revoked
    }
    {
        Cp199Fixture fixture;fixture.compare();const auto read=fixture.frame().tlsIndexCompare->operand;
        fixture.store->releaseAdapterScope();
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP196 requires exact Session Report source binding"));
        CHECK(read->domain==engine::TlsIndexReadExpression::Domain::FullUnknownDword);
    }
    std::shared_ptr<const engine::TlsIndexReadExpression> retained;
    std::shared_ptr<doe::TaskFactoryRuntime> factory;
    std::weak_ptr<engine::WidgetRuntimeState> root;
    engine::DiagramEffectDriver driver;
    {
        engine::ConversionSession session(driver);factory=session.taskFactoryRuntimeOwner();root=session.widgetRuntimeOwner();
        REQUIRE(refuses([&]{factory->createTaskBase(L("Custom"));},"RVA 0x2961b"));
        retained=session.taskAllocationStoreOwner()->retainedPrototypeAt(0)->qobjectPrefix->tlsIndexCompare->operand;
    }
    CHECK(root.expired() && retained->root.expired());
    CHECK(refuses([&]{factory->createTaskBase(L("Custom"));},"released"));
}


TEST_CASE(doe_task_factory_cp199_unqualified_issuance_and_partial_phase_flags_rewinds_refuse)
{
    {
        auto root=std::make_shared<engine::WidgetRuntimeState>(true,"unqualified symbolic namespace");
        const auto before=tlsRootValues(*root);
        CHECK(refuses([&]{Cp199Access::unqualifiedTlsRead(*root);},"CP199 requires exact Session TLS-read context"));
        CHECK(tlsRootValues(*root)==before);
    }
    for(int mode=0;mode!=3;++mode) {
        Cp199Fixture fixture;fixture.compare();
        if(mode==0)fixture.frame().phase=Cp180Phase::BeforeTlsIndexCompare;
        if(mode==1)fixture.frame().tlsIndexCompare.reset();
        if(mode==2) {
            fixture.frame().tlsIndexCompare.reset();
            Cp199Access::clearRootRead(*fixture.widgets);
        }
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(const bool enabled:{false,true}) {
            Cp199Access::tlsCmpAt(*fixture.factory,enabled);
            bool caught=false;try{fixture.factory->createTaskBase(L("Custom"));}catch(const Error& error){caught=true;
                CHECK(std::string(error.what()).find("stops at QtCore RVA")==std::string::npos);}
            CHECK(caught);before.check();
        }
    }
}

namespace {
using Cp200Jne=Cp199Prefix::TlsIndexJneUnresolved;
template<class T>concept Cp200SelectedEdge=requires(T value){value.selectedEdge;};
template<class T>concept Cp200ConcreteZf=requires(T value){value.concreteZf;};
static_assert(!Cp200SelectedEdge<Cp200Jne> && !Cp199ConcreteValue<Cp200Jne> && !Cp200ConcreteZf<Cp200Jne>);
static_assert(std::is_const_v<typename decltype(Cp199Prefix::tlsIndexJneUnresolved)::element_type>);
static void cp200Advance(Cp199Fixture& fixture)
{
    Cp199Access::jneSchema(*fixture.factory);
    REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before QtCore RVA 0x2961b"));
}
// Compare qualified semantics across independent runs; opaque occurrence/allocation
// tokens intentionally differ. Their relationships are authenticated separately below.
static auto cp201StaticEdgeValues(const Cp200Jne::Edge& edge)
{
    const auto& e=edge.potentialEffect;
    return std::make_tuple(edge.guard,edge.targetRva,e.status,e.bytes,e.instructionLength,e.operandWidth,
        e.operand,e.moduleOperandRva,e.rspDisplacement,e.stackFromEntryS,e.destination,e.result,
        e.flagsUnchanged,e.rspUnchanged,e.prospectiveNextRva);
}
static void cp200SemanticEqual(const Cp199Prefix& a,const Cp199Prefix& b)
{
    REQUIRE(a.tlsIndexJneUnresolved && b.tlsIndexJneUnresolved && a.tlsIndexCompare && b.tlsIndexCompare);
    const auto& x=*a.tlsIndexJneUnresolved;const auto& y=*b.tlsIndexJneUnresolved;
    CHECK(std::tie(x.moduleSha256,x.producerRva,x.bytes,x.instructionLength,x.flagsIndex,x.status)==
        std::tie(y.moduleSha256,y.producerRva,y.bytes,y.instructionLength,y.flagsIndex,y.status));
    for(std::size_t i=0;i!=2;++i)CHECK(cp201StaticEdgeValues(x.edges[i])==cp201StaticEdgeValues(y.edges[i]));
    CHECK(!x.edges[0].potentialEffect.existingSpill && !y.edges[0].potentialEffect.existingSpill);
    CHECK(x.edges[1].potentialEffect.existingSpill==a.calleePrefix->slots[7]);
    CHECK(y.edges[1].potentialEffect.existingSpill==b.calleePrefix->slots[7]);
    const auto& c=*a.tlsIndexCompare;const auto& d=*b.tlsIndexCompare;
    CHECK(std::tie(c.width,c.producerRva,c.immediate,c.encodedImmediateWidth,c.signExtended,c.result,c.arithmetic)==
        std::tie(d.width,d.producerRva,d.immediate,d.encodedImmediateWidth,d.signExtended,d.result,d.arithmetic));
    REQUIRE(c.operand && d.operand);
    CHECK(c.operand->identity==d.operand->identity && c.operand->domain==d.operand->domain);
    CHECK(c.operand->context.ordinal==d.operand->context.ordinal &&
        c.operand->context.instructionRva==d.operand->context.instructionRva);
    REQUIRE(a.calleePrefix && b.calleePrefix);const auto& h=*a.calleePrefix;const auto& k=*b.calleePrefix;
    CHECK(h.completed==k.completed);
    for(std::size_t i=0;i!=8;++i) {
        REQUIRE(h.slots[i] && k.slots[i]);const auto& u=*h.slots[i];const auto& v=*k.slots[i];
        CHECK(std::tie(u.generation,u.width,u.instructionRva,u.fromEntryS,u.value,u.numeric)==
            std::tie(v.generation,v.width,v.instructionRva,v.fromEntryS,v.value,v.numeric));
        CHECK((u.task==a.restoredTaskRbxIdentity)==(v.task==b.restoredTaskRbxIdentity));
        CHECK((u.parent==a.restoredRdiToken)==(v.parent==b.restoredRdiToken));
    }
    const std::array af{h.firstSub,h.secondSub,h.currentSub},bf{k.firstSub,k.secondSub,k.currentSub};
    for(std::size_t i=0;i!=3;++i) {
        REQUIRE(af[i] && bf[i]);const auto& u=*af[i];const auto& v=*bf[i];
        CHECK(std::tie(u.width,u.producerRva,u.operandFromEntryS,u.resultFromEntryS,u.immediate,u.arithmetic)==
            std::tie(v.width,v.producerRva,v.operandFromEntryS,v.resultFromEntryS,v.immediate,v.arithmetic));
    }
    REQUIRE(h.nestedCall && k.nestedCall);const auto& n=*h.nestedCall;const auto& m=*k.nestedCall;
    CHECK(std::tie(n.generation,n.precedingGeneration,n.callRva,n.targetRva,n.returnRva,n.callerRsp,n.calleeRsp,n.slotFromEntryS)==
        std::tie(m.generation,m.precedingGeneration,m.callRva,m.targetRva,m.returnRva,m.callerRsp,m.calleeRsp,m.slotFromEntryS));
    REQUIRE(a.nullParentCall && b.nullParentCall);const auto& o=*a.nullParentCall;const auto& p=*b.nullParentCall;
    CHECK(std::tie(o.callGeneration,o.precedingGeneration,o.callRva,o.targetRva,o.returnRva,o.callerRspFromEntryS,o.calleeRspFromEntryS,o.slotFromEntryS)==
        std::tie(p.callGeneration,p.precedingGeneration,p.callRva,p.targetRva,p.returnRva,p.callerRspFromEntryS,p.calleeRspFromEntryS,p.slotFromEntryS));
    const auto as=a.activeReturnSlots(),bs=b.activeReturnSlots();
    for(std::size_t i=0;i!=as.size();++i) {
        CHECK(bool(as[i])==bool(bs[i]));if(as[i] && bs[i])
            CHECK(std::tie(as[i]->callGeneration,as[i]->valueRva,as[i]->slotFromEntryS)==
                std::tie(bs[i]->callGeneration,bs[i]->valueRva,bs[i]->slotFromEntryS));
    }
    CHECK(std::tie(a.phase,a.rspFromEntryS,a.callerRsi,a.callerArithmeticFlags,a.parentBranchPath,a.restoredRdiParentIsNull,a.complete)==
        std::tie(b.phase,b.rspFromEntryS,b.callerRsi,b.callerArithmeticFlags,b.parentBranchPath,b.restoredRdiParentIsNull,b.complete));
    CHECK(a.returnBoundaryRva()==b.returnBoundaryRva() && a.currentArithmeticFlags()==b.currentArithmeticFlags());
    REQUIRE(a.savedTaskRbx && b.savedTaskRbx && a.savedEntryRdi && b.savedEntryRdi && a.returnToCaller && b.returnToCaller);
    const auto& ra=*a.savedTaskRbx;const auto& rb=*b.savedTaskRbx;
    CHECK(std::tie(ra.reportOrdinal,ra.moduleSha256,ra.initializerEntryRva,ra.callRva,ra.returnRva,ra.rspFromEntryS,ra.saveSlotFromEntryS,ra.saveSlotFromRsp)==
        std::tie(rb.reportOrdinal,rb.moduleSha256,rb.initializerEntryRva,rb.callRva,rb.returnRva,rb.rspFromEntryS,rb.saveSlotFromEntryS,rb.saveSlotFromRsp));
    const auto& da=*a.savedEntryRdi;const auto& db=*b.savedEntryRdi;
    CHECK(std::tie(da.reportOrdinal,da.moduleSha256,da.initializerEntryRva,da.saveRva,da.origin,da.incomingMoveRva,da.callRva,da.returnRva,da.parentIsNull)==
        std::tie(db.reportOrdinal,db.moduleSha256,db.initializerEntryRva,db.saveRva,db.origin,db.incomingMoveRva,db.callRva,db.returnRva,db.parentIsNull));
    const auto& ta=*a.returnToCaller;const auto& tb=*b.returnToCaller;
    CHECK(std::tie(ta.reportOrdinal,ta.moduleSha256,ta.initializerEntryRva,ta.callRva,ta.targetRva,ta.slotFromEntryS,ta.callGeneration)==
        std::tie(tb.reportOrdinal,tb.moduleSha256,tb.initializerEntryRva,tb.callRva,tb.targetRva,tb.slotFromEntryS,tb.callGeneration));
    REQUIRE(a.privateInitializerVersion && b.privateInitializerVersion && a.sessionReportSource && b.sessionReportSource);
    const auto& va=*a.privateInitializerVersion;const auto& vb=*b.privateInitializerVersion;
    CHECK(std::tie(va.moduleSha256,va.argument,va.argumentLoadRva,va.callRva)==
        std::tie(vb.moduleSha256,vb.argument,vb.argumentLoadRva,vb.callRva));
    CHECK(a.sessionReportSource->source==b.sessionReportSource->source);
    REQUIRE(a.flags20Eax && b.flags20Eax);const auto& fa=*a.flags20Eax;const auto& fb=*b.flags20Eax;
    CHECK(std::tie(fa.offset,fa.width,fa.readRva,fa.stage,fa.sourceKnownOne,fa.sourceKnownZero,fa.knownOne,fa.knownZero)==
        std::tie(fb.offset,fb.width,fb.readRva,fb.stage,fb.sourceKnownOne,fb.sourceKnownZero,fb.knownOne,fb.knownZero));
    CHECK((fa.privateIdentity==a.allocatedPrivateIdentity) && (fb.privateIdentity==b.allocatedPrivateIdentity));
    REQUIRE(a.parentTestOutcome && b.parentTestOutcome);
    const auto& pa=*a.parentTestOutcome;const auto& pb=*b.parentTestOutcome;
    CHECK(std::tie(pa.parentIsNull,pa.carry,pa.overflow,pa.zero,pa.sign,pa.parity,pa.auxiliary)==
        std::tie(pb.parentIsNull,pb.carry,pb.overflow,pb.zero,pb.sign,pb.parity,pb.auxiliary));
}
static void cp200ExactAuthority(Cp196Fixture& fixture)
{
    const auto& f=fixture.frame();const auto state=Cp199Access::sourceState(*fixture.factory);
    const auto root=Cp199Access::tlsRootState(*fixture.widgets);const auto jne=f.tlsIndexJneUnresolved;
    REQUIRE(jne && f.tlsIndexCompare && f.calleePrefix);
    const auto same=[](const auto& a,const auto& b){return a==b && !a.owner_before(b) && !b.owner_before(a);};
    CHECK(same(jne,std::get<9>(state)) && same(jne->predecessor,f.tlsIndexCompare));
    CHECK(same(f.tlsIndexCompare,std::get<8>(state)) && same(f.tlsIndexCompare->operand,std::get<2>(root)));
    CHECK(Cp180Snapshot::sameWeak(f.tlsIndexCompare->predecessor,std::weak_ptr{f.calleePrefix}));
    const auto read=f.tlsIndexCompare->operand;REQUIRE(read);
    CHECK(Cp180Snapshot::sameWeak(read->root,std::weak_ptr{fixture.widgets}));
    CHECK(Cp180Snapshot::sameWeak(read->thread,std::weak_ptr{fixture.widgets->threadState()}));
    CHECK(Cp180Snapshot::sameWeak(read->grant,std::weak_ptr{std::get<1>(root)}));
    CHECK(Cp180Snapshot::sameWeak(read->symbol,std::weak_ptr{std::get<0>(root)}));
    CHECK(Cp180Snapshot::sameWeak(read->context.frame,std::weak_ptr{f.sourceOccurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(read->context.outerCall,std::weak_ptr{f.nullParentCall->occurrence.identity()}));
    CHECK(Cp180Snapshot::sameWeak(read->context.innerCall,std::weak_ptr{f.calleePrefix->nestedCall->occurrence.identity()}));
}
}

TEST_CASE(doe_task_factory_cp200_paired_unexecuted_alternatives_preserve_exact_cp199_history)
{
    Cp199Fixture fixture;fixture.compare();auto& f=fixture.frame();
    const Cp180Snapshot predecessor(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    const auto cmp=f.tlsIndexCompare;const auto read=cmp->operand;const auto prefix=f.calleePrefix;
    CHECK(!f.tlsIndexJneUnresolved);
    CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP199 stops at"));predecessor.check();
    cp200Advance(fixture);const auto jne=f.tlsIndexJneUnresolved;REQUIRE(jne);
    CHECK(jne->predecessor==cmp && !jne->predecessor.owner_before(cmp) && !cmp.owner_before(jne->predecessor));
    CHECK(jne->moduleSha256==Cp199Prefix::moduleSha256 && jne->producerRva==0x2961b);
    const std::array<std::uint8_t,6> expected{0x0f,0x85,0x9f,0,0,0};CHECK(jne->bytes==expected);
    CHECK(jne->instructionLength==6 && jne->flagsIndex==3 && jne->occurrence.identity());
    CHECK(jne->status==Cp200Jne::Status::UnresolvedBeforeExecution);
    CHECK(jne->edges[0].guard==Cp200Jne::Guard::ZeroFlagSet && jne->edges[0].targetRva==0x29621);
    CHECK(jne->edges[1].guard==Cp200Jne::Guard::ZeroFlagClear && jne->edges[1].targetRva==0x296c0);
    CHECK(0x2961b+jne->instructionLength==jne->edges[0].targetRva);
    CHECK(0x2961b+jne->instructionLength+0x9f==jne->edges[1].targetRva);
    CHECK(f.phase==Cp180Phase::TlsIndexJneUnresolved && f.atTlsIndexBoundary() && !f.inCalleePrefix());
    CHECK(f.returnBoundaryRva()==std::optional<std::uint32_t>{0x2961b} && f.calleeRspFromEntryS()==-0xa8);
    CHECK(f.currentArithmeticFlags()==Cp199Prefix::CurrentFlags::TlsIndexCompare);
    CHECK(cmp->operand==read && read->domain==engine::TlsIndexReadExpression::Domain::FullUnknownDword);
    CHECK(f.calleePrefix==prefix && f.rspFromEntryS==-0xa8);
    CHECK(f.activeReturnSlots()[0]->callGeneration==2 && f.liveReturnSlot()->callGeneration==3);
    CHECK(!f.complete && !fixture.pending->qobjectBaseCompleted && fixture.store->retainedTaskCount()==0);
    const Cp180Snapshot terminal(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for(const bool enabled:{false,true})for(std::size_t stop=0;stop!=11;++stop) {
        Cp199Access::jneSchema(*fixture.factory,enabled);Cp199Access::calleeStopAt(*fixture.factory,stop);
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));terminal.check();
    }
    // Privileged fixture reconstruction proves all other captured state stayed intact.
    Cp199Access::restoreJnePredecessor(*fixture.factory,f);predecessor.check();
}

TEST_CASE(doe_task_factory_cp200_every_callee_interruption_and_preparation_failure_are_stable)
{
    Cp199Fixture baseline;baseline.compare();cp200Advance(baseline);
    for(std::size_t split=0;split!=11;++split) {
        Cp196Fixture fixture(18);cp198Advance(fixture,split);
        Cp199Access::tlsCmpAt(*fixture.factory,true);Cp199Access::calleeStopAt(*fixture.factory,10);
        REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP199 stops at"));
        Cp199Access::jneSchema(*fixture.factory);Cp199Access::failJnePreparation(*fixture.factory,true);
        const auto controls=Cp199Access::jneControls(*fixture.factory);
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(int retry=0;retry!=2;++retry) {
            bool failed=false;try{fixture.factory->createTaskBase(L("Custom"));}catch(const std::bad_alloc&){failed=true;}
            CHECK(failed);before.check();CHECK(Cp199Access::jneControls(*fixture.factory)==controls);
        }
        Cp199Access::failJnePreparation(*fixture.factory,false);
        REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));
        REQUIRE(fixture.frame().tlsIndexJneUnresolved);
        cp200SemanticEqual(fixture.frame(),baseline.frame());cp200ExactAuthority(fixture);
        const auto issued=fixture.frame().tlsIndexJneUnresolved;
        Cp199Access::restoreJnePredecessor(*fixture.factory,fixture.frame());before.check();
        Cp199Access::restoreJneCheckpoint(*fixture.factory,fixture.frame(),issued);
        const Cp180Snapshot terminal(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp200_malformed_records_foreign_owners_and_rewinds_refuse_atomically)
{
    for(int damage=0;damage!=31;++damage) {
        Cp199Fixture fixture;fixture.compare();cp200Advance(fixture);auto& f=fixture.frame();
        Cp199Fixture other;other.compare();cp200Advance(other);
        const auto original=f.tlsIndexJneUnresolved;auto jne=std::make_shared<Cp200Jne>(*original);
        switch(damage) {
        case 0:++jne->producerRva;break;case 1:jne->moduleSha256="foreign";break;
        case 2:++jne->bytes[2];break;case 3:++jne->instructionLength;break;case 4:jne->flagsIndex=0;break;
        case 5:jne->status=static_cast<Cp200Jne::Status>(1);break;
        case 6:std::swap(jne->edges[0],jne->edges[1]);break;
        case 7:jne->edges[0].guard=jne->edges[1].guard;break;
        case 8:++jne->edges[0].targetRva;break;case 9:++jne->edges[1].targetRva;break;
        case 10:jne->predecessor.reset();break;case 11:jne->predecessor=other.frame().tlsIndexCompare;break;
        case 12:jne->predecessor=std::shared_ptr<const Cp199Cmp>(f.tlsIndexCompare.get(),[](auto*){});break;
        case 13:jne->occurrence=other.frame().tlsIndexJneUnresolved->occurrence;break;
        case 14:break; // exact public copy still lacks issuance authority
        case 15:f.tlsIndexJneUnresolved=std::shared_ptr<const Cp200Jne>(original.get(),[](auto*){});break;
        case 16:f.tlsIndexJneUnresolved=other.frame().tlsIndexJneUnresolved;break;
        case 17:f.phase=Cp180Phase::BeforeTlsIndexJne;break;
        case 18:f.tlsIndexJneUnresolved.reset();break;
        case 19:f.tlsIndexCompare.reset();break;
        case 20:f.rspFromEntryS=-0x70;break;
        case 21:f.calleePrefix=other.frame().calleePrefix;break;
        case 22:Cp199Access::clearRootRead(*fixture.widgets);break;
        case 23:Cp199Access::clearRootGrant(*fixture.widgets);break;
        case 24:Cp199Access::replaceRootRead(*fixture.widgets,other.frame().tlsIndexCompare->operand);break;
        case 25:f.phase=Cp180Phase::BeforeTlsIndexJne;f.tlsIndexJneUnresolved.reset();break;
        case 26:f.phase=Cp180Phase::BeforeTlsIndexCompare;f.tlsIndexJneUnresolved.reset();f.tlsIndexCompare.reset();break;
        case 27:f.sourceOccurrence=other.frame().sourceOccurrence;break;
        case 28:f.nullParentCall=other.frame().nullParentCall;break;
        case 29:f.complete=true;break;
        case 30:{auto lost=std::move(jne->occurrence);break;}
        }
        if(damage<=14 || damage==30)f.tlsIndexJneUnresolved=jne;
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        bool caught=false;try{fixture.factory->createTaskBase(L("Custom"));}catch(const Error& error){caught=true;
            CHECK(std::string(error.what()).find("CP200 stops before")==std::string::npos);}
        CHECK(caught);before.check();
    }
}

TEST_CASE(doe_task_factory_cp200_retained_records_cannot_restore_released_scope_authority)
{
    for(int mode=0;mode!=2;++mode) {
        Cp199Fixture fixture;fixture.compare();cp200Advance(fixture);const auto jne=fixture.frame().tlsIndexJneUnresolved;
        if(mode==0) {
            fixture.factory->releaseAdapterScope();
            // Factory release retains the diagnostic frame, so the complete frame snapshot is valid.
            const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
            CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"released"));before.check();
        } else {
            fixture.store->releaseAdapterScope();
            // Allocation release destroys prototypes, including fixture.pending. Compare every
            // surviving owner and retained record; never dereference the released frame.
            const auto factoryBefore=constructionValues(*fixture.factory);
            const auto rootBefore=tlsRootValues(*fixture.widgets);
            const auto jneBefore=tlsJneValues(jne);
            const auto cmpBefore=tlsCmpValues(jne->predecessor);
            const auto readBefore=tlsReadValues(jne->predecessor->operand);
            const auto issuedBefore=std::get<9>(Cp199Access::sourceState(*fixture.factory));
            CHECK(!fixture.store->adapterScopeLive());
            CHECK(fixture.store->retainedPrototypeCount()==0 && fixture.store->retainedTaskCount()==0 &&
                fixture.store->retainedSessionCount()==0);
            for(int retry=0;retry!=2;++retry) {
                CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"source binding"));
                CHECK(constructionValues(*fixture.factory)==factoryBefore);
                CHECK(tlsRootValues(*fixture.widgets)==rootBefore);
                CHECK(tlsJneValues(jne)==jneBefore && tlsCmpValues(jne->predecessor)==cmpBefore);
                CHECK(tlsReadValues(jne->predecessor->operand)==readBefore);
                const auto issuedAfter=std::get<9>(Cp199Access::sourceState(*fixture.factory));
                CHECK(Cp180Snapshot::sameWeak(std::weak_ptr{issuedBefore},std::weak_ptr{issuedAfter}));
                CHECK(!fixture.store->adapterScopeLive());
                CHECK(fixture.store->retainedPrototypeCount()==0 && fixture.store->retainedTaskCount()==0 &&
                    fixture.store->retainedSessionCount()==0);
            }
        }
        CHECK(jne->predecessor->operand->domain==engine::TlsIndexReadExpression::Domain::FullUnknownDword);
    }
}

TEST_CASE(doe_task_factory_cp200_ordinary_session_driver_failure_and_expired_root_are_inert)
{
    engine::DiagramEffectDriver driver;
    std::shared_ptr<doe::TaskFactoryRuntime> retainedFactory;
    std::shared_ptr<const Cp200Jne> retainedJne;
    std::weak_ptr<engine::WidgetRuntimeState> weakRoot;
    {
        engine::ConversionSession session(driver);
        const auto factory=session.taskFactoryRuntimeOwner();const auto store=session.taskAllocationStoreOwner();
        const auto root=session.widgetRuntimeOwner();weakRoot=root;retainedFactory=factory;
        Cp199Access::jneSchema(*factory);Cp199Access::failJnePreparation(*factory,true);
        REQUIRE(refuses([&]{factory->createTaskBase(L("Custom"));},"CP199 stops at"));
        const auto* pending=store->retainedPrototypeAt(0);REQUIRE(pending && pending->qobjectPrefix);
        CHECK(!pending->qobjectPrefix->tlsIndexJneUnresolved);
        const Cp180Snapshot before(*pending,*store,*factory,*root);
        bool failed=false;try{factory->createTaskBase(L("Custom"));}catch(const std::bad_alloc&){failed=true;}
        CHECK(failed);before.check();
        Cp199Access::failJnePreparation(*factory,false);
        REQUIRE(refuses([&]{factory->createTaskBase(L("Custom"));},"CP200 stops before"));
        retainedJne=pending->qobjectPrefix->tlsIndexJneUnresolved;REQUIRE(retainedJne);
        Cp199Fixture other;other.compare();cp200Advance(other);const auto otherJne=other.frame().tlsIndexJneUnresolved;
        CHECK(retainedJne!=otherJne && retainedJne->occurrence.identity()!=otherJne->occurrence.identity());
        CHECK(retainedJne->bytes==otherJne->bytes);
        for(std::size_t i=0;i!=2;++i)CHECK(cp201StaticEdgeValues(retainedJne->edges[i])==cp201StaticEdgeValues(otherJne->edges[i]));
        CHECK(retainedJne->producerRva==otherJne->producerRva && retainedJne->status==otherJne->status);
        cp200SemanticEqual(*pending->qobjectPrefix,other.frame());cp200ExactAuthority(other);
        CHECK(store->retainedTaskCount()==0 && !factory->construction().nativeInitializationComplete);
    }
    CHECK(weakRoot.expired() && retainedJne->predecessor->operand->root.expired());
    CHECK(refuses([&]{retainedFactory->createTaskBase(L("Custom"));},"released"));
}

TEST_CASE(doe_task_factory_cp200_earlier_constructor_cuts_match_uninterrupted_semantics)
{
    Cp199Fixture baseline;baseline.compare();cp200Advance(baseline);cp200ExactAuthority(baseline);
    for(const std::size_t stop:{0u,4u,8u,12u,16u,17u,18u}) {
        Cp196Fixture fixture(stop);
        Cp199Access::flagsStopAt(*fixture.factory,18);Cp199Access::calleeStopAt(*fixture.factory,10);
        Cp199Access::tlsCmpAt(*fixture.factory,true);Cp199Access::jneSchema(*fixture.factory);
        REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP199 stops at"));
        const Cp180Snapshot predecessor(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        REQUIRE(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));
        cp200SemanticEqual(fixture.frame(),baseline.frame());cp200ExactAuthority(fixture);
        const auto issued=fixture.frame().tlsIndexJneUnresolved;
        Cp199Access::restoreJnePredecessor(*fixture.factory,fixture.frame());predecessor.check();
        Cp199Access::restoreJneCheckpoint(*fixture.factory,fixture.frame(),issued);
        const Cp180Snapshot terminal(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));terminal.check();
    }
}

TEST_CASE(doe_task_factory_cp200_independent_authority_substitution_revocation_and_factory_reuse_refuse)
{
    for(int damage=0;damage!=12;++damage) {
        Cp199Fixture fixture;fixture.compare();cp200Advance(fixture);
        Cp199Fixture other;other.compare();cp200Advance(other);
        const auto retained=fixture.frame().tlsIndexJneUnresolved;
        const auto originalGrant=std::get<1>(Cp199Access::tlsRootState(*fixture.widgets));
        switch(damage) {
        case 0:Cp199Access::replaceWidgetOwner(*fixture.factory,other.widgets);break;
        case 1:{ // Expired replacement weak owner with the adapter and original diagnostics still live.
            auto temporary=std::make_shared<engine::WidgetRuntimeState>(true,"expired replacement TLS root");
            Cp199Access::replaceWidgetOwner(*fixture.factory,temporary);break;}
        case 2:Cp199Access::replaceRootThread(*fixture.widgets,other.widgets->threadState());break;
        case 3:Cp199Access::replaceRootThread(*fixture.widgets,{});break;
        case 4:Cp199Access::clearCapability(*fixture.factory);break;
        case 5:Cp199Access::copyCapability(*fixture.factory,*other.factory);break;
        case 6:Cp199Access::replaceGrants(*fixture.factory,*fixture.widgets,
            std::get<1>(Cp199Access::tlsRootState(*other.widgets)));break;
        case 7:Cp199Access::replaceGrants(*fixture.factory,*fixture.widgets,
            Cp199Access::cloneGrant(*originalGrant,{},originalGrant->capability));break;
        case 8:Cp199Access::replaceGrants(*fixture.factory,*fixture.widgets,
            Cp199Access::cloneGrant(*originalGrant,originalGrant->thread,{}));break;
        case 9:Cp199Access::revokeLiveGrant(*fixture.factory,*fixture.widgets);break;
        case 10:Cp199Access::restoreJneCheckpoint(*other.factory,other.frame(),retained);break;
        case 11:Cp199Access::clearRootGrant(*fixture.widgets);break;
        }
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        const Cp180Snapshot otherBefore(*other.pending,*other.store,*other.factory,*other.widgets);
        const auto authority=Cp199Access::authorityState(*fixture.factory,*fixture.widgets);
        const auto values=tlsJneValues(retained);const auto read=retained->predecessor->operand;
        const auto readValues=tlsReadValues(read);
        for(int retry=0;retry!=2;++retry) {
            auto& target=damage==10?other:fixture;
            bool caught=false;try{target.factory->createTaskBase(L("Custom"));}catch(const Error& error){caught=true;
                CHECK(std::string(error.what()).find("CP200 stops before")==std::string::npos);}
            CHECK(caught);before.check();otherBefore.check();
            CHECK(Cp199Access::authorityState(*fixture.factory,*fixture.widgets)==authority);
            CHECK(tlsJneValues(retained)==values && tlsReadValues(read)==readValues);
        }
    }
}

TEST_CASE(doe_task_factory_cp201_potential_mov_descriptors_are_exact_and_never_applied)
{
    using Effect=Cp200Jne::PotentialEffect;
    static_assert(!Cp199ConcreteValue<Effect> && !Cp200SelectedEdge<Effect>);
    Cp199Fixture fixture;fixture.compare();const auto cmp=fixture.frame().tlsIndexCompare;
    const auto read=cmp->operand;const auto prefix=fixture.frame().calleePrefix;
    const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    cp200Advance(fixture);const auto jne=fixture.frame().tlsIndexJneUnresolved;
    const auto& module=jne->edges[0].potentialEffect;const auto& stack=jne->edges[1].potentialEffect;
    const std::array<std::uint8_t,6> moduleBytes{0x8b,0x05,0x01,0xa7,0x2c,0};
    const std::array<std::uint8_t,6> stackBytes{0x48,0x8b,0x5c,0x24,0x40,0};
    CHECK(module.bytes==moduleBytes && stack.bytes==stackBytes);
    CHECK(module.status==Effect::Status::UnevaluatedNotApplied && stack.status==module.status);
    CHECK(module.instructionLength==6 && module.operandWidth==32 && module.moduleOperandRva==0x2f3d28);
    CHECK(module.operand==Effect::Operand::ModuleMemory && !module.existingSpill);
    CHECK(module.rspDisplacement==0 && module.stackFromEntryS==0);
    CHECK(module.destination==Effect::Destination::EaxAndZeroExtendedRax && module.result==Effect::Result::ZeroExtendPotentialDword);
    CHECK(module.prospectiveNextRva==0x29627 && jne->edges[0].targetRva+module.instructionLength==module.prospectiveNextRva);
    // Test arithmetic verifies the encoded RIP displacement; it never supplies y.
    CHECK(module.prospectiveNextRva+0x2ca701==module.moduleOperandRva);
    CHECK(module.moduleOperandRva!=0x2f08e8);
    CHECK(stack.instructionLength==5 && stack.operandWidth==64 && stack.moduleOperandRva==0);
    CHECK(stack.operand==Effect::Operand::AuthenticatedStackSpill);
    CHECK(stack.rspDisplacement==0x40 && stack.stackFromEntryS==-0x68);
    CHECK(fixture.frame().calleeRspFromEntryS()+stack.rspDisplacement==stack.stackFromEntryS);
    CHECK(stack.destination==Effect::Destination::Rbx && stack.result==Effect::Result::TaskRbxFromSpill);
    CHECK(stack.prospectiveNextRva==0x296c5 && jne->edges[1].targetRva+stack.instructionLength==stack.prospectiveNextRva);
    CHECK(module.flagsUnchanged && module.rspUnchanged && stack.flagsUnchanged && stack.rspUnchanged);
    REQUIRE(stack.existingSpill);const auto spill=prefix->slots[7];
    CHECK(stack.existingSpill==spill && !stack.existingSpill.owner_before(spill) && !spill.owner_before(stack.existingSpill));
    CHECK(spill->instructionRva==0x2960f && spill->generation==3 && spill->width==64 && spill->fromEntryS==-0x68);
    CHECK(spill->value==Cp199Prefix::CalleeStackSlot::Value::TaskRbx && spill->task==fixture.pending);
    CHECK(Cp180Snapshot::sameWeak(spill->callOccurrence,std::weak_ptr{prefix->nestedCall->occurrence.identity()}));
    CHECK(jne->predecessor==cmp && cmp->operand==read);cp200ExactAuthority(fixture);
    const Cp180Snapshot terminal(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
    for(int retry=0;retry!=3;++retry){CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 stops before"));terminal.check();}
    Cp199Access::restoreJnePredecessor(*fixture.factory,fixture.frame());before.check();
}

TEST_CASE(doe_task_factory_cp201_every_descriptor_field_and_spill_owner_are_validated_atomically)
{
    using Effect=Cp200Jne::PotentialEffect;
    for(std::size_t edge=0;edge!=2;++edge)for(int damage=0;damage!=22;++damage) {
        Cp199Fixture fixture;fixture.compare();cp200Advance(fixture);
        Cp199Fixture other;other.compare();cp200Advance(other);
        auto record=std::make_shared<Cp200Jne>(*fixture.frame().tlsIndexJneUnresolved);
        auto& e=record->edges[edge].potentialEffect;
        switch(damage) {
        case 0:e.status=static_cast<Effect::Status>(1);break;
        case 1:++e.bytes[0];break;case 2:++e.bytes[5];break;
        case 3:++e.instructionLength;break;case 4:++e.operandWidth;break;
        case 5:e.operand=static_cast<Effect::Operand>(9);break;
        case 6:++e.moduleOperandRva;break;case 7:++e.rspDisplacement;break;
        case 8:++e.stackFromEntryS;break;case 9:e.destination=static_cast<Effect::Destination>(9);break;
        case 10:e.result=static_cast<Effect::Result>(9);break;
        case 11:e.flagsUnchanged=false;break;case 12:e.rspUnchanged=false;break;
        case 13:++e.prospectiveNextRva;break;
        case 14:e.existingSpill=other.frame().calleePrefix->slots[7];break;
        case 15:e.existingSpill=std::make_shared<const Cp199Prefix::CalleeStackSlot>(*fixture.frame().calleePrefix->slots[7]);break;
        case 16:e.existingSpill=std::shared_ptr<const Cp199Prefix::CalleeStackSlot>(fixture.frame().calleePrefix->slots[7].get(),[](auto*){});break;
        case 17:e.existingSpill=fixture.frame().calleePrefix->slots[0];break;
        case 18:if(edge==1)e.existingSpill.reset();else e.existingSpill=fixture.frame().calleePrefix->slots[7];break;
        case 19:e=record->edges[1-edge].potentialEffect;break;
        case 20:std::swap(record->edges[0].potentialEffect,record->edges[1].potentialEffect);break;
        case 21:e.existingSpill=std::shared_ptr<const Cp199Prefix::CalleeStackSlot>(fixture.frame().calleePrefix->slots[7],nullptr);break;
        }
        // Privileged replacement of BOTH handles bypasses the final private
        // issuance check, so rejection proves field/provenance validation itself.
        Cp199Access::restoreJneCheckpoint(*fixture.factory,fixture.frame(),record);
        const Cp180Snapshot before(*fixture.pending,*fixture.store,*fixture.factory,*fixture.widgets);
        for(int retry=0;retry!=2;++retry) {
            CHECK(refuses([&]{fixture.factory->createTaskBase(L("Custom"));},"CP200 requires exact unresolved JNE alternatives"));
            before.check();
        }
    }
}
