#include "m2m/engine/picking_owner.hpp"
#include <algorithm>

namespace m2m::engine {

PickingByteRead PickingSelectionBuffer::readByte(std::size_t offset) const noexcept
{
    if (offset >= nativeByteExtent) return {PickingByteKnowledge::OutOfRange, std::nullopt};
    if (!prefixCleared_ || offset >= clearByteCount)
        return {PickingByteKnowledge::Uninitialized, std::nullopt};
    return {PickingByteKnowledge::Known, prefix_[offset]};
}

std::span<const std::byte> PickingSelectionBuffer::knownPrefix() const noexcept
{
    return prefixCleared_ ? std::span<const std::byte>(prefix_) : std::span<const std::byte>{};
}

void PickingSelectionBuffer::clearPrefix() noexcept
{
    std::fill(prefix_.begin(), prefix_.end(), std::byte{0});
    prefixCleared_ = true;
}

PendingPickingOwner::PendingPickingOwner(GuiModel& model) : model_(&model)
{
    report_.effects.reserve(11); // reconstruction receipt storage only
}

bool PendingPickingOwner::identityMatches(const GuiModel& model) const noexcept
{
    return model_.get() == &model;
}

void PendingPickingOwner::constructReference(std::size_t index)
{
    auto& owner = references_[index];
    owner = std::make_unique<PickingReferenceState>();
    owner->nullGuardConstructed = true; //14120c727
    owner->indices_[0] = std::make_unique<std::vector<std::int32_t>>(); //E8
    owner->indices_[1] = std::make_unique<std::vector<std::int32_t>>(); //F0
    owner->values.emplace(); //14120c800->14120c820 scalar reset
    //14120e1a0 independently resizes both freshly empty vectors to0.
    owner->indices_[0]->resize(0, 0);
    owner->indices_[1]->resize(0, 0);
    owner->nullModelAssignmentReached = true; //14120c76c changeGuard(null)
    owner->values->values028[1] = 0; //14120c77a mode02c, after reset/guard
    owner->modeAssignmentReached = true;
}

const PickingConstructorReport& PendingPickingOwner::prepareConstructor(GuiModel& model)
{
    if (report_.stage != PickingConstructorStage::Allocated) {
        report_.repeatedPreparationRejected = true;
        return report_;
    }
    if (!model_.get()) {
        report_.stage = PickingConstructorStage::Refused;
        report_.issue = PickingConstructorIssue::ModelExpired;
        return report_;
    }
    if (!identityMatches(model)) {
        report_.stage = PickingConstructorStage::Refused;
        report_.issue = PickingConstructorIssue::ModelMismatch;
        return report_;
    }
    report_.stage = PickingConstructorStage::Constructing;
    try {
        // Parentless QObject and fixed original SfIPicking vtable precede
        // application-specific members. This projection adds no parent events
        // or synthetic QObject connections.
        report_.effects.push_back(PickingConstructorEffect::ParentlessQObject);
        entryListConstructed_ = true; //141202bb0 shared empty QList primitive
        report_.effects.push_back(PickingConstructorEffect::EmptyEntryList);
        constructReference(0);
        report_.effects.push_back(PickingConstructorEffect::FirstReference);
        constructReference(1);
        report_.effects.push_back(PickingConstructorEffect::SecondReference);
        nativeModelGuardConstructed_ = true;
        report_.effects.push_back(PickingConstructorEffect::NullModelGuard);
        chain_.emplace(); //140a1c780(null), no model-derived callback
        report_.effects.push_back(PickingConstructorEffect::EmptyChain);
        nativeModelBound_ = true; //141202c1b actual GuiModel changeGuard
        report_.effects.push_back(PickingConstructorEffect::BindActualModel);

        //141202ce0->e90(false)->ed0. The just-created list is necessarily
        // empty, so its foreach snapshot has no reached pointee destructor.
        resetValue407c_ = 0;
        report_.effects.push_back(PickingConstructorEffect::ResetValue407c);
        report_.effects.push_back(PickingConstructorEffect::EmptyEntrySnapshot);
        entries_.clear();
        report_.effects.push_back(PickingConstructorEffect::ClearEntryList);
        selection_.clearPrefix(); //14120315a memset(7c,0,0x1000 BYTES)
        report_.effects.push_back(PickingConstructorEffect::ClearSelectionPrefix);
        report_.stage = PickingConstructorStage::AwaitingGraphicsRegistration;
        report_.issue = PickingConstructorIssue::GraphicsRegistrationUnrecovered;
    } catch (...) {
        report_.reconstructionException = true;
        report_.stage = PickingConstructorStage::Refused;
        throw;
    }
    return report_;
}

const PickingReferenceState* PendingPickingOwner::reference(std::size_t index) const noexcept
{
    return index < references_.size() ? references_[index].get() : nullptr;
}

std::optional<std::size_t> PendingPickingOwner::entryCount() const noexcept
{
    if (!entryListConstructed_) return std::nullopt;
    return entries_.size();
}

std::optional<PendingSelectionRegistration> PendingPickingOwner::registrationRequest() const noexcept
{
    if (report_.stage != PickingConstructorStage::AwaitingGraphicsRegistration || !model_.get())
        return std::nullopt;
    return PendingSelectionRegistration{this, &selection_,
        PickingSelectionBuffer::registrationEntryCount, 0x14120316d};
}

} // namespace m2m::engine
