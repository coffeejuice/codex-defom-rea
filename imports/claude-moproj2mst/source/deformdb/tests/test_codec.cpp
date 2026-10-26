// Round-trip tests for the RLE codec (no DEFORM files needed).
#include <cstdio>
#include <random>
#include <vector>

#include "deformdb/codec.hpp"

using namespace deformdb;

static int failures = 0;
#define CHECK(c)                                                         \
    do {                                                                 \
        if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } \
    } while (0)

template <class T>
static void roundtrip(const std::vector<T>& data) {
    auto enc = rle_encode(std::span<const T>(data));
    auto dec = rle_decode(std::span<const T>(enc));
    CHECK(dec == data);
}

int main() {
    // Hand-built stored array: 1 2 [7 x5] 3  ->  stored: 1 2 7 5 0 3 | N=8 link=5
    std::vector<std::int32_t> stored = {1, 2, 7, 5, 0, 3, 8, 5};
    auto out = rle_decode(std::span<const std::int32_t>(stored));
    CHECK((out == std::vector<std::int32_t>{1, 2, 7, 7, 7, 7, 7, 3}));

    // Short arrays are stored literally followed by (n, 0).
    std::vector<std::int32_t> lit = {4, 4, 4, 3, 0};
    CHECK((rle_decode(std::span<const std::int32_t>(lit)) == std::vector<std::int32_t>{4, 4, 4}));

    roundtrip(std::vector<std::int32_t>{});
    roundtrip(std::vector<std::int32_t>{5});
    roundtrip(std::vector<std::int32_t>{0, 0, 0, 0, 0, 0, 0, 0, 0});
    roundtrip(std::vector<double>{1.5, 1.5, 1.5, 1.5, 2.0, 2.0, 2.0, 3.0});

    std::mt19937 rng(42);
    for (int trial = 0; trial < 200; ++trial) {
        std::vector<std::int32_t> v(rng() % 500);
        for (auto& x : v) x = (rng() % 4 == 0) ? static_cast<std::int32_t>(rng() % 1000) : 0;
        roundtrip(v);
        std::vector<double> d(v.begin(), v.end());
        roundtrip(d);
    }

    // Big-endian helpers.
    const std::byte be[4] = {std::byte{0}, std::byte{0}, std::byte{1}, std::byte{2}};
    CHECK(load_be_i32(be) == 258);

    std::printf(failures ? "%d failures\n" : "all codec tests passed\n", failures);
    return failures ? 1 : 0;
}
