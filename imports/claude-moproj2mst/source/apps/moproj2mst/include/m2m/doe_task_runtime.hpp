#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "m2m/doe_task_lists.hpp"
#include "m2m/engine/view_manager.hpp"

namespace m2m::doe {

struct SessionRecord;
struct SessionRecordIdentity {
    SessionRecord* current = nullptr; //adapter lifetime observation, not native field
};

class TaskAllocationStore;
class TaskFactoryRuntime;
class SessionReportNullBinding;
class TaskObserver;

// A retained native allocation at a constructor boundary, not a completed
// QObject/SessionTask. No uninitialized native bytes are assigned values here.
struct PendingTaskPrototype {
    enum class Kind { Report };
    struct SparseQObjectPrivate;
    // One unknown dword B read from one retained allocation. Masks summarize
    // knowledge; origin and stage preserve the expression's B dependency.
    struct PrivateFlags20Expression {
        enum class Stage { Read, And, Or };
        const SparseQObjectPrivate* privateIdentity = nullptr;
        std::uint32_t offset = 0x20;
        std::uint32_t width = 32;
        std::uint32_t readRva = 0x131224;
        Stage stage = Stage::Read;
        std::uint32_t sourceKnownOne = 0;
        std::uint32_t sourceKnownZero = 0;
        std::uint32_t knownOne = 0;
        std::uint32_t knownZero = 0;
        constexpr std::uint32_t unknownMask() const noexcept
        { return ~(knownOne | knownZero); }
        static constexpr PrivateFlags20Expression read(const SparseQObjectPrivate* identity,
            std::uint32_t ones = 0, std::uint32_t zeros = 0) noexcept
        { return {identity, 0x20, 32, 0x131224, Stage::Read, ones, zeros, ones, zeros}; }
        constexpr PrivateFlags20Expression afterAnd() const noexcept
        {
            auto result = *this;
            result.stage = Stage::And;
            result.knownOne &= 0xfffffcf0u;
            result.knownZero |= 0x30fu;
            return result;
        }
        constexpr PrivateFlags20Expression afterOr() const noexcept
        {
            auto result = *this;
            result.stage = Stage::Or;
            result.knownOne |= 0x60u;
            result.knownZero &= ~std::uint32_t{0x60};
            return result;
        }
        friend bool operator==(const PrivateFlags20Expression&,
            const PrivateFlags20Expression&) = default;
    };
    // Copies represent a new model occurrence; moves retain the prepared
    // occurrence while the fresh frame is published at its stable node.
    class SourceOccurrence {
    public:
        SourceOccurrence() : identity_(std::make_shared<const unsigned char>(0)) {}
        SourceOccurrence(const SourceOccurrence&) : SourceOccurrence() {}
        SourceOccurrence& operator=(const SourceOccurrence&)
        { identity_ = std::make_shared<const unsigned char>(0); return *this; }
        SourceOccurrence(SourceOccurrence&&) noexcept = default;
        SourceOccurrence& operator=(SourceOccurrence&&) noexcept = default;
        const auto& identity() const noexcept { return identity_; }
    private:
        std::shared_ptr<const unsigned char> identity_;
    };
    struct SparseQObjectPrivate {
        // Allocation lifetime, independent of its address and copied fields.
        SourceOccurrence allocationOccurrence;
        std::size_t nativeAllocationBytes = 0x88;
        std::uint32_t addressPointRva = 0x1a9a30;
        bool addressPointWritten = false;
        std::weak_ptr<const engine::WidgetRuntimeState::QListSharedNullState> sharedNullBinding;
        bool sharedNullReferenceCommitted = false;
        // Native private+0x30, distinct from the earlier QList field at +0x18.
        std::weak_ptr<const engine::WidgetRuntimeState::QStringSharedNullState> qstringSharedNullBinding;
        bool qstringSharedNullReferenceCommitted = false;
        // Native private+0x70 publishes the same root QList address as +0x18.
        std::weak_ptr<const engine::WidgetRuntimeState::QListSharedNullState> secondQListSharedNullBinding;
        bool secondQListReferenceCommitted = false; // second symbolic atomic add committed once
        std::weak_ptr<const engine::WidgetRuntimeState::QVectorSharedNullState> qvectorSharedNullBinding;
        bool qvectorSharedNullReferenceCommitted = false;
        // Native QObjectPrivate+0x40: unknown until the zeroing store executes.
        std::optional<std::uint64_t> private40;
        // Native QObjectPrivate+0x48: unknown until the next zeroing store.
        std::optional<std::uint64_t> private48;
        // Native QObjectPrivate+0x50: unknown until the CP169 zeroing store.
        std::optional<std::uint64_t> private50;
        // Native QObjectPrivate+0x58: unknown until the CP170 zeroing store.
        std::optional<std::uint64_t> private58;
        // Native QObjectPrivate+0x78: unknown until the CP176 64-bit store.
        std::optional<std::uint64_t> private78;
        // Native QObjectPrivate+0x80: unknown until the CP177 64-bit store.
        std::optional<std::uint64_t> private80;
        // Current numeric private+0x08 observation: CP179 zero, cleared when
        // CP193 replaces it with QObjectPrefix::privateTaskPublication.
        std::optional<std::uint64_t> private08;
        // CP180 selected straight-line private stores, in native order.
        std::optional<std::uint64_t> private10;
        std::optional<std::uint32_t> private24;
        std::optional<std::uint64_t> private38;
        std::optional<std::uint32_t> private64;
        std::optional<std::uint32_t> private60;
        // CP181 qword store, separate from the adjacent flags dword.
        std::optional<std::uint64_t> private28;
        // Disengaged until CP182's dword store. This is symbolic, never a
        // concrete allocator value or a numeric flags result.
        std::optional<PrivateFlags20Expression> private20;
    };
    struct QObjectPrefix {
        using SourceOccurrence = PendingTaskPrototype::SourceOccurrence;
        SourceOccurrence sourceOccurrence;
        std::shared_ptr<const SessionReportNullBinding> sessionReportSource;
        enum class Phase { QObjectAddressPoint, PrivateAllocation, PrivateAddressPoint, BeforeFirstSharedQList, BeforePrivateFinalAddressPoint, BeforeQStringSharedNull, BeforePrivateZeroFields, BeforePrivateZero48, BeforePrivateZero50, BeforePrivateZero58, BeforeQVectorSharedNull, BeforeQVectorSharedNullIncrement, BeforeSecondQListBinding, BeforeSecondQListIncrement, BeforePrivateZero78, BeforePrivateZero80, BeforeQtVersionCheck, BeforePrivateZero08, BeforePrivateZero10, BeforePrivateZero24, BeforePrivateZero38, BeforePrivateZero64, BeforePrivateZero60, BeforePrivateZero28, BeforePrivateFlags20Read, BeforeFlags20And, BeforeFlags20Or, BeforeFlags20Write, BeforePrivateReturnSetup, BeforePrivateEpilogue, BeforePrivateStackAdjustment, BeforePrivateRdiRestore, BeforePrivateReturn, BeforeCallerXor, BeforeCallerJump, BeforeTaskPrivateBacklink, BeforeCallerPrivateCopy, BeforeCallerPrivateSpill, BeforePrivateTaskBacklink, BeforeCallerParentTest, BeforeCallerParentJump, BeforeCallerNullParentCall, BeforeCallerParentLoad, BeforeNullParentHelperEntry, BeforeHelperPushRsi, BeforeHelperPushRdi, BeforeHelperStackSub, BeforeHelperMarker, BeforeNestedInitializerCall, BeforeTlsInitializerEntry, BeforeInitializerStackSub, BeforeInitializerMarker, BeforeInitializerRbxSpill, BeforeTlsIndexCompare, BeforeTlsIndexJne, TlsIndexJneUnresolved };
        // Qualified module/RVA address points, never host/native pointers.
        static constexpr const char* moduleSha256 = "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160";
        struct PrivateInitializerVersion {
            std::string moduleSha256;
            std::uint32_t argument = 0;
            std::uint32_t argumentLoadRva = 0;
            std::uint32_t callRva = 0;
        };
        // EDX supplied by the selected public QObject caller after non-null
        // private allocation. Absent on an unqualified/foreign caller frame.
        std::optional<PrivateInitializerVersion> privateInitializerVersion;
        // Historical CP182 dword expression after CP183 replaces live RAX.
        // During the flags instructions its native write zero-extends into RAX.
        std::optional<PrivateFlags20Expression> flags20Eax;
        // Live CP188 effects only; absent before XOR means unavailable native
        // bits. CP182 flags20Eax remains a historical private-memory expression.
        std::optional<std::uint64_t> callerRsi;
        struct CallerArithmeticFlags {
            bool carry = false;
            bool parity = true;
            bool zero = true;
            bool sign = false;
            bool overflow = false;
            std::optional<bool> auxiliary; // XOR leaves AF undefined.
            bool operator==(const CallerArithmeticFlags& other) const noexcept
            {
                return carry == other.carry && parity == other.parity &&
                    zero == other.zero && sign == other.sign &&
                    overflow == other.overflow && auxiliary == other.auxiliary;
            }
            bool operator!=(const CallerArithmeticFlags& other) const noexcept { return !(*this == other); }
        };
        std::optional<CallerArithmeticFlags> callerArithmeticFlags;
        // CP183 RAX identity after mov rax,rbx. An opaque allocation token,
        // never a numeric native address; later publication retains this token.
        const SparseQObjectPrivate* returnPrivateIdentity = nullptr;
        // S is initializer-entry RSP, a symbolic origin, never a host address.
        // Derived from the qualified caller/prologue, not supplied stack data.
        struct SavedTaskRbx {
            std::weak_ptr<TaskAllocationStore> allocationOwner;
            std::size_t reportOrdinal = 0;
            const PendingTaskPrototype* taskIdentity = nullptr;
            const SparseQObjectPrivate* privateIdentity = nullptr;
            std::string moduleSha256;
            std::uint32_t initializerEntryRva = 0x131160;
            std::uint32_t callRva = 0x131417;
            std::uint32_t returnRva = 0x13141c;
            std::int32_t rspFromEntryS = -0x38;
            std::int32_t saveSlotFromEntryS = 0x10;
            std::int32_t saveSlotFromRsp = 0x48;
        };
        std::optional<SavedTaskRbx> savedTaskRbx;
        // Opaque saved entry RDI: caller incoming RDX, never concrete parent bits.
        // Its record identity is a symbolic token, not a native pointer value.
        struct SavedEntryRdi {
            std::weak_ptr<TaskAllocationStore> allocationOwner;
            std::size_t reportOrdinal = 0;
            const PendingTaskPrototype* taskIdentity = nullptr;
            const SparseQObjectPrivate* privateIdentity = nullptr;
            std::string moduleSha256;
            std::uint32_t initializerEntryRva = 0x131160;
            std::uint32_t saveRva = 0x131165;
            enum class Origin { IncomingRdxToCallerRdi };
            Origin origin = Origin::IncomingRdxToCallerRdi;
            std::uint32_t incomingMoveRva = 0x1313eb;
            std::uint32_t callRva = 0x131417;
            std::uint32_t returnRva = 0x13141c;
            // Optional refinement of P, never its concrete native bits.
            // Disengaged means unknown nullness, not a null parent.
            std::optional<bool> parentIsNull;
            SourceOccurrence sourceOccurrence;
            std::shared_ptr<const SessionReportNullBinding> sessionReportSource;
        };
        std::optional<SavedEntryRdi> savedEntryRdi;
        // POP copies the saved qualification alongside the register token.
        std::optional<bool> restoredRdiParentIsNull;
        struct ParentTestOutcome {
            enum class Flag { Clear, Set, ParentIsZero, ParentSign, ParentLowByteEvenParity, Undefined };
            const SavedEntryRdi* operand = nullptr;
            std::optional<bool> parentIsNull;
            Flag carry = Flag::Clear;
            Flag overflow = Flag::Clear;
            Flag zero = Flag::ParentIsZero;
            Flag sign = Flag::ParentSign;
            Flag parity = Flag::ParentLowByteEvenParity;
            Flag auxiliary = Flag::Undefined;
            static ParentTestOutcome test(const SavedEntryRdi* token) noexcept
            {
                ParentTestOutcome result;
                result.operand = token;
                result.parentIsNull = token->parentIsNull;
                if (result.parentIsNull) {
                    result.zero = *result.parentIsNull ? Flag::Set : Flag::Clear;
                    if (*result.parentIsNull) {
                        result.sign = Flag::Clear;
                        result.parity = Flag::Set;
                    }
                }
                return result;
            }
            std::optional<std::uint32_t> predictedJeSuccessorRva() const noexcept
            {
                if (!parentIsNull) return std::nullopt;
                return *parentIsNull ? 0x131449u : 0x13143au;
            }
            friend bool operator==(const ParentTestOutcome&, const ParentTestOutcome&) = default;
        };
        // TEST replaces XOR current flags; after stack SUB this record is branch history.
        // Symbolic SF/PF/ZF remain expressions over this same P operand.
        std::optional<ParentTestOutcome> parentTestOutcome;
        enum class ParentBranchPath { TakenNullParent, FallthroughNonnullParent };
        // JE path only. Neither successor's load/call has executed.
        std::optional<ParentBranchPath> parentBranchPath;
        // Historical initializer CALL generation. Its live [S] observation is
        // derived below until RET; after RET this tuple remains history only.
        struct ReturnToCaller {
            std::weak_ptr<TaskAllocationStore> allocationOwner;
            std::size_t reportOrdinal = 0;
            const PendingTaskPrototype* taskIdentity = nullptr;
            const SparseQObjectPrivate* privateIdentity = nullptr;
            std::string moduleSha256;
            std::uint32_t initializerEntryRva = 0x131160;
            std::uint32_t callRva = 0x131417;
            std::uint32_t targetRva = 0x13141c;
            std::int32_t slotFromEntryS = 0;
            std::uint32_t callGeneration = 1;
            SourceOccurrence occurrence;
        };
        std::optional<ReturnToCaller> returnToCaller;
        struct NullParentCall {
            std::shared_ptr<const SessionReportNullBinding> sourceBinding;
            std::weak_ptr<const unsigned char> callerOccurrence;
            std::weak_ptr<const unsigned char> initializerOccurrence;
            std::uint32_t callGeneration = 2;
            std::uint32_t precedingGeneration = 1;
            std::uint32_t callRva = 0x131449;
            std::uint32_t targetRva = 0x2aca0;
            std::uint32_t returnRva = 0x13144e;
            std::int32_t callerRspFromEntryS = 8;
            std::int32_t calleeRspFromEntryS = 0;
            std::int32_t slotFromEntryS = 0;
            SourceOccurrence occurrence;
        };
        std::optional<NullParentCall> nullParentCall;
        struct LiveReturnSlot {
            std::uint32_t callGeneration = 0;
            std::uint32_t valueRva = 0;
            std::int32_t slotFromEntryS = 0;
            std::weak_ptr<const unsigned char> callerOccurrence;
            std::weak_ptr<const unsigned char> callOccurrence;
        };
        struct NestedInitializerCall {
            std::shared_ptr<const SessionReportNullBinding> sourceBinding;
            std::weak_ptr<const unsigned char> callerOccurrence;
            std::weak_ptr<const unsigned char> enclosingOccurrence;
            std::uint32_t generation = 3, precedingGeneration = 2;
            std::uint32_t callRva = 0x2acb1, targetRva = 0x29600, returnRva = 0x2acb6;
            std::int32_t callerRsp = -0x68, calleeRsp = -0x70, slotFromEntryS = -0x70;
            SourceOccurrence occurrence;
        };
        struct CalleeStackSlot {
            enum class Value { TaskRbx, CallerRsi, ParentRdi, MinusTwo, NestedReturn };
            std::weak_ptr<const unsigned char> callerOccurrence;
            std::weak_ptr<const unsigned char> callOccurrence;
            std::uint32_t generation = 0, width = 64, instructionRva = 0;
            std::int32_t fromEntryS = 0;
            Value value = Value::TaskRbx;
            const PendingTaskPrototype* task = nullptr;
            const SavedEntryRdi* parent = nullptr;
            // Numeric kinds use this scalar; TaskRbx/ParentRdi use only
            // their qualified symbolic tokens, never these unused zero bits.
            std::uint64_t numeric = 0;
            SourceOccurrence occurrence;
        };
        struct StackSubFlags {
            enum class Function { CarryOfSub64, EvenParityOfSubLowByte, AuxiliaryBorrowOfSub64,
                ZeroOfSub64, SignOfSub64, OverflowOfSub64 };
            std::weak_ptr<const unsigned char> stackOrigin;
            std::uint32_t width = 64, producerRva = 0;
            std::int32_t operandFromEntryS = 0, resultFromEntryS = 0;
            std::uint64_t immediate = 0;
            std::array<Function, 6> arithmetic{Function::CarryOfSub64, Function::EvenParityOfSubLowByte,
                Function::AuxiliaryBorrowOfSub64, Function::ZeroOfSub64, Function::SignOfSub64,
                Function::OverflowOfSub64};
            SourceOccurrence occurrence;
        };
        struct CalleePrefixState {
            std::size_t completed = 0;
            std::array<std::shared_ptr<const CalleeStackSlot>, 8> slots{};
            std::shared_ptr<const NestedInitializerCall> nestedCall;
            std::optional<LiveReturnSlot> innerReturnSlot;
            std::shared_ptr<const StackSubFlags> firstSub, secondSub, currentSub;
            SourceOccurrence occurrence;
        };
        // For unknown x: r=(x-0xffffffff) mod 2^32. Every flag below
        // depends on this same operand/result; ZF is the unresolved x==-1 predicate.
        struct TlsIndexCompare {
            enum class Function { UnsignedBorrow32,EvenParityOfResultLowByte,AuxiliaryBorrow32,
                ZeroOfResult32,SignOfResult32,SignedOverflow32 };
            enum class Result { SubtractModulo32 };
            std::shared_ptr<const engine::TlsIndexReadExpression> operand;
            std::weak_ptr<const CalleePrefixState> predecessor;
            std::uint32_t width=32,producerRva=0x29614,immediate=0xffffffffu,encodedImmediateWidth=8;
            bool signExtended=true;
            Result result=Result::SubtractModulo32;
            std::array<Function,6> arithmetic{Function::UnsignedBorrow32,Function::EvenParityOfResultLowByte,
                Function::AuxiliaryBorrow32,Function::ZeroOfResult32,Function::SignOfResult32,Function::SignedOverflow32};
            SourceOccurrence occurrence;
        };
        std::shared_ptr<const TlsIndexCompare> tlsIndexCompare;
        // Qualified alternatives only: the JNE is unexecuted and neither edge is selected.
        struct TlsIndexJneUnresolved {
            enum class Status { UnresolvedBeforeExecution };
            enum class Guard { ZeroFlagSet, ZeroFlagClear };
            // Static guarded semantics only. No operand issuer, value, selected
            // edge or completed effect exists here; even the module operand is
            // unevaluated. The sole spill handle authenticates existing history.
            struct PotentialEffect {
                enum class Status { UnevaluatedNotApplied };
                enum class Operand { ModuleMemory, AuthenticatedStackSpill };
                enum class Destination { EaxAndZeroExtendedRax, Rbx };
                enum class Result { ZeroExtendPotentialDword, TaskRbxFromSpill };
                Status status=Status::UnevaluatedNotApplied;
                std::array<std::uint8_t,6> bytes{}; // zero padding beyond length
                std::uint32_t instructionLength=0, operandWidth=0;
                Operand operand=Operand::ModuleMemory;
                std::uint32_t moduleOperandRva=0;
                std::int32_t rspDisplacement=0, stackFromEntryS=0;
                Destination destination=Destination::EaxAndZeroExtendedRax;
                Result result=Result::ZeroExtendPotentialDword;
                bool flagsUnchanged=true, rspUnchanged=true;
                std::uint32_t prospectiveNextRva=0;
                std::shared_ptr<const CalleeStackSlot> existingSpill;
                friend bool operator==(const PotentialEffect&, const PotentialEffect&) = default;
            };
            static PotentialEffect potentialModuleMov()
            {
                PotentialEffect effect;
                effect.bytes={0x8b,0x05,0x01,0xa7,0x2c,0x00};
                effect.instructionLength=6;effect.operandWidth=32;
                effect.moduleOperandRva=0x2f3d28;effect.prospectiveNextRva=0x29627;
                return effect;
            }
            static PotentialEffect potentialSpillMov()
            {
                PotentialEffect effect;
                effect.bytes={0x48,0x8b,0x5c,0x24,0x40,0x00};
                effect.instructionLength=5;effect.operandWidth=64;
                effect.operand=PotentialEffect::Operand::AuthenticatedStackSpill;
                effect.rspDisplacement=0x40;effect.stackFromEntryS=-0x68;
                effect.destination=PotentialEffect::Destination::Rbx;
                effect.result=PotentialEffect::Result::TaskRbxFromSpill;
                effect.prospectiveNextRva=0x296c5;
                return effect;
            }
            struct Edge {
                Guard guard;
                std::uint32_t targetRva;
                PotentialEffect potentialEffect;
                friend bool operator==(const Edge&, const Edge&) = default;
            };
            std::shared_ptr<const TlsIndexCompare> predecessor;
            std::string_view moduleSha256=QObjectPrefix::moduleSha256;
            std::uint32_t producerRva=0x2961b;
            std::array<std::uint8_t,6> bytes{0x0f,0x85,0x9f,0x00,0x00,0x00};
            std::uint32_t instructionLength=6, flagsIndex=3;
            Status status=Status::UnresolvedBeforeExecution;
            std::array<Edge,2> edges{{{Guard::ZeroFlagSet,0x29621,potentialModuleMov()},
                {Guard::ZeroFlagClear,0x296c0,potentialSpillMov()}}};
            SourceOccurrence occurrence;
        };
        std::shared_ptr<const TlsIndexJneUnresolved> tlsIndexJneUnresolved;
        // Each issued boundary and its observations are immutable. The factory
        // independently retains the exact handle; copied replacements cannot
        // authenticate their own slots, nested generation or flag expressions.
        std::shared_ptr<const CalleePrefixState> calleePrefix;
        bool inCalleePrefix() const noexcept
        { return phase >= Phase::BeforeNullParentHelperEntry && phase <= Phase::BeforeTlsIndexCompare; }
        bool atTlsIndexBoundary() const noexcept
        { return phase==Phase::BeforeTlsIndexJne || phase==Phase::TlsIndexJneUnresolved; }
        bool inCalleeOrTlsIndexBoundary() const noexcept
        { return inCalleePrefix() || atTlsIndexBoundary(); }
        std::int32_t calleeRspFromEntryS() const noexcept
        {
            if(atTlsIndexBoundary()) return -0xa8;
            constexpr std::int32_t displacements[]{0,-8,-0x10,-0x18,-0x68,-0x68,-0x70,-0x78,-0xa8,-0xa8,-0xa8};
            return displacements[static_cast<std::size_t>(phase) - static_cast<std::size_t>(Phase::BeforeNullParentHelperEntry)];
        }
        enum class CurrentFlags { Unavailable, CallerXor, ParentTest, StackSubtract, TlsIndexCompare };
        CurrentFlags currentArithmeticFlags() const noexcept
        {
            if(tlsIndexCompare) return CurrentFlags::TlsIndexCompare;
            if (calleePrefix && calleePrefix->currentSub) return CurrentFlags::StackSubtract;
            if (parentTestOutcome) return CurrentFlags::ParentTest;
            return callerArithmeticFlags ? CurrentFlags::CallerXor : CurrentFlags::Unavailable;
        }
        bool entrySavedRdiSlotLive() const noexcept { return phase <= Phase::BeforePrivateRdiRestore; }
        // Engaged only for the new CALL. The initializer slot is derived from
        // its retained history and phase, avoiding two simultaneously live values.
        std::optional<LiveReturnSlot> helperReturnSlot;
        std::optional<LiveReturnSlot> liveReturnSlot() const noexcept
        {
            if (calleePrefix && calleePrefix->innerReturnSlot) return calleePrefix->innerReturnSlot;
            if (helperReturnSlot) return helperReturnSlot;
            if (phase <= Phase::BeforePrivateReturn && returnToCaller)
                return LiveReturnSlot{returnToCaller->callGeneration, returnToCaller->targetRva,
                    returnToCaller->slotFromEntryS, sourceOccurrence.identity(),
                    returnToCaller->occurrence.identity()};
            return std::nullopt; // initializer RET has consumed generation one
        }
        std::array<std::optional<LiveReturnSlot>, 2> activeReturnSlots() const noexcept
        {
            if (!helperReturnSlot) return {liveReturnSlot(), std::nullopt};
            return {helperReturnSlot, calleePrefix ? calleePrefix->innerReturnSlot : std::nullopt};
        }
        // CP192 symbolic [RSP+0x58] = [S+0x60], distinct from saved slots.
        // Qualified allocation token, never concrete native pointer bits.
        struct CallerPrivateSpill {
            std::weak_ptr<TaskAllocationStore> allocationOwner;
            std::size_t reportOrdinal = 0;
            const PendingTaskPrototype* taskIdentity = nullptr;
            const SparseQObjectPrivate* privateIdentity = nullptr;
            std::string_view moduleSha256;
            std::uint32_t initializerEntryRva = 0x131160;
            std::uint32_t callRva = 0x131417;
            std::uint32_t returnRva = 0x13141c;
            std::uint32_t instructionRva = 0x13142c;
            std::int32_t slotFromEntryS = 0x60;
            std::int32_t slotFromRsp = 0x58;
        };
        std::optional<CallerPrivateSpill> callerPrivateSpill;
        // CP193 current private+8 task token, mutually exclusive with private08.
        // Task/private identities are adapter tokens, never native pointer bits.
        struct PrivateTaskPublication {
            std::weak_ptr<TaskAllocationStore> allocationOwner;
            std::size_t reportOrdinal = 0;
            const PendingTaskPrototype* taskIdentity = nullptr;
            const SparseQObjectPrivate* privateIdentity = nullptr;
            std::string_view moduleSha256;
            std::uint32_t instructionRva = 0x131431;
            std::uint32_t destinationOffset = 0x08;
        };
        std::optional<PrivateTaskPublication> privateTaskPublication;
        // RIP is derived from the phase rather than duplicated mutable state.
        std::optional<std::uint32_t> returnBoundaryRva() const noexcept
        {
            if (phase == Phase::BeforePrivateReturn) return 0x13123f;
            if (phase == Phase::BeforeCallerXor) return 0x13141c;
            if (phase == Phase::BeforeCallerJump) return 0x13141e;
            if (phase == Phase::BeforeTaskPrivateBacklink) return 0x131425;
            if (phase == Phase::BeforeCallerPrivateCopy) return 0x131429;
            if (phase == Phase::BeforeCallerPrivateSpill) return 0x13142c;
            if (phase == Phase::BeforePrivateTaskBacklink) return 0x131431;
            if (phase == Phase::BeforeCallerParentTest) return 0x131435;
            if (phase == Phase::BeforeCallerParentJump) return 0x131438;
            if (phase == Phase::BeforeCallerNullParentCall) return 0x131449;
            if (phase == Phase::BeforeCallerParentLoad) return 0x13143a;
            if(atTlsIndexBoundary()) return 0x2961b;
            if (inCalleePrefix()) {
                constexpr std::uint32_t cursors[]{0x2aca0,0x2aca2,0x2aca3,0x2aca4,0x2aca8,0x2acb1,
                    0x29600,0x29602,0x29606,0x2960f,0x29614};
                return cursors[static_cast<std::size_t>(phase) - static_cast<std::size_t>(Phase::BeforeNullParentHelperEntry)];
            }
            return std::nullopt;
        }
        // Null means POP has not run, independently of unknown native bits.
        const SavedEntryRdi* restoredRdiToken = nullptr;
        // Live symbolic stack displacement. SavedTaskRbx retains the historical
        // MOV input relation; these slots have no captured native contents.
        std::int32_t rspFromEntryS = -0x38;
        std::int32_t savedRdiSlotFromEntryS = -8;
        std::int32_t returnSlotFromEntryS = 0;
        // CP184 live RBX = saved caller task; RAX remains private identity.
        const PendingTaskPrototype* restoredTaskRbxIdentity = nullptr;
        // CP191 copies the qualified RAX token. Null means unobserved R12,
        // without establishing its incoming native contents or a concrete address.
        const SparseQObjectPrivate* r12PrivateIdentity = nullptr;
        std::unique_ptr<SparseQObjectPrivate> privateState;
        // The allocation-time pointee remains stable across retained constructor cuts.
        const SparseQObjectPrivate* allocatedPrivateIdentity = nullptr;
        std::uint32_t taskAddressPointRva = 0x23cac8;
        Phase phase = Phase::BeforeFirstSharedQList;
        // BeforePrivateZero08 records the qualified equal branch at 0x1311f7
        // to 0x13120b. BeforePrivateZero10 records only private+0x08 zero;
        // CP180 successors record one store each through BeforePrivateZero28
        // at 0x131220. CP181 observes +0x28. CP182 retains four symbolic
        // flags steps through 0x131232. CP183 copies the private identity into
        // the symbolic return register at 0x131235. CP184 restores saved task
        // RBX at 0x13123a. CP185 adds 0x30 to symbolic RSP and stops
        // at 0x13123e before pop rdi. CP186 restores the opaque saved entry
        // RDI and advances RSP to S at 0x13123f before RET. CP187 consumes
        // the qualified return target, RSP=S+8, RIP=0x13141c before caller XOR;
        // CP188 zeroes RSI and defines XOR arithmetic flags at 0x13141e,
        // before the caller jump. CP189 takes EB 05 to 0x131425 before
        // the public task+8 store. CP190 publishes only task+8, then stops
        // at 0x131429 before R12 copy. CP191 copies RAX into R12 and stops
        // at 0x13142c before caller spill. CP192 spills to [S+0x60] and
        // stops at 0x131431 before private+8 reverse store. CP193 publishes
        // private+8 = task and stops at 0x131435 before TEST RDI. CP194
        // computes symbolic TEST flags and stops at 0x131438 before JE.
        // CP195 selects only a qualified JE successor: null at 0x131449
        // before the call, nonnull at 0x13143a before the parent load.
        // Unknown nullness remains at 0x131438 without a path selection.
        // CP197 pushes only a source-qualified null-parent CALL return address
        // into generation-two [S], RSP=S, and stops at callee entry 0x2aca0.
        bool taskAddressPointWritten = false;
        // Qualified task+8 value: nullptr means no publication observation,
        // not a known native null pointer. Never a concrete native address.
        const SparseQObjectPrivate* taskPrivateIdentity = nullptr;
        bool taskPrivateBacklinkAvailable = false;
        bool privateTaskBacklinkAvailable = false;
        bool complete = false;
        const SparseQObjectPrivate* privateIdentity() const noexcept { return privateState.get(); }
    };
    Kind kind = Kind::Report;
    std::size_t nativeAllocationBytes = 0xc0;
    bool qobjectBaseCompleted = false;
    std::optional<QObjectPrefix> qobjectPrefix;
};

// Privately issued static caller qualification, not a captured native frame.
// Public read-only fields make every qualification inspectable; a public value
// record or matching strings cannot manufacture an instance.
class SessionReportNullBinding final {
public:
    SessionReportNullBinding(const SessionReportNullBinding&) = delete;
    SessionReportNullBinding& operator=(const SessionReportNullBinding&) = delete;
    struct SourceIdentity {
        std::string_view originalSha256 = "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc";
        std::string_view qtCoreSha256 = PendingTaskPrototype::QObjectPrefix::moduleSha256;
        std::string_view symbol = "??0QObject@@QEAA@PEAV0@@Z";
        std::string_view module = "QtCore4_64.dll";
        // Allocation call, Report, Report call, intermediate entry/call,
        // TaskBase, XOR EDX, IAT call/slot, public export, saved-RDX move.
        std::array<std::uint64_t, 11> addresses{0x140137644, 0x1407b89e0, 0x1407b89f7,
            0x14157ead0, 0x14157eae7, 0x141403210, 0x141403222, 0x141403229,
            0x141e041c0, 0x671313d0, 0x671313eb};
        friend bool operator==(const SourceIdentity&, const SourceIdentity&) = default;
    };
    const SourceIdentity source{};
    const TaskFactoryRuntime* const receiver;
    const std::weak_ptr<TaskAllocationStore> allocations;
    const std::weak_ptr<engine::WidgetRuntimeState> widgets;
    const std::shared_ptr<const SessionReportNullBinding> capability;
    const PendingTaskPrototype* const task;
    const PendingTaskPrototype::SparseQObjectPrivate* const privateIdentity;
    const std::size_t ordinal = 0;
    const std::weak_ptr<const unsigned char> frameOccurrence;
    const std::weak_ptr<const unsigned char> tokenOccurrence;
    const std::weak_ptr<const unsigned char> privateOccurrence;
    const std::weak_ptr<const unsigned char> initializerCallOccurrence;
private:
    friend class TaskFactoryRuntime;
    SessionReportNullBinding(const TaskFactoryRuntime* receiverValue,
        const std::shared_ptr<TaskAllocationStore>& allocationValue,
        const std::shared_ptr<engine::WidgetRuntimeState>& widgetValue,
        std::shared_ptr<const SessionReportNullBinding> capabilityValue = {},
        const PendingTaskPrototype* taskValue = nullptr,
        const PendingTaskPrototype::SparseQObjectPrivate* privateValue = nullptr,
        std::weak_ptr<const unsigned char> frameValue = {},
        std::weak_ptr<const unsigned char> tokenValue = {},
        std::weak_ptr<const unsigned char> privateOccurrenceValue = {},
        std::weak_ptr<const unsigned char> initializerCallOccurrenceValue = {})
        : receiver(receiverValue), allocations(allocationValue), widgets(widgetValue),
          capability(std::move(capabilityValue)), task(taskValue), privateIdentity(privateValue),
          frameOccurrence(std::move(frameValue)), tokenOccurrence(std::move(tokenValue)),
          privateOccurrence(std::move(privateOccurrenceValue)),
          initializerCallOccurrence(std::move(initializerCallOccurrenceValue)) {}
};

// Recovered list-state of the actual parentless Task QObject. Constructor
// preferences and unrelated fields remain opaque; this is not a task runner.
struct SessionTask {
    SessionTask() = default;
    SessionTask(const SessionTask&) = delete;
    SessionTask& operator=(const SessionTask&) = delete;
    TaskLists lists{{}, {0}};
    bool executionPreferencesRecovered = false;

    // Native Task+30 is a RAW Session backlink. Initial null is distinct from a
    // formerly bound, now destroyed record; the latter guards at consumption.
    SessionRecord* sessionBacklink() const;
    bool hasSessionBacklink() const noexcept { return session_.has_value(); }

private:
    friend class TaskAllocationStore;
    friend void bindSessionTask(SessionRecord&, const TaskObserver&);
    std::shared_ptr<const unsigned char> lifetime_ = std::make_shared<const unsigned char>(0);
    std::optional<std::weak_ptr<SessionRecordIdentity>> session_;
};

// QPointer-style observation: copies do not own the Task. This is distinct from
// the native factory's raw cache, whose initialization remains guarded.
class TaskObserver {
public:
    TaskObserver() = default;
    SessionTask* get() const noexcept { return lifetime_.expired() ? nullptr : pointer_; }
    SessionTask* operator->() const noexcept { return get(); }
    SessionTask& operator*() const { return *get(); }
    explicit operator bool() const noexcept { return get() != nullptr; }
    void reset() noexcept { lifetime_.reset(); pointer_ = nullptr; }
    friend bool operator==(const TaskObserver& task, std::nullptr_t) noexcept
    { return task.get() == nullptr; }

private:
    friend class TaskAllocationStore;
    TaskObserver(SessionTask* pointer, const std::shared_ptr<const unsigned char>& lifetime)
        : pointer_(pointer), lifetime_(lifetime) {}
    SessionTask* pointer_ = nullptr;
    std::weak_ptr<const unsigned char> lifetime_;
};

// Adapter allocation retention for one original fresh-process/session scope.
// Type2 clear and concrete Mo factory clear do not delete these allocations.
// This does not model an initialized 17-prototype registry or all native Close.
class TaskAllocationStore {
public:
    TaskAllocationStore();
    ~TaskAllocationStore();
    TaskAllocationStore(const TaskAllocationStore&) = delete;
    TaskAllocationStore& operator=(const TaskAllocationStore&) = delete;

    TaskObserver createParsedDoeTask();
    const PendingTaskPrototype& allocatePendingReportPrototype();
    SessionRecord& createSessionRecord();
    // The owning Session explicitly pairs this allocation scope with its root
    // WidgetRuntimeState. A factory cannot establish the pairing by itself.
    void associateWidgetRuntime(const std::shared_ptr<engine::WidgetRuntimeState>& widgets) noexcept;
    bool ownsWidgetRuntime(const std::shared_ptr<engine::WidgetRuntimeState>& widgets) const noexcept;
    // Adapter fresh-process boundary, NOT native normal Close deletion. A
    // retained external shared lease cannot keep semantic objects live past it.
    void releaseAdapterScope() noexcept;
    bool adapterScopeLive() const noexcept { return adapterScopeLive_; }
    std::size_t retainedTaskCount() const noexcept { return tasks_.size(); }
    std::size_t retainedSessionCount() const noexcept { return sessions_.size(); }
    std::size_t retainedPrototypeCount() const noexcept { return prototypes_.size(); }
    PendingTaskPrototype* retainedPrototypeAt(std::size_t index);
    const PendingTaskPrototype* retainedPrototypeAt(std::size_t index) const;
    TaskObserver retainedTaskAt(std::size_t index) const;
    const SessionRecord* retainedSessionAt(std::size_t index) const;

private:
    void requireAdapterScope() const;
    bool adapterScopeLive_ = true;
    bool widgetRuntimeAssociated_ = false;
    std::weak_ptr<engine::WidgetRuntimeState> widgetRuntime_;
    std::vector<std::unique_ptr<SessionTask>> tasks_;
    std::vector<std::unique_ptr<SessionRecord>> sessions_;
    std::vector<std::unique_ptr<PendingTaskPrototype>> prototypes_;
};

//658520: assign Session+58, then Task+30 only if the observed Task is live.
void bindSessionTask(SessionRecord& record, const TaskObserver& task);

} // namespace m2m::doe
