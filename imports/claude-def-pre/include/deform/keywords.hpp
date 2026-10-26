#pragma once

// Keyword registry: every keyword name the DEFORM v11 text pre-processor knows,
// with its group. Generated from docs/re/keyword_registry.tsv.

#include <cstdint>
#include <span>
#include <string_view>

namespace dk {

enum class KeywordGroup : std::uint8_t {
    Unknown = 0,
    SimulationControl = 1,
    Object = 2,
    InterObject = 3,
    Material = 4,
    PhaseTransformation = 5,
    Command = 6,
    Dimension = 7,
};

struct KeywordInfo {
    std::int16_t id;
    std::string_view name;
    KeywordGroup group;
    std::int16_t flag;
};

// All registered keywords, ordered by id.
[[nodiscard]] std::span<const KeywordInfo> keywords() noexcept;

// nullptr when the name is not registered.
[[nodiscard]] const KeywordInfo *find_keyword(std::string_view name) noexcept;

[[nodiscard]] std::string_view group_name(KeywordGroup group) noexcept;

} // namespace dk
