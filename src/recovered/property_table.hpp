#pragma once

#include "keyword_reader.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace deform::recovered {

// Owned scientific-table state from12d050/12d2a0, not a native0x78-byte ABI
// mirror. Rank and mode are owned here; TimeTableControls keeps its root aliases.
// Foreign/borrowed data pointers and the nonnull native cache68 are excluded.
// Unwritten padding70 and other padding bytes have no fabricated defaults.
struct PropertyTable {
    explicit PropertyTable(std::int32_t type = 0);
    // Owned value copies support C++ observation snapshots. They do not claim
    // reconstruction of native table-copy or borrowed-pointer semantics.
    PropertyTable(const PropertyTable&) = default;
    PropertyTable& operator=(const PropertyTable&) = default;
    PropertyTable(PropertyTable&&) noexcept = default;
    PropertyTable& operator=(PropertyTable&&) noexcept = default;

    std::int32_t type_0x00 = 0;
    std::int32_t rank_0x04 = 0;
    double constant_0x08 = 0.0;
    std::array<std::int32_t, 3> selectors_0x10{};
    std::array<std::int32_t, 3> counts_0x1c{};
    std::array<std::vector<double>, 3> axes;
    std::int32_t values_per_point_0x40 = 1;
    std::vector<double> dependent;
    std::int32_t flag_0x50 = 0;
    std::int32_t preserved_0x54 = 0;
    std::int32_t mode_0x58 = 0;
    std::uint64_t preserved_0x60 = 0;

    //12d2a0: release old storage, apply the exact native type profile, and
    // allocate except types65/66/67/51. Unknown type IDs get the native base.
    void initialize(std::int32_t type);
    //12fc70 saves metadata54/60 across12d2a0. Set false for direct12d2a0.
    void reset_type(std::int32_t type, bool preserve_metadata = true);
    //12fad0: release, clamp negative counts to0 and width<1 to1, then calloc.
    // Logical counts and actual vector extents remain distinct between calls.
    void allocate_zeroed();
    [[nodiscard]] std::int32_t dependent_count() const;

private:
    void release_owned() noexcept;
};

struct PropertyHeaderRead {
    std::int32_t selector = 0;
    std::array<double, 3> values{};
    int conversions = -1;
    int status = 1;
};
struct PropertyReadResult {
    PropertyHeaderRead header;
    int status = 1;
    bool payload_invoked = false;
};

//41b270's header: one fgets1024, commas become spaces, zeroed
// %i %Flf %Flf %Flf. Only EOF or aggregate scan-1 fails.
[[nodiscard]] PropertyHeaderRead read_property_header(KeywordReader& reader);
//41a800: allocate, then rank0 vector/rank1 interleaved/rank2-3 staged values.
// EOF retains all prior direct stores. Staged dependent data commits only after
// all conversions; conversion0 there is explicitly unsupported (native malloc
// bytes are uninitialized). Native EOF scratch leaks are not reproduced.
[[nodiscard]] int read_property_values(KeywordReader& reader, PropertyTable& table);
//41ac10: allocate a rank0 positive-width vector, preserve direct partial reads,
// and leave the trailing line unconsumed. Unlike41a800, width1 is read too.
[[nodiscard]] int read_property_vector(KeywordReader& reader, PropertyTable& table);
//41b270: failed header preserves table/auxiliary; otherwise reset type0 while
// preserving54/60, set selector profile, optionally write auxiliary, read values.
// Caller-specific HEATCP/PMEAB/COARSE side effects remain in the importer.
[[nodiscard]] PropertyReadResult read_property(KeywordReader& reader,
    PropertyTable& table, int auxiliary_flag = 0, double* auxiliary = nullptr);

} // namespace deform::recovered
