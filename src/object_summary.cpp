#include "deform/object_summary.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
#include <span>

namespace deform {
namespace {
// REA: 1cd1f0 + 3dd950 copy address tables. 1c7080/1c77c0 assign their indices.
// 1d2230 locates each descriptor; 16caf0 populates the object's basic fields.
// Full dossiers are retained as analysis/import-0x<address>.json.
std::int32_t integer(double value) {
    if (!std::isfinite(value) || std::trunc(value) < 0 ||
        std::trunc(value) > std::numeric_limits<std::int32_t>::max())
        throw database_error("Invalid object-summary header value");
    return static_cast<std::int32_t>(value); // Original C++ conversion truncates.
}

std::size_t descriptor(std::int32_t base, std::int32_t stride, std::size_t slot,
                       std::size_t width, std::size_t available) {
    if (base < 1 || stride < static_cast<std::int32_t>(width))
        throw database_error("Invalid object descriptor address or stride");
    const auto start = static_cast<std::uint64_t>(base - 1) +
        static_cast<std::uint64_t>(stride) * slot;
    if (start > available || width > available - start)
        throw database_error("Object descriptor exceeds decoded array");
    return static_cast<std::size_t>(start);
}

std::int32_t range_count(std::int32_t first, std::int32_t last) {
    const auto count = std::max<std::int64_t>(0, std::int64_t{last} - first + 1);
    if (count > std::numeric_limits<std::int32_t>::max())
        throw database_error("Object range count overflows int32");
    return static_cast<std::int32_t>(count);
}

std::string object_name(std::span<const double> words, std::int32_t source_order) {
    // Reproduce the observed Windows consumer's byte view, even on another host.
    // 324020 + 323de0 + 323ec0 reverse each eight-byte group for source mode 0.
    if (source_order > 3) throw database_error("Unsupported object-name byte order");
    std::string text(64, '\0');
    for (std::size_t i = 0; i < 8; ++i) {
        const auto bits = std::bit_cast<std::uint64_t>(words[i]);
        for (std::size_t j = 0; j < 8; ++j)
            text[i * 8 + j] = static_cast<char>((bits >> (8 * j)) & 255);
    }
    // Original strncpy pads after the first NUL before the byte-order transform.
    const auto zero = text.find('\0');
    if (zero != std::string::npos) std::fill(text.begin() + zero, text.end(), '\0');
    if (source_order == 0)
        for (std::size_t i = 0; i < 64; i += 8)
            std::reverse(text.begin() + i, text.begin() + i + 8);
    const auto padding = [](char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\0';
    };
    // 3239f0 preserves one byte for an entirely blank fixed-width field.
    auto end = text.size();
    while (end > 1 && padding(text[end - 1])) --end;
    text.resize(end);
    const auto terminator = text.find('\0');
    if (terminator != std::string::npos) text.resize(terminator);
    return text;
}
}

std::vector<object_summary> summarize_objects(const loaded_step& step) {
    if (step.record.dimension() != 3 || step.record.version() != 11000)
        throw database_error("Object summaries currently support 3D version 11000 only");
    if (step.header.size() < 6) throw database_error("Object-summary header is too short");
    const auto count = static_cast<std::size_t>(integer(step.header[0]));
    const auto byte_order = integer(step.header[5]);
    const auto& mar = step.integers;
    if (mar.size() < 420 || std::int64_t{mar[1]} - mar[0] != 140 ||
        std::int64_t{mar[2]} - mar[1] != 280)
        throw database_error("Unsupported object-summary address-table layout");
    const std::span<const std::int32_t> integer_addresses(mar.data(), 140);
    const std::span<const std::int32_t> real_addresses(mar.data() + 140, 280);
    if (count) {
        (void)descriptor(integer_addresses[3], integer_addresses[73], count - 1, 13, mar.size());
        (void)descriptor(real_addresses[5], real_addresses[145], count - 1, 12, step.reals.size());
    }
    std::vector<object_summary> result;
    result.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        object_summary object;
        object.id = static_cast<std::int32_t>(i + 1);
        object.integer_descriptor_index =
            descriptor(integer_addresses[3], integer_addresses[73], i, 13, mar.size());
        object.real_descriptor_index =
            descriptor(real_addresses[5], real_addresses[145], i, 12, step.reals.size());
        const auto d = std::span<const std::int32_t>(mar).subspan(object.integer_descriptor_index, 13);
        object.raw_type = d[1];
        object.active = d[0] != 0;
        object.internal_type = 7;
        if (object.active) {
            if (d[0] != 1 && d[0] != 2)
                throw database_error("Unsupported object presence flag");
            object.has_mesh = d[0] == 1;
            constexpr std::int32_t types[]{0, 1, 3, 4, 6};
            if (d[2] == 1) object.internal_type = 2;
            else {
                if (d[1] < 0 || d[1] > 4) throw database_error("Unsupported object type code");
                object.internal_type = types[d[1]];
            }
            if (object.has_mesh) {
                object.node_count = range_count(d[4], d[5]);
                object.element_count = range_count(d[6], d[7]);
            }
            if (std::int64_t{d[9]} - d[8] + 1 > 100)
                throw database_error("Object boundary count exceeds original limit");
            object.name = object_name(
                std::span<const double>(step.reals).subspan(object.real_descriptor_index + 4, 8),
                byte_order);
        }
        result.push_back(std::move(object));
    }
    return result;
}
} // namespace deform
