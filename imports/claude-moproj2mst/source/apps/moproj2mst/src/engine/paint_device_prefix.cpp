#include "m2m/engine/paint_device_prefix.hpp"

#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) noexcept
{ return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
}

PaintDevicePrefixState::PaintDevicePrefixState(std::string qualification)
    : qualification_(std::move(qualification)) {}
PaintDevicePrefixState::~PaintDevicePrefixState() { releaseNativeOwner(); }

std::shared_ptr<PaintDevicePrefixState>
PaintDevicePrefixState::fromReturnedImageConstruction(std::string qualification)
{
    if (qualification.empty()) return {};
    auto state = std::make_shared<PaintDevicePrefixState>(std::move(qualification));
    state->qualified_ = true;
    state->paintingWord_ = 0;
    state->classIdentity_ = PaintDevicePrefixClass::QImage;
    return state;
}

bool PaintDevicePrefixState::observe(const PaintDevicePrefixObservation& o) noexcept
{
    const auto self = weak_from_this().lock(); const auto receiver = o.receiver.lock();
    if (!alive_ || !self || !receiver || !sameOwner(self, receiver) ||
        qualification_.empty() || o.qualification != qualification_) return false;
    if (paintingWord_ && o.paintingWord && *paintingWord_ != *o.paintingWord) return false;
    if (!paintingWord_ && o.paintingWord) paintingWord_ = o.paintingWord;
    qualified_ = true;
    return true;
}

PaintDevicePrefixCall::PaintDevicePrefixCall(const std::shared_ptr<PaintDevicePrefixState>& state)
    : state_(state), invocation_(std::make_shared<const std::uint8_t>(0)) {}

PaintDevicePrefixReport PaintDevicePrefixCall::resume()
{
    using Stage = PaintDevicePrefixStage;
    using Issue = PaintDevicePrefixIssue;
    using Effect = PaintDevicePrefixEffect;
    const auto pending = [&](Issue issue) { report_.issue = issue; return report_; };
    // Historical receipt only; it cannot authorize a second native invocation.
    if (report_.stage == Stage::TailReady) return report_;
    const auto state = state_.lock();
    if (!state || !state->nativeOwnerAlive() || !sameOwner(state, state->weak_from_this().lock()))
        return pending(Issue::OwnerUnavailable);
    if (!state->qualified()) return pending(Issue::QualificationUnavailable);
    if (state->prefixStarted_ && !sameOwner(invocation_, state->invocation_.lock()))
        return pending(Issue::PrefixAlreadyStarted);
    if (report_.stage == Stage::PaintingWord) {
        if (!state->paintingWord_) return pending(Issue::PaintingWordUnavailable);
        state->prefixStarted_ = true; state->invocation_ = invocation_;
        report_.readPaintingWord = *state->paintingWord_;
        effects_.push_back(Effect::PaintingWordRead); //65138196
        report_.stage = Stage::BaseClassInstall;
    }
    if (report_.stage == Stage::BaseClassInstall) {
        state->classIdentity_ = PaintDevicePrefixClass::QPaintDevice; //651381a5
        effects_.push_back(Effect::BaseClassInstalled);
        if (*report_.readPaintingWord != 0) {
            report_.stage = Stage::WarningContinuation;
            return pending(Issue::WarningUnrecovered);
        }
        //651381a8 skips qWarning;651381bf tail-jumps, not destructor RET.
        effects_.push_back(Effect::CleanupTailReached);
        report_.stage = Stage::TailReady; report_.issue = Issue::None;
    }
    if (report_.stage == Stage::WarningContinuation) return pending(Issue::WarningUnrecovered);
    return report_;
}

} // namespace m2m::engine
