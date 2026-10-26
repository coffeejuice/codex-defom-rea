// XXH64 (Yann Collet's xxHash, 64-bit variant, seed 0), streaming form.
#include <cstring>

#include "defcmp/defcmp.hpp"

namespace defcmp {

namespace {

constexpr std::uint64_t P1 = 11400714785074694791ULL;
constexpr std::uint64_t P2 = 14029467366897019727ULL;
constexpr std::uint64_t P3 = 1609587929392839161ULL;
constexpr std::uint64_t P4 = 9650029242287828579ULL;
constexpr std::uint64_t P5 = 2870177450012600261ULL;

inline std::uint64_t rotl(std::uint64_t x, int r) { return (x << r) | (x >> (64 - r)); }

inline std::uint64_t read64(const unsigned char* p) {
    std::uint64_t v;
    std::memcpy(&v, p, 8);   // little-endian host
    return v;
}

inline std::uint32_t read32(const unsigned char* p) {
    std::uint32_t v;
    std::memcpy(&v, p, 4);
    return v;
}

inline std::uint64_t round(std::uint64_t acc, std::uint64_t input) {
    acc += input * P2;
    acc = rotl(acc, 31);
    return acc * P1;
}

inline std::uint64_t merge_round(std::uint64_t acc, std::uint64_t val) {
    acc ^= round(0, val);
    return acc * P1 + P4;
}

}  // namespace

Hasher::Hasher() : v_{P1 + P2, P2, 0, 0 - P1}, buf_{} {}

void Hasher::update(const void* data, std::size_t bytes) {
    auto p = static_cast<const unsigned char*>(data);
    total_ += bytes;
    if (buffered_ + bytes < 32) {
        std::memcpy(buf_ + buffered_, p, bytes);
        buffered_ += bytes;
        return;
    }
    if (buffered_ > 0) {
        const std::size_t fill = 32 - buffered_;
        std::memcpy(buf_ + buffered_, p, fill);
        for (int i = 0; i < 4; ++i) v_[i] = round(v_[i], read64(buf_ + 8 * i));
        p += fill;
        bytes -= fill;
        buffered_ = 0;
    }
    while (bytes >= 32) {
        for (int i = 0; i < 4; ++i) v_[i] = round(v_[i], read64(p + 8 * i));
        p += 32;
        bytes -= 32;
    }
    std::memcpy(buf_, p, bytes);
    buffered_ = bytes;
}

Digest Hasher::finish() const {
    std::uint64_t h;
    if (total_ >= 32) {
        h = rotl(v_[0], 1) + rotl(v_[1], 7) + rotl(v_[2], 12) + rotl(v_[3], 18);
        for (auto v : v_) h = merge_round(h, v);
    } else {
        h = P5;
    }
    h += total_;
    const unsigned char* p = buf_;
    std::size_t n = buffered_;
    while (n >= 8) {
        h ^= round(0, read64(p));
        h = rotl(h, 27) * P1 + P4;
        p += 8;
        n -= 8;
    }
    if (n >= 4) {
        h ^= static_cast<std::uint64_t>(read32(p)) * P1;
        h = rotl(h, 23) * P2 + P3;
        p += 4;
        n -= 4;
    }
    while (n > 0) {
        h ^= (*p) * P5;
        h = rotl(h, 11) * P1;
        ++p;
        --n;
    }
    h ^= h >> 33;
    h *= P2;
    h ^= h >> 29;
    h *= P3;
    h ^= h >> 32;
    return h;
}

Digest hash_bytes(std::string_view data) {
    Hasher h;
    h.update(data.data(), data.size());
    return h.finish();
}

std::string digest_hex(Digest d) {
    static const char* hex = "0123456789abcdef";
    std::string s(16, '0');
    for (int i = 15; i >= 0; --i, d >>= 4) s[static_cast<std::size_t>(i)] = hex[d & 15];
    return s;
}

std::optional<Digest> parse_digest(std::string_view hex) {
    if (hex.empty() || hex.size() > 16) return std::nullopt;
    Digest d = 0;
    for (char c : hex) {
        int v;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
        else return std::nullopt;
        d = (d << 4) | static_cast<Digest>(v);
    }
    return d;
}

}  // namespace defcmp
