#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace m2m::engine { struct GuiModel; }

namespace m2m::doe {

struct PropertyTableQuery {
    std::int32_t row = 0;
    std::int32_t column = 0;
};

// Actual GuiModel+230 SfObjectManager projection. The constructor creates a
// QStandardItemModel root with zero rows/columns. Populated item behavior and
// Qt selection/signal machinery remain unrecovered, on this same owner.
class PropertyTableState {
public:
    explicit PropertyTableState(engine::GuiModel& owner) noexcept : owner_(&owner) {}

    engine::GuiModel* owner() const noexcept { return owner_; } //native raw+50
    bool constructorEmpty() const noexcept { return constructorEmpty_; }
    void markPopulationUnrecovered() noexcept { constructorEmpty_ = false; }

    // Shipped QtGui65529390: item() neither creates nor grows children. Native
    // negative indices always return null; constructor-empty indices do too.
    // An otherwise unqualified query guards before item/check-state effects.
    bool itemIsAbsent(std::int32_t row, std::int32_t column) const;

    // Adapter diagnostics only, not fields of native SfObjectManager. Retains
    // the first 256 actual lookups; truncation never refuses semantic work.
    std::span<const PropertyTableQuery> queryPrefix() const noexcept
    { return {queries_.data(), queryPrefixSize_}; }
    std::uint64_t queryCount() const noexcept { return queryCount_; }
    std::optional<PropertyTableQuery> lastQuery() const noexcept { return lastQuery_; }
    bool queryReportTruncated() const noexcept { return queryReportTruncated_; }

private:
    engine::GuiModel* const owner_;
    bool constructorEmpty_ = true;
    mutable std::array<PropertyTableQuery, 256> queries_{};
    mutable std::size_t queryPrefixSize_ = 0;
    mutable std::uint64_t queryCount_ = 0;
    mutable std::optional<PropertyTableQuery> lastQuery_;
    mutable bool queryReportTruncated_ = false;
};

} // namespace m2m::doe
