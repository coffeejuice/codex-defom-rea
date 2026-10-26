// defdbio/dbcore/globals_p1_12.hpp: the static state of datarm23 / DBOperations23 (packet p1-12).
//
// The Fortran DB module keeps everything in fixed storage. The block 0x140b45b80..0x140b6cb88 is
// one struct with every member at its address (port-conventions section 5), because the original
// indexes past the end of some arrays into the next member (FUN_1403b92bc reads the index table
// with a word position up to 255 for the 200th index record, which lands in the chunk buffer that
// follows). Code that does such arithmetic goes through words(), a flat view of the block.
//
// Loop counters and temporaries the Fortran compiler put in static storage (0x140707134 ..
// 0x140707838, 0x14070e184 .. 0x14070e218) are locals in the port: every routine sets them before
// use and no other routine reads them. The exception is DAT_140707758 (FUN_1403bcf76 compares
// step numbers against it but nothing writes it), kept as a constant 0 there.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace def::dbio::dbcore {

// @data 140b45b80  datarm23 COMMON: region tables, dynamic-array tables, index, chunk, record 2
struct DbCommon {
    // 140b45b80 (the struct's first member)  A region table (506 words: count, max length, 4 words per region)
    std::int32_t a_regions[0x1fa];
    std::byte pad_7e8[0x800 - 0x7e8];
    // @data 140b46380  MAR region table (234 words, same layout)
    std::int32_t mar_regions[0xea];
    std::byte pad_ba8[0xbc0 - 0xba8];
    // @data 140b46740  real dynamic-array table, 10 words per array (size, RMALLOC index,
    // stored words, record, ...); row k (1-based) at 0x140b46718 + k*0x28
    std::int32_t dyn_real[200][10];
    // @data 140b48680  integer dynamic-array table, same layout
    std::int32_t dyn_int[50][10];
    std::byte pad_32d0[0x32e0 - 0x32d0];
    // @data 140b48e60  dynamic-array descriptor, 20 words: [0] NI, [1] max int size,
    // [2] int table record, [3] int table words, [10] NR, [11] max real size, [12] real table
    // record, [13] real table words
    std::int32_t dyn_hdr[20];
    // @data 140b48eb0  MAR region selector of op 3 (0 all, -1 none, else a region start)
    std::int32_t sel_mar;
    // @data 140b48eb4  A region selector of op 3
    std::int32_t sel_a;
    // @data 140b48eb8  set by licence code; only the omitted licence branches read it
    std::int32_t u3338;
    std::byte pad_333c[0x3570 - 0x333c];
    // @data 140b490f0  family of the step being read or written (2, 3), from the entry for
    // family 23 files
    std::int32_t step_family;
    // @data 140b490f4  version of that step (e.g. 11000)
    std::int32_t step_version;
    // @data 140b490f8  family of the open file from its signature (2, 3, 23; 0 unknown)
    std::int32_t file_family;
    // @data 140b490fc  version number of the open file (FUN_1403b32b5)
    std::int32_t file_version;
    std::byte pad_3580[0xb220 - 0x3580];
    // @data 140b50da0  index records, 128 words each: words 1..127 chunk records, word 128 the
    // next index record
    std::int32_t index[200][128];
    // @data 140b69da0  the directory chunk in memory: 128 entries of 13 words
    std::int32_t chunk[0x680];
    // @data 140b6b7a0  number of the chunk in `chunk` (0 none)
    std::int32_t cur_chunk;
    std::byte pad_25c24[0x26e60 - 0x25c24];
    // @data 140b6c9e0  directory words of the step to write (set by the callers of op 4):
    // entry w1 (mesh number) and w2 (step number)
    std::int32_t put_mesh_no;
    std::int32_t put_step;
    std::int32_t u26e68;
    // @data 140b6c9ec  added to the step number for the process-monitor message (not ported)
    std::int32_t u26e6c;
    // @data 140b6c9f0  step interval: op 4 keeps a step whose (step-1) % interval != 0 only
    // until the next one is appended
    std::int32_t put_interval;
    std::byte pad_26e74[0x26e80 - 0x26e74];
    // @data 140b6ca00  entry w6 (simulation number) and w7 (simulation mode) of the step to write
    std::int32_t put_sim_no;
    std::int32_t put_sim_mode;
    std::int32_t u26e88;
    std::byte pad_26e8c[0x26f80 - 0x26e8c];
    // @data 140b6cb00  record 2, 34 words: TITLE (80 chars), SMI1..SMI4 (8 chars each), then
    // ints 29..34: ISTART, ISMTYP, NRSETS, NXTREC, 2 more
    union {
        std::int32_t rec2[34];
        struct {
            char title[80];
            char smi1[8]; // @data 140b6cb50  version text, compared as a double
            char smi2[8];
            char smi3[8];
            char smi4[8];
            std::int32_t istart; // @data 140b6cb70
            std::int32_t ismtyp; // @data 140b6cb74
            std::int32_t nrsets; // @data 140b6cb78  directory entries in use
            std::int32_t nxtrec; // @data 140b6cb7c  next free record
            std::int32_t w33;
            std::int32_t w34;
        } h;
    };

    // Flat view in 4-byte words from 0x140b45b80, for the original's out-of-range indexing.
    std::int32_t* words() { return reinterpret_cast<std::int32_t*>(this); }
    // Word w (1-based) of directory entry e (1-based) of the chunk: DAT_140b69d68 + (w + e*13)*4.
    std::int32_t& entry(std::int32_t e, std::int32_t w)
    {
        return words()[(0x140b69d6c - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(e) * 13 + (w - 1)];
    }
    // Word i (1-based) of index record k (1-based), as the original addresses it (k may be 0 or
    // 201 and i up to 255: the address is computed, never checked).
    std::int32_t& index_word(std::int32_t k, std::int32_t i)
    {
        return words()[(0x140b50da0 - 0x140b45b80) / 4 + static_cast<std::ptrdiff_t>(k - 1) * 128 + (i - 1)];
    }
};

static_assert(sizeof(DbCommon) == 0x27008);
static_assert(offsetof(DbCommon, mar_regions) == 0x140b46380 - 0x140b45b80);
static_assert(offsetof(DbCommon, dyn_real) == 0x140b46740 - 0x140b45b80);
static_assert(offsetof(DbCommon, dyn_int) == 0x140b48680 - 0x140b45b80);
static_assert(offsetof(DbCommon, dyn_hdr) == 0x140b48e60 - 0x140b45b80);
static_assert(offsetof(DbCommon, sel_mar) == 0x140b48eb0 - 0x140b45b80);
static_assert(offsetof(DbCommon, step_family) == 0x140b490f0 - 0x140b45b80);
static_assert(offsetof(DbCommon, file_version) == 0x140b490fc - 0x140b45b80);
static_assert(offsetof(DbCommon, index) == 0x140b50da0 - 0x140b45b80);
static_assert(offsetof(DbCommon, chunk) == 0x140b69da0 - 0x140b45b80);
static_assert(offsetof(DbCommon, cur_chunk) == 0x140b6b7a0 - 0x140b45b80);
static_assert(offsetof(DbCommon, put_mesh_no) == 0x140b6c9e0 - 0x140b45b80);
static_assert(offsetof(DbCommon, put_interval) == 0x140b6c9f0 - 0x140b45b80);
static_assert(offsetof(DbCommon, put_sim_no) == 0x140b6ca00 - 0x140b45b80);
static_assert(offsetof(DbCommon, rec2) == 0x140b6cb00 - 0x140b45b80);

inline DbCommon g_db{};

// @data 1406277a0  byte-order mode of the DB words (FUN_140407e60 / FUN_140407e75); op 0 sets 2
inline std::int32_t g_swap_mode = 0;

// RMALLOC / RFREE bookkeeping (FUN_14043c190, FUN_14043c520).
// @data 14070dc90  RFREE calls
inline std::int32_t g_rm_frees = 0;
// @data 14070dc94  words allocated now
inline std::int32_t g_rm_total = 0;
// @data 14070dc98  trace flag: print every RMALLOC / RFREE to stderr
inline std::int32_t g_rm_trace = 0;
// @data 14070dc9c  high-water mark of g_rm_total
inline std::int32_t g_rm_peak = 0;
// @data 14070dca4  RMALLOC calls
inline std::int32_t g_rm_allocs = 0;
// @data 140636ae0  bytes per RMALLOC word in the error message (4)
inline constexpr std::int32_t kRmWordBytes = 4;

// @data 140613588  words RMALLOCed for the dynamic arrays of the step in memory (FUN_1403bc85b)
inline std::int32_t g_dyn_words = 0;



// Resets everything above to its load-time value (tests).
inline void reset_globals()
{
    std::memset(&g_db, 0, sizeof g_db);
    g_swap_mode = 0;
    g_rm_frees = g_rm_total = g_rm_trace = g_rm_peak = g_rm_allocs = 0;
    g_dyn_words = 0;
}

} // namespace def::dbio::dbcore
