// defdbio/dbcore/util.hpp: byte swapping, run-length coding, RMALLOC and the small helpers of
// the DB core (packet p1-12). Fortran calling style: arguments by pointer, arrays 1-based in
// the comments.
#pragma once

#include <cstdint>

namespace def::dbio::dbcore {

// ---- byte order ------------------------------------------------------------------------------

// @src 140407e60  current byte-order mode (DAT_1406277a0)
std::int32_t swap_mode();
// @src 140407e6d  byte-order mode of this platform: always 2 (swap to big-endian)
std::int32_t platform_swap_mode();
// @src 140407e75  set the byte-order mode
void set_swap_mode(const std::int32_t* mode);

// @src 1404445bf  reverse the 4 bytes of one word
void swap4(void* p);
// @src 14044461c  reverse the bytes of `*n` 4-byte words
void swap4_words(void* p, const std::int32_t* n);
// @src 140444955  reverse the 8 bytes of one word
void swap8(void* p);
// @src 1404449af  reverse the bytes of `*n` 8-byte words
void swap8_words(void* p, const std::int32_t* n);

// ---- run-length coding (layout: docs/db-format-spec.md section 5.3) --------------------------

// @src 14044ac31  encode `*n` ints in place; `*stored` = words of the stored array
void rle_encode_int(const std::int32_t* n, std::int32_t* a, std::int32_t* stored);
// @src 14044b0fa  encode `*n` reals in place
void rle_encode_real(const std::int32_t* n, double* a, std::int32_t* stored);
// @src 14044aeeb  decode a stored int array of `*stored` words in place; `*n` = decoded words
void rle_decode_int(const std::int32_t* stored, std::int32_t* a, std::int32_t* n);
// @src 14044b3f5  decode a stored real array in place
void rle_decode_real(const std::int32_t* stored, double* a, std::int32_t* n);

// @src 14044ac11  1-based position inside a block of `*m`: *i mod *m, or *m when that is 0
std::int32_t mod1(const std::int32_t* i, const std::int32_t* m);

// ---- RMALLOC / RFREE --------------------------------------------------------------------------
//
// RMALLOC returns ((p - base) >> 2) + 1, the 1-based index at which the Fortran array `base`
// reaches the new block p. DEF_PRE stores some of these indices as 32-bit integers (the dynamic
// array tables), which only works when the heap lies within 8 GB of the program image. The port
// cannot rely on that, so blocks allocated against a Fortran base array get a virtual address in
// a private 32-bit-index space and are found again through rm_word(); blocks used only as
// scratch memory (RMALLOC against a local) are plain pointers.

// Fortran arrays the DB core allocates against, by their original address.
inline constexpr std::uint64_t kRmIntBase = 0x140613580;  // @data 140613580  INTEGER base
inline constexpr std::uint64_t kRmRealBase = 0x140613584; // @data 140613584  REAL*8 base

// @src 14043c190  RMALLOC: `*n` zeroed 4-byte words against `base`; adds `*n` to `*total`.
// Prints the original's trace lines to stderr when g_rm_trace is set; on failure prints the
// fatal message and exits with -1 as the original does.
std::int64_t rmalloc(const std::int32_t* n, std::uint64_t base, std::int32_t* total);
// RMALLOC for scratch memory (same bookkeeping and messages), returning the block itself.
void* rmalloc_scratch(const std::int32_t* n, std::int32_t* total);
// @src 14043c520  RFREE: frees the block (a scratch pointer or the address rm_word() gave),
// takes `*n` off `*total`. A null pointer does nothing.
void rfree(void* p, const std::int32_t* n, std::int32_t* total);

// Address of word `idx` (1-based, 4-byte units) of the Fortran array at `base`, for an index
// that rmalloc() returned (or that lies inside such a block). Null if no block holds it.
void* rm_word(std::uint64_t base, std::int64_t idx);

} // namespace def::dbio::dbcore
