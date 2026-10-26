#include "m2m/doe_task_factory.hpp"
#include <new>

#include <map>
#include <optional>
#include <string_view>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/view_manager.hpp"

namespace m2m::doe {
engine::WidgetFontValue isolatedTaskInfoDefaultFontValue(
    const engine::WidgetFontValue& firstCurrentApplicationFont)
{
    return {firstCurrentApplicationFont.data, 0};
}

void assignIsolatedTaskInfoSecondFontValue(engine::WidgetFontValue& destination,
    const engine::WidgetFontValue& secondCurrentApplicationFont)
{
    destination.data = secondCurrentApplicationFont.data;
    destination.resolveMask = secondCurrentApplicationFont.resolveMask;
}

bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, qt::String& destination)
{
    if (!source.contains(key)) return false;
    destination = source.stringValue(key);
    return true;
}

bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, bool& destination)
{
    if (!source.contains(key)) return false;
    destination = source.boolValue(key);
    return true;
}

bool readIsolatedTaskPreference(const IsolatedTaskPreferenceSource& source,
    const qt::String& key, int& destination)
{
    if (!source.contains(key)) return false;
    destination = source.intValue(key);
    return true;
}

namespace {
struct QualifiedZeroPrefix {
    std::shared_ptr<engine::WidgetRuntimeState> widgets;
    std::shared_ptr<const engine::WidgetRuntimeState::QListSharedNullState> list;
    std::shared_ptr<const engine::WidgetRuntimeState::QStringSharedNullState> string;
    std::shared_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState> vector;
    PendingTaskPrototype::SparseQObjectPrivate* privateState;
};
std::optional<QualifiedZeroPrefix> qualifyRetainedZeroPrefix(
    const std::shared_ptr<TaskAllocationStore>& allocations, const std::optional<std::size_t>& ordinal,
    PendingTaskPrototype* pending, PendingTaskPrototype::QObjectPrefix& frame,
    const std::shared_ptr<engine::WidgetRuntimeState>& widgets)
{
    const auto list = widgets ? widgets->qlistSharedNull() : nullptr;
    const auto string = widgets ? widgets->qstringSharedNull() : nullptr;
    const auto boundList = frame.privateState ? frame.privateState->sharedNullBinding.lock() : nullptr;
    const auto boundString = frame.privateState ? frame.privateState->qstringSharedNullBinding.lock() : nullptr;
    if (!widgets || !allocations->ownsWidgetRuntime(widgets) || !ordinal ||
        allocations->retainedPrototypeAt(*ordinal) != pending || pending->kind != PendingTaskPrototype::Kind::Report ||
        pending->nativeAllocationBytes != 0xc0 || !frame.privateState ||
        frame.privateIdentity() != frame.allocatedPrivateIdentity || frame.privateState->nativeAllocationBytes != 0x88 ||
        !frame.privateState->addressPointWritten || frame.privateState->addressPointRva != 0x1a9d10 ||
        !frame.taskAddressPointWritten || frame.taskAddressPointRva != 0x23cac8 ||
        !frame.privateState->sharedNullReferenceCommitted || !boundList || boundList != list || boundList.owner_before(list) || list.owner_before(boundList) ||
        std::string_view(boundList->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        boundList->symbolRva != 0x2f0a50 || !frame.privateState->qstringSharedNullReferenceCommitted ||
        !boundString || boundString != string || boundString.owner_before(string) || string.owner_before(boundString) ||
        std::string_view(boundString->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        boundString->symbolRva != 0x2f0b30 || pending->qobjectBaseCompleted || frame.complete) return std::nullopt;
    const bool reverse = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    if (reverse ? (!frame.privateTaskBacklinkAvailable || !frame.privateTaskPublication ||
            frame.privateState->private08.has_value() || frame.privateTaskPublication->taskIdentity != pending ||
            frame.privateTaskPublication->privateIdentity != frame.allocatedPrivateIdentity) :
            (frame.privateTaskBacklinkAvailable || frame.privateTaskPublication.has_value())) return std::nullopt;
    const bool published = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateCopy ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    if (published ? (!frame.taskPrivateBacklinkAvailable ||
            frame.taskPrivateIdentity != frame.allocatedPrivateIdentity) :
            (frame.taskPrivateBacklinkAvailable || frame.taskPrivateIdentity)) return std::nullopt;
    return QualifiedZeroPrefix{widgets, list, string, widgets->qvectorSharedNull(), frame.privateState.get()};
}
} // namespace

struct TaskFactoryRuntime::Factory {
    struct Data {
        explicit Data(Factory* value) : owner(value) {}
        Factory* owner; //base7c7f70 writes data+18 to actual factory
        // Native maps are nonowning. Both are empty before prototype1 returns;
        // no unavailable TaskInfo payload or QObject is manufactured here.
        std::map<qt::String, const void*> metadata;
        std::map<qt::String, SessionTask*> tasks;
    };
    std::unique_ptr<Data> data;
};

TaskFactoryRuntime::TaskFactoryRuntime(const std::shared_ptr<TaskAllocationStore>& allocations,
    const std::shared_ptr<engine::WidgetRuntimeState>& widgets)
    : allocations_(allocations), widgets_(widgets) {}

TaskFactoryRuntime::~TaskFactoryRuntime() { releaseAdapterScope(); }

std::shared_ptr<TaskFactoryRuntime> TaskFactoryRuntime::boundedParsedDoeProjection(
    const std::shared_ptr<TaskAllocationStore>& allocations)
{
    auto runtime = std::make_shared<TaskFactoryRuntime>(allocations);
    runtime->profile_ = TaskFactoryProfile::BoundedParsedDoeProjection;
    return runtime;
}

void TaskFactoryRuntime::bindSessionReportNullCaller()
{
    const auto allocations = allocations_.lock();
    const auto widgets = widgets_.lock();
    if (!adapterScopeLive_ || profile_ != TaskFactoryProfile::Native ||
        sessionReportSourceEligible_ || sessionReportCapability_ || sessionReportFrameIssued_ ||
        sessionReportFrameBinding_ || factory_ || pendingReportAllocation_ ||
        !allocations || !allocations->adapterScopeLive() || !widgets ||
        !allocations->ownsWidgetRuntime(widgets) || allocations->retainedPrototypeCount() != 0 ||
        allocations->retainedTaskCount() != 0 || allocations->retainedSessionCount() != 0)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP196 requires pristine Session Report issuer");
    auto capability = std::shared_ptr<const SessionReportNullBinding>(
        new SessionReportNullBinding(this, allocations, widgets));
    auto tlsGrant=widgets->prepareTlsGrant(this,capability,allocations);
    auto retainedGrant=tlsGrant.grant;
    widgets->commitTlsGrant(std::move(tlsGrant));
    tlsIndexGrant_=std::move(retainedGrant);
    sessionReportCapability_ = std::move(capability);
    sessionReportIssuedCapability_ = sessionReportCapability_;
    sessionReportSourceEligible_ = true;
}

void TaskFactoryRuntime::validateSessionReportNullCaller() const
{
    const auto allocations = allocations_.lock();
    const auto widgets = widgets_.lock();
    const auto fail = [] { throw Error(M2M_E_UNSUPPORTED,
        "DOE Task factory CP196 requires exact Session Report source binding"); };
    const auto sameOwner = [](const auto& weak, const auto& strong) {
        return strong && weak.lock() == strong && !weak.owner_before(strong) && !strong.owner_before(weak);
    };
    if (!sessionReportSourceEligible_) {
        if (sessionReportCapability_ || sessionReportFrameBinding_ || sessionReportFrameIssued_ || tlsIndexGrant_ || issuedTlsIndexCompare_ || issuedTlsIndexJneUnresolved_) fail();
        return;
    }
    const auto& capability = sessionReportCapability_;
    if (!adapterScopeLive_ || profile_ != TaskFactoryProfile::Native || !allocations ||
        !allocations->adapterScopeLive() || !widgets || !allocations->ownsWidgetRuntime(widgets) ||
        !capability || !sameOwner(sessionReportIssuedCapability_, capability) || capability->receiver != this || capability->capability || capability->task ||
        capability->privateIdentity || capability->source != SessionReportNullBinding::SourceIdentity{} ||
        !sameOwner(capability->allocations, allocations) || !sameOwner(capability->widgets, widgets)) fail();
    if(!widgets->matchesTlsGrant(tlsIndexGrant_,this,capability))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 requires exact Session TLS grant");
    if (!sessionReportFrameIssued_) {
        if (sessionReportFrameBinding_ || pendingReportAllocation_ || factory_ ||
            allocations->retainedPrototypeCount() != 0) fail();
        return;
    }
    if (!factory_ || !factory_->data || !sessionReportFrameBinding_ || !pendingReportAllocation_ || *pendingReportAllocation_ != 0 ||
        allocations->retainedPrototypeCount() != 1) fail();
    const auto* pending = allocations->retainedPrototypeAt(0);
    if (!pending || !pending->qobjectPrefix || !pending->qobjectPrefix->savedEntryRdi ||
        !pending->qobjectPrefix->returnToCaller) fail();
    const auto& frame = *pending->qobjectPrefix;
    const auto& token = *frame.savedEntryRdi;
    const auto& binding = sessionReportFrameBinding_;
    if (binding->receiver != this || binding->source != SessionReportNullBinding::SourceIdentity{} ||
        binding->capability != capability || binding->capability.owner_before(capability) ||
        capability.owner_before(binding->capability) || !sameOwner(binding->allocations, allocations) ||
        !sameOwner(binding->widgets, widgets) || binding->ordinal != 0 || binding->task != pending ||
        pending->kind != PendingTaskPrototype::Kind::Report || pending->nativeAllocationBytes != 0xc0 ||
        pending->qobjectBaseCompleted || frame.complete || !frame.privateState ||
        frame.privateState->nativeAllocationBytes != 0x88 ||
        frame.privateIdentity() != frame.allocatedPrivateIdentity ||
        binding->privateIdentity != frame.allocatedPrivateIdentity ||
        !sameOwner(binding->privateOccurrence, frame.privateState->allocationOccurrence.identity()) ||
        !sameOwner(binding->initializerCallOccurrence, frame.returnToCaller->occurrence.identity()) ||
        !sameOwner(binding->frameOccurrence, frame.sourceOccurrence.identity()) ||
        !sameOwner(binding->tokenOccurrence, token.sourceOccurrence.identity()) ||
        frame.sessionReportSource != binding || frame.sessionReportSource.owner_before(binding) ||
        binding.owner_before(frame.sessionReportSource) || token.sessionReportSource != binding ||
        token.sessionReportSource.owner_before(binding) || binding.owner_before(token.sessionReportSource) ||
        token.parentIsNull != std::optional<bool>{true} ||
        token.moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        token.reportOrdinal != 0 || token.taskIdentity != pending ||
        token.privateIdentity != frame.allocatedPrivateIdentity || token.initializerEntryRva != 0x131160 ||
        token.saveRva != 0x131165 || token.incomingMoveRva != 0x1313eb || token.callRva != 0x131417 ||
        token.returnRva != 0x13141c ||
        token.origin != PendingTaskPrototype::QObjectPrefix::SavedEntryRdi::Origin::IncomingRdxToCallerRdi) fail();
}

void TaskFactoryRuntime::validateCalleePrefix(const PendingTaskPrototype::QObjectPrefix& frame) const
{
    using Phase=PendingTaskPrototype::QObjectPrefix::Phase;
    if(!frame.inCalleePrefix()) throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP198 requires callee-prefix phase");
    validateCalleePrefixHistory(frame,static_cast<std::size_t>(frame.phase)-static_cast<std::size_t>(Phase::BeforeNullParentHelperEntry));
}
void TaskFactoryRuntime::validateCalleePrefixHistory(const PendingTaskPrototype::QObjectPrefix& frame,std::size_t k) const
{
    using Prefix = PendingTaskPrototype::QObjectPrefix;
    const auto fail = [] { throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP198 requires exact callee-prefix state"); };
    const auto sameOwner = [](const auto& weak, const auto& strong) {
        return strong && weak.lock() == strong && !weak.owner_before(strong) && !strong.owner_before(weak);
    };
    constexpr std::int32_t rsp[]{0,-8,-0x10,-0x18,-0x68,-0x68,-0x70,-0x78,-0xa8,-0xa8,-0xa8};
    if(k>10 || frame.rspFromEntryS!=rsp[k]) fail();
    if (k == 0) {
        if (frame.calleePrefix || issuedCalleePrefix_) fail();
        return;
    }
    if (!frame.calleePrefix || frame.calleePrefix->completed != k) fail();
    const auto& state = *frame.calleePrefix;
    if (!state.occurrence.identity() || !frame.nullParentCall || !frame.returnToCaller) fail();
    const auto& call = state.nestedCall;
    if (k >= 6) {
        if (!call || !state.innerReturnSlot || call->sourceBinding != sessionReportFrameBinding_ ||
            call->sourceBinding.owner_before(sessionReportFrameBinding_) || sessionReportFrameBinding_.owner_before(call->sourceBinding) ||
            !sameOwner(call->callerOccurrence, frame.sourceOccurrence.identity()) ||
            !sameOwner(call->enclosingOccurrence, issuedNullParentCallOccurrence_) ||
            call->generation != 3 || call->precedingGeneration != 2 || call->callRva != 0x2acb1 ||
            call->targetRva != 0x29600 || call->returnRva != 0x2acb6 || call->callerRsp != -0x68 ||
            call->calleeRsp != -0x70 || call->slotFromEntryS != -0x70 || !call->occurrence.identity()) fail();
        const auto& slot = *state.innerReturnSlot;
        if (slot.callGeneration != 3 || slot.valueRva != 0x2acb6 || slot.slotFromEntryS != -0x70 ||
            !sameOwner(slot.callerOccurrence, frame.sourceOccurrence.identity()) ||
            !sameOwner(slot.callOccurrence, call->occurrence.identity())) fail();
    } else if (call || state.innerReturnSlot) fail();
    constexpr std::size_t producers[]{0,1,2,4,5,6,8,9};
    constexpr std::uint32_t rvas[]{0x2aca0,0x2aca2,0x2aca3,0x2aca8,0x2acb1,0x29600,0x29606,0x2960f};
    constexpr std::int32_t offsets[]{-8,-0x10,-0x18,-0x28,-0x70,-0x78,-0x88,-0x68};
    using Value = Prefix::CalleeStackSlot::Value;
    constexpr Value values[]{Value::TaskRbx,Value::CallerRsi,Value::ParentRdi,Value::MinusTwo,
        Value::NestedReturn,Value::ParentRdi,Value::MinusTwo,Value::TaskRbx};
    for (std::size_t i = 0; i != 8; ++i) {
        const auto& slot = state.slots[i];
        if (k <= producers[i]) { if (slot) fail(); continue; }
        const auto generation = i < 4 ? 2u : 3u;
        const auto occurrence = i < 4 ? issuedNullParentCallOccurrence_ : call->occurrence.identity();
        const auto number = values[i] == Value::CallerRsi ? *frame.callerRsi :
            values[i] == Value::MinusTwo ? UINT64_C(0xfffffffffffffffe) : values[i] == Value::NestedReturn ? UINT64_C(0x2acb6) : 0;
        if (!slot || slot->generation != generation || slot->width != 64 || slot->instructionRva != rvas[i] ||
            slot->fromEntryS != offsets[i] || slot->value != values[i] || slot->numeric != number ||
            slot->task != (values[i] == Value::TaskRbx ? frame.restoredTaskRbxIdentity : nullptr) ||
            slot->parent != (values[i] == Value::ParentRdi ? frame.restoredRdiToken : nullptr) ||
            !sameOwner(slot->callerOccurrence, frame.sourceOccurrence.identity()) ||
            !sameOwner(slot->callOccurrence, occurrence) || !slot->occurrence.identity()) fail();
    }
    const auto flagsMatch = [&](const auto& flags, bool inner) {
        using Function = Prefix::StackSubFlags::Function;
        constexpr std::array expected{Function::CarryOfSub64,Function::EvenParityOfSubLowByte,
            Function::AuxiliaryBorrowOfSub64,Function::ZeroOfSub64,Function::SignOfSub64,Function::OverflowOfSub64};
        return flags && flags->width == 64 && flags->producerRva == (inner ? 0x29602u : 0x2aca4u) &&
            flags->operandFromEntryS == (inner ? -0x78 : -0x18) && flags->resultFromEntryS == (inner ? -0xa8 : -0x68) &&
            flags->immediate == (inner ? 0x30u : 0x50u) && flags->arithmetic == expected &&
            sameOwner(flags->stackOrigin, frame.sourceOccurrence.identity()) && flags->occurrence.identity();
    };
    if (k >= 4 ? !flagsMatch(state.firstSub, false) : static_cast<bool>(state.firstSub)) fail();
    if (k >= 8 ? !flagsMatch(state.secondSub, true) : static_cast<bool>(state.secondSub)) fail();
    const auto current = k >= 8 ? state.secondSub : k >= 4 ? state.firstSub : nullptr;
    if (state.currentSub != current || state.currentSub.owner_before(current) || current.owner_before(state.currentSub)) fail();
    if (frame.calleePrefix != issuedCalleePrefix_ || frame.calleePrefix.owner_before(issuedCalleePrefix_) ||
        issuedCalleePrefix_.owner_before(frame.calleePrefix))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP198 requires issued callee-prefix occurrence");
}

void TaskFactoryRuntime::advanceCalleePrefix(PendingTaskPrototype::QObjectPrefix& frame)
{
    using Prefix = PendingTaskPrototype::QObjectPrefix;
    using Phase = Prefix::Phase;
    const char* stops[]{"0x2aca0","0x2aca2","0x2aca3","0x2aca4","0x2aca8","0x2acb1",
        "0x29600","0x29602","0x29606","0x2960f","0x29614"};
    for (;;) {
        validateSessionReportNullCaller();
        validateCalleePrefix(frame);
        const auto k = static_cast<std::size_t>(frame.phase) - static_cast<std::size_t>(Phase::BeforeNullParentHelperEntry);
        if(k==10 && calleePrefixStopOrdinal_==10 && tlsIndexCompareEnabled_) advanceTlsIndexCompare(frame);
        if (k == 10 || k >= calleePrefixStopOrdinal_)
            throw Error(M2M_E_UNSUPPORTED, std::string("DOE Task factory CP198 stops at QtCore RVA ") + stops[k] +
                " (structural prefix; TLS/global read/result unavailable)");
        Prefix::CalleePrefixState next = frame.calleePrefix ? *frame.calleePrefix : Prefix::CalleePrefixState{};
        next.completed = k + 1;
        const auto write = [&](std::size_t index, std::uint32_t rva, std::int32_t offset,
            Prefix::CalleeStackSlot::Value value, std::uint64_t numeric = 0) {
            Prefix::CalleeStackSlot slot;
            slot.callerOccurrence = frame.sourceOccurrence.identity();
            slot.callOccurrence = index < 4 ? issuedNullParentCallOccurrence_ : next.nestedCall->occurrence.identity();
            slot.generation = index < 4 ? 2 : 3; slot.instructionRva = rva; slot.fromEntryS = offset;
            slot.value = value; slot.numeric = numeric;
            if (value == Prefix::CalleeStackSlot::Value::TaskRbx) slot.task = frame.restoredTaskRbxIdentity;
            if (value == Prefix::CalleeStackSlot::Value::ParentRdi) slot.parent = frame.restoredRdiToken;
            next.slots[index] = std::make_shared<const Prefix::CalleeStackSlot>(std::move(slot));
        };
        const auto subtract = [&](bool inner) {
            Prefix::StackSubFlags flags;
            flags.stackOrigin = frame.sourceOccurrence.identity(); flags.producerRva = inner ? 0x29602 : 0x2aca4;
            flags.operandFromEntryS = inner ? -0x78 : -0x18; flags.resultFromEntryS = inner ? -0xa8 : -0x68;
            flags.immediate = inner ? 0x30 : 0x50;
            auto prepared = std::make_shared<const Prefix::StackSubFlags>(std::move(flags));
            if (inner) next.secondSub = prepared; else next.firstSub = prepared;
            next.currentSub = std::move(prepared);
        };
        using Value = Prefix::CalleeStackSlot::Value;
        switch (k) {
        case 0: write(0,0x2aca0,-8,Value::TaskRbx); break;
        case 1: write(1,0x2aca2,-0x10,Value::CallerRsi,*frame.callerRsi); break;
        case 2: write(2,0x2aca3,-0x18,Value::ParentRdi); break;
        case 3: subtract(false); break;
        case 4: write(3,0x2aca8,-0x28,Value::MinusTwo,UINT64_C(0xfffffffffffffffe)); break;
        case 5: {
            Prefix::NestedInitializerCall call;
            call.sourceBinding = sessionReportFrameBinding_; call.callerOccurrence = frame.sourceOccurrence.identity();
            call.enclosingOccurrence = issuedNullParentCallOccurrence_;
            next.nestedCall = std::make_shared<const Prefix::NestedInitializerCall>(std::move(call));
            next.innerReturnSlot = Prefix::LiveReturnSlot{3,0x2acb6,-0x70,
                frame.sourceOccurrence.identity(),next.nestedCall->occurrence.identity()};
            write(4,0x2acb1,-0x70,Value::NestedReturn,0x2acb6); break;
        }
        case 6: write(5,0x29600,-0x78,Value::ParentRdi); break;
        case 7: subtract(true); break;
        case 8: write(6,0x29606,-0x88,Value::MinusTwo,UINT64_C(0xfffffffffffffffe)); break;
        case 9: write(7,0x2960f,-0x68,Value::TaskRbx); break;
        }
        // Every allocation above precedes these nonthrowing publications.
        auto prepared = std::make_shared<const Prefix::CalleePrefixState>(std::move(next));
        issuedCalleePrefix_ = prepared; frame.calleePrefix = std::move(prepared);
        frame.phase = static_cast<Phase>(static_cast<std::size_t>(Phase::BeforeNullParentHelperEntry) + k + 1);
        frame.rspFromEntryS = frame.calleeRspFromEntryS();
    }
}

engine::TlsIndexReadExpression::Context TaskFactoryRuntime::tlsReadContext(const PendingTaskPrototype::QObjectPrefix& frame) const
{
    engine::TlsIndexReadExpression::Context context;
    context.sourceBinding=frame.sessionReportSource;context.frame=frame.sourceOccurrence.identity();
    context.outerCall=frame.nullParentCall->occurrence.identity();context.innerCall=frame.calleePrefix->nestedCall->occurrence.identity();
    context.task=frame.restoredTaskRbxIdentity;context.privateIdentity=frame.allocatedPrivateIdentity;
    return context;
}
void TaskFactoryRuntime::advanceTlsIndexCompare(PendingTaskPrototype::QObjectPrefix& frame)
{
    using Prefix=PendingTaskPrototype::QObjectPrefix;
    validateSessionReportNullCaller();validateCalleePrefix(frame);
    if(frame.phase!=Prefix::Phase::BeforeTlsIndexCompare || frame.tlsIndexCompare || issuedTlsIndexCompare_)
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 requires pristine CMP predecessor");
    const auto widgets=widgets_.lock();
    auto read=widgets->prepareTlsRead(tlsIndexGrant_,frame.sessionReportSource,tlsReadContext(frame));
    if(failTlsComparePreparation_) throw std::bad_alloc();
    Prefix::TlsIndexCompare comparison;comparison.operand=read;comparison.predecessor=frame.calleePrefix;
    auto prepared=std::make_shared<const Prefix::TlsIndexCompare>(std::move(comparison));
    // Root read, factory receipt, current flags and phase publish without allocation.
    widgets->commitTlsRead(std::move(read));issuedTlsIndexCompare_=prepared;frame.tlsIndexCompare=std::move(prepared);
    frame.phase=Prefix::Phase::BeforeTlsIndexJne;
    throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 stops at QtCore RVA 0x2961b (unknown CMP; JNE unavailable)");
}
void TaskFactoryRuntime::validateTlsIndexCompare(const PendingTaskPrototype::QObjectPrefix& frame) const
{
    using Prefix=PendingTaskPrototype::QObjectPrefix;
    if(frame.phase!=Prefix::Phase::BeforeTlsIndexJne)
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 requires CMP successor phase");
    validateTlsIndexCompareHistory(frame);
}
void TaskFactoryRuntime::validateTlsIndexCompareHistory(const PendingTaskPrototype::QObjectPrefix& frame) const
{
    using Prefix=PendingTaskPrototype::QObjectPrefix;
    validateCalleePrefixHistory(frame,10);
    const auto widgets=widgets_.lock();const auto& cmp=frame.tlsIndexCompare;
    using Function=Prefix::TlsIndexCompare::Function;
    constexpr std::array functions{Function::UnsignedBorrow32,Function::EvenParityOfResultLowByte,Function::AuxiliaryBorrow32,
        Function::ZeroOfResult32,Function::SignOfResult32,Function::SignedOverflow32};
    if(!cmp || cmp->width!=32 || cmp->producerRva!=0x29614 || cmp->immediate!=0xffffffffu ||
        cmp->encodedImmediateWidth!=8 || !cmp->signExtended || cmp->result!=Prefix::TlsIndexCompare::Result::SubtractModulo32 ||
        cmp->arithmetic!=functions || !cmp->occurrence.identity() || !widgets ||
        cmp->predecessor.lock()!=issuedCalleePrefix_ || cmp->predecessor.owner_before(issuedCalleePrefix_) ||
        issuedCalleePrefix_.owner_before(cmp->predecessor) ||
        !widgets->matchesTlsRead(tlsIndexGrant_,cmp->operand,frame.sessionReportSource,tlsReadContext(frame)))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 requires exact symbolic TLS CMP state");
    if(cmp!=issuedTlsIndexCompare_ || cmp.owner_before(issuedTlsIndexCompare_) || issuedTlsIndexCompare_.owner_before(cmp))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 requires issued symbolic CMP occurrence");
}

void TaskFactoryRuntime::advanceTlsIndexJneUnresolved(PendingTaskPrototype::QObjectPrefix& frame)
{
    using Prefix=PendingTaskPrototype::QObjectPrefix;
    validateSessionReportNullCaller();validateTlsIndexCompare(frame);
    if(frame.tlsIndexJneUnresolved || issuedTlsIndexJneUnresolved_)
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 requires pristine unresolved JNE predecessor");
    Prefix::TlsIndexJneUnresolved record;record.predecessor=frame.tlsIndexCompare;
    record.edges[1].potentialEffect.existingSpill=frame.calleePrefix->slots[7];
    auto prepared=std::make_shared<const Prefix::TlsIndexJneUnresolved>(std::move(record));
    const Error terminal(M2M_E_UNSUPPORTED,"DOE Task factory CP200 stops before QtCore RVA 0x2961b (unresolved JNE alternatives; no edge selected)");
    if(failTlsJnePreparation_) throw std::bad_alloc();
    // Serialized adapter publication: no read, flags, register, stack or edge evaluation.
    issuedTlsIndexJneUnresolved_=prepared;
    frame.tlsIndexJneUnresolved=std::move(prepared);
    frame.phase=Prefix::Phase::TlsIndexJneUnresolved;
    throw terminal;
}
void TaskFactoryRuntime::validateTlsIndexJneUnresolved(const PendingTaskPrototype::QObjectPrefix& frame) const
{
    using Prefix=PendingTaskPrototype::QObjectPrefix;
    using Jne=Prefix::TlsIndexJneUnresolved;
    const auto& record=frame.tlsIndexJneUnresolved;
    if(frame.phase!=Prefix::Phase::TlsIndexJneUnresolved || !record || !issuedTlsIndexJneUnresolved_)
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 requires issued unresolved JNE phase");
    validateTlsIndexCompareHistory(frame);
    const auto sameOwner=[](const auto& a,const auto& b){return a==b && !a.owner_before(b) && !b.owner_before(a);};
    constexpr std::array<std::uint8_t,6> bytes{0x0f,0x85,0x9f,0x00,0x00,0x00};
    std::array<Jne::Edge,2> edges{{{Jne::Guard::ZeroFlagSet,0x29621,Jne::potentialModuleMov()},
        {Jne::Guard::ZeroFlagClear,0x296c0,Jne::potentialSpillMov()}}};
    edges[1].potentialEffect.existingSpill=frame.calleePrefix->slots[7];
    if(!record->occurrence.identity() || !sameOwner(record->predecessor,frame.tlsIndexCompare) ||
        !sameOwner(record->predecessor,issuedTlsIndexCompare_) || record->moduleSha256!=Prefix::moduleSha256 ||
        record->producerRva!=0x2961b || record->bytes!=bytes || record->instructionLength!=6 || record->flagsIndex!=3 ||
        record->status!=Jne::Status::UnresolvedBeforeExecution || record->edges!=edges ||
        !sameOwner(record->edges[0].potentialEffect.existingSpill,edges[0].potentialEffect.existingSpill) ||
        !sameOwner(record->edges[1].potentialEffect.existingSpill,frame.calleePrefix->slots[7]))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 requires exact unresolved JNE alternatives");
    if(!sameOwner(record,issuedTlsIndexJneUnresolved_))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 requires issued unresolved JNE occurrence");
}

void TaskFactoryRuntime::releaseAdapterScope() noexcept
{
    if (!adapterScopeLive_) return;
    adapterScopeLive_ = false;
    if(const auto widgets=widgets_.lock()) widgets->revokeTlsGrant(tlsIndexGrant_);
    tlsIndexGrant_.reset(); issuedTlsIndexCompare_.reset(); issuedTlsIndexJneUnresolved_.reset();
    sessionReportCapability_.reset();
    issuedNullParentCallOccurrence_.reset();
    issuedCalleePrefix_.reset();
    factory_.reset();
    pendingReportAllocation_.reset();
    // The actual allocation store owns pending Task allocations. Destroying a
    // factory is not an invented native Task deletion; Session releases its
    // adapter allocation scope independently at the fresh-process boundary.
}

TaskFactoryConstructionSnapshot TaskFactoryRuntime::construction() const noexcept
{
    TaskFactoryConstructionSnapshot result;
    result.factoryCommitted = factory_ != nullptr;
    result.dataCommitted = factory_ && factory_->data;
    if (result.dataCommitted) {
        result.dataFactoryBacklinkMatches = factory_->data->owner == factory_.get();
        result.metadataCacheCount = factory_->data->metadata.size();
        result.taskCacheCount = factory_->data->tasks.size();
    }
    result.pendingReportAllocation = pendingReportAllocation_;
    return result;
}

void TaskFactoryRuntime::initializeNativePrefix()
{
    if (privateZeroBatchStopOrdinal_ < 1 || privateZeroBatchStopOrdinal_ > 6)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP180 invalid internal stop ordinal");
    if (calleePrefixStopOrdinal_ > 10)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP198 invalid callee-prefix stop ordinal");
    if (privateFlags20StopOrdinal_ > 18)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP182 invalid internal stop ordinal");
    validateSessionReportNullCaller();
    if (!factory_) {
        factory_ = std::make_unique<Factory>(); //01d960 commits mainwindow+4a0
        factory_->data = std::make_unique<Factory::Data>(factory_.get()); //before137610
    }
    if (!pendingReportAllocation_) {
        const auto allocations = allocations_.lock();
        if (!allocations || !allocations->adapterScopeLive())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory allocation owner unavailable before first Report allocation");
        const auto index = allocations->retainedPrototypeCount();
        allocations->allocatePendingReportPrototype(); //137610 allocates0xc0
        pendingReportAllocation_ = index;
    }
    const auto allocations = allocations_.lock();
    if (!allocations || !allocations->adapterScopeLive())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task allocation owner unavailable during QObject prefix");
    auto* pending = allocations->retainedPrototypeAt(*pendingReportAllocation_);
    if (!pending)
        throw Error(M2M_E_UNSUPPORTED, "DOE pending Report allocation disappeared during QObject prefix");
    if (!pending->qobjectPrefix) {
        // Normal non-null private allocation path only. Prepare the complete
        // adapter checkpoint before publishing it, so host allocation failure
        // cannot leave an advanced cursor without its separately owned state.
        PendingTaskPrototype::QObjectPrefix frame;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::QObjectAddressPoint;
        frame.taskAddressPointWritten = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::PrivateAllocation;
        frame.privateState = std::make_unique<PendingTaskPrototype::SparseQObjectPrivate>();
        frame.allocatedPrivateIdentity = frame.privateState.get();
        // QtCore public QObject RVA 0x13140f loads EDX after this non-null
        // allocation; RVA 0x131417 calls the private initializer. Its retained
        // prefix has no EDX write or intervening call before RVA 0x1311f1.
        frame.privateInitializerVersion = PendingTaskPrototype::QObjectPrefix::PrivateInitializerVersion{
            PendingTaskPrototype::QObjectPrefix::moduleSha256, 0x40804, 0x13140f, 0x131417};
        frame.savedTaskRbx = PendingTaskPrototype::QObjectPrefix::SavedTaskRbx{
            allocations, *pendingReportAllocation_, pending, frame.allocatedPrivateIdentity,
            PendingTaskPrototype::QObjectPrefix::moduleSha256};
        frame.savedEntryRdi = PendingTaskPrototype::QObjectPrefix::SavedEntryRdi{
            allocations, *pendingReportAllocation_, pending, frame.allocatedPrivateIdentity,
            PendingTaskPrototype::QObjectPrefix::moduleSha256};
        frame.returnToCaller = PendingTaskPrototype::QObjectPrefix::ReturnToCaller{
            allocations, *pendingReportAllocation_, pending, frame.allocatedPrivateIdentity,
            PendingTaskPrototype::QObjectPrefix::moduleSha256};
        frame.privateState->addressPointWritten = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::PrivateAddressPoint;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList;
        std::shared_ptr<const SessionReportNullBinding> sourceBinding;
        if (sessionReportSourceEligible_) {
            if (sessionReportFrameIssued_ || sessionReportFrameBinding_ || *pendingReportAllocation_ != 0 ||
                allocations->retainedPrototypeCount() != 1 || pending->kind != PendingTaskPrototype::Kind::Report ||
                pending->nativeAllocationBytes != 0xc0 || pending->qobjectBaseCompleted ||
                frame.privateState->nativeAllocationBytes != 0x88 || frame.complete)
                throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP196 refuses Report source reissuance");
            sourceBinding = std::shared_ptr<const SessionReportNullBinding>(new SessionReportNullBinding(
                this, allocations, widgets_.lock(), sessionReportCapability_, pending,
                frame.allocatedPrivateIdentity, frame.sourceOccurrence.identity(),
                frame.savedEntryRdi->sourceOccurrence.identity(),
                frame.privateState->allocationOccurrence.identity(),
                frame.returnToCaller->occurrence.identity()));
            frame.savedEntryRdi->parentIsNull = true;
            frame.savedEntryRdi->sessionReportSource = sourceBinding;
            frame.sessionReportSource = sourceBinding;
        }
        // All allocations/qualification preparation precede the nonthrowing
        // publication. A retained issued occurrence can never enter this block.
        pending->qobjectPrefix = std::move(frame);
        if (sessionReportSourceEligible_) {
            sessionReportFrameBinding_ = std::move(sourceBinding);
            sessionReportFrameIssued_ = true;
        }
    }
    if (sessionReportSourceEligible_) validateSessionReportNullCaller();
    else if (pending->qobjectPrefix->sessionReportSource ||
        (pending->qobjectPrefix->savedEntryRdi && pending->qobjectPrefix->savedEntryRdi->sessionReportSource))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP196 forbids foreign source binding");
    auto& frame = *pending->qobjectPrefix;
    // CP193 current private+8 is either its earlier numeric zero or the
    // qualified task publication. Refuse inconsistent states before any write.
    const bool reversePublished = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    // These historical phases already reject a bare reverse marker in their
    // complete prewrite validator. Preserve those phase-specific diagnostics;
    // a value-bearing CP193 record is always rejected here when premature.
    const bool legacyReverseMarkerGuard =
        frame.phase >= PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull &&
        frame.phase <= PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink;
    const auto reverseOwner = frame.privateTaskPublication ? frame.privateTaskPublication->allocationOwner.lock() : nullptr;
    if (reversePublished ? (!frame.privateTaskBacklinkAvailable || !frame.privateTaskPublication ||
            !frame.privateState || frame.privateState->private08.has_value() ||
            !reverseOwner || reverseOwner != allocations || reverseOwner.owner_before(allocations) ||
            allocations.owner_before(reverseOwner) ||
            frame.privateTaskPublication->reportOrdinal != *pendingReportAllocation_ ||
            frame.privateTaskPublication->taskIdentity != pending ||
            !frame.allocatedPrivateIdentity || frame.privateTaskPublication->privateIdentity != frame.allocatedPrivateIdentity ||
            frame.privateTaskPublication->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            frame.privateTaskPublication->instructionRva != 0x131431 || frame.privateTaskPublication->destinationOffset != 0x08) :
            (frame.privateTaskPublication.has_value() ||
             (frame.privateTaskBacklinkAvailable && !legacyReverseMarkerGuard)))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP193 requires phase-qualified private+8 publication");
    // Refuse premature publication before even an earlier prefix can write.
    // Terminal reentry must carry exactly the allocation-bound task+8 value.
    const bool taskPrivatePublished = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateCopy ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    if (taskPrivatePublished ? (!frame.taskPrivateBacklinkAvailable ||
            !frame.allocatedPrivateIdentity || frame.taskPrivateIdentity != frame.allocatedPrivateIdentity) :
            (frame.taskPrivateBacklinkAvailable || frame.taskPrivateIdentity))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP190 requires phase-qualified task+8 publication");
    // Refuse missing/foreign terminal R12 and premature observations before
    // any prefix can mutate. The cursor is derived exclusively from phase.
    const bool privateCopied = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    if (privateCopied ? (!frame.allocatedPrivateIdentity ||
            frame.r12PrivateIdentity != frame.allocatedPrivateIdentity) : frame.r12PrivateIdentity != nullptr)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP191 requires phase-qualified R12 identity");
    const bool spilled = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    const auto spillOwner = frame.callerPrivateSpill ? frame.callerPrivateSpill->allocationOwner.lock() : nullptr;
    if (spilled ? (!frame.callerPrivateSpill || !spillOwner || spillOwner != allocations ||
            spillOwner.owner_before(allocations) || allocations.owner_before(spillOwner) ||
            frame.callerPrivateSpill->reportOrdinal != *pendingReportAllocation_ ||
            frame.callerPrivateSpill->taskIdentity != pending ||
            frame.callerPrivateSpill->privateIdentity != frame.allocatedPrivateIdentity ||
            frame.callerPrivateSpill->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            frame.callerPrivateSpill->initializerEntryRva != 0x131160 ||
            frame.callerPrivateSpill->callRva != 0x131417 || frame.callerPrivateSpill->returnRva != 0x13141c ||
            frame.callerPrivateSpill->instructionRva != 0x13142c ||
            frame.callerPrivateSpill->slotFromEntryS != 0x60 || frame.callerPrivateSpill->slotFromRsp != 0x58) :
            frame.callerPrivateSpill.has_value())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP192 requires phase-qualified caller spill");
    if (frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateEpilogue &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateStackAdjustment &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateRdiRestore &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateReturn &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateCopy &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad &&
        !frame.inCalleeOrTlsIndexBoundary() &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerJump &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerXor &&
        frame.returnPrivateIdentity)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP183 forbids premature return identity");
    if (frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateStackAdjustment &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateRdiRestore &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateReturn &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateCopy &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad &&
        !frame.inCalleeOrTlsIndexBoundary() &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerJump &&
        frame.phase != PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerXor &&
        frame.restoredTaskRbxIdentity)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP184 forbids premature restored RBX");
    // Check retained symbolic prologue provenance before any resumed write.
    // Shared ownership is qualified independently of opaque pointer equality.
    const auto savedOwner = frame.savedTaskRbx ? frame.savedTaskRbx->allocationOwner.lock() : nullptr;
    if (!frame.savedTaskRbx || !savedOwner || savedOwner != allocations ||
        savedOwner.owner_before(allocations) || allocations.owner_before(savedOwner) ||
        frame.savedTaskRbx->reportOrdinal != *pendingReportAllocation_ ||
        frame.savedTaskRbx->taskIdentity != pending ||
        frame.savedTaskRbx->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        frame.savedTaskRbx->initializerEntryRva != 0x131160 ||
        frame.savedTaskRbx->callRva != 0x131417 || frame.savedTaskRbx->returnRva != 0x13141c ||
        frame.savedTaskRbx->rspFromEntryS != -0x38 ||
        frame.savedTaskRbx->saveSlotFromEntryS != 0x10 || frame.savedTaskRbx->saveSlotFromRsp != 0x48 ||
        (frame.privateState && frame.privateIdentity() == frame.allocatedPrivateIdentity &&
            frame.savedTaskRbx->privateIdentity != frame.allocatedPrivateIdentity))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP184 requires qualified saved RBX stack relation");
    const bool callerXorCompleted = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeTaskPrivateBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateCopy ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerPrivateSpill ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateTaskBacklink ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentTest ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    const bool parentTested = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentJump ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall ||
        frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad ||
        frame.inCalleeOrTlsIndexBoundary();
    using ParentBranchPath = PendingTaskPrototype::QObjectPrefix::ParentBranchPath;
    const bool helperEntered = frame.inCalleeOrTlsIndexBoundary();
    const bool nullBranch = helperEntered || frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerNullParentCall;
    const bool nonnullBranch = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerParentLoad;
    if ((nullBranch || nonnullBranch) ? (!frame.savedEntryRdi ||
            frame.savedEntryRdi->parentIsNull != std::optional<bool>{nullBranch} ||
            frame.parentBranchPath != std::optional<ParentBranchPath>{nullBranch ?
                ParentBranchPath::TakenNullParent : ParentBranchPath::FallthroughNonnullParent}) :
            frame.parentBranchPath.has_value())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP195 requires phase-qualified parent JE path");
    if (parentTested ? (!frame.savedEntryRdi ||
            frame.parentTestOutcome != std::optional<PendingTaskPrototype::QObjectPrefix::ParentTestOutcome>{
                PendingTaskPrototype::QObjectPrefix::ParentTestOutcome::test(&*frame.savedEntryRdi)} ||
            frame.callerArithmeticFlags.has_value()) : frame.parentTestOutcome.has_value())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP194 requires phase-qualified parent TEST flags");
    if (callerXorCompleted ?
            (frame.callerRsi != std::optional<std::uint64_t>{0} ||
             (!parentTested && frame.callerArithmeticFlags != std::optional<PendingTaskPrototype::QObjectPrefix::CallerArithmeticFlags>{
                 PendingTaskPrototype::QObjectPrefix::CallerArithmeticFlags{}})) :
            (frame.callerRsi.has_value() || frame.callerArithmeticFlags.has_value()))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP188 requires exact caller XOR state");
    const bool returned = callerXorCompleted || frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeCallerXor;
    const bool rdiRestored = returned || frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateReturn;
    if (!rdiRestored && frame.restoredRdiToken)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP186 forbids premature restored RDI");
    const auto rdiOwner = frame.savedEntryRdi ? frame.savedEntryRdi->allocationOwner.lock() : nullptr;
    if (!frame.savedEntryRdi || !rdiOwner || rdiOwner != allocations ||
        rdiOwner.owner_before(allocations) || allocations.owner_before(rdiOwner) ||
        frame.savedEntryRdi->reportOrdinal != *pendingReportAllocation_ ||
        frame.savedEntryRdi->taskIdentity != pending ||
        // Preserve earlier phase-specific allocation-identity diagnostics, as
        // with saved RBX. Admitted flags/epilogue cuts enforce this strictly.
        (frame.privateState && frame.privateIdentity() == frame.allocatedPrivateIdentity &&
            frame.savedEntryRdi->privateIdentity != frame.allocatedPrivateIdentity) ||
        frame.savedEntryRdi->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        frame.savedEntryRdi->initializerEntryRva != 0x131160 || frame.savedEntryRdi->saveRva != 0x131165 ||
        frame.savedEntryRdi->origin != PendingTaskPrototype::QObjectPrefix::SavedEntryRdi::Origin::IncomingRdxToCallerRdi ||
        frame.savedEntryRdi->incomingMoveRva != 0x1313eb ||
        frame.savedEntryRdi->callRva != 0x131417 || frame.savedEntryRdi->returnRva != 0x13141c ||
        (rdiRestored && (frame.restoredRdiToken != &*frame.savedEntryRdi ||
            frame.restoredRdiParentIsNull != frame.savedEntryRdi->parentIsNull)) ||
        (!rdiRestored && frame.restoredRdiParentIsNull.has_value()))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP186 requires qualified saved entry RDI token");
    // CP185 has no concrete RFLAGS model. ADD changes arithmetic flags, but
    // flags20Eax/private20 are historical CP182 expressions and stay untouched.
    const bool stackAdjusted = frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateRdiRestore;
    if (frame.rspFromEntryS != (helperEntered ? frame.calleeRspFromEntryS() : returned ? 8 : rdiRestored ? 0 : stackAdjusted ? -8 : -0x38) ||
        frame.savedRdiSlotFromEntryS != -8 || frame.returnSlotFromEntryS != 0)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP185 requires qualified symbolic stack relation");
    const auto returnOwner = frame.returnToCaller ? frame.returnToCaller->allocationOwner.lock() : nullptr;
    if (!frame.returnToCaller || !returnOwner || returnOwner != allocations ||
        returnOwner.owner_before(allocations) || allocations.owner_before(returnOwner) ||
        frame.returnToCaller->reportOrdinal != *pendingReportAllocation_ ||
        frame.returnToCaller->taskIdentity != pending ||
        (frame.privateState && frame.privateIdentity() == frame.allocatedPrivateIdentity &&
            frame.returnToCaller->privateIdentity != frame.allocatedPrivateIdentity) ||
        frame.returnToCaller->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
        frame.returnToCaller->initializerEntryRva != 0x131160 ||
        frame.returnToCaller->callRva != 0x131417 || frame.returnToCaller->targetRva != 0x13141c ||
        frame.returnToCaller->slotFromEntryS != 0)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP187 requires qualified symbolic return slot");
    // Retained initializer history is a distinct generation even after RET.
    // The new CALL may replace the live [S] value, never this historical tuple.
    if (frame.returnToCaller->callGeneration != 1 || !frame.returnToCaller->occurrence.identity())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP197 requires initializer CALL generation");
    const auto sameCallOwner = [](const auto& weak, const auto& strong) {
        return strong && weak.lock() == strong && !weak.owner_before(strong) && !strong.owner_before(weak);
    };
    if (!helperEntered) {
        if (frame.nullParentCall || frame.helperReturnSlot || issuedNullParentCallOccurrence_)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP197 forbids premature null-parent CALL state");
    } else {
        const auto& call = frame.nullParentCall;
        const auto& slot = frame.helperReturnSlot;
        if (!sessionReportSourceEligible_ || !frame.sessionReportSource || !call || !slot ||
            call->sourceBinding != sessionReportFrameBinding_ ||
            call->sourceBinding.owner_before(sessionReportFrameBinding_) ||
            sessionReportFrameBinding_.owner_before(call->sourceBinding) ||
            !sameCallOwner(call->callerOccurrence, frame.sourceOccurrence.identity()) ||
            !sameCallOwner(call->initializerOccurrence, frame.returnToCaller->occurrence.identity()) ||
            call->callGeneration != 2 || call->precedingGeneration != 1 ||
            call->callRva != 0x131449 || call->targetRva != 0x2aca0 || call->returnRva != 0x13144e ||
            call->callerRspFromEntryS != 8 || call->calleeRspFromEntryS != 0 || call->slotFromEntryS != 0 ||
            !call->occurrence.identity() || slot->callGeneration != 2 || slot->valueRva != 0x13144e ||
            slot->slotFromEntryS != 0 || !sameCallOwner(slot->callerOccurrence, frame.sourceOccurrence.identity()) ||
            !sameCallOwner(slot->callOccurrence, call->occurrence.identity()))
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP197 requires exact null-parent CALL and live return slot");
        if (!sameCallOwner(std::weak_ptr{call->occurrence.identity()}, issuedNullParentCallOccurrence_))
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP197 requires issued null-parent CALL occurrence");
    }
    const auto cmpRoot=widgets_.lock();
    if(!frame.atTlsIndexBoundary() &&
        (frame.tlsIndexCompare || issuedTlsIndexCompare_ || (sessionReportSourceEligible_ && cmpRoot && cmpRoot->tlsRead_)))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 forbids premature TLS CMP state");
    if(frame.phase==PendingTaskPrototype::QObjectPrefix::Phase::TlsIndexJneUnresolved ?
        (!frame.tlsIndexJneUnresolved || !issuedTlsIndexJneUnresolved_) :
        (bool(frame.tlsIndexJneUnresolved) || bool(issuedTlsIndexJneUnresolved_)))
        throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 requires consistent unresolved JNE phase and issuance");
    if (!helperEntered && (frame.calleePrefix || issuedCalleePrefix_))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP198 forbids premature callee-prefix state");
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeFirstSharedQList) {
        if (!frame.privateState || frame.privateIdentity() != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory QList shared-null binding requires allocation-time private identity at RVA 0x131185");
        const auto widgets = widgets_.lock();
        if (!widgets || !allocations->ownsWidgetRuntime(widgets))
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory QList shared-null binding requires its live matching root WidgetRuntimeState at RVA 0x131185");
        const auto rootIdentity = widgets->qlistSharedNull();
        if (!rootIdentity || std::string(rootIdentity->moduleSha256) !=
                PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            rootIdentity->symbolRva != 0x2f0a50)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory QList shared-null root identity is unqualified");
        frame.privateState->sharedNullBinding = rootIdentity; // native private+0x18 store
        widgets->commitQListSharedNullReference(); // symbolic LOCK ADD +1, modulo 2^32
        frame.privateState->sharedNullReferenceCommitted = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateFinalAddressPoint;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateFinalAddressPoint) {
        const auto widgets = widgets_.lock();
        const auto boundRoot = frame.privateState ? frame.privateState->sharedNullBinding.lock() : nullptr;
        if (!widgets || !frame.privateState || !allocations->ownsWidgetRuntime(widgets) ||
            !frame.privateState->sharedNullReferenceCommitted || !boundRoot ||
            boundRoot != widgets->qlistSharedNull() ||
            std::string(boundRoot->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            boundRoot->symbolRva != 0x2f0a50 || frame.privateState->addressPointRva != 0x1a9a30 ||
            pending->qobjectBaseCompleted || frame.complete ||
            frame.privateIdentity() != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory QObjectPrivate final address point requires its committed matching QList root at RVA 0x131198");
        frame.privateState->addressPointRva = 0x1a9d10;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQStringSharedNull) {
        const auto widgets = widgets_.lock();
        const auto boundList = frame.privateState ? frame.privateState->sharedNullBinding.lock() : nullptr;
        const auto rootList = widgets ? widgets->qlistSharedNull() : nullptr;
        const auto rootString = widgets ? widgets->qstringSharedNull() : nullptr;
        // Validate the retained CP165 frame and the allocation store's exact
        // owner before publishing either the new binding or its symbolic add.
        if (!widgets || !allocations->ownsWidgetRuntime(widgets) ||
            !pendingReportAllocation_ ||
            allocations->retainedPrototypeAt(*pendingReportAllocation_) != pending ||
            pending->kind != PendingTaskPrototype::Kind::Report ||
            pending->nativeAllocationBytes != 0xc0 || !frame.privateState ||
            frame.privateIdentity() != frame.allocatedPrivateIdentity ||
            frame.privateState->nativeAllocationBytes != 0x88 ||
            !frame.privateState->addressPointWritten ||
            frame.privateState->addressPointRva != 0x1a9d10 ||
            !frame.taskAddressPointWritten || frame.taskAddressPointRva != 0x23cac8 ||
            !frame.privateState->sharedNullReferenceCommitted || !boundList ||
            boundList != rootList ||
            std::string_view(boundList->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            boundList->symbolRva != 0x2f0a50 || !rootString ||
            std::string_view(rootString->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            rootString->symbolRva != 0x2f0b30 ||
            !frame.privateState->qstringSharedNullBinding.expired() ||
            frame.privateState->qstringSharedNullReferenceCommitted ||
            pending->qobjectBaseCompleted || frame.complete ||
            frame.taskPrivateBacklinkAvailable || frame.privateTaskBacklinkAvailable)
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory QString shared-null requires its committed matching CP165 root at RVA 0x1311a2");
        // No host allocation or throwing operation remains in this checkpoint.
        frame.privateState->qstringSharedNullBinding = rootString; // private+0x30
        widgets->commitQStringSharedNullReference(); // symbolic LOCK ADD +1, modulo 2^32
        frame.privateState->qstringSharedNullReferenceCommitted = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZeroFields) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        if (!qualified || qualified->privateState->private40.has_value() ||
            qualified->privateState->private48.has_value() || qualified->privateState->private50.has_value() ||
            qualified->privateState->private58.has_value())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory private+0x40 zero requires committed matching CP166 roots at RVA 0x1311b5");
        qualified->privateState->private40 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        if (!qualified || qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48.has_value() || qualified->privateState->private50.has_value() ||
            qualified->privateState->private58.has_value())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory private+0x48 zero requires committed matching CP167 roots and private+0x40 at RVA 0x1311bb");
        qualified->privateState->private48 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        if (!qualified || qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50.has_value() || qualified->privateState->private58.has_value())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory private+0x50 zero requires committed matching CP168 state at RVA 0x1311bf");
        qualified->privateState->private50 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        if (!qualified || qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58.has_value())
            throw Error(M2M_E_UNSUPPORTED,
                "DOE Task factory private+0x58 zero requires committed matching CP169 state at RVA 0x1311c3");
        qualified->privateState->private58 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNull;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNull) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        if (!qualified || !qualified->vector ||
            std::string_view(qualified->vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            (qualified->vector->qualifiedSymbolRva && *qualified->vector->qualifiedSymbolRva != 0x2f0bb8) ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            !qualified->privateState->qvectorSharedNullBinding.expired() ||
            qualified->privateState->qvectorSharedNullReferenceCommitted)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory QVector shared-null binding requires qualified CP171 root at RVA 0x1311c7");
        qualified->privateState->qvectorSharedNullBinding = qualified->vector;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNullIncrement;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNullIncrement) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto bound = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        if (!qualified || !qualified->vector || !bound || bound != qualified->vector ||
            bound.owner_before(qualified->vector) || qualified->vector.owner_before(bound) ||
            std::string_view(bound->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            (bound->qualifiedSymbolRva && *bound->qualifiedSymbolRva != 0x2f0bb8) ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->qvectorSharedNullReferenceCommitted)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory QVector shared-null increment requires qualified CP172 root at RVA 0x1311d2");
        // Models the locked DWORD add modulo 2^32; no native baseline is invented.
        qualified->widgets->commitQVectorSharedNullReference();
        qualified->privateState->qvectorSharedNullReferenceCommitted = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListBinding;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListBinding) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto bound = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        if (!qualified || !qualified->vector || !bound || bound != qualified->vector ||
            bound.owner_before(qualified->vector) || qualified->vector.owner_before(bound) ||
            std::string_view(bound->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            (bound->qualifiedSymbolRva && *bound->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->secondQListSharedNullBinding.owner_before(
                std::weak_ptr<const engine::WidgetRuntimeState::QListSharedNullState>{}) ||
            std::weak_ptr<const engine::WidgetRuntimeState::QListSharedNullState>{}.owner_before(
                qualified->privateState->secondQListSharedNullBinding) ||
            qualified->privateState->secondQListReferenceCommitted)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory second QList binding requires qualified CP173 root at RVA 0x1311da");
        // RCX retains the address already stored at +0x18; no reference add here.
        qualified->privateState->secondQListSharedNullBinding = qualified->list;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListIncrement;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListIncrement) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || qualified->privateState->secondQListReferenceCommitted)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory second QList increment requires qualified CP174 root at RVA 0x1311de");
        // Same root record as private+0x18/+0x70; native LOCK ADD is DWORD +1.
        qualified->widgets->commitQListSharedNullReference();
        qualified->privateState->secondQListReferenceCommitted = true;
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero78;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero78) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            qualified->privateState->private78.has_value())
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory private+0x78 zero requires qualified CP175 state at RVA 0x1311e6");
        // Qualified normal path retains RBX=private and RDI=0 from RVA 0x1311b5.
        qualified->privateState->private78 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero80;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero80) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            qualified->privateState->private78 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private80.has_value())
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory private+0x80 zero requires qualified CP176 state at RVA 0x1311ea");
        // Qualified normal path retains RBX=private and RDI=0 from RVA 0x1311b5.
        qualified->privateState->private80 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforeQtVersionCheck;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQtVersionCheck) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        const auto& version = frame.privateInitializerVersion;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private78 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private80 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            !version || version->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            version->argumentLoadRva != 0x13140f || version->callRva != 0x131417 || version->argument != 0x40804)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory Qt version comparison requires qualified CP177 state and public QObject caller at RVA 0x1311f1");
        // [0x1311f1,0x1311f9): cmp edx,0x40804; je +0x12. Only this
        // qualified equality is admitted. Mismatch fallthrough is unmodeled.
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero08;
    }
    if (frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero08) {
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        const auto& version = frame.privateInitializerVersion;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private78 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private80 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            !version || version->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            version->argumentLoadRva != 0x13140f || version->callRva != 0x131417 || version->argument != 0x40804 ||
            qualified->privateState->private08.has_value())
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory private+0x08 zero requires qualified CP178 equal branch at RVA 0x13120b");
        // [0x13120b,0x13120f): mov qword ptr [rbx+8],rdi. On the
        // qualified equal branch RBX retains private and RDI remains zero.
        qualified->privateState->private08 = std::uint64_t{0};
        frame.phase = PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero10;
    }
    using Phase = PendingTaskPrototype::QObjectPrefix::Phase;
    const Phase batchPhases[] = {Phase::BeforePrivateZero10, Phase::BeforePrivateZero24,
        Phase::BeforePrivateZero38, Phase::BeforePrivateZero64, Phase::BeforePrivateZero60,
        Phase::BeforePrivateZero28};
    const char* batchStops[] = {
        "DOE Task factory CP180 stops before QtCore RVA 0x13120f (private+0x10 zero; qualified Qt version equal branch taken at RVA 0x1311f7)",
        "DOE Task factory CP180 stops before QtCore RVA 0x131213 (private+0x24 zero; qualified Qt version equal branch taken at RVA 0x1311f7)",
        "DOE Task factory CP180 stops before QtCore RVA 0x131216 (private+0x38 zero; qualified Qt version equal branch taken at RVA 0x1311f7)",
        "DOE Task factory CP180 stops before QtCore RVA 0x13121a (private+0x64 zero; qualified Qt version equal branch taken at RVA 0x1311f7)",
        "DOE Task factory CP180 stops before QtCore RVA 0x13121d (private+0x60 zero; qualified Qt version equal branch taken at RVA 0x1311f7)",
        "DOE Task factory CP180 stops before QtCore RVA 0x131220 (private+0x28 zero; qualified Qt version equal branch taken at RVA 0x1311f7)"};
    for (std::size_t ordinal = 0; ordinal != 6; ++ordinal) {
        if (frame.phase != batchPhases[ordinal]) continue;
        // Validate the entire current prefix before *each* next write. A
        // malformed later unknown refuses before consuming an earlier store.
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        const auto& version = frame.privateInitializerVersion;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            qualified->privateState->private40 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private48 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private50 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private58 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private78 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private80 != std::optional<std::uint64_t>{0} ||
            qualified->privateState->private08 != std::optional<std::uint64_t>{0} ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            !version || version->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            version->argumentLoadRva != 0x13140f || version->callRva != 0x131417 || version->argument != 0x40804)
            throw Error(M2M_E_UNSUPPORTED, ordinal == 5
                ? "DOE Task factory CP181 requires qualified CP180 retained prefix"
                : "DOE Task factory CP180 requires qualified CP179 retained prefix");
        auto& state = *qualified->privateState;
        if (state.private20 || frame.flags20Eax)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP182 forbids premature symbolic flags state");
        const auto matches = [ordinal](const auto& field, std::size_t storeOrdinal) {
            return ordinal > storeOrdinal ? field && *field == 0 : !field;
        };
        if (!matches(state.private10, 0) || !matches(state.private24, 1) ||
            !matches(state.private38, 2) || !matches(state.private64, 3) || !matches(state.private60, 4))
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP180 requires exact sparse store prefix");
        if (ordinal >= privateZeroBatchStopOrdinal_)
            throw Error(M2M_E_UNSUPPORTED, batchStops[ordinal]);
        if (ordinal == 5) {
            if (state.private28)
                throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP181 requires unobserved private+0x28");
            // Qualified PE [0x131220,0x131224): 48 89 7b 28.
            // RBX is the retained private allocation, RDI=0. This cut
            // publishes no flags result; the separate symbolic cut follows.
            state.private28 = std::uint64_t{0};
            frame.phase = Phase::BeforePrivateFlags20Read;
            break;
        }
        // PE [0x13120f,0x131220): 48897b10 897b24 48897b38
        // 897b64 897b60. Each zero/cursor pair is nonthrowing; no rollback.
        switch (ordinal) {
        case 0: state.private10 = std::uint64_t{0}; break;
        case 1: state.private24 = std::uint32_t{0}; break;
        case 2: state.private38 = std::uint64_t{0}; break;
        case 3: state.private64 = std::uint32_t{0}; break;
        case 4: state.private60 = std::uint32_t{0}; break;
        }
        frame.phase = batchPhases[ordinal + 1];
    }
    using Flags = PendingTaskPrototype::PrivateFlags20Expression;
    const Phase flagsPhases[] = {Phase::BeforePrivateFlags20Read, Phase::BeforeFlags20And,
        Phase::BeforeFlags20Or, Phase::BeforeFlags20Write, Phase::BeforePrivateReturnSetup, Phase::BeforePrivateEpilogue, Phase::BeforePrivateStackAdjustment, Phase::BeforePrivateRdiRestore, Phase::BeforePrivateReturn, Phase::BeforeCallerXor, Phase::BeforeCallerJump, Phase::BeforeTaskPrivateBacklink, Phase::BeforeCallerPrivateCopy, Phase::BeforeCallerPrivateSpill, Phase::BeforePrivateTaskBacklink, Phase::BeforeCallerParentTest, Phase::BeforeCallerParentJump,
        Phase::BeforeCallerNullParentCall, Phase::BeforeCallerParentLoad, Phase::BeforeNullParentHelperEntry};
    const char* flagsStops[] = {
        "DOE Task factory CP181 stops before QtCore RVA 0x131224 (unknown-input private+0x20 flags read)",
        "DOE Task factory CP182 stops before QtCore RVA 0x131227 (symbolic flags AND)",
        "DOE Task factory CP182 stops before QtCore RVA 0x13122c (symbolic flags OR)",
        "DOE Task factory CP182 stops before QtCore RVA 0x13122f (symbolic flags dword store)",
        "DOE Task factory CP182 stops before QtCore RVA 0x131232 (private return setup)",
        "DOE Task factory CP183 stops before QtCore RVA 0x131235 (private epilogue)",
        "DOE Task factory CP184 stops before QtCore RVA 0x13123a (private stack adjustment)",
        "DOE Task factory CP185 stops before QtCore RVA 0x13123e (private RDI restore)",
        "DOE Task factory CP186 stops before QtCore RVA 0x13123f (private RET)",
        "DOE Task factory CP187 stops before QtCore RVA 0x13141c (caller XOR)",
        "DOE Task factory CP188 stops before QtCore RVA 0x13141e (caller jump; public task+8 store unavailable)",
        "DOE Task factory CP189 stops before QtCore RVA 0x131425 (public task+8 store; private backlink unavailable)",
        "DOE Task factory CP190 stops before QtCore RVA 0x131429 (caller R12 copy; reverse private backlink unavailable)",
        "DOE Task factory CP191 stops before QtCore RVA 0x13142c (caller stack spill; reverse private backlink unavailable)",
        "DOE Task factory CP192 stops before QtCore RVA 0x131431 (reverse private backlink unavailable)",
        "DOE Task factory CP193 stops before QtCore RVA 0x131435 (caller parent TEST unavailable)",
        "DOE Task factory CP194 stops before QtCore RVA 0x131438 (caller parent JE unavailable)",
        "DOE Task factory CP195 stops before QtCore RVA 0x131449 (null-parent successor call unavailable)",
        "DOE Task factory CP195 stops before QtCore RVA 0x13143a (nonnull-parent successor load unavailable)",
        "DOE Task factory CP197 stops at QtCore RVA 0x2aca0 (callee entry; first PUSH/TLS/result unavailable)"};
    for (std::size_t ordinal = 0; ordinal != 20; ++ordinal) {
        if (frame.phase != flagsPhases[ordinal] && !(ordinal == 19 && frame.inCalleeOrTlsIndexBoundary())) continue;
        // Revalidate the whole retained prefix before each instruction and at
        // the final stop, including original pointees and control blocks.
        const auto qualified = qualifyRetainedZeroPrefix(allocations, pendingReportAllocation_, pending, frame, widgets_.lock());
        const auto vector = frame.privateState ? frame.privateState->qvectorSharedNullBinding.lock() : nullptr;
        const auto second = frame.privateState ? frame.privateState->secondQListSharedNullBinding.lock() : nullptr;
        const auto& version = frame.privateInitializerVersion;
        if (!qualified || !qualified->vector || !vector || vector != qualified->vector ||
            vector.owner_before(qualified->vector) || qualified->vector.owner_before(vector) ||
            std::string_view(vector->moduleSha256) != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            vector->symbolRva != 0x2f0bb8 ||
            (vector->qualifiedSymbolRva && *vector->qualifiedSymbolRva != 0x2f0bb8) ||
            !qualified->privateState->qvectorSharedNullReferenceCommitted ||
            !second || second != qualified->list || second.owner_before(qualified->list) ||
            qualified->list.owner_before(second) || !qualified->privateState->secondQListReferenceCommitted ||
            !version || version->moduleSha256 != PendingTaskPrototype::QObjectPrefix::moduleSha256 ||
            version->argumentLoadRva != 0x13140f || version->callRva != 0x131417 || version->argument != 0x40804)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP182 requires qualified CP181 retained prefix");
        auto& state = *qualified->privateState;
        if (state.private40 != std::optional<std::uint64_t>{0} ||
            state.private48 != std::optional<std::uint64_t>{0} ||
            state.private50 != std::optional<std::uint64_t>{0} ||
            state.private58 != std::optional<std::uint64_t>{0} ||
            state.private78 != std::optional<std::uint64_t>{0} ||
            state.private80 != std::optional<std::uint64_t>{0} ||
            (ordinal >= 15 ? state.private08.has_value() : state.private08 != std::optional<std::uint64_t>{0}) ||
            state.private10 != std::optional<std::uint64_t>{0} ||
            state.private24 != std::optional<std::uint32_t>{0} ||
            state.private38 != std::optional<std::uint64_t>{0} ||
            state.private64 != std::optional<std::uint32_t>{0} ||
            state.private60 != std::optional<std::uint32_t>{0} ||
            state.private28 != std::optional<std::uint64_t>{0})
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP182 requires exact sparse zero prefix");
        const auto read = Flags::read(frame.allocatedPrivateIdentity);
        const auto expected = ordinal < 2 ? read : ordinal == 2 ? read.afterAnd() : read.afterAnd().afterOr();
        if ((ordinal == 0 ? frame.flags20Eax.has_value() : frame.flags20Eax != std::optional<Flags>{expected}) ||
            (ordinal >= 4 ? state.private20 != std::optional<Flags>{expected} : state.private20.has_value()))
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP182 requires exact symbolic flags prefix");
        if (ordinal >= 5 && frame.returnPrivateIdentity != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP183 requires allocation-bound return identity");
        if (frame.returnToCaller->privateIdentity != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP187 requires allocation-bound return slot origin");
        if (frame.savedEntryRdi->privateIdentity != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP186 requires allocation-bound saved RDI origin");
        if (frame.savedTaskRbx->privateIdentity != frame.allocatedPrivateIdentity)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP184 requires allocation-bound saved stack origin");
        if (ordinal >= 6 && frame.restoredTaskRbxIdentity != frame.savedTaskRbx->taskIdentity)
            throw Error(M2M_E_UNSUPPORTED, "DOE Task factory CP184 requires qualified restored RBX identity");
        if(ordinal==19 && frame.phase==Phase::TlsIndexJneUnresolved) {
            validateTlsIndexJneUnresolved(frame);
            throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP200 stops before QtCore RVA 0x2961b (unresolved JNE alternatives; no edge selected)");
        }
        if(ordinal==19 && frame.phase==Phase::BeforeTlsIndexJne) {
            validateTlsIndexCompare(frame);
            if(tlsIndexJneSchemaEnabled_) advanceTlsIndexJneUnresolved(frame);
            throw Error(M2M_E_UNSUPPORTED,"DOE Task factory CP199 stops at QtCore RVA 0x2961b (unknown CMP; JNE unavailable)");
        }
        if (ordinal == 19) advanceCalleePrefix(frame);
        if (ordinal >= 18 || ordinal >= privateFlags20StopOrdinal_)
            throw Error(M2M_E_UNSUPPORTED, flagsStops[ordinal]);
        // Qualified PE: 8b4320 / 25f0fcffff / 83c860 / 894320.
        // These POD publications cannot throw: retain each expression with
        // its cursor. Only the fourth instruction writes private memory.
        switch (ordinal) {
        case 0: frame.flags20Eax = read; break;
        case 1: frame.flags20Eax = frame.flags20Eax->afterAnd(); break;
        case 2: frame.flags20Eax = frame.flags20Eax->afterOr(); break;
        case 3: state.private20 = *frame.flags20Eax; break;
        // Qualified PE [0x131232,0x131235): 48 8b c3 (mov rax,rbx).
        // The retained RBX identity is copied without dereference or pointer
        // publication. No saved-register restore, stack adjustment, or ret.
        case 4: frame.returnPrivateIdentity = frame.allocatedPrivateIdentity; break;
        // [0x131235,0x13123a): 48 8b 5c 24 48, mov rbx,[rsp+0x48].
        // Only RBX and cursor change. RSP remains S-0x38; no flags change,
        // RDI restoration, RET, caller advancement or backlink publication.
        case 5: frame.restoredTaskRbxIdentity = frame.savedTaskRbx->taskIdentity; break;
        // [0x13123a,0x13123e): 48 83 c4 30, add rsp,0x30.
        // Only symbolic RSP and cursor change. Saved RDI and the return slot
        // remain unconsumed; RAX/private and RBX/task identities are distinct.
        case 6: frame.rspFromEntryS = -8; break;
        // [0x13123e,0x13123f): 5f, pop rdi. Nonthrowing symbolic
        // register publications; slot contents and arithmetic flags persist.
        // RET and the caller continuation/publication remain unavailable.
        case 7:
            frame.restoredRdiToken = &*frame.savedEntryRdi;
            frame.restoredRdiParentIsNull = frame.savedEntryRdi->parentIsNull;
            frame.rspFromEntryS = 0;
            break;
        // [0x13123f,0x131240): c3, ret. Qualified [S] targets 0x13141c.
        // These nonthrowing assignments commit the stack/cursor together;
        // no GPR, memory, flags or caller instruction is changed by RET.
        case 8: frame.rspFromEntryS = 8; break;
        // [0x13141c,0x13141e): 33 f6, xor esi,esi. ESI zero-extends
        // into RSI. Defined arithmetic flags follow XOR; AF stays undefined.
        // No jump, memory store, RAX/RBX/RDI or stack effect is admitted.
        case 9:
            frame.callerRsi = std::uint64_t{0};
            frame.callerArithmeticFlags = PendingTaskPrototype::QObjectPrefix::CallerArithmeticFlags{};
            break;
        // [0x13141e,0x131420): eb 05, unconditional JMP rel8.
        // Next RIP 0x131420 + 5 = 0x131425; skip the null-allocation arm.
        // Only phase/derived RIP changes. Stop before either backlink store.
        case 10: break;
        // [0x131425,0x131429): 48 89 43 08, mov [rbx+8],rax.
        // Only public task+8 receives the qualified private token. Commit
        // the nonthrowing value/marker with the cursor; private+8 stays zero.
        // R12 copy, caller stack spill and the reverse store are later cuts.
        case 11:
            frame.taskPrivateIdentity = frame.returnPrivateIdentity;
            frame.taskPrivateBacklinkAvailable = true;
            break;
        // [0x131429,0x13142c): 4c 8b e0, mov r12,rax. Only the
        // value-bearing R12 observation and cursor change. No memory/flags
        // effect, caller spill or private+8 reverse publication is admitted.
        case 12: frame.r12PrivateIdentity = frame.returnPrivateIdentity; break;
        // [0x13142c,0x131431): 48 89 44 24 58, mov [rsp+0x58],rax.
        // Nonthrowing record/phase commit after full admission. No reverse store.
        case 13:
            frame.callerPrivateSpill = PendingTaskPrototype::QObjectPrefix::CallerPrivateSpill{
                allocations, *pendingReportAllocation_, pending, frame.returnPrivateIdentity,
                PendingTaskPrototype::QObjectPrefix::moduleSha256};
            break;
        // [0x131431,0x131435): 48 89 58 08, mov [rax+8],rbx.
        // Full admission precedes these nonthrowing current-slot/phase writes.
        // TEST RDI has not executed; inherited XOR flags remain current.
        case 14:
            frame.privateTaskPublication = PendingTaskPrototype::QObjectPrefix::PrivateTaskPublication{
                allocations, *pendingReportAllocation_, frame.restoredTaskRbxIdentity,
                frame.returnPrivateIdentity, PendingTaskPrototype::QObjectPrefix::moduleSha256};
            state.private08.reset();
            frame.privateTaskBacklinkAvailable = true;
            break;
        // [0x131435,0x131438): 48 85 ff, TEST RDI,RDI.
        // Replace current XOR flags with expressions over qualified P. No
        // register, memory, JE, parent load or call executes in this cut.
        case 15:
            frame.parentTestOutcome = PendingTaskPrototype::QObjectPrefix::ParentTestOutcome::test(frame.restoredRdiToken);
            frame.callerArithmeticFlags.reset();
            break;
        // [0x131438,0x13143a): 74 0f, JE. Unknown ZF retains the
        // admitted CP194 guard. Known refinements select a path only;
        // JE preserves flags/registers and executes neither load nor call.
        case 16:
            if (!frame.parentTestOutcome->parentIsNull)
                throw Error(M2M_E_UNSUPPORTED, flagsStops[16]);
            if (*frame.parentTestOutcome->parentIsNull) {
                frame.parentBranchPath = ParentBranchPath::TakenNullParent;
                frame.phase = Phase::BeforeCallerNullParentCall;
            } else {
                frame.parentBranchPath = ParentBranchPath::FallthroughNonnullParent;
                frame.phase = Phase::BeforeCallerParentLoad;
            }
            continue;
        // [0x131449,0x13144e): E8 52 98 EF FF. Prepare all potentially
        // throwing call-history state before the nonthrowing slot/SP/cursor
        // commit. This CALL executes no instruction at the target entry.
        case 17: {
            if (!sessionReportSourceEligible_ || !frame.sessionReportSource)
                throw Error(M2M_E_UNSUPPORTED, flagsStops[17]);
            PendingTaskPrototype::QObjectPrefix::NullParentCall call;
            call.sourceBinding = sessionReportFrameBinding_;
            call.callerOccurrence = frame.sourceOccurrence.identity();
            call.initializerOccurrence = frame.returnToCaller->occurrence.identity();
            PendingTaskPrototype::QObjectPrefix::LiveReturnSlot slot{2, 0x13144e, 0,
                frame.sourceOccurrence.identity(), call.occurrence.identity()};
            auto issuedOccurrence = call.occurrence.identity();
            frame.nullParentCall = std::move(call);
            frame.helperReturnSlot = std::move(slot);
            issuedNullParentCallOccurrence_ = std::move(issuedOccurrence);
            frame.rspFromEntryS = 0;
            frame.phase = Phase::BeforeNullParentHelperEntry;
            continue;
        }
        }
        frame.phase = flagsPhases[ordinal + 1];
    }
    // No callee instruction/TLS, helper result, completion or cache effect.
    throw Error(M2M_E_UNSUPPORTED,
        frame.phase == Phase::BeforePrivateFlags20Read
            ? "DOE Task factory CP181 stops before QtCore RVA 0x131224 (unknown-input private+0x20 flags read)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero10
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x13120f (private+0x10 zero; qualified Qt version equal branch taken at RVA 0x1311f7)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero08
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x13120b (private+0x08 zero; qualified Qt version equal branch taken at RVA 0x1311f7)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQtVersionCheck
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311f1 (Qt version comparison)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero80
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311ea (private+0x80 zero)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListIncrement
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311de (second QList increment)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeSecondQListBinding
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311da (second QList binding at private+0x70)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNullIncrement
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311d2 (QVector shared-null increment)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforeQVectorSharedNull
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311c7 (QVector shared-null load)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero58
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311c3 (private+0x58)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero50
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311bf (private+0x50)"
            : frame.phase == PendingTaskPrototype::QObjectPrefix::Phase::BeforePrivateZero48
            ? "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311bb (private+0x48)"
            : "DOE Task factory Report QObject prefix stops before QtCore RVA 0x1311b5 (private zero fields)");
}

TaskObserver TaskFactoryRuntime::createTaskBase(const qt::String& type)
{
    if (!adapterScopeLive_)
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory adapter scope has been released");
    if (profile_ == TaskFactoryProfile::Native) {
        initializeNativePrefix(); //initializes before requested-key lookup
        return {}; //unreachable until the full17-prototype initialization closes
    }
    // A copied production capability cannot escape its source guard through
    // the explicitly separate parsed profile.
    validateSessionReportNullCaller();
    // Old bounded parsed list-state is retained only by this explicitly named
    // fixture profile. It is not native factory/cache/constructor completion.
    if (type == qt::L("SfTaskDoe")) {
        const auto allocations = allocations_.lock();
        if (!allocations || !allocations->adapterScopeLive())
            throw Error(M2M_E_UNSUPPORTED, "DOE bounded parsed Task projection allocation owner is unavailable");
        return allocations->createParsedDoeTask();
    }
    for (const char* known : {"SfTaskReport", "SfTaskDeformSummary", "SfTaskDeformGraph",
            "SfTaskDeformStateVar", "SfTaskDeformPointTrack", "SfTaskDeformFlownet",
            "SfTaskUsrVarTrack", "SfTaskDeformCoupons", "SfTaskDeformRegions",
            "SfTaskDeformMetalFlow", "SfTaskReportGenerate", "SfTaskDoeGlobalMinMax",
            "SfTaskDoeObjMinMax", "SfTaskDoeSVMinMax", "SfTaskDoeAllNodeValue", "Custom"})
        if (type == qt::L(known))
            throw Error(M2M_E_UNSUPPORTED, "DOE session registered non-DOE Task construction remains unsupported in bounded projection");
    return {};
}

std::shared_ptr<TaskFactoryRuntime> bindBoundedParsedDoeProjection(StudyIo& io,
    const std::shared_ptr<TaskAllocationStore>& allocations)
{
    auto runtime = TaskFactoryRuntime::boundedParsedDoeProjection(allocations);
    auto& notifier = io.sessionNotificationState();
    if (notifier.createTaskBaseRegistrations)
        throw Error(M2M_E_UNSUPPORTED, "DOE bounded parsed projection cannot replace existing receiver population");
    io.bindTaskFactoryRuntime(runtime);
    notifier.createTaskBaseRegistrations = TaskFactoryRegistrationList{
        {{runtime, "slotCreateTaskBase"}},
        "Explicit bounded parsed DOE fixture; not native factory completion", true};
    return runtime;
}
} // namespace m2m::doe
