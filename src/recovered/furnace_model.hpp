#pragma once

#include "property_table.hpp"
#include <cstdint>
#include <vector>

namespace deform::recovered {

struct FurnaceParameter {
    explicit FurnaceParameter(int type) : table(type) {}
    PropertyTable table;
    // Empty means all allocated bytes are known. Packed text can copy an
    // uninitialized stack buffer after its first failed line read; mark those
    // bytes explicitly rather than claiming their zero placeholders are native.
    std::vector<std::uint8_t> known_dependent_bytes;
};

struct FurnaceCollection {
    bool enabled_0xb10 = false;
    // Native next pointers at table+60 are represented by ordered ownership,
    // not copied into PropertyTable::preserved_0x60 as synthetic addresses.
    std::vector<FurnaceParameter> parameters;

    void reset_entries(int requested); //375600: at least21 typed entries.
    void initialize_defaults(int process_unit_mode); //375e90,3769f0.
    //136860 maps every index<=1 to the first linked-list entry.
    [[nodiscard]] FurnaceParameter* lookup(int index) noexcept;
};

struct FurnaceHeaderRead {
    std::array<int, 3> integers{};
    std::array<double, 2> reals{};
    int conversions = -1;
    int status = 1;
};

[[nodiscard]] FurnaceHeaderRead read_furnace_header(KeywordReader& reader);
//41ace0 and41b060 ignore header-reader failure; the parent ENVTMP also
// ignores these helpers' return statuses and proceeds to subsequent entries.
[[nodiscard]] int read_furnace_parameter(KeywordReader& reader, FurnaceParameter& parameter);
[[nodiscard]] int read_furnace_text(KeywordReader& reader, FurnaceParameter& parameter);
[[nodiscard]] int native_furnace_integer(double value) noexcept;

} // namespace deform::recovered
