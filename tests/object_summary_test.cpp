#include "deform/object_summary.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

// Authored from analysis/object-summary-schema.json and the retained
// 1d2230/16caf0 import routines, independently of the captured seven-object DB.
// Addresses are deliberately different; descriptors end exactly at the buffers.
constexpr std::size_t integer_start = 449;
constexpr std::size_t integer_stride = 19;
constexpr std::size_t real_start = 23;
constexpr std::size_t real_stride = 17;

void name_bytes(deform::loaded_step& step, std::size_t object,
                std::string_view value) {
    require(value.size() <= 64, "test name exceeds fixed field");
    std::array<unsigned char, 64> bytes{};
    bytes.fill(' ');
    std::copy(value.begin(), value.end(), bytes.begin());
    for (std::size_t i = 0; i < 8; ++i) {
        std::uint64_t bits = 0;
        for (std::size_t j = 0; j < 8; ++j)
            bits |= std::uint64_t{bytes[i * 8 + j]} << (j * 8);
        step.reals.at(real_start + real_stride * object + 4 + i) =
            std::bit_cast<double>(bits);
    }
}

deform::loaded_step fixture() {
    deform::loaded_step step;
    step.record.effective_dimension = 3;
    step.record.effective_version = 11000;
    // Raw legacy slots must not override the separately resolved attributes.
    step.record.words[7] = 2;
    step.record.words[8] = 6000;
    step.header = {2.0, 0.0, 0.0, 0.0, 0.0, 2.0};
    step.integers.resize(integer_start + integer_stride + 13, -987);
    step.reals.resize(real_start + real_stride + 12, -123.0);
    // DBMADR copies from MAR start; these values establish table lengths only.
    step.integers[0] = 17;
    step.integers[1] = 157;
    step.integers[2] = 437;
    step.integers[3] = static_cast<std::int32_t>(integer_start + 1);
    step.integers[73] = static_cast<std::int32_t>(integer_stride);
    step.integers[140 + 5] = static_cast<std::int32_t>(real_start + 1);
    step.integers[140 + 145] = static_cast<std::int32_t>(real_stride);
    const std::array<std::int32_t, 13> first{
        1, 3, 0, 91, 40, 43, 70, 75, 6, 105, 92, 93, 8};
    const std::array<std::int32_t, 13> second{
        1, 4, 0, 94, 100, 106, 800, 801, 2, 1, 95, 96, 4};
    std::copy(first.begin(), first.end(), step.integers.begin() + integer_start);
    std::copy(second.begin(), second.end(),
              step.integers.begin() + integer_start + integer_stride);
    name_bytes(step, 0, "  First object\t\r\n");
    name_bytes(step, 1, "Second object");
    return step;
}

template<class Mutation>
void rejects(Mutation change, const char* message) {
    auto step = fixture();
    change(step);
    try {
        (void)deform::summarize_objects(step);
    } catch (const deform::database_error& error) {
        // These are checked reconstruction boundaries, not original status codes.
        require(error.original_code() == 0, "invented original error code");
        return;
    }
    throw std::runtime_error(message);
}

void moved_tables_and_ranges() {
    auto step = fixture();
    const auto objects = deform::summarize_objects(step);
    require(objects.size() == 2, "object count");
    const auto& first = objects[0];
    const auto& second = objects[1];
    require(first.id == 1 && second.id == 2, "one-based source slots");
    require(first.active && first.has_mesh && second.active && second.has_mesh,
            "active mesh status");
    require(first.raw_type == 3 && first.internal_type == 4 &&
            second.raw_type == 4 && second.internal_type == 6,
            "application types are not KEY codes");
    require(first.node_count == 4 && first.element_count == 6 &&
            second.node_count == 7 && second.element_count == 2,
            "inclusive global range counts");
    require(first.name == "  First object" && second.name == "Second object",
            "names preserve leading and trim trailing whitespace");
    require(first.integer_descriptor_index == integer_start &&
            second.integer_descriptor_index == integer_start + integer_stride &&
            first.real_descriptor_index == real_start &&
            second.real_descriptor_index == real_start + real_stride,
            "descriptor addresses and independent strides");

    step.integers[integer_start + 4] = 20;
    step.integers[integer_start + 5] = 19;
    step.integers[integer_start + 6] = 9;
    step.integers[integer_start + 7] = 3;
    const auto empty = deform::summarize_objects(step);
    require(empty[0].node_count == 0 && empty[0].element_count == 0,
            "reversed ranges clamp to zero");
}

void status_and_type_rules() {
    constexpr std::array<std::int32_t, 5> expected{0, 1, 3, 4, 6};
    for (std::size_t raw = 0; raw < expected.size(); ++raw) {
        auto step = fixture();
        step.integers[integer_start + 1] = static_cast<std::int32_t>(raw);
        require(deform::summarize_objects(step)[0].internal_type == expected[raw],
                "complete recovered type mapping");
        step.integers[integer_start + 2] = 1;
        require(deform::summarize_objects(step)[0].internal_type == 2,
                "special type override");
    }
    auto step = fixture();
    step.integers[integer_start] = 2;
    auto objects = deform::summarize_objects(step);
    require(objects[0].active && !objects[0].has_mesh &&
            objects[0].node_count == 0 && objects[0].element_count == 0 &&
            objects[0].name == "  First object",
            "status two retains name and type but zeroes mesh counts");

    step.integers[integer_start] = 0;
    step.integers[integer_start + 1] = -444;
    step.integers[integer_start + 2] = 1;
    step.integers[integer_start + 9] = 2000;
    objects = deform::summarize_objects(step);
    require(!objects[0].active && !objects[0].has_mesh &&
            objects[0].internal_type == 7 && objects[0].raw_type == -444 &&
            objects[0].name.empty() && objects[0].node_count == 0 &&
            objects[0].element_count == 0 && objects[1].id == 2,
            "inactive early exit precedes type override and boundary check");
}

void names_and_header_conversion() {
    auto step = fixture();
    step.header[0] = 1.999;
    require(deform::summarize_objects(step).size() == 1, "count truncates");
    step.header[0] = 0.99;
    require(deform::summarize_objects(step).empty(), "fractional count below one");
    step.header[0] = 2.0;
    name_bytes(step, 0, "");
    name_bytes(step, 1, std::string_view("A\0discard", 9));
    auto objects = deform::summarize_objects(step);
    require(objects[0].name == " ", "all-space field retains first byte");
    require(objects[1].name == "A", "strncpy stops at embedded NUL");
    name_bytes(step, 0, std::string(64, 'x'));
    name_bytes(step, 1, std::string(64, '\0'));
    objects = deform::summarize_objects(step);
    require(objects[0].name == std::string(64, 'x'), "full-width name");
    require(objects[1].name.empty(), "all-NUL field");

    // The original source mode 0 reverses each eight-byte group. Use literal
    // source bytes, not an encoder that mirrors the implementation.
    step.header[5] = 0.0;
    name_bytes(step, 0, "HGFEDCBAponmlkji");
    name_bytes(step, 1, std::string_view("AB\0discard", 10));
    objects = deform::summarize_objects(step);
    require(objects[0].name == "ABCDEFGHijklmnop", "source mode zero reversal");
    require(objects[1].name.empty(), "NUL padding precedes source reversal");
    for (const double mode : {1.0, 2.8, 3.0}) {
        step.header[5] = mode;
        name_bytes(step, 0, "Native");
        require(deform::summarize_objects(step)[0].name == "Native",
                "other supported character modes retain native byte view");
    }
}

void checked_boundaries() {
    rejects([](auto& s) { s.header.resize(5); }, "short header accepted");
    rejects([](auto& s) { s.header[0] = -1; }, "negative count accepted");
    rejects([](auto& s) { s.header[0] = std::numeric_limits<double>::quiet_NaN(); },
            "NaN count accepted");
    rejects([](auto& s) { s.header[0] = std::numeric_limits<double>::infinity(); },
            "infinite count accepted");
    rejects([](auto& s) { s.header[0] = 2147483648.0; }, "count overflow accepted");
    rejects([](auto& s) { s.header[0] = 3; }, "missing final object accepted");
    rejects([](auto& s) { s.header[5] = 4; }, "unknown byte mode accepted");
    rejects([](auto& s) { s.record.effective_version = 10000; },
            "unsupported effective version accepted");
    rejects([](auto& s) { s.record.effective_dimension = 2; },
            "unsupported effective dimension accepted");
    rejects([](auto& s) { s.integers.resize(419); }, "short tables accepted");
    rejects([](auto& s) { ++s.integers[1]; }, "wrong integer table length accepted");
    rejects([](auto& s) { --s.integers[2]; }, "wrong real table length accepted");
    rejects([](auto& s) { s.integers[3] = 0; }, "zero integer base accepted");
    rejects([](auto& s) { s.integers[145] = -1; }, "negative real base accepted");
    rejects([](auto& s) { s.integers[73] = 12; }, "short integer stride accepted");
    rejects([](auto& s) { s.integers[285] = 11; }, "short real stride accepted");
    rejects([](auto& s) { s.integers[73] = std::numeric_limits<std::int32_t>::max(); },
            "large descriptor stride accepted");
    rejects([](auto& s) { s.integers.pop_back(); }, "truncated descriptor accepted");
    rejects([](auto& s) { s.reals.pop_back(); }, "truncated name accepted");
    rejects([](auto& s) { s.integers[integer_start] = 3; },
            "unestablished status accepted");
    rejects([](auto& s) { s.integers[integer_start + 1] = 5; },
            "unestablished raw type accepted");
    rejects([](auto& s) { ++s.integers[integer_start + 9]; },
            "boundary count 101 accepted");
    rejects([](auto& s) {
        s.integers[integer_start + 4] = std::numeric_limits<std::int32_t>::min();
        s.integers[integer_start + 5] = std::numeric_limits<std::int32_t>::max();
    }, "overflowing inclusive range accepted");
}
} // namespace

int main() {
    try {
        moved_tables_and_ranges();
        status_and_type_rules();
        names_and_header_conversion();
        checked_boundaries();
        std::cout << "object summary tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "object summary test failure: " << error.what() << '\n';
        return 1;
    }
}
