#include "m2m/engine/palette_native_ref.hpp"
#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& first,
    const std::shared_ptr<T>& second) noexcept
{
    return first && second && first.get() == second.get() &&
        !first.owner_before(second) && !second.owner_before(first);
}
}

PaletteNativeRefState::PaletteNativeRefState(std::string qualification)
    : qualification_(std::move(qualification)) {}

bool PaletteNativeRefState::observeCurrent(std::uint32_t bits, std::uint64_t currentSequence,
    const std::string& qualification) noexcept
{
    if (!alive_ || qualification_.empty() || qualification != qualification_ ||
        currentSequence != sequence_ || (bits_ && *bits_ != bits)) return false;
    if (!bits_) bits_ = bits;
    return true;
}

// @src QtCore67001aa0..67001aa8: LOCK ADD dword,+1; SETNE AL; RET.
PaletteNativeAtomicReceipt PaletteNativeRefState::retain() noexcept
{
    PaletteNativeAtomicReceipt receipt;
    receipt.beforeBits = bits_;
    if (bits_) {
        *bits_ += std::uint32_t{1};
        receipt.nonzero = *bits_ != 0;
    }
    receipt.afterBits = bits_;
    receipt.sequence = ++sequence_;
    ++refs_; ++valueBalance_;
    return receipt;
}

// @src QtCore67001ac0..67001ac8: LOCK ADD dword,-1; SETNE AL; RET.
// Caller requires a held token and known current word before this mutation.
PaletteNativeAtomicReceipt PaletteNativeRefState::release() noexcept
{
    PaletteNativeAtomicReceipt receipt;
    receipt.beforeBits = bits_;
    *bits_ -= std::uint32_t{1};
    receipt.afterBits = bits_;
    receipt.nonzero = *bits_ != 0;
    receipt.sequence = ++sequence_;
    ++derefs_; --valueBalance_;
    if (*bits_ == 0) destructionPending_ = true;
    return receipt;
}

PaletteNativeValueRef::PaletteNativeValueRef(const std::shared_ptr<PaletteNativeRefState>& state,
    PaletteNativeValueKind kind) : kind_(kind)
{
    // Reject a foreign block while the supplied object is still live. Never
    // retain a wrapper that could later outlive its canonical raw object.
    if (!state) bindingIssue_ = PaletteNativeRefIssue::StateUnavailable;
    else if (!sameOwner(state, state->weak_from_this().lock()))
        bindingIssue_ = PaletteNativeRefIssue::StateOwnerMismatch;
    else state_ = state;
}

PaletteNativeRefIssue PaletteNativeValueRef::validate(std::shared_ptr<PaletteNativeRefState>& state) const noexcept
{
    if (bindingIssue_ != PaletteNativeRefIssue::None) return bindingIssue_;
    state = state_.lock();
    if (!state || !state->nativeOwnerAlive()) return PaletteNativeRefIssue::StateUnavailable;
    if (!sameOwner(state, state->weak_from_this().lock())) return PaletteNativeRefIssue::StateOwnerMismatch;
    if (state->qualification().empty()) return PaletteNativeRefIssue::MissingQualification;
    return PaletteNativeRefIssue::None;
}

const PaletteNativeValueReport& PaletteNativeValueRef::retain()
{
    std::shared_ptr<PaletteNativeRefState> state;
    report_.issue = validate(state);
    if (report_.issue != PaletteNativeRefIssue::None) return report_;
    if (state->destructionPending()) report_.issue = PaletteNativeRefIssue::DataDestructionPending;
    else if (report_.stage == PaletteNativeValueStage::Released)
        report_.issue = PaletteNativeRefIssue::ValueAlreadyReleased;
    else if (report_.stage == PaletteNativeValueStage::AwaitingRetain) {
        retained_ = state->retain();
        report_.stage = PaletteNativeValueStage::Held;
        // A zero updated word/false ref return is diagnostic only. Native
        //6504a8c6/6504ac17 callers do not branch on that returned boolean.
    }
    return report_;
}

const PaletteNativeValueReport& PaletteNativeValueRef::release()
{
    std::shared_ptr<PaletteNativeRefState> state;
    report_.issue = validate(state);
    if (report_.issue != PaletteNativeRefIssue::None) return report_;
    if (state->destructionPending()) report_.issue = PaletteNativeRefIssue::DataDestructionPending;
    else if (report_.stage == PaletteNativeValueStage::AwaitingRetain)
        report_.issue = PaletteNativeRefIssue::ValueNotRetained;
    else if (report_.stage == PaletteNativeValueStage::Held) {
        if (!state->currentBits()) report_.issue = PaletteNativeRefIssue::RefCountUnavailable;
        else {
            released_ = state->release();
            report_.stage = PaletteNativeValueStage::Released;
            if (state->destructionPending()) report_.issue = PaletteNativeRefIssue::DataDestructionPending;
        }
    }
    return report_;
}

} // namespace m2m::engine
