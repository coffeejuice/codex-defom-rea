#include "m2m/engine/close_busy_false_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close busy false prefix: ") + detail);
}
bool qualified(const std::string& text)
{
    return text.find_first_not_of(" \t\r\n") != std::string::npos;
}
bool fieldBound(const std::optional<CloseBusyFalseMainField>& field, std::uint64_t identity)
{
    return field && field->mainIdentity == identity && qualified(field->qualification);
}
}

CloseBusyFalseGlobalOwner::CloseBusyFalseGlobalOwner(const ConversionSession& session,
    CloseBusyNativeFields fields, std::string sourceOccurrence, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()), fields_(fields),
      qualification_(std::move(qualification))
{
    const auto manager = manager_.lock();
    if (!manager || manager->ownerSession() != &session)
        unavailable("actual Session manager owner binding is unavailable");
    if (sourceOccurrence != closeBusyFalseSourceOccurrence || !qualified(qualification_))
        unavailable("late false entry occurrence/qualification is unavailable");
}

CloseBusyFalseResult attemptCloseBusyFalsePrefix(const ConversionSession& session,
    CloseBusyFalseGlobalOwner& owner, const CloseBusyFalseMainObservations& facts)
{
    const auto actualManager = session.modelManagerOwner();
    const auto recordedManager = owner.manager_.lock();
    if (owner.session_ != &session || !actualManager || !recordedManager ||
        actualManager != recordedManager || actualManager->ownerSession() != &session)
        unavailable("state does not bind this live Session manager identity");

    bool reset = false, branch = false;
    if (owner.stage_ == CloseBusyFalseStage::BeforeResetStores) {
        owner.fields_.flag9 = 0; //2caa12
        owner.fields_.counter143d91230 = 0; //2caa21, before2ca920/show=false
        owner.stage_ = CloseBusyFalseStage::AwaitMainBusyFacts;
        reset = true;
    }
    const auto missing = [&](const char* site) {
        return CloseBusyFalseResult{CloseBusyFalseStop::MainBusyFactsUnavailable,
                                    reset, false, std::nullopt, site};
    };
    const bool thisOccurrence = facts.sessionOwner == &session &&
        facts.sourceOccurrence == closeBusyFalseSourceOccurrence;
    const auto resolveBranch = [&](bool busy) {
        // show=false cannot run2caf10, even when this main remains busy.
        const bool enable = !busy;
        owner.fields_.fieldA = enable ? 1 : 0; //2caa4f or2caca8 ->2cb543
        owner.fields_.fieldB = enable ? 1 : 0; //2caa5e or2cacb7 ->2cb583
        owner.requestedEnable_ = enable;
        owner.stage_ = CloseBusyFalseStage::BeforeTreeEnableSelectionDispatch;
        branch = true;
    };
    if (owner.stage_ == CloseBusyFalseStage::AwaitMainBusyFacts) {
        if (!thisOccurrence || !facts.globalMainPresent || !qualified(facts.globalMainQualification))
            return missing("0x1402ca84e:late global main-window lookup");

        if (*facts.globalMainPresent) {
            if (!facts.mainIdentity || !qualified(facts.mainBindingQualification) ||
                facts.mainProfile != closeBusyFalseMainProfile)
                return missing("0x1402ca878:qualified same-main virtual+300");
            owner.mainIdentity_ = facts.mainIdentity;
            owner.stage_ = CloseBusyFalseStage::AwaitStackedCurrentIndex;
        } else {
            resolveBranch(false);
        }
    }
    if (owner.stage_ == CloseBusyFalseStage::AwaitStackedCurrentIndex) {
        if (!thisOccurrence || !fieldBound(facts.stackedCurrentIndex, owner.mainIdentity_))
            return missing("0x14001db05:currentIndex of actual main+480 stacked widget");
        owner.stackedIndex_ = facts.stackedCurrentIndex->value;
        if (*owner.stackedIndex_ == 2) resolveBranch(true);
        else owner.stage_ = CloseBusyFalseStage::AwaitMainField270;
    }
    if (owner.stage_ == CloseBusyFalseStage::AwaitMainField270) {
        if (!thisOccurrence || !fieldBound(facts.mainField270, owner.mainIdentity_))
            return missing("0x1400327ce:actual same-main dword+270");
        resolveBranch(facts.mainField270->value == 1);
    }
    // A resolved native query is latched. Retrying does not re-query later facts,
    // replay stores, reset notifier+90, insert keys or execute a tree slot.
    return {CloseBusyFalseStop::TreeEnableSelectionDispatchUnavailable, reset, branch,
            owner.requestedEnable_, *owner.requestedEnable_
                ? "0x1402cacbc:TreeEnableSelection(true) arguments"
                : "0x1402caa63:TreeEnableSelection(false) arguments"};
}
} // namespace m2m::engine
