#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace deform::recovered {

class KeywordReader;

// Owned alternate geometry at object3998, separate from spatial geometry3c00.
// This models the first GEO2D_AND_MESH2D container's segment collection and
// matrix only. Native borrowed pointers, other mesh collections, allocation
// failure, signed extent overflow, and downstream geometry processing are not
// reconstructed. See analysis/geometry-segments-implementation-contract.json.
class GeometrySegments {
public:
    struct Segment {
        std::int32_t id = 0; // DEF_EDGE wrapper+30; one-based insertion ID.
        std::int32_t format = 0; // OBJ_GEO2D+10:1 has3 values,2 has6.
        std::int32_t count = 0; // OBJ_GEO2D+14.
        std::vector<double> values; // Owned storage corresponding to+18.
        bool flag_0x28 = false;
    };

    // 27fa30 clears the selected collection and sets container+b8; it leaves
    // the4x4 transform intact. Caller decides whether its dispatch clears.
    void clear() noexcept;

    // 39a200: status0 success,2 input failure/invalid format,3 matching failure.
    // No final line discard. Successful groups replace existing segments by
    // index; trailing segments survive. Early errors preserve committed groups.
    // Unsupported native-unsafe domains throw instead of inventing a status.
    [[nodiscard]] int import_segments(KeywordReader& reader, int format, int count);

    // 399e60: validates0<prefix<total, optionally discards two prefix rows,
    // clears segments, creates one empty segment, optionally reads a4x4 matrix,
    // then imports format1 with(total-prefix) rows. Matrix replacement commits
    // only after all four row helpers succeed; earlier clear is not undone.
    [[nodiscard]] int import_transformed(KeywordReader& reader, int total_rows,
                                         int prefix_rows);

    [[nodiscard]] std::span<const Segment> segments() const noexcept { return segments_; }
    [[nodiscard]] const std::array<double, 16>& matrix() const noexcept { return matrix_; }
    [[nodiscard]] bool flag_0xb8() const noexcept { return flag_0xb8_; }

private:
    Segment& ensure_segment(std::size_t index);

    std::vector<Segment> segments_;
    std::array<double, 16> matrix_{
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0};
    bool flag_0xb8_ = true;
};

} // namespace deform::recovered
