// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <array>
#include <cstdint>

#include "m2m/doe_qobject_private_flags.hpp"

using m2m::doe::isolatedQObjectPrivateFlags;

TEST_CASE(doe_qobject_private_flags_isolated_known_bits_follow_native_word_writes)
{
    // These are explicit supplied words, not presumed native allocation defaults.
    CHECK(isolatedQObjectPrivateFlags(0) == 0x60u);
    CHECK(isolatedQObjectPrivateFlags(0xffffffffu) == 0xfffffcf0u);
    CHECK(isolatedQObjectPrivateFlags(0x80000010u) == 0x80000070u);
    CHECK(isolatedQObjectPrivateFlags(0x80u) == 0xe0u);

    constexpr std::array<unsigned, 6> clearedBits{0, 1, 2, 3, 8, 9};
    constexpr std::array<unsigned, 2> forcedBits{5, 6};
    constexpr std::array<std::uint32_t, 5> inputs{
        0, 0xffffffffu, 0x30fu, 0xa5a5a590u, 0x80000010u};
    for (const auto supplied : inputs) {
        const auto result = isolatedQObjectPrivateFlags(supplied);
        for (const auto bit : clearedBits)
            CHECK((result & (std::uint32_t{1} << bit)) == 0);
        for (const auto bit : forcedBits)
            CHECK((result & (std::uint32_t{1} << bit)) != 0);
    }
}

TEST_CASE(doe_qobject_private_flags_isolated_preserves_every_unwritten_bit_and_input)
{
    unsigned preservedBits = 0;
    for (unsigned bit = 0; bit < 32; ++bit) {
        if (bit <= 3 || bit == 5 || bit == 6 || bit == 8 || bit == 9) continue;
        const auto selected = std::uint32_t{1} << bit;
        CHECK((isolatedQObjectPrivateFlags(selected) & selected) == selected);
        CHECK((isolatedQObjectPrivateFlags(~selected) & selected) == 0);
        ++preservedBits;
    }
    CHECK(preservedBits == 24); // Includes bits4/7 and all high bits10..31.

    std::uint32_t supplied = 0xa5a5a590u;
    const auto result = isolatedQObjectPrivateFlags(supplied);
    CHECK(result == 0xa5a5a4f0u);
    CHECK(supplied == 0xa5a5a590u);
    // No native private pointer, Report constructor, TLS or shared-ref state runs.
}

int main() { return testkit::run_all(); }
