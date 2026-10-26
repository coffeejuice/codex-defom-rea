#pragma once
#include <array>
#include <memory>
#include <optional>

namespace m2m::engine {
struct Display;

// The actual scene embeds this state before Project construction. Geometry
// supplied by native widgets is unknown here, not a fabricated zero-size view.
std::shared_ptr<Display> createSceneDisplayOwner();

struct DisplayGeometryProfile {
    enum class Source { Unresolved, HistoricalOracleB };
    Source source = Source::Unresolved;
    std::optional<std::array<int, 2>> pixels;
};
DisplayGeometryProfile displayGeometryProfile(const Display&) noexcept;
} // namespace m2m::engine
