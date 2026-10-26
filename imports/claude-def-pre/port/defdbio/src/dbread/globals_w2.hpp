// dbread/globals_w2.hpp: static storage of the old-format (version < 2301, "DB230") DB operations
// 1403bef07 / 1403c0e60 and of 1403dc957 / 140444458 (packet p-dbread, helper W2).
//
// The Fortran code keeps its directory index, the loaded directory record and even its loop
// counters in fixed storage; they are kept here at their addresses so that the port reads like
// the decompile. Record 2 (140b6cb00..) and the step-to-write words (140b6c9e0..) are p1-12's
// dbcore::g_db.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <defdbio/dbcore/globals_p1_12.hpp>

namespace def::dbio::dbread {

// @data 140613590  old-format index record (128 words: word i = first record of directory
// record i) followed by the loaded directory record (128 entries of 8 words) and its number.
// The original addresses the index as DAT_14061358c + i*4 (i 1-based, never checked) and the
// entries as DAT_140613770 + e*0x20 (e 1-based).
struct OldDbIndex {
    // 140613590 (the struct's first member)  index record (record 3 of the file)
    std::int32_t index[128];
    // @data 140613790  directory record in memory (8 file records, 0x400 words)
    std::int32_t chunk[0x400];
    // @data 140614790  number of the directory record in `chunk` (0 none)
    std::int32_t cur_rec;

    std::int32_t* words() { return reinterpret_cast<std::int32_t*>(this); }
    // DAT_14061358c + i*4 (i 1-based; i > 128 runs into `chunk` as in the original)
    std::int32_t& index_word(std::int32_t i) { return words()[static_cast<std::ptrdiff_t>(i) - 1]; }
    // word w (1-based) of entry e (1-based): DAT_14061376c + (w + e*8)*4
    std::int32_t& entry(std::int32_t e, std::int32_t w)
    {
        return words()[128 + static_cast<std::ptrdiff_t>(e - 1) * 8 + (w - 1)];
    }
};
static_assert(offsetof(OldDbIndex, chunk) == 0x140613790 - 0x140613590);
static_assert(offsetof(OldDbIndex, cur_rec) == 0x140614790 - 0x140613590);
inline OldDbIndex g_old_db{};

// @data 14070783c  statics of 1403bef07 (temporaries and loop counters the compiler put in
// fixed storage; each is set before use within one call)
struct OldDbScratch {
    std::int32_t u83c;      // 14070783c (first member)  =0 on entry, not read
    std::int32_t rec;       // @data 140707840  record number for the block I/O
    std::int32_t swap;      // @data 140707844  platform byte-order mode
    std::int32_t keep;      // @data 140707848  op 4: keep the last entry
    std::int32_t appended;  // @data 14070784c  op 4: the step was appended
    std::int32_t drec;      // @data 140707850  directory record number
    std::int32_t pos;       // @data 140707854  entry position in it
    std::int32_t pos2;      // @data 140707858  second position output
    std::int32_t u85c;      // @data 14070785c
    char smi[8];            // @data 140707860  copy of SMI1
    std::int32_t u868[2];   // @data 140707868
    std::int32_t n;         // @data 140707870  word count (RLE)
    std::int32_t first;     // @data 140707874  first slot of a directory record
    std::int32_t count;     // @data 140707878  entries in it
    std::int32_t last;      // @data 14070787c  last slot covered so far
    std::int32_t out;       // @data 140707880  op 1 output index
    std::int32_t i;         // @data 140707884  op 1 entry loop
    std::int32_t j;         // @data 140707888  op 1 word loop
    std::int32_t cmp_len;   // @data 14070788c  =8, ftn_strncmp length
    std::int32_t cmp_res;   // @data 140707890  ftn_strncmp result
    std::int32_t u894;      // @data 140707894
    std::int32_t u898;      // @data 140707898
    std::int32_t chunk_rec; // @data 14070789c  file record of the directory record (1403c0e60)
};
static_assert(sizeof(OldDbScratch) == 0x14070789c - 0x14070783c + 4);
inline OldDbScratch g_old_tmp{};

// @data 140708030  unit (3), step number (never written: 0) and op of 1403dc957
struct ReadARegionStatics {
    std::int32_t unit; // 140708030 (first member)
    std::int32_t step; // @data 140708034
    std::int32_t op;   // @data 140708038
};
inline ReadARegionStatics g_rdarya{};

// @data 140617360  1: the file stores run-length coded arrays (SMI1 >= " 1.0    ")
inline std::int32_t g_old_compressed = 0;
// @data 14070dd00  loop index of 140444458
inline std::int32_t g_copy_index = 0;

// @data 140617350  version text compared with SMI1
inline constexpr char kSmi201[9] = " 2.0.1  ";
// @data 140617370  first version with run-length coding
inline constexpr char kSmi10[9] = " 1.0    ";
// @data 140617380  record 1 signature written by op 4 (5 CHARACTER*4 items)
inline constexpr char kSig3d230[21] = "SFTC-DEFORM-3D-DB230";

// DAT_140b6ca20: CHARACTER*200 name of the DB file opened last by 1403dc957. It lies in the gap
// of dbcore::g_db after the step-to-write words (no other routine of the port uses it).
// 140b6ca20 (in the datarm23 COMMON, see dbread/w3.hpp)
inline char* last_db_file()
{
    return reinterpret_cast<char*>(&dbcore::g_db) + (0x140b6ca20 - 0x140b45b80);
}

inline void reset_globals_w2()
{
    std::memset(&g_old_db, 0, sizeof g_old_db);
    std::memset(&g_old_tmp, 0, sizeof g_old_tmp);
    g_rdarya = {};
    g_old_compressed = 0;
    g_copy_index = 0;
    std::memset(last_db_file(), 0, 200);
}

} // namespace def::dbio::dbread
