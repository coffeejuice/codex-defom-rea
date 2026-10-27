#pragma once

#include "type3_optimizer_scalar_state.hpp"

namespace m2m::mst::detail {

// Read-only value view of the recovered DefOptModel getter surface.
// It models prepared values only, not native Qt storage or object lifetime.
class Type3OptimizerDefOptView {
public:
    explicit Type3OptimizerDefOptView(const Type3OptimizerScalarState& state) noexcept;

    std::int32_t value0c() const noexcept;
    std::int32_t value10() const noexcept;
    std::int32_t value30() const noexcept;
    std::int32_t value34() const noexcept;
    std::int32_t value38() const noexcept;
    double value40() const noexcept;
    bool value48() const noexcept;

    std::int32_t numConCount() const noexcept;
    const Type3NumConRowValues* numConAt(std::int32_t index) const noexcept;
    std::int32_t regionCount() const noexcept;
    qt::String regionNameAt(std::int32_t index) const;

private:
    const Type3OptimizerScalarState& state_;
};

}  // namespace m2m::mst::detail
