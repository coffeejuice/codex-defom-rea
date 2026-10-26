// Native GuiModel+248 picking allocation, before its constructor returns.
// A pending record is never the published model member. See the source-bound
// constructor/publication contract in wave3-editor-picking-owner-prefix-contract.
#pragma once
#include "m2m/engine.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <vector>

namespace m2m::engine {

enum class PickingConstructorStage { Allocated, Constructing,
    AwaitingGraphicsRegistration, Refused };
enum class PickingConstructorIssue { None, ModelExpired, ModelMismatch,
    GraphicsRegistrationUnrecovered };
enum class PickingConstructorEffect {
    ParentlessQObject, EmptyEntryList, FirstReference, SecondReference,
    NullModelGuard, EmptyChain, BindActualModel, ResetValue407c,
    EmptyEntrySnapshot, ClearEntryList, ClearSelectionPrefix
};

struct PickingConstructorReport {
    PickingConstructorStage stage = PickingConstructorStage::Allocated;
    PickingConstructorIssue issue = PickingConstructorIssue::None;
    std::vector<PickingConstructorEffect> effects;
    bool repeatedPreparationRejected = false;
    // Host allocation exceptions are retained/rethrown, not claimed to model
    // original allocator failure or constructor unwind behavior.
    bool reconstructionException = false;
    static constexpr bool completeForPublication = false;
};

// Values explicitly reset by14120c820, with padding deliberately absent.
// Unexplained source fields use offsets rather than invented domain names.
struct PickingReferenceValues {
    std::int32_t value008 = 0;
    std::array<double, 3> point010{};
    std::array<std::int32_t, 22> values028{}; // 028..07c, including mode02c
    bool flag080 = false;
    std::array<double, 3> point088{};
    bool flag0a0 = false;
    std::array<double, 3> point0a8{};
    bool flag0c0 = false;
    double value0c8 = 0.0;
    std::int32_t value0d0 = 0, count0e0 = 0;
};

struct PickingReferenceState {
    bool nullGuardConstructed = false; // original+D8
    // Original two separately allocated32-byte vector wrappers at+E8/+F0.
    // Modern vector payloads preserve distinct ownership; original debug-proxy
    // and memory-exhaustion behavior are separately qualified.
    const std::vector<std::int32_t>* indices(std::size_t index) const noexcept
    { return index < indices_.size() ? indices_[index].get() : nullptr; }
    std::optional<PickingReferenceValues> values;
    bool nullModelAssignmentReached = false;
    bool modeAssignmentReached = false;
private:
    friend class PendingPickingOwner;
    std::array<std::unique_ptr<std::vector<std::int32_t>>, 2> indices_;
};

struct PickingChainState {
    // Constructor a1c780(null)->a1c7c0. No model-derived branch is taken.
    const void* first000 = nullptr;
    const void* last008 = nullptr;
    std::int32_t index010 = -1, count014 = 0, kind018 = 2;
    const GuiModel* model020 = nullptr;
    const void* value028 = nullptr;
};

enum class PickingByteKnowledge { Known, Uninitialized, OutOfRange };
struct PickingByteRead {
    PickingByteKnowledge knowledge;
    std::optional<std::byte> value;
};

class PickingSelectionBuffer final {
public:
    static constexpr std::size_t nativeByteExtent = 0x4000;
    static constexpr std::size_t clearByteCount = 0x1000;
    static constexpr std::uint32_t registrationEntryCount = 0x1000;
    std::size_t knownByteCount() const noexcept { return prefixCleared_ ? clearByteCount : 0; }
    PickingByteRead readByte(std::size_t offset) const noexcept;
    std::span<const std::byte> knownPrefix() const noexcept;
private:
    friend class PendingPickingOwner;
    void clearPrefix() noexcept;
    // Only the bytes written before the unresolved graphics call have values.
    // The remaining0x3000 bytes are represented as unknown, never read from
    // uninitialized host storage or silently manufactured as zero.
    std::array<std::byte, clearByteCount> prefix_{};
    bool prefixCleared_ = false;
};

class PendingPickingOwner;
struct PendingSelectionRegistration {
    // Identities are borrowed while the pending owner lives. They identify the
    // logical original buffer; they are NOT an executable native GL pointer.
    const PendingPickingOwner* owner;
    const PickingSelectionBuffer* buffer;
    std::uint32_t entryCount;
    std::uint64_t nativeCallAddress;
};

class PendingPickingOwner final {
public:
    explicit PendingPickingOwner(GuiModel&);
    PendingPickingOwner(const PendingPickingOwner&) = delete;
    PendingPickingOwner& operator=(const PendingPickingOwner&) = delete;
    // One source-ordered prefix attempt with the same actual model identity.
    // No success boolean/context substitute can advance the graphics boundary.
    const PickingConstructorReport& prepareConstructor(GuiModel&);
    const PickingConstructorReport& construction() const noexcept { return report_; }
    bool identityMatches(const GuiModel&) const noexcept;
    GuiModel* guardedModel() const noexcept { return model_.get(); }
    GuiModel* boundNativeModel() const noexcept { return nativeModelBound_ ? model_.get() : nullptr; }
    bool nativeModelGuardConstructed() const noexcept { return nativeModelGuardConstructed_; }
    const PickingReferenceState* reference(std::size_t index) const noexcept;
    const PickingChainState* chain() const noexcept { return chain_ ? &*chain_ : nullptr; }
    std::optional<std::size_t> entryCount() const noexcept;
    std::optional<std::int32_t> resetValue407c() const noexcept { return resetValue407c_; }
    const PickingSelectionBuffer& selectionBuffer() const noexcept { return selection_; }
    std::optional<PendingSelectionRegistration> registrationRequest() const noexcept;
    // All flags/counters/values after14120316d remain unreached. In particular
    // counters4278 and pointer4388/4420 may not be synthesized as fresh zero.
    static constexpr bool postGraphicsStateInitialized = false;
    static constexpr bool completeForPublication = false;
private:
    void constructReference(std::size_t);
    // This guard records external identity/liveness from allocation. The
    // represented native+4380 binding is separately reached at its source point.
    GuiModelGuard model_;
    PickingConstructorReport report_;
    bool entryListConstructed_ = false;
    std::vector<const void*> entries_;
    std::array<std::unique_ptr<PickingReferenceState>, 2> references_;
    bool nativeModelGuardConstructed_ = false, nativeModelBound_ = false;
    std::optional<PickingChainState> chain_;
    std::optional<std::int32_t> resetValue407c_;
    PickingSelectionBuffer selection_;
};

// Root keeps this pending allocation outside the actual GuiModel+248 member.
// c90f10 publishes only after141202b70 returns; no path here reaches that point.
// Destroying this paused reconstruction record releases its C++ storage only;
// it is not evidence that a successful native picking destructor has executed.
} // namespace m2m::engine
