#include "geometry_segments.hpp"

#include "keyword_reader.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace deform::recovered {
namespace {

std::size_t checked_value_count(int count, int width) {
    if (count < 0)
        throw std::invalid_argument("negative geometry segment extent is outside the compatibility domain");
    const auto values = static_cast<std::int64_t>(count) * width;
    // Both the native count*width and the275fa0 memset byte extent use signed
    // 32-bit arithmetic. This bound excludes wraparound before any allocation.
    if (values > std::numeric_limits<std::int32_t>::max() / 8)
        throw std::out_of_range("geometry segment byte extent exceeds signed32 compatibility domain");
    return static_cast<std::size_t>(values);
}

int scan_status(int conversions) {
    if (conversions == -1) return 2;
    return conversions < 1 ? 3 : 0;
}

} // namespace

void GeometrySegments::clear() noexcept {
    segments_.clear();
    flag_0xb8_ = true;
}

GeometrySegments::Segment& GeometrySegments::ensure_segment(std::size_t index) {
    if (index >= static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
        throw std::out_of_range("geometry segment ID exceeds signed32 compatibility domain");
    while (segments_.size() <= index) {
        Segment segment;
        segment.id = static_cast<std::int32_t>(segments_.size() + 1);
        segments_.push_back(std::move(segment));
        flag_0xb8_ = true;
    }
    return segments_[index];
}

int GeometrySegments::import_segments(KeywordReader& reader, int format, int count) {
    if (format != 1 && format != 2) return 2;
    const int width = format == 1 ? 3 : 6;
    const auto extent = checked_value_count(count, width);
    // Native temporary storage is uninitialized. C++ zeros are unobservable:
    // only rows whose every scan succeeded can reach a committed segment.
    std::vector<double> temporary(extent);
    std::size_t group_rows = 0;
    std::size_t segment_index = 0;
    int completed_rows = 0;
    while (completed_rows < count) {
        const auto label = reader.selector_integer();
        if (const auto status = scan_status(label.conversions)) return status;
        const auto base = group_rows * static_cast<std::size_t>(width);
        for (int column = 0; column < width; ++column) {
            const auto read = reader.stream_real(temporary[base + column]);
            temporary[base + column] = read.values[0];
            if (const auto status = scan_status(read.conversions)) return status;
        }
        ++group_rows;
        const bool separator = label.values[0] == -1;
        if (separator || completed_rows == count - 1) {
            auto& segment = ensure_segment(segment_index);
            const auto retained_rows = group_rows - static_cast<std::size_t>(separator);
            segment.count = static_cast<std::int32_t>(retained_rows);
            segment.format = format;
            const auto retained_values = retained_rows * static_cast<std::size_t>(width);
            // Native virtual allocation releases old data after count/format
            // stores. Resource failures have no compatibility claim here.
            segment.values.clear();
            segment.values.resize(retained_values, 0.0);
            std::copy_n(temporary.begin(), retained_values, segment.values.begin());
            ++segment_index;
            group_rows = 0;
        }
        // A separator's numeric row is discarded and does not count toward
        // the advertised rows, including an initial/consecutive separator.
        if (!separator) ++completed_rows;
    }

    if (format == 2) {
        // Native code assumes stride6 for EVERY stored segment, even untouched
        // trailing ones. Detect an unsafe retained format1 allocation before
        // any conversion rather than reproducing an out-of-bounds access.
        for (const auto& segment : segments_) {
            const auto needed = checked_value_count(segment.count, 6);
            if (segment.values.size() < needed)
                throw std::invalid_argument("format2 conversion over retained narrow geometry storage is unsupported");
        }
        for (auto& segment : segments_)
            for (int row = 0; row < segment.count; ++row)
                segment.values[static_cast<std::size_t>(row) * 6 + 5] *=
                    0.017453292519943295;
    }
    // Native errors leak temporary storage; private C++ ownership releases it.
    // No file read or geometry mutation is added by that resource cleanup.
    return 0;
}

int GeometrySegments::import_transformed(KeywordReader& reader, int total_rows,
                                         int prefix_rows) {
    if (total_rows < 1 || prefix_rows < 1 || prefix_rows >= total_rows) return 2;
    (void)checked_value_count(total_rows - prefix_rows, 3);
    if (prefix_rows == 2 || prefix_rows == 6) {
        // Native destinations alias one discarded scalar. The established
        // coordinate helper has the same scan and stream consumption here.
        if (reader.coordinates3().status != 0) return 2;
        if (reader.coordinates3().status != 0) return 2;
    }
    clear();
    (void)ensure_segment(0);
    if (prefix_rows == 4 || prefix_rows == 6) {
        auto replacement = matrix_;
        for (std::size_t row = 0; row < 4; ++row) {
            const auto read = reader.matrix_row();
            std::copy(read.values.begin(), read.values.end(), replacement.begin() + row * 4);
            if (read.status != 0) return 2;
        }
        matrix_ = replacement;
    }
    return import_segments(reader, 1, total_rows - prefix_rows);
}

} // namespace deform::recovered
