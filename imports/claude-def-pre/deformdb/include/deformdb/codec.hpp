// Byte-order and run-length codec used by DEFORM .DB files.
// See docs/db-format-spec.md, section "Compression".
#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace deformdb {

// DEFORM writes every integer and real word big-endian.
std::int32_t load_be_i32(const std::byte* p) noexcept;
double load_be_f64(const std::byte* p) noexcept;

// Convert a buffer of big-endian words to host order.
void decode_be_i32(const std::byte* src, std::size_t count, std::int32_t* dst) noexcept;
void decode_be_f64(const std::byte* src, std::size_t count, double* dst) noexcept;

// Character data kept in real*8 slots: the writer swapped the whole 8-byte word,
// so the text is recovered by reading the 8 bytes in reverse order.
void real8_to_chars(const std::byte* src_be, char out[8]) noexcept;

// Run-length decoding of a stored array (stored word count = stored.size()).
// Layout of the stored array s[1..n] (1-based):
//   s[n]   = position of the last run triple's link word (0 = no runs)
//   s[n-1] = decoded length N
//   literal words are copied as is; a run is the triple (value, count, link)
//   where link is the position of the previous run's link word (0 = none).
// Integer arrays keep count/link/N as int32; real arrays keep them as real*8.
std::vector<std::int32_t> rle_decode(std::span<const std::int32_t> stored);
std::vector<double> rle_decode(std::span<const double> stored);

// Encoders that produce the same stored layout as DEFORM's writer
// (runs of 4 or more equal values become triples). Used by tests.
std::vector<std::int32_t> rle_encode(std::span<const std::int32_t> data);
std::vector<double> rle_encode(std::span<const double> data);

}  // namespace deformdb
