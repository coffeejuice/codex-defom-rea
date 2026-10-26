#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace m2m::engine {

// QPair<double,QColor>: preserve the copied fields, not native pair padding.
struct BrushGradientStop {
    std::uint64_t positionBits;
    std::array<std::uint8_t, 14> color;
    bool operator==(const BrushGradientStop&) const = default;
    std::uint8_t alphaHighByte() const noexcept { return color[5]; }
};

struct BrushGradientStopDataObservation {
    std::optional<std::int32_t> count, capacity, externalReferences;
    std::optional<std::uint8_t> flagsLow2;
    std::optional<std::vector<BrushGradientStop>> payload;
};

enum class BrushGradientStopOwnerKind { ObservedSource, SharedNullSymbol, ConstructedDefault };
class BrushGradientStopsCall;
class BrushGradientStopDataState final {
public:
    explicit BrushGradientStopDataState(std::uint32_t observationAlias);
    // Source-qualified shared-null header profile. One actual runtime must
    // qualify its stage/header and own/intern this symbol before integration.
    // Its physical atomic count remains unknown; only added value refs are tracked.
    static std::shared_ptr<BrushGradientStopDataState> makeSharedNullSymbol();
    bool observe(const BrushGradientStopDataObservation&);
    std::optional<std::uint32_t> observationAlias() const noexcept { return alias_; }
    BrushGradientStopOwnerKind kind() const noexcept { return kind_; }
    std::optional<std::int32_t> count() const noexcept { return count_; }
    std::optional<std::int32_t> capacity() const noexcept { return capacity_; }
    std::optional<std::int32_t> externalReferences() const noexcept { return externalReferences_; }
    std::optional<std::uint8_t> flagsLow2() const noexcept { return flagsLow2_; }
    const std::optional<std::vector<BrushGradientStop>>& payload() const noexcept { return payload_; }
    std::size_t modeledValueReferences() const noexcept { return valueReferences_; }
    bool nativeDataAlive() const noexcept { return alive_; }
private:
    friend class BrushGradientStopsCall;
    explicit BrushGradientStopDataState(BrushGradientStopOwnerKind);
    void retainValue() noexcept;
    void releaseValue() noexcept;
    std::optional<std::uint32_t> alias_;
    BrushGradientStopOwnerKind kind_ = BrushGradientStopOwnerKind::ObservedSource;
    std::optional<std::int32_t> count_, capacity_, externalReferences_;
    std::optional<std::uint8_t> flagsLow2_;
    std::optional<std::vector<BrushGradientStop>> payload_;
    std::size_t valueReferences_ = 0;
    bool alive_ = true;
};

// Source value embedded in the actual shared brush's gradient. Host shared_ptr
// references are not Qt atomic refs. Runtime interning/live-widget qualification
// must precede binding this isolated helper to WidgetBrushDataState.
class BrushGradientStopsSource final {
public:
    bool observeSourceData(const std::shared_ptr<BrushGradientStopDataState>&) noexcept;
    std::shared_ptr<const BrushGradientStopDataState> sourceData() const noexcept { return data_; }
private:
    friend class BrushGradientStopsCall;
    std::shared_ptr<BrushGradientStopDataState> data_;
};

enum class BrushGradientStopsStage { SourceCount, NonzeroSource, DefaultLocal, Returned, Released };
enum class BrushGradientStopsIssue {
    None, SourceUnavailable, SourceDataUnavailable, SourceCountUnavailable,
    NonzeroSourceUnrecovered, SharedNullUnavailable, SharedNullOwnerMismatch
};
enum class BrushGradientStopsEffect {
    SourceCountRead, LocalValueConstructed, SharedNullRetained,
    CapacityOneAllocated, SharedNullReleased, ZeroStopAppended,
    CapacityThreeAllocated, ExistingStopCopied, CapacityOneReleased,
    CapacityOneFreed, OneStopAppended, ReturnedValueRetained,
    LocalValueReleased, LocalValueDestroyed, ReturnedValueReleased,
    CapacityThreeFreed, ReturnedValueDestroyed
};
struct BrushGradientStopsReport {
    BrushGradientStopsStage stage = BrushGradientStopsStage::SourceCount;
    BrushGradientStopsIssue issue = BrushGradientStopsIssue::None;
    static constexpr bool completeForPublicSave = false;
};

// QGradient::stops6511b390, successful-allocation profile, exactly-zero source
// count only. Caller enters after style15/17 dispatch and the brush alpha read.
// This returns a vector VALUE: its ref survives until explicit caller destruction.
// Nonzero source, radial, texture, allocation failure and public binding stay out.
class BrushGradientStopsCall final {
public:
    BrushGradientStopsCall(std::shared_ptr<const BrushGradientStopsSource>,
        std::shared_ptr<BrushGradientStopDataState> actualSharedNull);
    ~BrushGradientStopsCall();
    BrushGradientStopsCall(const BrushGradientStopsCall&) = delete;
    BrushGradientStopsCall& operator=(const BrushGradientStopsCall&) = delete;
    // Refine an unavailable runtime symbol without replaying the native frame.
    // Actual runtime qualification precedes this call. No vector ref/effect is
    // introduced; an existing owner accepts only the same control block/object.
    bool bindSharedNull(const std::shared_ptr<BrushGradientStopDataState>&) noexcept;
    BrushGradientStopsReport resume();
    bool destroyReturnedValue();
    const BrushGradientStopsReport& report() const noexcept { return report_; }
    const std::vector<BrushGradientStopsEffect>& effects() const noexcept { return effects_; }
    std::shared_ptr<const BrushGradientStopDataState> returnedData() const noexcept { return returned_; }
    const std::vector<std::shared_ptr<BrushGradientStopDataState>>& allocatedData() const noexcept { return allocated_; }
private:
    void dropOwnedValues() noexcept;
    std::shared_ptr<const BrushGradientStopsSource> source_;
    std::shared_ptr<BrushGradientStopDataState> sharedNull_, local_, returned_;
    // Diagnostic leases keep records inspectable, never native vector refs alive.
    std::vector<std::shared_ptr<BrushGradientStopDataState>> allocated_;
    BrushGradientStopsReport report_;
    std::vector<BrushGradientStopsEffect> effects_;
};

} // namespace m2m::engine
