#include "deformdb/codec.hpp"

#include <bit>
#include <cstring>
#include <stdexcept>

namespace deformdb {

namespace {

template <class T>
T byteswap_value(T v) noexcept {
    unsigned char b[sizeof(T)];
    std::memcpy(b, &v, sizeof(T));
    for (std::size_t k = 0; k < sizeof(T) / 2; ++k) std::swap(b[k], b[sizeof(T) - 1 - k]);
    std::memcpy(&v, b, sizeof(T));
    return v;
}

template <class T>
T load_be(const std::byte* p) noexcept {
    T v;
    std::memcpy(&v, p, sizeof(T));
    if constexpr (std::endian::native == std::endian::little) v = byteswap_value(v);
    return v;
}

int as_int(std::int32_t v) noexcept { return v; }
int as_int(double v) noexcept { return static_cast<int>(v); }

// Shared decoder. Positions follow the 1-based convention of the file format.
template <class T>
std::vector<T> decode(std::span<const T> s) {
    const std::size_t n = s.size();
    if (n < 2) return {};
    auto at = [&](std::size_t pos) -> const T& { return s[pos - 1]; };
    const long long total = as_int(at(n - 1));
    long long link = as_int(at(n));
    if (total < 0) throw std::runtime_error("rle: negative decoded length");

    std::vector<T> out(static_cast<std::size_t>(total));
    if (link == 0 || n <= 2) {
        // Nothing was packed: the first N words are the data.
        if (static_cast<std::size_t>(total) > n - 2) throw std::runtime_error("rle: truncated literal array");
        std::copy(s.begin(), s.begin() + total, out.begin());
        return out;
    }

    long long i = static_cast<long long>(n) - 2;  // current stored position
    long long j = total;                           // current output position
    while (i > 0 && j > 0) {
        if (i == link) {
            if (i < 3) throw std::runtime_error("rle: bad run link");
            const T value = at(static_cast<std::size_t>(i - 2));
            const long long count = as_int(at(static_cast<std::size_t>(i - 1)));
            link = as_int(at(static_cast<std::size_t>(i)));
            if (count < 0 || count > j) throw std::runtime_error("rle: run overflows output");
            for (long long k = 0; k < count; ++k) out[static_cast<std::size_t>(--j)] = value;
            i -= 3;
        } else {
            out[static_cast<std::size_t>(--j)] = at(static_cast<std::size_t>(i));
            --i;
        }
    }
    return out;
}

template <class T>
std::vector<T> encode(std::span<const T> d) {
    const std::size_t n = d.size();
    std::vector<T> out;
    out.reserve(n + 2);
    long long link = 0;
    std::size_t k = 0;
    while (k < n) {
        std::size_t run = 1;
        while (k + run < n && d[k + run] == d[k]) ++run;
        if (run >= 4 && n >= 4) {
            out.push_back(d[k]);
            out.push_back(static_cast<T>(run));
            out.push_back(static_cast<T>(link));
            link = static_cast<long long>(out.size());
        } else {
            for (std::size_t r = 0; r < run; ++r) out.push_back(d[k]);
        }
        k += run;
    }
    out.push_back(static_cast<T>(n));
    out.push_back(static_cast<T>(link));
    return out;
}

}  // namespace

std::int32_t load_be_i32(const std::byte* p) noexcept { return load_be<std::int32_t>(p); }
double load_be_f64(const std::byte* p) noexcept { return load_be<double>(p); }

void decode_be_i32(const std::byte* src, std::size_t count, std::int32_t* dst) noexcept {
    for (std::size_t k = 0; k < count; ++k) dst[k] = load_be<std::int32_t>(src + 4 * k);
}

void decode_be_f64(const std::byte* src, std::size_t count, double* dst) noexcept {
    for (std::size_t k = 0; k < count; ++k) dst[k] = load_be<double>(src + 8 * k);
}

void real8_to_chars(const std::byte* src_be, char out[8]) noexcept {
    for (int k = 0; k < 8; ++k) out[k] = static_cast<char>(src_be[7 - k]);
}

std::vector<std::int32_t> rle_decode(std::span<const std::int32_t> stored) { return decode(stored); }
std::vector<double> rle_decode(std::span<const double> stored) { return decode(stored); }
std::vector<std::int32_t> rle_encode(std::span<const std::int32_t> data) { return encode(data); }
std::vector<double> rle_encode(std::span<const double> data) { return encode(data); }

}  // namespace deformdb
