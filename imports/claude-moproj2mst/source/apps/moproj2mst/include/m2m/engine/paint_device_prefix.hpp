#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine {

class PaintDevicePrefixState;
struct PaintDevicePrefixObservation {
    std::weak_ptr<PaintDevicePrefixState> receiver;
    std::string qualification;
    std::optional<std::uint16_t> paintingWord;
};
enum class PaintDevicePrefixClass { Unprojected, QImage, QPaintDevice };

// One actual returned QImage value, distinct from its shared image data, cached
// source value and module globals. Its creator must end the native lifetime;
// diagnostic shared_ptr leases do not extend it. Observations never resume code.
class PaintDevicePrefixState final : public std::enable_shared_from_this<PaintDevicePrefixState> {
public:
    explicit PaintDevicePrefixState(std::string qualification);
    ~PaintDevicePrefixState();
    PaintDevicePrefixState(const PaintDevicePrefixState&) = delete;
    PaintDevicePrefixState& operator=(const PaintDevicePrefixState&) = delete;
    // Only a caller that modeled returned-image construction may use this:
    //650b9a93 calls65138140;6513814c writes the returned value's word+8=0.
    // No source-image word or runtime startup observation is substituted.
    static std::shared_ptr<PaintDevicePrefixState> fromReturnedImageConstruction(std::string qualification);
    bool observe(const PaintDevicePrefixObservation&) noexcept;
    void releaseNativeOwner() noexcept { alive_ = false; }
    bool nativeOwnerAlive() const noexcept { return alive_; }
    bool qualified() const noexcept { return qualified_; }
    const std::string& qualification() const noexcept { return qualification_; }
    std::optional<std::uint16_t> paintingWord() const noexcept { return paintingWord_; }
    PaintDevicePrefixClass classIdentity() const noexcept { return classIdentity_; }
private:
    friend class PaintDevicePrefixCall;
    std::string qualification_;
    bool alive_ = true, qualified_ = false, prefixStarted_ = false;
    std::optional<std::uint16_t> paintingWord_;
    PaintDevicePrefixClass classIdentity_ = PaintDevicePrefixClass::Unprojected;
    std::weak_ptr<const std::uint8_t> invocation_;
};

enum class PaintDevicePrefixStage { PaintingWord, BaseClassInstall, WarningContinuation, TailReady };
enum class PaintDevicePrefixIssue {
    None, OwnerUnavailable, QualificationUnavailable, PaintingWordUnavailable,
    PrefixAlreadyStarted, WarningUnrecovered
};
enum class PaintDevicePrefixEffect { PaintingWordRead, BaseClassInstalled, CleanupTailReached };
struct PaintDevicePrefixReport {
    PaintDevicePrefixStage stage = PaintDevicePrefixStage::PaintingWord;
    PaintDevicePrefixIssue issue = PaintDevicePrefixIssue::None;
    std::optional<std::uint16_t> readPaintingWord;
    static constexpr bool completeForPublicSave = false;
};

//65138190 prefix only: word read BEFORE base-class store; known0 tail-transfers
// to65148840. Nonzero stops AFTER class installation, before unrecovered warning.
// TailReady is not cleanup/destructor return. No warning/global/Qt ref/deletion
// effects occur here. Host abandonment does not perform native destruction.
class PaintDevicePrefixCall final {
public:
    explicit PaintDevicePrefixCall(const std::shared_ptr<PaintDevicePrefixState>&);
    PaintDevicePrefixCall(const PaintDevicePrefixCall&) = delete;
    PaintDevicePrefixCall& operator=(const PaintDevicePrefixCall&) = delete;
    PaintDevicePrefixReport resume();
    const PaintDevicePrefixReport& report() const noexcept { return report_; }
    const std::vector<PaintDevicePrefixEffect>& effects() const noexcept { return effects_; }
private:
    std::weak_ptr<PaintDevicePrefixState> state_;
    // Host identity only. A second call cannot replay a prefix on this value,
    // including after the first call was abandoned with committed native effects.
    std::shared_ptr<const std::uint8_t> invocation_;
    PaintDevicePrefixReport report_;
    std::vector<PaintDevicePrefixEffect> effects_;
};

} // namespace m2m::engine
