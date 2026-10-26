// Passive adapter storage behind Project's existing epoch control block.
// This is not a native allocation, map constructor or cleanup admission.
#pragma once
#include "m2m/model.hpp"
#include <memory>

namespace m2m::detail {
class FeatureEpochBundle;

std::shared_ptr<const engine::ProjectStorageLifetime> makeFeatureEpoch();
// Borrowed identity only; a live owning token must span any use of the result.
// Foreign control blocks and aliases to a different marker are rejected.
FeatureEpochBundle* featureEpoch(
    const std::shared_ptr<const engine::ProjectStorageLifetime>&) noexcept;

struct FeatureContextAccess final {
    // Checks the actual Project anchor's weak epoch without retaining it.
    // This diagnostic does not authorize native construction or continuation.
    static bool hasTypedEpoch(const Project::FeatureContextAnchor&) noexcept;
};
} // namespace m2m::detail
