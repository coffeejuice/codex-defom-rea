#include "deform/packed_array.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

template <typename Function>
void rejects(Function function) {
    bool rejected = false;
    try {
        function();
    } catch (const deform::packed_array_error&) {
        rejected = true;
    }
    require(rejected, "Invalid encoding was accepted");
}

void integer_tests() {
    const std::array<std::int32_t, 5> literal{-3, 1, 4, 3, 0};
    require(deform::unpack_integers(literal) ==
                std::vector<std::int32_t>({-3, 1, 4}),
            "Literal array or metadata trimming");
    const std::array<std::int32_t, 6> trailing{11, 22, 33, 44, 2, 0};
    require(deform::unpack_integers(trailing) ==
                std::vector<std::int32_t>({11, 22}),
            "Zero marker must preserve the reported prefix");
    const std::array<std::int32_t, 2> empty{0, 0};
    require(deform::unpack_integers(empty).empty(), "Empty payload");

    // Positions 2..4 encode five copies of 7. Position 4 links to no earlier run.
    const std::array<std::int32_t, 7> one{1, 7, 5, 0, 2, 7, 4};
    require(deform::unpack_integers(one) ==
                std::vector<std::int32_t>({1, 7, 7, 7, 7, 7, 2}),
            "Run surrounded by literals");
    // Run link at position 8 points back to the run link at position 4.
    const std::array<std::int32_t, 11> two{1, -7, 5, 0, 2, 9, 4, 4, 3, 12, 8};
    require(deform::unpack_integers(two) ==
                std::vector<std::int32_t>({1, -7, -7, -7, -7, -7, 2, 9, 9, 9, 9, 3}),
            "Linked runs, negative payload and trailing literal");
    const std::array<std::int32_t, 5> whole{42, 6, 0, 6, 3};
    require(deform::unpack_integers(whole) == std::vector<std::int32_t>(6, 42),
            "Entire payload represented by one run");

    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 1>{0}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 3>{1, 2, 0}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 5>{1, 5, 0, 5, 4}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 5>{1, 5, 3, 5, 3}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 5>{1, 6, 0, 5, 3}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 5>{1, 4, 0, 5, 3}); });
    rejects([] { (void)deform::unpack_integers(std::array<std::int32_t, 2>{-1, 0}); });
}

void real_tests() {
    constexpr std::uint64_t nan_bits = 0x7ff8000000000123ULL;
    constexpr std::uint64_t signaling_nan_bits = 0x7ff0000000000123ULL;
    constexpr std::uint64_t negative_zero_bits = 0x8000000000000000ULL;
    const double nan = std::bit_cast<double>(nan_bits);
    const double negative_zero = std::bit_cast<double>(negative_zero_bits);
    const std::array<double, 9> encoded{nan, 5, 0, negative_zero, 4, 3, 2.25, 10, 6};
    const auto decoded = deform::unpack_reals(encoded);
    require(decoded.size() == 10, "Real decoded length");
    for (std::size_t i = 0; i < 5; ++i) {
        require(std::bit_cast<std::uint64_t>(decoded[i]) == nan_bits,
                "Run must preserve NaN payload bits");
    }
    for (std::size_t i = 5; i < 9; ++i) {
        require(std::bit_cast<std::uint64_t>(decoded[i]) == negative_zero_bits,
                "Run must preserve signed zero");
    }
    require(decoded.back() == 2.25, "Real trailing literal");

    const std::array<double, 4> raw{
        std::bit_cast<double>(signaling_nan_bits), negative_zero, 2, 0};
    const auto copied = deform::unpack_reals(raw);
    require(std::bit_cast<std::uint64_t>(copied[0]) == signaling_nan_bits,
            "Literal signaling NaN must remain unchanged");
    require(std::bit_cast<std::uint64_t>(copied[1]) == negative_zero_bits,
            "Literal signed zero must remain unchanged");

    // Metadata casts in the original truncate rather than round.
    const std::array<double, 5> fractional{1.25, 4.9, 0.9, 4.9, 3.9};
    require(deform::unpack_reals(fractional) == std::vector<double>(4, 1.25),
            "Real metadata truncation");
    rejects([&] { (void)deform::unpack_reals(std::array<double, 2>{nan, 0}); });
    rejects([] { (void)deform::unpack_reals(std::array<double, 2>{0, 2147483648.0}); });
    rejects([] { (void)deform::unpack_reals(
        std::array<double, 2>{0, std::numeric_limits<double>::infinity()}); });
}

} // namespace

int main() {
    try {
        integer_tests();
        real_tests();
        std::cout << "packed_array tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
